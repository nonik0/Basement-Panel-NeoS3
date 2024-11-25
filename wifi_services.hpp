#pragma once

#include <Arduino.h>
#include <ArduinoOTA.h>
#include <ESPmDNS.h>
#include <WebServer.h>
#include <WiFi.h>

#include "secrets.h"

class WifiServices
{
private:
  WebServer _restServer;

  int _disconnectCount = 0;
  long _statusCheckDelayMs = 0;

public:
  void setup(const char *hostname);
  void start()
  {
    // xTaskCreatePinnedToCore(this->task, "WifiServices", 8192, NULL, 1, NULL, 1);
  }

private:
  void task();
  void checkWifiStatus();

  void wifiSetup();
  void otaSetup();
  void mDnsSetup();

  void restIndex();
  void restDisplay();
  void restSetup();
};

void WifiServices::setup(const char *hostname)
{
  log_i("Starting setup...");

  wifiSetup();
  mDnsSetup();
  otaSetup();
  restSetup();

  log_i("Setup complete.");
}

void WifiServices::task()
{
  while (1)
  {
    ArduinoOTA.handle();
    _restServer.handleClient();
    checkWifiStatus();
    delay(10);
  }
}

void WifiServices::wifiSetup()
{
  log_i("Wifi setting up...");
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.waitForConnectResult() != WL_CONNECTED)
  {
    log_e("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  log_i("Wifi ready, IP address: %s", WiFi.localIP().toString().c_str());
}

void WifiServices::checkWifiStatus()
{
  if (_statusCheckDelayMs < 0)
  {
    try
    {
      if (WiFi.status() != WL_CONNECTED)
      {
        Serial.println("Reconnecting to WiFi...");
        WiFi.disconnect();
        WiFi.reconnect();
        _disconnectCount++;
        Serial.println("Reconnected to WiFi");
      }
    }
    catch (const std::exception &e)
    {
      Serial.println("Wifi error:" + String(e.what()));
      _statusCheckDelayMs = 10 * 60 * 1000; // 10 minutes
    }

    _statusCheckDelayMs = 60 * 1000; // 1 minute
  }
}

void WifiServices::otaSetup()
{
  log_i("OTA setting up...");

  // ArduinoOTA.setPort(3232);
  ArduinoOTA.setHostname(HOSTNAME);

  ArduinoOTA
      .onStart([]()
               {
        String type;
        if (ArduinoOTA.getCommand() == U_FLASH)
          type = "sketch";
        else  // U_SPIFFS
          type = "filesystem";

        Serial.println("Start updating " + type); })
      .onEnd([]()
             { Serial.println("\nEnd"); })
      .onProgress([](unsigned int progress, unsigned int total)
                  { Serial.printf("Progress: %u%%\r", (progress / (total / 100))); })
      .onError([](ota_error_t error)
               {
        Serial.printf("Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR)
          log_e("Auth Failed");
        else if (error == OTA_BEGIN_ERROR)
          log_e("Begin Failed");
        else if (error == OTA_CONNECT_ERROR)
          log_e("Connect Failed");
        else if (error == OTA_RECEIVE_ERROR)
          log_e("Receive Failed");
        else if (error == OTA_END_ERROR)
          log_e("End Failed"); });
  ArduinoOTA.begin();

  log_i("OTA setup complete.");
}

void WifiServices::mDnsSetup()
{
  if (!MDNS.begin(hostname))
  {
    log_e("Error setting up mDNS responder!");

    return;
  }
  log_i("mDNS responder started");
}

void WifiServices::restIndex()
{
  log_i("Serving index.html");
  _restServer.send(200, "text/plain", HOSTNAME);
  log_i("Served index.html");
}

void WifiServices::restDisplay()
{
  if (_restServer.hasArg("plain"))
  {
    String body = _restServer.arg("plain");
    body.toLowerCase();

    if (body == "off")
    {
      display = false;
      inputTask.setDisplay(false);
    }
    else if (body == "on")
    {
      display = true;
      inputTask.setDisplay(true);
    }
    else
    {
      _restServer.send(400, "text/plain", body);
      return;
    }
  }

  _restServer.send(200, "text/plain", display ? "on" : "off");
}

void WifiServices::restSetup()
{
  // restServer.on("/", restIndex);
  // restServer.on("/display", restDisplay);
  _restServer.begin();

  log_i("REST server running");
}