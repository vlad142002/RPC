#include"so_client.h"

typedef struct _so_conn
{
    int socket_desc;
    struct sockaddr_in server_addr;
    char server_message[2000], client_message[2000];
} SO_CONN;

SO_CONN *connectToServer(const char *serverIP)
{
    SO_CONN *connection=(SO_CONN*)malloc(sizeof(SO_CONN));
    if(connection==NULL)
        return NULL;

    memset(connection->server_message,'\0',sizeof(connection->server_message));
    memset(connection->client_message,'\0',sizeof(connection->client_message));
 
    // Create socket, we use SOCK_STREAM for TCP
    connection->socket_desc = socket(AF_INET, SOCK_STREAM, 0);
 
    if(connection->socket_desc < 0){
        printf("Unable to create socket\n");
        return NULL;
    }
 
    printf("Socket created successfully\n");
 
    // Set port and IP the same as server-side:
    connection->server_addr.sin_family = AF_INET;
    connection->server_addr.sin_port = htons(2000);
    connection->server_addr.sin_addr.s_addr = inet_addr(serverIP);
 
    // Send connection request to server:
    if(connect(connection->socket_desc, (struct sockaddr*)&connection->server_addr, sizeof(connection->server_addr)) < 0){
        printf("Unable to connect\n");
        return NULL;
    }
    printf("Connected with server successfully\n");
    return connection;
}

void endConnectionToServer(SO_CONN *connection)
{
    close(connection->socket_desc);
    free(connection);
    printf("Connection closed successfully\n");
}

void sendMessage(SO_CONN *connection)
{
    // Get input from the user:
    printf("Enter message: ");
    gets(connection->client_message);

    // Send the message to server:
    if(send(connection->socket_desc, connection->client_message, strlen(connection->client_message), 0) < 0){
        printf("Unable to send message\n");
        return;
    }
    memset(connection->server_message,'\0',strlen(connection->server_message));
    // Receive the server's response:
    if(recv(connection->socket_desc, connection->server_message, sizeof(connection->server_message), 0) < 0){
        printf("Error while receiving server's msg\n");
        return;
    }
    
    printf("Server's response: %s\n",connection->server_message);
}

float sum(SO_CONN *connection,float a,float b)
{
    memset(connection->server_message,'\0',sizeof(connection->server_message));
    memset(connection->client_message,'\0',sizeof(connection->client_message));

    strcpy(connection->client_message,"x01");
    char *firstNmb=(char*)malloc(20);
    char *secondNmb=(char*)malloc(20);
    sprintf(firstNmb,"%f",a);
    sprintf(secondNmb,"%f",b);
    strcat(connection->client_message,firstNmb);
    strcat(connection->client_message,",");
    strcat(connection->client_message,secondNmb);
    
    if(send(connection->socket_desc, connection->client_message, strlen(connection->client_message), 0) < 0){
        printf("Unable to send message\n");
        return;
    }

    // Receive the server's response:
    if(recv(connection->socket_desc, connection->server_message, sizeof(connection->server_message), 0) < 0){
        printf("Error while receiving server's msg\n");
        return;
    }

    
    float rez=0;
    rez=atof(connection->server_message);
    
    return rez;
}

const char *pwd(SO_CONN *connection)
{
    memset(connection->server_message,'\0',sizeof(connection->server_message));
    memset(connection->client_message,'\0',sizeof(connection->client_message));
    strcpy(connection->client_message,"x02");
    if(send(connection->socket_desc, connection->client_message, strlen(connection->client_message), 0) < 0)
    {
        printf("Unable to send message\n");
        return;
    }

    if(recv(connection->socket_desc, connection->server_message, sizeof(connection->server_message), 0) < 0)
    {
        printf("Error while receiving server's msg\n");
        return;
    }
    return connection->server_message;
}

const char *ls(SO_CONN *connection,const char *firstPar,const char *secondPar)
{
    memset(connection->server_message,'\0',sizeof(connection->server_message));
    memset(connection->client_message,'\0',sizeof(connection->client_message));
    strcpy(connection->client_message,"x03");

    if(firstPar!=NULL)
        strcat(connection->client_message,firstPar);
    strcat(connection->client_message,",");
    if(secondPar!=NULL)
        strcat(connection->client_message,secondPar);
    if(send(connection->socket_desc, connection->client_message, strlen(connection->client_message), 0) < 0)
    {
        printf("Unable to send message\n");
        return;
    }

    if(recv(connection->socket_desc, connection->server_message, sizeof(connection->server_message), 0) < 0)
    {
        printf("Error while receiving server's msg\n");
        return;
    }
    return connection->server_message;
}

void mkdir(SO_CONN *connection,const char *newDirName)
{
    memset(connection->server_message,'\0',sizeof(connection->server_message));
    memset(connection->client_message,'\0',sizeof(connection->client_message));
    strcpy(connection->client_message,"x04");

    strcat(connection->client_message,newDirName);
    if(send(connection->socket_desc, connection->client_message, strlen(connection->client_message), 0) < 0)
    {
        printf("Unable to send message\n");
        return;
    }

    if(recv(connection->socket_desc, connection->server_message, sizeof(connection->server_message), 0) < 0)
    {
        printf("Error while receiving server's msg\n");
        return;
    }
}