#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

char * output_file = "a.exe";
char * entry_point = "0x700000";
char * input_file;
int main(int argc,char ** argv){
  for(int i = 1;i<argc;i++){
    if(strcmp(argv[i],"-o")==0){
      output_file = argv[i+1];
      i++;
    }
    else if(strcmp(argv[i],"-e")==0){
     entry_point = argv[i+1];
     i++;
    }
    else input_file = argv[i];
  }
  char cmd_line[1000] = "gcc -m32 -c -std=gnu99 -O0 -nostdlib -fno-exceptions  -mno-red-zone -pipe ";
  strcat(cmd_line,input_file);
  strcat(cmd_line," -o temp.o&&");
  strcat(cmd_line,"ld -melf_i386 *.o -o ");
  strcat(cmd_line,output_file);
  strcat(cmd_line," -Ttext ");
  strcat(cmd_line,entry_point);
  printf(cmd_line);
  system(cmd_line);
}
