#pragma once
class Easing
{
private:
	int timer;
	int maxTimer;
	double rate;

	static double easeBasic(const int &timer,const int &maxTimer);
	static double easeInSine(const int &timer, const int &maxTimer);
	static double easeOutSine(const int &timer, const int &maxTimer);
	static double easeinOutSine(const int &timer, const int &maxTimer);
	static double easeInQuad(const int &timer, const int &maxTimer);
	static double easeOutQuad(const int &timer, const int &maxTimer);
	static double easeInOutQuad(const int &timer, const int &maxTimer);
	static double easeInCubic(const int &timer, const int &maxTimer);
	static double  easeOutCubic(const int &timer, const int &maxTimer);
	static double easeInOutCubic(const int &timer, const int &maxTimer);
	static double easeInQuart(const int &timer, const int &maxTimer);
	static double easeOutQuart(const int &timer, const int &maxTimer);
	static double easeInOutQuart(const int &timer, const int &maxTimer);
	static double easeInQuint(const int &timer, const int &maxTimer);
	static double easeOutQuint(const int &timer, const int &maxTimer);
	static double easeInOutQuint(const int &timer, const int &maxTimer);
	static double easeInExpo(const int &timer, const int &maxTimer);
	static double easeOutExpo(const int &timer, const int &maxTimer);
	static double easeInOutExpo(const int &timer, const int &maxTimer);
	static double easeInCirc(const int &timer, const int &maxTimer);
	static double easeOutCirc(const int &timer, const int &maxTimer);
	static double easeInOutCirc(const int &timer, const int &maxTimer);
	static double easeInBack(const int &timer, const int &maxTimer);
	static double easeOutBack(const int &timer, const int &maxTimer);
	static double easeInOutBack(const int &timer, const int &maxTimer);
	static double easeInElastic(const int &timer, const int &maxTimer);
	static double easeOutElastic(const int &timer, const int &maxTimer);
	static double easeInOutElastic(const int &timer, const int &maxTimer);
	static double easeInBounce(const int &timer, const int &maxTimer);
	static double easeOutBounce(const int &timer, const int &maxTimer);
	static double easeInOutBounce(const int &timer, const int &maxTimer);

public:
	static const enum InOut
	{
		In,
		Out,
		InOut
	};

	static const enum Type
	{
		Sine,
		Quad,
		Cubic,
		Quart,
		Quint,
		Expo,
		Circ,
		Back,
		Elastic,
		Bounce
	};
	Easing();

	//�C�[�W���O�v�Z
	static double EaseCalc(int InOrOut, int type, const int &rate, const int &max);

	//�C�[�W���O�̑J��
	double Do(int InOrOut, int type);


	double Linear();
	//���l�̏�����
	void Init(int max = 1);

	//���l�̑J�ڂ��������Ă��邩���m�F����
	bool IsEnd();

	double Read();
	void Reset();

};