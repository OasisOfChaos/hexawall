using HexawallController.Hubs;
using HexawallController.Models;
using HexawallController.Models.Enums;
using Microsoft.AspNetCore.SignalR;
using Microsoft.AspNetCore.SignalR.Client;
using MQTTnet;
using MQTTnet.Client;
using MQTTnet.Client.Connecting;
using MQTTnet.Client.Disconnecting;
using MQTTnet.Client.Options;
using MQTTnet.Exceptions;
using MudBlazor.Utilities;
using System.Text;

//https://www.emqx.com/en/blog/esp8266_mqtt_led

namespace HexawallController.Services
{
    public class MqttClientService : IMqttClientService
    {
        private readonly IMqttClient _mqttClient;
        private readonly IMqttClientOptions _options;
        private readonly IHubContext<NotificationHub> _hubContext;
        private readonly ILogger<MqttClientService> _logger;

        private CancellationToken _cancellationToken;
        private int _retryAttempts = 0;

        public IMqttClient MqttClient => _mqttClient;

        public MqttClientService(IMqttClientOptions options, IHubContext<NotificationHub> hubContext, ILogger<MqttClientService> logger)
        {
            _options = options;
            _hubContext = hubContext;
            _logger = logger;

            _mqttClient = new MqttFactory().CreateMqttClient();

            ConfigureMqttClient();
        }

        private void ConfigureMqttClient()
        {
            _mqttClient.ConnectedHandler = this;
            _mqttClient.DisconnectedHandler = this;
            _mqttClient.ApplicationMessageReceivedHandler = this;
        }

        public async Task HandleApplicationMessageReceivedAsync(MqttApplicationMessageReceivedEventArgs eventArgs)
        {
            var payload = Encoding.UTF8.GetString(eventArgs.ApplicationMessage.Payload);

            Console.WriteLine("MQTT message: topic: {0}, payload: {1}", eventArgs.ApplicationMessage.Topic, payload);

            string topic = eventArgs.ApplicationMessage.Topic.Remove(0, MqttTopics.MQTT_TOPIC_PREFIX.Length);
            string method = "Notify";
            foreach (var s in topic.Split("/"))
            {
                method += UppercaseFirst(s);
            }

            //string method = "Notify" + UppercaseFirst(eventArgs.ApplicationMessage.Topic[(eventArgs.ApplicationMessage.Topic.IndexOf('/') + 1)..]);

            await _hubContext.Clients.All.SendAsync(method, eventArgs.ApplicationMessage.Topic, payload);

            //await Task.FromResult(0);
        }

        static string UppercaseFirst(string s)
        {
            // Check for empty string
            if (string.IsNullOrEmpty(s))
            {
                return string.Empty;
            }
            // Return char and concat substring
            return char.ToUpper(s[0]) + s[1..];
        }

        public async Task HandleConnectedAsync(MqttClientConnectedEventArgs eventArgs)
        {
            Console.WriteLine($"Connected. Subscribing to {MqttTopics.MQTT_STATE_TOPIC_LIST.Count} topics...");
            foreach (string topic in MqttTopics.MQTT_STATE_TOPIC_LIST)
            {
                await _mqttClient.SubscribeAsync(topic);
            }
        }

        public async Task HandleDisconnectedAsync(MqttClientDisconnectedEventArgs eventArgs)
        {
#pragma warning disable CS0618 // Type or member is obsolete
            if (eventArgs.ReasonCode != MqttClientDisconnectReason.NormalDisconnection)
            {
                try
                {
                    _logger.LogInformation("MQTT Client reconnecting...");
                    var seconds = Math.Pow(2, _retryAttempts);
                    var maxSeconds = Math.Max(seconds, 60);
                    var timeToWait = TimeSpan.FromSeconds(maxSeconds);
                    await Task.Delay(timeToWait, _cancellationToken);
                    await StartAsync(_cancellationToken);
                    _retryAttempts = 0;
                }
                catch (Exception ex)
                {
                    _retryAttempts++;
                    _logger.LogError("An MQTT exception occurred: {Exception}.", ex);
                }
            }
#pragma warning restore CS0618 // Type or member is obsolete

            await Task.FromResult(0);
        }

        public async Task StartAsync(CancellationToken cancellationToken)
        {
            _cancellationToken = cancellationToken;

            await _mqttClient.ConnectAsync(_options, _cancellationToken);
            if (!_mqttClient.IsConnected)
            {
                await _mqttClient.ReconnectAsync(_cancellationToken);
            }
        }

        public async Task StopAsync(CancellationToken cancellationToken)
        {
            if (cancellationToken.IsCancellationRequested)
            {
                var disconnectOption = new MqttClientDisconnectOptions
                {
                    ReasonCode = MqttClientDisconnectReason.NormalDisconnection,
                    ReasonString = "NormalDisconnection"
                };
                await _mqttClient.DisconnectAsync(disconnectOption, cancellationToken);
            }

            await _mqttClient.DisconnectAsync(cancellationToken);
        }


        // COMMANDS
        public async Task SetPowerState(CommandPowerState state)
        {
            switch (state)
            {
                case CommandPowerState.PowerOff:
                    await SendMqttMessageAsync(MqttTopics.MQTT_LIGHT_COMMAND_TOPIC, "OFF");
                    break;
                case CommandPowerState.PowerOn:
                    await SendMqttMessageAsync(MqttTopics.MQTT_LIGHT_COMMAND_TOPIC, "ON");
                    break;
                case CommandPowerState.Standby:
                    await SendMqttMessageAsync(MqttTopics.MQTT_LIGHT_COMMAND_TOPIC, "STANDBY");
                    break;
            }
        }

        public async Task SetLEDProgram(LedProgram program)
        {
            string payload = ((int)program).ToString();
            await SendMqttMessageAsync(MqttTopics.MQTT_PROGRAM_COMMAND_TOPIC, payload);
        }

        public async Task SetLEDSolidProgramColor(MudColor color)
        {
            string payload = $"{color.R:d2},{color.G:d2},{color.B:d2}";
            await SendMqttMessageAsync(MqttTopics.MQTT_RGB_COMMAND_TOPIC, payload);
        }

        public async Task SetLEDBrightness(int brightness)
        {
            string payload = brightness.ToString();
            await SendMqttMessageAsync(MqttTopics.MQTT_BRIGHTNESS_COMMAND_TOPIC, payload);
        }

        public async Task SetLEDProgramDelay(int delay)
        {
            string payload = delay.ToString();
            await SendMqttMessageAsync(MqttTopics.MQTT_PROGRAM_SPEED_COMMAND_TOPIC, payload);
        }

        public async Task LoadPlaylist(int index)
        {
            string payload = index.ToString();
            await SendMqttMessageAsync(MqttTopics.MQTT_PLAYLIST_COMMAND_TOPIC, payload);
        }

        public async Task RebootEsp()
        {
            await SendMqttMessageAsync(MqttTopics.MQTT_OS_REBOOT_COMMAND_TOPIC, "");
        }

        public async Task EnableProgramCycle(bool enable)
        {
            var payload = (enable ? 1 : 0).ToString();
            await SendMqttMessageAsync(MqttTopics.MQTT_PROGRAM_CYCLE_COMMAND_TOPIC, payload);
        }

        public async Task SetLEDBlinkState(bool enable)
        {
            string payload = (enable ? 1 : 0).ToString();
            await SendMqttMessageAsync(MqttTopics.MQTT_OS_BLINKLED_COMMAND_TOPIC, payload);
        }

        public async Task SetProgramDuration(int minDuration, int maxDuration)
        {
            string payload = $"{minDuration},{maxDuration}";
            await SendMqttMessageAsync(MqttTopics.MQTT_PROGRAM_DURATION_COMMAND_TOPIC, payload);
        }


        //PUBLISH
        private async Task SendMqttMessageAsync(string topic, string? payload, bool retainFlag = false)
        {
            try
            {
                var message = new MqttApplicationMessageBuilder()
                    .WithTopic(topic)
                    //.WithPayload(payload)
                    .WithExactlyOnceQoS()
                    //.WithRetainFlag()
                    .Build();

                if (payload != null)
                {
                    message.Payload = Encoding.UTF8.GetBytes(payload);
                }

                message.Retain = retainFlag;

                await _mqttClient.PublishAsync(message, CancellationToken.None);
            } catch (MqttCommunicationException cex) {
                if (!_mqttClient.IsConnected)
                {
                    await _mqttClient.ReconnectAsync(_cancellationToken);
                }
            } catch
            {
                throw;
            }
        }

        public async Task RequestStatus()
        {
            //await SendRequestAsync("getstatus");
            foreach (string topic in MqttTopics.MQTT_GETSTATE_TOPIC_LIST)
            {
                await SendMqttMessageAsync(topic, null);
            }
        }

        public async Task RequestPlaylists()
        {
            await SendMqttMessageAsync(MqttTopics.MQTT_PLAYLIST_GETLIST_TOPIC, null);
        }

        public async Task CycleProgram()
        {
            await SendMqttMessageAsync(MqttTopics.MQTT_PROGRAM_CYCLE_COMMAND_TOPIC, null);
        }
    }
}

