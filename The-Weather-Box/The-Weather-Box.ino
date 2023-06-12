/*
The Weather Box is a device that utilizes wifi connectivity to retrieve the current weather conditions at a specified location, 
determined by the longitude and latitude entered. 
The device then displays a customized light show corresponding to the current weather condition.
Blue - for a rainy day
Gray white - for cloudy days
Yellow- for a sunny/clear day
Pink - for a snowy day
when is cloudy the box will preset white lights!

The program utilizes an ESP8266 modem to connect the CPX to wifi, and once connected, 
it sends a request to the website https://openweathermap.org/ to obtain the requested API data 
based on the specified longitude and latitude. 
The program then converts the weather data, in json format, into a string by reading it letter by letter. 
It then extracts the main weather condition from the string and subsequently displays a corresponding color light show based on the condition..

  The Circuit:
  
  *ESP8266 WiFi Module Interfacing with the cpx.
  *One wire is connected to the pin A1 on one side, and to a conductive sticker on the other side. 
  When the conductive sticker is touched, it completes a circuit and causes the light show to stop. 
  *The program also uses 10 neopixels to display the light show according to the weather conditions.

  Video link: 
  https://www.youtube.com/watch?v=788kr_7DwH4&ab_channel=BarEfrima
  Created By:
  Bar Efrima 208236604 and Tal Barda 206585614
*/





#define TINY_GSM_MODEM_ESP8266
#include <iostream>
#include <string.h>

using namespace std;
#define SerialMon Serial
#define SerialAT Serial1

#if !defined(TINY_GSM_RX_BUFFER)
#define TINY_GSM_RX_BUFFER 650
#endif

// Define the serial console for debug prints, if needed
#define TINY_GSM_DEBUG SerialMon

// Range to attempt to autobaud
#define GSM_AUTOBAUD_MIN 9600
#define GSM_AUTOBAUD_MAX 115200

// Your WiFi connection credentials, if applicable
const char wifiSSID[]  = "RUNI-Wireless";
const char wifiPass[] = "";
 
int brightness = 0;  
int fadeAmount = 5; 
int sensorValue1 = 0;
String condition = "";

// Our api key 
const char server[] = "api.openweathermap.org";
String apiKey= "5efc2b39a8e5fe8d9bb06b8c8e3542be";

#include <Adafruit_CircuitPlayground.h>
#include <TinyGsmClient.h>

#ifdef DUMP_AT_COMMANDS
#include <StreamDebugger.h>
StreamDebugger debugger(SerialAT, SerialMon);
TinyGsm modem(debugger);
#else
TinyGsm modem(SerialAT);
#endif

#ifdef USE_SSL
TinyGsmClientSecure client(modem);
const int  port = 443;
#else
TinyGsmClient client(modem);
const int  port = 80;
#endif
 String output = "";
 
void setup() {
  SerialMon.begin(115200);
  CircuitPlayground.begin();
  delay(1000);
  SerialAT.begin(115200);
  modem.restart();
  String modemInfo = modem.getModemInfo();
  
  // Wifi connection parameters must be set before waiting for the network
  SerialMon.print(F("Setting SSID/password..."));
  if (!modem.networkConnect(wifiSSID, wifiPass)) {
    SerialMon.println(" fail");
    delay(5000);
    return;
  }
  SerialMon.println(" success");
  SerialMon.print("Waiting for network...");
  if (!modem.waitForNetwork()) {
    SerialMon.println(" fail");
    delay(5000);
    return;
  }
  SerialMon.println(" success");

  if (modem.isNetworkConnected()) {
    SerialMon.println("Network connected");
  }

  SerialMon.print("Connecting to ");
  SerialMon.println(server);
  if (!client.connect(server, port)) {
    SerialMon.println(" fail");
    delay(10000);
    return;
  }
  SerialMon.println(" success");

  // Herzeliya longitude and latitude
  String lat = "32.162413";
  String lon ="34.844675";
  
  //our request api
  //https://api.openweathermap.org/data/2.5/weather?lat=32.090112&lon=34.774795&appid=5efc2b39a8e5fe8d9bb06b8c8e3542be
  
  // Make a HTTP GET request:
  SerialMon.println("Performing HTTP GET request...");
  client.print(String("GET /data/2.5/weather?lat=") + lat + "&lon=" + lon + "&appid=" + apiKey + "&units=metric"); 
  client.print(String("Host: ") + server + "\r\n");
  client.print("User-Agent: ArduinoWifi/1.1");
  client.println();
  SerialMon.println("START");
  uint32_t timeout = millis();
  while (client.connected() && millis() - timeout < 10000L) {
    // Print available data
    while (client.available()) {
      char c = client.read();
      output = output + c;
      SerialMon.print(c);
      timeout = millis();
    }
  }
  // getting the currrent condition in Herzeliya
   int pos = output.indexOf("main");
   String text_sub = output.substring(pos+7);
   int pos2 = text_sub.indexOf(",");
   condition = text_sub.substring(0,pos2-1);
   SerialMon.println();
   SerialMon.println("FINISH");

  // Shutdown

  client.stop();
  SerialMon.println(F("Server disconnected"));

  modem.networkDisconnect();
  SerialMon.println(F("WiFi disconnected"));
}

void loop() {
  //checking sensor value when touched
  // returning fading led color according to the current condition in Herzeliya
  sensorValue1 = CircuitPlayground.readCap(A1);
  if(sensorValue1 < 1015 ){
      //Rain
    if(condition == "Rain" ){
        for (int i = 0; i < 10; i++) {
        CircuitPlayground.setPixelColor(i, 0,0,brightness);
        brightness = brightness + fadeAmount;
          if (brightness <= 0 || brightness >= 255) {
            fadeAmount = -fadeAmount;
          }
        }
        delay(100);
    }
    // CircuitPlayground.setPixelColor(i, brightness,0,brightness); pink
      //Clouds
    else if(condition== "Cloudy"){
        for (int i = 0; i < 10; i++) {
        CircuitPlayground.setPixelColor(i, brightness,brightness,brightness);
        brightness = brightness + fadeAmount;
          if (brightness <= 0 || brightness >= 50) {
            fadeAmount = -fadeAmount -30;
          }
        }
        delay(100);
    }
     //snow
    else if(condition== "Clear"){
        for (int i = 0; i < 10; i++) {
        CircuitPlayground.setPixelColor(i, brightness,0,brightness);
        brightness = brightness + fadeAmount;
          if (brightness <= 0 || brightness >= 50) {
            fadeAmount = -fadeAmount -30;
          }
        }
        delay(100);
    }
       //Sunny
    else{
            for (int i = 0; i < 10; i++) {
            CircuitPlayground.setPixelColor(i, brightness,brightness,0);
            brightness = brightness + fadeAmount;
              if (brightness <= 0 || brightness >= 255) {
                fadeAmount = -fadeAmount;
              }
            }
            delay(100);
         }
  }else{
   CircuitPlayground.clearPixels();
   exit(0);
  }
}
 
        
