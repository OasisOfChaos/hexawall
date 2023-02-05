#pragma once

#include "fled.h"

class Sequence
{
private:


protected:
	int LEDCount;
	CRGB* leds;
	bool secondPassed = false;


public:
	Sequence(struct CRGB* data, int nLedsOrOffset);
	~Sequence();

	virtual void tick() = 0;	//pure virtual 'abstract' method
	virtual void tock() = 0;
	void test(struct CRGB* data, int count);
};



class SequenceOne : public Sequence
{
public:
	SequenceOne(struct CRGB* data, int nLedsOrOffset) : Sequence(data, nLedsOrOffset) {}
	~SequenceOne();

	void tick();
	void tock();
};

class SequenceTwo : public Sequence
{
public:
	SequenceTwo(struct CRGB* data, int nLedsOrOffset);
	//~SequenceTwo();

	void tick();
	void tock();
};
