// SeqNone.h

#pragma once

#include "arduino.h"
#include <FastLED.h>
#include "Sequence.hpp"

class SeqNone : public Sequence
{
private:
	bool _processed = false;

    void secondTick() {}
public:
	SeqNone(struct CRGB* data, int nLedsOrOffset) : Sequence(data, nLedsOrOffset) { }

	void tick()
	{
        if (_isFinished)
        {
            return;
        }

        EVERY_N_MILLISECONDS(40)
        {

            // Try different fade values here and note how it changes the look.
            fadeToBlackBy(LEDArray, LEDCount, 10);  // 10/255 = 4%
            //fadeToBlackBy(LEDArray, LEDCount, 64);  // 64/255 = 25%
            //fadeToBlackBy(LEDArray, LEDCount, 128);  // 128/255 = 50%
            //fadeToBlackBy(LEDArray, LEDCount, 200);  // 200/255 = 78%

            // Check if we have gone all the way to black (off)
            if (LEDArray[0] == CRGB(0, 0, 0))
            {
                PRINT_DEBUG("SeqNone is finished.\n");

                _isFinished = true;
            }

        }  // end every_N
	}
};