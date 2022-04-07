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
    char group[16] = "224.0.0.88";
    char group1[16] = "224.0.0.66";
    if(fd < 0){
        perror("socket failed!");
        return 2;
    }

    struct sockaddr_in localAddr = {0};
    localAddr.sin_family = AF_INET;
    localAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    localAddr.sin_port = htons(8000);

    int ret = bind(fd,(struct sockaddr *)&localAddr, sizeof(localAddr));
    if(ret < 0){
        perror("bind failed !");
        return 3;
    }

    struct ip_mreq mreq;
    mreq.imr_multiaddr.s_addr = inet_addr(group);
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);

    ret = setsockopt(fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq));
    if(ret < 0){
        perror("setsockopt failed !");
        return 3;
    }
    mreq.imr_multiaddr.s_addr = inet_addr(group1);
    ret = setsockopt(fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq));
    if(ret < 0){
        perror("setsockopt failed !");
        return 3;
    }

    char buf[1024];
    int length = 0 ;
    struct sockaddr_in sender;
    int senderLen = sizeof(sender);
    while(1){
        memset(buf, 0, sizeof(buf));
        length = recvfrom(fd, buf,sizeof(buf), 0, (struct sockaddr *)&sender,&senderLen);
        buf[length] = 0;
        printf("%s %d:%s\n",inet_ntoa(sender.sin_addr), ntohs(sender.sin_port), buf);
    }
    setsockopt(fd, IPPROTO_IP, IP_DROP_MEMBERSHIP, &mreq, sizeof(mreq));
    close(fd);
    return 0;
}