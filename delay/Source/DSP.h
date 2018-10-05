#include <math.h>


class OnePoleLp {

public:

	OnePoleLp(){};
	~OnePoleLp() {};
    
    void prepareToPlay(void){
        a0 = 1.0f; b1 = 0.0f; z1 = 0.0f;
    }
    
    void setSamplingFreq(int sf){
        SAMPLINGFREQ = sf;
    }

	void setFc(float Fc) {
		b1 = exp(-2.0f * 3.14159f * Fc / SAMPLINGFREQ);
		a0 = 1.0f - b1;
	}

	float process(float in) {
		z1 = in * a0 + z1 * b1;
		return z1;
	}

private:
	
	float a0, b1, z1;
    int SAMPLINGFREQ;

};


//make class for gordon smith oscillator
class gsOsc {
public:
	
	gsOsc() {
		yn = 0;					//initial condition sine(0) = 0
		yq = 1;					//initial condition cos(0) = 1
		yn_1 = 1, yq_1 = 0;     //hmm...
		pi = 3.14159;
	}

	~gsOsc() {};
    
    void setSamplingFreq(int sf){
        SAMPLINGFREQ = sf;
    }

	void setF(float frequency, float amp) {
		fw = float(2.0)*pi*frequency / SAMPLINGFREQ;
		eps = float(2.0)*sin(fw / float(2.0));
		amp2 = amp;
	}

	float process(int quad) {
		yq = yq_1 - eps*yn_1;
		yn = eps*yq + yn_1;
		yn_1 = yn;
		yq_1 = yq;
		//check to see if it's going crazy
		if ((yq > 3.0f) || (yq < -3.0f)) {
			yn = 0; yq = 1; yn_1 = 1; yq_1 = 0;
			return 0;
		}
		else {
			if (quad)
				return yn*amp2;
			else
				return yq*amp2;
		}
	}

private:

	//put variables here that you'll use within process
	float yn, yq, yn_1, yq_1, fw, pi, eps, amp2;
    int SAMPLINGFREQ;

};

class stateVariable{

public:

	//maybe put stuff like this in preparetoplay instead.
	stateVariable(){
	}

	~stateVariable(){};

	void prepareToPlay() {
		f = 0.0f;
		q = 0.0f;
		hp = 0.0f;
		bp = 0.0f;
		lp = 0.0f;
		bp_1 = 0.0f;
		lp_1 = 0.0f;
	}
    
    void setSamplingFreq(int sf){
        SAMPLINGFREQ = sf;
    }

	void setFc(float fc){
		f = 2.0f * sin(3.14159f * fc / SAMPLINGFREQ);
	}	

	//override for upsampling
	void setFc(float fc, int upSamp) {
		f = 2.0f * sin(3.14159f * fc / (SAMPLINGFREQ * upSamp));
	}

	void setQ(float Q){	//range of q should be 1.0 to 0.0
		if (Q < 0.1) {
			Q = 0.1;
		}
		q = 0.05f * (1/(Q/2.0));
	}

	float processLP(float input){
		//calculate filte outputs
		hp = input - (q*bp_1) - lp;
		bp = bp_1 + (f*hp);
		lp = lp_1 + (f*bp_1);
		//do integration for next time around
		lp_1 = lp;
		bp_1 = bp;
        return lp;
	}
    
    float processHP(float input){
        //calculate filte outputs
        hp = input - (q*bp_1) - lp;
        bp = bp_1 + (f*hp);
        lp = lp_1 + (f*bp_1);
        //do integration for next time around
        lp_1 = lp;
        bp_1 = bp;
        return hp;
    }

private:

	float f, q, hp, bp, lp, bp_1, lp_1;
    int SAMPLINGFREQ;
    
};
