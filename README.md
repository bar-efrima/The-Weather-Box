# The-Weather-Box

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
