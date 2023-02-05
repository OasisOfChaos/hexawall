// SeqMovingColors.h

#pragma once

#include "arduino.h"
#include <FastLED.h>
#include "Sequence.hpp"

class SeqMovingColors : public Sequence
{
private:
	// Set initial start position of each color
	int16_t positionA = LEDCount * 2 / 3;
	int16_t positionB = LEDCount / 3;
	int16_t positionC = 0;

	const uint16_t holdTime = 120;  // Adjusts speed of travel
	int8_t delta = 1;  // 1 or -1.  Sets travel direction

public:
	SeqMovingColors(struct CRGB* data, int nLedsOrOffset) : Sequence(data, nLedsOrOffset) {}

    void tick()
    {
        EVERY_N_MILLISECONDS(holdTime)
        {

            // Fading tail effect.  Comment out for solid colors
            fadeToBlackBy(LEDArray, LEDCount, 100);

            // assign pixel colors
            LEDArray[positionA] = CRGB::Red;
            LEDArray[positionB] = CRGB::Grey;  // Using grey so not as bright
            LEDArray[positionC] = CRGB::Blue;

            FastLED.show();  // Show the pixels

            // Advance position based on delta, and rollover if needed.
            positionA = ((positionA + delta + LEDCount) % LEDCount);
            positionB = ((positionB + delta + LEDCount) % LEDCount);
            positionC = ((positionC + delta + LEDCount) % LEDCount);

        }//end every_n}
    }
};

//***************************************************************
// Three moving colors.
//     Uses modulo, %, to make pixel position "loop" around and
//     stay in valid pixel range.
//
// Marc Miller,  Jan. 2019
//     Apr. 2019 - updated to use EVERY_N instead of delay()
//     Apr. 2021 - cleaned up some duplicate code
//***************************************************************
