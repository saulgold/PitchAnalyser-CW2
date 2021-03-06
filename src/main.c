/**************************************************************************************************
* @file		Projects\AudioAdjuster\src\main.c
*
* Summary:
*         	main function for the audio adjuster project
*
* ToDo:
*     		none
*
* Originator:
*     		Andy Watt
*
* History:
* 			Version 1.00	20/04/2013	Andy Watt	Initial Version copied from Microchip Demo Project and modified 
*      		Version 1.01    28/04/2013	Andy Watt	Added filter and modulate function calls 
*      		Version 1.02    01/05/2013	Andy Watt	Added mode switching and low pass filter function calls 
*      		Version 1.03	07/05/2013	Andy Watt	Added transform function calls
*
***************************************************************************************************/
#include <p33FJ256GP506.h>
#include <sask.h>
#include <ex_sask_generic.h>
#include <ex_sask_led.h>
#include <dsp.h>
#include <ADCChannelDrv.h>
#include <OCPWMDrv.h>


#include "..\inc\filter.h"
#include "..\inc\modulate.h"
#include "..\inc\complexmultiply.h"
#include "..\inc\transform.h"

#define FRAME_SIZE 			128
#define DEBUG
//Allocate memory for input and output buffers
fractional		adcBuffer		[ADC_CHANNEL_DMA_BUFSIZE] 	__attribute__((space(dma)));
fractional		ocPWMBuffer		[OCPWM_DMA_BUFSIZE]		__attribute__((space(dma)));

//variables for FFT
fractcomplex compx[FRAME_SIZE]__attribute__ ((space(ymemory),far));
fractcomplex compX[FRAME_SIZE]__attribute__ ((space(ymemory),far));
fractcomplex compXfiltered[FRAME_SIZE/2]__attribute__ ((space(ymemory),far));
fractcomplex compXshifted[FRAME_SIZE]__attribute__ ((space(ymemory),far));
fractcomplex testFFT[FRAME_SIZE]__attribute__ ((space(ymemory),far));

fractional generatedSignal[FRAME_SIZE];
fractional peakFreqSignal[FRAME_SIZE];
fractional testSignal[FRAME_SIZE];
fractional lowFreq[FRAME_SIZE];
fractional highFreq[FRAME_SIZE];

double compXfilteredAbs[FRAME_SIZE/2]__attribute__ ((space(ymemory),far));
double compXSmoothed[FRAME_SIZE/2]__attribute__ ((space(ymemory),far));

//variables for audio processing
fractional		frctAudioIn			[FRAME_SIZE]__attribute__ ((space(xmemory),far));

fractional		frctAudioOut		[FRAME_SIZE]__attribute__ ((space(xmemory),far));


//Instantiate the drivers
ADCChannelHandle adcChannelHandle;
OCPWMHandle 	ocPWMHandle;

//Create the driver handles
ADCChannelHandle *pADCChannelHandle 	= &adcChannelHandle;
OCPWMHandle 	*pOCPWMHandle 		= &ocPWMHandle;

float getFrequency(int samplingRate, int frameSize, int FFTPosition){
	float frequency = (samplingRate/2)*(FFTPosition/64);
	return frequency; 
}
	float testFrequency = 0;
	
#define averageFrame 5
int averageMidPoint =averageFrame- averageFrame/2;
int totalAvg;
/*masks must be odd*/
#define peakMask 3
int peakMaskMid = peakMask- peakMask/2;
int play;
int record;
int peakFrequencies[2][30]__attribute__ ((space(ymemory),far));
int i __attribute__((space(dma)));
int j __attribute__((space(dma)));
int count __attribute__((space(dma))); 
int freqNumber;
int erase; 
int peakCount;
int main(void)

{
	count =0;
	int max;
	int maxPosition=0;
	int x=0;
	ex_sask_init( );
	
//	createSimpleSignal(500,FRAME_SIZE,lowFreq);
//	createSimpleSignal(1000,FRAME_SIZE,highFreq);	
//	for(i=0;i<FRAME_SIZE;i++){
//		testSignal[i] =lowFreq[i]+highFreq[i];
//	}	
//	
//	for(i=0; i<FRAME_SIZE;i++){
//		if(i==23){
//			testFFT[i].real=10000;
//		}
//		else if(i==90){
//			testFFT[i].real=10000;
//		}
//		else{
//		testFFT[i].real=0;
//		}			
//	}
	
	inverseFourierTransform(FRAME_SIZE, testSignal, testFFT);

	//Initialise Audio input and output function
	ADCChannelInit	(pADCChannelHandle,adcBuffer);			
	OCPWMInit		(pOCPWMHandle,ocPWMBuffer);			

	//Start Audio input and output function
	ADCChannelStart	(pADCChannelHandle);
	OCPWMStart		(pOCPWMHandle);	
	
	 testFrequency = 3000;
	
	
   	max = 0;
		maxPosition =0;
		float freq;
		

		
	while(1)
	{   
		    
		
		//Wait till the ADC has a new frame available
		while(ADCChannelIsBusy(pADCChannelHandle));
		//Read in the Audio Samples from the ADC
		ADCChannelRead	(pADCChannelHandle,frctAudioIn,FRAME_SIZE);
		
		
		if(record ==1){
		

			
		
			//work in the frequency domain
			fourierTransform(FRAME_SIZE,compX,frctAudioIn);
	
			
			averageMidPoint = 5/2;
			//take absolute value of the FFT result
//			for(i=1;i<FRAME_SIZE/2;i++){
//					compXfilteredAbs[i] = pow(compX[i].real,2) + pow(compX[i].imag,2);	
//			}	
		
		takeModulus(FRAME_SIZE,compX,compXfilteredAbs);
		
			/*smooth signal by taking rolling average*/
//			for(i=0;i<FRAME_SIZE/2-averageMidPoint;i++){
//				totalAvg=0;
//				for(j=i;j<(i+averageFrame);j++){
//					totalAvg += compXfilteredAbs[j];
//				}
//				compXSmoothed[i +averageMidPoint] = totalAvg; /*no need to divide if dont divide all*/
//			}
			/*find multiple peaks*/
			
			findPeaks(5,FRAME_SIZE,compXfilteredAbs,peakFrequencies, peakCount);

//			for(i=peakmaskmid;i<(frame_size/2-peakmaskmid);i++){
//				if((compxfilteredabs[i+peakmask-peakmaskmid]<compxfilteredabs[i]) && (compxfilteredabs[i-peakmask+peakmaskmid]<compxfilteredabs[i])){
//
//					peakfrequencies[0][x] = i;
//					peakfrequencies[1][x]=compxfilteredabs[i];
//					x++;
//					if(x>29){
//						x=0;
//					}
//				}	
//			}
//			//find the peak frequency	
//			for(i = 0; i<FRAME_SIZE/2;i++){
//					if(compXfilteredAbs[i] > max){
//					max = compXfilteredAbs[i];
//					maxPosition = i;
//					}
//			}
//
			findMax(FRAME_SIZE, compXfilteredAbs,maxPosition);
			makeEmptySignal(FRAME_SIZE,generatedSignal);
//			for(i = 0;i <FRAME_SIZE;i++){
//				generatedSignal[i]=0;
//			}
		
		}
		
		if(play==1){

		
						
			freq= 4000.0 *(peakFrequencies[0][freqNumber]/64.0);
			createSimpleSignal(freq, FRAME_SIZE, generatedSignal);
			
			/*remove small peaks as percentage of max*/
			for(i=0;i<30;i++){
				if(peakFrequencies[1][i] >= max/10){
					freq= 4000.0 *(peakFrequencies[0][i]/64.0);
					createSimpleSignal(freq, FRAME_SIZE,generatedSignal);
					/*add each peak frequency to output signal*/
					for(i = 0;i<FRAME_SIZE;i++){
						peakFreqSignal[i]+=generatedSignal[i];
					}
				}
			}
			
			//determine which led to display based on peak fequency
			if(max< 100){
				GREEN_LED=1;
				RED_LED = 1;
				YELLOW_LED=1;
			}
			
			else if(maxPosition<FRAME_SIZE/6){
					GREEN_LED=0;
						RED_LED = 1;
						YELLOW_LED=1;
			}
			else if(maxPosition>FRAME_SIZE/6 && maxPosition<FRAME_SIZE/3){
					GREEN_LED=1;
						RED_LED = 1;
						YELLOW_LED=0;
			}
			else if(maxPosition>FRAME_SIZE/3){
					GREEN_LED=1;
						RED_LED = 0;
						YELLOW_LED=1;
			}
			else{
				 GREEN_LED=1;
						RED_LED = 1;
						YELLOW_LED=1;	
			}
	
		}
		
		if(erase==1){
			
		}
		
		
		//Wait till the OC is available for a new frame
		while(OCPWMIsBusy(pOCPWMHandle));	
		//Write the real part of the frequency shifted complex audio signal to the output
		OCPWMWrite (pOCPWMHandle,generatedSignal,FRAME_SIZE);
					

		
		if(SWITCH_S1 ==0){
			record =1;
			play=0;
			RED_LED = 0;
			GREEN_LED=1;
			freqNumber=0;
			
		/*erase peak frequencies*/
//			for(i=0;i<30;i++){
//				for(j=0;j<2;j++){
//					peakFrequencies[j][i]=0;
//				}
//			}
		}
		
		if(SWITCH_S2==0){
			record=0;
			play=1;
			RED_LED = 1;
			GREEN_LED=0;
		
		}
		
		if(SWITCH_S1 == 1 && SWITCH_S2==1){
			record=0;
			play=0;
			RED_LED = 1;
			GREEN_LED=1;
			x=0;
			/*play nothing*/
			for(i = 0;i <FRAME_SIZE;i++){
				generatedSignal[i]=0;
			}
		
			freqNumber++;
			
		}
		
	}
}
