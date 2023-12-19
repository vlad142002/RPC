#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include"so_client.h"

int main(void)
{

    SO_CONN *newConnection=connectToServer("192.168.43.75");
    
    printf("Sum of 5.2 + 2.33= %f\n",sum(newConnection,5.2,2.33));
    printf("Current directory: %s\n",pwd(newConnection));
    printf("ls command on ~ : %s\n",ls(newConnection,"-l","~"));
    mkdir(newConnection,"dirTest2");
    printf("ls command on cur dir: %s\n",ls(newConnection,"-l",NULL));

    endConnectionToServer(newConnection);
    return 0;
}
