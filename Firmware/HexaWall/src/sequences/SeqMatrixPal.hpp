// SeqMatrixPal.h

#pragma once

#include "arduino.h"
#include <FastLED.h>
#include "Sequence.hpp"

class SeqMatrixPal : public Sequence
{
private:
	// Palette definitions
	CRGBPalette16 currentPalette;
	CRGBPalette16 targetPalette;
	TBlendType    currentBlending;

	uint8_t tickCounter = 0;
	int _secondCounter = 0;

	// Initialize global variables for sequences.
	int thisdelay = 50;                                     // A delay value for the sequence(s)
	uint8_t palIndex = 95;
	uint8_t thissat = 255;
	int thisdir = 0;
	uint8_t thisbright = 255;
	bool huerot = 0;                                     // Does the hue rotate? 1 = yes
	uint8_t bgclr = 0;
	uint8_t bgbri = 0;

	void secondTick() {
		switch (_secondCounter)
		{
			case  0: thisdelay = 50; palIndex = 95; bgclr = 140; bgbri = 4; huerot = 0; break;
			case  5: targetPalette = OceanColors_p; thisdir = 1; bgbri = 0; huerot = 1; break;
			case 10: targetPalette = LavaColors_p; thisdelay = 30; palIndex = 0; bgclr = 50; bgbri = 8; huerot = 0; break;
			case 15: targetPalette = ForestColors_p; thisdelay = 80; bgbri = 16; bgclr = 96; palIndex = random8(); break;
			case 20: palIndex = random8(); huerot = 1; break;
		}

		if (++_secondCounter > 25)
		{
			_secondCounter = 0;
		}
	}

public:
	SeqMatrixPal(struct CRGB* data, int nLedsOrOffset) : Sequence(data, nLedsOrOffset)
	{
		currentBlending = LINEARBLEND;
	}

	void tick()
	{
		if (++tickCounter > 10)
		{
			// AWESOME palette blending capability provided as required.
			uint8_t maxChanges = 24;
			nblendPaletteTowardPalette(currentPalette, targetPalette, maxChanges);

			tickCounter = 0;
		}

		EVERY_N_MILLISECONDS(thisdelay)
		{
			matrix();
		}
	}

	void matrix()
	{                                               // One line matrix

		if (huerot) palIndex++;

		if (random8(90) > 80)
		{
			if (thisdir == 0)
				LEDArray[0] = ColorFromPalette(currentPalette, palIndex, thisbright, currentBlending);
			else
				LEDArray[LEDCount - 1] = ColorFromPalette(currentPalette, palIndex, thisbright, currentBlending);
		}
		else
		{
			if (thisdir == 0)
				LEDArray[0] = CHSV(bgclr, thissat, bgbri);
			else
				LEDArray[LEDCount - 1] = CHSV(bgclr, thissat, bgbri);
		}

		if (thisdir == 0)
		{
			for (int i = LEDCount - 1; i > 0; i--) LEDArray[i] = LEDArray[i - 1];
		}
		else
		{
			for (int i = 0; i < LEDCount - 1; i++) LEDArray[i] = LEDArray[i + 1];
		}

	} // matrix()

};