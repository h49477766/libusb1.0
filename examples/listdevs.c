/*
 * libusb example program to list devices on the bus
 * Copyright © 2007 Daniel Drake <dsd@gentoo.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include <stdio.h>

#include "libusb.h"
#include "libusbi.h"


#define ID_VENDOR  0x7565
#define ID_PRODUCT 0x4001

static void print_devs(libusb_device **devs)
{
	libusb_device *dev;
	int i = 0, j = 0;
	uint8_t path[8]; 

	while ((dev = devs[i++]) != NULL) {
		struct libusb_device_descriptor desc;
		int r = libusb_get_device_descriptor(dev, &desc);
		if (r < 0) {
			fprintf(stderr, "failed to get device descriptor");
			return;
		}

		printf("%04x:%04x (bus %d, device %d)",
			desc.idVendor, desc.idProduct,
			libusb_get_bus_number(dev), libusb_get_device_address(dev));

		r = libusb_get_port_numbers(dev, path, sizeof(path));
		if (r > 0) {
			printf(" path: %d", path[0]);
			for (j = 1; j < r; j++)
				printf(".%d", path[j]);
		}
		printf("\n");
	}
}

int main(void)
{
	libusb_device **devs;
	int r;
	ssize_t cnt;
	int i = 0;

    char data[63] = {0xfe,0x00,0x00,0x00,0x00,0x3f,0x00,0x00,
	             0x04,0x28,0x00,0x00,0x04,0x18,0x00,0x10,
	             0x00,0x00,0x88,0xcc,0xcc,0xcc,0xcc,0xcc,
	             0xcc,0xcc,0xcc,0xcc,0xcc,0xcc,0xcc,0xcc,
	             0xcc,0xcc,0xcc,0xcc,0xcc,0xcc,0xcc,0xcc,
	             0xcc,0xcc,0xcc,0xcc,0xcc,0xcc,0xcc,0xcc,
	             0xcc,0xcc,0xcc,0xcc,0xcc,0xcc,0xcc,0xcc,
	             0xcc,0xcc,0xcc,0xcc,0xcc,0xcc,0xcc};
	
	unsigned char buf[63] = {'\0'};
	libusb_device *dev;
	struct libusb_device_descriptor desc;
	libusb_device_handle *dev_handle = NULL;

	r = libusb_init(NULL);
	if (r < 0)
		return r;

	int act_len = 0;
	int ret = 0;
	dev_handle = libusb_open_device_with_vid_pid(NULL,ID_VENDOR,ID_PRODUCT);
	
	printf("ret = %d\n",ret);
	if(!dev_handle)
	{
		printf("open uti usb device failed!\n");
		return 0;
	}
	
	ret = libusb_interrupt_transfer(dev_handle,1,data,sizeof(data),&act_len,1000);
	printf("act_len = %d\n",act_len);
	
	ret = libusb_interrupt_transfer(dev_handle,1|0x80,buf,sizeof(buf),&act_len,1000);
	printf("read act_len = %d\n",act_len);

	for(i = 0;i < 63;i++)
	{
		if(0 == i %8 && 0 != i) printf("\n");
		printf("0x%02x ",buf[i]);
	}
    printf("\n");

	if(0 != ret)
	{
		printf("interrupt_transfer failed \n");
		
	}

	libusb_close(dev_handle);


	
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
