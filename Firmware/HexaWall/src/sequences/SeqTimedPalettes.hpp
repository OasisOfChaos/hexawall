// SeqTimedPalettes.h

#ifndef _SEQTIMEDPALETTES_H
#define _SEQTIMEDPALETTES_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <FastLED.h>
#include "Sequence.hpp"

class SeqTimedPalettes : public Sequence
{
private:
	CRGBPalette16 _currentPalette;
	TBlendType    _currentBlending ;

	uint8_t _colorIndex = 0;
	int _secondCounter = 0;

	void secondTick() {
		//Serial.printf("_secondCounter: %d\n", _secondCounter);

		// FastLED provides several 'preset' palettes: RainbowColors_p, RainbowStripeColors_p,
		// OceanColors_p, CloudColors_p, LavaColors_p, ForestColors_p, and PartyColors_p.

		if (_secondCounter == 00) { _currentPalette = RainbowColors_p;			_currentBlending = LINEARBLEND; }
		if (_secondCounter == 10) { _currentPalette = RainbowStripeColors_p;	_currentBlending = LINEARBLEND; }
		if (_secondCounter == 20) { _currentPalette = OceanColors_p;			_currentBlending = LINEARBLEND; }
		if (_secondCounter == 30) { _currentPalette = LavaColors_p;				_currentBlending = LINEARBLEND; }
		if (_secondCounter == 40) { _currentPalette = ForestColors_p;			_currentBlending = LINEARBLEND; }
		if (_secondCounter == 50) { _currentPalette = CloudColors_p;			_currentBlending = LINEARBLEND; }
		if (_secondCounter == 60) { _currentPalette = PartyColors_p;			_currentBlending = LINEARBLEND; }

		if (_secondCounter == 70) { SetupPurpleAndGreenPalette();				_currentBlending = LINEARBLEND; }
		if (_secondCounter == 80) { SetupTotallyRandomPalette();				_currentBlending = LINEARBLEND; }
		if (_secondCounter == 90) { SetupBlackAndWhiteStripedPalette();			_currentBlending = LINEARBLEND; }
		if (_secondCounter == 100) { _currentPalette = myRedWhiteBluePalette_p; _currentBlending = LINEARBLEND; }

		if (++_secondCounter > 100)
		{
			_secondCounter = 0;
		}
	}

public:
	SeqTimedPalettes(struct CRGB* data, int nLedsOrOffset) : Sequence(data, nLedsOrOffset)
	{
		_currentPalette = RainbowColors_p;
		_currentBlending  = LINEARBLEND;
	}

	void tick()
	{
		_colorIndex = _colorIndex + 1; /* motion speed */

		FillLEDsFromPaletteColors(_colorIndex);
	}

	void FillLEDsFromPaletteColors(uint8_t colorIndex)
	{
		for (int i = 0; i < LEDCount; i++)
		{
			LEDArray[i] = ColorFromPalette(_currentPalette, colorIndex, LEDBrightness, _currentBlending );
			colorIndex += 3;
		}
	}

	// This function fills the palette with totally random colors.
	void SetupTotallyRandomPalette()
	{
		for (int i = 0; i < 16; i++)
		{
			_currentPalette[i] = CHSV(random8(), 255, random8());
		}
	}

	// This function sets up a palette of black and white stripes,
	// using code.  Since the palette is effectively an array of
	// sixteen CRGB colors, the various fill_* functions can be used
	// to set them up.
	void SetupBlackAndWhiteStripedPalette()
	{
		// 'black out' all 16 palette entries...
		fill_solid(_currentPalette, 16, CRGB::Black);
		// and set every fourth one to white.
		_currentPalette[0] = CRGB::White;
		_currentPalette[4] = CRGB::White;
		_currentPalette[8] = CRGB::White;
		_currentPalette[12] = CRGB::White;
	}

	// This function sets up a palette of purple and green stripes.
	void SetupPurpleAndGreenPalette()
	{
		CRGB purple = CHSV(HUE_PURPLE, 255, 255);
		CRGB green = CHSV(HUE_GREEN, 255, 255);
		CRGB black = CRGB::Black;

		_currentPalette = CRGBPalette16(
			green, green, black, black,
			purple, purple, black, black,
			green, green, black, black,
			purple, purple, black, black);
	}


	// This example shows how to set up a static color palette
	// which is stored in PROGMEM (flash), which is almost always more
	// plentiful than RAM.  A static PROGMEM palette like this
	// takes up 64 bytes of flash.
	const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM =
	{
		CRGB::Red,
		CRGB::Gray, // 'white' is too bright compared to red and blue
		CRGB::Blue,
		CRGB::Black,

		CRGB::Red,
		CRGB::Gray,
		CRGB::Blue,
		CRGB::Black,

		CRGB::Red,
		CRGB::Red,
		CRGB::Gray,
		CRGB::Gray,
		CRGB::Blue,
		CRGB::Blue,
		CRGB::Black,
		CRGB::Black
	};
};

#endif

