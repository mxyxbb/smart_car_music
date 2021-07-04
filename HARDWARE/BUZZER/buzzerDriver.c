/*
 * buzzerDriver.c
 *
 *  Created on: Feb 22, 2019
 *      Author: lukasz
 */
//#include "main.h"
//#include "stdint.h"
#include "include.h"


#include "buzzerDriver.h"
#include "melody.h"
#include "stdbool.h"
#include "mymath.h"
#include "stdio.h"
#include "shell_port.h"

#define CPU_FREQ	96000000
#define PRESCALER 	96

u8 music_volume=50;

/*
 * Buzzer driver init
 */
inline void buzzerDriverInit(void)
{
	//Base Timer configuration:
	TIM17 -> CR1 &= ~(TIM_CR1_CKD_0 | TIM_CR1_CKD_1 | TIM_CR1_DIR);	// TIME x1 and DIR UP
	TIM17 -> CR1 |= TIM_CR1_ARPE;									// Auto reload
	TIM17 -> PSC = PRESCALER - 1;									// Prescaler
	TIM17 ->ARR = 60000;												// Period

	//Pulse
	TIM17 -> CCER &= ~TIM_CCER_CC4E;
	TIM17 -> CCER |= TIM_CCER_CC4P;

	TIM17 -> CCMR2 &= ~TIM_CR2_OIS4;
	TIM17 -> CCMR2 |= TIM_CCMR2_OC4M_2 | TIM_CCMR2_OC4M_1;

	//Set pulse
	TIM17 -> CCR1 = 60000/2;
}

/*
 * 	Set new frequency
 *
 * 	args:
 * 	newFreq - new frequency in Hz.
 */
void buzzerSetNewFrequency(uint32_t newFreq)
{
	uint64_t tempFreq = newFreq;
	if(newFreq != 0){

	uint64_t tempNewValue = (uint64_t) CPU_FREQ / PRESCALER / tempFreq;

	// setting new value
	TIM17 ->ARR = (uint32_t)tempNewValue;
	TIM17 -> CCR1 = (uint32_t)tempNewValue/music_volume;
	}
	else{
		TIM17 -> CCR1 = 0;
	}
}
void buzzerSetNewFrequency2(uint32_t newFreq)
{
	#if 0
	uint64_t tempFreq = newFreq;
	if(newFreq != 0){

	uint64_t tempNewValue = (uint64_t) CPU_FREQ / PRESCALER / tempFreq;

	// setting new value
	TIM12 ->ARR = (uint32_t)tempNewValue;
	TIM12 -> CCR2 = (uint32_t)tempNewValue/2;
	}
	else{
		TIM12 -> CCR2 = 0;
	}
	#endif
}


//void musicPlay()
//{
//	__ExecuteOnce(buzzerDriverInit());

//  TIM9 -> CR1 |= TIM_CR1_CEN;
//  TIM9 ->CCER |= TIM_CCER_CC4E;

//  int melodyCount = sizeof(melodySizes)/ sizeof(uint32_t);

//  for(int melodyIndex = 0; melodyIndex < melodyCount; melodyIndex++)
//  {
//	  for(int noteIndex = 0; noteIndex < melodySizes[melodyIndex]; noteIndex++)
//  	  {
//	  	  buzzerSetNewFrequency(melody[melodyIndex][noteIndex]);
//	  	  HAL_Delay(noteDurations[melodyIndex][noteIndex] * melodySlowfactor[melodyIndex]);
//  	  }
//  }
//   TIM9 -> CR1 &= ~TIM_CR1_CEN;
//   TIM9 ->CCER &= ~TIM_CCER_CC4E;
//}
//非堵塞
void musicPlay()
{
	__ExecuteOnce(buzzerDriverInit());
	__ExecuteOnce(TIM17 -> CR1 |= TIM_CR1_CEN);
	__ExecuteOnce(TIM17 ->CCER |= TIM_CCER_CC4E);
//  TIM9 -> CR1 |= TIM_CR1_CEN;
//  TIM9 ->CCER |= TIM_CCER_CC4E;
	static uint32_t cnt_=0;
//  static int melodyCount = sizeof(melodySizes)/ sizeof(uint32_t);
//	static int melodyIndex = 0;
	static int noteIndex = 0;
	if( noteIndex >= melodySizes[0]) noteIndex=0;
	if(cnt_++%(noteDurations[0][noteIndex] /** melodySlowfactor[0]*/)==0)
	{
		
		buzzerSetNewFrequency(melody[0][noteIndex]);
		
		noteIndex++;
//			HAL_Delay(noteDurations[melodyIndex][noteIndex] * melodySlowfactor[melodyIndex]);

	}
//	__ValueStep(melodyIndex,1,melodyCount);
//   TIM9 -> CR1 &= ~TIM_CR1_CEN;
//   TIM9 ->CCER &= ~TIM_CCER_CC4E;
}
void music2Play()//堵塞式
{
  int melodyCount = sizeof(melodySizes2)/ sizeof(uint32_t);

  for(int melodyIndex = 0; melodyIndex < melodyCount; melodyIndex++)
  {
	  for(int noteIndex = 0; noteIndex < melodySizes2[melodyIndex]; noteIndex++)
  	  {
	  	  buzzerSetNewFrequency(melody2[melodyIndex][noteIndex]);
	  	  delayms((u16)(noteDurations2[melodyIndex][noteIndex] * melodySlowfactor[melodyIndex]));
  	  }
  }

}
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), m2, music2Play, music2Play());


void music3Play()
{
	#if 0
	__ExecuteOnce(buzzerDriverInit());
	__ExecuteOnce(TIM9 -> CR1 |= TIM_CR1_CEN);
	__ExecuteOnce(TIM9 ->CCER |= TIM_CCER_CC4E);
	__ExecuteOnce(TIM12 -> CR1 |= TIM_CR1_CEN);
	__ExecuteOnce(TIM12 ->CCER |= TIM_CCER_CC4E);
	
	static uint32_t cnt_=0;
	static uint32_t cnt2_=0;
	
//  static int melodyCount = sizeof(melodySizes)/ sizeof(uint32_t);
//	static int melodyIndex = 0;
	static int noteIndex = 0;
	static int noteIndex2 = 0;
	
	if( noteIndex >= melodySizes3[0]) noteIndex=0;
	if( noteIndex2 >= melodySizes3_2[0]) noteIndex2=0;
	
	if(cnt_++%(noteDurations3[0][noteIndex] * melodySlowfactor[0])==0){
		buzzerSetNewFrequency(melody3twincle[0][noteIndex]);
		noteIndex++;
	}
	if(cnt2_++%(noteDurations3_2[0][noteIndex2] * melodySlowfactor[0])==0){
		buzzerSetNewFrequency2(melody3twincle2[0][noteIndex2]);
		noteIndex2++;
	}
//	__ValueStep(melodyIndex,1,melodyCount);
//   TIM9 -> CR1 &= ~TIM_CR1_CEN;
//   TIM9 ->CCER &= ~TIM_CCER_CC4E;
	#endif
}

//非堵塞
void music4Play()
{
	__ExecuteOnce(buzzerDriverInit());
	__ExecuteOnce(TIM17 -> CR1 |= TIM_CR1_CEN);
	__ExecuteOnce(TIM17 ->CCER |= TIM_CCER_CC4E);
//  TIM9 -> CR1 |= TIM_CR1_CEN;
//  TIM9 ->CCER |= TIM_CCER_CC4E;
	static uint32_t cnt_=0;
//  static int melodyCount = sizeof(melodySizes)/ sizeof(uint32_t);
//	static int melodyIndex = 0;
	static int noteIndex = 0;
	if( noteIndex >= melodySizes4[0]) noteIndex=0;
	
	
	buzzerSetNewFrequency(melody4[0][noteIndex]);
	
	if(cnt_++%(noteDurations4[0][noteIndex] * (melodySlowfactor[0]-7))==0)
	{
		noteIndex++;
//		if(noteIndex%8==2)
//			buzzer_flag=1;
	}
}

/*
非堵塞
定时调用music5_ticks
通过标志位music5_play_flag来控制播放的开始
置位music5_play_flag，可以非堵塞地播放一遍提示音
播放完之后函数自动将标志位清零
*/
char music5_play_flag=0;
void music5_ticks()
{
	static uint32_t cnt_=0;
	static int noteIndex = 0;
	if(!music5_play_flag) return;
	if( noteIndex >= melodySizes5[0]) 
	{
		music5_play_flag=0;
		noteIndex=0;
		cnt_=0;
		return;
	}
	buzzerSetNewFrequency(melody5[0][noteIndex]);
	if(cnt_++%(noteDurations5[0][noteIndex] * (melodySlowfactor[0]))==0)
	{
		noteIndex++;
//		if(noteIndex%8==2)
//			buzzer_flag=1;
	}
}
