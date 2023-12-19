#include"so_server.h"




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
    /*
    char *buffer=(char*)malloc(2000);
    int pid=fork();
    if(pid<0)
        return NULL;
    int pipe_fd[2];
    pipe(pipe_fd);
    if(pid==0)
    {    
        close(pipe_fd[0]);
        dup2(pipe_fd[1],STDIN_FILENO);
        execlp("sh","sh","-c","pwd",NULL);
        exit(-1);
    }
    else
    {
        wait(NULL);
        close(pipe_fd[1]);
        int ret=read(pipe_fd[0],buffer,2000);
        buffer[ret]='\0';
        close(pipe_fd[0]);
        
    }
    return buffer;
    */
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
                const char *homeDir = getenv("HOME");
                strcat(homeDir,firstPar+1);
                strcpy(firstPar,homeDir);
            }
            char *args[] = {"ls", firstPar, NULL};
            execvp("ls", args);
        }
        if(firstPar[0]=='~')
            {
                const char *homeDir = getenv("HOME");
                strcat(homeDir,firstPar+1);
                strcpy(firstPar,homeDir);
            }
            if(secondPar[0]=='~')
            {
                const char *homeDir = getenv("HOME");
                strcat(homeDir,secondPar+1);
                strcpy(secondPar,homeDir);
            }
        char *args[] = {"ls", firstPar,secondPar, NULL};
        execvp("ls", args);
        
    }


}

void mkdir(int clientSocket, char *pars)
{
    int pid=fork();
    if(pid<0)
        return;
    int pipe_fd[2];
    pipe(pipe_fd);
    if(pid==0)
    {    
        execlp("mkdir","mkdir",pars,NULL);
    }

}