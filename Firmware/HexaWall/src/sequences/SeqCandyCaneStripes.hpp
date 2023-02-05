// SeqCandyCaneStripes.h

#pragma once

#include "arduino.h"
#include <FastLED.h>
#include "Sequence.hpp"

class SeqCandyCaneStripes : public Sequence
{
private:
    CRGB color1 = CRGB::White;  // color used between color 2 (and 3 if used)
    CRGB color2 = CRGB::Red;
    //CRGB color3 = CHSV(0,170,255);  //optional 3rd color
    CRGB color3 = CRGB(0, 255, 0);  //optional 3rd color
    const uint16_t travelSpeed = 150;
    int shiftBy = 1;  //shiftBy can be positive or negative (to change direction)
    uint8_t numColors = 2;  // Can be either 2 or 3
    uint8_t stripeLength = 5;  //number of pixels per color
    int offset;

public:
	SeqCandyCaneStripes(struct CRGB* data, int nLedsOrOffset) : Sequence(data, nLedsOrOffset) {}

    void tick()
    {
        EVERY_N_SECONDS(5)
        {
            numColors = random8(2, 4);  //picks either 2 or 3
            stripeLength = random8(3, 6);  //picks random length
        }

        EVERY_N_MILLISECONDS(travelSpeed)
        {
            if (numColors == 2)
            {
                for (uint8_t i = 0; i < LEDCount; i++)
                {
                    if ((i + offset) % ((numColors)*stripeLength) < stripeLength)
                    {
                        LEDArray[i] = color2;
                    }
                    else
                    {
                        LEDArray[i] = color1;
                    }
                }
            }

            if (numColors == 3)
            {
                for (uint8_t i = 0; i < LEDCount; i++)
                {
                    if ((i + offset) % ((numColors + 1) * stripeLength) < stripeLength)
                    {
                        LEDArray[i] = color2;
                    }
                    else if ((i + offset + (2 * stripeLength)) % ((numColors + 1) * stripeLength) < stripeLength)
                    {
                        LEDArray[i] = color3;
                    }
                    else
                    {
                        LEDArray[i] = color1;
                    }
                }
            }

            FastLED.show();

            offset = offset + shiftBy;
            if (shiftBy > 0)
            {  //for positive shiftBy
                if (offset >= LEDCount) offset = 0;
            }
            else
            {  //for negitive shiftBy
                if (offset < 0) offset = LEDCount;
            }

        }//end EVERY_N}
    }
};

//***************************************************************
// "candy cane" stripes
// Original code concept by Christopher Kirkman
// https://plus.google.com/118127046082317810519/posts/WfEcTXiroke
//
// Modified to use EVERY_N timer, added option to travel in
// reverse, and added a third color option.
//
// Marc Miller, Dec 2017
// Add randomness for number of colors and length, Dec 2018
//***************************************************************
