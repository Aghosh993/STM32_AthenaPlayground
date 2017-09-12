/*
	File: serial_comms_highlevel_hal.c

	(c) Abhimanyu Ghosh, 2016
 */

#include "usbd_cdc_if.h"
#include "board_usb.h"
#include "serial_comms_highlevel_hal.h"
#include "serial_comms_highlevel.h"

static uint8_t rxbuffer[64U];
static uint8_t txbuffer[64U];

static uint32_t readbuf_pos;

/*
	Initializes all required high-level real/virtual serial port HAL drivers:
 */
void serialport_hal_init(serialport_desc port_descriptor)
{
  _disable_interrupts();
  	readbuf_pos = 0U;
	switch(port_descriptor)
	{
		case PORT1_USB:
			board_usb_init();
			break;
		default:
			break;
	}
  _enable_interrupts();
}

void serialport_hal_enable_tx_isr(serialport_desc port_descriptor)
{
	switch(port_descriptor)
	{
		case PORT1_USB:
			HAL_NVIC_EnableIRQ(USB_LP_CAN_RX0_IRQn);
			break;
		default:
			break;
	}
}

void serialport_hal_disable_tx_isr(serialport_desc port_descriptor)
{
	switch(port_descriptor)
	{
		case PORT1_USB:
			HAL_NVIC_DisableIRQ(USB_LP_CAN_RX0_IRQn);
			break;
		default:
			break;
	}
}

int serialport_send_byte(serialport_desc port_descriptor, uint8_t byte_to_send)
{
	switch(port_descriptor)
	{
		case PORT1_USB:
			break;
		default:
			return -1;
	}
}

uint8_t serialport_receive_byte(serialport_desc port_descriptor)
{
	switch(port_descriptor)
	{
		case PORT1_USB:
			break;
		default:
			return 0;
	}
}

void transferRxData(uint8_t *buffer, uint32_t len)
{
	uint32_t i = 0U;
	for(i=0U; i<len; ++i)
	{
		rxbuffer[i] = buffer[i];
	}
	readbuf_pos = (uint32_t)len;
}