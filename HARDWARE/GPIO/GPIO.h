#ifndef __GPIO_H
#define __GPIO_H	  

#include "include.h"

void GPIO_LED_Init(void);
void GPIO_beep_init(void);
void beep_ON(void);
void beep_OFF(void);
void GPIO_KEY_Init(void);
int if_key_zero(void);
int if_key_one(void);
int if_key_two(void);
void beep_briefly_on(void);
void start_or_stop(void);
void beep_on_once(void);
void LED_trun(void);
void led_toggle(void);

extern char if_key;

#endif

