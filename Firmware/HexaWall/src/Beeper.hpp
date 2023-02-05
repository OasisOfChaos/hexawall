// Beeper.h

#ifndef _BEEPER_h
#define _BEEPER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class Beeper
{
private:
	uint8_t BeeperPin;

protected:

public:
	Beeper(uint8_t pin);
	void beep(int duration = 200, int delayAfter = 0);
};

#endif

