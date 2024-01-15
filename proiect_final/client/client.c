#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include"so_client.h"

int main(void)
{
   

    SO_CONN *newConnection=connectToServer("192.168.43.41");
    
    //printf("Sum of 5.2 + 2.33= %f\n",sum(newConnection,5.2,2.33));
    //printf("Current directory: %s\n",pwd(newConnection));
    //printf("ls command on ~ : %s\n",ls(newConnection,"-l","~"));
    
    //printf("ls command on cur dir: %s\n",ls(newConnection,"-l",NULL));
    /*
    for(int i=0;i<5;i++)
    {
        printf("ls command on cur dir: %s\n",ls(newConnection,"-l",NULL));
    }
    */
    /*
    char message[64];
    char messageBas64[100];
    strcpy(message,"Hello, World!");
    strcpy(messageBas64,base64(newConnection,message));
    printf("The message \"%s\" coded in base64 is \"%s\"\n",message,messageBas64);
    printf("The message \"%s\" uncoded in base64 is \"%s\"\n",messageBas64,unbase64(newConnection,messageBas64));

    printf("Message hash is: %s\n",hash_djb2(newConnection,message));
    printf("Message hash is: %s\n",hash_djb2(newConnection,message));
    */
    
    //downloadFile(newConnection,"/a.txt");
    //downloadFile(newConnection,"/wow.txt");
    
    //upload_file(newConnection,"./download/a.txt");
    

    commandExec(newConnection);
    
    endConnectionToServer(newConnection);
    return 0;
}
