#pragma once

#include <Arduino.h>
#include <ArduinoOTA.h>
#include <ESPmDNS.h>
#include <WebServer.h>
#include <WiFi.h>

#include "secrets.h"

const char *HOSTNAME = "Basement-Panel-NeoS3";
const char *hostname = "basement-panel-neos3";

WebServer restServer(80);
extern bool display;
extern void ack1Wake();
extern void ack1Clear();

void wifiSetup()
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

volatile long wifiStatusDelayMs = 0;
int wifiDisconnects = 0;
void checkWifiStatus()
{
  if (wifiStatusDelayMs < 0)
  {
    try
    {
      if (WiFi.status() != WL_CONNECTED)
      {
        Serial.println("Reconnecting to WiFi...");
        WiFi.disconnect();
        WiFi.reconnect();
        wifiDisconnects++;
        Serial.println("Reconnected to WiFi");
      }
    }
    catch (const std::exception &e)
    {
      Serial.println("Wifi error:" + String(e.what()));
      wifiStatusDelayMs = 10 * 60 * 1000; // 10 minutes
    }

    wifiStatusDelayMs = 60 * 1000; // 1 minute
  }
}

void otaSetup()
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

void mDnsSetup()
{
  if (!MDNS.begin(hostname))
  {
    log_e("Error setting up mDNS responder!");

    return;
  }
  log_i("mDNS responder started");
}

void restIndex()
{
  log_i("Serving index.html");
  restServer.send(200, "text/plain", "Basement-Panel-Xiao");
  log_i("Served index.html");
}

void restDisplay()
{
  if (restServer.hasArg("plain"))
  {
    String body = restServer.arg("plain");
    body.toLowerCase();

    if (body == "off")
    {
      display = false;
    }
    else if (body == "on")
    {
      display = true;
    }
    else
    {
      restServer.send(400, "text/plain", body);
      return;
    }
  }

  restServer.send(200, "text/plain", display ? "on" : "off");
}


void restSetup()
{
  restServer.on("/", restIndex);
  restServer.on("/display", restDisplay);
  restServer.begin();

  log_i("REST server running");
}