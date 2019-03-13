#include <TinyGPS++.h>
#include <WiFi.h> //Connect to WiFi Network
#include <TFT_eSPI.h> // Graphics and font library for ST7735 driver chip
#include <SPI.h> //Used in support of TFT Display
#include <string.h>  //used for some string handling and processing.
#include <stdio.h>
#include <list>

char network[] = "MIT";  //SSID for 6.08 Lab
char password[] = ""; //Password for 6.08 Lab
//Some constants and some resources:
const int RESPONSE_TIMEOUT = 6000; //ms to wait for response from host
const int POSTING_PERIOD = 6000; //periodicity of getting a number fact.
const uint16_t IN_BUFFER_SIZE = 1000; //size of buffer to hold HTTP request
const uint16_t OUT_BUFFER_SIZE = 400; //size of buffer to hold HTTP response
char request_buffer[IN_BUFFER_SIZE]; //char array buffer to hold HTTP request
char response_buffer[OUT_BUFFER_SIZE]; //char array buffer to hold HTTP response

TFT_eSPI tft = TFT_eSPI();  // Invoke library, pins defined in User_Setup.h

HardwareSerial gps_serial(2);
TinyGPSPlus gps;
unsigned long timer;
const int input_pin1 = 16; //pin connected to button1
const int input_pin2 = 5; //pin connected to button2

unsigned long startscreen() {
  tft.setCursor(0, 0);
  tft.fillScreen(TFT_BLACK); //fill background
  tft.setRotation(3); //adjust rotation
  tft.setTextSize(2); //default font size
  tft.setTextColor(TFT_CYAN);  
  tft.println("Treasure Hunt");
  tft.setTextSize(1); //default font size
  tft.setTextColor(TFT_GREEN); 
  tft.println("");
  tft.println("Press button 2 to start");
  tft.println(" or restart game");
  tft.println("");
  tft.println("");
  tft.setTextColor(TFT_YELLOW);
  tft.println("Press button 1 when you");
  tft.println("think you've reached your");
  tft.println("objective location");
  }

void setup() {
  Serial.begin(115200);
  gps_serial.begin(9600,SERIAL_8N1,32,33);
  WiFi.begin(network,password); //attempt to connect to ESP WiFi
  uint8_t count = 0; //count used for ESP8266WiFi check times
  Serial.print("Attempting to connect to ");
  Serial.println(network);
  while (WiFi.status() != WL_CONNECTED && count<12) {
    delay(500);
    Serial.print(".");
    count++;
  }
  delay(2000);
  if (WiFi.isConnected()) { //if we connected then print our IP, Mac, and SSID we're on
    Serial.println("CONNECTED!");
    Serial.println(WiFi.localIP().toString() + " (" + WiFi.macAddress() + ") (" + WiFi.SSID() + ")");
    delay(500);
  } else { //if we failed to connect just Try again.
    Serial.println("Failed to Connect :/  Going to restart");
    Serial.println(WiFi.status());
    ESP.restart(); // restart the ESP (proper way)
  }
  
  tft.init();  //init screen
  startscreen();
  tft.setRotation(3); //adjust rotation
  pinMode(input_pin1, INPUT_PULLUP); //set input pin as an input!
  pinMode(input_pin2, INPUT_PULLUP); //set input pin as an input!
  timer = millis();
}

void loop() {
  if (gps_serial.available()){
    while (gps_serial.available()) 
      gps.encode(gps_serial.read());      // Check GPS
   Serial.println(gps.location.lat());
   Serial.println(gps.location.lng());
  }
  
 if ( (digitalRead(input_pin2)==0) && ((millis() - timer) > 1000) ){
  Serial.println("button 2 pressed");
  Serial.println("starting new game");
  getreq();
  timer = millis();
  }

  if ( (digitalRead(input_pin1)==0) && ((millis() - timer) > 1000) ){
    Serial.println("button 1 pressed");
    Serial.println("checking to see if gps is where objective location is");
     postreq();
     timer = millis();
  }
}



char getreq(){ // for starting game or restarting game
    char body[200]; //for body;
      int body_len = strlen(body); //calculate body length (for header reporting)
      sprintf(request_buffer,"GET http://608dev.net/sandbox/sc/sfgordon/ex05/treasurehunt.py HTTP/1.1\r\n");
      strcat(request_buffer,"Host: 608dev.net\r\n");
      strcat(request_buffer,"Content-Type: application/x-www-form-urlencoded\r\n");
      sprintf(request_buffer+strlen(request_buffer),"Content-Length: %d\r\n", body_len); //append string formatted to end of request buffer
      strcat(request_buffer,"\r\n"); //new line from header to body
      strcat(request_buffer,body); //body
      strcat(request_buffer,"\r\n"); //header
      Serial.println(request_buffer);
      do_http_request("608dev.net", request_buffer, response_buffer, OUT_BUFFER_SIZE, RESPONSE_TIMEOUT,true);
      //resets screen to display question
      tft.fillScreen(TFT_BLACK); //fill background
      tft.setTextSize(1); //default font size
      tft.setCursor(0,0,1); // set the cursor
      tft.println(response_buffer); //print the trivia question
    }

char postreq(){ // to be used when playing game, to see if you have reached your objective location
    Serial.println("calling gps for postreq");
    if(gps.location.isValid()){
      Serial.println("GOOD DATA");
      char body[200]; //for body;
      sprintf(body,"lat=%f&lon=%f",gps.location.lat(),gps.location.lng());//generate body, 
      int body_len = strlen(body); //calculate body length (for header reporting)
      sprintf(request_buffer,"POST http://608dev.net/sandbox/sc/sfgordon/ex05/treasurehunt.py HTTP/1.1\r\n");
      strcat(request_buffer,"Host: 608dev.net\r\n");
      strcat(request_buffer,"Content-Type: application/x-www-form-urlencoded\r\n");
      sprintf(request_buffer+strlen(request_buffer),"Content-Length: %d\r\n", body_len); //append string formatted to end of request buffer
      strcat(request_buffer,"\r\n"); //new line from header to body
      strcat(request_buffer,body); //body
      strcat(request_buffer,"\r\n"); //header
      Serial.println(request_buffer);
      do_http_request("608dev.net", request_buffer, response_buffer, OUT_BUFFER_SIZE, RESPONSE_TIMEOUT,true);
      tft.fillScreen(TFT_BLACK); //fill background
      tft.setCursor(0,0,1); // set the cursor
      tft.setTextSize(1); //default font size
      tft.println(response_buffer); //print the result
      
        }
    }



    
