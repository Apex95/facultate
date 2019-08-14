/**
 * Operating Systems 2013-2017 - Assignment 2
 *
 * Sporici Dan / 335CB
 *
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

#include <fcntl.h>
#include <unistd.h>

#include "cmd.h"
#include "utils.h"

#include <stdio.h>

#include <stdlib.h>
#include <string.h>

#define READ		0
#define WRITE		1

#define CHECK_ARGS_ALLOC() {\
		if (args[i+1] == NULL) { \
			freeArgs(args, i+1); \
			DIE(1, "calloc failed"); \
		} \
		}

/**
 * Internal change-directory command.
 */
static bool shell_cd(word_t *dir)
{
	DIE(dir == NULL, "no args");

	int res = chdir(dir->string);

	return res;
}

/**
 * Internal exit/quit command.
 */
static int shell_exit(void)
{
	return SHELL_EXIT;
}


/**
 * Constructs the path to a file using word parts
 */
char *constructPath(word_t *wordPart)
{
	char *filePath = NULL;

	const char *value = NULL;

	while (wordPart != NULL) {
		value = wordPart->string;

		if (wordPart->expand)
			value = getenv(value);

		if (filePath == NULL) {
			if (value == NULL) {
				filePath = calloc(1, 2);
				DIE(filePath == NULL, "calloc failed");
				strcpy(filePath, "");
			} else {
				filePath = calloc(1, strlen(value) + 1);
				DIE(filePath == NULL, "calloc failed");
				strcpy(filePath, value);
			}
		} else {
			if (value != NULL) {
				filePath = realloc(filePath, strlen(filePath)
							+ strlen(value) + 1);
				DIE(filePath == NULL, "realloc failed");
				filePath = strcat(filePath, value);
			}
		}

		wordPart = wordPart->next_part;
	}

	return filePath;
}

/**
 * Frees the array of strings containing the args for execv
 */
void freeArgs(char *args[], int maxIndex)
{
	int i = 0;

	for (i = 0; i < maxIndex; i++)
		free(args[i]);

}


/**
 * Parse a simple command (internal, environment variable assignment,
 * external command).
 */
static int parse_simple(simple_command_t *s, int level, command_t *father)
{
	if (s == NULL || s->verb == NULL || s->verb->string == NULL)
		return 0;


	/* -- CD */
	if (strcmp(s->verb->string, "cd") == 0) {
		if (s->out != NULL) {
			int fd = open(s->out->string, O_WRONLY | O_CREAT |
							O_TRUNC, 0644);
			close(fd);
		}

		return shell_cd(s->params);
	}


	/* -- EXIT/QUIT */
	if (strcmp(s->verb->string, "exit") == 0 ||
		strcmp(s->verb->string, "quit") == 0)
		return shell_exit();


	/* -- Assignments */
	if (s->verb->next_part != NULL &&
		strcmp(s->verb->next_part->string, "=") == 0 &&
		s->verb->next_part->next_part != NULL &&
		s->verb->next_part->next_part->string != NULL) {

		setenv(s->verb->string,
			s->verb->next_part->next_part->string, 1);
		return 0;
	}



	pid_t childId;
	int status;

	childId = fork();

	if (childId == 0) {
		/* redirects */
		int fdin = -1, fdout = -1, fderr = -1, res = 0;

		if (s->in != NULL) {
			char *filePath = constructPath(s->in);

			fdin = open(filePath, O_RDONLY, 0644);
			DIE(fdin < 0, "open failed");

			res = dup2(fdin, STDIN_FILENO);
			DIE(res < 0, "dup2 failed");

			close(fdin);
			free(filePath);
		}


		if (s->out != NULL) {
			char *filePath = constructPath(s->out);


			if ((s->io_flags & IO_OUT_APPEND) != 0) {
				fdout = open(filePath, O_WRONLY | O_CREAT,
									 0644);
				if (fdout < 0) {
					free(filePath);
					DIE(1, "open failed");
				}

				lseek(fdout, 0, SEEK_END);
			} else {
				fdout = open(filePath, O_WRONLY |
						 O_CREAT | O_TRUNC, 0644);
				if (fdout < 0) {
					free(filePath);
					DIE(1, "open failed");
				}
			}

			res = dup2(fdout, STDOUT_FILENO);
			if (res < 0) {
				free(filePath);
				DIE(1, "dup2 failed");
			}

			if (s->err != NULL &&
				strcmp(s->err->string, s->out->string) == 0) {
				fderr = fdout;
				res = dup2(fderr, STDERR_FILENO);

				if (res < 0) {
					free(filePath);
					DIE(1, "dup2 failed");
				}
			}
			close(fdout);
			free(filePath);
		}


		if (s->err != NULL && fderr == -1) {
			char *filePath = constructPath(s->err);


			if ((s->io_flags & IO_ERR_APPEND) != 0) {
				if (fderr == -1) {
					fderr = open(filePath, O_WRONLY |
								O_CREAT, 0644);
					if (fderr < 0) {
						free(filePath);
						DIE(1, "open failed");
					}
				}
				lseek(fderr, 0, SEEK_END);
			} else
				if (fderr == -1) {
					fderr = open(filePath, O_WRONLY |
						O_CREAT | O_TRUNC, 0644);

					if (fderr < 0) {
						free(filePath);
						DIE(1, "open failed");
					}
				}

			res = dup2(fderr, STDERR_FILENO);
			if (res < 0) {
				free(filePath);
				DIE(1, "dup2 failed");
			}

			close(fderr);
			free(filePath);
		}

		int nOfArgs = 0, i = 0;
		word_t *crtWord = s->params;

		while (crtWord != NULL) {
			nOfArgs++;
			crtWord = crtWord->next_word;
		}


		char *args[nOfArgs + 2];

		memset(args, 0, sizeof(args));

		args[0] = calloc(1, strlen(s->verb->string) + 1);
		DIE(args[0] == NULL, "calloc failed");


		strcpy(args[0], s->verb->string);
		args[nOfArgs + 1] = NULL;

		crtWord = s->params;

		word_t *wordPart = NULL;
		const char *value = NULL;

		/* creates the array of args */
		for (i = 0; i < nOfArgs; i++) {
			args[i+1] = NULL;

			wordPart = crtWord;


			while (wordPart != NULL) {
				value = wordPart->string;

				if (wordPart->expand)
					value = getenv(value);

				if (value == NULL) {
					if (args[i+1] == NULL) {
						args[i+1] = calloc(1, 2);

						CHECK_ARGS_ALLOC();

						strcpy(args[i+1], "");
					}
				} else {
					if (args[i+1] == NULL) {
						args[i+1] = calloc(1,
							strlen(value) + 1);

						CHECK_ARGS_ALLOC();

						strcpy(args[i+1], value);
					} else {
						args[i+1] = realloc(args[i+1],
							strlen(args[i+1]) +
							strlen(value) + 1);

						CHECK_ARGS_ALLOC();

						args[i+1] = strcat(args[i+1],
									value);
					}
				}

				wordPart = wordPart->next_part;
			}
			crtWord = crtWord->next_word;
		}


		execvp(s->verb->string, args);


		freeArgs(args, nOfArgs + 1);

		char *message = calloc(1, 24 + 1 + strlen(s->verb->string));

		DIE(message == NULL, "calloc failed");

		sprintf(message, "Execution failed for '%s'\n",
							s->verb->string);
		write(STDERR_FILENO, message, strlen(message));
		free(message);

		exit(EXIT_FAILURE);
	} else {
		DIE(childId == -1, "can't spawn child");
		waitpid(childId, &status, 0);

		return WEXITSTATUS(status);
	}


	return 0;
}

/**
 * Process two commands in parallel, by creating two children.
 */
static bool do_in_parallel(command_t *cmd1, command_t *cmd2, int level,
		command_t *father)
{
	pid_t childId1 = -1, childId2 = -1;
	int status1, status2;

	childId1 = fork();

	if (childId1 == 0) {
		int res = parse_command(cmd2, level, cmd2->up);

		exit(res);
	} else {
		DIE(childId1 == -1, "can't spawn child");

		childId2 = fork();

		if (childId2 == 0) {
			int res = parse_command(cmd1, level, cmd1->up);
			exit(res);
		} else {
			DIE(childId2 == -1, "can't spawn child");

			waitpid(childId1, &status1, 0);
			waitpid(childId2, &status2, 0);
		}

		return WEXITSTATUS(status2);
	}

	return true;
}

/**
 * Run commands by creating an anonymous pipe (cmd1 | cmd2)
 */
static bool do_on_pipe(command_t *cmd1, command_t *cmd2, int level,
		command_t *father)
{
	int fds[2], childId = -1, status, res;

	res = pipe(fds);
	DIE(res < 0, "pipe failed");

	childId = fork();


	if (childId == 0) {
		close(fds[0]);
		res = dup2(fds[1], STDOUT_FILENO);
		close(fds[1]);

		DIE(res < 0, "dup2 failed");

		res = parse_command(cmd1, level+1, father);

		exit(EXIT_SUCCESS);
	} else {
		DIE(childId == -1, "can't spawn child");

		int originalFd = dup(STDIN_FILENO);

		DIE(originalFd < 0, "dup failed");

		close(fds[1]);
		res = dup2(fds[0], STDIN_FILENO);
		close(fds[0]);

		DIE(res < 0, "dup2 failed");


		res = parse_command(cmd2, level+1, father);

		waitpid(childId, &status, 0);

		/* otherwise it reads NULL and exits */
		int res2 = dup2(originalFd, STDIN_FILENO);

		DIE(res2 < 0, "dup2 failed");
		close(originalFd);

		return res;
	}

	return true;
}

/**
 * Parse and execute a command.
 */
int parse_command(command_t *c, int level, command_t *father)
{
	if (c->op == OP_NONE)
		return parse_simple(c->scmd, level, father);

	int res1, res2 = 0;

	switch (c->op) {
	case OP_SEQUENTIAL:
		parse_command(c->cmd1, level+1, c);
		parse_command(c->cmd2, level+1, c);
		break;

	case OP_PARALLEL:
		return do_in_parallel(c->cmd1, c->cmd2, level+1, c);

	case OP_CONDITIONAL_NZERO:
		res1 = parse_command(c->cmd1, level+1, c);

		if (res1 != 0)
			res2 = parse_command(c->cmd2, level+1, c);

		return res2;

	case OP_CONDITIONAL_ZERO:
		res1 = parse_command(c->cmd1, level+1, c);

		if (res1 == 0)
			res2 = parse_command(c->cmd2, level+1, c);

		return res2;

	case OP_PIPE:
		return do_on_pipe(c->cmd1, c->cmd2, level+1, c);

	default:
		return SHELL_EXIT;
	}

	return 0;
}
