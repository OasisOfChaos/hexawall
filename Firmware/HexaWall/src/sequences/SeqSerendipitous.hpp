// SeqSerendipitous.h

#pragma once

#include "arduino.h"
#include <FastLED.h>
#include "Sequence.hpp"

class SeqSerendipitous : public Sequence
{
private:
	uint8_t maxChanges = 24;      // Value for blending between palettes.

	CRGBPalette16 currentPalette;
	CRGBPalette16 targetPalette;
	TBlendType    currentBlending;                                // NOBLEND or LINEARBLEND

	uint16_t Xorig = 0x012;
	uint16_t Yorig = 0x015;
	uint16_t X;
	uint16_t Y;
	uint16_t Xn;
	uint16_t Yn;
	uint8_t index;

public:
	SeqSerendipitous(struct CRGB* data, int nLedsOrOffset) : Sequence(data, nLedsOrOffset) {
		currentBlending = LINEARBLEND;

		X = Xorig;
		Y = Yorig;
	}

	void tick()
	{
		EVERY_N_MILLISECONDS(60)
		{
			nblendPaletteTowardPalette(currentPalette, targetPalette, maxChanges);  // Blend towards the target palette
		}

		EVERY_N_MILLISECONDS(50)
		{
			serendipitous();
		}
	}

	void serendipitous()
	{

		EVERY_N_SECONDS(5)
		{
			uint8_t baseC = random8();
			targetPalette = CRGBPalette16(CHSV(baseC - 3, 255, random8(192, 255)), CHSV(baseC + 2, 255, random8(192, 255)), CHSV(baseC + 5, 192, random8(192, 255)), CHSV(random8(), 255, random8(192, 255)));

			X = Xorig;
			Y = Yorig;
		}

		//  Xn = X-(Y/2); Yn = Y+(Xn/2);
		//  Xn = X-Y/2;   Yn = Y+Xn/2;
		//  Xn = X-(Y/2); Yn = Y+(X/2.1);
		Xn = X - (Y / 3); Yn = Y + (X / 1.5);
		//  Xn = X-(2*Y); Yn = Y+(X/1.1);

		X = Xn;
		Y = Yn;

		index = (sin8(X) + cos8(Y)) / 2;                            // Guarantees maximum value of 255

		CRGB newcolor = ColorFromPalette(currentPalette, index, 255, LINEARBLEND);

		//  nblend(leds[X%LEDCount-1], newcolor, 224);          // Try and smooth it out a bit. Higher # means less smoothing.
		nblend(LEDArray[map(X, 0, 65535, 0, LEDCount)], newcolor, 224); // Try and smooth it out a bit. Higher # means less smoothing.

		fadeToBlackBy(LEDArray, LEDCount, 16);                    // 8 bit, 1 = slow, 255 = fast

	} // serendipitous()
};

/* serendipitous
 *
 *  By: Andrew Tuline
 *
 *  Date: August, 2016
 *
 *  This is from the Serendipitous Circles article in the August 1977 and April 1978 issues of Byte Magazine.
 *  This is a poorly rendered 1D version and I really should get around to converting it to 2D and buying a 32x32 matrix to display it. Some day . .
 *
 *  Check the magazine article out online, as this 1D routine doesn't do it justice.
 *
 *
 *  Base Routine is:
 *
 *  Choose X, Y
 *  Top: Display point
 *  Xn = X-Y/2
 *  Yn = Y+Xn/2
 *  X = Xn
 *  Y = Yn
  *  Goto top
 *
 *  Other maths are:
 *  Xn = X-Y/2,   Yn = Y+Xn/2
 *  Xn = X-(Y/2), Yn = Y+(X/2.1)
 *  Xn = X-(Y/3), Yn = Y+(X/1.5)
 *  Xn = X-(2*Y), Yn = Y+(X/1.1)
 *
 */