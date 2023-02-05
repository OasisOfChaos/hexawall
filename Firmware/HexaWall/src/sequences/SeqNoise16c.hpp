// SeqNoise16c.h

#pragma once

#include "arduino.h"
#include <FastLED.h>
#include "Sequence.hpp"

class SeqNoise16c : public Sequence
{
private:
	CRGBPalette16 currentPalette;
	CRGBPalette16 targetPalette;
	TBlendType    currentBlending;                                // NOBLEND or LINEARBLEND

	uint8_t maxChanges = 24;      // Number of tweens between palettes.

	void secondTick() {}

public:
	SeqNoise16c(struct CRGB* data, int nLedsOrOffset) : Sequence(data, nLedsOrOffset)
	{
		currentPalette = PartyColors_p;
		currentBlending = LINEARBLEND;
	}

	void tick()
	{
		EVERY_N_MILLISECONDS(50)
		{
			nblendPaletteTowardPalette(currentPalette, targetPalette, maxChanges);  // Blend towards the target palette
		}

		EVERY_N_SECONDS(5)
		{             // Change the target palette to a random one every 5 seconds.
			targetPalette = CRGBPalette16(CHSV(random8(), 255, random8(128, 255)), CHSV(random8(), 255, random8(128, 255)), CHSV(random8(), 192, random8(128, 255)), CHSV(random8(), 255, random8(128, 255)));
		}

		noise16_3();
	}

	void noise16_3()
	{                                            // no x/y shifting but scrolling along

		uint16_t scale = 1000;                                       // the "zoom factor" for the noise

		for (uint16_t i = 0; i < LEDCount; i++)
		{

			uint16_t shift_x = 4223;                                  // no movement along x and y
			uint16_t shift_y = 1234;

			uint32_t real_x = (i + shift_x) * scale;                  // calculate the coordinates within the noise field
			uint32_t real_y = (i + shift_y) * scale;                  // based on the precalculated positions
			uint32_t real_z = millis() * 2;                             // increment z linear

			uint8_t noise = inoise16(real_x, real_y, real_z) >> 7;    // get the noise data and scale it down

			uint8_t index = sin8(noise * 3);                            // map led color based on noise data
			uint8_t bri = noise;

			LEDArray[i] = ColorFromPalette(currentPalette, index, bri, LINEARBLEND);   // With that value, look up the 8 bit colour palette value and assign it to the current LED.
		}

	} // noise16_3()

};