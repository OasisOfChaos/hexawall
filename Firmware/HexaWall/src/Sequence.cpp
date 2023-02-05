#include "Sequence.hpp"

Sequence::Sequence(struct CRGB *data, int nLedsOrOffset, int nLedBrightness) : Sequence(data, nLedsOrOffset)
{
	// LEDArray = data;
	// LEDCount = nLedsOrOffset;
	LEDBrightness = nLedBrightness;

	PRINT_DEBUG("ctor Sequence LEDBrightness set to %d.\n", LEDBrightness);
}

// returns the start led index of the led array based on a hexagon number
int Sequence::getIndex(int number)
{
	int arrayIndex = (number * 30) - 1;
	if (arrayIndex < 0)
	{
		arrayIndex = 0;
	}

	return arrayIndex;
}

Sequence::Sequence(struct CRGB *data, int nLedsOrOffset)
{
	LEDArray = data;
	LEDCount = nLedsOrOffset;
}

void Sequence::secondCounter()
{
	SecondIndicator++;
	secondTick();

	////rudimentary global second indicator
	// SecondHasPassed = true;
	// if (!SecondHasPassed)
	//{
	//	uint8_t secondHand = (millis() / 1000) % 60;
	//	if (SecondIndicator != secondHand)
	//	{
	//		SecondIndicator = secondHand;
	//		SecondHasPassed = true;
	//	}
	// }
}

void Sequence::setLEDBrightness(int ledBrightness)
{
	LEDBrightness = ledBrightness;

	PRINT_DEBUG("Sequence LEDBrightness set to %d.\r\n", LEDBrightness);
}

bool Sequence::isFinished()
{
	return _isFinished;
}

// void Sequence::sequenceIsChanging()
//{
//	_sequenceChanging = true;
// }

void Sequence::setSolidColor(CRGB color)
{
	CurrentLEDSolidColor = color;
	Processed = false;
}


// =================================================
// Modified helper function that blends one uint8_t toward another,
// based on function from Mark Kriegsman's fadeTowardColor example:
// https://gist.github.com/kriegsman/d0a5ed3c8f38c64adcb4837dafb6e690
void Sequence::nblendU8TowardU8(uint8_t &current, const uint8_t target, fract8 scale)
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