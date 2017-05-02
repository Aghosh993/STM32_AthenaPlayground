#ifndef BUTTON_H_
#define BUTTON_H_	1

#include "hal_common_includes.h"

typedef enum {
	USER_BUTTON
} button;

void board_button_init(void);
uint32_t board_button_pressed(button b);

#endif