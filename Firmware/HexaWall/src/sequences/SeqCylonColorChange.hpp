// SeqCylonColorChange.h

//bug at end of sequence

#pragma once

#include "arduino.h"
#include <FastLED.h>
#include "Sequence.hpp"

class SeqCylonColorChange : public Sequence
{
private:
	// Custom colors to choose from
	CRGB colorArray[4] = {
	  CRGB(0,255,0),    //green
	  CRGB(180,180,180),  //a dim white
	  CHSV(0,255,255),  //red
	  CRGB::Grey,
	};

	uint8_t numberOfColors = sizeof(colorArray) / sizeof(colorArray[0]);
	uint8_t colorChoice;  //the current color choice
	int pos = 0;  //dot position
	int delta = 1;  //direction of travel

	void secondTick() {}

public:
	SeqCylonColorChange(struct CRGB* data, int nLedsOrOffset) : Sequence(data, nLedsOrOffset) {}

	void tick()
	{
		//Comment or un-comment one of these examples to try each out.
		cylon_cc1();
		//cylon_cc2();
	}

	//---------------------------------------------------------------
	void colorChange(bool r)
	{
		try
		{
			// Change the color.
			// If r = 0, the next color in the array is picked and then
			// it cycles back around to the first color in the array.
			// If r = 1 then a random color is picked from the array.

			if (r == 0)
			{
				colorChoice = colorChoice + 1;
				if (colorChoice >= numberOfColors) { colorChoice = 0; }
				Serial.print("cycling: "); Serial.println(colorChoice);
			}
			else
			{
				colorChoice = random8(numberOfColors);
				Serial.print("random: "); Serial.println(colorChoice);
			}
		}
		catch (...)
		{
			Serial.println("Exception in colorChange");
		}
	}


	//---------------------------------------------------------------
	void cylon_cc1()
	{
		//cylon effect with color change based on time
		EVERY_N_MILLISECONDS(200)
		{
			colorChange(0);  //set next color
		}

		EVERY_N_MILLISECONDS(60)
		{
			try
			{
				fadeToBlackBy(LEDArray, LEDCount, 5);

				LEDArray[pos] = colorArray[colorChoice];

				pos = pos + delta;

				if (pos == LEDCount - 1) { delta = -1; }  //change direction
				if (pos == 0) { delta = 1; }  //change direction
			}
			catch (...)
			{
				Serial.println("Exception in cylon_cc1");
			}

		}

	}//end_cylon_cc1


	//---------------------------------------------------------------
	void cylon_cc2()
	{
		//cylon effect with color change when end of strip is reached

		EVERY_N_MILLISECONDS(60)
		{
			fadeToBlackBy(LEDArray, LEDCount, 200);
			LEDArray[pos] = colorArray[colorChoice];
			pos = pos + delta;
			if (pos == LEDCount - 1)
			{
				delta = -1;  //change direction
				colorChange(true);  //random next color
			}
			if (pos == 0)
			{
				delta = 1;  //change direction
				colorChange(true);  //random next color
			}
		}

	}//end_cylon_cc2
};

//***************************************************************
// This is a Cylon effect that doesn't use delay().  The color
// of the moving dot is picked from a set of custom colors
// stored in an array.
//
// There are two different examples in the main loop.
// Un-comment one at a time to try each out.  One changes color
// based on a timer, and the other changes color whenever the
// dot reaches the end of the stirp.
// The color change could also be based on something such as
// a potentiometer, distance sensor, button, sine wave, random, etc.
//
// Marc Miller, Nov 2018
//***************************************************************
