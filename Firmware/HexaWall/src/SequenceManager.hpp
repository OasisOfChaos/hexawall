// SequenceManager.h

#pragma once

#ifndef _SEQUENCEMANAGER_h
#define _SEQUENCEMANAGER_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

using namespace std;

//#define NUM_LEDS	750
#define LED_BUILTIN			2

#define FASTLED_ALLOW_INTERRUPTS 0

#include <memory>
#include <FastLED.h>
#include "Sequence.hpp"
#include "PlayLists.hpp"
#include "SequenceNames.hpp"
#include "Playlist.hpp"
#include "settings.hpp"
#include "enum.h"

BETTER_ENUM(
	CommandPowerState,
	int,
	PowerOff,
	PowerOn,
	Standby
);

BETTER_ENUM(
	ManagerPowerState,
	int,
	ManagerPowerOff,
	ManagerPoweringOff,
	ManagerPowerOn,
	ManagerPoweringOn,
	ManagerGoingToStandby,
	ManagerStandby
);

BETTER_ENUM(
	ManagerStatusTopic,
	int,
	StatusTopicPowerState,
	StatusTopicBrightness,
	StatusTopicSequence,
	StatusTopicSolidColor,
	StatusTopicPilotLight,
	StatusTopicPlaylist,
	StatusTopicPlaylistDuration,
	StatusTopicInternalLEDBlink,
	StatusTopicSequenceDelay
);

#define LED_PIN     14
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
//#define LED_COUNT    750

class SequenceManager
{
private:
	Sequence* _currentSequence = NULL;
	SequenceName _currentSequenceName = SequenceName::SwitchOn;
	bool _sequenceIsChanging = false;
	int _sequenceDelayMSecs = 10;

	int _ledBrightness = 255;
	int _ledTargetBrightness = 255;
	int _ledLastSetBrightness = 255;

	uint8_t _tickCounter = 0;

	CRGB _currentLEDSolidColor = CRGB::Red;

	ManagerPowerState _currentPowerState = ManagerPowerState::ManagerPowerOff;

	const SequenceName _testSequence = SequenceName::CyclePrimaryColors;

	bool _internalLEDBlinkEnabled = true;
	bool _pilotLightEnabled = false;

	int _playlistCycleDuration = 0;
	int _playlistCycleSecondCounter = 0;
	bool _playlistCycleEnabled = false;
	bool _loadNewPlaylistItem = false;
	// uint8_t _currentPlaylistItemIndex = 0;
	// uint8_t _previousPlaylistItemIndex = 254;
	// uint8_t _newPlaylistItemIndex = 0;
	PlayList* _currentPlaylist = NULL;
	uint8_t _currentPlaylistIndex = 0;

	const long _switchStandbyToPowerOffTimeout = (3 * 60 * 60); //3 hours
	long _standbyModeDurationSecond = 0;

	TimerHandle_t _secondTimer;

	std::function<void(ManagerStatusTopic)> _onStatusChangeCallback;
	std::function<void(ManagerPowerState)> _onPowerStateChange;

	static void secondTimerCallback(TimerHandle_t xTimer);
	void secondCounterCallback(void);
	void enableSecondTimer(bool value);

	//void secondCounter(void);
	void getNextPlaylistItem();

	void wakeFromStandby();


protected:
	static const int LEDCount = LED_COUNT;
	CRGB LEDArray[LEDCount];

public:
	SequenceManager();
	~SequenceManager();

	void init();
	void tick();

	void setSequence(SequenceName sequence);
	void setPowerState(CommandPowerState state);
	void setLEDBrightness(int brightness, bool userInitiated);
	void setLEDSolidColor(CRGB color);
	void setSequenceDelay(int newDelay);

	void restoreSavedLEDBrightness();
	//void setRandomCycleDuration(int minDurationSeconds, int maxDurationSeconds);
	//void getNewRandomSequence();

	void enablePilotLight(bool value);
	void enableRandomCycle(bool enable);

	SequenceName getSequence();
	ManagerPowerState getPowerState();
	int getLEDBrightness();
	CRGB getLEDSolidColor();
	int getCurrentPlaylistItemRuntime();
	int getRemainingRandomCyleDuration();
	int getMinimumPlaylistDurationSeconds();
	int getMaximumPlaylistDurationSeconds();
	String getCurrentPlaylistName();
	int getCurrentPlaylistIndex();
	bool isRandomCycleEnabled();
	bool isInternalLEDBlinkEnabled();
	bool isPilotLightEnabled();
	int getSequenceDelay();

	void loadPlayList(uint8_t index);
	void setMinimumPlaylistDuration(int value);
	void setMaximumPlaylistDuration(int value);
	void gotoNextPlaylistProgram();

	void enableInternalLEDBlink(bool state);

	void onStatusChange(std::function<void(ManagerStatusTopic)> fn);
	void onPowerStateChange(std::function<void(ManagerPowerState)> fn);
	//void onStatusChange(std::function<int(int, int)> fn) { _onStatusChangeCallback = std::bind(fn, std::placeholders::_1, std::placeholders::_2); }
};


#endif
