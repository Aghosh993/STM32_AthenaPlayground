#ifndef BOARD_USB_H_
#define BOARD_USB_H_	1

#include "hal_common_includes.h"

#include "usb_device.h"
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_cdc.h"
#include "usbd_cdc_if.h"

void board_usb_init(void);
int board_usb_sendString(uint8_t *pbuf, int len);

#endif