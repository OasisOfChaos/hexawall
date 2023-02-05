// SeqCyclePrimaryColors.h

#ifndef _SEQCYCLEPRIMARYCOLORS_h
#define _SEQCYCLEPRIMARYCOLORS_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <FastLED.h>
#include "Sequence.hpp"

class SeqCyclePrimaryColors : public Sequence
{
private:
	static const uint8_t DURATION = 5;
	static const uint8_t COLORCOUNT = 3;
	const fract8 _blendSpeed = 15;
	const CRGB _colors[COLORCOUNT] = {CRGB(255, 0, 0), CRGB(0, 255, 0), CRGB(0, 0, 255)};

	CRGB _currentColor;
	CRGB _targetColor;
	bool _blendToColor = true;

	uint8_t counter = 0;
	uint8_t color = 0;

	void secondTick()
	{
		if(_blendToColor) {
			return;
		}

		// Serial.printf("counter:%d, color:%d\r\n", counter, color);

		if (counter % DURATION == 0)
		{
			//start fading to new color
			_targetColor = _colors[color];
			_blendToColor = true;
		}

		if (counter++ == DURATION)
		{
			counter = 0;

			if (++color > COLORCOUNT - 1)
			{
				color = 0;
			}
		}
	}

public:
	SeqCyclePrimaryColors(struct CRGB *data, int nLedsOrOffset) : Sequence(data, nLedsOrOffset) {}

	void tick()
	{
		if (_currentColor == _targetColor)
		{
			_blendToColor = false;
			return;
		}

		if (_blendToColor)
		{
			nblendU8TowardU8(_currentColor.r, _targetColor.r, _blendSpeed);
			nblendU8TowardU8(_currentColor.g, _targetColor.g, _blendSpeed);
			nblendU8TowardU8(_currentColor.b, _targetColor.b, _blendSpeed);

			_currentColor = CRGB(_currentColor.r, _currentColor.g, _currentColor.b);
		}
		else
		{
			_currentColor = _targetColor;
		}

		fill_solid(LEDArray, LEDCount, _currentColor);
	}
};

#endif
