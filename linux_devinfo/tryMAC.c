#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

int main () {
    unsigned char mac_add[6] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    int cnt_i = 0;
    int cnt_j = 0;
    int fd = 0;
    char buffer[17], tmp;

    // try if we can get any MAC in this four interface
    if( (fd=open("/sys/class/net/br0/address", O_RDONLY))<=0 ) {
        if( (fd=open("/sys/class/net/eth0/address", O_RDONLY))<=0 ) {
            if( (fd=open("/sys/class/net/lte0pdn0/address", O_RDONLY))<=0 ) {
                if( (fd=open("/sys/class/net/wimax/address", O_RDONLY))<=0 ) {
                    printf("(ERR) No MAC address for generate UID. Stop!\n");
                    return -1;
                }
            }
        }
    }

    read(fd, buffer, 17); // read by line and if not \0
buffer[15] = 'A';
buffer[16] = 'B';
buffer[3] = 'f';
    for(cnt_i=0; cnt_i<6; cnt_i++) {
        cnt_j = cnt_i*3;
        if( buffer[cnt_j] < 'A' ) mac_add[cnt_i] = (buffer[cnt_j]-'0')*16;
        else if( buffer[cnt_j] < 'a' ) mac_add[cnt_i] = (buffer[cnt_j]-'A'+10)*16;
        else mac_add[cnt_i] = (buffer[cnt_j]-'a'+10)*16; 

        cnt_j++;
        if( buffer[cnt_j] < 'A' ) mac_add[cnt_i] += (buffer[cnt_j]-'0');
        else if( buffer[cnt_j] < 'a' ) mac_add[cnt_i] += (buffer[cnt_j]-'A'+10);
        else mac_add[cnt_i] += (buffer[cnt_j]-'a'+10); 
    }

    printf("ma_add %17s \n", buffer);
    for( cnt_i=0; cnt_i<6; cnt_i++ )
        printf(" %02x", mac_add[cnt_i]);
    puts("\n");

    close(fd);
    return 0;
}

