// SeqStripedPalettes.h

#pragma once

#include "arduino.h"
#include <FastLED.h>
#include "Sequence.hpp"

class SeqStripedPalettes : public Sequence
{
private:
#define UPDATES_PER_SECOND 100
    int _secondCounter = 0;

	CRGBPalette16 currentPalette;
	TBlendType    currentBlending;

	uint8_t colorDelta = 8;  // determines the size of the "stripe" length/gap

    void secondTick() {
        CRGB p = CHSV(HUE_PURPLE, 255, 255);
        CRGB g = CHSV(HUE_GREEN, 255, 255);
        CRGB b = CRGB::Black;
        CRGB w = CRGB::White;

        switch (_secondCounter)
        {
            case  0: UpdateColorIndexDelta(); SetupPalette_RandomStripes1(); currentBlending = LINEARBLEND; break;
            case  6: SetupPalette_RandomStripes1(); currentBlending = LINEARBLEND; break;
            case 12: SetupPalette_RandomStripes2(); currentBlending = LINEARBLEND; break;
            case 18: SetupPalette_RandomStripes2(); currentBlending = LINEARBLEND; break;
            case 24: SetupPalette_RandomStripes2(); currentBlending = LINEARBLEND; break;
            case 30: SetupPalette_GradientStriped(); currentBlending = LINEARBLEND; break;
            case 45: SetupPalette_GradientRandomStriped(); currentBlending = LINEARBLEND; break;
            case 50: SetupPalette_GradientRandomStriped(); currentBlending = LINEARBLEND; break;
            case 55: SetupPalette_GradientRandomStriped(); currentBlending = LINEARBLEND; break;
        }

        if (++_secondCounter > 60)
        {
            _secondCounter = 0;
        }
    }

public:
	SeqStripedPalettes(struct CRGB* data, int nLedsOrOffset) : Sequence(data, nLedsOrOffset)
	{
		SetupPalette_RandomStripes1();  // starting out with this palette
		currentBlending = LINEARBLEND;
	}

	void tick()
	{
		static uint8_t startIndex = 0;

		EVERY_N_MILLISECONDS(20)
		{  //lower value = faster
			startIndex = startIndex + 1;  //higher + value = faster
		}

		FillLEDsFromPaletteColors(startIndex);
	}

	void UpdateColorIndexDelta()
	{
		colorDelta = random8(4, 13);  //pick new random delta
		//Serial.print("  New colorDelta: "); //Serial.println(colorDelta);
	}


	void FillLEDsFromPaletteColors(uint8_t colorIndex)
	{
		uint8_t brightness = 255;

		for (int i = 0; i < LEDCount; i++)
		{
			LEDArray[i] = ColorFromPalette(currentPalette, colorIndex, brightness, currentBlending);
			colorIndex += colorDelta;
		}
	}

    //---------------------------------------------------------------
// Custom Palettes
//---------------------------------------------------------------

    void SetupPalette_RandomStripes1()
    {
        // 'black out' all 16 palette entries...
        fill_solid(currentPalette, 16, CRGB::Black);
        // pick a random color and set every fourth one to this color.
        CHSV randomColor = CHSV(random8(), 255, 255);
        currentPalette[0] = randomColor;
        currentPalette[4] = randomColor;
        currentPalette[8] = randomColor;
        currentPalette[12] = randomColor;

        //Serial.println("Running: SetupPalette_RandomStripes1");
    }


    void SetupPalette_RandomStripes2()
    {
        // 'black out' all 16 palette entries...
        fill_solid(currentPalette, 16, CRGB::Black);
        // and set every fourth one with a few random colors.
        currentPalette[0] = CHSV(random8(), random8(100, 255), 255);
        currentPalette[4] = CHSV(random8(), random8(100, 255), 255);
        currentPalette[8] = currentPalette[0];  //setting the same as 0 for some uniformity
        currentPalette[12] = currentPalette[4];  //setting the same as 4 for some uniformity

        //Serial.println("Running: SetupPalette_RandomStripes2");
    }


    void SetupPalette_GradientStriped()
    {
        // 'black out' all 16 palette entries...
        fill_solid(currentPalette, 16, CRGB::Black);

        // Fill a temp CRGB array with specific colors using fill_gradient.
        //     fill_gradient works like this:
        //     fill_gradient(arrayToUse, startpos, startColor, endpos, endColor, blendType);
        CRGB temp[4];  //size of 4 since that's all we need for below.
        fill_gradient(temp, 0, CHSV(90, 255, 255), 3, CHSV(135, 255, 255), SHORTEST_HUES);

        // Set every fourth one using colors from gradient in temp.
        currentPalette[0] = temp[0];
        currentPalette[4] = temp[1];
        currentPalette[8] = temp[2];
        currentPalette[12] = temp[3];

        //Serial.println("Running: SetupPalette_GradientStriped");
    }


    void SetupPalette_GradientRandomStriped()
    {
        // 'black out' all 16 palette entries...
        fill_solid(currentPalette, 16, CRGB::Black);

        // Fill a temp CRGB array with random colors using fill_gradient.
        //     fill_gradient works like this:
        //     fill_gradient(arrayToUse, startpos, startColor, endpos, endColor, blendType);
        CRGB temp[4];  //size of 4 since that's all we need for below.
        uint8_t randomHue = random8();
        fill_gradient(temp, 0, CHSV(randomHue, 255, 255), 3, CHSV(randomHue + 60, 255, 255), SHORTEST_HUES);
        //For the endColor we are adding 60 so the two colors are
        //always relatively close together on the color wheel.

        // Set every fourth one using colors from gradient in temp.
        currentPalette[0] = temp[0];
        currentPalette[4] = temp[1];
        currentPalette[8] = temp[2];
        currentPalette[12] = temp[3];

        //Serial.println("Running: SetupPalette_GradientRandomStriped");
    }
};

//***************************************************************
// Messing with "striped" color palettes.
//
// This example is based off of the FastLED ColorPalette example.
// See that exmaple for some more info on color palettes.
// https://github.com/FastLED/FastLED/blob/master/examples/ColorPalette/ColorPalette.ino
//
//
// Marc Miller, Oct 2020
//***************************************************************
