/*
 * BAYER_RGB24 ROUTINE TAKEN FROM:
 *
 * Sonix SN9C101 based webcam basic I/F routines
 * Copyright (C) 2004 Takafumi Mizuno <taka-qce@ls-a.jp>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
 
#include "fmt_rgb24.h"

void YUV420P_RGB24(unsigned char *dst, unsigned char *src, long int WIDTH, long int HEIGHT)
{
	int i,j;
	unsigned char *y=src;
	unsigned char *u=y + WIDTH * HEIGHT;
	unsigned char *v=u + WIDTH * HEIGHT / 4;
	unsigned char *d = dst;
	unsigned char *us,*vs;

	for(i = 0; HEIGHT > i; i++){

		us = u; vs = v; 
		for(j = 0; HEIGHT > j; j += 2){ // step by 2pixel
			int r, g, b, u0, v0, y0, cr, cg, cb;
			u0 = -128 + *u++;
			v0 = -128 + *v++;

			cr = 718 * v0;
			cg = -176 * u0 - 366 * v0;
			cb = 906 * u0;
			
			y0 = (*y++) << 9;
			r =(y0 + cr)>> 9;
			g =(y0 + cg)>> 9;
			b =(y0 + cb)>> 9;
			*d++ = RGB_VALID(r);
			*d++ = RGB_VALID(g);
			*d++ = RGB_VALID(b);

			y0 =  (*y++) << 9;
			r =(y0 + cr)>> 9;
			g =(y0 + cg)>> 9;
			b =(y0 + cb)>> 9;
			*d++ = RGB_VALID(r);
			*d++ = RGB_VALID(g);
			*d++ = RGB_VALID(b);
		}
		if( (i % 2) ==0){
			u = us; v = vs;
		}
	}
}

void BAYER_RGB24(unsigned char *dst, unsigned char *src, long int WIDTH,
                 long int HEIGHT)
{
        long int i;
        unsigned char *rawpt, *scanpt;
        long int size;

        rawpt = src;
        scanpt = dst;
        size = WIDTH * HEIGHT;

        for (i = 0; i < size; i++) {
                if ((i / WIDTH) % 2 == 0) {
                        if ((i % 2) == 0) {
                                /* B */
                                if ((i > WIDTH) && ((i % WIDTH) > 0)) {
                                        *scanpt++ = (*(rawpt - WIDTH - 1) + *(rawpt - WIDTH + 1) + *(rawpt + WIDTH - 1) + *(rawpt + WIDTH + 1)) / 4;    /* R */
                                        *scanpt++ = (*(rawpt - 1) + *(rawpt + 1) + *(rawpt + WIDTH) + *(rawpt - WIDTH)) / 4;    /* G */
                                        *scanpt++ = *rawpt;     /* B */
                                } else {
                                        /* first line or left column */
                                        *scanpt++ = *(rawpt + WIDTH + 1);       /* R */
                                        *scanpt++ = (*(rawpt + 1) + *(rawpt + WIDTH)) / 2;      /* G */
                                        *scanpt++ = *rawpt;     /* B */
                                }
                        } else {
                                /* (B)G */
                                if ((i > WIDTH)
                                    && ((i % WIDTH) < (WIDTH - 1))) {
                                        *scanpt++ = (*(rawpt + WIDTH) + *(rawpt - WIDTH)) / 2;  /* R */
                                        *scanpt++ = *rawpt;     /* G */
                                        *scanpt++ = (*(rawpt - 1) + *(rawpt + 1)) / 2;  /* B */
                                } else {
                                        /* first line or right column */
                                        *scanpt++ = *(rawpt + WIDTH);   /* R */
                                        *scanpt++ = *rawpt;     /* G */
                                        *scanpt++ = *(rawpt - 1);       /* B */
                                }
                        }
                } else {
                        if ((i % 2) == 0) {
                                /* G(R) */
                                if ((i < (WIDTH * (HEIGHT - 1)))
                                    && ((i % WIDTH) > 0)) {
                                        *scanpt++ = (*(rawpt - 1) + *(rawpt + 1)) / 2;  /* R */
                                        *scanpt++ = *rawpt;     /* G */
                                        *scanpt++ = (*(rawpt + WIDTH) + *(rawpt - WIDTH)) / 2;  /* B */
                                } else {
                                        /* bottom line or left column */
                                        *scanpt++ = *(rawpt + 1);       /* R */
                                        *scanpt++ = *rawpt;     /* G */
                                        *scanpt++ = *(rawpt - WIDTH);   /* B */
                                }
                        } else {
                                /* R */
                                if (i < (WIDTH * (HEIGHT - 1))
                                    && ((i % WIDTH) < (WIDTH - 1))) {
                                        *scanpt++ = *rawpt;     /* R */
                                        *scanpt++ = (*(rawpt - 1) + *(rawpt + 1) + *(rawpt - WIDTH) + *(rawpt + WIDTH)) / 4;    /* G */
                                        *scanpt++ = (*(rawpt - WIDTH - 1) + *(rawpt - WIDTH + 1) + *(rawpt + WIDTH - 1) + *(rawpt + WIDTH + 1)) / 4;    /* B */
                                } else {
                                        /* bottom line or right column */
                                        *scanpt++ = *rawpt;     /* R */
                                        *scanpt++ = (*(rawpt - 1) + *(rawpt - WIDTH)) / 2;      /* G */
                                        *scanpt++ = *(rawpt - WIDTH - 1);       /* B */
                                }
                        }
                }
                rawpt++;
        }

}


void RGB32_RGB24(unsigned char *dst, unsigned char *src, long int WIDTH,
	  	 long int HEIGHT)
{
	long int size;
	int i, j;
	
	size = WIDTH * HEIGHT * 4;
	
	for(i = 0, j = 0; i < size; i+=4) {
		dst[j] = src[i];
		dst[j+1] = src[i+1];
		dst[j+2] = src[i+2];
		j+=3;
	}
}

void BGR32_RGB24(unsigned char *dst, unsigned char *src, long int WIDTH,
	  	 long int HEIGHT)
{
	long int size;
	int i, j;
	
	size = WIDTH * HEIGHT * 4;
	
	for(i = 0, j = 0; i < size; i+=4) {
		dst[j]   =  src[i+2];
		dst[j+1] =  src[i+1];
		dst[j+2] =  src[i];
		j+=3;
	}
}


void RGB565_RGB24(unsigned char *dst, unsigned char *src, long int WIDTH,
	  	 long int HEIGHT)
{
	long int size = WIDTH * HEIGHT * 3;
	int i, j;
	unsigned int tmp;
	
	for(i = 0, j = 0; i < size; i+= 3) {
		tmp = src[j] + (src[j+1] << 8);
		
		dst[i]   = ((tmp & 0x1f) << 3);
		dst[i+1] = (((tmp >> 5) & 0x3f) << 2);
		dst[i+2] = (((tmp >> 11) & 0x1f) << 3);
		j+=2;
	}
}

void RGB565X_RGB24(unsigned char *dst, unsigned char *src, long int WIDTH,
	  	 long int HEIGHT)
{
	long int size = WIDTH * HEIGHT * 3;
	int i, j;
	unsigned int tmp;
	
	for(i = 0, j = 0; i < size; i+= 3) {
		tmp = src[j+1] + (src[j] << 8);
		
		dst[i]   = ((tmp & 0x1f) << 3);
		dst[i+1] = (((tmp >> 5) & 0x3f) << 2);
		dst[i+2] = (((tmp >> 11) & 0x1f) << 3);
		j+=2;
	}
}


void RGB555_RGB24(unsigned char *dst, unsigned char *src, long int WIDTH,
	  	 long int HEIGHT)
{
	long int size = WIDTH * HEIGHT * 3;
	int i, j;
	unsigned int tmp;
	
	for(i = 0, j = 0; i < size; i+=3) {
		tmp = src[j] + (src[j+1] << 8);
		
		dst[i]   = ((tmp & 0x1f) << 3);
		dst[i+1] = (((tmp >> 5) & 0x1f) << 3);
		dst[i+2] = (((tmp >> 10) & 0x1f) << 3);
		j+=2;
	}
}

void RGB555X_RGB24(unsigned char *dst, unsigned char *src, long int WIDTH,
	  	 long int HEIGHT)
{
	long int size = WIDTH * HEIGHT * 3;
	int i, j;
	unsigned int tmp;
	
	for(i = 0, j = 0; i < size; i+=3) {
		tmp = src[j+1] + (src[j] << 8);
		
		dst[i]   = ((tmp & 0x1f) << 3);
		dst[i+1] = (((tmp >> 5) & 0x1f) << 3);
		dst[i+2] = (((tmp >> 10) & 0x1f) << 3);
		j+=2;
	}
}


void RGB332_RGB24(unsigned char *dst, unsigned char *src, long int WIDTH,
	  	 long int HEIGHT)
{
	long int size = WIDTH * HEIGHT * 3;
	int i, j;
	
	for(i = 0, j= 0; i < size; i+=3) {
		dst[i]   = (src[j] & 0x07) * 255/7;
		dst[i+1] = ((src[j] & 0x38) >> 3) * 255/7;
		dst[i+2] = ((src[j] & 0xc0) >> 6) * 255/3;
		j++;
	}
}

void BGR24_RGB24(unsigned char *dst, unsigned char *src, long int WIDTH,
	  	 long int HEIGHT)
{
	long int size;
	int i;
	
	size = WIDTH * HEIGHT * 3;
	
	for(i = 0; i < size; i+=3) {	
		dst[i] = src[i+2];
		dst[i+1] = src[i+1];
		dst[i+2] = src[i];
	}
}

void RGB24_RGB24(unsigned char *dst, unsigned char *src, long int WIDTH,
                 long int HEIGHT)
{
	memcpy(dst, src, WIDTH * HEIGHT * 3);
}               


