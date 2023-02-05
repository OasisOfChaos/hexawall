// SeqChaseTargetValues.h

#pragma once

#include "arduino.h"
#include <FastLED.h>
#include "Sequence.hpp"

class SeqChaseTargetValues : public Sequence
{
private:
	int count;
	int delta;
	int temp;
	int lowCutoff = 50;

    int target[750];  // Place to save some target values.

    void secondTick() {}

public:
	SeqChaseTargetValues(struct CRGB* data, int nLedsOrOffset) : Sequence(data, nLedsOrOffset) {}

	void tick()
	{
        // Assign random target values whenever count is zero.
        if (count == 0)
        {
            for (int i = 0; i < LEDCount; i++)
            {
                target[random8(LEDCount)] = random8();  // Pick random pixels, assign random values.
                if (target[i] < lowCutoff)
                {
                    target[i] = 0;  // Force low values to clamp to zero.
                }
            }
            count = random8(35, 70);  // Pick a new count value.
            Serial.println("*New targets assigned!*");
        }


        for (int i = 0; i < LEDCount; i++)
        {
            // Check current values against target values.
            if (LEDArray[i].r < target[i])
            {  // less then the target, so fade up.
                delta = (target[i] - LEDArray[i].r) / 5;
                if (LEDArray[i].r + delta >= target[i])
                {
                    LEDArray[i].r = target[i];  // limit to target.
                }
                else { LEDArray[i].r += delta; }
            }

            else
            {  // greater then the target, so fade down.
                delta = ((LEDArray[i].r - target[i]) / 8) + 1;  // +1 so delta is always at least 1.
                if (LEDArray[i].r - delta <= target[i])
                {
                    LEDArray[i].r = target[i];  // limit to target.
                }
                else { LEDArray[i].r -= delta; }
            }
            if (i == 0) { temp = delta; }  // Save first pixel's delta number to print below.


            // Continously fade target to zero.
            if (target[i] > lowCutoff)
            {
                target[i] -= 2;  // Fade by this ammount.
            }
            else { target[i] = 0; }  // If target is less then lowCutoff, clamp to zero.


        }//end of looping over pixels.

        FastLED.show();  // Display the LEDArray[] array.

        //// Print out info for led[0] to follow the numbers.
        //Serial.print("c: "); Serial.print(count);
        //Serial.print("\t target: "); Serial.print(target[0]);
        //Serial.print("\t  led[0].r: "); Serial.print(LEDArray[0].r);
        //Serial.print("\t  delta: "); Serial.println(temp);

        count--;  // reduce count by one.
        delay(10);
    }

};

// Test of chasing random target values, version A.
//
// Random target values are generated and assigned to random pixels.
// Pixel brightness is then increased or decreased toward the target.
//
// This version only varies the red channel.
//
// Marc Miller,  April 2015
//---------------------------------------------------------------