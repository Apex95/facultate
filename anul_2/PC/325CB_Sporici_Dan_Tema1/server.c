/* Sporici Dan / 325CB */


#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include "lib.h"

#define HOST "127.0.0.1"
#define PORT 10001

#define TRY_RECEIVE_MESSAGE(r)\
{if (recv_message(&(r)) < 0) {\
perror("[RECEIVER] Receive data error. Exiting.\n");\
return -1;}\
printf("%s\n", (r).payload);}

#define TRY_SEND_MESSAGE(r)\
{if (send_message(&(r)) < 0) {\
perror("[RECEIVER] Send data error. Exiting.\n");\
return -1; }\
printf("%s\n", (r).payload);}

enum
{
    NOF_INTRO_MSGS = 5, // nr de mesaje pana la joc
    NOF_GUESS_TRIES = 10 // nr de incercari la joc
};

// cele 4 moduri de lucru
enum
{
    MODE_BASIC = 0,
    MODE_ACK = 1,
    MODE_PARITY = 2,
    MODE_HAMMING = 3
};

const char * responses[] = {"Hello", "", "", "YEY", "OK"};
const char * MSG_SUCCESS = "success";
const char * MSG_BIGGER = "bigger";
const char * MSG_SMALLER = "smaller";
const char * MSG_EXIT = "exit";

int handleIntro(msg, msg, msg, int);
int handleGame(msg, msg, msg, int);
char getPayloadParity(char [], char, int);
char isValidParity(char [], int);
void adaptPayloadForParity(char [], int);
void hammingDecode(const char [], int, char []);
void hammingEncode(const char [], int, char []);




char getPayloadParity(char payload[], char hasParityFlag, int len)
{
    int j = 0;
    int i = 0;

    char bitSum = 0;

    // daca are flag de paritate, incep sa numar de la al 2-lea octet
    // altfel, de la primul    

    // verific toti octetii cu exceptia celui de paritate
    for (i = hasParityFlag ? 1 : 0; i < len; i++)
    {
        // obs: success / smaller / bigger contin 3 caractere in plus

        // verific bitii din octet
        for (j = 0; j < 8; j++)
            bitSum ^= ((payload[i] >> j) & 1);        
    }
    
    
    // clientul nu raspunde bine daca trimit primul octet ca fiind 
    // format doar din 0 (null terminator) => ii trimit caracter
    return bitSum + '0';
}

// verifica daca paritatea corespunde
char isValidParity(char payload[], int len)
{
    char bitSum = getPayloadParity(payload, 1, len); // sigur are flag de paritate
    return (payload[0] & 1) == bitSum - '0'; 
}

// adauga bit (de fapt octet) de paritate
void adaptPayloadForParity(char payload[], int len)
{
    char * _payload = strdup(payload);
    // sigur nu are deja flag pt paritate
    sprintf(payload, "%c%s", getPayloadParity(payload, 0, len), _payload);
    free(_payload);
}

void hammingDecode(const char buffer[], int len, char decodedBuffer[])
{
    int i = 0, j = 0;
    unsigned char d[8];
    unsigned char p[4];

    unsigned char decodedByte = 0;

    for (i = 0; i < len-1; i+=2)
    {
        // primul octet
        decodedByte = 0;
        for (j = 0; j < 4; j++)
            if (j == 2)
                d[0] = (buffer[i] >> (3-j)) & 1;
            else
                if (j == 3)
                    p[2] = (buffer[i] >> (3-j)) & 1;
                else
                    p[j] = (buffer[i] >> (3-j)) & 1;


        // al 2-lea octet
        for (j = 0; j < 8; j++)
        {
            if (j == 3)
                p[3] = (buffer[i+1] >> (7-j)) & 1;
            else
                if (j < 3)
                    d[j+1] = (buffer[i+1] >> (7-j)) & 1;
                else
                    d[j] = (buffer[i+1] >> (7-j)) & 1;
        }

        p[0] = p[0] ^ d[0] ^ d[1] ^ d[3] ^ d[4] ^ d[6];
        p[1] = p[1] ^ d[0] ^ d[2] ^ d[3] ^ d[5] ^ d[6];
        p[2] = p[2] ^ d[1] ^ d[2] ^ d[3] ^ d[7];
        p[3] = p[3] ^ d[4] ^ d[5] ^ d[6] ^ d[7];

        // sindromul
        int pos = p[0] + p[1] * 2 + p[2] * 4 + p[3] * 8;

        // intereseaza doar corectitudinea bitilor de date
        if (pos != 0)
            switch (pos)
            {
                case 3: d[0] ^= 1; break;
                case 5: d[1] ^= 1; break;
                case 6: d[2] ^= 1; break;
                case 7: d[3] ^= 1; break;
                case 9: d[4] ^= 1; break;
                case 10: d[5] ^= 1; break;
                case 11: d[6] ^= 1; break;
                case 12: d[7] ^= 1; break;
            }

        // construiesc octetul decodificat & corectat
        for (j = 0; j < 8; j++)
        {
            decodedByte ^= d[j];
            
            if (j != 7)
                decodedByte <<= 1;
        }

        // il trec in buffer
        decodedBuffer[i/2] = decodedByte;
    }
}

void hammingEncode(const char buffer[], int len, char encodedBuffer[])
{
   int i = 0, j = 0;
   unsigned char d[8]; // 8 octeti de date
   unsigned char p[4]; // 4 octeti de paritate (P1, P2, P4, P8)
   unsigned char encodedByte1 = 0, encodedByte2 = 0;

   for (i = 0; i < len; i++)
   {
       for (j = 7; j >= 0; j--)
           d[7-j] = (buffer[i] >> j) & 1;

       p[0] = d[0] ^ d[1] ^ d[3] ^ d[4] ^ d[6];
       p[1] = d[0] ^ d[2] ^ d[3] ^ d[5] ^ d[6];
       p[2] = d[1] ^ d[2] ^ d[3] ^ d[7];
       p[3] = d[4] ^ d[5] ^ d[6] ^ d[7];

       encodedByte1 = 0;
       encodedByte2 = 0;

       // extrag datele din primul octet (0 0 0 0 P1 P2 D1 P4)
       for (j = 0; j < 4; j++)
       {
           if (j == 2)
               encodedByte1 ^= d[0];
           else
               if (j == 3)
                   encodedByte1 ^= p[j-1];
               else
                   encodedByte1 ^= p[j];
      
           // pt ultima pozitie nu mai shiftez 
           if (j != 3)
               encodedByte1 <<= 1;
       }


       // apoi din al 2-lea octet
       for (j = 0; j < 8; j++)
       {
           if (j == 3)
               encodedByte2 ^= p[3];
           else
               if (j > 3)
                   encodedByte2 ^= d[j];
               else
                   encodedByte2 ^= d[j+1];

           if (j != 7)
               encodedByte2 <<= 1;
            
       }


       encodedBuffer[i*2] = encodedByte1;
       encodedBuffer[i*2+1] = encodedByte2;
   }

   // daca nu completez cei 8 octeti, imi zice ca nu e numar (la modul hamming)
   // => 2 null terminators la final
   encodedBuffer[i*2] = '\0';
   encodedBuffer[i*2+1] = '\0';
}


int handleIntro(msg r, msg ack_msg, msg nack_msg, int mode)
{
    int i;
    msg _ack;
    char * _payload;

    for (i = 0; i < NOF_INTRO_MSGS; i++) 
    {
        if (i != 4)  // intre YEY si OK nu se asteapta alte date de la client
        {
            TRY_RECEIVE_MESSAGE(r);
            if (mode != MODE_BASIC) TRY_SEND_MESSAGE(ack_msg);
        }
        
        // decid cand e nevoie sa raspund clientului
        if (i == 0 || i == 3 || i == 4) 
        {    
            strcpy(r.payload, responses[i]);
            
            if (mode == MODE_PARITY)
                adaptPayloadForParity(r.payload, strlen(r.payload));   
 
            _payload = strdup(r.payload);
            if (mode == MODE_HAMMING)
                hammingEncode(_payload, strlen(r.payload), r.payload);
            free(_payload);

            r.len = strlen(r.payload)+1;

            TRY_SEND_MESSAGE(r);
            if (mode != MODE_BASIC) TRY_RECEIVE_MESSAGE(_ack);
        }
    }

    return handleGame(r, ack_msg, nack_msg, mode);
}

int handleGame(msg r, msg ack_msg, msg nack_msg, int mode)
{
    int i;
    // "Time to play that game"
    TRY_RECEIVE_MESSAGE(r);
    if (mode != MODE_BASIC) TRY_SEND_MESSAGE(ack_msg);

    int max = 1000; int n = 0;
    int min = 0; int encodedLen = 0;

    char * _payload;
    msg _ack;

    // cautare binara iterativa
    for (i = 0; i < NOF_GUESS_TRIES; i++) 
    {
        n = (min + max) / 2;
         
        sprintf(r.payload, "%d", n);

        // daca primul octet are toti bitii 0, clientul
        // trimite ACK dar pe urma nu mai comunica
        if (mode == MODE_PARITY)
            adaptPayloadForParity(r.payload, strlen(r.payload));

        // stabilesc dimensiunea mesajului pentru Hamming, ca sa nu fie nevoie
        // sa mai dau memset la payload
        encodedLen = strlen(r.payload) * 2;

        if (mode == MODE_HAMMING)
        {
            _payload = strdup(r.payload);
            hammingEncode(_payload, strlen(r.payload), r.payload);
            r.len = encodedLen+2;
            free(_payload);
        }
        else
            r.len = strlen(r.payload)+1;

        TRY_SEND_MESSAGE(r);
        if (mode != MODE_BASIC) TRY_RECEIVE_MESSAGE(_ack);
            
        // e necesar sa primesc mesajul corect => nack-uri
        // pana e trimis cum trebuie
        do
        {
            TRY_RECEIVE_MESSAGE(r); // bigger / smaller / success ?

            if (mode == MODE_PARITY && isValidParity(r.payload, r.len))
                break;

            if (mode == MODE_PARITY) TRY_SEND_MESSAGE(nack_msg);

        } while (1 && mode == MODE_PARITY);

        // confirmat ca e bun (ack)
        if (mode != MODE_BASIC) TRY_SEND_MESSAGE(ack_msg);

        if (mode == MODE_HAMMING)
            hammingDecode(r.payload, r.len, r.payload);

        // offset ca sa depasesc bitul de paritate
        int parityOffset = (mode == 2) ? 1 : 0;

        if (strstr(r.payload+parityOffset, MSG_SUCCESS) != NULL)
            break;
            
        if (strstr(r.payload+parityOffset, MSG_SMALLER) != NULL)
            max = n;

        if (strstr(r.payload+parityOffset, MSG_BIGGER) != NULL)
            min = n;
    }

    // "Good job, thanks for playing"
    TRY_RECEIVE_MESSAGE(r);
    if (mode != MODE_BASIC) TRY_SEND_MESSAGE(ack_msg);
    
    sprintf(r.payload, "%s", MSG_EXIT);

    if (mode == MODE_PARITY)
        adaptPayloadForParity(r.payload, strlen(r.payload));
    
    // "exit"
    if (mode == MODE_HAMMING)
    {
        hammingEncode(MSG_EXIT, strlen(MSG_EXIT), r.payload);
        r.len = strlen(MSG_EXIT) * 2;
    }
    else
        r.len = strlen(r.payload);
    TRY_SEND_MESSAGE(r);

    if (mode != MODE_BASIC) TRY_RECEIVE_MESSAGE(_ack);

    return 0;
}

int main(int argc, char ** argv)
{
    msg r;
      
    // ack	
    msg ack_msg;
    strcpy(ack_msg.payload, "ACK");
    ack_msg.len = strlen(ack_msg.payload)+1;

    // nack
    msg nack_msg;
    strcpy(nack_msg.payload, "NACK");
    nack_msg.len = strlen(nack_msg.payload)+1;

	printf("[RECEIVER] Starting.\n");
	init(HOST, PORT);

    int mode = MODE_BASIC;

    if (argc > 1)
    {
        if (strstr(argv[1], "ack"))
            mode = MODE_ACK;

        if (strstr(argv[1], "parity"))
            mode = MODE_PARITY;

        if (strstr(argv[1], "hamming"))
            mode = MODE_HAMMING;
    }


    handleIntro(r, ack_msg, nack_msg, mode);

	printf("[RECEIVER] Finished receiving..\n");
	return 0;
}
