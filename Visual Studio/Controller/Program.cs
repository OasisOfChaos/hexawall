using HexawallController.Data;
using MudBlazor.Services;
using Microsoft.AspNetCore.ResponseCompression;
using HexawallController.Hubs;
using HexawallController.Settings;
using Mqtt.Client.AspNetCore.Extensions;

var builder = WebApplication.CreateBuilder(args);

builder.Host.ConfigureLogging(logging =>
{
    logging.ClearProviders();
    //logging.AddConsole();
    logging.AddSystemdConsole(options =>
    {
        options.IncludeScopes = false;
    });
});

builder.Configuration.AddJsonFile("appsettings.json");

//MQTT Broker settings
BrokerHostSettings brokerHostSettings = new();
builder.Configuration.GetSection(nameof(BrokerHostSettings)).Bind(brokerHostSettings);
AppSettingsProvider.BrokerHostSettings = brokerHostSettings;

//MQTT Client settings
ClientSettings clientSettings = new();
builder.Configuration.GetSection(nameof(ClientSettings)).Bind(clientSettings);
AppSettingsProvider.ClientSettings = clientSettings;

// Add services to the container.
builder.Services.AddRazorPages();
builder.Services.AddServerSideBlazor();
builder.Services.AddMqttClientHostedService();
builder.Services.AddSingleton<WeatherForecastService>();
builder.Services.AddMudServices();
builder.Services.AddResponseCompression(opts =>
{
    opts.MimeTypes = ResponseCompressionDefaults.MimeTypes.Concat(
        new[] { "application/octet-stream" });
});

var app = builder.Build();

app.UseResponseCompression();

// Configure the HTTP request pipeline.
if (!app.Environment.IsDevelopment())
{
    app.UseExceptionHandler("/Error");
    // The default HSTS value is 30 days. You may want to change this for production scenarios, see https://aka.ms/aspnetcore-hsts.
    app.UseHsts();
}

app.UseHttpsRedirection();

app.UseStaticFiles();

app.UseRouting();

app.MapBlazorHub();
app.MapHub<NotificationHub>("/notificationhub");
app.MapFallbackToPage("/_Host");

app.Run();