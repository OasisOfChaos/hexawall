#pragma once
#include <cstdint>

#define LED_PIN     4
#define NUM_LEDS    10
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB

/// Representation of an HSV pixel (hue, saturation, value (aka brightness)).
struct CRGB {
    union {
        struct {
            union {
                uint8_t r;
                uint8_t red;
            };
            union {
                uint8_t g;
                uint8_t green;
            };
            union {
                uint8_t b;
                uint8_t blue;
            };
        };
        uint8_t raw[3];
    };
};