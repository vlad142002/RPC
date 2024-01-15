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
#define maxCommandDim 100
#define maxNameDim 20
#define maxLineDim 1024
#define maxBufferDim 100

const char *sum(const char *pars);
void pwd(int clientSocket); 
void ls(int clientSocket,const char *pars);
const char *base64(const char *pars);
const char *unbase64(const char *pars);
const char *hash_djb2(const char *pars);
void send_file(char* received_message, int client_socket);
void upload_file(char* received_buffer, int socket_fd);


//functii pentru "Shell"
void commandExec(int clientSocket,char *line);
void setEnvValue(char* line,int clientSocket);
int pipeExist(char* line);
int equalExist(char* line);
void simpleCommandExecution(char **command,int clientSocket);
void redirectCommandExecution(char**firstPart,char* secondPart,int clientSocket);
char* getFileName(char *line);
void pipeCommandExecution(char** firstCommand, char ** secondCommand,int clientSocket);
char** parse_line(char *line);

#endif