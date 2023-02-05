// SeqMover.h

#pragma once

#include "arduino.h"
#include <FastLED.h>
#include "Sequence.hpp"

class SeqMover : public Sequence
{
private:
	// have 3 independent CRGBs - 2 for the sources and one for the output
	CRGB leds[750];
	CRGB leds2[750];

	// Palette definitions
	CRGBPalette16 currentPalette;
	CRGBPalette16 targetPalette;
	TBlendType    currentBlending;                                // NOBLEND or LINEARBLEND

	void secondTick() {}

public:
	SeqMover(struct CRGB* data, int nLedsOrOffset) : Sequence(data, nLedsOrOffset)
	{
		currentBlending = LINEARBLEND;
		currentPalette = RainbowColors_p;
		targetPalette = ForestColors_p;
	}

	void tick()
	{
		EVERY_N_MILLISECONDS(100)
		{
			uint8_t maxChanges = 24;
			nblendPaletteTowardPalette(currentPalette, targetPalette, maxChanges);   // AWESOME palette blending capability.
		}

		EVERY_N_SECONDS(5)
		{
			// Change the target palette to a random one every 5 seconds.

			// You can use this as a baseline colour if you want similar hues in the next line.
			static uint8_t baseC = random8();

			targetPalette = CRGBPalette16(CHSV(random8(), 255, random8(128, 255)), CHSV(random8(), 255, random8(128, 255)), CHSV(random8(), 192, random8(128, 255)), CHSV(random8(), 255, random8(128, 255)));
		}

		animationA();                                               // render the first animation into leds2
	}

	void animationA()
	{                                             // running red stripe.
		for (int i = 0; i < LEDCount; i++)
		{
			uint8_t red = (millis() / 5) + (i * 12);                    // speed, length
			if (red > 128) red = 0;
			LEDArray[i] = ColorFromPalette(currentPalette, red, red, currentBlending);
		}
	} // animationA()
};