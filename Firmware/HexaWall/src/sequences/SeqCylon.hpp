// SeqCylon.h

#ifndef _SEQCYLON_h
#define _SEQCYLON_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <FastLED.h>
#include "Sequence.hpp"

class SeqCylon : public Sequence
{
public:
	SeqCylon(struct CRGB* data, int nLedsOrOffset) : Sequence(data, nLedsOrOffset) {}

	void fadeall() { for (int i = 0; i < LEDCount; i++) { LEDArray[i].nscale8(250); } }

	void tick() {
		static uint8_t hue = 0;

		//Serial.print("x");

		// First slide the led in one direction
		for (int i = 0; i < LEDCount; i++) {
			// Set the i'th led to red
			LEDArray[i] = CHSV(hue++, 255, 255);

			// Show the leds
			FastLED.show();

			// now that we've shown the leds, reset the i'th led to black
			// leds[i] = CRGB::Black;
			fadeall();

			// Wait a little bit before we loop around and do it again
			delay(10);
		}

		//Serial.print("x");

		// Now go in the other direction.
		for (int i = (LEDCount)-1; i >= 0; i--) {
			// Set the i'th led to red
			LEDArray[i] = CHSV(hue++, 255, 255);

			// Show the leds
			FastLED.show();

			// now that we've shown the leds, reset the i'th led to black
			// leds[i] = CRGB::Black;
			fadeall();

			// Wait a little bit before we loop around and do it again
			delay(10);
		}
	}

};

#endif

