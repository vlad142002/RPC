#ifndef SO_SERVER_H
#define SO_SERVER_H

#include <stdio.h>
#include <string.h>
#include<stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include<unistd.h>
#include<sys/types.h>
#include<wait.h>

#define sizeProt 3

const char *sum(const char *pars);
void pwd(int clientSocket); 
void ls(int clientSocket,const char *pars);
void mkdir(int clientSocket, char *pars);

#endif