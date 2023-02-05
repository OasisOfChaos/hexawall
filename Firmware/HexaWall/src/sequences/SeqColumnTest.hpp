// SeqColumnTest.h
#pragma once

#include "arduino.h"
#include <FastLED.h>
#include "Sequence.hpp"

#define NUM_COLUMNS 11
#define NUM_ROWS 3

#define NUM_VERT_COLUMNS 10
#define NUM_VERT_ROWS 4

#define NUM_HORI_COLUMNS 4
#define NUM_HORI_ROWS 7

uint8_t _colorIndex = 0;

class SeqColumnTest : public Sequence
{
private:
	uint8_t _sequence[NUM_COLUMNS][NUM_ROWS] = {
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
		{0, 255, 255}};

	uint8_t hexa_columns_vertical[NUM_VERT_COLUMNS][NUM_VERT_ROWS] = {
		{6, 255, 255, 255},
		{5, 8, 7, 255},	  //{5, 8, 255, 255},
		{4, 10, 25, 9},	  //{4, 10, 25, 255},
		{3, 24, 26, 255}, //{3, 24, 26, 11},
		{2, 11, 23, 27},
		{1, 12, 22, 28},
		{13, 21, 29, 255}, //{13, 21, 29, 1},
		{14, 20, 30, 19},  //{14, 20, 30, 255},
		{15, 18, 17, 255},
		{16, 255, 255, 255}};

	uint8_t hexa_columns_horizontal[4][7] = {
		{1, 2, 3, 4, 5, 6, 20},
		{7, 22, 21, 23, 24, 25, 19},
		{9, 26, 27, 28, 29, 30, 17},
		{10, 11, 12, 13, 14, 15, 16},
	};

	uint8_t _second = 0;
	uint8_t _prevsecond = -1;

	uint8_t _colorIndex = 0;

	int8_t _column = -1;
	bool _direction = true;

	CRGB _color;

	CRGBPalette16 _currentPalette;
	TBlendType _currentBlending;

	void secondTick()
	{
		_second++;
	}

public:
	SeqColumnTest(struct CRGB *data, int nLedsOrOffset) : Sequence(data, nLedsOrOffset)
	{
		_isFinished = false;

		_currentPalette = RainbowColors_p;
		//_currentPalette = ForestColors_p;
		_currentBlending = LINEARBLEND;

		Serial.printf("Starting SeqColumnTest...\n");
	}

	void tick()
	{
		EVERY_N_MILLISECONDS(100)
		{
			/*if (_prevsecond == _second)
			{
				return;
			}

			_prevsecond = _second;*/

			if (_isFinished)
			{
				return;
			}

			//_colorIndex = _colorIndex + 1; /* motion speed */
			// FillLEDsFromPaletteColors(_colorIndex);

			// Serial.printf("index: %d, index+30: %d, _sequenceIndex: %d\r\n", (_sequence[_sequenceIndex][0] * 30), (_sequence[_sequenceIndex][0] * 30)+30, _sequenceIndex);

			// doVertical();
			// doHorizontal();

			// vertPaletteLeftToRight();
			vertPaletteSweep();

			// vertPaletteTimedSweep();

			FastLED.show();
		}
	}

	void FillLEDsFromPaletteColors(uint8_t colorIndex)
	{
		for (int i = 0; i < LEDCount; i++)
		{
			LEDArray[i] = ColorFromPalette(_currentPalette, colorIndex, LEDBrightness, _currentBlending);
			colorIndex += 3;
		}
	}

	//_isFinished = true;

	void doHorizontal()
	{
		if (_second >= NUM_HORI_COLUMNS)
		{
			_second = 0;
		}

		fill_solid(LEDArray, LEDCount, CRGB::Black);

		uint8_t hc = _second;
		{
			if (hc == _second)
			{
				_color = CRGB::Red;
			}
			else
			{
				_color = CRGB::Black;
			}

			for (uint8_t c = 0; c < NUM_COLUMNS; c++)
			{
				for (uint8_t r = 0; r < NUM_ROWS; r++)
				{
					if (_sequence[c][r] != 255)
					{
						int idx = this->getIndex(_sequence[c][r]);

						for (uint8_t hr = 0; hr < NUM_HORI_ROWS; hr++)
						{
							int cvert = hexa_columns_horizontal[hc][hr];
							if (cvert != 255)
							{
								LEDArray[idx + cvert] = _color;
								// Serial.printf("c: %d, r: %d, hc: %d, hr: %d, _sequence: %d, hexa_columns_vertical: %d\n", c, r, hc, hr, _sequence[c][r], cvert);
							}
						}
					}
				}
			}
		}
	}

	void vertPaletteTimedSweep()
	{
		if (_direction)
		{
			if (++_column >= NUM_COLUMNS)
			{
				_column = 0;
			};
		}
		else
		{
			if (--_column < 0)
			{
				_column = NUM_COLUMNS - 1;
			}
		}

		if (_second > 10)
		{
			_second = 0;
			_direction = !_direction;
		}

		uint8_t c = _column;
		{
			for (uint8_t r = 0; r < NUM_ROWS; r++)
			{
				if (_sequence[c][r] != 255)
				{
					int idx = this->getIndex(_sequence[c][r]);

					_color = ColorFromPalette(_currentPalette, _colorIndex, LEDBrightness, _currentBlending);

					for (uint8_t hc = 0; hc < NUM_VERT_COLUMNS; hc++)
					{
						for (uint8_t hr = 0; hr < NUM_VERT_ROWS; hr++)
						{
							int cvert = hexa_columns_vertical[hc][hr];
							if (cvert != 255)
							{
								LEDArray[idx + cvert] = _color;
								// Serial.printf("c: %d, r: %d, hc: %d, hr: %d, _sequence: %d, hexa_columns_vertical: %d\n", c, r, hc, hr, _sequence[c][r], cvert);
							}
						}

						_colorIndex += 1;
					}
				}
			}
		}
	}

	void vertPaletteSweep()
	{
		if (_direction)
		{
			if (++_column >= NUM_COLUMNS)
			{
				_direction = !_direction;
			};
		}
		else
		{
			if (--_column == 0)
			{
				_direction = !_direction;
			}
		}

		uint8_t c = _column;
		{
			for (uint8_t r = 0; r < NUM_ROWS; r++)
			{
				if (_sequence[c][r] != 255)
				{
					int idx = this->getIndex(_sequence[c][r]);

					_color = ColorFromPalette(_currentPalette, _colorIndex, LEDBrightness, _currentBlending);

					for (uint8_t hc = 0; hc < NUM_VERT_COLUMNS; hc++)
					{
						for (uint8_t hr = 0; hr < NUM_VERT_ROWS; hr++)
						{
							int cvert = hexa_columns_vertical[hc][hr];
							if (cvert != 255)
							{
								LEDArray[idx + cvert] = _color;
								// Serial.printf("c: %d, r: %d, hc: %d, hr: %d, _sequence: %d, hexa_columns_vertical: %d\n", c, r, hc, hr, _sequence[c][r], cvert);
							}
						}

						//_colorIndex += 1;
					}

					_colorIndex += 1;
				}
			}
		}
	}

	void vertPaletteLeftToRight()
	{

		for (uint8_t c = 0; c < NUM_COLUMNS; c++)
		{
			for (uint8_t r = 0; r < NUM_ROWS; r++)
			{
				if (_sequence[c][r] != 255)
				{
					int idx = this->getIndex(_sequence[c][r]);

					_color = ColorFromPalette(_currentPalette, _colorIndex, LEDBrightness, _currentBlending);

					for (uint8_t hc = 0; hc < NUM_VERT_COLUMNS; hc++)
					{
						for (uint8_t hr = 0; hr < NUM_VERT_ROWS; hr++)
						{
							int cvert = hexa_columns_vertical[hc][hr];
							if (cvert != 255)
							{
								LEDArray[idx + cvert] = _color;
								// Serial.printf("c: %d, r: %d, hc: %d, hr: %d, _sequence: %d, hexa_columns_vertical: %d\n", c, r, hc, hr, _sequence[c][r], cvert);
							}
						}

						_colorIndex += 1;
					}
				}
			}
		}
	}

	void doVertical()
	{
		fill_solid(LEDArray, LEDCount, CRGB::Black);

		if (_second >= NUM_VERT_COLUMNS)
		{
			_second = 0;
		}

		// for (uint8_t hc = 0; hc < 10; hc++)
		uint8_t hc = _second;
		{
			if (hc == _second)
			{
				_color = CRGB::Red;
			}
			else
			{
				_color = CRGB::Black;
			}

			for (uint8_t c = 0; c < NUM_COLUMNS; c++)
			{
				for (uint8_t r = 0; r < NUM_ROWS; r++)
				{
					if (_sequence[c][r] != 255)
					{
						int idx = this->getIndex(_sequence[c][r]);
						// fill_solid(LEDArray + idx, 30, CRGB::Black);
						// LEDArray[idx + 6] = CRGB::Red;

						for (uint8_t hr = 0; hr < NUM_VERT_ROWS; hr++)
						{
							int cvert = hexa_columns_vertical[hc][hr];
							if (cvert != 255)
							{
								LEDArray[idx + cvert] = _color;
								// Serial.printf("c: %d, r: %d, hc: %d, hr: %d, _sequence: %d, hexa_columns_vertical: %d\n", c, r, hc, hr, _sequence[c][r], cvert);
							}
						}
					}
				}
			}
		}
	}
};