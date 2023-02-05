using HexawallController.Models.Enums;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.Json;
using System.Text.Json.Serialization;
using System.Threading.Tasks;

namespace HexawallController.Models
{
    public class StatusMessage
    {
        [JsonInclude]
        public CommandPowerState PowerState { get; private set; }

        [JsonInclude]
        public LedProgram LedProgram { get; private set; }

        [JsonInclude]
        public int LedBrightness { get; private set; }

        [JsonInclude]
        public string CurrentPlaylist { get; private set; }

        [JsonInclude]
        public int MinPlaylistDuration { get; private set; }

        [JsonInclude]
        public int MaxPlaylistDuration { get; private set; }


        public DateTime Timestamp { get; private set; }
        public string Json { get; private set; }

        public StatusMessage() { }

        public StatusMessage(string message)
        {
            var msg = JsonSerializer.Deserialize<StatusMessage>(message, new JsonSerializerOptions
            {
                IncludeFields = true,
                PropertyNameCaseInsensitive = true,
                Converters = { new JsonStringEnumConverter(JsonNamingPolicy.CamelCase) }
            });

            PowerState = msg.PowerState;
            LedProgram = msg.LedProgram;
            LedBrightness = msg.LedBrightness;

            CurrentPlaylist = msg.CurrentPlaylist;
            if(msg.CurrentPlaylist == "")
            {
                CurrentPlaylist ="No Playlist";
            }

            MinPlaylistDuration = msg.MinPlaylistDuration;
            MaxPlaylistDuration = msg.MaxPlaylistDuration;

            Timestamp = DateTime.Now;
            Json = message;
        }
    }
}
