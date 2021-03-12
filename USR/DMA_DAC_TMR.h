#ifndef _DMA_DAC_TMR_H
#define _DMA_DAC_TMR_H
#include "dac.h"
#include "dma.h"
#include "timer.h"
#include "math.h"

#define pi 3.14

typedef enum _waveType
{
	WAVE_SQUARE,
	WAVE_SINE,
	WAVE_RAMP,
	WAVE_TRIANGLE,
	WAVE_MUX,
} waveType;

void wave_gen(waveType type, u16 len, u16 vpp);
void resetWave(u16 arr,u16 psc);
#endif
