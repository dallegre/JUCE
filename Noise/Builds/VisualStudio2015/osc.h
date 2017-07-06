class gsOsc {
public:
	gsOsc() {
		yn = 0;         //initial condition sine(0) = 0
		yq = 0;         //initial condition cos(0) = 1
		yn_1 = 1, yq_1 = 0;     //hmm...
		pi = 3.14159f;
	}
	void setF(float frequency, float amp);
	float process(int quad);
protected:
	//put variables here that you'll use within process
	float yn, yq, yn_1, yq_1, fw, pi, eps, amp2;
};

inline float gsOsc::process(int quad) {
	yq = yq_1 - eps*yn_1;
	yn = eps*yq + yn_1;
	yn_1 = yn;
	yq_1 = yq;
	if (quad)
		return yn*amp2;
	else
		return yq*amp2;
}

void gsOsc::setF(float frequency, float amp) {
	fw = 2.0f*pi*frequency;
	eps = 2.0f*sin(fw / 2.0f);
	amp2 = amp;
}