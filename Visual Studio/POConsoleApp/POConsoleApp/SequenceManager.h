#pragma once

#include "fled.h"
#include "Sequence.h"

enum class SequenceName
{
	Boot,
	Cylon,
	SolidColor
};

class SequenceManager
{
private:
	Sequence* CurrentSequence = NULL;
	SequenceName CurrentSequenceName = SequenceName::Boot;
	bool SequenceChanging = false;

	uint16_t ledBrightness = 0;
	uint8_t ledTargetBrightness = 255;
	uint8_t ledLastSetBrightness = 255;

protected:
	static const int LEDCount = 750;
	CRGB LEDArray[LEDCount];

public:
	SequenceManager();
	~SequenceManager();

	void tick();
	void setSequence(SequenceName sequence);
};