#ifndef __ENCODER_H
#define __ENCODER_H	 

#include "HAL_device.h"
#include "HAL_conf.h"

void encoder_init(void);
int read_encoder(u8 TIMX);
void test_encoder_lcd(void);






#endif

