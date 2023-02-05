#pragma once

#ifndef _SETTINGS_H
#define _SETTINGS_H

//#define DEVELOP_MODE
#define ENABLE_SERIAL_PRINT_INFO
//#define ENABLE_SERIAL_PRINT_DEBUG

const char* const ssid = "OasisOfChaos_";
const char* const password = "pyY5v6kCHYAs3FEJ";

#define MQTT_HOST IPAddress(192, 168, 0, 1)
#define MQTT_PORT 1883

const char* const mqtt_user = "admin";
const char* const mqtt_password = "MqTT5634!";

#define LED_BUILTIN			2
#define PIN_BEEPER			19
#define PIN_IR_RECEIVER		22
#define PIN_OTA_BUTTON		22
#define PIN_PS_ON		    33
#define PIN_LED_PS_ON		21
#define PIN_PUSHBUTTON1		23
#define PIN_POWER_RELAY		26

//#define LED_PIN             14
//#define LED_TYPE            WS2812B
//#define COLOR_ORDER         GRB
//#define LED_COUNT           750

#ifdef ENABLE_SERIAL_PRINT_INFO
#define PRINT_INFO_0() (Serial.printf())
#define PRINT_INFO_1(a) (Serial.printf(a))
#define PRINT_INFO_2(a, b) (Serial.printf(a, b))
#define PRINT_INFO_3(a, b, c) (Serial.printf(a, b, c))
#define PRINT_INFO_4(a, b, c, d) (Serial.printf(a, b, c, d))
#else
#define PRINT_INFO_0()
#define PRINT_INFO_1(a)
#define PRINT_INFO_2(a, b)
#define PRINT_INFO_3(a, b, c)
#define PRINT_INFO_4(a, b, c, d)
#endif

#ifdef ENABLE_SERIAL_PRINT_DEBUG
#define PRINT_DEBUG_0() (Serial.printf())
#define PRINT_DEBUG_1(a) (Serial.printf(a))
#define PRINT_DEBUG_2(a, b) (Serial.printf(a, b))
#define PRINT_DEBUG_3(a, b, c) (Serial.printf(a, b, c))
#define PRINT_DEBUG_4(a, b, c, d) (Serial.printf(a, b, c, d))

#else
#define PRINT_DEBUG_0()
#define PRINT_DEBUG_1(a)
#define PRINT_DEBUG_2(a, b)
#define PRINT_DEBUG_3(a, b, c)
#define PRINT_DEBUG_4(a, b, c, d)
#endif

// The interim macro that simply strips the excess and ends up with the required macro
#define PRINT_INFO_X(x,a,b,c,d,FUNC, ...)  FUNC
#define PRINT_DEBUG_X(x,a,b,c,d,FUNC, ...)  FUNC

// The macro that the programmer uses
#define PRINT_INFO(...)                   PRINT_INFO_X(,##__VA_ARGS__,\
                                          PRINT_INFO_4(__VA_ARGS__),\
                                          PRINT_INFO_3(__VA_ARGS__),\
                                          PRINT_INFO_2(__VA_ARGS__),\
                                          PRINT_INFO_1(__VA_ARGS__),\
                                          PRINT_INFO_0(__VA_ARGS__)\
                                          )

#define PRINT_DEBUG(...)                  PRINT_DEBUG_X(,##__VA_ARGS__,\
                                          PRINT_DEBUG_4(__VA_ARGS__),\
                                          PRINT_DEBUG_3(__VA_ARGS__),\
                                          PRINT_DEBUG_2(__VA_ARGS__),\
                                          PRINT_DEBUG_1(__VA_ARGS__),\
                                          PRINT_DEBUG_0(__VA_ARGS__)\
                                          )
#endif