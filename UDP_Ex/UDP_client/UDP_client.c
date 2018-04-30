#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include <string.h>
#include <unistd.h>
#include <stdlib.h>



int main(int argc , char *argv[])
{

    struct sockaddr_in serv_addr;
    int sockfd ,portno ,n ,check ;
    socklen_t addrlen = sizeof(serv_addr);
    struct hostent *server;
    char buffer[64];
    char temp[64];

    if(argc <3)
    {
        printf("ussage %s hostname port",argv[0]);
        exit(0);
    }
    portno = atoi(argv[1]);

    sockfd = socket(AF_INET,SOCK_DGRAM,0);
    if(sockfd<0)
    {
        printf("Error opening socket\n");
        exit(0);
    }
    
    server = gethostbyname(argv[2]);
    if (server == NULL)
    {
        printf("ERROR, no such host");
        exit(0);
    }
    

    bzero(serv_addr.sin_zero,sizeof(serv_addr.sin_zero));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);
    bcopy((char *)server->h_addr,(char *)&serv_addr.sin_addr.s_addr,server->h_length);
    
    sendto(sockfd,&check,sizeof(check),0,(struct sockaddr*)&serv_addr,sizeof(serv_addr));
    printf("Give a filename :");
    do{
    fgets(buffer,63,stdin);
    n = sendto(sockfd,buffer,63,0,( struct sockaddr *) &serv_addr,sizeof(serv_addr));

    n = recvfrom(sockfd,&check,sizeof(check),0,( struct sockaddr*) &serv_addr,&addrlen);
    if(check==1)
        printf("File not found\nSpecify a file that exists :");
    n = sendto(sockfd,&check,sizeof(check),0,( struct sockaddr *) &serv_addr,sizeof(serv_addr));
    }while(check==1);

    strncpy(temp,buffer,strlen(buffer));
    temp[strlen(buffer)-1]='\0';    
    FILE *fp = fopen(temp,"w");
    if(fp == NULL)
        printf("Error opening file\n");
    n = sendto(sockfd,buffer,63,0,( struct sockaddr *) &serv_addr,sizeof(serv_addr));

    bzero((char *) &buffer,64);
    int i = 0;
    
    do{
    n = recvfrom(sockfd,buffer,63,0,( struct sockaddr *) &serv_addr,&addrlen);
    fwrite(buffer,sizeof(char),strlen(buffer),fp);
    sendto(sockfd,&i,sizeof(i),0,( struct sockaddr *) &serv_addr,sizeof(serv_addr));
    }while(strcmp(buffer,"\0")!=0);
    close(sockfd);
    fclose(fp);

    return 0;

}

