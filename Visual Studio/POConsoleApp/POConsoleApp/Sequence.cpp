#include "Sequence.h"
#include <iostream>
#include <array>

using namespace std;

Sequence::Sequence(CRGB* data, int nLedsOrOffset)
{
	LEDCount = nLedsOrOffset;
	leds = data;

	cout << "Sequence constructor" << endl;
}

Sequence::~Sequence()
{
	cout << "Sequence destructor" << endl;
}

void Sequence::test(CRGB* data, int count)
{
	LEDCount = count;
	leds = data;

	//
	std::cout << "data[0].r: " << (int)data[0].r << "\n";
	std::cout << "data[0].g: " << (int)data[0].g << "\n";
	std::cout << "data[0].b: " << (int)data[0].b << "\n";

	data[1].green = 110;

	std::cout << "data[1].r: " << (int)data[1].r << "\n";
	std::cout << "data[1].g: " << (int)data[1].g << "\n";
	std::cout << "data[1].b: " << (int)data[1].b << "\n";

	std::cout << "data[2].r: " << (int)data[2].r << "\n";
	std::cout << "data[2].g: " << (int)data[2].g << "\n";
	std::cout << "data[2].b: " << (int)data[2].b << "\n";
}

//void Sequence::tick()
//{
//	std::cout << "Sequence::tick()\n";
//
//	leds[1].green = 120;
//
//	std::cout << "leds[2].r: " << (int)leds[2].r << "\n";
//	std::cout << "leds[2].g: " << (int)leds[2].g << "\n";
//	std::cout << "leds[2].b: " << (int)leds[2].b << "\n";
//}


//SequenceOne::SequenceOne(CRGB* data, int nLedsOrOffset)
//{
//
//
//	cout << "SequenceOne constructor" << endl;
//}


SequenceOne::~SequenceOne()
{
	cout << "SequenceOne destructor" << endl;
}

void SequenceOne::tock()
{
	cout << "SequenceOne tock()" << endl;
}

void SequenceOne::tick()
{
	std::cout << "SequenceOne::tick()\n";

	leds[1].green = 120;

	std::cout << "leds[2].r: " << (int)leds[2].r << "\n";
	std::cout << "leds[2].g: " << (int)leds[2].g << "\n";
	std::cout << "leds[2].b: " << (int)leds[2].b << "\n";
}

SequenceTwo::SequenceTwo(CRGB* data, int nLedsOrOffset) : Sequence(data, nLedsOrOffset)
{
	cout << "SequenceTwo constructor" << endl;
}


//SequenceTwo::~SequenceTwo()
//{
//	cout << "SequenceTwo destructor" << endl;
//}

void SequenceTwo::tock()
{
	cout << "SequenceTwo tock()" << endl;
}

void SequenceTwo::tick()
{
	std::cout << "SequenceTwo::tick()\n";

	leds[1].green = 120;

	std::cout << "leds[2].r: " << (int)leds[2].r << "\n";
	std::cout << "leds[2].g: " << (int)leds[2].g << "\n";
	std::cout << "leds[2].b: " << (int)leds[2].b << "\n";
}