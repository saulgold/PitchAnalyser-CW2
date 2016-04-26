#ifndef __MODULATE_H__
#define __MODULATE_H__

#include <dsp.h>

void createComplexSignal(float fFrequency,int iFrameSize,fractcomplex *compComplexSignal);
void createSimpleSignal(float fFrequency,int iFrameSize,fractional *fractSimpleSignal);

#endif
