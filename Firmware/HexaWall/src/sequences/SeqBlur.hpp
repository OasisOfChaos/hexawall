// SeqBlur.h

#pragma once

#include "arduino.h"
#include <FastLED.h>
#include "Sequence.hpp"

class SeqBlur : public Sequence
{
private:
	void secondTick() {}

public:
	SeqBlur(struct CRGB* data, int nLedsOrOffset) : Sequence(data, nLedsOrOffset) {}

	//String Name() { return "Blur"; }

	void tick()
	{
		uint16_t blurAmount = dim8_raw(beatsin8(3, 64, 192));		// A sinewave at 3 Hz with values ranging from 64 to 192.
		blur1d(LEDArray, LEDCount, blurAmount);						// Apply some blurring to whatever's already on the strip, which will eventually go black.

		uint16_t  i = beatsin16(9, 0, LEDCount);
		uint16_t  j = beatsin16(7, 0, LEDCount);
		uint16_t  k = beatsin16(5, 0, LEDCount);

		// The color of each point shifts over time, each at a different speed.
		uint16_t ms = millis();
		LEDArray[(i + j) / 2] = CHSV(ms / 29, 200, 255);
		LEDArray[(j + k) / 2] = CHSV(ms / 41, 200, 255);
		LEDArray[(k + i) / 2] = CHSV(ms / 73, 200, 255);
		LEDArray[(k + i + j) / 3] = CHSV(ms / 53, 200, 255);
	}

};