/**************************************************************************************************
* @file		Projects\AudioAdjuster\src\modulate.c
*
* Summary:
*         	the function to create a complex and a simple signal for the audio adjuster project
*
* ToDo:
*     		none
*
* Originator:
*     		Andy Watt
*
* History:
*      		Version 1.00	28/04/2013	Andy Watt	Initial Version adapted from another project 
*			Version 1.01	06/05/2013	Andy Watt	Changed the signal created from a simple sinusoid to a complex exponential. 
*			Version 1.02	10/05/2013	Andy Watt	Added a function to create a simple sinusoid.
*
**************************************************************************************************/
#include "..\inc\modulate.h"
#include <dsp.h>
#include <math.h>

void makeEmptySignal(int iFrameSize,fractional *signal){
	int i;
	for(i = 0;i <iFrameSize;i++){
		signal[i]=0;
	}	
}

void findMax(int iFrameSize,double *modSignal, int maxPosition){
	int i;
	int max = 0;
	
	for(i = 0; i<iFrameSize/2;i++){
		if(modSignal[i] > max){
			max = modSignal[i];
			maxPosition = i;
		}
	}
}

void findPeaks(int maskSize,int iFrameSize, double *modSignal, int peakFrequencies[][30], int peakCount){
	int i;
	int peakMaskMid = maskSize/2 +1;
		
		
	for(i=peakMaskMid;i<(iFrameSize/2-peakMaskMid);i++){
		if((modSignal[i+maskSize-peakMaskMid]<modSignal[i]) && (modSignal[i-maskSize+peakMaskMid]<modSignal[i])){

			peakFrequencies[0][peakCount] = i;
			peakFrequencies[1][peakCount]=modSignal[i];
			peakCount++;
			if(peakCount>29){
				peakCount=0;
			}
		}	
	}	
}

/************************************************************************************************** 
* @fn createComplexSignal
* @param float fFrequency,int iFrameSize,fractcomplex *compComplexSignal
* @return None
* @brief generates a complex exponential of the requested length at the frequency given and stores
* it in the complex array supplied 
**************************************************************************************************/
void takeModulus(int iFrameSize, fractcomplex *complexSignal, double *modSignal){
	int i;
	for(i=1;i<iFrameSize/2;i++){
		modSignal[i] = pow(complexSignal[i].real,2) + pow(complexSignal[i].imag,2);	
	}
}
	
void createComplexSignal(float fFrequency,int iFrameSize,fractcomplex *compComplexSignal)
{
	int n;
	float w, f;				
	float t, dt;
	float Fs = 8000;		
	const float pi = 3.14159265359;

	dt = 1 / Fs;				
	w = 2 * pi * fFrequency;
	for ( n = 0; n < iFrameSize; n++ )
	{
		t = n * dt;
		f=cos( w * t );
		compComplexSignal[n].real = Float2Fract(f);
		f=sin( w * t );
		compComplexSignal[n].imag = Float2Fract(f);
	}
}

/************************************************************************************************** 
* @fn createSimpleSignal
* @param float fFrequency,int iFrameSize,fractional *fractSimpleSignal
* @return None
* @brief generates a simple sinusoid of the requested length at the frequency given and stores
* it in the fractional array supplied 
**************************************************************************************************/

void createSimpleSignal(float fFrequency,int iFrameSize,fractional *fractSimpleSignal)
{
	int n;
	float w;				
	float t, dt;
	float Fs = 8000;
	float v;		
	const float pi = 3.14159265359;

	dt = 1 / Fs;				
	w = 2 * pi * fFrequency;
	for ( n = 0; n < iFrameSize; n++ )
	{
		t = n * dt;
		v = sin( w * t );
		fractSimpleSignal[n] = Float2Fract(v);
	}
}
