#pragma once

#include "DSP.h"

#define DELAYSIZE 24000

class Delay {

public:

	Delay(void) {}

	~Delay(void) {}

	//Put into preparetoplay..
	void prepareToPlay(void) {
		
		//initialize all variables
		delayWriteIndex = 0;
		delayReadIndex = 0;
		delaySize = 0;
		delaySizeOld = 0;

		delaySizeFrac = 0.0f;
		lpVal = 0.0f;
		
		//clear the delay buffer
		for (int i = 0; i < DELAYSIZE; i++) {
			delay[i] = 0.0f;
		}

		//filter for smoothing the delay time control
		controlFilter.setFc(1.0f);
		modOsc.setF(0.1f, 0.0f);
	}

	//update index.  operates on delayReadIndex and delayWJriteIndex, which are arrays of 2 for left/right indeces.
	void updateIndex(float delayVal, float modAmt, float modFreq, int quad) {

		delayWriteIndex++;
		delayReadIndex++;
		if (delayWriteIndex >= DELAYSIZE) {
			delayWriteIndex = 0;
		}
		if (delayReadIndex >= DELAYSIZE) {
			delayReadIndex = 0;
		}

		delaySize = float(DELAYSIZE) - float(DELAYSIZE) * delayVal;
		delaySize = controlFilter.process(delaySize);
		modOsc.setF(modFreq, modAmt);
		delaySize += modOsc.process(quad);
		delaySizeFrac = delaySize - int(delaySize);


		delayReadIndex = delayWriteIndex + int(delaySize);
		if (delayReadIndex >= DELAYSIZE) {
			delayReadIndex = delayWriteIndex + int(delaySize) - DELAYSIZE;
		}

	}

	float read(void) {
		if ((delayReadIndex < DELAYSIZE) && (delayReadIndex >= 0)) {
			if(delayReadIndex+1 < DELAYSIZE){
				return linterp(delay[delayReadIndex], delay[delayReadIndex+1], delaySizeFrac);
			}else{
				return delay[delayReadIndex];
			}
		}else{
			return 0.0f;
		}
	}

	void write(float input) {
		if ((delayWriteIndex < DELAYSIZE) && (delayWriteIndex >= 0)) {
			delay[delayWriteIndex] = input;
		}
	}

	float linterp(float y1, float y2, float frac) {
		return (y1*(1.0f - frac) + y2*frac);
	}

private:

	//actual delay buffer
	float delay[DELAYSIZE];

	int delayReadIndex;
	int delayWriteIndex;

	//counting on a reasonable conversion from float to int
	float delaySize;
	float delaySizeOld;

	float delaySizeFrac;
	float lpVal;

	OnePoleLp controlFilter;
	gsOsc modOsc;

};
