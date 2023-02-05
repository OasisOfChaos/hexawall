// SeqBlendwave.h

#pragma once

#include "arduino.h"
#include <FastLED.h>
#include "Sequence.hpp"

class SeqBlendwave : public Sequence
{
private:
	uint8_t max_bright = 128;                                      // Overall brightness definition. It can be changed on the fly.

	CRGB clr1;
	CRGB clr2;
	uint8_t speed;
	uint8_t loc1;
	uint8_t loc2;
	uint8_t ran1;
	uint8_t ran2;

	void secondTick() {}

public:
	SeqBlendwave(struct CRGB* data, int nLedsOrOffset) : Sequence(data, nLedsOrOffset) {}

	//String Name() { return "Blendwave"; }

	void tick()
	{
		speed = beatsin8(6, 0, 255);

		clr1 = blend(CHSV(beatsin8(3, 0, 255), 255, 255), CHSV(beatsin8(4, 0, 255), 255, 255), speed);
		clr2 = blend(CHSV(beatsin8(4, 0, 255), 255, 255), CHSV(beatsin8(3, 0, 255), 255, 255), speed);

		loc1 = beatsin8(10, 0, LEDCount - 1);

		fill_gradient_RGB(LEDArray, 0, clr2, loc1, clr1);
		fill_gradient_RGB(LEDArray, loc1, clr2, LEDCount - 1, clr1);
	}

};