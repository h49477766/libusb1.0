#include <stdio.h>
#include <stdlib.h>


#include <libusb.h>

#include "usb_ctrl.h"



#define ID_VENDOR  0x7565
#define ID_PRODUCT 0x4001
int fd;

libusb_device_handle *dev_handle = NULL;

void  sync_transfer_wait_for_completion(struct libusb_transfer *transfer);

int open_uti_usb(void)
{
	
	dev_handle = libusb_open_device_with_vid_pid(NULL,ID_VENDOR,ID_PRODUCT);
	if(!dev_handle)
	{
		return -1;
	}
	return 0;
}

void close_uti_usb(void)
{
	if(!dev_handle)
	{
		libusb_close(dev_handle);
	}
}

int start_dma()
{
	/*
	dev->ctrl_dr.bRequestType = 0x40;
	dev->ctrl_dr.bRequest = 0x0c;
	dev->ctrl_dr.wValue = 0x0;
	dev->ctrl_dr.wIndex = 0x0477;//startdma
	dev->ctrl_dr.wLength = 0x04;	
	*/
	int ret;
	char buf[4];
	int buflen = sizeof(buf);
	if(!dev_handle)
	{
		printf("device not exists !\n");
		return -1;
	}
	ret = libusb_control_transfer(dev_handle, 0x40, 0x0c,0, 0x477, buf, buflen, 1000); 
	if(ret != 4)
	{
		printf("start DMA failed !\n");
		return -1;
	}
	printf("start DMA successed !\n");
	return 0;
}

int test_uti_usb(void)
{

	char data[63] = {
	         0xfe,0x00,0x00,0x00,0x00,0x3f,0x00,0x00,
			 0x04,0x28,0x00,0x00,0x04,0x18,0x00,0x10,
			 0x00,0x00,0x88,0xcc,0xcc,0xcc,0xcc,0xcc,
			 0xcc,0xcc,0xcc,0xcc,0xcc,0xcc,0xcc,0xcc,
			 0xcc,0xcc,0xcc,0xcc,0xcc,0xcc,0xcc,0xcc,
			 0xcc,0xcc,0xcc,0xcc,0xcc,0xcc,0xcc,0xcc,
			 0xcc,0xcc,0xcc,0xcc,0xcc,0xcc,0xcc,0xcc,
			 0xcc,0xcc,0xcc,0xcc,0xcc,0xcc,0xcc};
	
	unsigned char buf[63] = {'\0'};
	int ret;
	int act_len;
	int i = 0;

	ret = libusb_interrupt_transfer(dev_handle,1,data,sizeof(data),&act_len,1000); // write
	if(0 != ret)
	{
		printf("interrupt_transfer failed \n");
		
	}
	printf("act_len = %d\n",act_len);
	
	ret = libusb_interrupt_transfer(dev_handle,1|LIBUSB_ENDPOINT_IN,buf,sizeof(buf),&act_len,1000); // read
	if(0 != ret)
	{
		printf("interrupt_transfer failed \n");
		
	}
	printf("read act_len = %d\n",act_len);

	for(i = 0;i < 63;i++)
	{
		if(0 == i %8 && 0 != i) printf("\n");
		printf("0x%02x ",buf[i]);
	}
	printf("\n");

	return 0;
}

static void  read_ts_cb(struct libusb_transfer *transfer)
{
	int *completed = transfer->user_data;
	*completed = 1;
	int r;
	//usbi_dbg("actual_length=%d", transfer->actual_length);
	printf("actual_length=%d\n", transfer->actual_length);
	/* caller interprets result and frees transfer */

	if(transfer->status == LIBUSB_TRANSFER_COMPLETED)
	{

		write(fd,transfer->buffer,transfer->actual_length);
		r = libusb_submit_transfer(transfer);
		if (r < 0) {
			printf("libusb_submit failed!\n");
			libusb_free_transfer(transfer);
			return r;
		}
		
		//sync_transfer_wait_for_completion(transfer);
	}

}


int read_ts()
{


	int r;
	int *transferred;

	int i;
	struct usb_transfer  usb_read_ts[READ_TS_NUM];

	for(i = 0;i < READ_TS_NUM;i++)
	{
		usb_read_ts[i].transfer  = libusb_alloc_transfer(0);
		usb_read_ts[i].buffer = (unsigned char *)malloc(READ_LEN);
	}
	

	for(i = 0;i < READ_TS_NUM;i++)
	{
	   if (!usb_read_ts[i].transfer)  return LIBUSB_ERROR_NO_MEM;
	   	
		libusb_fill_bulk_transfer(usb_read_ts[i].transfer, dev_handle, 2|LIBUSB_ENDPOINT_IN, usb_read_ts[i].buffer , READ_LEN,
			read_ts_cb, &usb_read_ts[i].completed, 1000);
		usb_read_ts[i].transfer->type = LIBUSB_TRANSFER_TYPE_BULK;

		r = libusb_submit_transfer(usb_read_ts[i].transfer);
		if (r < 0) {
			printf("libusb_submit failed!\n");
			libusb_free_transfer(usb_read_ts[i].transfer);
			return r;
		}

		//sync_transfer_wait_for_completion(usb_read_ts[i].transfer);
	   
	}
		


	/*
	*transferred = transfer->actual_length;
	switch (transfer->status) {
	case LIBUSB_TRANSFER_COMPLETED:
		r = 0;
		break;
	case LIBUSB_TRANSFER_TIMED_OUT:
		r = LIBUSB_ERROR_TIMEOUT;
		break;
	case LIBUSB_TRANSFER_STALL:
		r = LIBUSB_ERROR_PIPE;
		break;
	case LIBUSB_TRANSFER_OVERFLOW:
		r = LIBUSB_ERROR_OVERFLOW;
		break;
	case LIBUSB_TRANSFER_NO_DEVICE:
		r = LIBUSB_ERROR_NO_DEVICE;
		break;
	case LIBUSB_TRANSFER_ERROR:
	case LIBUSB_TRANSFER_CANCELLED:
		r = LIBUSB_ERROR_IO;
		break;
	default:
		usbi_warn(HANDLE_CTX(dev_handle),
			"unrecognised status code %d", transfer->status);
		r = LIBUSB_ERROR_OTHER;
	}

	libusb_free_transfer(transfer);
	return r;
	*/

	return 0;
}
	
