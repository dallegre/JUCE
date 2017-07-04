#include <math.h>

class OnePoleLp {

public:

	OnePoleLp() { a0 = 1.0f; b1 = 0.0f; z1 = 0.0f; };
	OnePoleLp(float Fc) { z1 = 0.0; setFc(Fc); };
	~OnePoleLp(){};

	void setFc(float Fc) {
		b1 = exp(-2.0f * 3.14159f * Fc);
		a0 = 1.0f - b1;
	}

	float process(float in) {
		z1 = in * a0 + z1 * b1;
		return z1;
	}

private:
	
	float a0, b1, z1;

};


//make class for gordon smith oscillator
class gsOsc {
public:
	
	gsOsc() {
		yn = 0;					//initial condition sine(0) = 0
		yq = 0;					//initial condition cos(0) = 1
		yn_1 = 1, yq_1 = 0;     //hmm...
		pi = 3.14159;
		fs = 48000 * 0.1f;
	}

	~gsOsc() {};

	void setF(float frequency, float amp) {
		fw = float(2.0)*pi*frequency / fs;
		eps = float(2.0)*sin(fw / float(2.0));
		amp2 = amp;
	}

	float process(int quad) {
		yq = yq_1 - eps*yn_1;
		yn = eps*yq + yn_1;
		yn_1 = yn;
		yq_1 = yq;
		if (quad)
			return yn*amp2;
		else
			return yq*amp2;
	}

private:

	//put variables here that you'll use within process
	float yn, yq, yn_1, yq_1, fw, pi, fs, eps, amp2;

};
