#ifndef SO_CLIENT_H
#define SO_CLIENT_H

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include<stdlib.h>

struct _so_conn;

typedef struct _so_conn SO_CONN;


SO_CONN *connectToServer(const char *serverIP);

void endConnectionToServer(SO_CONN *connection);

void sendMessage(SO_CONN *connection);

float sum(SO_CONN *connection,float a,float b);
const char *pwd(SO_CONN *connection);
const char *ls(SO_CONN *connection,const char *firstPar,const char *secondPar);
void mkdir(SO_CONN *connection,const char *newDirName);


#endif