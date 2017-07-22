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
        z2 = z1*(1 - (b1 * 1.0001)) + z2*(b1 * 1.0001);
		z2 = tanh(z2);
        z3 = z2*(1 - (b1 * 0.999)) + z3*(b1 * 0.999);
		z3 = tanh(z3);
        lp = z3*(1 - (b1 * 1.00005)) + feedback*(b1 * 1.00005); 
		lp = tanh(lp);
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
    
    void setFc2(float freqArg){
        b1 = exp(-2.0f * 3.14159f * freqArg / SAMPLINGFREQ);
        a0 = 1.0f - b1;
    }
    
    float process(float input){
        z1 = a0*input + b1*z1;
        return z1;
    }
    
private:
    float a0, b1, z1;
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
