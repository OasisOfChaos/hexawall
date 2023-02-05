// SeqOneSinePal.h

#pragma once

#include "arduino.h"
#include <FastLED.h>
#include "Sequence.hpp"

// Use qsuba for smooth pixel colouring and qsubd for non-smooth pixel colouring
#define qsubd(x, b)  ((x>b)?b:0)                   // Digital unsigned subtraction macro. if result <0, then => 0. Otherwise, take on fixed value.
#define qsuba(x, b)  ((x>b)?x-b:0)                 // Analog Unsigned subtraction macro. if result <0, then => 0

class SeqOneSinePal : public Sequence
{
private:
	int _secondCounter = 0;

	uint8_t thisrot = 1;                                          // You can change how quickly the hue rotates for this wave.
	int8_t thisspeed = 8;                                         // You can change the speed of the wave, and use negative values.
	uint8_t allfreq = 32;                                         // You can change the frequency, thus distance between bars.
	int thisphase = 0;                                            // Phase change value gets calculated.
	uint8_t thiscutoff = 128;                                     // You can change the cutoff value to display this wave. Lower value = longer wave.
	uint8_t thisdelay = 30;                                           // You can change the delay. Also you can change the allspeed variable above.
	uint8_t bgclr = 0;                                            // A rotating background colour.
	uint8_t bgbright = 0;                                        // Brightness of background colour
	uint8_t bgclrinc = 0;
	int startIndex = 0;

	// Palette definitions
	CRGBPalette16 currentPalette;
	CRGBPalette16 targetPalette;
	TBlendType    currentBlending;

	void secondTick() {
		switch (_secondCounter)
		{
			case  0: break;
			case  5: targetPalette = RainbowColors_p; bgclr = 0; bgbright = 32; bgclrinc = 1; thisrot = 1; thisdelay = 10; thisspeed = -4; allfreq = 16; thiscutoff = 128; currentBlending = NOBLEND; break;
			case 10: targetPalette = ForestColors_p; thisspeed = 12; thisrot = 0; thisspeed = 12; thisdelay = 10; thiscutoff = 128; allfreq = 64; bgclr = 50; bgbright = 15; currentBlending = LINEARBLEND; break;
			case 15: SetupRandomColorPalette(); thisrot = 2; thisspeed = 16; break;
			case 20: targetPalette = LavaColors_p; thisspeed = 4; break; allfreq = 16; bgclr = 50; break;
			case 25: thiscutoff = 96; thisspeed = -4; thiscutoff = 224; bgclr = 20; bgbright = 8; break;
			case 30: SetupRandomColorPalette(); thiscutoff = 96; thisrot = 1; break;
			case 35: targetPalette = OceanColors_p; break;
			case 40: SetupSimilarColorPalette(); thiscutoff = 128; break;
			case 45: SetupSimilarColorPalette(); thisspeed = 3; break;
			case 50: targetPalette = PartyColors_p; break;
			case 55: break;
			case 60: break;
		}

		if (++_secondCounter > 60)
		{
			_secondCounter = 0;
		}
	}

public:
	SeqOneSinePal(struct CRGB* data, int nLedsOrOffset) : Sequence(data, nLedsOrOffset)
	{
		currentPalette = LavaColors_p;
		currentBlending = LINEARBLEND;
	}


	void tick()
	{
		EVERY_N_MILLISECONDS(100)
		{
			uint8_t maxChanges = 24;
			nblendPaletteTowardPalette(currentPalette, targetPalette, maxChanges);   // AWESOME palette blending capability.
		}

		EVERY_N_MILLISECONDS(thisdelay)
		{
			// FastLED based non-blocking delay to update/display the sequence.
			startIndex += thisrot;                                    // Motion speed
			one_sine_pal(startIndex);
		}
	}


	void one_sine_pal(uint8_t colorIndex)
	{                                                         // This is the heart of this program. Sure is short.

		thisphase += thisspeed;                                                     // You can change direction and speed individually.

		for (int k = 0; k < LEDCount; k++)
		{                                          // For each of the LED's in the strand, set a brightness based on a wave as follows:
			int thisbright = qsuba(cubicwave8((k * allfreq) + thisphase), thiscutoff);    // qsub sets a minimum value called thiscutoff. If < thiscutoff, then bright = 0. Otherwise, bright = 128 (as defined in qsub)..
			LEDArray[k] = CHSV(bgclr, 255, bgbright);                                     // First set a background colour, but fully saturated.
			LEDArray[k] += ColorFromPalette(currentPalette, colorIndex + k, thisbright, currentBlending);
			colorIndex += thisrot;
		}

		bgclr += bgclrinc;                                                                    // You can change the background colour or remove this and leave it fixed.

	} // one_sine_pal()

	void SetupRandomColorPalette()
	{

		targetPalette = CRGBPalette16(CHSV(random8(), 255, random8(128, 255)), CHSV(random8(), 255, random8(128, 255)), CHSV(random8(), 192, random8(128, 255)), CHSV(random8(), 255, random8(128, 255)));

	} // SetupRandomColorPalette()



	void SetupSimilarColorPalette()
	{

		static uint8_t baseC = random8();                         // You can use this as a baseline colour if you want similar hues in the next line, i.e. baseC+random8(0,5) for the hue.
		targetPalette = CRGBPalette16(CHSV(baseC + random8(0, 10), 255, random8(128, 255)), CHSV(baseC + random8(0, 10), 255, random8(128, 255)), CHSV(baseC + random8(0, 10), 192, random8(128, 255)), CHSV(baseC + random8(0, 10), 255, random8(128, 255)));

	} // SetupSimilarColorPalette()
};