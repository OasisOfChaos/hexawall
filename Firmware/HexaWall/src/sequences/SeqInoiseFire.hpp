// SeqInoiseFire.h

#pragma once

#include "arduino.h"
#include <FastLED.h>
#include "Sequence.hpp"

class SeqInoiseFire : public Sequence
{
private:
	CRGBPalette16 currentPalette = CRGBPalette16(
		CRGB::Black, CRGB::Black, CRGB::Black, CHSV(0, 255, 4),
		CHSV(0, 255, 8), CRGB::Red, CRGB::Red, CRGB::Red,
		CRGB::DarkOrange, CRGB::Orange, CRGB::Orange, CRGB::Orange,
		CRGB::Yellow, CRGB::Yellow, CRGB::Gray, CRGB::Gray);

	uint32_t xscale = 20;                                          // How far apart they are
	uint32_t yscale = 5;                                           // How fast they move
	uint8_t index = 0;

	void secondTick() {}

public:
	SeqInoiseFire(struct CRGB* data, int nLedsOrOffset) : Sequence(data, nLedsOrOffset) {}

	void tick()
	{
		for (int i = 0; i < LEDCount; i++)
		{
			index = inoise8(i * xscale, millis() * yscale * LEDCount / 255);												// X location is constant, but we move along the Y at the rate of millis()
			LEDArray[i] = ColorFromPalette(currentPalette, min(i * (index) >> 6, 255), i * 255 / LEDCount, LINEARBLEND);  // With that value, look up the 8 bit colour palette value and assign it to the current LED.
		}                                                                                                   // The higher the value of i => the higher up the palette index (see palette definition).
																											// Also, the higher the value of i => the brighter the LED.
	}

};