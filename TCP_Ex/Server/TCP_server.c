#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <string.h>
#include <unistd.h>
#include <stdlib.h>

void socketHandler(int );

int main(int argc , char *argv[])
{
    int sockfd, newsockfd, portno, clientlen, n,pid;
    struct sockaddr_in serv_addr , cli_addr;
    
    if(argc <2)
    {
        printf("Error , Port number not provided\n");
        exit(0);
    }
    
    sockfd = socket(PF_INET,SOCK_STREAM,0);
    if(sockfd<0)
    {
        printf("Error opening socket,\n");
        exit(0);
    }

    bzero((char *) &serv_addr , sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    
    if(bind(sockfd,(struct sockaddr *) &serv_addr , sizeof(serv_addr)) <0)
    {
        printf("Error binding socket\n");
        exit(0);
    }
    listen(sockfd,5);

    clientlen = sizeof(cli_addr);
    while(1){
    newsockfd = accept(sockfd,(struct sockaddr*) &cli_addr , &clientlen);
    pid = fork();
    if (pid == 0)
    {
        close(sockfd);
        socketHandler(newsockfd);
    }
    else
    {
        close(newsockfd);
    }
    }
    return 0;
}


void socketHandler(int sock)
{
    int n;
    char temp[64];
    char buffer[64];
    char buf[64];
    int i = 0;
    int check = 0;
    FILE *fp;

    printf("Connection accepted\n");
    do{
    n = read(sock,buffer,63);
    if (n < 0)
        printf("Error with reading\n");
    
    strncpy(temp,buffer,strlen(buffer));
    temp[strlen(buffer)-1]='\0';
        fp = fopen(temp,"r");
        if(fp == NULL)
        {
            check = 1;
            n = write(sock,&check,sizeof(check));
            n = read(sock,&check,sizeof(check));
        }
        else
        {
            check = 0;
            n = write(sock,&check,sizeof(check));
            n = read(sock,&check,sizeof(check));
        }
    }while(check==1);
    
    bzero(buf,64);
    do{
    fread((char *) &buf,sizeof(char),63,fp);
    n = write(sock,buf,63);
    if(n < 0)
        printf("Error with writing\n");
    bzero(buf,64);
    n = read(sock,&i,sizeof(i));
    if (n < 0)
        printf("Error with reading\n");
    }while(!feof(fp));
    n = write(sock,"\0",1);
    if (n < 0)
        printf("Error with writing\n");
    fclose(fp);
    exit(0);

}