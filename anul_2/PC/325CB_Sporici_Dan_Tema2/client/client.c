#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/stat.h>

#include "message.h"
#include "db.h"




void die(char *msg)
{
    printf("%s\n", msg);
    exit(0);
}

void displayPrompt(char * logFile)
{
    // - actualizez promptul
    char * currentUser = getCurrentUser();
    
    FILE * g = fopen(logFile, "a");

    if (currentUser == NULL)
    {
        printf("$ ");
        fprintf(g, "$ ");
    }
    else
    {
        printf("%s> ", currentUser);
        fprintf(g, "$ ");
    }
    
    fflush(stdout); 
    fclose(g);
}


int main(int argc, char *argv[])
{
    int serverSock, n;
    struct sockaddr_in siServer;

    char buffer[BUFFER_SIZE];

    if (argc < 3)
       die("More params.");
    
	serverSock = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSock < 0) 
        die("Can't open sock.");
    
    siServer.sin_family = AF_INET;
    siServer.sin_port = htons(atoi(argv[2]));
    inet_aton(argv[1], &siServer.sin_addr);
    
    
    if (connect(serverSock,(struct sockaddr*) &siServer,sizeof(siServer)) < 0) 
        die("Can't connect.");    

    fd_set readFds;
    fd_set tmpFds;
    int fdmax = 0;

    FD_SET(serverSock, &readFds);
    fdmax = serverSock;
    
    FD_SET(0, &readFds);

    struct timeval timer;

    timer.tv_sec = 2;
    timer.tv_usec = 0;
    
    char logFile[100] = {0};
    sprintf(logFile, "client-%ld.log", (long)getpid());
    
    displayPrompt(logFile);

    MessageType lastMessageType = -1;
    char showPrompt = 0;
    

    FILE * g = NULL;
    
    while(1)
    {
        tmpFds = readFds;

        if (select(fdmax + 1, &tmpFds, NULL, NULL, &timer) == -1)
            die("Can't select.");

        if (FD_ISSET(serverSock, &tmpFds))
        {
            memset(buffer, 0, BUFFER_SIZE);
            n = recv(serverSock, buffer, sizeof(buffer), 0);
  
            if (n <= 0)
            {
                close(serverSock);
                FD_CLR(serverSock, &readFds);
                break;
            }

            // aici raspunsul de la server
            if (buffer[0] != TYPE_DOWNLOAD && buffer[0] != TYPE_GETUSERLIST
                && buffer[0] != TYPE_GETFILELIST)
            {
                showPrompt = 1;
                printf("%s\n", buffer);
                
                g = fopen(logFile, "a");
                fprintf(g, "%s\n", buffer);
                fclose(g);
            }
            // --- login ---
            if (lastMessageType == TYPE_LOGIN)
            {
                // si serverul a raspuns 200
                if (strcmp(buffer, MSG_SUCCESS) == 0)
                    setLogin(); // zic ca e autentificat
            }

            // --- logout ---
            if (lastMessageType == TYPE_LOGOUT)
            {
                if (strcmp(buffer, MSG_SUCCESS) == 0)
                    setLogout();
            }
           
            // --- getuserlist ---
            while (buffer[0] == TYPE_GETUSERLIST)
            {
                UsersListMessage m;
                memset(&m, 0, sizeof(m));

                memcpy(&m, buffer, sizeof(m));

                int i = 0;
                showPrompt = 1;
                for (i = 0; i < m.nOfUsers; i++)
                {
                    printf("%s\n", m.users[i]);
                    g = fopen(logFile, "a");
                    fprintf(g, "%s\n", m.users[i]);
                    fclose(g);
                }

                if (m.lastMsg)
                    break;

                n = recv(serverSock, buffer, sizeof(buffer), 0);        
            }
            
            // --- getfilelist ---
            while (buffer[0] == TYPE_GETFILELIST)
            {
                FilesListMessage m;
                memset(&m, 0, sizeof(m));

                memcpy(&m, buffer, sizeof(m));
                
                int i = 0;

                g = fopen(logFile, "a");
                for (i = 0; i < m.nOfFiles; i++)
                {
                    printf("%s   %lu bytes   %s\n", m.files[i].name, 
                                                    m.files[i].size, 
                                                    m.files[i].access == 0 ? "PRIVATE" : "SHARED");
                    fprintf(g, "%s   %lu bytes   %s\n", m.files[i].name, 
                                                        m.files[i].size, 
                                                        m.files[i].access == 0 ? "PRIVATE" : "SHARED");

                }
                fclose(g);

                showPrompt = 1;                

                if (m.lastMsg)
                    break;
                
                n = recv(serverSock, buffer, sizeof(buffer), 0);
            }

           

            // --- download ---
            while (buffer[0] == TYPE_DOWNLOAD)
            {
                FileUploadMessage m;
                memset(&m, 0, sizeof(m));

                memcpy(&m, buffer, sizeof(m));

                char name[strlen(m.name) + 2 + sizeof(pid_t)];
                sprintf(name, "%ld_%s", (long)getpid(), m.name);

                FILE * f = fopen(name, "ab");
                fwrite(m.content, m.size, 1, f);
                fclose(f);

                if (m.lastMsg)
                {
                    struct stat _st;
                    stat(name, &_st);
                    printf("Download finished: %s - %lu bytes\n", name, _st.st_size);
                    g = fopen(logFile, "a");
                    fprintf(g, "Download finished: %s - %lu bytes\n", name, _st.st_size);
                    fclose(g);

                    showPrompt = 1;
                    break;
                }

                tmpFds = readFds;
                if (FD_ISSET(0, &tmpFds))
                    break;

                n = recv(serverSock, buffer, sizeof(buffer), 0);
            }
           
            if (showPrompt)
            {
                displayPrompt(logFile);
                showPrompt = 0;
            }

            //displayPrompt();
        }
        else 
            if (FD_ISSET(0, &tmpFds))
            {
  		        //citesc de la tastatura
    	        memset(buffer, 0 , BUFFER_SIZE);
                showPrompt = 1;
    	        fgets(buffer, BUFFER_SIZE-1, stdin);
                
                g = fopen(logFile, "a");
                fprintf(g, "%s\n", buffer);
                fclose(g);
                
                buffer[strlen(buffer)-1] = 0; // sterg '\n'
           
                int newSize = 0;
                char * tmpBuffer = createMessage(buffer, &newSize);



                // comanda nevalida 
                if (tmpBuffer == NULL)
                {
                    if (showPrompt)
                        displayPrompt(logFile);
                    continue; 
                }
                lastMessageType = tmpBuffer[0];

                //printf("Sending with size: %d\n", newSize);
                
                //trimit mesaj la server
    	        n = send(serverSock, tmpBuffer, BUFFER_SIZE, 0);
                if (n < 0) 
        	        die("Can't write to sock.");

            }
            else
            {
                // --- trimit un chunk de upload, daca exista
                int nOfBytes = 0;
                char * name = NULL;
                char isLastMsg = 0;
                char * content = getUploadChunk(&name, &nOfBytes, &isLastMsg);

                if (nOfBytes == 0)
                    continue;


                FileUploadMessage m;
                m.type = TYPE_UPLOAD;
                m.size = nOfBytes;
                m.lastMsg = isLastMsg;
                memcpy(m.name, name, strlen(name)+1);

                memcpy(m.content, content, nOfBytes);

                memcpy(buffer, &m, sizeof(m));


                send(serverSock, buffer, sizeof(buffer), 0);
               
                // altfel se umple bufferul de la receiver?
                usleep(700); 
            }

    }
    return 0;
}


