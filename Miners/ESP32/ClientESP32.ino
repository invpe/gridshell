/*
   Example integration with the GRIDSHELL
   ESP does idle and takes load from Grid Server if available
   https://www.gridshell.net/
   https://github.com/invpe/gridshell

*/
#include <WiFi.h>
#include <HTTPClient.h>
#include "CGridShell.h"

////////////////////////////////
// Your WiFi Credentials here //
////////////////////////////////
#define WIFI_SSID ""
#define WIFI_PWD  ""

////////////////////////////////
// Your GridShell user-hash   //
////////////////////////////////
#define GRID_USERHASH ""



void setup()
{
  //
  Serial.begin(115200);

  //
  Serial.println("Connecting to WiFi");

  // Connect to WiFi, internet required ;-)
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PWD);

  // Give it 10 seconds to connect, otherwise reboot
  uint8_t iRetries = 0;
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.println(".");
    delay(1000);
    iRetries += 1;

    if (iRetries >= 10)
      ESP.restart();
  }

  ///////////////////////////////////////////////////
  // Initialize GridShell Miner with your user hash//
  ///////////////////////////////////////////////////
  if (CGridShell::GetInstance().Init(GRID_USERHASH) == true)
  {
  }
  else
    ESP.restart();
}
void loop()
{
  static uint32_t m_uiLastHB   = 0;

  /////////////////////////////
  // Keep working on the grid//
  /////////////////////////////
  CGridShell::GetInstance().Tick();

  // Check WiFi
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("WIFI Bad, rebooting");
    ESP.restart();
  }

  // Your Normal Sketch things
  if (millis() - m_uiLastHB > 1000)
  {
    m_uiLastHB = millis();
  }
}
