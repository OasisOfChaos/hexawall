// SeqRainbowMarch.h

#pragma once

#include "arduino.h"
#include <FastLED.h>
#include "Sequence.hpp"

class SeqRainbowMarch : public Sequence
{
private:
	int _secondCounter = 0;

	uint8_t thisdelay = 40;                                       // A delay value for the sequence(s)
	uint8_t thishue = 0;                                          // Starting hue value.
	int8_t thisrot = 1;                                           // Hue rotation speed. Includes direction.
	uint8_t deltahue = 1;                                         // Hue change between pixels.
	bool thisdir = 0;                                             // I use a direction variable instead of signed math so I can use it in multiple routines.

	void secondTick() {
		switch (_secondCounter)
		{
			case  0: thisrot = 1; deltahue = 5; break;
			case  5: thisdir = -1; deltahue = 10; break;
			case 10: thisrot = 5; break;
			case 15: thisrot = 5; thisdir = -1; deltahue = 20; break;
			case 20: deltahue = 30; break;
			case 25: deltahue = 2; thisrot = 5; break;
		}

		if (++_secondCounter > 30)
		{
			_secondCounter = 0;
		}
	}
public:
	SeqRainbowMarch(struct CRGB* data, int nLedsOrOffset) : Sequence(data, nLedsOrOffset) {}

	void tick()
	{
		EVERY_N_MILLISECONDS(thisdelay)
		{
			// FastLED based non-blocking delay to update/display the sequence.
			if (thisdir == 0) thishue += thisrot; else thishue -= thisrot;  // I could use signed math, but 'thisdir' works with other routines.
			fill_rainbow(LEDArray, LEDCount, thishue, deltahue);            // I don't change deltahue on the fly as it's too fast near the end of the strip.
		}
	}

};

/* rainbow_march_demo
 *
 * By: Andrew Tuline
 *
 * Date: March, 2015
 *
 * Rainbow marching up the strand. Pretty basic, but oh so popular, and we get a few options as well. No 'wheel' routine required.
 *
 */