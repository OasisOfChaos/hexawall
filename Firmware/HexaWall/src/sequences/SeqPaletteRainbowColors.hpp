// SeqPaletteRainbowColors.h

#ifndef _SEQPALETTERAINBOWCOLORS_H
#define _SEQPALETTERAINBOWCOLORS_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <FastLED.h>
#include "Sequence.hpp"

class SeqPaletteRainbowColors : public Sequence
{
private:
	CRGBPalette16 currentPalette;
	TBlendType    currentBlending;

public:
	SeqPaletteRainbowColors(struct CRGB* data, int nLedsOrOffset) : Sequence(data, nLedsOrOffset)
	{
		currentPalette = RainbowColors_p;
		currentBlending = LINEARBLEND;
	}

	void tick()
	{
		static uint8_t colorIndex = 0;
		colorIndex = colorIndex + 1; /* motion speed */

		for (int i = 0; i < LEDCount; i++)
		{
			LEDArray[i] = ColorFromPalette(currentPalette, colorIndex, LEDBrightness, currentBlending);
			colorIndex += 3;
		}
	}
};

#endif

