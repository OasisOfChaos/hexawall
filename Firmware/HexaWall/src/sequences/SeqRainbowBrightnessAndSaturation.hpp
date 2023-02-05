// SeqRainbowBrightnessAndSaturation.h

#pragma once

#include "arduino.h"
#include <FastLED.h>
#include "Sequence.hpp"

class SeqRainbowBrightnessAndSaturation : public Sequence
{
private:
	uint8_t V;  //brightness for rainbow
	uint8_t S;  //saturation for rainbow
	boolean toggleS;
	boolean toggleV;

	//Amount to tint (desaturate) rainbow.  Can use either RGB or HSV format
	//CRGB tintAmt(128,128,128);
	CHSV tintAmt;

public:
	SeqRainbowBrightnessAndSaturation(struct CRGB* data, int nLedsOrOffset) : Sequence(data, nLedsOrOffset) {

		tintAmt = CHSV(0, 0, 90);
		//tintAmt = CHSV(128, 128, 128);

		Serial.println("SeqRainbowBrightnessAndSaturation");
	}

	void tick()
	{
		fill_rainbow(LEDArray, LEDCount, millis() / 100);  //fill strip with moving rainbow

		EVERY_N_SECONDS(5) { toggleS = !toggleS; }  //toggle Saturation (ie. "tint")
		EVERY_N_SECONDS(30) { toggleV = !toggleV; }  //toggle Value (brightness)

		if (toggleS)
		{
			for (int16_t i = 0; i < LEDCount; i++)
			{
				LEDArray[i] += tintAmt;
			}
		}
		else
		{
			//No tint added
		}

		if (toggleV)
		{
			V = 128;  //fade down by V/256
			fadeToBlackBy(LEDArray, LEDCount, V);  //fade by value V
		}
		else
		{
			V = 0;  //no fade
		}
	}

};

//***************************************************************
//  Example of changing the brightness and saturation of
//  FastLED's fill_rainbow, which by default fills pixels with
//  the rainbow at full brightness and full saturation.
//
//  Marc Miller, Sept 2018
//***************************************************************