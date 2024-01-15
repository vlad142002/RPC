#ifndef SO_CLIENT_H
#define SO_CLIENT_H

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>

#define maxCommandDim 100
#define maxNameDim 20
#define maxLineDim 1024
#define maxBufferDim 100

struct _so_conn;

typedef struct _so_conn SO_CONN;


SO_CONN *connectToServer(const char *serverIP);                                         //functie pentru conectarea cu serverul

void endConnectionToServer(SO_CONN *connection);                                        //functie pentru terminarea conexiunii cu serverul

void sendMessage(SO_CONN *connection);                                                  //functie generica pentru a trimite un mesaj catre server
                                                                                        
float sum(SO_CONN *connection,float a,float b);                                         //functia sum                       x01firstNr,secondNr
const char *pwd(SO_CONN *connection);                                                   //functia pwd                       x02
const char *ls(SO_CONN *connection,const char *firstPar,const char *secondPar);         //functia ls                        x03firstArg,secArg
const char* base64(SO_CONN *connection,const char *message);                            //functia de criptare base64        x04mesaj
const char* unbase64(SO_CONN *connection,const char *message);                          //functia de decriptare base64      x06mesaj
const char* hash_djb2(SO_CONN *connection,const char *message);                         //functie de hash                   x07mesaj
void downloadFile(SO_CONN *connection,char *filename);                                  //functia pentru download           x08
void upload_file(SO_CONN *connection, char* filename);                                  //functia pentru upload             x09
void commandExec(SO_CONN *connection);                                                  //functia "Shell"                   x10command
#endif