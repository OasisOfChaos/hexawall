﻿// SeqTwoSinPal.h

#pragma once

#include "arduino.h"
#include <FastLED.h>
#include "Sequence.hpp"

#define qsubd(x, b)  ((x>b)?b:0)                     // A digital unsigned subtraction macro. if result <0, then => 0. Otherwise, take on fixed value.
#define qsuba(x, b)  ((x>b)?x-b:0)                            // Unsigned subtraction macro. if result <0, then => 0

class SeqTwoSinPal : public Sequence
{
private:
	int _secondCounter = 0;

	//unsigned long previousMillis;                                 // Store last time the strip was updated.

	// Most of these variables can be mucked around with. Better yet, add some form of variable input or routine to change them on the fly. 1970's here I come. .

	uint8_t thishue;                                              // You can change the starting hue value for the first wave.
	uint8_t thathue;                                              // You can change the starting hue for other wave.
	uint8_t thisrot;                                              // You can change how quickly the hue rotates for this wave. Currently 0.
	uint8_t thatrot;                                              // You can change how quickly the hue rotates for the other wave. Currently 0.
	uint8_t allsat;                                               // I like 'em fully saturated with colour.
	uint8_t thisdir;
	uint8_t thatdir;
	uint8_t alldir;                                               // You can change direction.
	int8_t thisspeed;                                             // You can change the speed.
	int8_t thatspeed;                                             // You can change the speed.
	uint8_t allfreq;                                              // You can change the frequency, thus overall width of bars.
	int thisphase;                                                // Phase change value gets calculated.
	int thatphase;                                                // Phase change value gets calculated.
	uint8_t thiscutoff;                                           // You can change the cutoff value to display this wave. Lower value = longer wave.
	uint8_t thatcutoff;                                           // You can change the cutoff value to display that wave. Lower value = longer wave.
	int thisdelay;                                                // Standard delay. .
	uint8_t fadeval;                                              // Use to fade the led's of course.

	CRGBPalette16 thisPalette;
	CRGBPalette16 thatPalette;
	TBlendType    currentBlending;                                // NOBLEND or LINEARBLEND

	void secondTick() {
		switch (_secondCounter)
		{
			case  0: thisrot = 1; thatrot = 1; thisPalette = PartyColors_p; thatPalette = PartyColors_p; break;
			case  5: thisrot = 0; thatdir = 1; thatspeed = -4; thisPalette = ForestColors_p; thatPalette = OceanColors_p; break;
			case 10: thatrot = 0; thisPalette = PartyColors_p; thatPalette = RainbowColors_p; break;
			case 15: allfreq = 16; thisdir = 1; thathue = 128; break;
			case 20: thiscutoff = 96; thatcutoff = 240; break;
			case 25: thiscutoff = 96; thatdir = 0; thatcutoff = 96; thisrot = 1; break;
			case 30: thisspeed = -4; thisdir = 0; thatspeed = -4; break;
			case 35: thiscutoff = 128; thatcutoff = 128; break;
			case 40: thisspeed = 3; break;
			case 45: thisspeed = 3; thatspeed = -3; break;
			case 50: thisspeed = 2; thatcutoff = 96; thiscutoff = 224; thatspeed = 3; break;

			case 55: resetvars(); break;
		}

		if (++_secondCounter > 60)
		{
			_secondCounter = 0;
		}
	}

public:
	SeqTwoSinPal(struct CRGB* data, int nLedsOrOffset) : Sequence(data, nLedsOrOffset)
	{
		// Also, RainbowColors_p, RainbowStripeColors_p, OceanColors_p, CloudColors_p, LavaColors_p, ForestColors_p, and PartyColors_p
		thisPalette = RainbowColors_p;
		thatPalette = RainbowColors_p;
		currentBlending = LINEARBLEND;

		resetvars();                                                  // Initialize the variables
	}

	void tick()
	{
		uint8_t maxChanges = 24;

		EVERY_N_MILLISECONDS(thisdelay)
		{
			two_sin();                                                // Routine is still delay based, but at least it's now a non-blocking day.
		}
	}

	void two_sin()
	{

		thisdir ? thisphase += beatsin8(thisspeed, 2, 10) : thisphase -= beatsin8(thisspeed, 2, 10);
		thatdir ? thatphase += beatsin8(thisspeed, 2, 10) : thatphase -= beatsin8(thatspeed, 2, 10);
		thishue += thisrot;                                        // Hue rotation is fun for thiswave.
		thathue += thatrot;                                        // It's also fun for thatwave.

		for (int k = 0; k < LEDCount - 1; k++)
		{
			int thisbright = qsuba(cubicwave8((k * allfreq) + thisphase), thiscutoff);      // qsub sets a minimum value called thiscutoff. If < thiscutoff, then bright = 0. Otherwise, bright = 128 (as defined in qsub)..
			int thatbright = qsuba(cubicwave8((k * allfreq) + 128 + thatphase), thatcutoff);  // This wave is 180 degrees out of phase (with the value of 128).

			LEDArray[k] = ColorFromPalette(thisPalette, thishue, thisbright, currentBlending);
			LEDArray[k] += ColorFromPalette(thatPalette, thathue, thatbright, currentBlending);
		}
		nscale8(LEDArray, LEDCount, fadeval);

	} // two_sin()

	void resetvars()
	{                       // Reset the variable back to the beginning.

		thishue = 0;                          // You can change the starting hue value for the first wave.
		thathue = 140;                        // You can change the starting hue for other wave.
		thisrot = 1;                          // You can change how quickly the hue rotates for this wave. Currently 0.
		thatrot = 1;                          // You can change how quickly the hue rotates for the other wave. Currently 0.
		allsat = 255;                         // I like 'em fully saturated with colour.
		thisdir = 0;                          // Change the direction of the first wave.
		thatdir = 0;                          // Change the direction of the other wave.
		alldir = 0;                           // You can change direction.
		thisspeed = 4;                        // You can change the speed, and use negative values.
		thatspeed = 4;                        // You can change the speed, and use negative values.
		allfreq = 32;                         // You can change the frequency, thus overall width of bars.
		thisphase = 0;                        // Phase change value gets calculated.
		thatphase = 0;                        // Phase change value gets calculated.
		thiscutoff = 192;                     // You can change the cutoff value to display this wave. Lower value = longer wave.
		thatcutoff = 192;                     // You can change the cutoff value to display that wave. Lower value = longer wave.
		thisdelay = 10;                       // You can change the delay. Also you can change the allspeed variable above.
		fadeval = 192;                        // How quickly we fade.

	} // resetvars()
};

/* two_sin_pal_demo
 *
 * By: Andrew Tuline
 *
 * Date: July, 2015
 *
 * Two moving waves of different colours, consisting of palettes. The cool thing about this routine is that there are lot of configurable items without requiring a lot of complex code.
 * It uses sine waves to create 'bars' and not lengthy/complex 'marching' code with nasty delay statements.
  *
 * With a few potentiometers or other inputs, you could change several values:
 *
 *  - Change the width of each wave
 *  - Change the speed
 *  - Change the direction
 *  - Change the overall frequency
 *  - Change the colour of each wave or even change the hue rotation speed.
 *  - You could even get real funky and have different frequencies and phase changes for the waves. Epilepsy alert!
 *
 * I've got codes that does this and it makes for an awesome interactive display for youth.
 *
 */