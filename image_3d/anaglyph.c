/*
     input L and R BMP and output a anaglyph bmp (red-cyan) 
     BIG Endden & 54 bytes HEADER BMP
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "bmpDEF.h"

#define BUFF_SIZE 3000 

int main () {
  unsigned char buffL[BUFF_SIZE], buffR[BUFF_SIZE], pixel[3]; // pixel[B, G, R]
  int leftFile, rightFile, anaglyphFile;
  int i, pedding, read_size, imgW = 0, imgH = 0;

// open MPO file, initial L and R files, reset to empty
  if( (leftFile = open("./bL_DSCF3957.bmp", O_RDONLY) ) <= 0 )  printf("open left file err!\n");
  if( (rightFile = open("./bR_DSCF3957.bmp", O_RDONLY) ) <= 0 )  printf("open right file err!\n");
  if( (anaglyphFile = open("./an_DSCF3957.bmp", O_RDWR|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR) )<=0 )  printf("open anaglyph err!\n");

  // read BMP header, get W, H 
  lseek(leftFile, 0, SEEK_SET);
  if( read(leftFile, buffL, HEAD_SIZE) != HEAD_SIZE ) printf("read head err!\n");
  for(i=0; i<4; i++) {
    imgW = ( imgW * 256 ) + (int)buffL[21-i];
    imgH = ( imgH * 256 ) + (int)buffL[25-i];
  }
  printf("file W and H are: %d and %d.\n", imgW, imgH);
  // each line must %4 = 0
  pedding = (imgW*3)%4;
  if( pedding ) pedding = 4 - pedding;
  // write BMP Header to anaglyphFile
  lseek(anaglyphFile, 0, SEEK_SET);
  if( write(anaglyphFile, buffL, HEAD_SIZE) != HEAD_SIZE ) printf("write err!\n");
  // ignore the right file header
  lseek(rightFile, 54, SEEK_SET);

  // Ba = Br
  // Ga = Gr
  // Ra = (0.7*Gl + 0.3*Bl) [[ ^(1/gamma) ]]
  while( (read_size = read(leftFile, buffL, imgW*3)) > 0 ) {
    if( read(rightFile, buffR, read_size) != read_size ) printf("L R files are not match!\n");
    for(i=0; i<read_size; i+=3) {
      pixel[0] = buffR[i];
      pixel[1] = buffR[i+1];
      pixel[2] = (unsigned char)(0.3*buffL[i] + 0.7*buffL[i+1]);
      if( write(anaglyphFile, pixel, 3) != 3 ) printf("write err!\n");
    }
    if( pedding ) {
      lseek(leftFile, pedding, SEEK_CUR);
      lseek(rightFile, pedding, SEEK_CUR);
      pixel[0] = 0;
      pixel[1] = 0;
      pixel[2] = 0;
      write(anaglyphFile, pixel, pedding);
    }
  }
  printf("%x\n", leftFile);
  printf("%x\n", rightFile);

  printf("finish data writing!\n");

// close files
  if( leftFile > 0 ) close(leftFile);
  if( rightFile > 0 ) close(rightFile);
  if( anaglyphFile > 0 ) close(anaglyphFile);
}

