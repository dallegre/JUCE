class OnePole {
public:
	OnePole() { a0 = 1.0; b1 = 0.0; z1 = 0.0; };
	OnePole(float Fc) { z1 = 0.0; setFc(Fc); };
	//~OnePole();

	void setFc(float Fc) {
		b1 = float(exp(-2.0f * 3.14159f * Fc));
		a0 = 1.0f - b1;
	}

	float process(float in) {
		return z1 = in * a0 + z1 * b1;
	}

	protected:
	float a0, b1, z1;
};
