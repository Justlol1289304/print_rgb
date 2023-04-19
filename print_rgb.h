#ifndef _PRINT_RGB_H
#define _PRINT_RGB_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <inttypes.h>

#define MAX_IMAGE_NAME 255
#define HEAD_SIZE_OFF 0x2
#define HEAD_WIDTH_OFF 0x12
#define HEAD_HEIGHT_OFF 0x16
#define HEAD_RAW_SIZE_OFF 0x22

#define RGB(r,g,b) \
	(rgb){r,g,b}

typedef struct{
	uint8_t r,g,b;
} rgb;


uint8_t _bmp_header[54]= {
	'B','M',	//signature
	0,0,0,0,	//size total
	0,0,		//unused
	0,0,		//unused
	0x36,0,0,0,	//pixel data offset bytes (from after here)
	0x28,0,0,0,	//size of DIB header (from here)
	0,0,0,0,	//width pixels
	0,0,0,0,	//height pixels
	0x1, 0,		//num planes
	0x18, 0, 	//bits per pixel (24)
	0,0,0,0,	//BI_RGB compression (none)
	0,0,0,0,	//size of raw bitmap data inc. padding
	0x13, 0xB, 0,0,	//print res. horizontal
	0x13, 0xB, 0,0, //print res. vertical
	0,0,0,0,	//colors in palette
	0,0,0,0		//important colors
};
static const uint8_t _zero_pad[4];

void print_rgb(void *data, uint32_t w, uint32_t h, char *file_name){
	uint32_t w_pad = w%4, raw_bmp_size = w*(3*h + w_pad), bmp_size = raw_bmp_size + sizeof(_bmp_header);
	if(strlen(file_name) > MAX_IMAGE_NAME){
		fprintf(stderr, "Abort: image name is too large.\n");
		abort();
	}
	memcpy(_bmp_header+HEAD_SIZE_OFF, &bmp_size, sizeof(uint32_t));
	memcpy(_bmp_header+HEAD_WIDTH_OFF, &w, sizeof(uint32_t));
	memcpy(_bmp_header+HEAD_HEIGHT_OFF, &h, sizeof(uint32_t));
	memcpy(_bmp_header+HEAD_RAW_SIZE_OFF, &raw_bmp_size, sizeof(uint32_t));
	FILE *bmp_file = fopen(file_name, "w+");
	if(!bmp_file){
		fprintf(stderr, "Abort: failed to open file.\n");
		abort();	
	}
	fwrite(_bmp_header, sizeof(_bmp_header), 1, bmp_file);
	while(h-- > 0){
		uint32_t cur = h * w * 3;
		for(uint32_t i = 0; i < w * 3; i+= 3){
			fwrite(&(rgb){  *(uint8_t*)(data + cur + i + 2),
					*(uint8_t*)(data + cur + i + 1),
					*(uint8_t*)(data + cur + i)}, 
					sizeof(rgb), 1, bmp_file);
		}
		fwrite(_zero_pad, w_pad, 1, bmp_file);
	}
	fclose(bmp_file);
}

#endif //print_rgb.h
