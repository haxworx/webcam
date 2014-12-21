#include "v4l2_camera.h"


int safety = 1;

int main(int argc, char **argv)
{
	vdev_t device;
	GtkWidget *window;
	GtkWidget *drawarea;
	GtkWidget *box;
	GtkWidget *button;
	
	if(argc != 2) {
		usage(argv[0]);
	}	

	device.dev_name = argv[1];

/*	open_device(&device);
	query_device(&device);
	set_format_convertor(&device);
	allocate_buffers(&device); */
		
	gtk_init(&argc, &argv);
	
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	box = gtk_hbox_new(TRUE, 0);
	gtk_window_set_title((GtkWindow *)window, "V4L2 Camera");
	drawarea = gtk_drawing_area_new();
	button = gtk_button_new_with_label ("Hello World");
//	gtk_widget_set_size_request (drawarea, device.width,device.height);
	gtk_widget_set_size_request (drawarea, 640, 480);
//	g_signal_connect(window, "destroy", G_CALLBACK(cleanup), &device);

	gtk_container_add(GTK_CONTAINER(box), button);
	gtk_container_add(GTK_CONTAINER(box), drawarea);
	gtk_container_add(GTK_CONTAINER(window), box);

	gtk_widget_show_all(box);	
	
        gtk_widget_show_all(window);
	
	device.window = window;
	device.drawarea = drawarea;
	     
        g_timeout_add(0, ((GSourceFunc) refresh_image), &device);

        gtk_main();

	return EXIT_SUCCESS;
}


static void set_format_convertor(vdev_t *device)
{
	int i;
	int status;
	
	device->format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

	status = ioctl(device->fd, VIDIOC_G_FMT, &device->format);
	if(status < 0) {
		fprintf(stderr, "Error: VIDIOC_G_FMT %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	if(device->format.fmt.pix.width < 320)
		device->width = device->format.fmt.pix.width = 320;
	else
		device->width = device->format.fmt.pix.width;
	
	if(device->format.fmt.pix.height < 240)
		device->height = device->format.fmt.pix.height = 240;
	else	
		device->height = device->format.fmt.pix.height;	
	
	status = ioctl(device->fd, VIDIOC_S_FMT, &device->format);
	if(status < 0) {
		fprintf(stderr, "Error: VIDIOC_S_FMT %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	
	for(i = 0; i < NUM_FORMATS; i++) {
		if(device->format.fmt.pix.pixelformat == formats[i].format) {
			device->size = device->format.fmt.pix.sizeimage;
			device->fmt_convert = formats[i].fmt_convert;
			return;
		}
	}
	
	fprintf(stderr, "Error: Your camera's image format is not supported by this program\n");
	exit(EXIT_FAILURE);
}

static int read_device(vdev_t *device)
{
	size_t len;
			
	len = read(device->fd, device->src, device->size);
	if(len != device->size)
		return 0;
	
	device->fmt_convert(device->dst, device->src, device->width, device->height);
	
	return 1;
}

gboolean refresh_image(vdev_t *device)
{
        int status;
               
        status = read_device(device);
        if(!status)
        	return FALSE;
        	
	if(safety)
		gdk_draw_rgb_image (device->drawarea->window, device->drawarea->style->fg_gc[GTK_STATE_NORMAL],
		      0, 0, device->width, device->height,
		      GDK_RGB_DITHER_MAX, device->dst, device->width * 3);

        return TRUE;
}


static void allocate_buffers(vdev_t *device)
{
	/* RGB24 Buffer */
	device->dst = malloc(device->width * device->height * 3);
	if(!device->dst) {
		fprintf(stderr, "Error: malloc()\n");
		exit(EXIT_FAILURE);
	}
	
	/* Input buffer dependent on format */
	device->src = malloc(device->size);
	if(!device->src) {
		fprintf(stderr, "Error: malloc()\n");
		exit(EXIT_FAILURE);
	}
}

static void query_device(vdev_t *device)
{
	int status;
	
	status = ioctl(device->fd, VIDIOC_QUERYCAP, &device->cap);
	if(status < 0) {
		if(EINVAL == errno) {
			fprintf(stderr, "%s is not a V4L2 device\n", device->dev_name);
			exit(EXIT_FAILURE);
		} else {
			fprintf(stderr, "Error: VIDIOC_QUERYCAP %s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}
	}
	
	if(!(device->cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
		fprintf(stderr, "%s is not a video capture device\n", device->dev_name);
		exit(EXIT_FAILURE);
	}
	
}

static void open_device(vdev_t *device)
{
	device->fd = open(device->dev_name, O_RDWR);
	if(device->fd < 0) {
		fprintf(stderr, "Error: Unable to open %s - %s\n", device->dev_name, strerror(errno));
		exit(EXIT_FAILURE);
	}		
}


static void usage(char *argv0)
{
	fprintf(stderr, "Usage:\n");
	fprintf(stderr, "%s [device]\n", argv0);
	
	exit(EXIT_FAILURE);
}

static int cleanup(void *device)
{
	vdev_t *tmp = device;
	
	close(tmp->fd);
	free(tmp->dst);
	free(tmp->src);
	safety = 0;
        /* Fix ME */        
        gtk_main_quit();

        exit(EXIT_SUCCESS);
}
