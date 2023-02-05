// SeqSwitchOff.h

#pragma once

#include "arduino.h"
#include <FastLED.h>
#include "Sequence.hpp"

class SeqSwitchOff : public Sequence
{
private:

public:
	SeqSwitchOff(struct CRGB* data, int nLedsOrOffset) : Sequence(data, nLedsOrOffset) {}

	void tick()
	{}

};