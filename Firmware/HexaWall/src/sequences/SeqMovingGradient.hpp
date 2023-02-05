// SeqMovingGradient.h

#pragma once

#include "arduino.h"
#include <FastLED.h>
#include "Sequence.hpp"

class SeqMovingGradient : public Sequence
{
private:
	// User variables
	CHSV gradStartColor;  // Gradient start color.
	CHSV gradEndColor;  // Gradient end color.
	uint16_t gradStartPos = 0;  // Starting position of the gradient.
#define gradLength 8  // How many pixels (in total) is the grad from start to end.

	int8_t gradDelta = 1;  // 1 or -1.  (Negative value reverses direction.)

	// If you wanted to move your gradient 32 pixels in 120 seconds, then:
	// 120sec / 32pixel = 3.75sec
	// 3.75sec x 1000miliseconds/sec = 3750milliseconds
#define gradMoveDelay 3750  // How fast to move the gradient (in Milliseconds)

	CRGB grad[gradLength];  // A place to save the gradient colors. (Don't edit this)

public:
	SeqMovingGradient(struct CRGB* data, int nLedsOrOffset) : Sequence(data, nLedsOrOffset) {
		gradStartColor= CHSV(0, 255, 255);
		gradEndColor = CHSV(161, 255, 255);

		//fill the array that stores the gradient colors.
		fill_gradient(grad, gradStartPos, gradStartColor, gradStartPos + gradLength - 1, gradEndColor, SHORTEST_HUES);
	}

	void tick()
	{
		EVERY_N_MILLISECONDS(gradMoveDelay)
		{
			uint16_t count = 0;
			for (uint16_t i = gradStartPos; i < gradStartPos + gradLength; i++)
			{
				LEDArray[i % LEDCount] = grad[count];
				count++;
			}
			FastLED.show();  // Display the pixels.
			FastLED.clear();  // Clear the strip to not leave behind lit pixels as grad moves.

			gradStartPos = gradStartPos + gradDelta;  // Update start position.
			if ((gradStartPos > LEDCount - 1) || (gradStartPos < 0))
			{
				// Check if outside LEDCount range
				gradStartPos = gradStartPos % LEDCount;  // Loop around as needed.
			}

		} //end EVERY_N gradMoveDelay}
	}
};

//***************************************************************
// Moving gradient example
// It will wrap around, and the direction can be reversed.
// This is done a bit different then you might expect as I'm
// filling the gradient once in setup() and saving the colors
// in a seperate CRGB array.  Then coping those into the main
// leds array as needed when moving it around.
//
// Marc Miller, Jan 2016
//***************************************************************
