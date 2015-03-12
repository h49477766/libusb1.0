#include <stdio.h>
#include <libusb.h>
#include <fcntl.h>

//#include "libusbi.h"


extern int fd;

int main()
{
	libusb_device **devs;
	int r;
	ssize_t cnt;
	
	int ret;


	libusb_device *dev;
	struct libusb_device_descriptor desc;
	

	fd = open("1.ts",O_WRONLY|O_TRUNC );
	if(fd < 0)
	{
		printf("open file failed!\n");
	}
	r = libusb_init(NULL);
	if (r < 0)
		return r;

	ret = open_uti_usb();
	if(0 != ret)
	{
		printf("open uti usb device failed!\n");
		exit(0);
	}

	start_dma();

	test_uti_usb();
	read_ts();

	close_uti_usb();
	
	

	
	


	
	/*
	cnt = libusb_get_device_list(NULL, &devs);
	if (cnt < 0)
		return (int) cnt;

	for(i = 0;i < cnt;i++)
	{
		dev = devs[i];
		r = libusb_get_device_descriptor(dev, &desc);
		if(r < 0) printf("failed to get device descriptor");
		printf("idVendor = %#x,idProduct = %#x\n",dev->device_descriptor.idVendor,dev->device_descriptor.idProduct);
		
	}
	
	
	
	//print_devs(devs);
	libusb_free_device_list(devs, 1);

	libusb_exit(NULL);
	*/
	return 0;
}

