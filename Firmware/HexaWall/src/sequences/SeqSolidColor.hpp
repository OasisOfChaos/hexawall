// SeqSolidColor.h

#pragma once

#ifndef _SEQSOLIDCOLORS_h
#define _SEQSOLIDCOLORS_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <FastLED.h>
#include "Sequence.hpp"

class SeqSolidColor : public Sequence
{
private:
	CRGB _currentColor;
	int val = 0;
	fract8 _blendSpeed = 5;
	bool _blendToColor = true;

	void secondTick() {}

public:
	SeqSolidColor(struct CRGB* data, int nLedsOrOffset, CRGB ledSolidColor, bool blendToColor) : Sequence(data, nLedsOrOffset)
	{
		_blendToColor = blendToColor;
		CurrentLEDSolidColor = ledSolidColor;

		if (blendToColor)
		{
			PRINT_DEBUG("SeqSolidColor special: current led brightness: %d.\n", LEDBrightness);

			_currentColor = CRGB::Black;
			fill_solid(LEDArray, LEDCount, _currentColor);
		}
	}


	void fadeall() { for (int i = 0; i < LEDCount; i++) { LEDArray[i].nscale8_video(255); } }

	void tick()
	{
		//EVERY_N_MILLISECONDS(20)
		{
			if (_currentColor == CurrentLEDSolidColor)
			{
				return;
			}

			if (_blendToColor)
			{
				nblendU8TowardU8(_currentColor.r, CurrentLEDSolidColor.r, _blendSpeed);
				nblendU8TowardU8(_currentColor.g, CurrentLEDSolidColor.g, _blendSpeed);
				nblendU8TowardU8(_currentColor.b, CurrentLEDSolidColor.b, _blendSpeed);

				_currentColor = CRGB(_currentColor.r, _currentColor.g, _currentColor.b);
			}
			else
			{
				_currentColor = CurrentLEDSolidColor;
			}

			fill_solid(LEDArray, LEDCount, _currentColor);
		}
	}
};

#endif

