// SeqSawTooth.h

#pragma once

#include "arduino.h"
#include <FastLED.h>
#include "Sequence.hpp"

class SeqSawTooth : public Sequence
{
private:
	CRGBPalette16 currentPalette;                                   // Use palettes instead of direct CHSV or CRGB assignments
	CRGBPalette16 targetPalette;                                    // Also support smooth palette transitioning
	TBlendType    currentBlending;                   // NOBLEND or LINEARBLEND


public:
	SeqSawTooth(struct CRGB* data, int nLedsOrOffset) : Sequence(data, nLedsOrOffset) {
		currentPalette = OceanColors_p;
		targetPalette = OceanColors_p;
		currentBlending = LINEARBLEND;
	}

	//a bit dull. maybe add palettes?

	void tick()
	{
		int bpm = 60;
		int ms_per_beat = 60000 / bpm;                                // 500ms per beat, where 60,000 = 60 seconds * 1000 ms
		int ms_per_led = 60000 / bpm / LEDCount;

		int cur_led = ((millis() % ms_per_beat) / ms_per_led) % (LEDCount);     // Using millis to count up the strand, with %NUM_LEDS at the end as a safety factor.

		if (cur_led == 0)
			fill_solid(LEDArray, LEDCount, CRGB::Black);
		else
			LEDArray[cur_led] = ColorFromPalette(currentPalette, 0, 255, currentBlending);    // I prefer to use palettes instead of CHSV or CRGB assignments.
	}

};

/* Sawtooth
 *
 * By: Andrew Tuline
 *
 * Date: February, 2018
 *
 * Creating a sawtooth pattern using millis and bpm. It runs at about 440 fps with the configuration below on an Arduino Nano.
 *
 */