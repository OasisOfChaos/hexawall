// SeqDripping.h

#pragma once

#include "arduino.h"
#include <FastLED.h>
#include "Sequence.hpp"
#include "Hexagon.hpp"

//one hexagon has 30 leds
//define paths? or network?

//struct Hexagon
//{
//	/*
//	 6 / \ 1
//	5 |   | 2
//	 4 \ / 3
//	*/
//	/// default values are UNITIALIZED
//	inline Hexagon() __attribute__((always_inline)) = default;
//
//	inline Hexagon(uint8_t number, uint8_t side1, uint8_t side2, uint8_t side3, uint8_t side4, uint8_t side5, uint8_t side6)
//	{
//		Number = number;
//		Sides[0] = side1;
//		Sides[1] = side2;
//		Sides[2] = side3;
//		Sides[3] = side4;
//		Sides[4] = side5;
//		Sides[5] = side6;
//	}
//
//	uint8_t Number;
//	uint8_t Sides[6];
//};

class SeqDripping : public Sequence
{
private:
	Hexagon _hexagons[25];
	int _mscounter = 0;
	uint8_t _currentLED = 0;
	uint8_t _prevCurrentLED = 255;

	uint8_t _hue;
	CRGB color = CRGB::Red;
	int _start = 0;
	int _end = 30;

	bool filled[25];
	bool _finished = false;

	void secondTick() {}

public:
	SeqDripping(struct CRGB* data, int nLedsOrOffset) : Sequence(data, nLedsOrOffset)
	{
		//index = 0, numbering according to arrays
		_hexagons[0] = Hexagon(0, 1, 255, 255, 255, 255, 255);
		_hexagons[1] = Hexagon(1, 2, 255, 255, 0, 255, 255);
		_hexagons[2] = Hexagon(2, 255, 4, 255, 1, 255, 3);
		_hexagons[3] = Hexagon(3, 255, 255, 2, 255, 255, 12);
		_hexagons[4] = Hexagon(4, 7, 6, 5, 255, 2, 255);
		_hexagons[5] = Hexagon(5, 6, 255, 255, 255, 255, 4);
		_hexagons[6] = Hexagon(6, 255, 255, 255, 5, 4, 7);
		_hexagons[7] = Hexagon(7, 255, 255, 6, 4, 255, 8);
		_hexagons[8] = Hexagon(8, 9, 255, 7, 255, 255, 10);
		_hexagons[9] = Hexagon(9, 17, 255, 255, 8, 10, 255);
		_hexagons[10] = Hexagon(10, 255, 9, 8, 255, 11, 255);
		_hexagons[11] = Hexagon(11, 255, 10, 255, 12, 255, 13);
		_hexagons[12] = Hexagon(12, 11, 255, 3, 255, 255, 255);
		_hexagons[13] = Hexagon(13, 14, 255, 11, 255, 255, 255);
		_hexagons[14] = Hexagon(14, 255, 15, 255, 13, 255, 24);
		_hexagons[15] = Hexagon(15, 20, 16, 255, 255, 14, 255);
		_hexagons[16] = Hexagon(16, 18, 255, 17, 255, 15, 20);
		_hexagons[17] = Hexagon(17, 255, 255, 255, 9, 255, 16);
		_hexagons[18] = Hexagon(18, 255, 255, 255, 16, 20, 19);
		_hexagons[19] = Hexagon(19, 255, 255, 18, 20, 255, 21);
		_hexagons[20] = Hexagon(20, 19, 18, 16, 15, 255, 255);
		_hexagons[21] = Hexagon(21, 255, 255, 19, 255, 22, 255);
		_hexagons[22] = Hexagon(22, 255, 21, 255, 23, 255, 255);
		_hexagons[23] = Hexagon(23, 22, 255, 255, 24, 255, 255);
		_hexagons[24] = Hexagon(24, 23, 255, 14, 255, 255, 255);		

		resetFilled();
	}

	void resetStartHex() { _currentLED = random8(21, 23); }

	void resetFilled() { for (uint8_t i = 0; i < 25; i++) { filled[i] = false; } }

	void fadeall() { for (int i = 0; i < LEDCount; i++) { LEDArray[i].nscale8(250); } }

	void fillup()
	{
		uint8_t next;
		bool found = false;

		//if (_finished)
		//{
		//	EVERY_N_MILLISECONDS(250)
		//	{
		//		fadeToBlackBy(LEDArray, LEDCount, 200);

		//		if (LEDArray[0] == CRGB(0, 0, 0))
		//		{
		//			resetStartHex();
		//			resetFilled();
		//			color = CRGB(random8(), random8(), random8());

		//			FastLED.delay(2000);

		//			_finished = false;
		//		}

		//		return;
		//	}
		//}

		if (++_mscounter > 5)
		{
			_mscounter = 0;

			//get next hex..
			//Serial.printf("current start led: %d\n", _currentLED);

			//_start = _currentLED * 30;
			_start = this->getIndex(_currentLED);
			_end = _start + 30;

			//fill_rainbow(LEDArray + _start, 30, random8(), (255 / 30));
			//fill_gradient(LEDArray, _start, CHSV(0, 255, 255), 30, CHSV(128, 255, 255), SHORTEST_HUES);

			if (_prevCurrentLED != 255)
			{
				fill_solid(LEDArray + (_prevCurrentLED * 30), 30, CRGB::Black);
			}

			fill_solid(LEDArray + _start, 30, color);

			uint8_t options[6];
			uint8_t count = 0;
			for (uint8_t i = 1; i < 6; i++)	//i is not 0; effect is less nice.
			{
				if (_hexagons[_currentLED].Sides[i] != 255)
				{
					if (filled[_hexagons[_currentLED].Sides[i]] == false)
					{
						options[count++] = _hexagons[_currentLED].Sides[i];
						//Serial.printf("led %d has been added.\n", _hexagons[_currentLED].Sides[i]);
					}
					else
					{
						//Serial.printf("led %d is filled already so not added.\n", _hexagons[_currentLED].Sides[i]);
					}
				}
			}

			if (count == 0)
			{
				_finished = true;
				//Serial.print("\nNO available options found! Next round!");

				resetStartHex();
				resetFilled();
				color = CRGB(random8(), random8(), random8());

				FastLED.delay(2000);
				FastLED.clear();

				return;
			}

			//Serial.print("\navailable options: ");
			//for (uint8_t i = 0; i < count; i++)
			//{
			//	Serial.printf("%d ", options[i]);
			//}

			_prevCurrentLED = _currentLED;

			_currentLED = options[random8(0, count)];
			filled[_currentLED] = true;

			//Serial.printf(" - next led: %d\n", _currentLED);
		}
	}

	void oneled()
	{

		if (++_mscounter > 10)
		{
			_mscounter = 0;

			FastLED.clear();
			//fadeall();

			//Serial.printf("Clearing LED %d to %d [currentLED: %d]...\n", _start, _end, _currentLED);
			//for (uint16_t i = _start; i < _end; i++)
			//{
			//	LEDArray[i].nscale8(250);
			//}

			//FastLED.delay(10);

			_start = _currentLED * 30;
			_end = _start + 30;

			Serial.printf("Doing LED %d to %d [currentLED: %d]...\n", _start, _end, _currentLED);

			for (uint16_t i = _start; i < _end; i++)
			{
				LEDArray[i] = CHSV(_hue, 255, 255);
			}

			if (++_currentLED == 25)
			{
				_currentLED = 0;
				_hue += 10;
			}
		}
	}

	void tick()
	{
		fillup();
	}

};


