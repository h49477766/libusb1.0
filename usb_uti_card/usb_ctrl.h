#ifndef __USB_CTRL_H__
#define __USB_CTRL_H__

#define  READ_TS_NUM  100
#define  READ_LEN       1024
struct usb_transfer
{
	struct libusb_transfer *transfer;
	char *buffer;
	int completed;
	
} ;
int open_uti_usb(void);
extern void sync_transfer_wait_for_completion(struct libusb_transfer *transfer);

#endif

