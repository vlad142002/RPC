#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

#include"so_server.h"

#define MAX_THREADS 10
#define sizeProt 3

pthread_mutex_t mutex;

void unPackMessage(char* message,int client_socket)
{
    char prot[sizeProt];
    if(strlen(message)<=1)
    {
        printf("Eroare! Pachet prea scurt!\n Mesaj: %s\n",message);
        return;
    }   
   
    if(message[0]!='x')
    {
        printf("Eroare! Check caracterul nu se potriveste!\n");
        return;
    }

    if(!isdigit(message[1]) && !isdigit(message[2]))
    {
        printf("Eroare! Valoarea pentru numarul protocolului nu e valida\n");
        return;
    }

    prot[0]=message[1];
    prot[1]=message[2];
    prot[2]='\0';
    int protNr=atoi(prot);
    char buffer[4096];
    char server_message[2000];
    switch (protNr)
    {
    case 1:
        strcpy(server_message, sum(message+3));
        
        if (send(client_socket, server_message, strlen(server_message), 0) < 0){
            printf("Can't send\n");
            return ;
        }
        memset(server_message, '\0', sizeof(server_message));
        break;
    case 2:
        pwd(client_socket);
        break;
    case 3:
        ls(client_socket,message+3);
        break;
    case 5:
        strcpy(server_message, base64(message+3));
        
        if (send(client_socket, server_message, sizeof(server_message), 0) < 0){
            printf("Can't send\n");
            return ;
        }
        memset(server_message, '\0', sizeof(server_message));
        break;
    case 6:
        strcpy(server_message, unbase64(message+3));
        
        if (send(client_socket, server_message, sizeof(server_message), 0) < 0){
            printf("Can't send\n");
            return ;
        }
        memset(server_message, '\0', sizeof(server_message));
        break;
    case 7:
        strcpy(server_message, hash_djb2(message+3));
        
        if (send(client_socket, server_message, sizeof(server_message), 0) < 0){
            printf("Can't send\n");
            return ;
        }
        memset(server_message, '\0', sizeof(server_message));
        break;
    case 8:
        memset(buffer, '\0', sizeof(buffer));
        if (recv(client_socket, buffer, sizeof(buffer), 0) <= 0){
            break;
        }
        send_file(buffer,client_socket);
        break;
    case 9:
        memset(buffer, '\0', sizeof(buffer));
        if (recv(client_socket, buffer, sizeof(buffer), 0) <= 0){
            break;
        }
        upload_file(buffer,client_socket);
        break;
    case 10:
        
        pthread_mutex_lock(&mutex);
        commandExec(client_socket,message+3);
        memset(server_message, '\0', sizeof(server_message));

        pthread_mutex_unlock(&mutex);
        break;
    default:
        printf("Eroare! Nu s-a putut selecta un protocol!\n");
        break;
    }
    
       
}

void* thread_routine(void* params) {
    char server_message[2000], client_message[2000];
    int bytes_received;

    int client_socket = (int*)params;

    while (1) {

        if (recv(client_socket, client_message, sizeof(client_message), 0) <= 0){
            break;
        }

        printf("Msg from client: %s\n", client_message);

        unPackMessage(client_message,client_socket);

        //strcpy(server_message, unPackMessage(client_message,client_socket));
        memset(client_message,'\0',sizeof(client_message));
        if (send(client_socket, server_message, strlen(server_message), 0) < 0){
            printf("Can't send\n");
            return -1;
        }
        memset(server_message, '\0', sizeof(server_message));

    }

    close(client_socket);

}

int main(void)
{

    pthread_mutex_init(&mutex, NULL);
    int socket_desc, client_sock, client_size;
    struct sockaddr_in server_addr, client_addr;
    char server_message[2000], client_message[2000];
 
    // Clean buffers:
    memset(server_message, '\0', sizeof(server_message));
    memset(client_message, '\0', sizeof(client_message));
 
    // Create socket:
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    int enable = 1;
    setsockopt(socket_desc, SOL_SOCKET, SO_REUSEPORT, &enable, sizeof(int));
 
    if(socket_desc < 0){
        printf("Error while creating socket\n");
        return -1;
    }
    printf("Socket created successfully\n");
 
    // Set port and IP that we'll be listening for, any other IP_SRC or port will be dropped:
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(2000);
    server_addr.sin_addr.s_addr = inet_addr("192.168.43.41");
 
    // Bind to the set port and IP:
    if(bind(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr))<0){
        printf("Couldn't bind to the port\n");
        return -1;
    }
    printf("Done with binding\n");
 
    // Listen for clients:
    if(listen(socket_desc, 1) < 0){
        printf("Error while listening\n");
        return -1;
    }
    printf("\nListening for incoming connections.....\n");

    pthread_t threads[MAX_THREADS];
    int thread_count = 0;
    
    while(1)
    {
        // Accept an incoming connection from one of the clients:
        client_size = sizeof(client_addr);
        client_sock = accept(socket_desc, (struct sockaddr*)&client_addr, &client_size);
    
        if (client_sock < 0){
            printf("Can't accept\n");
            return -1;
        }
        printf("Client connected at IP: %s and port: %i\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
    
        
        // Receive client's message:
        pthread_create(&threads[thread_count], NULL, thread_routine, (void*)client_sock);
        thread_count++;

        // Check if the maximum number of threads is reached
        if (thread_count == MAX_THREADS) {
            // Wait for threads to finish before accepting new connections
            for (int i = 0; i < thread_count; i++) {
                pthread_join(threads[i], NULL);
            }
            thread_count = 0;
        }
        
    }

    for (int i = 0; i < thread_count; i++) {
        pthread_join(threads[i], NULL);
    }

    close(socket_desc);

    return 0;
}
