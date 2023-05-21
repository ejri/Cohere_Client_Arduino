/*
 * Project Cohere Client For ESP32
 * Description: For HTTPS connection using WiFiClientSecure
 * Author: Ibrahim Elchami
 * Repo: https://github.com/ejri/Cohere_Client_Arduino
 * Date: May 20, 2023
 */

#include <WiFiClientSecure.h>
#include "cohereclient.hpp"

#define WIFI_SSID "WIFI_SSID"
#define WIFI_PW "WIFI_PW"
#define YOUR_API_KEY "YOUR_API_KEY"

WiFiClientSecure client;
CohereClient cohereClient(&client, YOUR_API_KEY);
int max_tokens = 200;
String prompt = "Once upon a time in a magical land called";

void setup()
{
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PW);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
}

void loop()
{
  String response = cohereClient.makeAPICall(max_tokens, prompt);
  String output = cohereClient.text_output(response);
  Serial.println(output);
  delay(5000);

  Serial.println("");

  String fullResponse = cohereClient.full_response(response);
  Serial.println(fullResponse);
  delay(5000);
}
