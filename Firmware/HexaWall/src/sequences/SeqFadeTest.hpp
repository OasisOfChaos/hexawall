// SeqFadeTest.h

#pragma once

#include "arduino.h"
#include <FastLED.h>
#include "Sequence.hpp"

class SeqFadeTest : public Sequence
{
private:
	boolean fadeToColor = 1;  // turns on/off the fading toward target
	const uint8_t fadeRate = 42;   // larger number is a slower fade

	CRGB currentColor;  // starting color
	CRGB targetColor;   // starting target color

	void secondTick() {}

public:
	SeqFadeTest(struct CRGB* data, int nLedsOrOffset) : Sequence(data, nLedsOrOffset)
	{
		currentColor = CRGB(130, 25, 25);
		targetColor = CRGB(30, 40, 130);

		fill_solid(LEDArray, LEDCount, currentColor);
	}

	void tick()
	{
		EVERY_N_SECONDS(10)
		{
			newTargetColor();  // set the new target color
			fadeToColor = 1;  // start fading toward target
		}

		EVERY_N_MILLISECONDS(fadeRate)
		{
			if (currentColor == targetColor)
			{
				fadeToColor = 0;  // stop fading
			}
			if (fadeToColor)
			{
				nblendU8TowardU8(currentColor.r, targetColor.r);
				nblendU8TowardU8(currentColor.g, targetColor.g);
				nblendU8TowardU8(currentColor.b, targetColor.b);
				currentColor = CRGB(currentColor.r, currentColor.g, currentColor.b);
			}

			fill_solid(LEDArray, LEDCount, currentColor);

			// For Testing --- always display target on first pixel
			//LEDArray[0] = targetColor;
			//FastLED.show();
		}
	}

	//===============================================================
	// Modified helper function that blends one uint8_t toward another,
	// based on function from Mark Kriegsman's fadeTowardColor example:
	// https://gist.github.com/kriegsman/d0a5ed3c8f38c64adcb4837dafb6e690
	void nblendU8TowardU8(uint8_t& current, const uint8_t target)
	{
		if (current == target)
		{
			return;
		}

		if (current < target)
		{
			uint8_t delta = target - current;
			delta = scale8_video(delta, 1);
			current += delta;
		}
		else
		{
			uint8_t delta = current - target;
			delta = scale8_video(delta, 1);
			current -= delta;
		}
	}


	//---------------------------------------------------------------
	// Pick a new target color to fade toward
	void newTargetColor()
	{
		random16_add_entropy(random16());
		targetColor = CHSV(random8(), 255, 255);

		//Serial.print("New targetColor ");
		//Serial.print("\tr: "); Serial.print(targetColor.r);
		//Serial.print("\tg: "); Serial.print(targetColor.g);
		//Serial.print("\tb: "); Serial.println(targetColor.b);
	}

};