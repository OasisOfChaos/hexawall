// SeqQuadwave8FadeUp.h

#pragma once

#include "arduino.h"
#include <FastLED.h>
#include "Sequence.hpp"

class SeqQuadwave8FadeUp : public Sequence
{
private:
	uint16_t count;
	uint8_t speed = 15;  // Higher number gives slower pulse

public:
	SeqQuadwave8FadeUp(struct CRGB* data, int nLedsOrOffset) : Sequence(data, nLedsOrOffset) {}

	void tick()
	{
		uint8_t value = quadwave8(count / speed);
		//Serial.print("  count: ");  Serial.print(count);
		//Serial.print("    value: ");  Serial.println(value);
		for (int i = 0; i < LEDCount; i++)
		{
			LEDArray[i] = CHSV(80, 255, value);
		}

		FastLED.show();

		count++;
		if (value == 255)
		{
			// If value is maxed out then reset count.
			count = 0;
		}
	}

};

//***************************************************************
// test_quadwave8_fade_up pulse example
//
// Marc Miller, 2015
//***************************************************************
