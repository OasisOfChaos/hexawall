// Sequence.h

#pragma once

#ifndef _SEQUENCE_h
#define _SEQUENCE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <FastLED.h>
#include "settings.hpp"

#define LED_COUNT    750

class Sequence
{
private:
	//const String _sequenceName = "UNKNOWN";
	//bool _sequenceChanging = false;
	virtual void secondTick() = 0;	//pure virtual 'abstract' method

 protected:
	 int LEDCount;
	 CRGB* LEDArray;
	 int LEDBrightness = 0;

	 uint16_t SecondIndicator = 0;
	 //bool SecondHasPassed;

	 CRGB CurrentLEDSolidColor = CRGB::Red;
	 bool Processed = false;
	 bool _isFinished = false;

	 int getIndex(int number);

	 void nblendU8TowardU8(uint8_t &current, const uint8_t target, fract8 scale = 1);

 public:
	 Sequence(struct CRGB* data, int nLedsOrOffset, int nLedBrightness);
	 Sequence(struct CRGB* data, int nLedsOrOffset);

	 virtual void tick() = 0;	//pure virtual 'abstract' method

	 //virtual String Name() = 0;	//pure virtual 'abstract' method

	 void setSolidColor(CRGB color);
	 void secondCounter();
	 void setLEDBrightness(int ledBrightness);

	 bool isFinished();

	 //void sequenceIsChanging();
};

#endif
