#pragma once

//one hexagon has 30 leds
//define paths? or network?

struct Hexagon
{
	/*
	 6 / \ 1
	5 |   | 2
	 4 \ / 3
	*/
	/// default values are UNITIALIZED
	inline Hexagon() __attribute__((always_inline)) = default;

	inline Hexagon(uint8_t number, uint8_t side1, uint8_t side2, uint8_t side3, uint8_t side4, uint8_t side5, uint8_t side6)
	{
		Number = number;
		Sides[0] = side1;
		Sides[1] = side2;
		Sides[2] = side3;
		Sides[3] = side4;
		Sides[4] = side5;
		Sides[5] = side6;
	}

	uint8_t Number;
	uint8_t Sides[6];
};
