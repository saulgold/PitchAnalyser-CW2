#ifndef __MODULATE_H__
#define __MODULATE_H__

#include <dsp.h>

void createComplexSignal(float fFrequency,int iFrameSize,fractcomplex *compComplexSignal);
void createSimpleSignal(float fFrequency,int iFrameSize,fractional *fractSimpleSignal);
void takeModulus(int iFrameSize, fractcomplex *complexSignal, double *modSignal);
void findPeaks(int maskSize,int iFrameSize, double *modSignal, int peakFrequencies[][30], int peakCount);
void findMax(int iFrameSize,double *modSignal, int maxPosition);
void makeEmptySignal(int iFrameSize,fractional *signal);
//void removeSmallPeaks(int iFrameSize, int peakFrequencies[][30],
#endif
