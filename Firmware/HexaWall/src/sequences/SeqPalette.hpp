// SeqPalette.h

#ifndef _SEQPALETTE_H
#define _SEQPALETTE_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <FastLED.h>
#include "Sequence.hpp"

class SeqPalette : public Sequence
{
private:
	CRGBPalette16 currentPalette;
	TBlendType    currentBlending;

public:
	SeqPalette(CRGBPalette16 palette, TBlendType blending, struct CRGB* data, int nLedsOrOffset) : Sequence(data, nLedsOrOffset)
	{
		currentPalette = palette;
		currentBlending = blending;
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

	void setPalette(CRGBPalette16 palette, TBlendType blending)
	{
		currentPalette = palette;
		currentBlending = blending;
	}

};

#endif

