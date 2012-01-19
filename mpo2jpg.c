/*
    Cut a MPO file (no mater how many views it contain) into "VIEW_NUM" JPEG files
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define MARKER_SIZE 4 // the marker size is the JPEG 0xff?? and the size of it, total 4 bytes
#define MP_INDEX_IFD 12
#define MP_INDEX_TAG 2
#define MP_ENTRY_SIZE 16
#define MP_ENTRY_ATTRIBUTE 4
#define MP_ENTRY_OFFSET 4
#define MP_ENTRY_IMG 4

#define BUFF_MAX_SIZE 2048
#define VIEW_NUM 2

int main (int argc, char* argv[]) {
  int inFile, outFile[VIEW_NUM];
  unsigned long jump_size=0, imgSize[VIEW_NUM];
  unsigned char inBuff[BUFF_MAX_SIZE], IFD_tag[2];
  //char* outName[2];
  int mpe_flag=0, num_img=0, file_count=0, read_size=0, ctr_1=0;
  int endianBig;  // 1: big endian(0x4e 4e 00 2a), 0: little endian(0x49 49 2a 00)
  
  //TODO: the code do not check if the file do not contain the MP Extensions!
  //outName[0] = "./dscf3957_1.jpg";
  //outName[1] = "./dscf3957_2.jpg";
  
  // initial the MPO file for read
  if( (inFile=open(argv[1], O_RDONLY))<=0 ) {
    printf("Cannot open the file: %s\n", argv[1]);
    return -1;
  }
  lseek(inFile, 2, SEEK_SET); // skip SOI (0xffd8)
	
  // find the MP Extensions marker in JPEG
  while( !mpe_flag ) {
	if( read(inFile, inBuff, MARKER_SIZE)!=MARKER_SIZE ) {
	  printf("Cannot read marker!!\n");
	  return -1;
	}
    if( inBuff[0]!=0xff ) {
      printf("Wrong file format: %s\n", argv[1]);
      return -1;
	}
	
	jump_size = 256*inBuff[2] + inBuff[3] - 2;
	if( inBuff[1]!=0xe2 ) // not APP2
	  lseek(inFile, jump_size, SEEK_CUR); // jump to find the next marker
	else {
	  if( read(inFile, inBuff, MARKER_SIZE)!=MARKER_SIZE ) {
	    printf("Cannot read marker!!\n");
		return -1;
	  }
	  if( inBuff[0]=='M' && inBuff[1]=='P' && inBuff[2]=='F' && inBuff[3]==0x00 ) // find MP Extensions
	    mpe_flag = 1;
	  else
	    lseek(inFile, jump_size-MARKER_SIZE, SEEK_CUR); // jump to find the next marker
	}
  }
	
  //get the endian infomation
  if( read(inFile, inBuff, MARKER_SIZE)!=MARKER_SIZE ) {
	printf("Cannot read marker!!\n");
	return -1;
  }
  endianBig = ( inBuff[0]==0x49 ) ? 0 : 1;

  //TODO: check if the offset to the first IFD is 8
  //Current: no check, just skip it
  if( read(inFile, inBuff, MARKER_SIZE)!=MARKER_SIZE ) {
	printf("Cannot read marker!!\n");
	return -1;
  }
 
  // find the number of Images in this MP file
  lseek(inFile, MP_INDEX_TAG, SEEK_CUR); // skip the count (2Bytes) in MP Index IFD
  while( 1 ) {
    if( read(inFile, inBuff, MP_INDEX_TAG)!=MP_INDEX_TAG ) {
	  printf("Cannot read marker!!\n");
	  return -1;
    }
	if( endianBig ) {
	  IFD_tag[0] = inBuff[0];
	  IFD_tag[1] = inBuff[1];
	}
	else { // littel endian
	  IFD_tag[0] = inBuff[1];
	  IFD_tag[1] = inBuff[0];
	}
	
    if( IFD_tag[0]!=0xb0 ) { // offset of Next IFD
	  lseek(inFile, MP_INDEX_TAG, SEEK_CUR);
	  break; // jump out of while loop
	}
	else { // MP Index IFD Tags
	  if( IFD_tag[1]!=0x01 ) // not the number of Images MP Index IFD
	    lseek(inFile, MP_INDEX_IFD-MP_INDEX_TAG, SEEK_CUR);
	  else { // NumberOfImg
        lseek(inFile, 3*MP_INDEX_TAG, SEEK_CUR);
	    if( read(inFile, inBuff, MARKER_SIZE)!=MARKER_SIZE ) {
	      printf("Cannot read marker!!\n");
	      return -1;
        }
		if( endianBig )  num_img = (((inBuff[0]*256+inBuff[1])*256)+inBuff[2])*256+inBuff[3];
		else             num_img = (((inBuff[3]*256+inBuff[2])*256)+inBuff[1])*256+inBuff[0];
	  }
	}
  }

  if( num_img<VIEW_NUM ) {
    printf("not enough imgs for 3D processing, STOP!\n");
	return -1;
  }

  // get the size of each imgs
  for(ctr_1=0; ctr_1<VIEW_NUM; ctr_1++) {
    if( read(inFile, inBuff, MP_ENTRY_SIZE)!=MP_ENTRY_SIZE ) {
	  printf("Cannot read MP entry number %d\n", ctr_1);
	  return -1;
    }
	if( endianBig ) imgSize[ctr_1] = (((inBuff[4]*256+inBuff[5])*256)+inBuff[6])*256+inBuff[7];
	else            imgSize[ctr_1] = (((inBuff[7]*256+inBuff[6])*256)+inBuff[5])*256+inBuff[4];
  }
  
  // write img files
  // initial the MPO file for read
  // TODO: open file inside the for loop!!!!!
  if( (outFile[0]=open("./outFile_0.jpg", O_RDWR|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR) ) <= 0 ) {
    printf("Cannot open the file: %s\n", "./outFile_0.jpg");
    return -1;
  }
  if( (outFile[1]=open("./outFile_1.jpg", O_RDWR|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR) ) <= 0 ) {
    printf("Cannot open the file: %s\n", "./outFile_0.jpg");
    return -1;
  }

  lseek(inFile, 0, SEEK_SET); // lseek to the begin of inFile
  for(ctr_1=0; ctr_1<VIEW_NUM; ctr_1++) {
    while( imgSize[ctr_1]>0 ) {
	  read_size = imgSize[ctr_1]>BUFF_MAX_SIZE ? BUFF_MAX_SIZE : imgSize[ctr_1];
	    if( read(inFile, inBuff, read_size)!=read_size ) {
	      printf("Cannot read MP entry number %d\n", ctr_1);
	      return -1;
		}
		if( write(outFile[ctr_1], inBuff, read_size)!=read_size ) {
          printf("writhe L file error!\n");
		  return -1;
		}
		imgSize[ctr_1] -= read_size;
    }
  }

  close(outFile[0]);
  close(outFile[1]);
  close(inFile);
}
