#pragma once

#ifndef _MQTT_H
#define _MQTT_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

using namespace std;

//#define SHOW_PAYLOAD

#include "settings.hpp"
#include <AsyncMqttClient.h>
#include <vector>
#include "SequenceManager.hpp"
//#include <ArduinoJson.h>

struct MqttMessage {
	string topic;
	string data;
};

class MQTT
{
private:
	constexpr static const uint8_t MQTT_MSG_BUFFER_SIZE = 20;

	//get: [received] topic to request status
	//value: [published] topic with current status
	//set: [received] topic to set a new value

	constexpr static const PROGMEM char* MQTT_LIGHT_GETVALUE_TOPIC = "myhome/hexawall/light/get";
	constexpr static const PROGMEM char* MQTT_LIGHT_VALUE_TOPIC = "myhome/hexawall/light/value";
	constexpr static const PROGMEM char* MQTT_LIGHT_COMMAND_TOPIC = "myhome/hexawall/light/switch";
	constexpr static const PROGMEM char* MQTT_POWERSTATE_GETVALUE_TOPIC = "myhome/hexawall/powerstate/get";
	constexpr static const PROGMEM char* MQTT_POWERSTATE_VALUE_TOPIC = "myhome/hexawall/powerstate/value";

	constexpr static const PROGMEM char* MQTT_BRIGHTNESS_GETVALUE_TOPIC = "myhome/hexawall/brightness/get";
	constexpr static const PROGMEM char* MQTT_BRIGHTNESS_VALUE_TOPIC = "myhome/hexawall/brightness/value";
	constexpr static const PROGMEM char* MQTT_BRIGHTNESS_COMMAND_TOPIC = "myhome/hexawall/brightness/set";

	constexpr static const PROGMEM char* MQTT_RGB_GETVALUE_TOPIC = "myhome/hexawall/rgb/get";
	constexpr static const PROGMEM char* MQTT_RGB_VALUE_TOPIC = "myhome/hexawall/rgb/value";
	constexpr static const PROGMEM char* MQTT_RGB_COMMAND_TOPIC = "myhome/hexawall/rgb/set";

	constexpr static const PROGMEM char* MQTT_PROGRAM_GETVALUE_TOPIC = "myhome/hexawall/program/get";
	constexpr static const PROGMEM char* MQTT_PROGRAM_VALUE_TOPIC = "myhome/hexawall/program/value";
	constexpr static const PROGMEM char* MQTT_PROGRAM_COMMAND_TOPIC = "myhome/hexawall/program/set";
	constexpr static const PROGMEM char* MQTT_PROGRAM_CYCLE_COMMAND_TOPIC = "myhome/hexawall/program/cycle";
	constexpr static const PROGMEM char* MQTT_PROGRAM_DURATION_COMMAND_TOPIC = "myhome/hexawall/program/duration/set";
	constexpr static const PROGMEM char* MQTT_PROGRAM_DURATION_VALUE_TOPIC = "myhome/hexawall/program/duration/value";

	constexpr static const PROGMEM char* MQTT_PROGRAM_SPEED_GETVALUE_TOPIC = "myhome/hexawall/program/speed/get";
	constexpr static const PROGMEM char* MQTT_PROGRAM_SPEED_VALUE_TOPIC = "myhome/hexawall/program/speed/value";
	constexpr static const PROGMEM char* MQTT_PROGRAM_SPEED_COMMAND_TOPIC = "myhome/hexawall/program/speed/set";

	constexpr static const PROGMEM char* MQTT_PLAYLIST_GETVALUE_TOPIC = "myhome/hexawall/playlist/get";
	constexpr static const PROGMEM char* MQTT_PLAYLIST_VALUE_TOPIC = "myhome/hexawall/playlist/value";
	constexpr static const PROGMEM char* MQTT_PLAYLIST_COMMAND_TOPIC = "myhome/hexawall/playlist/set";
	constexpr static const PROGMEM char* MQTT_PLAYLIST_GETLIST_TOPIC = "myhome/hexawall/playlist/getlist";
	constexpr static const PROGMEM char* MQTT_PLAYLIST_LIST_TOPIC = "myhome/hexawall/playlist/list";
	constexpr static const PROGMEM char* MQTT_PLAYLIST_RUNTIME_GETVALUE_TOPIC = "myhome/hexawall/playlist/runtime/get";
	constexpr static const PROGMEM char* MQTT_PLAYLIST_RUNTIME_VALUE_TOPIC = "myhome/hexawall/playlist/runtime/value";

	constexpr static const PROGMEM char* MQTT_OS_REBOOT_COMMAND_TOPIC = "myhome/hexawall/os/reboot";

	constexpr static const PROGMEM char* MQTT_OS_PILOTLIGHT_GETVALUE_TOPIC = "myhome/hexawall/os/pilotlight/get";
	constexpr static const PROGMEM char* MQTT_OS_PILOTLIGHT_VALUE_TOPIC = "myhome/hexawall/os/pilotlight/value";
	constexpr static const PROGMEM char* MQTT_OS_PILOTLIGHT_COMMAND_TOPIC = "myhome/hexawall/os/pilotlight/set";

	constexpr static const PROGMEM char* MQTT_OS_BLINKLED_GETVALUE_TOPIC = "myhome/hexawall/os/led/get";
	constexpr static const PROGMEM char* MQTT_OS_BLINKLED_VALUE_TOPIC = "myhome/hexawall/os/led/value";
	constexpr static const PROGMEM char* MQTT_OS_BLINKLED_COMMAND_TOPIC = "myhome/hexawall/os/led/set";

	constexpr static const PROGMEM char* LIGHT_ON = "ON";
	constexpr static const PROGMEM char* LIGHT_OFF = "OFF";
	constexpr static const PROGMEM char* LIGHT_STANDBY = "STANDBY";

	SequenceManager* _sequenceManager;

	AsyncMqttClient _mqttClient;
	//TimerHandle_t _mqttReconnectTimer;
	//TimerHandle_t _mqttPublishTimer;

	bool _isInitialized = false;

	uint16_t subscribe(const char* topic, uint8_t qos = 0);

	//static std::vector<MqttMessage> _mqttPublishQueue;

	static void connectTimerCallback(TimerHandle_t xTimer);
	//static void publishTimerCallback(TimerHandle_t xTimer);

	void onMqttConnect(bool sessionPresent);
	void onMqttDisconnect(AsyncMqttClientDisconnectReason reason);
	void onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total);
	void onMqttPublish(uint16_t packetId);
	void decodeMQTTHomeAssistantCommand(char* p_topic, char* p_payload, unsigned int p_length);

	std::function<void(CommandPowerState)> _onPowerStateChangeCallback;
	std::function<void(void)> _onRebootRequest;

public:
	void init(SequenceManager* seqmanager);
	void connect(void);
	//void scheduleUpdateStatus(void);
	void stopReconnectTimer(void);
	void disconnect(void);

	void publishDeviceState(void);
	void publishLEDBrightness(void);
	void publishLEDRGBColor(void);
	void publishLEDProgram(void);
	void publishLEDProgramDuration(void);
	//void publishLEDProgramCycle(void);
	void publishActivePlaylist(void);
	void publishAvailablePlaylists(void);
	void publishOSBlinkLEDState(void);
	void publishLEDProgramDelay(void);
	void publishPilotLight(void);
	void publish(const char* topic, int value);

	void onPowerStateChange(std::function<void(CommandPowerState)> fn);
	void onRebootRequest(std::function<void(void)> fn);

	bool isInitialized(void);
	bool isConnected(void);
};

#endif