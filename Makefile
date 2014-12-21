default:
	gcc *.c -Wall -g -O2 `pkg-config --cflags --libs gtk+-2.0` -I /lib/modules/`uname -r`/build/include -o v4l2_camera


##############################################################################
