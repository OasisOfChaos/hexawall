using HexawallController.Models.Enums;
using MQTTnet.Client.Connecting;
using MQTTnet.Client.Disconnecting;
using MQTTnet.Client.Receiving;
using MudBlazor.Utilities;

namespace HexawallController.Services
{
    public interface IMqttClientService : IHostedService,
                                          IMqttClientConnectedHandler,
                                          IMqttClientDisconnectedHandler,
                                          IMqttApplicationMessageReceivedHandler
    {
        Task SetPowerState(CommandPowerState state);
        Task SetLEDProgram(LedProgram ledProgram);
        Task SetLEDSolidProgramColor(MudColor color);
        Task SetLEDBrightness(int brightness);
        Task SetLEDProgramDelay(int delay);
        Task SetLEDBlinkState(bool state);
        Task RebootEsp();
        Task RequestStatus();
        Task RequestPlaylists();
        Task EnableProgramCycle(bool enable);
        Task LoadPlaylist(int index);
        Task SetProgramDuration(int minDuration, int maxDuration);
        Task CycleProgram();
    }
}
