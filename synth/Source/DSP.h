//
//  DSP.h
//  synth
//
//  Created by Mac on 7/17/17.
//
//

#ifndef DSP_h
#define DSP_h

#define SAMPLINGFREQ 44100
#include <math.h>

class Osc{
public:
    Osc(){};
    ~Osc(){};
    
    void setF(float freq){
        phase = freq / SAMPLINGFREQ;
    }
    
    void prepareToPlay(void){
        saw = -0.5f;
        phase = 100.0f / SAMPLINGFREQ;
    }
    
    float process(void){
        saw += phase;
        if(saw > 0.5f){
            saw = -0.5f;
        }
        return saw;
    }
    
private:
    float phase, saw;
};

class Filter{
public:
    Filter(){};
    
    ~Filter(){};
    
    void setFc(float freq){
        b1 = exp(-2.0f * 3.14159f * freq / SAMPLINGFREQ);
        a0 = 1.0f - b1;
    }
    
    void setQ(float Q){
        q = Q * 4.0f;
    }

    void prepareToPlay(void){
        setFc(200.0f);
        setQ(0.0f);
        z1 = 0.0;
        z2 = 0.0;
        z3 = 0.0;
        feedback = 0.0;
        lp = 0.0;
    }
    
    float process(float input){
        z1 = (input - q*feedback)*a0 + z1*b1;
		z1 = tanh(z1);
        z2 = z1*(1 - (b1 * 1.00002)) + z2*(b1 * 1.00002);
		//z2 = tanh(z2);
        z3 = z2*(1 - (b1 * 0.9994)) + z3*(b1 * 0.9994);
		//z3 = tanh(z3);
        lp = z3*(1 - (b1 * 1.00005)) + feedback*(b1 * 1.00005); 
		//lp = tanh(lp);
        feedback = lp;
		lp *= 1 + q / 3.0f;
        return lp;
    }
    
private:
    float b1, a0;
    float z1, z2, z3, feedback, lp, q;
};


class OnePoleLP{
public:
    OnePoleLP(){};
    ~OnePoleLP(){};
    
    void prepareForPlay(void){
        setFc2(300.0f);
        z1 = 0.0f;
    }
    
    void setFc2(float freq){
        b1 = exp(-2.0f * 3.14159f * freq / SAMPLINGFREQ);
        a0 = 1.0f - b1;
    }
    
    float process(float input){
        z1 = a0*input + b1*z1;
        return z1;
    }
    
private:
    float a0, b1, z1;
};


class gsOsc {
public:
    
    gsOsc() {}
    
    ~gsOsc() {};
    
    void setF(float frequency, float amp) {
        fw = float(2.0)*pi*frequency / fs;
        eps = float(2.0)*sin(fw / float(2.0));
        amp2 = amp;
    }
    
    void prepareForPlay(void){
        yn = 0;					//initial condition sine(0) = 0
        yq = 1;					//initial condition cos(0) = 1
        yn_1 = 1, yq_1 = 0;     //hmm...
        pi = 3.14159;
        fs = SAMPLINGFREQ;
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
    float yn, yq, yn_1, yq_1, fw, pi, fs, eps, amp2;
    
};



class Env{
public:
    Env(){};
    ~Env(){};
    
    void prepareToPlay(void){
        val = 0.0f;
        speed = 0.1f;
    }
    
    void reset(void){
        val = 1.0f;
    }
    
    void setSpeed(float Speed){
        speed = Speed / SAMPLINGFREQ * 20.0f;
    }
    
    float process(void){
        val -= speed;
        if(val < speed){
            val = 0;
        }
        return val;
    }
    
private:
    float val, speed;
};


#endif /* DSP_h */
