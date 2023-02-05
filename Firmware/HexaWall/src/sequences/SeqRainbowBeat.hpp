// SeqRainbowBeat.h

#pragma once

#include "arduino.h"
#include <FastLED.h>
#include "Sequence.hpp"

class SeqRainbowBeat : public Sequence
{
private:
	void secondTick() {}

public:
	SeqRainbowBeat(struct CRGB* data, int nLedsOrOffset) : Sequence(data, nLedsOrOffset) {}

	void tick()
	{
		uint8_t beatA = beatsin8(17, 0, 255);                        // Starting hue
		uint8_t beatB = beatsin8(13, 0, 255);
		fill_rainbow(LEDArray, LEDCount, (beatA + beatB) / 2, 8);            // Use FastLED's fill_rainbow routine.
	}

};

/* rainbow_beat
 *
 * By: Andrew Tuline
 *
 * Date: July, 2015
 *
 * A rainbow moving back and forth using FastLED function calls with no delays or 'for' loops. Very simple, yet effective.
 *
 */
