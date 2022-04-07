#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc,char *argv[])
{
    int fd = socket(AF_INET,SOCK_DGRAM,0);
    char groupAddr[16] = "224.0.0.88";
    if(fd < 0){
        perror("socket failed!");
        return 2;
    }

    struct sockaddr_in remoteAddr = {0};
    remoteAddr.sin_family = AF_INET;
    remoteAddr.sin_addr.s_addr = inet_addr(groupAddr);
    remoteAddr.sin_port = htons(8000);

    char buf[1024] = "This is a group udp";
    int length = 0;
    while(1){
        //memset(buf, 0, sizeof(buf));
        length = sendto(fd, buf, strlen(buf), 0, (struct sockaddr *)&remoteAddr,sizeof(remoteAddr));
        printf("Send message:%s\n",buf);
        sleep(5);
    }
    close(fd);
    return 0;
}