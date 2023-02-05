// SeqPlasma.h

#pragma once

#include "arduino.h"
#include <FastLED.h>
#include "Sequence.hpp"

// Use qsuba for smooth pixel colouring and qsubd for non-smooth pixel colouring
#define qsubd(x, b)  ((x>b)?b:0)                              // Digital unsigned subtraction macro. if result <0, then => 0. Otherwise, take on fixed value.
#define qsuba(x, b)  ((x>b)?x-b:0)                            // Analog Unsigned subtraction macro. if result <0, then => 0


class SeqPlasma : public Sequence
{
private:

	CRGBPalette16 currentPalette;
	CRGBPalette16 targetPalette;
	TBlendType currentBlending = LINEARBLEND;

	void secondTick() {}

public:
	SeqPlasma(struct CRGB* data, int nLedsOrOffset) : Sequence(data, nLedsOrOffset)
	{
		currentPalette = OceanColors_p;
	}

	void tick()
	{
		//EVERY_N_MILLISECONDS(5)
		{                                  // FastLED based non-blocking delay to update/display the sequence.
			plasma();
		}

		//EVERY_N_MILLISECONDS(1000)
		//{
		//	Serial.println(LEDS.getFPS());                            // Optional check of our fps.
		//}

		EVERY_N_MILLISECONDS(10)
		{
			uint8_t maxChanges = 24;
			nblendPaletteTowardPalette(currentPalette, targetPalette, maxChanges);   // AWESOME palette blending capability.
		}


		EVERY_N_SECONDS(5)
		{                                 // Change the target palette to a random one every 5 seconds.
			uint8_t baseC = random8();                         // You can use this as a baseline colour if you want similar hues in the next line.
			targetPalette = CRGBPalette16(CHSV(baseC + random8(32), 192, random8(128, 255)), CHSV(baseC + random8(32), 255, random8(128, 255)), CHSV(baseC + random8(32), 192, random8(128, 255)), CHSV(baseC + random8(32), 255, random8(128, 255)));
		}
	}

	void plasma()
	{                                                 // This is the heart of this program. Sure is short. . . and fast.

		int thisPhase = beatsin8(6, -64, 64);                           // Setting phase change for a couple of waves.
		int thatPhase = beatsin8(7, -64, 64);

		for (int k = 0; k < LEDCount; k++)
		{                              // For each of the LED's in the strand, set a brightness based on a wave as follows:

			int colorIndex = cubicwave8((k * 23) + thisPhase) / 2 + cos8((k * 15) + thatPhase) / 2;           // Create a wave and add a phase change and add another wave with its own phase change.. Hey, you can even change the frequencies if you wish.
			int thisBright = qsuba(colorIndex, beatsin8(7, 0, 96));                                 // qsub gives it a bit of 'black' dead space by setting sets a minimum value. If colorIndex < current value of beatsin8(), then bright = 0. Otherwise, bright = colorIndex..

			LEDArray[k] = ColorFromPalette(currentPalette, colorIndex, thisBright, currentBlending);  // Let's now add the foreground colour.
		}

	} // plasma()
};

/* Plasma
 *
 * By: Andrew Tuline
 *
 * Date: July, 2017
 *
 * This demonstrates 2D sinusoids in 1D using 16 bit math.
 *
 * It runs at about 450 fps on an Arduino Nano with 60 APA102's. That would be FAST.
 *
 *
 * References:
 *
 * https://www.bidouille.org/prog/plasma
 * http://lodev.org/cgtutor/plasma.html
 *
 */