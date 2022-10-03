#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <FastLED.h>
#define NUM_LEDS_PER_STRIP 50
#define BR_PIN 2
#define HL_PIN 14
#define SW_L 1
#define SW_R 3
#define HAZARD 5
#define RELAY_L 12
#define RELAY_R 16
const char* ssid = "BLACKBOX";       
const char* password = "6b17lzjyhgfe!"; 

byte pixelType = 0;
char drawIn[4];
char frameIn[768];

CRGB led[NUM_LEDS_PER_STRIP];
CRGB led2[NUM_LEDS_PER_STRIP];
CRGB led3[NUM_LEDS_PER_STRIP];
CRGB led4[NUM_LEDS_PER_STRIP];
ESP8266WebServer server(80);

bool rearledstatel = LOW;
bool rearledstater = LOW;
bool hlstate = LOW;
bool prevstate1 = LOW;
bool prevstate2 = LOW;
int buttonState1 = 0;         
int buttonState2 = 0;
int buttonState3 = 0;
int buttonState4 = 0;

void setup() {
  
  FastLED.addLeds<WS2812B, 4,GRB>(led, NUM_LEDS_PER_STRIP);
  FastLED.addLeds<WS2812B, 0, GRB>(led2, NUM_LEDS_PER_STRIP);
  FastLED.addLeds<WS2812B, 15,GRB>(led3, NUM_LEDS_PER_STRIP);
  FastLED.addLeds<WS2812B, 13,GRB>(led4, NUM_LEDS_PER_STRIP);  
  FastLED.setBrightness(100);

  Serial.begin(115200);
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("Access Point IP:");
  Serial.println(myIP);
  
  server.on("/", handle_OnConnect);
  server.on("/leftledon", handle_leftledon);
  server.on("/leftledoff", handle_leftledoff);
  server.on("/rightledon", handle_rightledon);
  server.on("/rightledoff", handle_rightledoff);
  server.on("/hlon",handle_hlon);
  server.on("/hloff",handle_hloff);
  server.onNotFound(handle_NotFound);
  server.begin();

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(BR_PIN, INPUT);
  pinMode(SW_L, INPUT);
  pinMode(SW_R, INPUT);
  pinMode(HAZARD, INPUT);
  pinMode(HL_PIN, OUTPUT);
  pinMode(RELAY_L, OUTPUT);
  pinMode(RELAY_R, OUTPUT);
  digitalWrite(HL_PIN, LOW);
  digitalWrite(BR_PIN, LOW);
  digitalWrite(HAZARD, HIGH);
  digitalWrite(RELAY_L, HIGH);
  digitalWrite(RELAY_R, HIGH);
  
}

void loop() {
  server.handleClient();
  buttonState1 = digitalRead(BR_PIN); 
  buttonState2 = digitalRead(SW_L);
  buttonState3 = digitalRead(SW_R);
  buttonState4 = digitalRead(HAZARD);
  
//HAZARD
  if(buttonState4 == HIGH){
   digitalWrite(LED_BUILTIN, HIGH);
    for(int i = 0; i < NUM_LEDS_PER_STRIP;i++){
      led[i] = CRGB(0,0,0);
      led2[i] = CRGB(0,0,0);
      led3[i] = CRGB(0,0,0);
      led4[i] = CRGB(0,0,0);
    }
      FastLED.show();
      rearledstatel = LOW;
      rearledstater = LOW;
      prevstate1 = LOW;
      prevstate2 = LOW;
  }else{ 
    if (buttonState1 == HIGH){
      for(int i = 0; i < NUM_LEDS_PER_STRIP;i++){
            led[i] = CRGB(255,97,0);
            led2[i] = CRGB(255,97,0);
            led3[i] = CRGB(255,97,0);
            led4[i] = CRGB(255,97,0);
          }
          FastLED.show();
          
          delay(1000);
      for(int i = 0; i < NUM_LEDS_PER_STRIP;i++){
        led[i] = CRGB(0,0,0);
        led2[i] = CRGB(0,0,0);
        led3[i] = CRGB(0,0,0);
        led4[i] = CRGB(0,0,0);
      }
      FastLED.show();
      delay(500);
      }
    else{
      for(int i = 0; i < NUM_LEDS_PER_STRIP;i++){
            led[i] = CRGB(255,97,0);
            led2[i] = CRGB(255,97,0);
            led3[i] = CRGB(255,97,0);
            led4[i] = CRGB(255,97,0);
          }
          FastLED.show();
          delay(1000);
          rearledstatel = LOW;
          rearledstater = LOW;
          prevstate1 = LOW;
          prevstate2 = LOW;
      for(int i = 0; i < NUM_LEDS_PER_STRIP;i++){
        led[i] = CRGB(0,0,0);
        led2[i] = CRGB(0,0,0);
        led3[i] = CRGB(0,0,0);
        led4[i] = CRGB(0,0,0);
      }
      FastLED.show();
      delay(500); 
      }
      if((rearledstatel == HIGH || buttonState2 == LOW) && prevstate1 == LOW){
        for(int i = 0; i < NUM_LEDS_PER_STRIP;i++){
            led[i] = CRGB(255,97,0);
            led2[i] = CRGB(255,97,0);
            led3[i] = CRGB(255,97,0);
            led4[i] = CRGB(255,97,0);
          }
          FastLED.show();
          delay(1000);
        
        for(int i = 0; i < NUM_LEDS_PER_STRIP;i++){
          led[i] = CRGB(0,0,0);
          led2[i] = CRGB(0,0,0);
          led3[i] = CRGB(0,0,0);
          led4[i] = CRGB(0,0,0);
        }
          FastLED.show();
          delay(500); 
          prevstate1 = LOW;
      }else if((rearledstater == HIGH || buttonState3 == LOW) && prevstate2 == LOW){
        for(int i = 0; i < NUM_LEDS_PER_STRIP;i++){
            led[i] = CRGB(255,97,0);
            led2[i] = CRGB(255,97,0);
            led3[i] = CRGB(255,97,0);
            led4[i] = CRGB(255,97,0);
          }
          FastLED.show();
          delay(1000);
        for(int i = 0; i < NUM_LEDS_PER_STRIP;i++){
          led[i] = CRGB(0,0,0);
          led2[i] = CRGB(0,0,0);
          led3[i] = CRGB(0,0,0);
          led4[i] = CRGB(0,0,0);
        }
          FastLED.show();
          delay(500);
          prevstate1 = LOW; 
      }else{
        for(int i = 0; i < NUM_LEDS_PER_STRIP;i++){
            led[i] = CRGB(255,97,0);
            led2[i] = CRGB(255,97,0);
            led3[i] = CRGB(255,97,0);
            led4[i] = CRGB(255,97,0);
          }
          FastLED.show();
          delay(1000);
        for(int i = 0; i < NUM_LEDS_PER_STRIP;i++){
          led[i] = CRGB(0,0,0);
          led2[i] = CRGB(0,0,0);
          led3[i] = CRGB(0,0,0);
          led4[i] = CRGB(0,0,0);
        }
          FastLED.show();
          delay(500); 
      }
      
 }
 //BRAKE
  if(buttonState1 == HIGH){
     // brstate = HIGH;
      digitalWrite(LED_BUILTIN, HIGH);
      for(int i = 0; i < NUM_LEDS_PER_STRIP;i++){
        led[i] = CRGB(0,0,0);
        led2[i] = CRGB(0,0,0);
        led3[i] = CRGB(0,0,0);
        led4[i] = CRGB(0,0,0);
      }
        FastLED.show();
        rearledstatel = LOW;
        rearledstater = LOW;
        prevstate1 = LOW;
        prevstate2 = LOW;
   }else{ 
      if((rearledstatel == HIGH || buttonState2 == LOW) && prevstate1 == LOW){
        fill_solid(led, NUM_LEDS_PER_STRIP, CRGB::Red);
        fill_solid(led2, NUM_LEDS_PER_STRIP, CRGB::Red);
        fill_solid(led3, NUM_LEDS_PER_STRIP, CRGB::Red);
        fill_solid(led4, NUM_LEDS_PER_STRIP, CRGB::Red);
        FastLED.show();
        prevstate1 = LOW; 
        //rearledstatel = LOW;
      }else if((rearledstater == HIGH || buttonState3 == LOW) && prevstate2 == LOW){ 
        fill_solid(led, NUM_LEDS_PER_STRIP, CRGB::Red);
        fill_solid(led2, NUM_LEDS_PER_STRIP, CRGB::Red);
        fill_solid(led3, NUM_LEDS_PER_STRIP, CRGB::Red);
        fill_solid(led4, NUM_LEDS_PER_STRIP, CRGB::Red);
        FastLED.show();
        prevstate2 = LOW;
       // rearledstater = LOW;
      }else{
        fill_solid(led, NUM_LEDS_PER_STRIP, CRGB::Red);
        fill_solid(led2, NUM_LEDS_PER_STRIP, CRGB::Red);
        fill_solid(led3, NUM_LEDS_PER_STRIP, CRGB::Red);
        fill_solid(led4, NUM_LEDS_PER_STRIP, CRGB::Red);
        FastLED.show();
        
      }
   }
 
  //LEFT
  if((rearledstatel == HIGH || buttonState2 == LOW) && prevstate1 == LOW){
    prevstate1 = HIGH;
    for(int i = 0; i < NUM_LEDS_PER_STRIP;i++){
        led[i] = CRGB(255,97,0);
        led2[i] = CRGB(255,97,0);
        led3[i] = CRGB(255,97,0);
        led4[i] = CRGB(255,97,0);
      }
      FastLED.show();
      
      delay(1000);
      for(int i = 0; i < NUM_LEDS_PER_STRIP;i++){
        led[i] = CRGB(0,0,0);
        led2[i] = CRGB(0,0,0);
        led3[i] = CRGB(0,0,0);
        led4[i] = CRGB(0,0,0);
      }
      FastLED.show();
      delay(500);
    }
    /*led[0] = CRGB(255, 0, 0);
    led[1] = CRGB(255, 0, 0);
    led[2] = CRGB(255, 0, 0);
    led[3] = CRGB(255, 0, 0);
    led[4] = CRGB(255, 0, 0);
    led[5] = CRGB(255, 0, 0);
    led[6] = CRGB(255, 0, 0);
    led[7] = CRGB(255, 0, 0);
    led[8] = CRGB(255, 0, 0);
    led[9] = CRGB(255, 97, 0);
    led[10] = CRGB(255, 97, 0);
    led[11] = CRGB(255, 97, 0);
    led[12] = CRGB(255, 0, 0);
    led[13] = CRGB(255, 0, 0);
    led[14] = CRGB(255, 0, 0);
    led[15] = CRGB(255, 97, 0);
    led[16] = CRGB(255, 97, 0);
    led[17] = CRGB(255, 97, 0);
    led[18] = CRGB(255, 97, 0);
    led[19] = CRGB(255, 97, 0);
    led[20] = CRGB(255, 0, 0);
    led[21] = CRGB(255, 0, 0);
    led[22] = CRGB(255, 97, 0);
    led[23] = CRGB(255, 97, 0);
    led[24] = CRGB(255, 97, 0);
    led[25] = CRGB(255, 97, 0);
    led[26] = CRGB(255, 97, 0);
    led[27] = CRGB(255, 0, 0);
    led[28] = CRGB(255, 0, 0);
    led[29] = CRGB(255, 97, 0);
    led[30] = CRGB(255, 97, 0);
    led[31] = CRGB(255, 97, 0);
    led[32] = CRGB(255, 97, 0);
    led[33] = CRGB(255, 97, 0);
    led[34] = CRGB(255, 0, 0);
    led[35] = CRGB(255, 0, 0);
    led[36] = CRGB(255, 0, 0);
    led[37] = CRGB(255, 97, 0);
    led[38] = CRGB(255, 97, 0);
    led[39] = CRGB(255, 97, 0);
    led[40] = CRGB(255, 0, 0);
    led[41] = CRGB(255, 0, 0);
    led[42] = CRGB(255, 0, 0);
    led[43] = CRGB(255, 0, 0);
    led[44] = CRGB(255, 0, 0);
    led[45] = CRGB(255, 0, 0);
    led[46] = CRGB(255, 0, 0);
    led[47] = CRGB(255, 0, 0);
    led[48] = CRGB(255, 0, 0);
    fill_solid(led3, NUM_LEDS_PER_STRIP, CRGB(255,97,0));
    FastLED.show();
    delay(1000);
    led[9] = CRGB(0, 0, 0);
    led[10] = CRGB(0, 0, 0);
    led[11] = CRGB(0, 0, 0);
    led[15] = CRGB(0, 0, 0);
    led[16] = CRGB(0, 0, 0);
    led[17] = CRGB(0, 0, 0);
    led[18] = CRGB(0, 0, 0);
    led[19] = CRGB(0, 0, 0);
    led[22] = CRGB(0, 0, 0);
    led[23] = CRGB(0, 0, 0);
    led[24] = CRGB(0, 0, 0);
    led[25] = CRGB(0, 0, 0);
    led[26] = CRGB(0, 0, 0);
    led[29] = CRGB(0, 0, 0);
    led[30] = CRGB(0, 0, 0);
    led[31] = CRGB(0, 0, 0);
    led[32] = CRGB(0, 0, 0);
    led[33] = CRGB(0, 0, 0);
    led[37] = CRGB(0, 0, 0);
    led[38] = CRGB(0, 0, 0);
    led[39] = CRGB(0, 0, 0);
    for(int i = 0; i < NUM_LEDS_PER_STRIP;i++){
      led3[i] = CRGB(0,0,0);
    }
    FastLED.show();
    delay(500);*/
  else if((rearledstater == HIGH || buttonState3 == LOW) && prevstate2 == LOW){
      prevstate2 = HIGH;
      for(int i = 0; i < NUM_LEDS_PER_STRIP;i++){
            led[i] = CRGB(255,97,0);
            led2[i] = CRGB(255,97,0);
            led3[i] = CRGB(255,97,0);
            led4[i] = CRGB(255,97,0);
          }
          FastLED.show();
          
          delay(1000);
      for(int i = 0; i < NUM_LEDS_PER_STRIP;i++){
        led[i] = CRGB(0,0,0);
        led2[i] = CRGB(0,0,0);
        led3[i] = CRGB(0,0,0);
        led4[i] = CRGB(0,0,0);
      }
      FastLED.show();
      delay(500);
      }
      /*led2[0] = CRGB(255, 0, 0);
      led2[1] = CRGB(255, 0, 0);
      led2[2] = CRGB(255, 0, 0);
      led2[3] = CRGB(255, 0, 0);
      led2[4] = CRGB(255, 0, 0);
      led2[5] = CRGB(255, 0, 0);
      led2[6] = CRGB(255, 0, 0);
      led2[7] = CRGB(255, 0, 0);
      led2[8] = CRGB(255, 0, 0);
      led2[9] = CRGB(255, 97, 0);
      led2[10] = CRGB(255, 97, 0);
      led2[11] = CRGB(255, 97, 0);
      led2[12] = CRGB(255, 0, 0);
      led2[13] = CRGB(255, 0, 0);
      led2[14] = CRGB(255, 0, 0);
      led2[15] = CRGB(255, 97, 0);
      led2[16] = CRGB(255, 97, 0);
      led2[17] = CRGB(255, 97, 0);
      led2[18] = CRGB(255, 97, 0);
      led2[19] = CRGB(255, 97, 0);
      led2[20] = CRGB(255, 0, 0);
      led2[21] = CRGB(255, 0, 0);
      led2[22] = CRGB(255, 97, 0);
      led2[23] = CRGB(255, 97, 0);
      led2[24] = CRGB(255, 97, 0);
      led2[25] = CRGB(255, 97, 0);
      led2[26] = CRGB(255, 97, 0);
      led2[27] = CRGB(255, 0, 0);
      led2[28] = CRGB(255, 0, 0);
      led2[29] = CRGB(255, 97, 0);
      led2[30] = CRGB(255, 97, 0);
      led2[31] = CRGB(255, 97, 0);
      led2[32] = CRGB(255, 97, 0);
      led2[33] = CRGB(255, 97, 0);
      led2[34] = CRGB(255, 0, 0);
      led2[35] = CRGB(255, 0, 0);
      led2[36] = CRGB(255, 0, 0);
      led2[37] = CRGB(255, 97, 0);
      led2[38] = CRGB(255, 97, 0);
      led2[39] = CRGB(255, 97, 0);
      led2[40] = CRGB(255, 0, 0);
      led2[41] = CRGB(255, 0, 0);
      led2[42] = CRGB(255, 0, 0);
      led2[43] = CRGB(255, 0, 0);
      led2[44] = CRGB(255, 0, 0);
      led2[45] = CRGB(255, 0, 0);
      led2[46] = CRGB(255, 0, 0);
      led2[47] = CRGB(255, 0, 0);
      led2[48] = CRGB(255, 0, 0);
      for(int i = 0; i < NUM_LEDS_PER_STRIP; i++){
        led4[i]=CRGB(255,97,0);
      }
      FastLED.show();
      delay(1000);
      led2[9] = CRGB(0, 0, 0);
      led2[10] = CRGB(0, 0, 0);
      led2[11] = CRGB(0, 0, 0);
      led2[15] = CRGB(0, 0, 0);
      led2[16] = CRGB(0, 0, 0);
      led2[17] = CRGB(0, 0, 0);
      led2[18] = CRGB(0, 0, 0);
      led2[19] = CRGB(0, 0, 0);
      led2[22] = CRGB(0, 0, 0);
      led2[23] = CRGB(0, 0, 0);
      led2[24] = CRGB(0, 0, 0);
      led2[25] = CRGB(0, 0, 0);
      led2[26] = CRGB(0, 0, 0);
      led2[29] = CRGB(0, 0, 0);
      led2[30] = CRGB(0, 0, 0);
      led2[31] = CRGB(0, 0, 0);
      led2[32] = CRGB(0, 0, 0);
      led2[33] = CRGB(0, 0, 0);
      led2[37] = CRGB(0, 0, 0);
      led2[38] = CRGB(0, 0, 0);
      led2[39] = CRGB(0, 0, 0);
      for(int i = 0; i < NUM_LEDS_PER_STRIP;i++){
        led4[i] = CRGB(0,0,0);
      }
      FastLED.show();
      delay(500);*/
   else {
     if (prevstate1 == HIGH){
      prevstate1 = LOW;
      for(int i = 0; i < NUM_LEDS_PER_STRIP;i++){
        led[i] = CRGB(0,0,0);
        led3[i] = CRGB(0,0,0);
      }FastLED.show();
      digitalWrite(RELAY_L, HIGH); 
     }else if(prevstate2 == HIGH){
      prevstate2 = LOW;
      for(int i = 0; i < NUM_LEDS_PER_STRIP;i++){
        led2[i] = CRGB(0,0,0);
        led4[i] = CRGB(0,0,0);
      }
      FastLED.show();
     }else{
      for(int i = 0; i < NUM_LEDS_PER_STRIP;i++){
        led2[i] = CRGB(0,0,0);
        led4[i] = CRGB(0,0,0);
      }FastLED.show();
     }
      
   }
  
  }

void handle_OnConnect() {
  rearledstatel = LOW;
  rearledstater = LOW;
  hlstate = LOW;
  Serial.println("LED: OFF");
  server.send(200, "text/html", updateWebpage(rearledstatel,hlstate,rearledstater)); 
}

void handle_hlon(){
  hlstate = HIGH;
  digitalWrite(HL_PIN, HIGH);
  Serial.println("HEADLIGHT: ON");
  server.send(200, "text/html", updateWebpage(rearledstatel,hlstate,rearledstater)); 
  
}
void handle_hloff(){
  hlstate = LOW;
  digitalWrite(HL_PIN, LOW);
  Serial.println("HEADLIGHT: OFF");
  server.send(200, "text/html", updateWebpage(rearledstatel, hlstate,rearledstater)); 
}

void handle_leftledon() {
  rearledstatel = HIGH;
  digitalWrite(RELAY_L, LOW);
  Serial.println("LED: ON");
  server.send(200, "text/html", updateWebpage(rearledstatel, hlstate,rearledstater));   
  if ((rearledstatel == HIGH || buttonState2 == LOW) && rearledstater ==HIGH){  
    rearledstater = LOW;
    prevstate2 = LOW;
  }   
}

void handle_leftledoff() {
  rearledstatel = LOW;
  digitalWrite(RELAY_L, HIGH);
  Serial.println("LED: OFF");
  /*if (prevstate1 == HIGH){
      prevstate1 = LOW;
      for(int i = 0; i < NUM_LEDS_PER_STRIP;i++){
        led[i] = CRGB(0,0,0);
        led3[i] = CRGB(0,0,0);
      }FastLED.show();
      digitalWrite(RELAY_L, HIGH); }*/
  server.send(200, "text/html", updateWebpage(rearledstatel,hlstate,rearledstater)); 
}

void handle_rightledon() {
  rearledstater = HIGH;
  digitalWrite(RELAY_R, LOW);
  Serial.println("LED: ON");
  server.send(200, "text/html", updateWebpage(rearledstatel, hlstate,rearledstater));  
  if ((rearledstatel == HIGH || buttonState2 == LOW) && rearledstater ==HIGH){  
    rearledstatel = LOW;
    prevstate1 = LOW;
  }     
}

void handle_rightledoff() {
  rearledstater = LOW;
  digitalWrite(RELAY_R, HIGH);
  Serial.println("LED: OFF");
  if(prevstate2 == HIGH){
      prevstate2 = LOW;
      for(int i = 0; i < NUM_LEDS_PER_STRIP;i++){
        led2[i] = CRGB(0,0,0);
        led4[i] = CRGB(0,0,0);
      }
      FastLED.show();
     }
  server.send(200, "text/html", updateWebpage(rearledstatel,hlstate,rearledstater)); 
}
void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

String updateWebpage(uint8_t rearledstatel, uint8_t hlstate, uint8_t rearledstater){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>LED Control</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr +=".button {display: block;width: 80px;background-color: #1abc9c;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr +=".button-on {background-color: #3498db;}\n";
  ptr +=".button-on:active {background-color: #3498db;}\n";
  ptr +=".button-off {background-color: #34495e;}\n";
  ptr +=".button-off:active {background-color: #2c3e50;}\n";
  ptr +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<h1>ESP8266 Web Server</h1>\n";
  ptr +="<h3>Using Access Point(AP) Mode</h3>\n";
  
   if(rearledstatel){
    ptr +="<p>REAR PANEL LEFT: ON</p><a class=\"button button-off\" href=\"/leftledoff\">OFF</a>\n";
   }else{
    ptr +="<p>REAR PANEL LEFT: OFF</p><a class=\"button button-on\" href=\"/leftledon\">ON</a>\n";
   }
    if(hlstate){
      ptr +="<p>HEADLIGHT: ON</p><a class=\"button button-off\" href=\"/hloff\">OFF</a>\n";
    }else{
    ptr +="<p>HEADLIGHT: OFF</p><a class=\"button button-on\" href=\"/hlon\">ON</a>\n";
   }
   if(rearledstater){
    ptr +="<p>REAR PANEL RIGHT: ON</p><a class=\"button button-off\" href=\"/rightledoff\">OFF</a>\n";
   }else{
    ptr +="<p>REAR PANEL RIGHT: OFF</p><a class=\"button button-on\" href=\"/rightledon\">ON</a>\n";
   }
   

  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}
