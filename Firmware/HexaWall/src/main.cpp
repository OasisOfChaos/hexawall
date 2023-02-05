#include <Arduino.h>

using namespace std;

#include "settings.hpp"
#include <WiFi.h>
#include "Beeper.hpp"
#include "SequenceManager.hpp"
#include "MQTT.hpp"

SequenceManager sequenceManager;
TimerHandle_t wifiReconnectTimer;
TaskHandle_t doSequenceManagerTaskHandle;
MQTT mqttManager;

CommandPowerState currentPowerState = CommandPowerState::PowerOff;

//function declarations
void Beep(uint8_t pattern);
void doSequenceManagerTask(void *pvParameters);
void setPowerStateFromSeqManager(ManagerPowerState state);
void setPowerStateFromMQTTManager(CommandPowerState state);
void setPowerState(CommandPowerState state, bool notifySeqManager);
void controlPowerIO(bool state);
void WiFiEvent(WiFiEvent_t event);
void startWifi(void);
void checkButtonState(void);
void publishStatusDirect(ManagerStatusTopic topic);
void initIO(void);
void onRebootRequest(void);
void powerStateChangeRequest(CommandPowerState state);


void Beep(uint8_t pattern)
{
  auto beeper = std::unique_ptr<Beeper>(new Beeper(PIN_BEEPER));

  switch (pattern)
  {
  case 0:
    beeper->beep(100, 100);
    beeper->beep(100);
    break;

  case 1:
    beeper->beep(100, 100);
    beeper->beep(100, 100);
    beeper->beep(400);
    break;

  case 2:
    beeper->beep(400, 100);
    beeper->beep(100, 100);
    beeper->beep(100);
    break;

  case 4:
    beeper->beep(100);
  }
}

void doSequenceManagerTask(void *pvParameters)
{
  PRINT_INFO("doSequenceManager: Executing on core: %d.\n", xPortGetCoreID());

  while (true)
  {
    sequenceManager.tick();
  }
}

void setPowerStateFromSeqManager(ManagerPowerState state)
{

  PRINT_INFO("setPowerStateFromSeqManager(): %s\n", state._to_string());

  switch (state)
  {
  case ManagerPowerState::ManagerPowerOff:
    setPowerState(CommandPowerState::PowerOff, false);

    //switch off main power
    controlPowerIO(LOW);
    break;

  case ManagerPowerState::ManagerPoweringOff:
    break;

  case ManagerPowerState::ManagerPowerOn:
    setPowerState(CommandPowerState::PowerOn, false);
    break;

  case ManagerPowerState::ManagerPoweringOn:
    break;

  case ManagerPowerState::ManagerGoingToStandby:
    break;

  case ManagerPowerState::ManagerStandby:
    setPowerState(CommandPowerState::Standby, false);
    break;

  default:
    Beep(0);
    PRINT_INFO("ManagerPowerState: UNKNOWN!\n");
    break;
  }
}

void setPowerStateFromMQTTManager(CommandPowerState state)
{
  setPowerState(state, true);
}

void setPowerState(CommandPowerState state, bool notifySeqManager)
{
  if (currentPowerState == state)
  {
    PRINT_INFO("setPowerState(): ignoring request for '%s' because we're in that state already.\n", state._to_string());
    return;
  }

  currentPowerState = state;

  switch (state)
  {
  case CommandPowerState::PowerOn:
    controlPowerIO(HIGH);
    Beep(4);
    break;

  case CommandPowerState::PowerOff:
    Beep(4);
    break;

  case CommandPowerState::Standby:
    controlPowerIO(HIGH);
    break;

  default:
    Beep(0);
    PRINT_INFO("New powerstate: UNKNOWN!\n");
    break;
  };

  PRINT_INFO("setPowerState(): New powerstate: %s\n", state._to_string());

  if (notifySeqManager)
  {
    sequenceManager.setPowerState(state);
  }
}

void controlPowerIO(bool state)
{
  digitalWrite(PIN_PS_ON, state);
  digitalWrite(PIN_LED_PS_ON, state);
  digitalWrite(PIN_POWER_RELAY, state);

  PRINT_DEBUG("New physical power state: %d.\n", state);
}

void connectToWifi()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  // Serial.print("Connecting to WiFi...");
  // while (WiFi.status() != WL_CONNECTED)
  //{
  //	Serial.print('.');
  //	delay(1000);
  // }
  // Sprintln("");
  PRINT_INFO("Connecting to WiFi...\n");
}

void WiFiEvent(WiFiEvent_t event)
{
  // Sprintf("[WiFi-event] event: %d\n", event);

  switch (event)
  {
  case SYSTEM_EVENT_STA_GOT_IP:
    PRINT_INFO("WiFi connected. IP address: %s\n", WiFi.localIP().toString().c_str());
    /*Serial.print("IP address: ");
    Sprintln(WiFi.localIP());*/
    mqttManager.connect();
    break;

  case SYSTEM_EVENT_STA_DISCONNECTED:
    PRINT_INFO("WiFi lost connection.\n");

    // ensure we don't reconnect to MQTT while reconnecting to Wi-Fi
    mqttManager.stopReconnectTimer();
    // xTimerStop(mqttReconnectTimer, 0); // ensure we don't reconnect to MQTT while reconnecting to Wi-Fi

    xTimerStart(wifiReconnectTimer, 0);
    break;
  }
}

void startWifi(void)
{
  // mqttReconnectTimer = xTimerCreate("mqttTimer", pdMS_TO_TICKS(2000), pdFALSE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(connectToMqtt));
  wifiReconnectTimer = xTimerCreate("wifiTimer", pdMS_TO_TICKS(2000), pdFALSE, (void *)0, reinterpret_cast<TimerCallbackFunction_t>(connectToWifi));

  WiFi.onEvent(WiFiEvent);

  // initMqtt();
  connectToWifi();
}

void checkButtonState(void)
{
  if (digitalRead(PIN_PUSHBUTTON1) == 0)
  {
    // debounce
    delay(1000);

    if (digitalRead(PIN_PUSHBUTTON1) == 0)
    {
      if (currentPowerState == +CommandPowerState::PowerOn)
      {
        setPowerState(CommandPowerState::PowerOff, true);
      }
      else
      {
        setPowerState(CommandPowerState::PowerOn, true);
      }
    }
  }
}

void publishStatusDirect(ManagerStatusTopic topic)
{
  if (!mqttManager.isInitialized() || !mqttManager.isConnected())
  {
    return;
  }

  PRINT_DEBUG("Received a publishStatus for topic '%s'.\n", topic._to_string());

  switch (topic)
  {
  case ManagerStatusTopic::StatusTopicPowerState:
    mqttManager.publishDeviceState();
    break;
  case ManagerStatusTopic::StatusTopicBrightness:
    mqttManager.publishLEDBrightness();
    break;
  case ManagerStatusTopic::StatusTopicSequence:
    mqttManager.publishLEDProgram();
    break;
  case ManagerStatusTopic::StatusTopicSolidColor:
    mqttManager.publishLEDRGBColor();
    break;
  case ManagerStatusTopic::StatusTopicPilotLight:
    mqttManager.publishPilotLight();
    break;
  case ManagerStatusTopic::StatusTopicPlaylist:
    mqttManager.publishActivePlaylist();
    break;
  case ManagerStatusTopic::StatusTopicPlaylistDuration:
    mqttManager.publishLEDProgramDuration();
    break;
  case ManagerStatusTopic::StatusTopicInternalLEDBlink:
    mqttManager.publishOSBlinkLEDState();
    break;
  default:
    break;
  }
}

void initIO(void)
{
  controlPowerIO(false);

  pinMode(PIN_PS_ON, OUTPUT);
  pinMode(PIN_LED_PS_ON, OUTPUT);
  pinMode(PIN_POWER_RELAY, OUTPUT);
  pinMode(PIN_PUSHBUTTON1, INPUT_PULLUP);
}

void onRebootRequest(void)
{
  ESP.restart();
}

void powerStateChangeRequest(CommandPowerState state)
{
  PRINT_INFO("MQTT powerStateChangeRequest for state: %d\n", state);
}

void setup(void)
{
  Serial.begin(115200);

  PRINT_DEBUG("\nCompiler version:%ld\n", __cplusplus);
  PRINT_INFO("Setup/Loop: Executing on core: %d.\n", xPortGetCoreID());

  initIO();

  startWifi();

  sequenceManager.onStatusChange(publishStatusDirect);
  sequenceManager.onPowerStateChange(setPowerStateFromSeqManager);

  mqttManager.onPowerStateChange(setPowerStateFromMQTTManager);
  mqttManager.onRebootRequest(onRebootRequest);

  sequenceManager.init();

  mqttManager.init(&sequenceManager);

  // set random seed to a 'real' random number
  random16_set_seed(esp_random());

  // xTaskCreatePinnedToCore(
  //	doSequenceManagerTask,   /* Function to implement the task */
  //	"doSequenceManagerTask", /* Name of the task */
  //	40000,      /* Stack size in words */
  //	NULL,       /* Task input parameter */
  //	20,          /* Priority of the task */
  //	&doSequenceManagerTaskHandle,       /* Task handle. */
  //	0);  /* Core where the task should run */

  Beep(0);

  PRINT_INFO("Intializing done. Ready.\n");

#ifdef DEVELOP_MODE
  setPowerState(CommandPowerState::PowerOn, true);

  // sequenceManager.enableInternalLEDBlink(true);
  // sequenceManager.setLEDBrightness(255, false);
  // sequenceManager.enableRandomCycle(true);

  // sequenceManager.setLEDSolidColor(CRGB(0, 0, 255));
  // sequenceManager.setLEDSolidColor(CRGB(255, 0, 0));
  // sequenceManager.setLEDBrightness(50, true);
  // sequenceManager.setSequence(SequenceName::CyclePrimaryColors);
  // sequenceManager.setSequence(SequenceName::None);
  // sequenceManager.setSequence(SequenceName::ColumnTest);

  sequenceManager.loadPlayList(1);	//all of them
  // sequenceManager.loadPlayList(2);	//flows
#endif
}

void loop(void)
{
  sequenceManager.tick();

  checkButtonState();
}
