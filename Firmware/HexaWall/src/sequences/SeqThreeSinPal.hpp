// SeqThreeSinPal.h

#pragma once

#include "arduino.h"
#include <FastLED.h>
#include "Sequence.hpp"

class SeqThreeSinPal : public Sequence
{
private:
	int _secondCounter = 0;

	int8_t thisdelay = 20;                                       // A delay value for the sequence(s)

	int wave1 = 0;                                                  // Current phase is calculated.
	int wave2 = 0;
	int wave3 = 0;

	uint8_t lvl1 = 80;                                            // Any result below this value is displayed as 0.
	uint8_t lvl2 = 80;
	uint8_t lvl3 = 80;

	uint8_t mul1 = 7;                                            // Frequency, thus the distance between waves
	uint8_t mul2 = 6;
	uint8_t mul3 = 5;


	CRGBPalette16 currentPalette;
	CRGBPalette16 targetPalette;

	void secondTick() {
		CRGB p = CHSV(HUE_PURPLE, 255, 255);
		CRGB g = CHSV(HUE_GREEN, 255, 255);
		CRGB u = CHSV(HUE_BLUE, 255, 255);
		CRGB b = CRGB::Black;
		CRGB w = CRGB::White;

		switch (_secondCounter)
		{
			case  0: targetPalette = RainbowColors_p; break;
			case  5: targetPalette = CRGBPalette16(u, u, b, b, p, p, b, b, u, u, b, b, p, p, b, b); break;
			case 10: targetPalette = OceanColors_p; break;
			case 15: targetPalette = CloudColors_p; break;
			case 20: targetPalette = LavaColors_p; break;
			case 25: targetPalette = ForestColors_p; break;
			case 30: targetPalette = PartyColors_p; break;
			case 35: targetPalette = CRGBPalette16(b, b, b, w, b, b, b, w, b, b, b, w, b, b, b, w); break;
			case 40: targetPalette = CRGBPalette16(u, u, u, w, u, u, u, w, u, u, u, w, u, u, u, w); break;
			case 45: targetPalette = CRGBPalette16(u, p, u, w, p, u, u, w, u, g, u, w, u, p, u, w); break;
			case 50: targetPalette = CloudColors_p; break;
			case 55: targetPalette = CRGBPalette16(u, u, u, w, u, u, p, p, u, p, p, p, u, p, p, w); break;
		}

		if (++_secondCounter > 60)
		{
			_secondCounter = 0;
		}
	}

public:
	SeqThreeSinPal(struct CRGB* data, int nLedsOrOffset) : Sequence(data, nLedsOrOffset) {
		fill_solid(&(currentPalette[0]), 16, CRGB::Black);
		targetPalette = PartyColors_p;
	}

	void tick()
	{
		uint8_t maxChanges = 24;

		EVERY_N_MILLISECONDS(thisdelay)
		{                           // FastLED based non-blocking delay to update/display the sequence.
			nblendPaletteTowardPalette(currentPalette, targetPalette, maxChanges);
			three_sin();                                              // Improved method of using non-blocking delay
		}
	}

	void three_sin()
	{

		wave1 += beatsin8(10, -4, 4);
		wave2 += beatsin8(15, -2, 2);
		wave3 += beatsin8(12, -3, 3);

		for (int k = 0; k < LEDCount; k++)
		{
			uint8_t tmp = sin8(mul1 * k + wave1) + sin8(mul1 * k + wave2) + sin8(mul1 * k + wave3);
			LEDArray[k] = ColorFromPalette(currentPalette, tmp, 255);
		}

	} // three_sin()

};

/* three_sin_pal_demo
 *
 * By: Andrew Tuline
 *
 * Date: March, 2015
 *
 * 3 sine waves, one for each colour. I didn't take this far, but you could change the beat frequency and so on. . .
 *
 */
