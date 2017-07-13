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

class stateVariable{

public:

	//maybe put stuff like this in preparetoplay instead.
	stateVariable(){
		f = 0.0f;
		q = 0.0f;
		hp = 0.0f;
		bp = 0.0f;
		lp = 0.0f;
		bp_1 = 0.0f;
		lp_1 = 0.0f;
	}

	~stateVariable(){};

	void setFc(float fc){
		f = 2.0f * sin(3.14159f * fc / 48000);
	}	

	void setQ(float Q){	//range of q should be 1.0 to 0.0
		q = 4.0f * 1/(Q + 1.0);
	}

	float process(float input, int type){
		//calculate filte outputs
		hp = input - (q*bp_1) - lp;
		bp = bp_1 + (f*hp);
		lp = lp_1 + (f*bp_1);
		//do integration for next time around
		lp_1 = lp;
		bp_1 = bp;
		if(type == 0){
			return lp;
		}else if(type == 1){
			return bp;
		}else if(type == 2){
			return hp;
		}
	}

private:

	float f, q, hp, bp, lp, bp_1, lp_1;
};
