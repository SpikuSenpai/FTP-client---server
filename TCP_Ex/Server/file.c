#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
    char *file = argv[1];
    FILE *fp = fopen(file,"rb");
    char buffer[64];
    do
    {
    fread((char *) &buffer,sizeof(char),63,fp);
    printf("%s",buffer);
    bzero(&buffer,sizeof(buffer));
    }while(!feof(fp));
    fclose(fp);
    return 0;
}
