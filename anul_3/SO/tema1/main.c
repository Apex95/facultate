#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "main.h"
#include "hash.h"

/* Sporici Dan - 335CB */

#define MAX_CMD_LENGTH 20000
char *crtCmd;

void extendBucket(THashTable hashTable, int bucketId)
{
	/* capacitate == 0 => n-am la ce sa fac realloc */
	if (hashTable.bucketsCapacity[bucketId] == 0)
		hashTable.table[bucketId] = malloc(sizeof(char *) *
			(hashTable.bucketsCapacity[bucketId] * 2 + 1));
	else
		hashTable.table[bucketId] = realloc(hashTable.table[bucketId],
		sizeof(char *) * (hashTable.bucketsCapacity[bucketId] * 2 + 1));
	hashTable.bucketsCapacity[bucketId] =
				hashTable.bucketsCapacity[bucketId] * 2 + 1;
}

void addWord(THashTable hashTable, char *word)
{
	unsigned int bucketId = hash(word, hashTable.nOfBuckets);
	int i = 0;

	/* verific sa nu fie deja in bucket */
	for (i = 0; i < hashTable.bucketsElements[bucketId]; i++)
		if (strcmp(word, hashTable.table[bucketId][i]) == 0)
			return;

	/* verific daca am destul loc in bucket */
	if (hashTable.bucketsCapacity[bucketId] -
				hashTable.bucketsElements[bucketId] == 0)
		extendBucket(hashTable, bucketId);

	/* aloc spatiu pt cuvant */
	hashTable.table[bucketId][hashTable.bucketsElements[bucketId]] =
				malloc(sizeof(char) * (strlen(word) + 1));
	strcpy(hashTable.table[bucketId][hashTable.bucketsElements[bucketId]],
									word);
	hashTable.bucketsElements[bucketId]++;
}

void allocateHashTable(THashTable *hashTable, int nOfBuckets)
{
	/* aloc spatiu pt campurile din THashTable */

	(*hashTable).table = malloc(sizeof(char **) * nOfBuckets);
	memset((*hashTable).table, 0, sizeof(char **) * nOfBuckets);

	(*hashTable).bucketsCapacity = malloc(sizeof(int) * nOfBuckets);
	memset((*hashTable).bucketsCapacity, 0, nOfBuckets * sizeof(int));

	(*hashTable).bucketsElements = malloc(sizeof(int) * nOfBuckets);
	memset((*hashTable).bucketsElements, 0, nOfBuckets * sizeof(int));

	(*hashTable).nOfBuckets = nOfBuckets;
}

void findWord(THashTable hashTable, char *word, FILE *stream)
{
	unsigned int bucketId = hash(word, hashTable.nOfBuckets);
	int i = 0;

	/* cautare prin bucket */
	for (i = 0; i < hashTable.bucketsElements[bucketId]; i++)
		if (strcmp(word, hashTable.table[bucketId][i]) == 0) {
			fprintf(stream, "True\n");
			return;
		}

	fprintf(stream, "False\n");
}

void printBucket(THashTable hashTable, int bucketId, FILE *stream)
{
	int i = 0;

	for (i = 0; i < hashTable.bucketsElements[bucketId]; i++)
		fprintf(stream, "%s ", hashTable.table[bucketId][i]);

	if (hashTable.bucketsElements[bucketId] > 0)
		fprintf(stream, "\n");
}

void printAll(THashTable hashTable, FILE *stream)
{
	int i = 0;

	for (i = 0; i < hashTable.nOfBuckets; i++)
		printBucket(hashTable, i, stream);
}

void removeWord(THashTable hashTable, char *word)
{
	unsigned int bucketId = hash(word, hashTable.nOfBuckets);
	int i = 0, pos = -1;

	/* cautare liniara in bucket */
	for (i = 0; i < hashTable.bucketsElements[bucketId]; i++)
		if (strcmp(word, hashTable.table[bucketId][i]) == 0) {
			pos = i;
			break;
		}

	/* nu exista cuvantul */
	if (pos == -1)
		return;

	/* exista->il sterg shiftand la stanga toate cuvintele ce urmau dupa */
	for (i = pos; i < hashTable.bucketsElements[bucketId] - 1; i++)
		hashTable.table[bucketId][i] = hashTable.table[bucketId][i+1];


	hashTable.bucketsElements[bucketId]--;

}


void parseInput(THashTable *hashTable, FILE *inputStream)
{
	char *token = NULL;
	int tokenIndex = 0;

	char *cmd = NULL, *parameter1 = NULL, *parameter2 = NULL;

	/* citire linie cu linie */
	while (fgets(crtCmd, MAX_CMD_LENGTH, inputStream) != NULL) {

		if (crtCmd[strlen(crtCmd)-2] == '\r'
			|| crtCmd[strlen(crtCmd)-2] == '\n')
			crtCmd[strlen(crtCmd)-2] = 0;

		if (crtCmd[strlen(crtCmd)-1] == '\n')
			crtCmd[strlen(crtCmd)-1] = 0;




		/* reset - pt urmatoarea comanda */
		tokenIndex = 0;
		cmd = NULL;
		parameter1 = NULL;
		parameter2 = NULL;


		token = strtok(crtCmd, " ");


		while (token != NULL) {
			/* primul token din comanda e chiar numele comenzii */
			if (tokenIndex == 0) {
				cmd = malloc(sizeof(char) * (strlen(token)+1));
				strcpy(cmd, token);
			}


			/* primul argument */
			if (tokenIndex == 1) {
				parameter1 = malloc(sizeof(char) *
						(strlen(token)+1));
				strcpy(parameter1, token);
			}

			if (tokenIndex == 2) {
				parameter2 = malloc(sizeof(char) *
						(strlen(token)+1));
				strcpy(parameter2, token);
			}

			token = strtok(NULL, " ");

			tokenIndex++;
		}

		/* well...linie goala */
		if (cmd == NULL)
			continue;

		/* aici am comanda parsata - verific ce functie se cere */
		if (strcmp(cmd, "add") == 0) {
			if (parameter1 == NULL) {
				freeAll(hashTable);
				free(cmd);
				free(parameter1);
				free(parameter2);
				DIE(1 == 1, "Missing parameter #1 for add");
			}

			addWord(*hashTable, parameter1);
		} else
		if (strcmp(cmd, "remove") == 0) {
			if (parameter1 == NULL) {
				freeAll(hashTable);
				free(cmd);
				free(parameter1);
				free(parameter2);
				DIE(1 == 1, "Missing parameter #1 for remove");
			}
			removeWord(*hashTable, parameter1);
		} else
		if (strcmp(cmd, "find") == 0) {
			if (parameter2 == NULL)
				findWord(*hashTable, parameter1, stdout);
			else {
				FILE *outputStream = fopen(parameter2, "a+");

				findWord(*hashTable, parameter1, outputStream);
				fclose(outputStream);
			}
		} else
		if (strcmp(crtCmd, "print_bucket") == 0) {
			int bucketId = -1;
			int sscanfRes = -1;

			sscanfRes = sscanf(parameter1, "%d", &bucketId);

			if (sscanfRes !=1) {
				freeAll(hashTable);
				free(cmd);
				free(parameter1);
				free(parameter2);
				DIE(sscanfRes != 1, "Parameter #1 is not int");
			}

			if (bucketId >= (*hashTable).nOfBuckets) {
				freeAll(hashTable);
				free(cmd);
				free(parameter1);
				free(parameter2);
				DIE(bucketId >= (*hashTable).nOfBuckets,
				"Parameter #2 exceeds number of buckets");
			}

			if (parameter2 == NULL)
				printBucket(*hashTable, bucketId, stdout);
			else {
				FILE *outputStream = fopen(parameter2, "a+");

				printBucket(*hashTable, bucketId,
							outputStream);
				fclose(outputStream);
			}
		} else
		if (strcmp(crtCmd, "print") == 0) {
			if (parameter1 == NULL)
				printAll(*hashTable, stdout);
			else {
				FILE *outputStream = fopen(parameter1, "a+");

				printAll(*hashTable, outputStream);
				fclose(outputStream);
			}
		} else
		if (strcmp(crtCmd, "clear") == 0)
			clearAll(*hashTable);
		else
		if (strcmp(crtCmd, "resize") == 0)
			if (strcmp(parameter1, "double") == 0)
				resize(hashTable, 1);
			else
			if (strcmp(parameter1, "halve") == 0)
				resize(hashTable, 2);
			else {
				freeAll(hashTable);
				free(cmd);
				free(parameter1);
				free(parameter2);
				DIE(1 == 1, "Invalid resize parameter.");
			}
		else {
			freeAll(hashTable);
			free(cmd);
			free(parameter1);
			free(parameter2);
			DIE(1 == 1, "Invalid command");
		}

		free(cmd);
		free(parameter1);
		free(parameter2);
	}
}

void clearAll(THashTable hashTable)
{
	int i = 0, j = 0;

	/* free all the buckets ! */
	for (i = 0; i < hashTable.nOfBuckets; i++) {
		for (j = 0; j < hashTable.bucketsElements[i]; j++) {
			free(hashTable.table[i][j]);
			hashTable.table[i][j] = NULL;
		}

		hashTable.bucketsElements[i] = 0;
		hashTable.bucketsCapacity[i] = 0;

		free(hashTable.table[i]);
		hashTable.table[i] = NULL;
	}
}

void resize(THashTable *hashTable, int mode)
{
	int i = 0, j = 0;

	/* tabelul nou */
	THashTable *hashTable2 = malloc(sizeof(THashTable));

	/* double */
	if (mode == 1)
		allocateHashTable(hashTable2, hashTable->nOfBuckets * 2);
	else
	/* halve */
	if (mode == 2)
		allocateHashTable(hashTable2, hashTable->nOfBuckets / 2);


	/* copiez datele din hashTable in hashTable2 */
	for (i = 0; i < hashTable->nOfBuckets; i++)
		for (j = 0; j < hashTable->bucketsElements[i]; j++)
			addWord(*hashTable2, hashTable->table[i][j]);

	clearAll(*hashTable);
	free(hashTable->table);
	free(hashTable->bucketsElements);
	free(hashTable->bucketsCapacity);

	/* copiez peste vechiul hashtable */
	memcpy(hashTable, hashTable2, sizeof(THashTable));
}

void freeAll(THashTable *hashTable)
{
	clearAll(*hashTable);

	free(hashTable->table);
	free(hashTable->bucketsElements);
	free(hashTable->bucketsCapacity);
	free(hashTable);

	free(crtCmd);
}

int main(int argc, char *argv[])
{
	int initialSize = -1; int i = 0;
	THashTable *hashTable = malloc(sizeof(THashTable));
	int sscanfRes = -1;

	FILE *inputStream = stdin;

	if (argc < 2) {
		free(hashTable);
		DIE(argc < 2, "Missing initial size parameter.");
	}

	sscanfRes = sscanf(argv[1], "%d", &initialSize);

	if (initialSize < 1 || sscanfRes != 1) {
		free(hashTable);
		DIE(initialSize < 1 || sscanfRes != 1, "Bad size.");
	}
	crtCmd = malloc(sizeof(char) * (MAX_CMD_LENGTH + 1));
	memset(crtCmd, 0, sizeof(char) * (MAX_CMD_LENGTH + 1));

	allocateHashTable(hashTable, initialSize);


	if (argc > 2) {
		for (i = 2; i < argc; i++) {
			inputStream = fopen(argv[i], "r");
			parseInput(hashTable, inputStream);
			fclose(inputStream);
		}
	} else
		parseInput(hashTable, inputStream);

	freeAll(hashTable);

	return EXIT_SUCCESS;
}
