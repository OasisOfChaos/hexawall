// SeqSwirlOut.h

#pragma once

#include "arduino.h"
#include <FastLED.h>
#include "Sequence.hpp"

#define MAX_SEQUENCE	11

//https://forum.makerforums.info/t/im-wondering-if-there-is-a-fastled-function-or-code-to-fade-the-brightness/61696

class SeqSwirlOut : public Sequence
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
	uint8_t roundIndex = 0;

	void secondTick() {}

public:
	SeqSwirlOut(struct CRGB* data, int nLedsOrOffset) : Sequence(data, nLedsOrOffset)
	{
		_fadeHasEnded = false;
		_isFinished = false;
	}

	void tick()
	{
		EVERY_N_MILLISECONDS(5)
		{
			if (_fadeHasEnded)
			{
				//set flag if not set yet
				if (!_isFinished)
				{
					fill_solid(LEDArray, LEDCount, CRGB::Black);

					//Serial.println("Sequence SwirlOut is finished.");
					_isFinished = true;
				}

				return;
			}

			//Serial.printf("index: %d, index+30: %d, _sequenceIndex: %d\r\n", (_sequence[_sequenceIndex][0] * 30), (_sequence[_sequenceIndex][0] * 30)+30, _sequenceIndex);

			for (uint8_t i = 0; i < 3; i++)
			{
				if (_sequence[_sequenceIndex][i] != 255)
				{
					LEDArray[this->getIndex(_sequence[_sequenceIndex][i]) + roundIndex] = CRGB::Black;
				}
			}

			if (++roundIndex > 30)
			{
				roundIndex = 0;

				if (--_sequenceIndex == 255)
				{
					_fadeHasEnded = true;
				}
			};
		}
	}
};