// SeqFadeInLeftToRight.h

#pragma once

#include "arduino.h"
#include <FastLED.h>
#include "Sequence.hpp"

#define MAX_SEQUENCE	11

//https://forum.makerforums.info/t/im-wondering-if-there-is-a-fastled-function-or-code-to-fade-the-brightness/61696

class SeqFadeInLeftToRight : public Sequence
{
private:
	uint8_t _sequence[MAX_SEQUENCE][3] = {
		{22, 21, 255},
		{23, 19, 255},
		{24, 20, 18},
		{14, 15, 16},
		{13, 17, 255},
		{11, 10, 9},
		{12, 8, 255},
		{3, 7, 255},
		{2, 4, 6},
		{1, 5, 255},
		{0, 255, 255}
	};

	uint8_t _sequenceIndex = 0;
	boolean _fadeHasEnded = false;

	//uint8_t _currentLED = 0;

	uint8_t fadeRate = 35;

	//uint8_t _prevCurrentLED = 255;

	/*uint8_t _hue;
	CRGB color = CRGB::Red;
	int _start = 0;
	int _end = 30;*/

	//int brightness = 0;
	//boolean fadeToColor = 1;

	//CRGB currentColor;  // starting color
	//CRGB targetColor;   // starting target color

	uint8_t hue = 0;
	uint8_t sat = 255;
	int val = 0;

	boolean _fadeDirection = 0;  // [1=fade up, 0=fade down]

	void secondTick() {}

public:
	//SeqFadeInLeftToRight(struct CRGB* data, int nLedsOrOffset) : Sequence(data, nLedsOrOffset)
	//{
	//	//reset();
	//}

	SeqFadeInLeftToRight(struct CRGB* data, int nLedsOrOffset, uint8_t direction) : Sequence(data, nLedsOrOffset)
	{
		_fadeDirection = direction;

		reset();
	}

	//String Name() { return "FadeInLeftToRight"; }

	void reset()
	{
		_fadeHasEnded = false;
		_isFinished = false;

		if (_fadeDirection == 0)
		{
			_sequenceIndex = 0;
			fill_solid(LEDArray, LEDCount, CRGB::Black);
		}
		else
		{
			_sequenceIndex = 10;
			fadeRate = fadeRate * -1;
		}
	}

	//void fadeall() { for (int i = 0; i < LEDCount; i++) { LEDArray[i].nscale8(250); } }

	void tick()
	{
		EVERY_N_SECONDS(30)
		{
			reset();
		}

		if (_fadeHasEnded)
		{
			//set flag if not set yet
			if (!_isFinished)
			{
				Serial.println("Sequence SeqFadeInLeftToRight is finished fading.");
				_isFinished = true;
			}

			return;
		}

		val = val + fadeRate;
		if (val >= 250)
		{
			val = 255;
		}

		if (val <= 10)
		{
			val = 0;
		}

		for (uint8_t i = 0; i < 3; i++)
		{
			if (_sequence[_sequenceIndex][i] != 255)
			{
				if (_fadeDirection == 0)
				{
					fill_solid(LEDArray + this->getIndex(_sequence[_sequenceIndex][i]), 30, CHSV(hue, sat, val));
				}
				else
				{
					fadeToBlackBy(LEDArray + this->getIndex(_sequence[_sequenceIndex][i]), 30, val);
				}

				//Serial.printf("index: %d, led: %d\n", _sequenceIndex, _sequence[_sequenceIndex][i]);
			}
		}

		if (_fadeDirection == 0)
		{
			if (val == 255)
			{
				val = 0;

				if (++_sequenceIndex > MAX_SEQUENCE - 1)
				{
					_fadeHasEnded = true;
					return;
				}
			}
		}
		else
		{
			if (val == 0)
			{
				val = 255;

				if (--_sequenceIndex == 0)
				{
					_fadeHasEnded = true;
					return;
				}
			}
		}

	}

	//void tick2()
	//{
	//	if (_fadeDirection == 1)
	//	{  //fade up
	//		//EVERY_N_MILLISECONDS(3)
	//		{
	//			fill_solid(LEDArray, LEDCount, CHSV(hue, sat, val));
	//			val = val + fadeRate;
	//			if (val >= 255)
	//			{
	//				val = 255;
	//				_fadeDirection = !_fadeDirection;  //reverse direction
	//				Serial.print("---fade up done---");
	//				Serial.print("  _fadeDirection: "); Serial.println(_fadeDirection);
	//			}
	//		}
	//	}

	//	if (_fadeDirection == 0)
	//	{  //fade down
	//		//EVERY_N_MILLISECONDS(9)
	//		{
	//			fill_solid(LEDArray, LEDCount, CHSV(hue, sat, val));
	//			val = val - fadeRate;
	//			if (val <= 0)
	//			{
	//				val = 0;
	//				_fadeDirection = !_fadeDirection;  //reverse direction
	//				Serial.print("--fade down done--");
	//				Serial.print("  _fadeDirection: "); Serial.println(_fadeDirection);
	//				hue++;
	//			}
	//		}
	//	}

	//	//FastLED.show();

	//	EVERY_N_MILLISECONDS(90)
	//	{  //cycle around the color wheel over time
	//		//hue++;
	//	}
	//}

	//void tick1()
	//{
	//	EVERY_N_SECONDS(30)
	//	{
	//		reset();
	//	}

	//	//EVERY_N_MILLISECONDS(fadeRate)
	//	{
	//		if (currentColor == targetColor)
	//		{
	//			if (++_sequenceIndex > MAX_SEQUENCE)
	//			{
	//				fadeToColor = 0;  // stop fading
	//				return;
	//			}

	//			currentColor = CRGB::Black;

	//			//get current color of first pixel of first hexagon
	//			//currentColor = CRGB(LEDArray[_sequence[_sequenceIndex][0]].r, LEDArray[_sequence[_sequenceIndex][0]].g, LEDArray[_sequence[_sequenceIndex][0]].b);

	//			//wait a while between the 'rows'...
	//			//FastLED.delay(300);
	//		}

	//		if (fadeToColor)
	//		{
	//			nblendU8TowardU8(currentColor.r, targetColor.r, 80);
	//			nblendU8TowardU8(currentColor.g, targetColor.g, 80);
	//			nblendU8TowardU8(currentColor.b, targetColor.b, 80);

	//			currentColor = CRGB(currentColor.r, currentColor.g, currentColor.b);

	//			for (uint8_t i = 0; i < 3; i++)
	//			{
	//				if (_sequence[_sequenceIndex][i] != 255)
	//				{
	//					fill_solid(LEDArray + (_sequence[_sequenceIndex][i] * 30), 30, currentColor);
	//				}
	//			}
	//		}

	//	}
	//}



	// =================================================
	// Modified helper function that blends one uint8_t toward another,
	// based on function from Mark Kriegsman's fadeTowardColor example:
	// https://gist.github.com/kriegsman/d0a5ed3c8f38c64adcb4837dafb6e690
	void nblendU8TowardU8(uint8_t& current, const uint8_t target, fract8 scale = 1)
	{
		if (current == target)
		{
			return;
		}

		if (current < target)
		{
			uint8_t delta = target - current;
			delta = scale8_video(delta, scale);
			current += delta;
		}
		else
		{
			uint8_t delta = current - target;
			delta = scale8_video(delta, scale);
			current -= delta;
		}
	}
};