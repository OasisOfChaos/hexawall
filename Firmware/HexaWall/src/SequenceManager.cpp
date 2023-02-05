#include "SequenceManager.hpp"

#include "sequences\SeqCylon.hpp"
#include "sequences\SeqCyclePrimaryColors.hpp"
#include "sequences\SeqCycleTertiaryColors.hpp"
#include "sequences\SeqSolidColor.hpp"
#include "sequences\SeqPride.hpp"
#include "sequences\SeqTimedPalettes.hpp"
#include "sequences\SeqDripping.hpp"
#include "sequences\SeqFire.hpp"
#include "sequences\SeqBlendwave.hpp"
#include "sequences\SeqBlur.hpp"
#include "sequences\SeqConfetti.hpp"
#include "sequences\SeqDotBeat.hpp"
#include "sequences\SeqEasing.hpp"
#include "sequences\SeqInoiseFire.hpp"
#include "sequences\SeqLightning.hpp"
#include "sequences\SeqJugglePal.hpp"
#include "sequences\SeqMatrixPal.hpp"
#include "sequences\SeqMover.hpp"
#include "sequences\SeqFadeTest.hpp"
#include "sequences\SeqFadeInLeftToRight.hpp"
#include "sequences\SeqFadeOutRightToLeft.hpp"
#include "sequences\SeqNoise16a.hpp"
#include "sequences\SeqNoise16b.hpp"
#include "sequences\SeqNoise16c.hpp"
#include "sequences\SeqOneSinePal.hpp"
#include "sequences\SeqNone.hpp"
#include "sequences\SeqPaletteCrossfade.hpp"
#include "sequences\SeqPlasma.hpp"
#include "sequences\SeqRainbowBeat.hpp"
#include "sequences\SeqRainbowMarch.hpp"
#include "sequences\SeqRipplePal.hpp"
#include "sequences\SeqRipples.hpp"
#include "sequences\SeqSawTooth.hpp"
#include "sequences\SeqSerendipitous.hpp"
#include "sequences\SeqThreeSinPal.hpp"
#include "sequences\SeqTwoSinPal.hpp"
#include "sequences\SeqSineLon.hpp"
#include "sequences\SeqBreathing.hpp"
#include "sequences\SeqCandyCaneStripes.hpp"
#include "sequences\SeqChaseTargetValues.hpp"
#include "sequences\SeqStripedPalettes.hpp"
#include "sequences\SeqCylonColorChange.hpp"
#include "sequences\SeqHeartPulseBloodFlow.hpp"
#include "sequences\SeqMovingColors.hpp"
#include "sequences\SeqSimulatedTVLighting.hpp"
#include "sequences\SeqRepeatingBlockPattern.hpp"
#include "sequences\SeqRainbowBrightnessAndSaturation.hpp"
#include "sequences\SeqPacifica.hpp"
#include "sequences\SeqGradientPalette.hpp"
#include "sequences\SeqMovingGradient.hpp"
#include "sequences\SeqQuadwave8FadeUp.hpp"
#include "sequences\SeqFireMirrored.hpp"
#include "sequences\SeqMirrorAndFadeEnds.hpp"
#include "sequences\SeqSwirlOut.hpp"
#include "sequences\SeqColumnTest.hpp"
#include "sequences\SeqVertColumnLeftRight.hpp"

// https://github.com/atuline/FastLED-Demos
// https://github.com/marmilicious/FastLED_examples

using namespace std;

SequenceManager::SequenceManager()
{
}

SequenceManager::~SequenceManager()
{
	PRINT_DEBUG("SequenceManager destructor\n");
}

void SequenceManager::init()
{
	FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(LEDArray, LEDCount).setCorrection(TypicalLEDStrip);

	pinMode(LED_BUILTIN, OUTPUT);

#ifndef DEVELOP_MODE
	setSequence(SequenceName::None);
#endif // DEVELOP_MODE

	setLEDBrightness(255, false);

	// https://stackoverflow.com/questions/71199868/c-use-a-class-non-static-method-as-a-function-pointer-callback-in-freertos-xti
	// https://fjrg76.wordpress.com/2021/05/19/objectifying-the-freertos-timers-through-the-crtp-pattern/
	_secondTimer = xTimerCreate("secondTimer", pdMS_TO_TICKS(1000), pdTRUE, this, secondTimerCallback);
	xTimerStart(_secondTimer, 0);

	PRINT_DEBUG("SequenceManager init() done.\n");

#ifdef DEVELOP_MODE
	PRINT_INFO("DEVELOPMENT MODE DETECTED.\n");
#endif
}

void SequenceManager::tick()
{
	EVERY_N_MILLISECONDS(5)
	{
		_tickCounter += 5;
		if (_tickCounter < _sequenceDelayMSecs)
		{
			return;
		}

		_tickCounter = 0;

		try
		{
			if (_loadNewPlaylistItem)
			{
				// do this here instead of in the timer callback to prevent timer stack issues
				this->setSequence(_currentPlaylist->getNextSequence());
				_loadNewPlaylistItem = false;
			}

			if (_currentSequence != NULL)
			{
				if (!_sequenceIsChanging)
				{
					_currentSequence->tick();
					FastLED.show();

					// do something after the current sequence...
					if (_currentSequence->isFinished())
					{
						if (_currentPowerState == +ManagerPowerState::ManagerPoweringOn)
						{
							this->setPowerState(CommandPowerState::PowerOn);
						}

						if (_currentPowerState == +ManagerPowerState::ManagerPoweringOff)
						{
							setPowerState(CommandPowerState::PowerOff);
						}

						if (_currentPowerState == +ManagerPowerState::ManagerGoingToStandby)
						{
							setPowerState(CommandPowerState::Standby);
						}
					}
				}
			}
		}
		catch (...)
		{
			PRINT_INFO("EXCEPTION handled in SequenceManager::tick().\n");
		}
	}
}

void SequenceManager::secondTimerCallback(TimerHandle_t xTimer)
{
	SequenceManager *p = static_cast<SequenceManager *>(pvTimerGetTimerID(xTimer));
	p->secondCounterCallback();
}

void SequenceManager::secondCounterCallback()
{
	// Serial.println("secondTimerCallback()");

	if (_currentSequence != NULL)
	{
		_currentSequence->secondCounter();
	}

	if (_playlistCycleEnabled)
	{
		_playlistCycleSecondCounter++;

		// Serial.printf("_playlistCycleSecondCounter: %d", _playlistCycleSecondCounter);
		// Serial.printf("%d ", _playlistCycleSecondCounter);

		if (_playlistCycleSecondCounter > _playlistCycleDuration)
		{
			getNextPlaylistItem();
		}
	}

	if (_currentPowerState == +ManagerPowerState::ManagerStandby)
	{
		// if device is standby more than n hours, switch off to save power...
		_standbyModeDurationSecond++;

		// Serial.printf("STANDBY TIMER: _standbyModeDurationSecond: %d, _switchStandbyToPowerOffTimeout: %d\n", _standbyModeDurationSecond, _switchStandbyToPowerOffTimeout);

		if (_standbyModeDurationSecond > _switchStandbyToPowerOffTimeout)
		{
			PRINT_INFO("Powering off because of a long standby time.\n");
			this->setPowerState(CommandPowerState::PowerOff);
		}
	}

	if (_internalLEDBlinkEnabled)
	{
		{
#ifdef DEVELOP_MODE
			digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
#endif // DEVELOP_MODE
			digitalWrite(PIN_LED_PS_ON, !digitalRead(PIN_LED_PS_ON));
		}
	}
}

void SequenceManager::enableSecondTimer(bool value)
{
	if (value)
	{
		xTimerStart(_secondTimer, 0);
		PRINT_INFO("SequenceManager: enabling second timer.\n");
	}
	else
	{
		xTimerStop(_secondTimer, 0);
		PRINT_INFO("SequenceManager: disabling second timer.\n");
	}
}

void SequenceManager::getNextPlaylistItem()
{
	_playlistCycleEnabled = false;
	_playlistCycleSecondCounter = 0;

	_playlistCycleDuration = random16(_currentPlaylist->getMinimumDurationSeconds(), _currentPlaylist->getMaximumDurationSeconds());
	PRINT_INFO("New Playlist Item Cycle Duration: %d seconds.\n", _playlistCycleDuration);

	_playlistCycleEnabled = true;
	_loadNewPlaylistItem = true;
}

void SequenceManager::wakeFromStandby()
{
	PRINT_DEBUG("SequenceManager is waking up from standby due to a sequence change.\n");
	_currentPowerState = ManagerPowerState::ManagerPowerOn;
	restoreSavedLEDBrightness();
	enableSecondTimer(true);

	if (_onPowerStateChange != nullptr)
	{
		_onPowerStateChange(_currentPowerState);
	}

	if (_onStatusChangeCallback != nullptr)
	{
		_onStatusChangeCallback(ManagerStatusTopic::StatusTopicPowerState);
	}
}

void SequenceManager::setSequence(SequenceName sequence)
{
	try
	{
		_sequenceIsChanging = true;

		if (_currentSequence != NULL)
		{
			PRINT_DEBUG("Destroying current sequence...\n");

			// clear/destroy current sequence
			delete _currentSequence;
		}

		// if a sequence is set and we're in standby, go back to power on
		// but without the normal procedure from setPowerState
		if (_currentPowerState == +ManagerPowerState::ManagerStandby)
		{
			wakeFromStandby();
		}

		switch (sequence)
		{
		case SequenceName::None:
			_currentSequence = new SeqNone(LEDArray, LEDCount);
			break;

		case SequenceName::SolidColorBlend:
			_currentSequence = new SeqSolidColor(LEDArray, LEDCount, _currentLEDSolidColor, true);
			break;

		case SequenceName::SwitchOn:
			_currentSequence = new SeqFadeInLeftToRight(LEDArray, LEDCount, 0);
			break;

		case SequenceName::SwitchOff:
			//_currentSequence = new SeqFadeInLeftToRight(LEDArray, LEDCount, 0);
			//_currentSequence = new SeqFadeInLeftToRight(LEDArray, LEDCount, 1);
			//_currentSequence = new SeqFadeOutRightToLeft(LEDArray, LEDCount);
			_currentSequence = new SeqSwirlOut(LEDArray, LEDCount);
			break;

			// case SequenceName::Cylon:
			//	_currentSequence = new SeqCylon(LEDArray, LEDCount);
			//	break;

		case SequenceName::CyclePrimaryColors:
			_currentSequence = new SeqCyclePrimaryColors(LEDArray, LEDCount);
			break;

		case SequenceName::CycleTertiaryColors:
			_currentSequence = new SeqCycleTertiaryColors(LEDArray, LEDCount);
			break;

		case SequenceName::SolidColorDirect:
			_currentSequence = new SeqSolidColor(LEDArray, LEDCount, _currentLEDSolidColor, false);
			break;

		case SequenceName::Pride:
			_currentSequence = new SeqPride(LEDArray, LEDCount);
			break;

		case SequenceName::Dripping:
			_currentSequence = new SeqDripping(LEDArray, LEDCount);
			break;

		case SequenceName::TimedPalettes:
			_currentSequence = new SeqTimedPalettes(LEDArray, LEDCount);
			break;

		case SequenceName::Fire:
			_currentSequence = new SeqFire(LEDArray, LEDCount);
			break;

		case SequenceName::Blendwave:
			_currentSequence = new SeqBlendwave(LEDArray, LEDCount);
			break;

		case SequenceName::Blur:
			_currentSequence = new SeqBlur(LEDArray, LEDCount);
			break;

		case SequenceName::Confetti:
			_currentSequence = new SeqConfetti(LEDArray, LEDCount);
			break;

		case SequenceName::DotBeat:
			_currentSequence = new SeqDotBeat(LEDArray, LEDCount);
			break;

		case SequenceName::Easing:
			_currentSequence = new SeqEasing(LEDArray, LEDCount);
			break;

		case SequenceName::InoiseFire:
			_currentSequence = new SeqInoiseFire(LEDArray, LEDCount);
			break;

		case SequenceName::Lightning:
			_currentSequence = new SeqLightning(LEDArray, LEDCount);
			break;

		case SequenceName::JugglePal:
			_currentSequence = new SeqJugglePal(LEDArray, LEDCount);
			break;

		case SequenceName::MatrixPal:
			_currentSequence = new SeqMatrixPal(LEDArray, LEDCount);
			break;

		case SequenceName::Mover:
			_currentSequence = new SeqMover(LEDArray, LEDCount);
			break;

		case SequenceName::FadeTest:
			_currentSequence = new SeqFadeTest(LEDArray, LEDCount);
			break;

		case SequenceName::FadeInLeftToRight:
			_currentSequence = new SeqFadeInLeftToRight(LEDArray, LEDCount, 0);
			break;

		case SequenceName::Noise16a:
			_currentSequence = new SeqNoise16a(LEDArray, LEDCount);
			break;

		case SequenceName::Noise16b:
			_currentSequence = new SeqNoise16b(LEDArray, LEDCount);
			break;

		case SequenceName::Noise16c:
			_currentSequence = new SeqNoise16c(LEDArray, LEDCount);
			break;

		case SequenceName::OneSinePal:
			_currentSequence = new SeqOneSinePal(LEDArray, LEDCount);
			break;

		case SequenceName::PaletteCrossfade:
			_currentSequence = new SeqPaletteCrossfade(LEDArray, LEDCount);
			break;

		case SequenceName::Plasma:
			_currentSequence = new SeqPlasma(LEDArray, LEDCount);
			break;

		case SequenceName::RainbowBeat:
			_currentSequence = new SeqRainbowBeat(LEDArray, LEDCount);
			break;

		case SequenceName::RainbowMarch:
			_currentSequence = new SeqRainbowMarch(LEDArray, LEDCount);
			break;

		case SequenceName::RipplePal:
			_currentSequence = new SeqRipplePal(LEDArray, LEDCount);
			break;

		case SequenceName::Ripples:
			_currentSequence = new SeqRipples(LEDArray, LEDCount);
			break;

			// case SequenceName::SawTooth:
			//	_currentSequence = new SeqSawTooth(LEDArray, LEDCount);
			//	break;

			// case SequenceName::Serendipitous:
			//	_currentSequence = new SeqSerendipitous(LEDArray, LEDCount);
			//	break;

		case SequenceName::TwoSinPal:
			_currentSequence = new SeqTwoSinPal(LEDArray, LEDCount);
			break;

		case SequenceName::ThreeSinPal:
			_currentSequence = new SeqThreeSinPal(LEDArray, LEDCount);
			break;

		case SequenceName::SineLon:
			_currentSequence = new SeqSineLon(LEDArray, LEDCount);
			break;

		case SequenceName::Breathing:
			_currentSequence = new SeqBreathing(LEDArray, LEDCount);
			break;

			// case SequenceName::CandyCaneStripes:
			//	_currentSequence = new SeqCandyCaneStripes(LEDArray, LEDCount);
			//	break;

		case SequenceName::ChaseTargetValues:
			_currentSequence = new SeqChaseTargetValues(LEDArray, LEDCount);
			break;

		case SequenceName::StripedPalettes:
			_currentSequence = new SeqStripedPalettes(LEDArray, LEDCount);
			break;

		case SequenceName::CylonColorChange:
			_currentSequence = new SeqCylonColorChange(LEDArray, LEDCount);
			break;

		case SequenceName::HeartPulseBloodFlow:
			_currentSequence = new SeqHeartPulseBloodFlow(LEDArray, LEDCount);
			break;

			// too boring
			// case SequenceName::MovingColors:
			//	_currentSequence = new SeqMovingColors(LEDArray, LEDCount);
			//	break;

		case SequenceName::SimulatedTVLighting:
			_currentSequence = new SeqSimulatedTVLighting(LEDArray, LEDCount);
			break;

		case SequenceName::RepeatingBlockPattern:
			_currentSequence = new SeqRepeatingBlockPattern(LEDArray, LEDCount);
			break;

			// case SequenceName::RainbowBrightnessAndSaturation:
			//	_currentSequence = new SeqRainbowBrightnessAndSaturation(LEDArray, LEDCount);
			//	break;

		case SequenceName::Pacifica:
			_currentSequence = new SeqPacifica(LEDArray, LEDCount);
			break;

		case SequenceName::GradientPalette:
			_currentSequence = new SeqGradientPalette(LEDArray, LEDCount);
			break;

			// case SequenceName::MovingGradient:
			//	_currentSequence = new SeqMovingGradient(LEDArray, LEDCount);
			//	break;

			// case SequenceName::Quadwave8FadeUp:
			//	_currentSequence = new SeqQuadwave8FadeUp(LEDArray, LEDCount);
			//	break;

		case SequenceName::FireMirrored:
			//_currentSequence = new SeqFireMirrored(LEDArray, LEDCount);
			break;

		case SequenceName::ColumnTest:
			_currentSequence = new SeqColumnTest(LEDArray, LEDCount);
			break;

			//#ifdef DEVELOP_MODE
			//		case SequenceName::None:
			//#endif // DEVELOP_MODE
			//		case SequenceName::MirrorAndFadeEnds:
			//			_currentSequence = new SeqMirrorAndFadeEnds(LEDArray, LEDCount);
			//			break;

		case SequenceName::VertColumnLeftRight:
			_currentSequence = new SeqVertColumnLeftRight(LEDArray, LEDCount);
			break;

		default:
			PRINT_INFO("SequenceManager ABORT! ERROR!! No case handler for sequence '%s', bloody!!!\n", sequence._to_string());
			break;
		}

		_currentSequence->setLEDBrightness(_ledBrightness);

		_currentSequenceName = sequence;

		// FastLED.clear();

		PRINT_INFO("SequenceManager changed sequence to: '%s' [%d].\n", sequence._to_string(), sequence);

		//_onStatusChangeCallback(0, 1);
		if (_onStatusChangeCallback != nullptr)
		{
			_onStatusChangeCallback(ManagerStatusTopic::StatusTopicSequence);
			_onStatusChangeCallback(ManagerStatusTopic::StatusTopicPlaylistDuration);
		}

		_sequenceIsChanging = false;
	}
	// catch (const std::exception &ex)
	// {
	// 	PRINT_INFO("EXCEPTION handled in SequenceManager::setSequence()\n");
	// }
	// catch (const std::string &ex)
	// {
	// 	PRINT_INFO("EXCEPTION handled in SequenceManager::setSequence()\n");
	// }
	catch (...)
	{
		std::exception_ptr p = std::current_exception();
		// PRINT_INFO(p ? p.__cxa_exception_type()->name() : "null");
		PRINT_INFO("EXCEPTION handled in SequenceManager::setSequence(): %s\n",
				   p ? p.__cxa_exception_type()->name() : "null");
	}
}

void SequenceManager::setPowerState(CommandPowerState state)
{
	// PRINT_DEBUG("SequenceManager::setPowerState _currentPowerState: ManagerPowerState::'%s', requested CommandPowerStatestate::'%s'\n", _currentPowerState._to_string(), state._to_string());

	switch (state)
	{
	case CommandPowerState::PowerOff:
		if (_currentPowerState != +ManagerPowerState::ManagerPoweringOff)
		{
			enableRandomCycle(false);

			_currentPowerState = ManagerPowerState::ManagerPoweringOff;
			setSequence(SequenceName::SwitchOff);
		}
		else
		{
			_currentPowerState = ManagerPowerState::ManagerPowerOff;
			setSequence(SequenceName::None);
			enableSecondTimer(false);

			//_onPoweroffCallback();
		}

		break;

	case CommandPowerState::PowerOn:
		if (_currentPowerState != +ManagerPowerState::ManagerPoweringOn)
		{
			_currentPowerState = ManagerPowerState::ManagerPoweringOn;
			restoreSavedLEDBrightness();

#ifndef DEVELOP_MODE
			setSequence(SequenceName::SwitchOn);
#endif // DEVELOP_MODE

			enableSecondTimer(true);
		}
		else
		{
			_currentPowerState = ManagerPowerState::ManagerPowerOn;

#ifndef DEVELOP_MODE
			this->loadPlayList(2); // flows
								   // this->loadPlayList(1);		//all of them
#endif							   // !DEVELOP_MODE
		}

		break;

	case CommandPowerState::Standby:
		// PRINT_DEBUG("entering standby case.");
		if (_currentPowerState != +ManagerPowerState::ManagerGoingToStandby)
		{
			_currentPowerState = ManagerPowerState::ManagerGoingToStandby;
			// PRINT_DEBUG("setting _currentPowerState = ManagerPowerState::ManagerGoingToStandby");

			this->setSequence(SequenceName::None);
			// PRINT_DEBUG("setting sequence to none");
		}
		else
		{
			enableSecondTimer(false);

			this->enablePilotLight(true);
			_standbyModeDurationSecond = 0;

			// PRINT_DEBUG("setting _currentPowerState = ManagerPowerState::ManagerStandby");
			_currentPowerState = ManagerPowerState::ManagerStandby;
		}

		break;

	default:
		break;
	}

	PRINT_INFO("SequenceManager entered ManagerPowerState::PowerState: '%s'.\n", _currentPowerState._to_string());

	if (_onPowerStateChange != nullptr)
	{
		_onPowerStateChange(_currentPowerState);
	}

	if (_onStatusChangeCallback != nullptr)
	{
		_onStatusChangeCallback(ManagerStatusTopic::StatusTopicPowerState);
	}
}

void SequenceManager::setLEDBrightness(int brightness, bool saveOldValue)
{
	if (brightness == _ledBrightness)
	{
		return;
	}

	if (brightness > 255)
	{
		brightness = 255;
	}

	if (brightness < 0)
	{
		brightness = 0;
	}

	if (saveOldValue)
	{
		_ledLastSetBrightness = _ledBrightness;
		PRINT_DEBUG("SequenceManager saved the current LED Brightness (%d).\n", _ledLastSetBrightness);
	}

	_ledBrightness = brightness;
	FastLED.setBrightness(_ledBrightness);

	if (_currentSequence != NULL)
	{
		_currentSequence->setLEDBrightness(_ledBrightness);
	}

	PRINT_INFO("SequenceManager changed LED Brightness to: %d.\n", _ledBrightness);

	if (_onStatusChangeCallback != nullptr)
	{
		_onStatusChangeCallback(ManagerStatusTopic::StatusTopicBrightness);
	}
}

void SequenceManager::setLEDSolidColor(CRGB color)
{
	_currentLEDSolidColor = color;

	// disable playlist if loaded
	if (_currentPlaylist != 0)
	{
		this->loadPlayList(0);
	}

	// make sure the right sequence is instantiated first
	if (_currentSequenceName != +SequenceName::SolidColorBlend && _currentSequenceName != +SequenceName::SolidColorDirect)
	{
		// default to the blend variant
		setSequence(SequenceName::SolidColorBlend);
	}

	if (_currentSequence != NULL)
	{
		_currentSequence->setSolidColor(color);
	}

	// PRINT_DEBUG("SequenceManager changed the CurrentLEDSolidColor to: R:%d, G:%d, B:%d.\n", color.r, color.g, color.b);

	if (_onStatusChangeCallback != nullptr)
	{
		_onStatusChangeCallback(ManagerStatusTopic::StatusTopicSolidColor);
	}
}

void SequenceManager::enablePilotLight(bool value)
{
	PRINT_DEBUG("SequenceManager enabled the PilotLight.\n");

	if (value)
	{
		this->setLEDBrightness(16, true);
	}
	else
	{
		restoreSavedLEDBrightness();
	}

	/*for (int i = 0; i < LEDCount; i++)
	{
		LEDArray[i] = CRGB::Black;
	}*/
	fill_solid(LEDArray, LEDCount, CRGB::Black);

	if (value)
	{
		LEDArray[14] = CRGB::Red;
	}

	FastLED.show();

	_pilotLightEnabled = value;

	if (_onStatusChangeCallback != nullptr)
	{
		_onStatusChangeCallback(ManagerStatusTopic::StatusTopicPilotLight);
	}
}

void SequenceManager::enableRandomCycle(bool enable)
{
	_playlistCycleEnabled = enable;

	if (_playlistCycleEnabled)
	{
		PRINT_DEBUG("Random Playlist Cycle has been enabled.\n");

		// turn on...
		this->getNextPlaylistItem();
	}
	else
	{
		PRINT_DEBUG("Random Playlist Cycle has been disabled.\n");
	}
}

SequenceName SequenceManager::getSequence()
{
	return _currentSequenceName;
}

ManagerPowerState SequenceManager::getPowerState()
{
	return _currentPowerState;
}

int SequenceManager::getLEDBrightness()
{
	return _ledBrightness;
}

CRGB SequenceManager::getLEDSolidColor()
{
	return _currentLEDSolidColor;
}

int SequenceManager::getCurrentPlaylistItemRuntime()
{
	return _playlistCycleDuration;
}

bool SequenceManager::isRandomCycleEnabled()
{
	return _playlistCycleEnabled;
}

bool SequenceManager::isInternalLEDBlinkEnabled()
{
	return _internalLEDBlinkEnabled;
}

bool SequenceManager::isPilotLightEnabled()
{
	return _pilotLightEnabled;
}

int SequenceManager::getSequenceDelay()
{
	return _sequenceDelayMSecs;
}

String SequenceManager::getCurrentPlaylistName()
{
	if (_currentPlaylist != NULL)
	{
		return _currentPlaylist->getName();
	}
	else
	{
		return "";
	}
}

int SequenceManager::getCurrentPlaylistIndex()
{
	return _currentPlaylistIndex;
}

int SequenceManager::getRemainingRandomCyleDuration()
{
	if (!_playlistCycleEnabled)
	{
		return 0;
	}

	return _playlistCycleDuration - _playlistCycleSecondCounter;
}

void SequenceManager::loadPlayList(uint8_t index)
{
	if (_currentPlaylist != NULL)
	{
		PRINT_DEBUG("Destroying current playlist...\n");

		// clear/destroy current sequence
		delete _currentPlaylist;
	}

	PRINT_DEBUG("Playlist index request: %d.\n", index);

	switch (index)
	{
	case 0:
		_currentPlaylist = NULL;
		PRINT_INFO("Unloaded playlist.\n");
		break;

	case 1:
		_currentPlaylist = new PlayListAll();
		break;

	case 2:
		_currentPlaylist = new PlayListFlows();
		break;

	case 3:
		_currentPlaylist = new PlayListAllShort();
		break;
		
	default:
		break;
	}

	if (index != 0)
	{
		PRINT_INFO("Loaded Playlist '%s'. Total sequences: %d.\n", _currentPlaylist->getName().c_str(), _currentPlaylist->getSequenceCount());
	}

	_currentPlaylistIndex = index;

	// now make sure it's enabled
	enableRandomCycle(index != 0);

	if (_onStatusChangeCallback != nullptr)
	{
		_onStatusChangeCallback(ManagerStatusTopic::StatusTopicPlaylist);
	}
}

void SequenceManager::setMinimumPlaylistDuration(int value)
{
	if (_currentPlaylist != NULL)
	{
		_currentPlaylist->setMinimumDurationSeconds(value);

		PRINT_INFO("SequenceManager changed the Minimum Playlist Duration to: %d\n", value);

		if (_onStatusChangeCallback != nullptr)
		{
			_onStatusChangeCallback(ManagerStatusTopic::StatusTopicPlaylistDuration);
		}
	}
}

void SequenceManager::setMaximumPlaylistDuration(int value)
{
	if (_currentPlaylist != NULL)
	{
		_currentPlaylist->setMaximumDurationSeconds(value);

		PRINT_INFO("SequenceManager changed the Maximum Playlist Duration to: %d\n", value);
		if (_onStatusChangeCallback != nullptr)
		{
			_onStatusChangeCallback(ManagerStatusTopic::StatusTopicPlaylistDuration);
		}
	}
}

int SequenceManager::getMinimumPlaylistDurationSeconds()
{
	if (_currentPlaylist == NULL)
	{
		return 0;
	}

	return _currentPlaylist->getMinimumDurationSeconds();
}

int SequenceManager::getMaximumPlaylistDurationSeconds()
{
	if (_currentPlaylist == NULL)
	{
		return 0;
	}

	return _currentPlaylist->getMaximumDurationSeconds();
}

void SequenceManager::gotoNextPlaylistProgram()
{
	this->getNextPlaylistItem();
}

void SequenceManager::setSequenceDelay(int newDelay)
{
	_sequenceDelayMSecs = newDelay;

	PRINT_INFO("SequenceManager changed the sequence delay to: %d\n", newDelay);

	if (_onStatusChangeCallback != nullptr)
	{
		_onStatusChangeCallback(ManagerStatusTopic::StatusTopicSequenceDelay);
	}
}

void SequenceManager::restoreSavedLEDBrightness()
{
	PRINT_DEBUG("Restoring saved LED Brightness (%d).\n", _ledLastSetBrightness);
	setLEDBrightness(_ledLastSetBrightness, false);
}

void SequenceManager::enableInternalLEDBlink(bool state)
{
	_internalLEDBlinkEnabled = state;
	if (_onStatusChangeCallback != nullptr)
	{
		_onStatusChangeCallback(ManagerStatusTopic::StatusTopicInternalLEDBlink);
	}
}

void SequenceManager::onStatusChange(std::function<void(ManagerStatusTopic)> fn)
{
	_onStatusChangeCallback = std::bind(fn, std::placeholders::_1);
}

void SequenceManager::onPowerStateChange(std::function<void(ManagerPowerState)> fn)
{
	_onPowerStateChange = std::bind(fn, std::placeholders::_1);
}

// void SequenceManager::onPoweroff(std::function<void(void)> fn)
//{
//	_onPoweroffCallback = std::bind(fn);
// }

// void SequenceManager::setRandomCycleDuration(int minDurationSeconds, int maxDurationSeconds)
//{
//	_randomCycleMinDurationSeconds = minDurationSeconds;
//	_randomCycleMaxDurationSeconds = maxDurationSeconds;
// }

// void SequenceManager::getNewRandomSequence()
//{
//	_playlistCycleEnabled = false;
//	_playlistCycleSecondCounter = 0;
//
//	_playlistCycleDuration = random16(_randomCycleMinDurationSeconds, _randomCycleMaxDurationSeconds);
//	Serial.printf("New Random Cycle Duration: %d seconds.\n", _playlistCycleDuration);
//
//	//SequenceName test = static_cast<SequenceName>(rand() % SequenceName::NUM_ITEMS);
//	SequenceName seq = static_cast<SequenceName>(random8(SequenceName::NUM_ITEMS - 1));
//
//	this->setSequence(seq);
//
//	_playlistCycleEnabled = true;
// }

// void SequenceManager::onStatusChange(FuncPtrBoolInt callback)
//{
//	_callbackStatusFunction = &callback;
//	//callback(0);
// }

// SequenceManager& SequenceManager::onMessage(AsyncMqttClientInternals::OnMessageUserCallback callback)
//{
//	_onMessageUserCallbacks.push_back(callback);
//	return *this;
// }

// SequenceManager& SequenceManager::onStatusChange(SequenceManagerInternals::OnMessageUserCallback callback)
//{
//	// TODO: insert return statement here
//	_onMessageUserCallbacks.push_back(callback);
//	return *this;
// }
