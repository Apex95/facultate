#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<sys/types.h>
#include<netdb.h>
#include<regex.h>


#include "client.h"

#define BUFFER_SIZE 4096

void die(char * msg)
{
    printf("\n[ERR] %s\n\n", msg);
    exit(1);
}


int regexMatch(char * pattern, char * text, regmatch_t groups[], int maxGroups)
{
    regex_t compiledExpr;
    memset(groups, 0, maxGroups * sizeof(regmatch_t));

    if (regcomp(&compiledExpr, pattern, REG_EXTENDED) != 0)
        die("Regex compilation failed");

    // voi considera ca url-ul e bun
    return regexec(&compiledExpr, text, maxGroups, groups, 0);
}


void sendLinks(int serverSock, char * hrefPattern, char * response, char * hostname, char * path)
{
    regmatch_t groups[4];
    int offset = 0;

    char link[300];
    char buffer[sizeof(Command)];

    Command cmd;


    int hostLen = -1;
    // cat timp pot face matching
    while (regexMatch(hrefPattern, response+offset, groups, 4) == 0)
    {
        hostLen = strlen(hostname) + strlen("http://");

        memset(link, 0, sizeof(link));
        strcat(link, "http://");
        strcat(link, hostname);
        

        char auxPath[strlen(path)+1];
        memset(auxPath, 0, sizeof(auxPath));

        memcpy(auxPath, path, sizeof(auxPath));

        int k = 0;
        int auxPathLen = strlen(auxPath);
        while (auxPath[auxPathLen - 1 - k] != '.' &&
               auxPath[auxPathLen - 1 - k] != '/' &&
               k + 1 < auxPathLen)
            k++;

        // path-ul se termina cu un nume de fisier, trebuie sters numele
        if (auxPath[auxPathLen-1-k] == '.')
        {
            k = 0;
            while (auxPath[auxPathLen - 1 - k] != '/')
            {
                auxPath[auxPathLen - 1- k] = 0;
                k++;
            }
        }

        // evit sa am de 2 ori '/'
        if (link[hostLen-1] == '/' && auxPath[0] == '/')
        {
            strcat(link + hostLen-1, auxPath);
            hostLen += strlen(auxPath)-1;
        }
        else
        {
            strcat(link + hostLen, auxPath);
            hostLen += strlen(auxPath);
        }

        // la fel si aici
        if (auxPath[strlen(auxPath)-1] == '/' && 
            response[offset + groups[2].rm_so] == '/')
            
            hostLen--;

        // adaug linkul gasit in pagina
        char newLink[100];
        memset(newLink, 0, sizeof(newLink));
         
        memcpy(newLink, response + offset + groups[2].rm_so, groups[2].rm_eo - groups[2].rm_so);

        // sar peste ".." deoarece astea pot produce o combinatie f. mare
        // de linkuri care duc la aceeasi resursa (gen web/../web)
        // ce sunt interpretate ca fiind linkuri diferite in hashtable...
        // nu exista parsare aici
        if (strstr(newLink, "..") != NULL)
        {
            offset += groups[2].rm_eo;
            continue;
        }
      
        int linkOffset = 0; 
        if (newLink[0] == '/')
        {
            linkOffset = 1;

            while (newLink[linkOffset] != '/' && linkOffset < strlen(newLink))
                linkOffset++;
        }

        memcpy(link+hostLen, newLink+linkOffset, strlen(newLink)-linkOffset); 
        

       
       
        memset(&cmd, 0, sizeof(cmd));

        cmd.cmdCode = CMD_ADDLINK;
        memcpy(cmd.url, link, strlen(link)+1);

        memset(buffer, 0, sizeof(cmd));
        memcpy(buffer, &cmd, sizeof(cmd));

        printf("%s | %s | %s\n", hostname, auxPath, cmd.url);

        // trimit url-ul catre server
        send(serverSock, buffer, sizeof(cmd), 0); 

        offset += groups[2].rm_eo;
    }

    printf("Done! Waiting for more links.\n");
    buffer[0] = CMD_AVAILABLE;
    send(serverSock, buffer, 1, 0); 
}


int main(int argc, char *argv[])
{
    int crtArg = 0;

    char * ipAddr = NULL;
    char * port = NULL;

    for (crtArg = 1; crtArg < argc; crtArg++)
    {
           if (strcmp(argv[crtArg], "-a") == 0)
               ipAddr = argv[crtArg+1];
               
           if (strcmp(argv[crtArg], "-p") == 0)
               port = argv[crtArg+1]; 
    }

    if (port == NULL || ipAddr == NULL)
        die("Check params.");


    // -- conexiunea la server
    int serverSock;
    struct sockaddr_in serverAddr;
    char buffer[BUFFER_SIZE];
    int recvSize = 0;


    // timer
    struct timeval timer;
    timer.tv_sec = 3;
    timer.tv_usec = 0;

    serverSock = socket(PF_INET, SOCK_STREAM, 0);
    if (serverSock < 0)
        die("Can't open socket.");
        
    serverAddr.sin_family = AF_INET;
    inet_aton(ipAddr, &serverAddr.sin_addr);
    serverAddr.sin_port = htons(atoi(port));
    
    if (connect(serverSock, (struct sockaddr*) &serverAddr, sizeof(serverAddr)) < 0)
        die("Can't connect");
        
    // -- conexiunea la website
    int siteSock; 
    
    struct sockaddr_in siteAddr; 

    siteAddr.sin_family = AF_INET;
    siteAddr.sin_port = htons(80);

    
    // -- setez descriptorii
    fd_set readFds; FD_ZERO(&readFds);
    fd_set tmpFds; FD_ZERO(&tmpFds);

    FD_SET(serverSock, &readFds);
    FD_SET(siteSock, &readFds);
    int maxFd = serverSock;
    

    struct hostent * he = NULL;
    Command cmd;
    cmd.cmdCode = -1;


    // -- parsare / regex
    char * urlPattern = "(http|https):\\/\\/([a-zA-Z0-9.-]+\\.[A-Za-z0-9]{2,6})(\\/.+){0,1}";
    char * hrefPattern = "<a\\s*[^>]+\\s*href=(\\\"|\')([a-zA-Z0-9_~.\\/-]+)(\\\"|\')";

    char hostname[100];
    char path[200];


    int i = 0;

    // -- buffer pt response
    char * response = NULL;
    int responseSize = 0;
    int crtResponseSize = 0;
    
    // -- loop
    while (1)
    {
        tmpFds = readFds;

        if (select(maxFd + 1, &tmpFds, NULL, NULL, &timer) == -1)
            die("Can't select.");

        for (i = 0; i <= maxFd; i++)
        {
            if (FD_ISSET(i, &tmpFds))
            {
                // daca primesc mesaj de la server (comanda)
                if (i == serverSock)
                {
                    memset(buffer, 0, BUFFER_SIZE);

                    recvSize = recv(serverSock, buffer, sizeof(Command), 0);

                    memcpy(&cmd, buffer, sizeof(cmd));

                    printf("CMD: %d | URL: %s \n", cmd.cmdCode, cmd.url);


                    if (cmd.cmdCode == CMD_DOWNLOAD)
                    {

                        response = NULL;
                        responseSize = 0;
                        crtResponseSize = 0;
                        // -- parsare pt host si path 
                        regmatch_t groups[4]; // 2 grupuri- host si path catre fisier
                        regexMatch(urlPattern, cmd.url, groups, 4);  

                        // sterg valorile anterioare
                        memset(hostname, 0, sizeof(hostname));
                        memset(path, 0, sizeof(path));
            
                        memcpy(hostname, cmd.url + groups[2].rm_so, groups[2].rm_eo - groups[2].rm_so); 

                        if (groups[3].rm_so == groups[3].rm_eo)
                            strcat(path, "/");
                        else
                            memcpy(path, cmd.url + groups[3].rm_so, groups[3].rm_eo - groups[3].rm_so);


                        he = gethostbyname(hostname);
                        if (he == NULL)
                            die("Can't get host information.");     

                        // setez ip-ul siteului
                        memcpy(&siteAddr.sin_addr, he->h_addr_list[0], he->h_length);
          
                        // deschid sock pt site
                        siteSock = socket(PF_INET, SOCK_STREAM, 0);
                        if (siteSock < 0)
                            die("Can't open socket.");

             
                        if (connect(siteSock, (struct sockaddr *) &siteAddr, sizeof(siteAddr)) < 0)
                            die("Can't connect to webhost.");

                        // construiesc o cerere
                        char request[600];
                        sprintf(request, "GET %s HTTP/1.0\nHOST: %s\n\n", path, hostname);
          
                        FD_SET(siteSock, &readFds);
                        if (siteSock > maxFd)
                            maxFd = siteSock;
            
                        send(siteSock, request, strlen(request), 0);
                    }
                }
                else
                {
                    memset(buffer, 0, sizeof(buffer));

                    DataMsg msg;
                    memset(&msg, 0, sizeof(msg));

                    recvSize = recv(i, msg.data, sizeof(msg.data)-1, 0);
                    
                    memcpy(msg.path, path, strlen(path)+1);
                    memcpy(msg.hostname, hostname, strlen(hostname)+1);
                    
                    msg.dataSize = recvSize;
                    msg.cmdCode = CMD_STOREDATA;
                  
                    memcpy(buffer, &msg, sizeof(msg)); 

                    // trimit inapoi catre server raspunsul primit de la site
                    send(serverSock, buffer, sizeof(msg), 0);


                    // pastrez toata sursa paginii intr-un buffer mai mare
                    // ca sa pot cauta linkuri (altfel risc sa am o parte
                    // intr-un chunk si cealalta parte in urmatorul)
                    
                    // mai trebuie extins bufferul
                    while (responseSize < crtResponseSize + recvSize)
                    {
                        response = realloc(response, 2 * responseSize + 1);
                        responseSize = responseSize * 2 + 1;
                    }

                    memcpy(response+crtResponseSize, msg.data, msg.dataSize);
                    crtResponseSize += msg.dataSize;
                    

                    if (recvSize <= 0)
                    {
                        // am descarcat toata pagina, webserverul a inchis conexiunea
                        // parsez pt linkuri

                        sendLinks(serverSock, hrefPattern, response, hostname, path);

                        free(response);
                        responseSize = 0;

                        FD_CLR(i, &readFds);
                        close(i);
                    }
                }
            }
        }
         
    }
     
}
