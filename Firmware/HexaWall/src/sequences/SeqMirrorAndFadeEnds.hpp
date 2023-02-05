// SeqMirrorAndFadeEnds.h

#pragma once

#include "arduino.h"
#include <FastLED.h>
#include "Sequence.hpp"

class SeqMirrorAndFadeEnds : public Sequence
{
private:

public:
	SeqMirrorAndFadeEnds(struct CRGB* data, int nLedsOrOffset) : Sequence(data, nLedsOrOffset) {}

	void tick()
	{ // fill first half the strip with a rainbow
		fill_rainbow(LEDArray, LEDCount / 2, (millis() / 50));

		// mirror data
		mirror();

		// fade ends by X number of pixels
		fadeEnds(7);

	}

	//---------------------------------------------------------------
	// mirror data to the other half
	void mirror()
	{
		for (uint8_t i = 0; i < LEDCount / 2; i++)
		{
			LEDArray[LEDCount - 1 - i] = LEDArray[i];
		}
	}


	//---------------------------------------------------------------
	// fade both ends of strip over X number of pixels
	void fadeEnds(uint8_t fadeOver)
	{
		uint8_t fadePP = 255 / fadeOver;  // fade per pixel
		for (uint8_t i = 0; i < fadeOver + 1; i++)
		{
			uint8_t fadeAmmount = (255 - (fadePP * i));
			LEDArray[i].fadeToBlackBy(fadeAmmount);
			LEDArray[LEDCount - 1 - i].fadeToBlackBy(fadeAmmount);
		}
	}
};

/****************************************************************
mirror display and faded out ends of strip
This example shows one basic way to mirror the display on
a strip.  It also fades out the ends of the strip to black.
This example fades the ends over 7 pixels.
Marc Miller, Feb 2021
****************************************************************/