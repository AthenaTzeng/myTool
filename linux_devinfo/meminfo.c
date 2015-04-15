#include <string.h>
#include <stdio.h>

#define READ_BUFF_SIZE 64

int main () {
    FILE* fp = NULL;
    char buffer[READ_BUFF_SIZE];
    char* p_str = NULL;
    int memTotal = 0;
    int k_flag = 0; // if kB

    if( (fp=fopen("/proc/meminfo", "r"))==NULL ) {
        printf("(ERR) Cannot read meminfo for dev_get_mem_cap\n");
        return -1;
    }

    while( fgets(buffer, READ_BUFF_SIZE, fp)!=NULL ) {
        if( strncmp(buffer, "MemTotal", 8)!=0 ) continue;
        fclose(fp);
        p_str = strchr(buffer, ':')+1;
        while( *p_str!='\0' ) {
            if( *p_str=='k' )
                k_flag = 1;
            else if( *p_str<'0' || *p_str>'9' ) ;
            else
                memTotal = (memTotal*10) + ((*p_str)-'0');
            p_str++;
        }
        if( k_flag ) memTotal = memTotal / 1024;
        printf("result: %d\n", memTotal);
        return 0;
    }

    printf("Cannot find mem info in /proc/meminfo\n");
    fclose(fp);
    return -1;
}

