#include <SoftwareSerial.h>
#include <TinyGPS.h>

#define BTN 9
bool state;
float gpslat, gpslon;

TinyGPS gps;
SoftwareSerial sgps(4, 5);
SoftwareSerial sgsm(2, 3);

void setup()
{
  pinMode(BTN, INPUT_PULLUP);
  sgsm.begin(9600);
  sgps.begin(9600);
  Serial.begin(9600);
  delay(1000);
}

void loop()
{
  state = digitalRead(BTN);;

  sgps.listen();
  while (sgps.available())
  {
    int c = sgps.read();
    if (gps.encode(c))
    {
      gps.f_get_position(&gpslat, &gpslon);
    }
  }
    if (state == LOW) {
      Serial.println("Button pressed");
      delay(200);
      
      sgsm.print("AT+CMGF=1\r");
      delay(500);
      /*Replace XXXXXXXXXX to 10 digit mobile number &
        ZZ to 2 digit country code*/
      sgsm.print("AT+CMGS=\"+639162726939\"\r");
      delay(500);
      //The text of the message to be sent.
      sgsm.print("Latitude :");
      sgsm.println(gpslat, 6);
      sgsm.print("Longitude:");
      sgsm.println(gpslon, 6);
      sgsm.print("http://maps.google.com/maps?q=loc:");
      sgsm.print(gpslat);
      sgsm.print(",");
      sgsm.print(gpslon);
      delay(500);
      sgsm.print((char)26);
      delay(500);
    }

  }
