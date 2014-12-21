#ifndef __V4L2_CAMERA__
#define __V4L2_CAMERA__
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <gtk/gtk.h>
#include <glib.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <sys/ioctl.h>
#include <linux/videodev.h>
#include <linux/videodev2.h>
#include "fmt_rgb24.h"


struct pix_fmt_conv_t {
	unsigned int format;
        void(*fmt_convert)(unsigned char *dst, unsigned char *src, long int WIDTH, long int HEIGHT);
};

#define NUM_FORMATS 11

struct pix_fmt_conv_t formats[NUM_FORMATS] = {
	{V4L2_PIX_FMT_SBGGR8, BAYER_RGB24},
	{V4L2_PIX_FMT_RGB24, RGB24_RGB24},
	{V4L2_PIX_FMT_BGR24, BGR24_RGB24},
	{V4L2_PIX_FMT_RGB32, RGB32_RGB24},
	{V4L2_PIX_FMT_BGR32, BGR32_RGB24},
	{V4L2_PIX_FMT_RGB332, RGB332_RGB24},
	{V4L2_PIX_FMT_RGB555, RGB555_RGB24},
	{V4L2_PIX_FMT_RGB555X,RGB555X_RGB24},
	{V4L2_PIX_FMT_RGB565, RGB565_RGB24},
	{V4L2_PIX_FMT_RGB565X, RGB565X_RGB24},
	{V4L2_PIX_FMT_YUV420, YUV420P_RGB24}
};


typedef struct vdev_t vdev_t;
struct vdev_t {
	int fd;
	char *dev_name;
	struct v4l2_capability cap;
	struct v4l2_format format;
	long int width;
	long int height;
	long int size;
	long int depth;
	unsigned char *dst;
	unsigned char *src;
        GtkWidget *window;
        GtkWidget *drawarea;
        void(*fmt_convert)(unsigned char *dst, unsigned char *src, long int WIDTH, long int HEIGHT);
};


static void usage(char *argv0);
static void open_device(vdev_t *device);
static void query_device(vdev_t *device);
static void set_format_convertor(vdev_t *device);
static void allocate_buffers(vdev_t *device);
static int read_device(vdev_t *device);
static int cleanup(void *device);
gboolean refresh_image(vdev_t *device);

#endif
