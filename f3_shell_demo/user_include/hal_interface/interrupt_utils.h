#ifndef INTERRUPT_UTILS_H
#define INTERRUPT_UTILS_H	1

#include "hal_common_includes.h"

#define _enable_interrupts() __enable_irq()
#define _disable_interrupts() __disable_irq()

#endif