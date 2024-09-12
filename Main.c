#include <Arduino.h>
#if defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#endif
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <WiFiClient.h>

const char* kSsid = "NAME_OF_YOUR_WIFI";
const char* kPassword = "PASSWORD";
MDNSResponder mdns;

#if defined(ESP8266)
ESP8266WebServer server(80);
#undef HOSTNAME
#define HOSTNAME "TechPosts IR Server"
#endif  // ESP8266

const uint16_t kIrLed = 4;  // ESP GPIO pin to use. Recommended: 4 (D2).

IRsend irsend(kIrLed);

void handleRoot() {
  server.send(200, "text/html",
              "<html>" \
                "<head><title>" Server " Demo </title>" \
                "<meta http-equiv=\"Content-Type\" " \
                    "content=\"text/html;charset=utf-8\">" \
                "</head>" \
                "<body>" \
                  "<h1>This's site for you " Server ".</h1>" \
                      "<p><a href=\"ir\">Foon btn</a></p>" \
                "</body>" \
              "</html>");
}

void foon() {
  //Your code
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++)
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  server.send(404, "text/plain", message);
}

void setup(void) {
  irsend.begin();

  Serial.begin(115200);
  WiFi.begin(kSsid, kPassword);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(kSsid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP().toString());

#if defined(ESP8266)
  if (mdns.begin(HOSTNAME, WiFi.localIP())) {
#else  // ESP8266
  if (mdns.begin(HOSTNAME)) {
#endif  // ESP8266
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);
  server.on("/ir", foon); //href to your func

  server.on("/inline", [](){
    server.send(200, "text/plain", "this works as well");
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
}
