#include <string.h>
#include <stdio.h>

#define READ_BUFF_SIZE 64

int main () {
    FILE* fp = NULL;
    char buffer[READ_BUFF_SIZE];
    char* p_str = NULL;

    if( (fp=fopen("/proc/cpuinfo", "r"))==NULL ) {
        printf("(ERR) Cannot read cpuinfo for dev_get_model\n");
        return -1;
    }

    while( fgets(buffer, READ_BUFF_SIZE, fp)!=NULL ) {
        if( strncmp(buffer, "model name", 10)!=0 ) continue;
        p_str = strchr(buffer, ':')+2;
        printf("%s\n", p_str);
            
        fclose(fp);
        return 0;
    }

    printf("Cannot find model name in /proc/cpuinfo\n");
    fclose(fp);
    return -1;
}

