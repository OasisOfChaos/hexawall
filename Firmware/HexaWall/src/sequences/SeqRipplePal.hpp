// SeqRipplePal.h

#pragma once

#include "arduino.h"
#include <FastLED.h>
#include "Sequence.hpp"

class SeqRipplePal : public Sequence
{
private:
#define maxsteps 16                                           // Case statement wouldn't allow a variable.

	CRGBPalette16 currentPalette;
	CRGBPalette16 targetPalette;
	TBlendType    currentBlending;                                // NOBLEND or LINEARBLEND

	uint8_t colour;                                               // Ripple colour is randomized.
	int center = 0;                                               // Center of the current ripple.
	int step = -1;                                                // -1 is the initializing step.
	uint8_t myfade = 255;                                         // Starting brightness.
	//uint8_t fadeval = 128;
	//uint8_t fadeval = 64;
	uint8_t fadeval = 32;

	uint8_t bgcol = 0;                                            // Background colour rotates.
	int thisdelay = 6;                                          // Standard delay value.

	void secondTick() {}

public:
	SeqRipplePal(struct CRGB* data, int nLedsOrOffset) : Sequence(data, nLedsOrOffset)
	{
		currentBlending = LINEARBLEND;
	}

	void tick()
	{
		EVERY_N_MILLISECONDS(10)
		{
			uint8_t maxChanges = 24;
			nblendPaletteTowardPalette(currentPalette, targetPalette, maxChanges);   // AWESOME palette blending capability.
		}

		EVERY_N_SECONDS(3)
		{
			targetPalette = CRGBPalette16(CHSV(random8(), 255, 32), CHSV(random8(), random8(64) + 192, 255), CHSV(random8(), 255, 32), CHSV(random8(), 255, 255));
		}

		EVERY_N_MILLISECONDS(thisdelay)
		{
			// FastLED based non-blocking delay to update/display the sequence.
			ripple();
		}
	}

	void ripple()
	{

		fadeToBlackBy(LEDArray, LEDCount, fadeval);                             // 8 bit, 1 = slow, 255 = fast

		switch (step)
		{

		case -1:                                                          // Initialize ripple variables.
			center = random(LEDCount);
			colour = random8();
			step = 0;
			break;

		case 0:
			LEDArray[center] = ColorFromPalette(currentPalette, colour, myfade, currentBlending);

			step++;
			break;

		case maxsteps:                                                    // At the end of the ripples.
			step = -1;
			break;

		default:                                                          // Middle of the ripples.
			LEDArray[(center + step + LEDCount) % LEDCount] += ColorFromPalette(currentPalette, colour, myfade / step * 2, currentBlending);       // Simple wrap from Marc Miller
			LEDArray[(center - step + LEDCount) % LEDCount] += ColorFromPalette(currentPalette, colour, myfade / step * 2, currentBlending);
			step++;                                                         // Next step.
			break;
		} // switch step

	} // ripple()

};

/* ripple_pal
 *
 * By: Balázs Suhajda
 *
 * Modified by: Andrew Tuline
 *
 * Date: Oct, 2014
 *
 * A cool ripple effect for an LED strip that's been re-written and optimized from the Neopixel version https://gist.github.com/suhajdab/9716635
 *
 */