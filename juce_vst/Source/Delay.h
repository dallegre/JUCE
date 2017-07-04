#pragma once

#include "DSP.h"

#define DELAYSIZE 24000

class Delay {

public:

	Delay(void) {}

	~Delay(void) {}

	//if you shortened the delay length, you'll want to clear the samples that you just decided to take out
	void clearUnused(int channel) {

		if (delaySizeOld > delaySize) {
			for (int i = delaySize; i < delaySizeOld; i++) {
				//make sure you don't go out of bounds.
				if (i < DELAYSIZE) {
					delay[channel][i] = 0.0f;
				}
			}
		}

		delaySizeOld = delaySize;

	}

	//Put into preparetoplay..
	void prepareToPlay(void) {

		//initialize all variables
		delayWriteIndex[0] = 0;
		delayWriteIndex[1] = 0;
		delayReadIndex[0] = 0;
		delayReadIndex[1] = 0;
		delaySize = 0;
		delaySizeOld = 0;

		delaySizeFrac = 0.0f;
		lpVal = 0.0f;

		//clear the delay buffer
		for (int i = 0; i < DELAYSIZE; i++) {
			for (int lr = 0; lr < 2; lr++) {
				delay[lr][i] = 0.0f;
			}
		}

		//filter for smoothing the delay time control
		controlFilter.setFc(1.0f / (48000));
		modOsc.setF(48000 * 0.1f, 100.0f);
	}

	//update index.  operates on delayReadIndex and delayWJriteIndex, which are arrays of 2 for left/right indeces.
	void updateIndex(float delayVal, float modAmt, float modFreq, int channel) {

		delayWriteIndex[channel]++;
		delayReadIndex[channel]++;
		if (delayWriteIndex[channel] >= DELAYSIZE) {
			delayWriteIndex[channel] = 0;
		}
		if (delayReadIndex[channel] >= DELAYSIZE) {
			delayReadIndex[channel] = 0;
		}

		delaySize = float(DELAYSIZE) - float(DELAYSIZE) * delayVal;
		delaySize = controlFilter.process(delaySize);
		modOsc.setF(48000 * modFreq, modAmt);
		delaySize += modOsc.process(0);
		delaySizeFrac = delaySize - int(delaySize);


		delayReadIndex[channel] = delayWriteIndex[channel] + int(delaySize);
		if (delayReadIndex[channel] >= DELAYSIZE) {
			delayReadIndex[channel] = delayWriteIndex[channel] + int(delaySize) - DELAYSIZE;
		}

	}

	float read(int channel) {
		if (delayReadIndex[channel] < DELAYSIZE) {
			if (delayReadIndex[channel] + 1 < DELAYSIZE) {
				return linterp(delay[channel][delayReadIndex[channel]], delay[channel][delayReadIndex[channel] + 1], delaySizeFrac);
			}
			else {
				return delay[channel][delayReadIndex[channel]];
			}
		}
		else {
			return 0.0f;
		}
	}

	void write(int channel, float input) {
		if (delayWriteIndex[channel] < DELAYSIZE) {
			delay[channel][delayWriteIndex[channel]] = input;
		}
	}

	float linterp(float y1, float y2, float frac) {
		return (y1*(1.0f - frac) + y2*frac);
	}

private:

	//actual delay buffer
	float delay[2][DELAYSIZE];

	int delayReadIndex[2];
	int delayWriteIndex[2];

	//counting on a reasonable conversion from float to int
	float delaySize;
	float delaySizeOld;

	float delaySizeFrac;
	float lpVal;

	OnePoleLp controlFilter;
	gsOsc modOsc;

};
