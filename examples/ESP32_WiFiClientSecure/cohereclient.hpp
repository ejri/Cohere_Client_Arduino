#ifndef COHERECLIENT_H
#define COHERECLIENT_H

#include <WiFiClientSecure.h>

class CohereClient {
public:
  CohereClient(WiFiClientSecure* client, const String& apiKey) : client(client), apiKey(apiKey) {}

  String makeAPICall(int maxTokens, const String& prompt);
  String text_output(const String& response);
  String full_response(const String& response);

private:
  WiFiClientSecure* client;
  String apiKey;
  const char* host = "api.cohere.ai";
  const int httpsPort = 443;
};

String CohereClient::makeAPICall(int maxTokens, const String& prompt) {
  client->setInsecure();

  if (!client->connect(host, httpsPort)) {
    Serial.println("Connection failed!");
    return "";
  }

  String post_body = "{\"max_tokens\":" + String(maxTokens) + ", \"prompt\":\"" + prompt + "\"}";

  String http_request = "POST /v1/generate HTTP/1.1\r\n";
  http_request += "Host: " + String(host) + "\r\n";
  http_request += "Content-Type: application/json\r\n";
  http_request += "Content-Length: " + String(post_body.length()) + "\r\n";
  http_request += "Authorization: Bearer " + apiKey + "\r\n";
  http_request += "Connection: close\r\n";
  http_request += "\r\n";
  http_request += post_body + "\r\n";

  client->print(http_request);

  String response;
  while (client->connected()) {
    if (client->available()) {
      response += client->readStringUntil('\n');
    }
  }

  client->stop();

  return response;
}

String CohereClient::text_output(const String& response) {
  int start = response.indexOf("\"text\":\"");
  int end = response.indexOf("\"}", start);

  if (start == -1 || end == -1) {
    Serial.println("Error extracting text from response");
    return "";
  }

  String text = response.substring(start + 8, end);
  String prompt = response.substring(response.indexOf("\"prompt\":\"") + 10, response.indexOf("\"", response.indexOf("\"prompt\":\"") + 10));

  return "for the following prompt: " + prompt + "\nGenerated output:\n" + text;
}

String CohereClient::full_response(const String& response) {
  String formattedResponse = response;

  // Replace line breaks with "\n" for better visibility
  formattedResponse.replace("\r\n", "\n");

  return "Full Response:\n" + formattedResponse;
}

#endif
