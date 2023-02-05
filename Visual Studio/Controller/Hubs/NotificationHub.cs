using System;
using System.Collections.Generic;
using System.Threading.Tasks;
using Microsoft.AspNetCore.SignalR;

namespace HexawallController.Hubs
{
    public interface IChatRepository
    {
        void Add(string name, string message);

        // Other methods not shown.
    }

    public class NotificationHub : Hub
    {
        public async Task SendMessage(string user, string message)
        {
            await Clients.All.SendAsync("Notify", user, message);
        }
    }
}
