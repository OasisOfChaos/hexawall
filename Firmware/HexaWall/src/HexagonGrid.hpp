#pragma once

#include "Arduino.h"
#include "Hexagon.hpp"

class HexagonGrid
{
private:
	const uint8_t Columns[11][3] = {
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

	Hexagon _hexagons[24];

public:
	HexagonGrid() {
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

		//index = 1, numbering according to drawing
		//_hexagons[0] = Hexagon(1, 2, 0, 0, 0, 0, 0);
		//_hexagons[1] = Hexagon(2, 3, 0, 0, 0, 0, 0);
		//_hexagons[2] = Hexagon(3, 0, 5, 0, 0, 0, 4);
		//_hexagons[3] = Hexagon(4, 0, 0, 3, 0, 0, 13);
		//_hexagons[4] = Hexagon(5, 8, 7, 0, 0, 3, 0);
		//_hexagons[5] = Hexagon(6, 7, 0, 0, 0, 0, 5);
		//_hexagons[6] = Hexagon(7, 0, 0, 0, 6, 5, 8);
		//_hexagons[7] = Hexagon(8, 0, 0, 7, 5, 0, 9);
		//_hexagons[8] = Hexagon(9, 10, 0, 8, 0, 0, 11);
		//_hexagons[9] = Hexagon(10, 18, 0, 0, 9, 11, 0);
		//_hexagons[10] = Hexagon(11, 0, 10, 9, 0, 12, 0);
		//_hexagons[11] = Hexagon(12, 0, 11, 0, 13, 0, 14);
		//_hexagons[12] = Hexagon(13, 12, 0, 4, 0, 0, 0);
		//_hexagons[13] = Hexagon(14, 15, 0, 12, 0, 0, 0);
		//_hexagons[14] = Hexagon(15, 0, 16, 0, 14, 0, 25);
		//_hexagons[15] = Hexagon(16, 21, 17, 0, 0, 15, 0);
		//_hexagons[16] = Hexagon(17, 19, 0, 0, 0, 16, 21);
		//_hexagons[17] = Hexagon(18, 0, 0, 0, 10, 0, 17);
		//_hexagons[18] = Hexagon(19, 0, 0, 0, 17, 21, 20);
		//_hexagons[19] = Hexagon(20, 0, 0, 19, 21, 0, 22);
		//_hexagons[20] = Hexagon(21, 20, 19, 17, 16, 0, 0);
		//_hexagons[21] = Hexagon(22, 0, 0, 20, 0, 23, 0);
		//_hexagons[22] = Hexagon(23, 0, 22, 0, 24, 0, 0);
		//_hexagons[23] = Hexagon(24, 23, 0, 0, 25, 0, 0);
		//_hexagons[24] = Hexagon(25, 24, 0, 25, 0, 0, 0);
	}
};