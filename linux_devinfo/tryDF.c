#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>

#define READ_BUFF_SIZE 64 
int main () {
    FILE* fp = NULL;
    char buffer[READ_BUFF_SIZE];
    char* p_str = NULL;
    int memTotal = 0;
    int flag_find = 0; 

    system("iostat > ./out_file");

    if( (fp=fopen("./out_file", "r"))==NULL ) {
        printf("(ERR) Cannot read meminfo for dev_get_mem_cap\n");
        return -1;
    }

    while( fgets(buffer, READ_BUFF_SIZE, fp)!=NULL ) {
        if( strstr(buffer, "avg-cpu")==NULL ) continue;
        flag_find = 1;
        break;
    }
    fclose(fp);
    fp = NULL;

    if( !flag_find ) {
        printf("Cannot find the rootfs size by command 'df.'\n");
        return -1;
    }
   
    p_str = buffer + 8;
#if 0
    while( *p_str!='\0' ) {
        if( *p_str<'0' || *p_str>'9' ) {
            if( memTotal!=0 ) break;
        }
        else
            memTotal = (memTotal*10) + ((*p_str)-'0');
        p_str++;
    }
#else
    memTotal = atoi(p_str);
    printf("result: %d, %s\n", memTotal, p_str);
    memTotal = atoi(buffer);
    printf("result: %d, %s\n", memTotal, buffer);
#endif
    printf("result: %d\n", memTotal);

memTotal = 0;
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

