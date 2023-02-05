// SeqConfetti.h

#pragma once

#include "arduino.h"
#include <FastLED.h>
#include "Sequence.hpp"

class SeqConfetti : public Sequence
{
private:
	unsigned long previousMillis;                                 // Store last time the strip was updated.
	int _secondCounter = 0;

	uint8_t  thisfade = 8;                                        // How quickly does it fade? Lower = slower fade rate.
	int       thishue = 50;                                       // Starting hue.
	uint8_t   thisinc = 1;                                        // Incremental value for rotating hues
	uint8_t   thissat = 100;                                      // The saturation, where 255 = brilliant colours.
	uint8_t   thisbri = 255;                                      // Brightness of a sequence. Remember, max_bright is the overall limiter.
	int       huediff = 256;                                      // Range of random #'s to use for hue
	uint8_t thisdelay = 5;                                        // We don't need much delay (if any)

	void secondTick()
	{
		switch (_secondCounter)
		{
		case  0: thisinc = 1; thishue = 192; thissat = 255; thisfade = 2; huediff = 256; break;  // You can change values here, one at a time , or altogether.
		case  5: thisinc = 2; thishue = 128; thisfade = 8; huediff = 64; break;
		case 10: thisinc = 1; thishue = random16(255); thisfade = 1; huediff = 16; break;      // Only gets called once, and not continuously for the next several seconds. Therefore, no rainbows.
		case 15: break;                                                                // Here's the matching 15 for the other one.
		}


		if (++_secondCounter > 15)
		{
			_secondCounter = 0;
		}
	}

public:
	SeqConfetti(struct CRGB* data, int nLedsOrOffset) : Sequence(data, nLedsOrOffset) {}

	//String Name() { return "Confetti"; }

	void tick()
	{
		// random colored speckles that blink in and fade smoothly
		fadeToBlackBy(LEDArray, LEDCount, thisfade);                    // Low values = slower fade.
		int pos = random16(LEDCount);                               // Pick an LED at random.
		LEDArray[pos] += CHSV((thishue + random16(huediff)) / 4, thissat, thisbri);  // I use 12 bits for hue so that the hue increment isn't too quick.
		thishue = thishue + thisinc;                                // It increments here.
	}

};