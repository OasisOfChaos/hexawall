// SeqRepeatingBlockPattern.h

#pragma once

#include "arduino.h"
#include <FastLED.h>
#include "Sequence.hpp"

class SeqRepeatingBlockPattern : public Sequence
{
private:
	uint8_t blockSize = 5;  // number of pixels to light up in a block
	uint8_t count;  // used to keep track of what block to light up.
	uint16_t loopStart = 0;
	uint16_t loopEnd = blockSize;
	uint8_t hue;
	bool sizeUpdate;

    void secondTick() {}

public:
	SeqRepeatingBlockPattern(struct CRGB* data, int nLedsOrOffset) : Sequence(data, nLedsOrOffset) {}

	void tick()
	{
        loopStart = count * blockSize;
        loopEnd = blockSize + count * blockSize;

        // Trying to write data to pixels that don't exit is bad.
        // Check to make sure we are still within our LEDCount range
        // and clamp to LEDCount if needed.
        if (loopEnd > LEDCount)
        {
            loopEnd = LEDCount;  // limit maximum to LEDCount
        }

        hue = hue + random8(8, 17);  // for fun, pick a new color for each block

        EVERY_N_MILLISECONDS(250)
        {
            for (uint16_t i = loopStart; i < loopEnd; i++)
            {
                LEDArray[i] = CHSV(hue, 255, 255);
            }

            FastLED.show();

            for (uint16_t i = loopStart; i < loopEnd; i++)
            {
                LEDArray[i].fadeToBlackBy(220);  // fade down
            }

            count++;  // increase count by one

            // reset count if we have come to the end of the strip
            if ((count * blockSize) >= LEDCount)
            {
                count = 0;
                // Only change the block size when starting over on the strip
                // and after the minimum time has passed (from timer below).
                if (sizeUpdate)
                {
                    blockSize = random8(2, 9);  // for fun, pick a new random block size
                    sizeUpdate = false;
                }
            }

        } //end_every_n


        EVERY_N_SECONDS(5)
        {
            sizeUpdate = true;  // trigger size update
        }
    }

};

//***************************************************************
//  Basic example that repeats colored blocks of random size down
//  the strip.  The block size is changed every several seconds.
//
//  Important to note is there is a check to make sure we don't
//  try to write data to pixels that don't exist (ie. values
//  equal or greater then LEDCount).  Trying to write pixel
//  data to pixels that don't exist the causes bad things to
//  happen in memory.
//
//  Marc Miller, July 2017
//               May 2020 - replaced delays with EVERY_N
//***************************************************************
