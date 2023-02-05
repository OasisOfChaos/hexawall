namespace HexawallController.Models;

public static class MqttTopics
{
    public const string MQTT_TOPIC_PREFIX = "myhome/hexawall/";    //end with slash!

    public const string MQTT_LIGHT_GETVALUE_TOPIC = "myhome/hexawall/light/get";
    public const string MQTT_LIGHT_VALUE_TOPIC = "myhome/hexawall/light/value";
    public const string MQTT_LIGHT_COMMAND_TOPIC = "myhome/hexawall/light/switch";

    public const string MQTT_BRIGHTNESS_GETVALUE_TOPIC = "myhome/hexawall/brightness/get";
    public const string MQTT_BRIGHTNESS_VALUE_TOPIC = "myhome/hexawall/brightness/value";
    public const string MQTT_BRIGHTNESS_COMMAND_TOPIC = "myhome/hexawall/brightness/set";
    public const string MQTT_POWERSTATE_GETVALUE_TOPIC = "myhome/hexawall/powerstate/get";

    public const string MQTT_RGB_GETVALUE_TOPIC = "myhome/hexawall/rgb/get";
    public const string MQTT_RGB_VALUE_TOPIC = "myhome/hexawall/rgb/value";
    public const string MQTT_RGB_COMMAND_TOPIC = "myhome/hexawall/rgb/set";

    public const string MQTT_PROGRAM_GETVALUE_TOPIC = "myhome/hexawall/program/get";
    public const string MQTT_PROGRAM_VALUE_TOPIC = "myhome/hexawall/program/value";
    public const string MQTT_PROGRAM_COMMAND_TOPIC = "myhome/hexawall/program/set";
    public const string MQTT_PROGRAM_CYCLE_COMMAND_TOPIC = "myhome/hexawall/program/cycle";
    public const string MQTT_PROGRAM_DURATION_COMMAND_TOPIC = "myhome/hexawall/program/duration/set";
    public const string MQTT_PROGRAM_DURATION_VALUE_TOPIC = "myhome/hexawall/program/duration/value";

    public const string MQTT_PROGRAM_SPEED_GETVALUE_TOPIC = "myhome/hexawall/program/speed/get";
    public const string MQTT_PROGRAM_SPEED_VALUE_TOPIC = "myhome/hexawall/program/speed/value";
    public const string MQTT_PROGRAM_SPEED_COMMAND_TOPIC = "myhome/hexawall/program/speed/set";

    public const string MQTT_PLAYLIST_GETVALUE_TOPIC = "myhome/hexawall/playlist/get";
    public const string MQTT_PLAYLIST_VALUE_TOPIC = "myhome/hexawall/playlist/value";
    public const string MQTT_PLAYLIST_COMMAND_TOPIC = "myhome/hexawall/playlist/set";
    public const string MQTT_PLAYLIST_GETLIST_TOPIC = "myhome/hexawall/playlist/getlist";
    public const string MQTT_PLAYLIST_LIST_TOPIC = "myhome/hexawall/playlist/list";
    public const string MQTT_PLAYLIST_RUNTIME_GETVALUE_TOPIC = "myhome/hexawall/playlist/runtime/get";
    public const string MQTT_PLAYLIST_RUNTIME_VALUE_TOPIC = "myhome/hexawall/playlist/runtime/value";

    public const string MQTT_OS_REBOOT_COMMAND_TOPIC = "myhome/hexawall/os/reboot";

    public const string MQTT_OS_PILOTLIGHT_GETVALUE_TOPIC = "myhome/hexawall/os/pilotlight/get";
    public const string MQTT_OS_PILOTLIGHT_VALUE_TOPIC = "myhome/hexawall/os/pilotlight/value";
    public const string MQTT_OS_PILOTLIGHT_COMMAND_TOPIC = "myhome/hexawall/os/pilotlight/set";

    public const string MQTT_OS_BLINKLED_GETVALUE_TOPIC = "myhome/hexawall/os/led/get";
    public const string MQTT_OS_BLINKLED_VALUE_TOPIC = "myhome/hexawall/os/led/value";
    public const string MQTT_OS_BLINKLED_COMMAND_TOPIC = "myhome/hexawall/os/led/set";

    /// <summary>
    /// Topic list to subscribe to
    /// </summary>
    public static List<string> MQTT_STATE_TOPIC_LIST = new(new string[]
    {
        MQTT_POWERSTATE_GETVALUE_TOPIC,
        MQTT_LIGHT_VALUE_TOPIC,
        MQTT_BRIGHTNESS_VALUE_TOPIC,
        MQTT_RGB_VALUE_TOPIC,
        MQTT_PROGRAM_VALUE_TOPIC,
        MQTT_PLAYLIST_VALUE_TOPIC,
        MQTT_PLAYLIST_RUNTIME_VALUE_TOPIC,
        MQTT_OS_PILOTLIGHT_VALUE_TOPIC,
        MQTT_OS_BLINKLED_VALUE_TOPIC,
        MQTT_PROGRAM_SPEED_VALUE_TOPIC
    });

    /// Topic list to request (initial) states
    /// </summary>
    public static List<string> MQTT_GETSTATE_TOPIC_LIST = new(new string[]
    {
        MQTT_LIGHT_GETVALUE_TOPIC,
        MQTT_BRIGHTNESS_GETVALUE_TOPIC,
        MQTT_RGB_GETVALUE_TOPIC,
        MQTT_PROGRAM_GETVALUE_TOPIC,
        MQTT_PLAYLIST_GETVALUE_TOPIC,
        MQTT_PLAYLIST_RUNTIME_GETVALUE_TOPIC,
        MQTT_OS_PILOTLIGHT_GETVALUE_TOPIC,
        MQTT_OS_BLINKLED_GETVALUE_TOPIC,
        MQTT_PROGRAM_SPEED_GETVALUE_TOPIC
    });

}
