// SeqEasing.h

#pragma once

#include "arduino.h"
#include <FastLED.h>
#include "Sequence.hpp"

class SeqEasing : public Sequence
{
private:
	void secondTick() {}

public:
	SeqEasing(struct CRGB* data, int nLedsOrOffset) : Sequence(data, nLedsOrOffset) {}

	void tick()
	{
		EVERY_N_MILLISECONDS(10)
		{
			static uint8_t easeOutVal = 0;
			static uint8_t easeInVal = 0;
			static uint16_t lerpVal = 0;

			easeOutVal = ease8InOutQuad(easeInVal);                     // Start with easeInVal at 0 and then go to 255 for the full easing.
			easeInVal++;

			lerpVal = lerp16by16(0, LEDCount, easeOutVal);                // Map it to the number of LED's you have.

			LEDArray[lerpVal] = CRGB::Red;
			fadeToBlackBy(LEDArray, LEDCount, 16);                          // 8 bit, 1 = slow fa
		}
	}

};