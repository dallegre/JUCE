#pragma once

#include "DSP.h"

#define DELAYSIZE 5000

class Delay {

public:

	Delay(void) {}

	~Delay(void) {}

	//if you shortened the delay length, you'll want to clear the samples that you just decided to take out
	void clearUnused(void) {

		if (delaySizeOld > delaySize) {
			for (int i = delaySize; i < delaySizeOld; i++) {
				//make sure you don't go out of bounds.
				if (i < DELAYSIZE) {
					delay[i] = 0.0f;
				}
			}
		}

		delaySizeOld = delaySize;

	}

	//Put into preparetoplay..
	void prepareToPlay(int sampleRate) {

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
        controlFilter.setSamplingFreq(sampleRate);
        controlFilter.setFc(1.0f);
        modOsc.setSamplingFreq(sampleRate);
		modOsc.setF(0.1f, 100.0f);
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
		if ((delayReadIndex >= DELAYSIZE) && (delayReadIndex >= 0)) {
			delayReadIndex = delayWriteIndex + int(delaySize) - DELAYSIZE;
		}

	}

	float read(void) {
		if ((delayReadIndex < DELAYSIZE) && (delayReadIndex >= 0)) {
			if (delayReadIndex + 1 < DELAYSIZE) {
				return linterp(delay[delayReadIndex], delay[delayReadIndex + 1], delaySizeFrac);
			}
			else {
				return delay[delayReadIndex];
			}
		}
		else {
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


	float cosinterp(float y1, float y2, float frac) {
		float frac2;
		frac2 = (1 - cos(frac*float(3.14159))) / 2;
		return(y1*(1 - frac2) + y2*frac2);
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
