// SeqFadeOutRightToLeft.h

#pragma once

#include "arduino.h"
#include <FastLED.h>
#include "Sequence.hpp"

#define MAX_SEQUENCE	11

//https://forum.makerforums.info/t/im-wondering-if-there-is-a-fastled-function-or-code-to-fade-the-brightness/61696

class SeqFadeOutRightToLeft : public Sequence
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

	uint8_t _sequenceIndex = 10;
	boolean _fadeHasEnded = false;
	//uint8_t _fadeRate = 30;
	uint8_t _fadeRate = 5;

public:
	SeqFadeOutRightToLeft(struct CRGB* data, int nLedsOrOffset) : Sequence(data, nLedsOrOffset)
	{
		_fadeHasEnded = false;
		_isFinished = false;
	}

	void tick()
	{
		if (_fadeHasEnded)
		{
			//set flag if not set yet
			if (!_isFinished)
			{
				fill_solid(LEDArray, LEDCount, CRGB::Black);

				Serial.println("Sequence SeqFadeOutRightToLeft is finished fading.");
				_isFinished = true;
			}

			return;
		}

		//Serial.printf("index: %d, index+30: %d, _sequenceIndex: %d\r\n", (_sequence[_sequenceIndex][0] * 30), (_sequence[_sequenceIndex][0] * 30)+30, _sequenceIndex);

		for (uint8_t i = 0; i < 3; i++)
		{
			if (_sequence[_sequenceIndex][i] != 255)
			{
				fadeToBlackBy(LEDArray + this->getIndex(_sequence[_sequenceIndex][i]), 30, _fadeRate);

				Serial.printf("index: %d, led: %d, ledindex: %d, R: %d, R: %d, B: %d\n",
					_sequenceIndex, _sequence[_sequenceIndex][i], this->getIndex(_sequence[_sequenceIndex][i]),
					LEDArray[this->getIndex(_sequence[_sequenceIndex][0])].r,
					LEDArray[this->getIndex(_sequence[_sequenceIndex][0])].g,
					LEDArray[this->getIndex(_sequence[_sequenceIndex][0])].b);
			}
		}

		if (LEDArray[this->getIndex(_sequence[_sequenceIndex][0])] == CRGB(0, 0, 0))
		{
			//ATTENTION: deliberate use of OVERFLOW here!!
			if (--_sequenceIndex == 255)
			{
				_fadeHasEnded = true;
			}
		}
	}
};