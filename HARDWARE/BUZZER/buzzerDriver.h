/*
 * buzzerDriver.h
 *
 *  Created on: Feb 22, 2019
 *      Author: lukasz
 */

#ifndef BUZZERDRIVER_H_
#define BUZZERDRIVER_H_

void buzzerDriverInit(void);
void buzzerSetNewFrequency(uint32_t);
void musicPlay(void);
void music2Play(void);
void music3Play(void);
void music4Play(void);
	
extern u8 music_volume;
#endif /* BUZZERDRIVER_H_ */
