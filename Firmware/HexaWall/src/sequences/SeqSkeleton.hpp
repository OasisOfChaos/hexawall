// SeqSkeleton.h

#pragma once

#include "arduino.h"
#include <FastLED.h>
#include "Sequence.hpp"

class SeqSkeleton : public Sequence
{
private:

public:
	SeqSkeleton(struct CRGB* data, int nLedsOrOffset) : Sequence(data, nLedsOrOffset) {}

	void tick()
	{}

};

