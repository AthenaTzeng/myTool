#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define READ_BUFF_SIZE 128
#define NETDEV_FILE "/proc/net/dev"
#define CHECK_INTERVAL 3

unsigned long long cnt_TxB = 0, cnt_RxB = 0;
unsigned long long cnt_TxPack = 0, cnt_RxPack = 0;
unsigned long long cnt_TxP_err = 0, cnt_RxP_err = 0;
unsigned long long cnt_TxF_err = 0, cnt_RxF_err = 0;
time_t update_time = 0;

int main () {
    FILE* fp = NULL;
    char* p_str = NULL;
    char buff[READ_BUFF_SIZE];
    unsigned long long cnt_arr[2][8];
    int cnt_i, cnt_j;
    time_t current_time = 0;
int i;
    char* ptr_test = "18446744073709551615"; // for test only
for(i=0;i<5;i++){
sleep(1);
    current_time = time(NULL);
    if( (current_time-update_time) < CHECK_INTERVAL ) 
        printf("won't update cur=%d, up=%d\n", current_time, update_time); // won't update/read info
    else {  // read info from NETDEV_FILE
        if( (fp=fopen(NETDEV_FILE, "r"))==NULL ) {
            printf("(ERR) Cannot read cpuinfo for dev_get_model\n");
            return -1;
        }

        while( fgets(buff, READ_BUFF_SIZE, fp)!=NULL ) {
            if( strstr(buff, "eth0:")==NULL ) continue;
            printf("line: %s", buff);
            break; // get Receiveing data
        }
        if( fp!=NULL ) {
            fclose(fp);
            fp = NULL;
        }

        p_str = strtok(buff, " \t\n");
        for( cnt_i=0; cnt_i<2; cnt_i++ ) {
            for( cnt_j=0; cnt_j<8; cnt_j++ ) {
                cnt_arr[cnt_i][cnt_j] = 0;
                p_str = strtok(NULL, " \t\n");
                if( cnt_j == 6 ) p_str = ptr_test; // for test long long only
                while( *p_str!='\0' ) {
                    cnt_arr[cnt_i][cnt_j] = (cnt_arr[cnt_i][cnt_j] * 10) + (*p_str-'0');
                    p_str++;
                }
            }
        }
        cnt_TxB = cnt_arr[1][0];
        cnt_RxB = cnt_arr[0][0];
        cnt_TxPack = cnt_arr[1][1]; 
        cnt_RxPack = cnt_arr[0][1];
        cnt_TxP_err = cnt_arr[1][2] + cnt_arr[1][3];
        cnt_RxP_err = cnt_arr[0][2] + cnt_arr[0][3];
        cnt_TxF_err = cnt_arr[1][5];
        cnt_RxF_err = cnt_arr[0][5];
        update_time = time(NULL);

        for( cnt_i=0; cnt_i<2; cnt_i++ ) {
            for( cnt_j=0; cnt_j<8; cnt_j++ ) {
                printf("get v=%llu\n", cnt_arr[cnt_i][cnt_j]);
            }
            printf("\n");
        }
    }
}
    return 0;
}

