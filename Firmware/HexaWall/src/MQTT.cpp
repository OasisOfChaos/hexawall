#include "MQTT.hpp"

uint16_t MQTT::subscribe(const char *topic, uint8_t qos)
{
	uint16_t packetIdSub = MQTT::_mqttClient.subscribe(topic, 0);
	PRINT_DEBUG("Subscribing to '%s' at QoS 0, packetId: %d\n", topic, packetIdSub);

	return packetIdSub;
}

void MQTT::connectTimerCallback(TimerHandle_t xTimer)
{
	MQTT *p = static_cast<MQTT *>(pvTimerGetTimerID(xTimer));
	p->connect();
}

// void MQTT::publishTimerCallback(TimerHandle_t xTimer)
//{
//	if (_mqttPublishQueue.size() == 0) {
//		return;
//	}
//
//	_mqttPublishQueue.pop_back()
//
//	PRINT_INFO("Publishing MQTT message '%s' at QoS 0, packetId: %d\n", MQTT_LIGHT_BRIGHTNESS_VALUE_TOPIC, packetIdSub);
// }

void MQTT::onMqttConnect(bool sessionPresent)
{
	uint16_t packetIdSub = 0;

	stopReconnectTimer();

	PRINT_INFO("Connected to MQTT. Session present: %d.\n", sessionPresent);

	subscribe(MQTT_LIGHT_GETVALUE_TOPIC, 0);
	subscribe(MQTT_LIGHT_COMMAND_TOPIC, 0);

	subscribe(MQTT_BRIGHTNESS_GETVALUE_TOPIC, 0);
	subscribe(MQTT_BRIGHTNESS_COMMAND_TOPIC, 0);

	subscribe(MQTT_RGB_GETVALUE_TOPIC, 0);
	subscribe(MQTT_RGB_COMMAND_TOPIC, 0);

	subscribe(MQTT_PROGRAM_GETVALUE_TOPIC, 0);
	subscribe(MQTT_PROGRAM_COMMAND_TOPIC, 0);
	subscribe(MQTT_PROGRAM_CYCLE_COMMAND_TOPIC, 0);
	subscribe(MQTT_PROGRAM_DURATION_COMMAND_TOPIC, 0);
	subscribe(MQTT_PROGRAM_SPEED_GETVALUE_TOPIC);
	subscribe(MQTT_PROGRAM_SPEED_COMMAND_TOPIC);

	subscribe(MQTT_PROGRAM_GETVALUE_TOPIC, 0);
	subscribe(MQTT_PROGRAM_SPEED_COMMAND_TOPIC, 0);

	subscribe(MQTT_PLAYLIST_GETVALUE_TOPIC, 0);
	subscribe(MQTT_PLAYLIST_COMMAND_TOPIC, 0);
	subscribe(MQTT_PLAYLIST_GETLIST_TOPIC, 0);
	subscribe(MQTT_PLAYLIST_RUNTIME_GETVALUE_TOPIC, 0);

	subscribe(MQTT_OS_REBOOT_COMMAND_TOPIC, 0);
	subscribe(MQTT_OS_BLINKLED_GETVALUE_TOPIC, 0);
	subscribe(MQTT_OS_BLINKLED_COMMAND_TOPIC, 0);

	subscribe(MQTT_OS_PILOTLIGHT_GETVALUE_TOPIC, 0);
}

void MQTT::onMqttDisconnect(AsyncMqttClientDisconnectReason reason)
{
	PRINT_INFO("Disconnected from MQTT. ");

	/*xTimerStart(_mqttReconnectTimer, 0);
	PRINT_INFO("MQTT Reconnect timer started.\n");*/

	connect();
}

void MQTT::onMqttMessage(char *topic, char *payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total)
{
#ifdef SHOW_PAYLOAD
	PRINT_INFO("MQTT message received: %s: ", topic);

	for (int i = 0; i < len; i++)
	{
		Serial.print((char)payload[i]);
		// str[i] = (char)payload[i];
	}

	////str[i] = 0; // Null termination
	Serial.println("//");
#endif

	// process HomeAssistant stuff
	decodeMQTTHomeAssistantCommand(topic, payload, len);
}

void MQTT::onMqttPublish(uint16_t packetId)
{
	PRINT_INFO("Publish acknowledged. packetId: %d", packetId);
}

void MQTT::decodeMQTTHomeAssistantCommand(char *p_topic, char *p_payload, unsigned int p_length)
{
	String payload;
	for (uint8_t i = 0; i < p_length; i++)
	{
		payload.concat((char)p_payload[i]);
	}

	if (String(MQTT_LIGHT_COMMAND_TOPIC).equals(p_topic))
	{
		if (payload.equals(String(LIGHT_ON)))
		{
			_onPowerStateChangeCallback(CommandPowerState::PowerOn);
		}
		else if (payload.equals(String(LIGHT_OFF)))
		{
			_onPowerStateChangeCallback(CommandPowerState::PowerOff);
		}
		else if (payload.equals(String(LIGHT_STANDBY)))
		{
			_onPowerStateChangeCallback(CommandPowerState::Standby);
		}
	}
	else if (String(MQTT_LIGHT_GETVALUE_TOPIC).equals(p_topic))
	{
		publishDeviceState();
	}
	else if (String(MQTT_BRIGHTNESS_COMMAND_TOPIC).equals(p_topic))
	{
		uint8_t brightness = payload.toInt();

		if (_sequenceManager->getLEDBrightness() != brightness)
		{
			_sequenceManager->setLEDBrightness(brightness, true);
		}
	}
	else if (String(MQTT_BRIGHTNESS_GETVALUE_TOPIC).equals(p_topic))
	{
		publishLEDBrightness();
	}
	else if (String(MQTT_RGB_COMMAND_TOPIC).equals(p_topic))
	{
		// get the position of the first and second commas
		uint8_t firstIndex = payload.indexOf(',');
		uint8_t lastIndex = payload.lastIndexOf(',');

		uint8_t rgb_red = payload.substring(0, firstIndex).toInt();
		uint8_t rgb_green = payload.substring(firstIndex + 1, lastIndex).toInt();
		uint8_t rgb_blue = payload.substring(lastIndex + 1).toInt();

		CRGB value = _sequenceManager->getLEDSolidColor();

		if (value.r != rgb_red || value.g != rgb_green || value.b != rgb_blue)
		{
			_sequenceManager->setLEDSolidColor(CRGB(rgb_red, rgb_green, rgb_blue));
		}
	}
	else if (String(MQTT_RGB_GETVALUE_TOPIC).equals(p_topic))
	{
		publishLEDRGBColor();
	}
	else if (String(MQTT_PROGRAM_COMMAND_TOPIC).equals(p_topic))
	{
		uint8_t index = payload.toInt();
		if (_sequenceManager->getSequence() != index)
		{
			//_sequenceManager->setSequence(static_cast<SequenceName>(index));
			_sequenceManager->setSequence(SequenceName::_from_integral(index));
		}
	}
	else if (String(MQTT_PROGRAM_GETVALUE_TOPIC).equals(p_topic))
	{
		publishLEDProgram();
	}
	else if (String(MQTT_PROGRAM_CYCLE_COMMAND_TOPIC).equals(p_topic))
	{
		uint8_t index = payload.toInt();

		// 0 = next
		// 1 = previous

		// todo: implement previous...

		_sequenceManager->gotoNextPlaylistProgram();

		// publishLEDProgram();
	}
	else if (String(MQTT_PROGRAM_DURATION_COMMAND_TOPIC).equals(p_topic))
	{
		uint8_t firstIndex = payload.indexOf(',');
		uint16_t min = payload.substring(0, firstIndex).toInt();
		uint16_t max = payload.substring(firstIndex + 1).toInt();

		if (min > 0 && _sequenceManager->getMinimumPlaylistDurationSeconds() != min)
		{
			_sequenceManager->setMinimumPlaylistDuration(min);
		}

		if (max > 0 && _sequenceManager->getMaximumPlaylistDurationSeconds() != max)
		{
			_sequenceManager->setMaximumPlaylistDuration(max);
		}
	}
	else if (String(MQTT_PROGRAM_SPEED_COMMAND_TOPIC).equals(p_topic))
	{
		uint8_t index = payload.toInt();
		_sequenceManager->setSequenceDelay(index);
	}
	else if (String(MQTT_PLAYLIST_COMMAND_TOPIC).equals(p_topic))
	{
		uint8_t index = payload.toInt();

		if (_sequenceManager->getCurrentPlaylistIndex() != index)
		{
			_sequenceManager->loadPlayList(index);
		}
	}
	else if (String(MQTT_PLAYLIST_GETVALUE_TOPIC).equals(p_topic))
	{
		publishActivePlaylist();
	}
	else if (String(MQTT_PLAYLIST_RUNTIME_GETVALUE_TOPIC).equals(p_topic))
	{
		publishLEDProgramDuration();
	}
	else if (String(MQTT_PLAYLIST_GETLIST_TOPIC).equals(p_topic))
	{
		publishAvailablePlaylists();
	}
	else if (String(MQTT_PLAYLIST_GETLIST_TOPIC).equals(p_topic))
	{
		_onRebootRequest();
	}
	else if (String(MQTT_OS_BLINKLED_COMMAND_TOPIC).equals(p_topic))
	{
		uint8_t index = payload.toInt();
		if (_sequenceManager->isInternalLEDBlinkEnabled() != index)
		{
			_sequenceManager->enableInternalLEDBlink(index);
		}
	}
	else if (String(MQTT_OS_BLINKLED_GETVALUE_TOPIC).equals(p_topic))
	{
		publishOSBlinkLEDState();
	}
	else if (String(MQTT_OS_REBOOT_COMMAND_TOPIC).equals(p_topic))
	{
		_onRebootRequest();
	}
}

void MQTT::init(SequenceManager *seqmanager)
{
	_sequenceManager = seqmanager;

	PRINT_INFO("MQTT init.\n");

	// PRINT_INFO("MQTT: seqman current sequence: %d", _sequenceManager->getSequence());

	_mqttClient.onConnect(std::bind(&MQTT::onMqttConnect, this, std::placeholders::_1));
	_mqttClient.onDisconnect(std::bind(&MQTT::onMqttDisconnect, this, std::placeholders::_1));
	_mqttClient.onMessage(std::bind(&MQTT::onMqttMessage, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5, std::placeholders::_6));

	_mqttClient.setServer(MQTT_HOST, MQTT_PORT);
	_mqttClient.setCredentials(mqtt_user, mqtt_password);

	//_mqttReconnectTimer = xTimerCreate("mqttConTimer", pdMS_TO_TICKS(2000), pdFALSE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(connectTimerCallback));
	//_mqttPublishTimer = xTimerCreate("mqttPubTimer", pdMS_TO_TICKS(1000), pdFALSE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(publishTimerCallback));

	_isInitialized = true;
}

void MQTT::connect(void)
{
	PRINT_INFO("Connecting to MQTT...\n");
	_mqttClient.disconnect();
	_mqttClient.connect();
}

void MQTT::stopReconnectTimer(void)
{
	// ensure we don't reconnect to MQTT while reconnecting to Wi-Fi
	// xTimerStop(_mqttReconnectTimer, 0);

	// PRINT_INFO("MQTT Reconnect timer stopped.\n");
}

void MQTT::disconnect(void)
{
	PRINT_INFO("Disconnecting from MQTT...\n");
	_mqttClient.disconnect();
}

void MQTT::publishDeviceState(void)
{
	switch (_sequenceManager->getPowerState())
	{
	case ManagerPowerState::ManagerPowerOn:
	case ManagerPowerState::ManagerPoweringOn:
		_mqttClient.publish(MQTT_LIGHT_VALUE_TOPIC, 0, false, LIGHT_ON);
		break;

	case ManagerPowerState::ManagerPowerOff:
	case ManagerPowerState::ManagerPoweringOff:
		_mqttClient.publish(MQTT_LIGHT_VALUE_TOPIC, 0, false, LIGHT_OFF);
		break;

	case ManagerPowerState::ManagerStandby:
		_mqttClient.publish(MQTT_LIGHT_VALUE_TOPIC, 0, false, LIGHT_STANDBY);
		break;
	}

	PRINT_INFO("Publishing MQTT message '%s' at QoS 0, value: '%s'.\n", MQTT_LIGHT_VALUE_TOPIC, _sequenceManager->getPowerState()._to_string());
}

void MQTT::publishLEDBrightness(void)
{
	publish(MQTT_BRIGHTNESS_VALUE_TOPIC, _sequenceManager->getLEDBrightness());

	// char m_msg_buffer[MQTT_MSG_BUFFER_SIZE];

	// snprintf(m_msg_buffer, MQTT_MSG_BUFFER_SIZE, "%d", _sequenceManager->getLEDBrightness());
	// uint16_t packetIdSub = _mqttClient.publish(MQTT_BRIGHTNESS_VALUE_TOPIC, 0, false, m_msg_buffer);

	// PRINT_INFO("Publishing MQTT message '%s' at QoS 0, value: %s\n", MQTT_BRIGHTNESS_VALUE_TOPIC, m_msg_buffer);
}

void MQTT::publishLEDRGBColor(void)
{
	char m_msg_buffer[MQTT_MSG_BUFFER_SIZE];

	snprintf(m_msg_buffer, MQTT_MSG_BUFFER_SIZE, "%d,%d,%d",
			 _sequenceManager->getLEDSolidColor().r,
			 _sequenceManager->getLEDSolidColor().g,
			 _sequenceManager->getLEDSolidColor().b);

	uint16_t packetIdSub = _mqttClient.publish(MQTT_RGB_VALUE_TOPIC, 0, false, m_msg_buffer);

	PRINT_INFO("Publishing MQTT message '%s' at QoS 0, packetId: %s\n", MQTT_RGB_VALUE_TOPIC, m_msg_buffer);
}

void MQTT::publishLEDProgram(void)
{
	char m_msg_buffer[MQTT_MSG_BUFFER_SIZE];

	uint8_t seq = _sequenceManager->getSequence();

	snprintf(m_msg_buffer, MQTT_MSG_BUFFER_SIZE, "%d", seq);
	_mqttClient.publish(MQTT_PROGRAM_VALUE_TOPIC, 0, false, m_msg_buffer);

	PRINT_INFO("Publishing MQTT message '%s' at QoS 0, value: '%s' (%d)\n",
			   MQTT_PROGRAM_VALUE_TOPIC,
			   _sequenceManager->getSequence()._to_string(),
			   seq);
}

void MQTT::publishLEDProgramDuration(void)
{
	char m_msg_buffer[MQTT_MSG_BUFFER_SIZE];

	snprintf(m_msg_buffer, MQTT_MSG_BUFFER_SIZE, "%d", _sequenceManager->getRemainingRandomCyleDuration());
	_mqttClient.publish(MQTT_PLAYLIST_RUNTIME_VALUE_TOPIC, 0, false, m_msg_buffer);

	PRINT_INFO("Publishing MQTT message '%s' at QoS 0, value: %s\n", MQTT_PLAYLIST_RUNTIME_VALUE_TOPIC, m_msg_buffer);
}

void MQTT::publishActivePlaylist(void)
{
	char m_msg_buffer[MQTT_MSG_BUFFER_SIZE];

	snprintf(m_msg_buffer, MQTT_MSG_BUFFER_SIZE, "%d", _sequenceManager->getCurrentPlaylistIndex());
	_mqttClient.publish(MQTT_PLAYLIST_VALUE_TOPIC, 0, false, m_msg_buffer);

	PRINT_INFO("Publishing MQTT message '%s' at QoS 0, value: %s\n", MQTT_PLAYLIST_VALUE_TOPIC, m_msg_buffer);
}

void MQTT::publishAvailablePlaylists(void)
{
	// hmmm......well, it works.
	uint16_t packetIdSub = _mqttClient.publish(
		MQTT_PLAYLIST_LIST_TOPIC,
		0,
		true,
		"{\"playlists\":[{\"index\":0,\"name\":\"No Playlist\"},{\"index\":1,\"name\":\"All of Them\"},{\"index\":2,\"name\":\"Nice Flows\"},{\"index\":3,\"name\":\"All of Them (but short)\"}]}");

	PRINT_INFO("Publishing MQTT message '%s' at QoS 0, packetId: %d\n", MQTT_PLAYLIST_LIST_TOPIC, packetIdSub);
}

void MQTT::publishOSBlinkLEDState(void)
{
	publish(MQTT_OS_BLINKLED_VALUE_TOPIC, _sequenceManager->isRandomCycleEnabled());
}

void MQTT::publishLEDProgramDelay(void)
{
	publish(MQTT_OS_BLINKLED_VALUE_TOPIC, _sequenceManager->getSequenceDelay());
}

void MQTT::publishPilotLight(void)
{
	publish(MQTT_OS_PILOTLIGHT_VALUE_TOPIC, _sequenceManager->isPilotLightEnabled());
}

void MQTT::publish(const char *topic, int value)
{
	char m_msg_buffer[MQTT_MSG_BUFFER_SIZE];

	snprintf(m_msg_buffer, MQTT_MSG_BUFFER_SIZE, "%d", value);
	_mqttClient.publish(topic, 0, false, m_msg_buffer);

	PRINT_INFO("Publishing MQTT message '%s' at QoS 0, value: %s\n", topic, m_msg_buffer);
}

void MQTT::onPowerStateChange(std::function<void(CommandPowerState)> fn)
{
	_onPowerStateChangeCallback = std::bind(fn, std::placeholders::_1);
}

void MQTT::onRebootRequest(std::function<void(void)> fn)
{
	_onRebootRequest = std::bind(fn);
}

bool MQTT::isInitialized(void)
{
	return _isInitialized;
}

bool MQTT::isConnected(void)
{
	return _mqttClient.connected();
}
