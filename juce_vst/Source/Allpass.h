#pragma once

#include "Delay.h"

class Allpass {

public:

	Allpass() {};
	~Allpass() {};

	float process(float input, float delayVal, float oscAmtValScaled, float oscFreqValScaled, float feedbackVal, int quad) {
		delay.updateIndex(delayVal, oscAmtValScaled, oscFreqValScaled, quad);
		feedback = input + feedbackVal * delay.read();
		delay.write(feedback);
		return delay.read() - feedbackVal * feedback;
	}

	void prepareToPlay(void){
		delay.prepareToPlay();
		feedback = 0.0f;
	}

private:

	Delay delay;
	float feedback;

};
