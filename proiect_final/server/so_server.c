#include"so_server.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <libgen.h>
#include <pthread.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

const char* sum(const char* pars)
{
    float a=0,b=0;
    char* buffer=(char*)malloc(sizeof(char)*(strlen(pars)+1));
    strcpy(buffer,pars);
    char* p=strtok(buffer,",");
    a=atof(p);
    p=strtok(NULL,",");
    b=atof(p);
    printf("S-a apelat functia de adunare: %f + %f = %f\n",a,b,a+b);
    char* charNmb = (char*)malloc(20);
    sprintf(charNmb,"%f",a+b);
    free(buffer);
    return charNmb;
}

void pwd(int clientSocket)
{
    int pid=fork();
    if(pid<0)
        return;
    int pipe_fd[2];
    pipe(pipe_fd);
    if(pid==0)
    {    
        dup2(clientSocket,STDOUT_FILENO);
        dup2(clientSocket,STDERR_FILENO);
        execlp("sh","sh","-c","pwd",NULL);
    }
    
    
}

void ls(int clientSocket,const char *pars)
{
    char firstPar[100];  
    char secondPar[100]; 

    char buffer[100];  
    strcpy(buffer, pars);

    char *token = strtok(buffer, ",");
    
    if (token != NULL) 
    {
        strcpy(firstPar, token);
        token = strtok(NULL, ",");
        
        if (token != NULL) 
            strcpy(secondPar, token);
        else 
            secondPar[0] = '\0';
    } 
    else 
    {
        firstPar[0] = '\0';
        secondPar[0] = '\0';
    }

    int pid=fork();
    if(pid<0)
        return;
    int pipe_fd[2];
    pipe(pipe_fd);
    if(pid==0)
    {   
        
    
        dup2(clientSocket,STDOUT_FILENO);
        dup2(clientSocket,STDERR_FILENO);
        if(firstPar[0]=='\0')
            execlp("ls","ls",NULL);
        if(secondPar[0]=='\0')
        {
            if(firstPar[0]=='~')
            {
                char *homeDir = getenv("HOME");
                strcat(homeDir,firstPar+1);
                strcpy(firstPar,homeDir);
            }
            char *args[] = {"ls", firstPar, NULL};
            execvp("ls", args);
        }
        if(firstPar[0]=='~')
            {
                char *homeDir = getenv("HOME");
                strcat(homeDir,firstPar+1);
                strcpy(firstPar,homeDir);
            }
            if(secondPar[0]=='~')
            {
                char *homeDir = getenv("HOME");
                strcat(homeDir,secondPar+1);
                strcpy(secondPar,homeDir);
            }
        char *args[] = {"ls", firstPar,secondPar, NULL};
        execvp("ls", args);
        
    }


}


const char *base64(const char *pars)
{
    size_t length=strlen(pars);
    const char base64_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";
    size_t encoded_size = ((length + 2) / 3) * 4;
    char* encoded = (char*)malloc(encoded_size + 1);

    if (encoded == NULL) {
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }

    size_t i, j;
    unsigned char octet2, octet3;

    for (i = 0, j = 0; i < length; i += 3, j += 4) {
        unsigned char octet1 = pars[i];

        if (i + 1 < length) {
            octet2 = pars[i + 1];
        }
        else {
            octet2 = 0;
        }

        if (i + 2 < length) {
            octet3 = pars[i + 2];
        }
        else {
            octet3 = 0;
        }

        encoded[j] = base64_table[octet1 >> 2];
        encoded[j + 1] = base64_table[((octet1 & 0x03) << 4) | (octet2 >> 4)];

        if (i + 1 < length) {
            encoded[j + 2] = base64_table[((octet2 & 0x0F) << 2) | (octet3 >> 6)];
        }
        else {
            encoded[j + 2] = '=';
        }

        if (i + 2 < length) {
            encoded[j + 3] = base64_table[octet3 & 0x3F];
        }
        else {
            encoded[j + 3] = '=';
        }
    }

    encoded[encoded_size] = '\0'; 
    return encoded;


}

const char *unbase64(const char *pars)
{
    size_t length=strlen(pars);
    const char base64_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    size_t decoded_size = (length / 4) * 3;
    unsigned char* decoded = (unsigned char*)malloc(decoded_size + 1);

    if (decoded == NULL) {
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }

    size_t i, j;
    unsigned char sextet1, sextet2, sextet3, sextet4;

    for (i = 0, j = 0; i < length; i += 4, j += 3) {
        sextet1 = strchr(base64_table, pars[i]) - base64_table;
        sextet2 = strchr(base64_table, pars[i + 1]) - base64_table;

        if (pars[i + 2] != '=') {
            sextet3 = strchr(base64_table, pars[i + 2]) - base64_table;
        } else {
            sextet3 = 0;
        }

        if (pars[i + 3] != '=') {
            sextet4 = strchr(base64_table, pars[i + 3]) - base64_table;
        } else {
            sextet4 = 0;
        }

        if (j < decoded_size) {
            decoded[j] = (sextet1 << 2) | (sextet2 >> 4);
        }
        if (j + 1 < decoded_size) {
            decoded[j + 1] = ((sextet2 & 0x0F) << 4) | (sextet3 >> 2);
        }
        if (j + 2 < decoded_size) {
            decoded[j + 2] = ((sextet3 & 0x03) << 6) | sextet4;
        }
    }

    decoded[j] = '\0';
    return decoded;

}

const char *hash_djb2(const char *pars)
{
    
    unsigned long hash = 5381;
    int c;
    
    while ((c = *pars++)) {
        hash = ((hash << 5) + hash) + c; 
    }

    char *hash_str = (char *)malloc(30);
    sprintf(hash_str, "%lu", hash);

    return hash_str;

}


void send_file(char* received_message, int client_socket)
{
    
    char filename[255];
    uint32_t size;
    memcpy(&size,received_message,sizeof(uint32_t));
    memcpy(filename,received_message+4,size);

    char path[255] = "./files";
    memcpy(path+7,filename,strlen(filename));


    int file_fd = open(path, O_RDONLY);
    if (file_fd == -1) 
    {
        fprintf(stderr, "Eroare la deschiderea fisierului");
        exit(EXIT_FAILURE);
    }
    off_t offset = 0;
    struct stat file_stat;
    fstat(file_fd, &file_stat);
    off_t fileSize = lseek(file_fd, 0, SEEK_END);
    if (sendfile(client_socket, file_fd, &offset, file_stat.st_size) == -1) {
        perror("Eroare la trimiterea fisierului");
        exit(EXIT_FAILURE);
    }
    

    send(client_socket, "ack", 3,0);
}

void upload_file(char* received_buffer, int socket_fd)
{
    char buffer[4096];
    char filename[255];
    uint32_t size;
    memcpy(&size, received_buffer+4,sizeof(uint32_t));
    memcpy(filename, received_buffer+8, size);
    char* filename1 = basename(filename);
    char path[255] = "./files/";
    memcpy(path+8,filename1,strlen(filename1));

    int file_fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (file_fd == -1) {
        fprintf(stderr, "Eroare la deschiderea fisierului");
        exit(EXIT_FAILURE);
    }

    ssize_t bytesRead;

    

    if (send(socket_fd, "ack", sizeof("ack"), 0) < 0)
    {
            printf("Can't send\n");
            return ;
    }
    memset(received_buffer,'\0',sizeof(received_buffer));
   
    while ((bytesRead = recv(socket_fd, received_buffer, 4096, 0)) > 0) {
   
    if (strcmp(received_buffer, "EOF") == 0) {
        printf("Received EOF signal. File transmission complete.\n");
        break;
    }  
    else 
    {
        
        write(file_fd, received_buffer, bytesRead);
    }

    memset(received_buffer, '\0', sizeof(received_buffer));
}
        

    if (bytesRead == -1) {
        fprintf(stderr, "Eroare la receptia fisierului");
        exit(EXIT_FAILURE);
    }

    printf("Fisierul a fost primit cu succes.\n");

}



void commandExec(int clientSocket,char *line)
{
    printf(">\"%s\"\n",line);

    
    if(pipeExist(line))
    {
        printf("command with pipe\n");

        char* lineCopy=(char*)malloc(sizeof(char)*(strlen(line)+1));
        strcpy(lineCopy,line);
        char*p=strtok(lineCopy,"|");
        char* firstPart=(char*)malloc(sizeof(char)*(strlen(p)+1));
        strcpy(firstPart,p);
        p=strtok(NULL,">|");
        char**firstCommand=parse_line(firstPart);
        char**secondCommand=parse_line(p);
        
        pipeCommandExecution(firstCommand,secondCommand,clientSocket);
    }
    else 
    {   
        
        if(equalExist(line))
            setEnvValue(line,clientSocket);
        else
        {
            if(redirectExist(line))
            {
                char* lineCopy=(char*)malloc(sizeof(char)*(strlen(line)+1));
                strcpy(lineCopy,line);
                char*p=strtok(lineCopy,">");
                char* firstPart=(char*)malloc(sizeof(char)*(strlen(p)+1));
                strcpy(firstPart,p);
                p=strtok(NULL,">");
                char**simpleComand=parse_line(firstPart);
                char*fileName=getFileName(p);
                redirectCommandExecution(simpleComand,fileName,clientSocket);
            }
            else
                if(!pipeExist(line) && !equalExist(line) && !redirectExist(line))
                {
                    char**simpleCommand=parse_line(line);
                    simpleCommandExecution(simpleCommand,clientSocket);
                }
        }
    }
    
}

int redirectExist(char*line)
{
    for(int i=0;i<strlen(line);i++)
        if(line[i]=='>')
            return 1;
    return 0;
}

char* get_line()
{
    char *buffer;
    char lineRead[maxLineDim];
    fgets(lineRead,maxLineDim,stdin);
    for(int i=0;i<strlen(lineRead);i++)
        if(lineRead[i]=='\n')
        {
            lineRead[i]='\0';
            break;
        }
    buffer=(char*)malloc(sizeof(char)*(strlen(lineRead)+1));
    strcpy(buffer,lineRead);
    
    return buffer;
}

void repalceDollar(char** command,int nrPar)
{
    for(int i=0;i<nrPar;i++)
        if(command[i][0]=='$')
        {
            char name[maxNameDim];
            strcpy(name,command[i]+1);
            char value[maxCommandDim];
            if(getenv(name))
                strcpy(value,getenv(name));
            else
                strcpy(value,"");
            strcpy(command[i],value);
        }
}

char** parse_line(char *line)
{
    
    
    int nrParametrii=0;
    char* lineCopy=(char*)malloc(sizeof(char)*(strlen(line)+1));
    strcpy(lineCopy,line);

    char *p;
    p=strtok(lineCopy," ");
    while(p!=NULL)
    {
        nrParametrii ++;
        p=strtok(NULL," ");
    }
    
    char **buffer=(char**)malloc(sizeof(char*)*(nrParametrii+1));
    for(int i=0;i<nrParametrii+1;i++)
    buffer[i]=(char*)malloc(sizeof(char)*maxCommandDim);
    buffer[nrParametrii]=NULL;

    strcpy(lineCopy,line);

    int i=0;
    p=strtok(lineCopy," ");
    
    while(p!=NULL)
    {
        
            strcpy(buffer[i++],p);
            p=strtok(NULL," ");
    }
    
    free(lineCopy);

    repalceDollar(buffer,nrParametrii);
    return buffer;
}

void setEnvValue(char* line,int clientSocket)
{
    char name[maxNameDim];
    char *p=strtok(line,"=");
    strcpy(name,p);
    p=strtok(NULL,"=");
    setenv(name,p,1);
    
    printf("command that sets value\n");

    if (send(clientSocket, "\n", strlen("\n"), 0) < 0){
        printf("Can't send\n");
        return ;
    }
}

int pipeExist(char* line)
{
    for(int i=0;i<strlen(line);i++)
        if(line[i]=='|')
            return 1;
    return 0;
}

int equalExist(char* line)
{
    for(int i=0;i<strlen(line);i++)
        if(line[i]=='=')
            return 1;
    return 0;
}

void simpleCommandExecution(char **command,int clientSocket)
{
    
    int pid;
    int pipe_fd[2];

    if (pipe(pipe_fd) == -1) {
        perror("Error creating pipe");
        exit(EXIT_FAILURE);
    }

    pid = fork();

    if (pid < 0) {
        perror("Error creating child process");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        
        close(pipe_fd[0]); 

        dup2(pipe_fd[1], STDOUT_FILENO);
        close(pipe_fd[1]);

        execvp(command[0],command);
        
        perror("execvp");
        exit(EXIT_FAILURE);
    } else {
        
        close(pipe_fd[1]); 

        
        char buffer[4096];
        ssize_t bytesRead;

    

        while ((bytesRead = read(pipe_fd[0], buffer, sizeof(buffer))) > 0) {
            // Send the output to the client socket
            if (send(clientSocket, buffer, bytesRead, 0) == -1) {
                perror("Error sending data to client");
                exit(EXIT_FAILURE);
            }
        }
        memset(buffer,'\0',sizeof(buffer));

        if (bytesRead == 0) {
            
            const char* defaultMessage = "\n";
            if (send(clientSocket, defaultMessage, strlen(defaultMessage), 0) == -1) {
                perror("Error sending default message to client");
                exit(EXIT_FAILURE);
            }
        }

        close(pipe_fd[0]);

        wait(NULL);
    }

}

void redirectCommandExecution(char**firstPart,char* secondPart,int clientSocket)
{
    printf("redirect command exist\n");
    int pid=fork();
    if(pid<0)
        printf("Error!\n");
    else if(pid==0)
    {    
        int fd=open(secondPart,O_WRONLY| O_CREAT | O_TRUNC,0664);
        dup2(fd,STDOUT_FILENO);
        execvp(firstPart[0],firstPart);
        exit(0);
    }
    if (send(clientSocket, "\n", strlen("\n"), 0) < 0){
        printf("Can't send\n");
        return ;
    }
}

char* getFileName(char *line)
{
    if(line[0]==' ')
        strcpy(line,line+1);
    return line;
}

void pipeCommandExecution(char** firstCommand, char ** secondCommand,int clientSocket)
{
    int pid=fork();
    if(pid<0)
        printf("Error!\n");
    else if(pid==0)
    {    
        dup2(clientSocket,STDOUT_FILENO);
        dup2(clientSocket,STDERR_FILENO);
        int pipe_fd[2];
        pipe(pipe_fd);

        int pid2=fork();
        if(pid2<0)
            printf("Error!\n");
        else if(pid2!=0)
        {
            wait(NULL);
            close(pipe_fd[1]);
            dup2(pipe_fd[0],STDIN_FILENO);
           
            execvp(secondCommand[0],secondCommand);        
        }
        else
        {
            close(pipe_fd[0]);
            dup2(pipe_fd[1],STDOUT_FILENO);
            
            execvp(firstCommand[0],firstCommand); 
        }
            exit(0);
    }
    else
        wait(NULL);
}