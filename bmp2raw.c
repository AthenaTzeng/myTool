#include <sys/ioctl.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/mman.h>
#include <linux/fb.h>
#include <math.h>

typedef struct BMPHeader {
	long int bfOffBits;
	long int biSize;
	long int biWidth;
	long int biHeight;
	short int biPlanes;
	short int biBitCount;
	long int biCompression;
	long int biSizeImage;
	long int biXPelsPerMeter;
	long int biYPelsPerMeter;
	long int biClrUsed;
	long int biClrImportant;
} BMPHeader;

typedef struct RawDecoder {
	struct BMPHeader BMP;
	struct fb_var_screeninfo vinfo;
	int x, y, w, h, bpp; //where to put raw data
} RawDecoder;

int save2File(char *filename, char *buf, int size) {
	FILE *out = fopen(filename, "w");
	fwrite( buf, size, 1, out);
	fclose(out);
}

short int getColors(int c, int b) {
	int to_max = pow(2,b);// 2^b
	int c_max = 256; // 2^8
	if(c>255) c=255;
	else if(c<0) c=0;
	int shift = 8-b;
	short int r = (0xff>>shift) & (c*to_max/c_max);
	return r;
}

int readBMPHeader(FILE *fd, struct BMPHeader *h) {
    char ftype[2];
    if(fseek(fd, 0, SEEK_SET)) {
	//printf("fd fail\n");
	return 1;
    }
    fread(ftype, 2, 1, fd);
    if( strncmp(ftype, "BM", 2) != 0 ) {;
	//printf("%s: not bmp header\n", ftype);
	return 1;
    }
    fseek(fd, 10, SEEK_SET);
    fread(h, sizeof(struct BMPHeader), 1, fd);
    return 0;
}

int read24Pixel(FILE *fd, struct BMPHeader header, char *buffer) {
	char *bitmap;
	if(buffer==NULL) {
		//printf("BMP Decoder need buffer\n");
		return 1;
	} else
		bitmap = buffer;

	if(fseek(fd, header.bfOffBits, SEEK_SET)) {
		//printf("fail to read bmp data");
		return 1;
	}

	int x,y, bpp = 0, pedding = 0, offset = 0;
	//char bpp[3]; // we only convert 24 bit to raw
	// a lines must be a multiple of 4 bytes 
	pedding = (header.biWidth * 3) % 4;
	if(pedding)
		pedding = 4 - pedding;

	int line = (header.biWidth * 3) + pedding;
	char *linebuffer = malloc(line);
	for(y=0;y<header.biHeight;y++) {
		if(fread(&bpp, 1, line, fd)) {
			//printf("no enought data to read\n");
			return 1;
		} else {
			memcpy(bitmap, linebuffer, line-pedding);
			bitmap+=line-pedding;
		}//24 bpp
	}

	return 0;
}

int read16Pixel(FILE *fd, struct BMPHeader header, char *buffer) {
	short int *bitmap = NULL;	
	if(buffer==NULL) {
		//printf("BMP Decoder need buffer\n");
		return 1;
	} else
		bitmap = (short int *)buffer;

	if(fseek(fd, header.bfOffBits, SEEK_SET)) {
		//printf("fail to read bmp data");
		return 1;
	}

	int x,y, bpp = 0, pedding = 0, offset = 0;
	//char bpp[3]; // we only convert 24 bit to raw
	// a lines must be a multiple of 4 bytes 
	pedding = (header.biWidth * 3) % 4;
	if(pedding)
		pedding = 4 - pedding;

	for(y=0;y<header.biHeight;y++) {
		for(x=0;x<header.biWidth;x++) {
			if(fread(&bpp, 1, 3, fd)!=3) {
				//printf("no enought data to read\n");
				return 1;
			} //24 bpp
			//bitmap[offset++] = ((bpp & 0xff0000 ) >> 19 )<< 11 | ((bpp & 0xff00) >> 11) << 5 | ((bpp & 0xff) >>3);
			bitmap[offset++] = ( getColors((bpp & 0xff0000) >> 16 , 5)<<11 ) | ( getColors((bpp & 0xff00) >> 8, 6)<<5 ) | ( getColors(bpp & 0xff, 5) );
		}
		if(pedding)
			fread(&bpp, pedding, 1, fd);
	}

	return 0;
}

int readFBInfo(struct fb_var_screeninfo *vinfo) {
	int fhandle = open("/dev/fb0", O_RDWR);
	if (ioctl(fhandle, FBIOGET_VSCREENINFO, vinfo))
	{
		perror("fb_ioctl");
		close(fhandle);
		return 1;
	}
	return 0;
}

char *setFBMapping(struct fb_var_screeninfo vinfo) {
	int fhandle = open("/dev/fb0", O_RDWR);
	int iSize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel/8;
	char *fb = (char *)mmap(0, iSize, PROT_READ | PROT_WRITE, MAP_SHARED, fhandle, 0);
	close(fhandle);
	return fb;
}

int main(int argc, char **argv) {
	FILE *fd = fopen(argv[1], "r");
	if(fd==NULL) {	
	    //printf("Open BMP File fail\n");
	    return 1;
	}

	struct RawDecoder raw;
	bzero(&raw, sizeof(raw));

	if(readBMPHeader(fd, &raw.BMP)) {
	    //printf("Read BMP Header fail\n");
	    fclose(fd);
	    return 1;
	}

	if(readFBInfo(&raw.vinfo)) {
	    fclose(fd);
	    return 1;
	}

	printf("BMP Header:\n");
	//printf("Bitmap offset: %ld\n", bmpH.bfOffBits);
	//printf("Bitmap width: %ld\n", bmpH.biWidth);
	//printf("Bitmap height: %ld\n", bmpH.biHeight);
	//printf("Bits per pixel: %d\n", bmpH.biBitCount);
	//printf("Size of image: %ld\n", bmpH.biSizeImage);
	//printf("XPelsPerMeter: %ld\n", bmpH.biXPelsPerMeter);
	//printf("YPelsPerMeter: %ld\n", bmpH.biYPelsPerMeter);
	//printf("Colours used: %ld\n", bmpH.biClrUsed);
	//printf("Important colours: %ld\n", bmpH.biClrImportant);

	// prepare to read bmp data
	//char *bitmap = malloc(2 * raw.BMP.biWidth * raw.BMP.biHeight);
	char *bitmap = setFBMapping(raw.vinfo);
	switch(raw.vinfo.bits_per_pixel) {
		case 16:
			if(read16Pixel(fd, raw.BMP, bitmap)) {
				fclose(fd);
				return 1;
			}
			break;
		case 24:
			if(read24Pixel(fd, raw.BMP, bitmap)) {
				fclose(fd);
				return 1;
			}
			break;
		default:
			printf("Not support bpp\n");
	}
	
	//save2File("test.raw", bitmap, 2 * bmpH.biWidth * bmpH.biHeight);
	
	fclose(fd);
printf("\nafter close file\n");
	return 0;
} 


