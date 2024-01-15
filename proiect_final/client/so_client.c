#include"so_client.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<libgen.h>


//structura care pastreaza datele conexiunii
typedef struct _so_conn
{
    int socket_desc;
    struct sockaddr_in server_addr;
    char server_message[2000], client_message[2000];
} SO_CONN;


//functia pentru conectarea la server si care imi populeaza structura _SO_CONN
SO_CONN *connectToServer(const char *serverIP)
{
    SO_CONN *connection=(SO_CONN*)malloc(sizeof(SO_CONN));
    if(connection==NULL)
        return NULL;

    memset(connection->server_message,'\0',sizeof(connection->server_message));
    memset(connection->client_message,'\0',sizeof(connection->client_message));
 
    // Create socket, we use SOCK_STREAM for TCP
    connection->socket_desc = socket(AF_INET, SOCK_STREAM, 0);
 
    if(connection->socket_desc < 0)
    {
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

//functia pentru terminarea conexiunii
void endConnectionToServer(SO_CONN *connection)
{
    close(connection->socket_desc);
    free(connection);
    printf("Connection closed successfully\n");
}

//functie pentru trimiterea unui mesaj generic catre server
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

//functie de adunare
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

//functia pwd
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

//functia ls
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

//functie criptare base64
const char* base64(SO_CONN *connection,const char *message)
{
    memset(connection->server_message,'\0',sizeof(connection->server_message));
    memset(connection->client_message,'\0',sizeof(connection->client_message));
    strcpy(connection->client_message,"x05");

    strcat(connection->client_message,message);
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

//functie decriptare base64
const char* unbase64(SO_CONN *connection,const char *message)
{
    memset(connection->server_message,'\0',sizeof(connection->server_message));
    memset(connection->client_message,'\0',sizeof(connection->client_message));
    strcpy(connection->client_message,"x06");

    strcat(connection->client_message,message);
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

//functie hash
const char* hash_djb2(SO_CONN *connection,const char *message)
{
    memset(connection->server_message,'\0',sizeof(connection->server_message));
    memset(connection->client_message,'\0',sizeof(connection->client_message));
    strcpy(connection->client_message,"x07");

    strcat(connection->client_message,message);
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

//functia download
void downloadFile(SO_CONN *connection,char *filename)
{
    memset(connection->server_message,'\0',sizeof(connection->server_message));
    memset(connection->client_message,'\0',sizeof(connection->client_message));
    strcpy(connection->client_message,"x08");

    if(send(connection->socket_desc, connection->client_message, strlen(connection->client_message), 0) < 0)
    {
        printf("Unable to send message\n");
        return;
    }

    char buffer[4096];
    char received_buffer[4096];
    char* filename1 = basename(filename);
    char path[255] = "./download/";
    memcpy(path+11,filename1,strlen(filename1));

    

    uint32_t size = strlen(path)+1;
    memcpy(buffer, &size, sizeof(size));
    memcpy(buffer+4, filename, strlen(filename));


    int bytes_sent = send(connection->socket_desc, buffer,  sizeof(uint32_t) + strlen(filename) + 1, 0);
	if( bytes_sent < 0){
        fprintf(stderr,"Unable to send message\n");
        exit(EXIT_FAILURE);
    }

    int file_fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (file_fd == -1) {
        fprintf(stderr, "Eroare la deschiderea fisierului");
        exit(EXIT_FAILURE);
    }

    ssize_t bytesRead;
    
    while ((bytesRead = recv(connection->socket_desc, received_buffer, sizeof(received_buffer), 0)) > 0) {
        
        if(strcmp(received_buffer,"ack")==0)
            break;
        else
        {
            
            write(file_fd, received_buffer, bytesRead);
        }
        memset(received_buffer,'\0',sizeof(received_buffer));
    }
    
    if (bytesRead == -1) {
        fprintf(stderr, "Eroare la receptia fisierului");
        exit(EXIT_FAILURE);
    }

    printf("Fisierul a fost primit cu succes.\n");

}

//functia upload
void upload_file(SO_CONN *connection, char* filename)
{

    memset(connection->server_message,'\0',sizeof(connection->server_message));
    memset(connection->client_message,'\0',sizeof(connection->client_message));
    strcpy(connection->client_message,"x09");

    if(send(connection->socket_desc, connection->client_message, strlen(connection->client_message), 0) < 0)
    {
        printf("Unable to send message\n");
        return;
    }

    char received_buffer[4096];
    char buffer[4096];
    memset(buffer,'\0',sizeof(buffer));
    uint32_t op_code = 0x3;
    uint32_t size=strlen(filename)+1;
    memcpy(buffer,&op_code, sizeof(uint32_t));
    memcpy(buffer+4,&size, sizeof(uint32_t));
    memcpy(buffer+8,filename,strlen(filename));
    
    
    send(connection->socket_desc, buffer, 2*sizeof(uint32_t) + strlen(filename)+1,0);

    int file_fd = open(filename, O_RDONLY);
    if (file_fd == -1) 
    {
        fprintf(stderr, "Eroare la deschiderea fisierului");
        exit(EXIT_FAILURE);
    }
    off_t offset = 0;
    struct stat file_stat;
    fstat(file_fd, &file_stat);

    memset(connection->server_message,'\0',sizeof(connection->server_message));
    
    if(recv(connection->socket_desc, connection->server_message, sizeof(connection->server_message), 0) < 0)
    {
        printf("Error while receiving server's msg\n");
        exit(EXIT_FAILURE);
    }
   // sleep(1);
    if (sendfile(connection->socket_desc, file_fd, &offset, file_stat.st_size) == -1) {
        perror("Eroare la trimiterea fisierului");
        exit(EXIT_FAILURE);
    }
    
    if(send(connection->socket_desc, "EOF", sizeof("EOF"), 0) < 0)
    {
        printf("Unable to send message\n");
        return;
    }
    //send(connection->socket_desc,"ack",3,0);
}

//functia "Shell" pentru executarea comenzilor pe pc-ul unde se afla serverul
void commandExec(SO_CONN *connection)
{

    while(1) 
    {
        printf("> ");

        char *line;
        char lineRead[maxLineDim];
        fgets(lineRead,maxLineDim,stdin);
        for(int i=0;i<strlen(lineRead);i++)
            if(lineRead[i]=='\n')
            {
                lineRead[i]='\0';
                break;
            }
        line=(char*)malloc(sizeof(char)*(strlen(lineRead)+1));
        strcpy(line,lineRead);
        
        if(!strcmp(line,"exit"))                  
            break;

        memset(connection->server_message,'\0',sizeof(connection->server_message));
        memset(connection->client_message,'\0',sizeof(connection->client_message));
        strcpy(connection->client_message,"x10");

        strcat(connection->client_message,line);
        if(send(connection->socket_desc, connection->client_message, strlen(connection->client_message), 0) < 0)
        {
            printf("Unable to send message\n");
            return;
        }
        
        if(recv(connection->socket_desc, connection->server_message, sizeof(connection->server_message), O_NONBLOCK) < 0)
        {
            printf("Error while receiving server's msg\n");
            return;
        }
        printf("%s",connection->server_message);
    }
}