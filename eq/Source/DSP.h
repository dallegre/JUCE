#include <math.h>

//Taken from Will Pirkle's book.  Peaking is a non constant q peaking filter.

//#define SAMPLINGFREQ 44100

class Peaking{
public:
 
    Peaking(){};
    ~Peaking(){};
    
    void prepareToPlay(void){
        
        setFc(100.0f, 0.0f, 0.5f);
        
        a1   = 0.0f;
        d0   = 1.0f;
        
        ip_1 = 0.0f;
        ip_2 = 0.0f;
        op_1 = 0.0f;
        op_2 = 0.0f;
        wet  = 0.0f;
        op   = 0.0f;
        
    }
    
    void setSamplingFreq(int sf){
        SAMPLINGFREQ = sf;
    }
    
    void setFc(float freq, float amp, float Q){
        
        q = 0.5f + 3.0f * Q;
        
        amp *= 30.0f;
        
        mu = pow(10.0f, amp / 20.0f);
        zeta = 4.0f / (1.0f + mu);
        fw = 2 * 3.14159 * freq / SAMPLINGFREQ;
        //need to recalculate these when changing either frequency, amp or q
        beta = 0.5f * ((1 - zeta * tan(fw / (2*q))) / (1 + zeta * tan(fw / (2*q))));
        phi = (0.5f + beta) * cos(fw);
        
        a0 = 0.5f - beta;
        a2 = -(0.5f - beta);
        
        b1 = -2.0f * phi;
        b2 = 2.0f * beta;
        
        c0 = mu - 1.0f;
        
    }
    
    float process(float ip){
        
        wet = (a0 * ip) + (a2 * ip_2) - (b1 * op_1) - (b2 * op_2);
        
        op =  (d0 * ip) + (c0 * wet);
        
        //calculate delayed values
        ip_2 = ip_1;
        ip_1 = ip;
        op_2 = op_1;
        op_1 = wet;
        
        return op;

    }
    
private:
    float fw, mu, beta, zeta, phi, a0, a1, a2, b1, b2, c0, d0, q, op;
    float ip_1, ip_2, op_1, op_2, wet;
    int SAMPLINGFREQ;
};
