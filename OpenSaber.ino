//bomberfish

// OpenSaber: Open-source DIY lightsaber

#include <Arduino.h> // not sure if we need this
#include <FastLED.h> // led controls
#include <AudioFileSourceSPIFFS.h> // spiffs audiofile source
#include <AudioGeneratorWAV.h> // .wav handler
#include <AudioOutputI2SNoDAC.h> // output
//#include <ESP8266WiFi.h> //wifi
//#include <ESPAsyncTCP.h> //tcp
//#include <ESPAsyncWebSrv.h> //webserver

//// CONFIG ////

//wifi
//const char* ssid = "no";
//const char* password = "no";

#define LED_PIN 5 // what your strip is connected to
#define LED_COUNT 60 // how many leds your strip has
#define BRIGHTNESS 64
#define CONTROLLER_TYPE WS2811 // THE IMPORTANT PART: what controller chip your strip uses. note that `WS2811` should also work for the ws2812/ws2812b. See https://github.com/FastLED/FastLED/wiki/Overview#3-wire-chipsets for supported chipsets.
#define COLOR_FORMAT GRB // what color format your strip uses. can be something like rgb, rgbw, etc etc etc. the most common ws2812b-based strips usually use grb
#define COLOR 0x00ff00 // color of your saber in hex format (0xrrggbb), not dependent on strip color format.

CRGB leds[LED_COUNT];

AudioGeneratorWAV *wav;
AudioFileSourceSPIFFS *file;
AudioOutputI2SNoDAC *out;

const int output = 2;
String sliderValue = "0";
const char* PARAM_INPUT = "color";
//const char html[] PROGMEM = R"rawliteral(
//<!DOCTYPE html><html lang=en><meta charset=UTF-8><meta content="width=device-width,initial-scale=1"name=viewport><meta content=#ffffff name=theme-color media="(prefers-color-scheme: light)"><meta content=#000000 name=theme-color media="(prefers-color-scheme: dark)"><title>OpenSaber Connect</title><style>:root{--fg:#000000;--bg:#ffffff}@media (prefers-color-scheme:dark){:root{--fg:#ffffff;--bg:#000000}}body{background:var(--bg);color:var(--fg);font-family:-apple-system,system-ui,BlinkMacSystemFont,'Segoe UI Variable','Segoe UI','Roboto Flex',Roboto,Oxygen-Sans,Ubuntu,Cantarell,'Helvetica Neue',Helvetica,Arial,'Noto Sans',sans-serif,'Apple Color Emoji','Segoe UI Emoji','Segoe UI Symbol'}button,input[type=text]{appearance:none;border:1px solid var(--fg);background:var(--bg);color:var(--fg);border-radius:8px;padding:7px}</style><h2>OpenSaber Connect</h2><p>Enter Color: <span style="border:1px #888"><input class=slider id=colorfield step=1 value=%COLOR%></span></p><button id=submit onclick=sendColor()>Send</button><script>function sendColor(){var e=document.getElementById("colorfield").value;console.log(e);var o=new XMLHttpRequest;o.open("GET","/setcolor?color="+e,!0),o.send()}</script>
//)rawliteral";

//AsyncWebServer server(80);

void setup() {
  // put your setup code here, to run once:
  FastLED.clear();
  Serial.begin(9600);
  Serial.println("OpenSaber 0.1");

  Serial.println("Initializing SPIFFS...");
  SPIFFS.begin();

  Serial.println("Initializing Audio...");
  out = new AudioOutputI2SNoDAC();
  wav = new AudioGeneratorWAV(); 
  file = new AudioFileSourceSPIFFS("/on.wav"); 
  
  delay(3000); // wait three seconds before first start
  Serial.println("Initializing LEDs...");
  FastLED.addLeds<CONTROLLER_TYPE, LED_PIN, COLOR_FORMAT>(leds, LED_COUNT).setCorrection(TypicalLEDStrip);
  Serial.println("Done!");
  Serial.println("Playing sound and loading hum...");
  wav->begin(file, out);
  file = new AudioFileSourceSPIFFS("/hum.wav"); 
  Serial.println("Playing startup animation...");
  startanim(COLOR);
  //playwav("/hum.wav");
  // Connect to Wi-Fi
  /*
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.println("Sending html!");
    request->send_P(200, "text/html", html, processor);
  });

  // Send a GET request to <ESP_IP>/setcolor?color=<inputMessage>
  server.on("/setcolor", HTTP_GET, [] (AsyncWebServerRequest *request) {
    Serial.println("Got Message!");
    String inputMessage;
    // GET input1 value on <ESP_IP>/setcolor?color=<inputMessage>
    if (request->hasParam(PARAM_INPUT)) {
      inputMessage = request->getParam(PARAM_INPUT)->value();
      sliderValue = inputMessage;
      startanim(sliderValue.toInt());
    }
    else {
      inputMessage = "No message sent";
    }
    Serial.println(inputMessage);
    request->send(200, "text/plain", "OK");
  });
  
  // Start server
  server.begin();
  */
}

void loop() {
  // put your main code here, to run repeatedly:
  if (wav->isRunning()) {
    if (!wav->loop()) wav->stop(); 
  } else {
    //Serial.printf("Audio file done\n");
    file = new AudioFileSourceSPIFFS("/hum.wav"); 
    wav->begin(file, out);
    //delay(1000);
  }

  //just realized audio would break this...
  //if (Serial.available() > 0) {
    //String cmd = Serial.readString();  
    //handlecmd(cmd);
  //}
}

void handlecmd(String cmd) {
  cmd.trim();
  if (cmd == "red") {
    Serial.println("Setting color to red");
    startanim(0xFF0000);
  } else if (cmd == "green") {
    Serial.println("Setting color to green");
    startanim(0x00FF00);
  } else if (cmd == "blue") {
    Serial.println("Setting color to blue");
    startanim(0x0000FF);
  } else if (cmd == "gaysaber") {
    gaysaber();
  } else {
    Serial.println("Unrecognized command");
  }
}

// startup animation :3
void startanim(int color) {
  FastLED.clear();
  Serial.println(color);
  for(int current = 0.00; current < LED_COUNT / 2; current++) {
    //serial prints commented out to normalize anim speed and save on them precious cpu cycles
    //Serial.print("Lighting LEDS ");
    //Serial.print(current);
    //Serial.print(", ");
    //Serial.print(LED_COUNT - current);
    //Serial.println();
    leds[current] = color;
    leds[LED_COUNT - current] = color;
    double current_double = current; // super janky fix
    double count_double = LED_COUNT;
    double brightness = current_double / count_double;
    //Serial.print("Setting Brightness to ");
    //Serial.print(brightness);
    //Serial.println();
    FastLED.setBrightness(255 * brightness);
    FastLED.show();
    FastLED.delay(7);
  } 
}

void gaysaber() {
  FastLED.clear();
  static uint8_t hue = 0;
  while (true) {
    FastLED.showColor(CHSV(hue++, 255, 255)); 
    delay(7);
  }
}

// Replaces placeholder with button section in your web page
String processor(const String& var){
  //Serial.println(var);
  if (var == "SLIDERVALUE"){
    return sliderValue;
  }
  return String();
}
