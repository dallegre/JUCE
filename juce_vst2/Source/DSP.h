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
