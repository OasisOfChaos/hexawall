// SeqDotBeat.h

#pragma once

#include "arduino.h"
#include <FastLED.h>
#include "Sequence.hpp"

class SeqDotBeat : public Sequence
{
private:
	int   thisdelay = 10;                                       // A delay value for the sequence(s)
	uint8_t  count = 0;                                        // Count up to 255 and then reverts to 0
	uint8_t fadeval = 224;                                        // Trail behind the LED's. Lower => faster fade.

	uint8_t bpm = 30;

	void secondTick() {}
public:
	SeqDotBeat(struct CRGB* data, int nLedsOrOffset) : Sequence(data, nLedsOrOffset) {}

	void tick()
	{
		uint16_t inner = beatsin16(bpm, LEDCount / 4, LEDCount / 4 * 3);		// Move 1/4 to 3/4
		uint16_t outer = beatsin16(bpm, 0, LEDCount - 1);						// Move entire length
		uint16_t middle = beatsin16(bpm, LEDCount / 3, LEDCount / 3 * 2);		// Move 1/3 to 2/3

		LEDArray[middle] = CRGB::Purple;
		LEDArray[inner] = CRGB::Blue;
		LEDArray[outer] = CRGB::Aqua;

		nscale8(LEDArray, LEDCount, fadeval);									// Fade the entire array. Or for just a few LED's, use  nscale8(&LEDArray[2], 5, fadeval);
	}

};