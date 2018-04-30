#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <string.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc , char *argv[])
{
    int sockfd, newsockfd, portno, clientlen, n,pid,check,i=0;
    struct sockaddr_in serv_addr , cli_addr;
    socklen_t addrlen = sizeof(cli_addr);
    FILE *fp;
    char temp[64];
    char buffer[64];
    char buf[64];
    if(argc <2)
    {
        printf("Error , Port number not provided\n");
        exit(0);
    }
    
    sockfd = socket(AF_INET,SOCK_DGRAM,0);
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

   
    clientlen = sizeof(cli_addr);
    recvfrom(sockfd,&check,sizeof(check),0,(struct sockaddr*) &cli_addr,&addrlen);
    printf("Connection accepted\n");
    do{
    n = recvfrom(sockfd,buffer,63,0,(struct sockaddr*)&cli_addr,&addrlen);
    if (n < 0)
        printf("Error with reading\n");
    
    strncpy(temp,buffer,strlen(buffer));
    temp[strlen(buffer)-1]='\0';
        fp = fopen(temp,"r");
        if(fp == NULL)
        {
            check = 1;
            n = sendto(sockfd,&check,sizeof(check),0,(struct sockaddr *) &cli_addr,addrlen);
            n = recvfrom(sockfd,&check,sizeof(check),0,(struct sockaddr*)&cli_addr,&addrlen);
        }
        else
        {
            check = 0;
            n = sendto(sockfd,&check,sizeof(check),0,(struct sockaddr *) &cli_addr,addrlen);
            n = recvfrom(sockfd,&check,sizeof(check),0,(struct sockaddr*)&cli_addr,&addrlen);
        }
    }while(check==1);
    
    bzero(buf,64);
    do{
    fread((char *) &buf,sizeof(char),63,fp);
    n = sendto(sockfd,buf,63,0,(struct sockaddr *) &cli_addr,addrlen);
    if(n < 0)
        printf("Error with writing\n");
    bzero(buf,64);
    n = recvfrom(sockfd,&i,sizeof(i),0,(struct sockaddr*)&cli_addr,&addrlen);
    if (n < 0)
        printf("Error with reading\n");
    }while(!feof(fp));
    n = sendto(sockfd,"\0",1,0,(struct sockaddr *) &cli_addr,addrlen);
    if (n < 0)
        printf("Error with writing\n");
    fclose(fp);
    return 0;
}
