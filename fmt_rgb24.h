#ifndef _FMT_RGB24_
#define _FMT_RGB24_

#include <string.h>

#define RGB_VALID(x) ((x) < 0)? 0 :(((x)>255)? 255: (x)) 
                 
void BAYER_RGB24(unsigned char *dst, unsigned char *src, long int WIDTH,
                 long int HEIGHT);

void YUV420P_RGB24(unsigned char *dst, unsigned char *src, long int WIDTH,
		 long int HEIGHT);

void RGB332_RGB24(unsigned char *dst, unsigned char *src, long int WIDTH,
	  	 long int HEIGHT);

void RGB555_RGB24(unsigned char *dst, unsigned char *src, long int WIDTH,
	  	 long int HEIGHT);

void RGB555X_RGB24(unsigned char *dst, unsigned char *src, long int WIDTH,
	  	 long int HEIGHT);

void RGB565_RGB24(unsigned char *dst, unsigned char *src, long int WIDTH,
	  	 long int HEIGHT);

void RGB565X_RGB24(unsigned char *dst, unsigned char *src, long int WIDTH,
	  	 long int HEIGHT);

void RGB24_RGB24(unsigned char *dst, unsigned char *src, long int WIDTH,
                 long int HEIGHT);
                 
void BGR24_RGB24(unsigned char *dst, unsigned char *src, long int WIDTH,
	  	 long int HEIGHT);

void RGB32_RGB24(unsigned char *dst, unsigned char *src, long int WIDTH,
	  	 long int HEIGHT);

void BGR32_RGB24(unsigned char *dst, unsigned char *src, long int WIDTH,
	  	 long int HEIGHT);


#endif
