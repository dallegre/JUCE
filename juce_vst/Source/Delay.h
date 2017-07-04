#pragma once

#define DELAYSIZE 48000

class Delay {

public:

	//actual delay buffer
	float delay[2][DELAYSIZE];

	int delaySize;
	int delaySizeOld;
	int delayReadIndex[2];
	int delayWriteIndex[2];

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
		
		//clear the delay buffer
		for (int i = 0; i < DELAYSIZE; i++) {
			for (int lr = 0; lr < 2; lr++) {
				delay[lr][i] = 0.0f;
			}
		}

	}

	//update index.  operates on delayReadIndex and delayWJriteIndex, which are arrays of 2 for left/right indeces.
	void updateIndex(float delayVal, int channel) {
		
		delayWriteIndex[channel] += 1;
		
		int delaySize = int(DELAYSIZE * delayVal);
		if (delayWriteIndex[channel] >= delaySize) {
			delayWriteIndex[channel] = 0;
		}
		
		delayReadIndex[channel] = delayWriteIndex[channel] + 1;
		if (delayReadIndex[channel] >= delaySize) {
			delayReadIndex[channel] = 0;
		}

	}

	float read(int channel) {
		if (delayWriteIndex[channel] < DELAYSIZE & delayReadIndex[channel] < DELAYSIZE) {
			return delay[channel][delayReadIndex[channel]];
		}
		else {
			return 0.0f;
		}
	}

	void write(int channel, float input) {
		if (delayWriteIndex[channel] < DELAYSIZE & delayReadIndex[channel] < DELAYSIZE) {
			delay[channel][delayWriteIndex[channel]] = input;
		}
	}

};
