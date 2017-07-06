//1 pole highpass filter
class OnePoleHp {
public:
	OnePoleHp() { a0 = 1.0f; b1 = 0.0f; z1 = 0.0f; };
	OnePoleHp(float Fc) { z1 = 0.0f; setFc(Fc); };
	//~OnePole();
	void setFc(float Fc);
	float process(float in);
protected:
	float a0, b1, z1;
};
inline void OnePoleHp::setFc(float Fc) {
	b1 = -exp(-2.0f * 3.14159f * (0.5f - Fc));
	a0 = 1.0f + b1;
}
inline float OnePoleHp::process(float in) {
	return z1 = in * a0 + z1 * b1;
}