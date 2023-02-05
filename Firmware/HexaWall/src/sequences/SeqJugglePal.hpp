// SeqJugglePal.h

#pragma once

#include "arduino.h"
#include <FastLED.h>
#include "Sequence.hpp"

class SeqJugglePal : public Sequence
{
private:
	CRGBPalette16 currentPalette;                                 // Use palettes instead of direct CHSV or CRGB assignments
	CRGBPalette16 targetPalette;                                  // Also support smooth palette transitioning
	TBlendType    currentBlending;                                // NOBLEND or LINEARBLEND

	uint8_t tickCounter = 0;
	int _secondCounter = 0;

	// Routine specific variables
	uint8_t    numdots = 4;                                     // Number of dots in use.
	uint8_t   thisfade = 2;                                     // How long should the trails be. Very low value = longer trails.
	uint8_t   thisdiff = 16;                                     // Incremental change in hue between each dot.
	uint8_t    thishue = 0;                                     // Starting hue.
	uint8_t     curhue = 0;                                     // The current hue
	uint8_t    thissat = 255;                                     // Saturation of the colour.
	//uint8_t thisbright = 255;                                     // How bright should the LED/display be.
	uint8_t   thisbeat = 5;                                     // Higher = faster movement.

	void secondTick() {
		switch (_secondCounter)
		{
			case  0: numdots = 1; thisbeat = 20; thisdiff = 16; thisfade = 2; thishue = 0; break;                  // You can change values here, one at a time , or altogether.
			case 10: numdots = 4; thisbeat = 10; thisdiff = 16; thisfade = 8; thishue = 128; break;
			case 20: numdots = 8; thisbeat = 3; thisdiff = 0; thisfade = 8; thishue = random8(); break;           // Only gets called once, and not continuously for the next several seconds. Therefore, no rainbows.
			case 30: break;
		}

		if (++_secondCounter > 30)
		{
			_secondCounter = 0;
		}
	}

public:
	SeqJugglePal(struct CRGB* data, int nLedsOrOffset) : Sequence(data, nLedsOrOffset)
	{
		currentPalette = CRGBPalette16(CRGB::Black);
		targetPalette = RainbowColors_p;
		currentBlending = LINEARBLEND;
	}

	void tick()
	{
		//EVERY_N_MILLISECONDS(100)
		if (++tickCounter > 10)
		{
			// AWESOME palette blending capability provided as required.
			uint8_t maxChanges = 24;
			nblendPaletteTowardPalette(currentPalette, targetPalette, maxChanges);

			tickCounter = 0;
		}

		// Several colored dots, weaving in and out of sync with each other
		curhue = thishue;                                           // Reset the hue values.
		fadeToBlackBy(LEDArray, LEDCount, thisfade);

		for (int i = 0; i < numdots; i++)
		{
			LEDArray[beatsin16(thisbeat + i + numdots, 0, LEDCount)] += ColorFromPalette(currentPalette, curhue, LEDBrightness, currentBlending);    // Munge the values and pick a colour from the palette
			curhue += thisdiff;
		}
	}

};