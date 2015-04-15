#include <sys/ioctl.h>
#include <net/if.h> 
//#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#define MAC_LEN 6 // max is 14 for struct sockaddr.(char*)sa_data

int main(int argc, char* argv[]) {
    struct ifreq ifr;
    int sock_fd, i;

    if( argc!=2 ) {
        printf("USAGE: input interface name as index.\n");
        return -1;
    }

    /* we need a socket fd to operate ioctl */
    /* family and type are not important here */
    sock_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
    if( sock_fd==-1 ) {
        printf("(ERR) Cannot setup socket: %s", strerror(errno));
        return errno;
    }

    strcpy(ifr.ifr_name, argv[1]);

    if( 0==ioctl(sock_fd, SIOCGIFHWADDR, &ifr) ) {
        if( sock_fd ) close(sock_fd);
        printf("ifr_ifindex of %s : %d\n", argv[1], ifr.ifr_ifindex);
        for (i = 0; i < MAC_LEN; ++i)
            printf(" %02x", (unsigned char) ifr.ifr_addr.sa_data[i]);
        puts("\n");
        if( sock_fd ) close(sock_fd);
        return 0;
    } else {
        printf("(ERR) Cannot get info of %s: %s\n", argv[1], strerror(errno));
        if( sock_fd ) close(sock_fd);
        return errno;
    }

    printf("(ERR) Should never be here, bye!\n ");
    return 1;
}

