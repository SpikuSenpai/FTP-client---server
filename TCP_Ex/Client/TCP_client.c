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
    int sockfd ,portno ,n ,check;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[64];
    char temp[64];

    if(argc <3)
    {
        printf("ussage %s hostname port",argv[0]);
        exit(0);
    }
    portno = atoi(argv[1]);

    sockfd = socket(PF_INET,SOCK_STREAM,0);
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

    if(connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr))<0)
    {
        printf("Error connecting to server\n");
        exit(0);
    }

    printf("Give a filename :");
    do{
    fgets(buffer,63,stdin);
    n = write(sockfd,buffer,63);

    n = read(sockfd,&check,sizeof(check));
    if(check==1)
        printf("File not found\nSpecify a file that exists :");
    n = write(sockfd,&check,sizeof(check));
    }while(check==1);

    strncpy(temp,buffer,strlen(buffer));
    temp[strlen(buffer)-1]='\0';    
    FILE *fp = fopen(temp,"w");
    if(fp == NULL)
        printf("Error opening file\n");
    n = write(sockfd,buffer,63);

    bzero((char *) &buffer,64);
    int i = 0;
    
    do{
    n = read(sockfd,buffer,63);
    fwrite(buffer,sizeof(char),strlen(buffer),fp);
    write(sockfd,&i,sizeof(i));
    }while(strcmp(buffer,"\0")!=0);
    close(sockfd);
    fclose(fp);

    return 0;

}

