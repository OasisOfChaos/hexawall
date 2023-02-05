//
//
//

#include "Beeper.hpp"

Beeper::Beeper(uint8_t pin)
{
	BeeperPin = pin;

	pinMode(BeeperPin, OUTPUT);
}

/// <summary>
/// Beep! Beep! Blocking function! Uses crude delay.
/// </summary>
/// <param name="duration"></param>
/// <param name="delayAfter"></param>
void Beeper::beep(int duration , int delayAfter)
{
	digitalWrite(BeeperPin, 1);
	delay(duration);
	digitalWrite(BeeperPin, 0);

	if (delayAfter > 0) {
		delay(delayAfter);
	}
}

