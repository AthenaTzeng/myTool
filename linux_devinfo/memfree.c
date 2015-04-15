#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define READ_BUFF_SIZE 128

int main () {
    FILE* fp = NULL;
    char buffer[READ_BUFF_SIZE];
    char* p_str = NULL;
    int memTotal = 0;
    int k_flag = 0; // if kB

    system("free > out_file");

    if( (fp=fopen("./out_file", "r"))==NULL ) {
        printf("(ERR) Cannot read meminfo for dev_get_mem_cap\n");
        return -1;
    }

    fgets(buffer, READ_BUFF_SIZE, fp);
    fgets(buffer, READ_BUFF_SIZE, fp);
printf("line 1: %s\n", buffer);
    fclose(fp);
   
    p_str = buffer + 5;
    while( *p_str!='\0' ) {   
        if( *p_str<'0' || *p_str>'9' ) {
            if( memTotal!=0 ) break;
        }
        else
            memTotal = (memTotal*10) + ((*p_str)-'0');
        p_str++;
    }
        printf("result: %d\n", memTotal);
    printf("Cannot find mem info in /proc/meminfo\n");
    return -1;
}

