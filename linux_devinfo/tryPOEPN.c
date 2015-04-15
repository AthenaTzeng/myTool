#include <stdio.h>
#include <string.h>

int main () {
  FILE* fp = NULL;
  char pipe_buffer[60] = {0x0, };
  char* str_need;
  int i = 0;
  int ff = 0, sum = 0;

  if( (fp=popen("iostat -c", "r"))==NULL ) {
    printf("Cannot pipe df command.\n");
    return -1;
  }

  while( fgets(pipe_buffer, 60, fp)!=NULL ) {
    printf("line: %s\n", pipe_buffer);
    if( !i ) {
      if( strstr(pipe_buffer, "cpu") ) i = 1; 
    } else {
      str_need = strtok(pipe_buffer, " \t\n");
      for(;i<7; str_need=strtok(NULL, " \t\n")) {
        ff = atoi(str_need)*100;
        if( ff < 1000 ) ff += atoi(str_need+2);
        else ff += atoi(str_need+3);
        sum += ff;
        printf("piece:\"%s\", %d, %d\n", str_need, ff, sum);
        i++;
      }
      break;
    }
  }

  pclose(fp); 
  return 0;
}

