#include <Servo.h>

#include <Adafruit_BMP085.h>

#include <Adafruit_BMP085_U.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiAP.h>
#include <ESP8266WiFiGeneric.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WiFiScan.h>
#include <ESP8266WiFiSTA.h>
#include <ESP8266WiFiType.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <WiFiServer.h>
#include <WiFiUdp.h>

WiFiServer server(80);
IPAddress ip(192,168,145,125);
//temperature sensor
Adafruit_BMP085 bmp;

//servo motors
Servo headServo;
Servo rightShoulderServo;
Servo leftShoulderServo;
Servo rightElbowServo;
Servo leftElbowServo;

//servo pin out
int headPin = 2;//pin D4
int rightShoulderPin = 14;//pin D5
int leftShoulderPin = 12;//pin D6
int rightElbowPin = 13;//pin D7
int leftElbowPin = 15;//pin D8

//servo motor position
int headPos;
int rsPos;
int lsPos;
int rePos;
int lePos;

bool moveUp = false;
bool moveDown = false;

void setup() {
  //sensors begin
bmp.begin();
//servo begin
headServo.attach(headPin);
rightShoulderServo.attach(rightShoulderPin);
leftShoulderServo.attach(leftShoulderPin);
rightElbowServo.attach(rightElbowPin);
leftElbowServo.attach(leftElbowPin);

//initialize the position
headPos = 0;
rsPos = 0;
lsPos = 0;
rePos = 0;
lePos = 0;

//wifi
WiFi.mode(WIFI_AP); //Our ESP8266-12E is an AccessPoint
WiFi.softAP("Hello_IoT", "12345678"); // Provide the (SSID, password); .
server.begin(); // Start the HTTP Server
delay(10);
//Looking under the hood
Serial.begin(115200); //Start communication between the ESP8266-12E and the monitor window
IPAddress HTTPS_ServerIP= WiFi.softAPIP(); // Obtain the IP of the Server
Serial.print("Server IP is: "); // Print the IP to the monitor window
Serial.println(HTTPS_ServerIP);
Serial.println();
Serial.print("The MAC address is ");
Serial.println(WiFi.macAddress());
}

void loop() {
  // put your main code here, to run repeatedly:
WiFiClient client = server.available();
if (!client) {
return;
}
//Looking under the hood
Serial.println("Somebody has connected :)");

//Read what the browser has sent into a String class and print the request to the monitor
String request = client.readStringUntil('\r');
//Looking under the hood
Serial.println(request);

//test the servo motor
if(request.indexOf("/ServoMove") != -1)
{
  lookAround(headServo,headPos);
}
if (request.indexOf("/ServoMotorHead") != -1)
{
  lookAround(headServo,headPos);
}

if (request.indexOf("/ServoMoveRightShoulder") != -1)
{
  lookAround(rightShoulderServo,rsPos);
}

if (request.indexOf("/ServoMoveLeftShoulder") != -1)
{
  lookAround(leftShoulderServo,lsPos);
}
if (request.indexOf("/ServoMoveLeftElbow") != -1)
{
  lookAround(leftElbowServo,lePos);
}

if (request.indexOf("/ServoMoveRightElbow") != -1)
{
  lookAround(rightElbowServo,rePos);
}

byte temperature = bmp.readTemperature();
byte pressure = bmp.readPressure();
byte seaLevel = bmp.readSealevelPressure();
byte realAltitude = bmp.readAltitude();

//makes the html page
String s = "HTTP/1.1 200 OK\r\n";
s += "Content-Type: text/html\r\n\r\n";
s += "<!DOCTYPE HTML>\r\n<html>\r\n";
s += "<style>";
s += "table, th, td { border: 1px solid black;";
s += "padding: 5px;";
s += "</style>";
s += "<br><input type=\"button\" name=\"dataButton\" value=\"GET DATA\" onclick=\"location.href='/READDATA'\">";
s += "<br><br><br>";
s += "<br><input type=\"button\" name=\"testButton\" value=\"Turn Servo\" onclick=\"location.href='/ServoMove'\">";
s += "<br><br><br>";
s += "<br><input type=\"button\" name=\"headButton\" value=\"Turn Servo Head\" onclick=\"location.href='/ServoMoveHead'\">";
s += "<br><br><br>";
s += "<br><input type=\"button\" name=\"rightShoulderButton\" value=\"Turn Servo Right Shoulder\" onclick=\"location.href='/ServoMoveRightShoulder'\">";
s += "<br><br><br>";
s += "<br><input type=\"button\" name=\"leftShoulderButton\" value=\"Turn Servo Left Shoulder\" onclick=\"location.href='/ServoMoveLeftShoulder'\">";
s += "<br><br><br>";
s += "<br><input type=\"button\" name=\"leftElbowButton\" value=\"Turn Servo Left Elbow\" onclick=\"location.href='/ServoMoveLeftElbow'\">";
s += "<br><br><br>";
s += "<br><input type=\"button\" name=\"rightElbowButton\" value=\"Turn Servo Right Elbow\" onclick=\"location.href='/ServoMoveRightElbow'\">";

    s += "<table style='width:100%'>";
    //shows the temperature
    s += "<tr>";
    s += "<th>";
    s += "Temperature";
    s += "</th>";
    
    s += "<th>";
    s += "Pressure";
    s += "</th>";
    
    s += "<th>";
    s += "Sea Level";
    s += "</th>";
    
    s += "<th>";
    s += "Altitude";
    s += "</th>";
    s += "</tr>";
    //end row 1
    s += "<tr>";
    s += "<th>";
    s += temperature;
    s += " *C";
    s += "</th>";
    s += "<th>";
    s += pressure;
    s += " Pa";
    s += "</th>";
    //shows the sea level
    s += "<th>";
    s += seaLevel;
    s += " Pa";
    s += "</th>";
    
    //shows the altitude
    s += "<th>";
    s +=  realAltitude;
    s +=" meters";
    s += "</th>";
        s += "</tr>";
    
    s += "</table>";

if(request.indexOf("/READDATA") != -1)
{
  getData(s);
}
s += "</html>\n";


//serve the html document to the browser
client.flush();
client.print(s);
delay(1);
Serial.println("client disconnected");

}
//Servo motor movement test
void lookAround(Servo servo, int pos)
{
  for(pos = 0; pos <= 180; pos += 1) // goes from 0 degrees to 180 degrees 
  {                                  // in steps of 1 degree 
    servo.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(15);                       // waits 15ms for the servo to reach the position 
  } 
  for(pos = 180; pos>=0; pos-=1)     // goes from 180 degrees to 0 degrees 
  {                                
    servo.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(15);                       // waits 15ms for the servo to reach the position 
  }
}

//move servo motor to the right or down
void moveServoDown(Servo servo, int pos)
{  
    for(pos = 0; pos <= 90; pos += 1) // goes from 0 degrees to 90 degrees 
  {                                  // in steps of 1 degree 
    servo.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(15);                       // waits 15ms for the servo to reach the position 
  } 
}
//move servo motor to the left or up
void moveServoUp(Servo servo, int pos)
{
  for(pos = 90; pos>=0; pos-=1)
  {                                
    servo.write(pos);
    delay(15);
  }
}

//reads the sensor data from the bmp180
void getData(String client)
{
    client += "<table>";
    
    client += "<tr>";
    client += "Temperature = ";
    client += "<th>";
    client += bmp.readTemperature();
    client += " *C";
    client += "</th>";
    client += "</tr>";
    
    client += "<tr>";
    client += "Pressure";
    client += "<th>";
    client += bmp.readPressure();
    client += " Pa";
    client += "</th>";
    client += "</tr>";
    
    // Calculate altitude assuming 'standard' barometric
    // pressure of 1013.25 millibar = 101325 Pascal
    client += "<tr>";
    client += "Altitude = ";
    client += "<th>";
    client +=  bmp.readAltitude();
    client += " meters";
    client += "</th>";
    client += "</tr>";

    client += "<tr>";
    client += "Pressure at sealevel (calculated) = ";
    client += "<th>";
    client += bmp.readSealevelPressure();
    client += " Pa";
    client += "</th>";
    client += "</tr>";
  // you can get a more precise measurement of altitude
  // if you know the current sea level pressure which will
  // vary with weather and such. If it is 1015 millibars
  // that is equal to 101500 Pascals.
    client += "<tr>";
    client += "Real altitude";
    client += "<th>";
    client +=  bmp.readAltitude(101500);
    client +=" meters";
    client += "</th>";
    client += "</tr>";
    
    client += "</table>";
    delay(500);
}
