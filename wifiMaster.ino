#define DEBUG true
#include <Wire.h>

//variable to send to slave
int x = 0;

void setup() 
{
  Wire.begin(); 
  
  Serial.begin(115200);
  Serial1.begin(115200);
  
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  
  sendData("AT+RST\r\n", 2000, false); // reset module
  sendData("AT+CWMODE=2\r\n", 1000, false); // configure as access point
  sendData("AT+CIFSR\r\n", 1000, DEBUG); // get ip address
  sendData("AT+CWSAP?\r\n", 2000, DEBUG); // get SSID info (network name) 
  sendData("AT+CIPMUX=1\r\n", 1000, false); // configure for multiple connections
  sendData("AT+CIPSERVER=1,80\r\n", 1000, false); // turn on server on port 80
}

void loop() 
{
  if (Serial1.available()) 
  {
    if (Serial1.find("+IPD,")) 
    {
      delay(500);
      int connectionId = Serial1.read() - 48; // read() function returns ASCII decimal value and 0 (the first decimal number) starts at 48
      
      //setup control web page
      String webpage = "<h1>Hello World!</h1>";
      
      String cipSend = "AT+CIPSEND=";
      cipSend += connectionId;
      cipSend += ",";
      
      //setup control buttons
      webpage += "<a href=\"/l0\"><button>FORWARD</button></a>";
      webpage += "<a href=\"/l1\"><button>BACKWARD</button></a>";
      webpage += "<a href=\"/l2\"><button>STOP</button></a>";
      webpage += "<a href=\"/l3\"><button>RIGHT</button></a>";
      webpage += "<a href=\"/l4\"><button>LEFT</button></a>";
      webpage += "<a href=\"/l5\"><button>HONK</button></a>";
      webpage += "<a href=\"/l6\"><button>ACCELERATE</button></a>";
      webpage += "<a href=\"/l7\"><button>DECELERATE</button></a>";
      webpage += "<a href=\"/l8\"><button>SERVO</button></a>";
      
      cipSend += webpage.length();
      cipSend += "\r\n";
      sendData(cipSend, 100, DEBUG);
      sendData(webpage, 150, DEBUG);
      
      String closeCommand = "AT+CIPCLOSE=";
      closeCommand += connectionId; // append connection id
      closeCommand += "\r\n";
      sendData(closeCommand, 300, DEBUG);
    }
  }
}

//send data to slave device
String sendData(String command, const int timeout, boolean debug) 
{
  String response = "";
  Serial1.print(command); // send command to the esp8266
  long int time = millis();
  
  while ((time + timeout) > millis()) 
  {
    while (Serial1.available()) 
    {
      char c = Serial1.read(); // read next char
      response += c;
    }
  }
  
  //going forward
  if (response.indexOf("/l0") != -1) 
  {
      Wire.beginTransmission(9); // transmit to device #9 
      Wire.write(1); // sends x 
      Wire.endTransmission(); // stop transmitting 
  }
  
  //going backwards
  if (response.indexOf("/l1") != -1) 
  {
      Wire.beginTransmission(9); // transmit to device #9 
      Wire.write(0); // sends x 
      Wire.endTransmission(); // stop transmitting 
  }

  //turn right
  if (response.indexOf("/l2") != -1) 
  {
      Wire.beginTransmission(9); // transmit to device #9 
      Wire.write(2); // sends x 
      Wire.endTransmission(); // stop transmitting 
  }

  //turn left
  if (response.indexOf("/l3") != -1) 
  {
      Wire.beginTransmission(9); // transmit to device #9 
      Wire.write(3); // sends x 
      Wire.endTransmission(); // stop transmitting 
  }
  
  //accelerate
  if (response.indexOf("/l4") != -1) 
  {
      Wire.beginTransmission(9); // transmit to device #9 
      Wire.write(4); // sends x 
      Wire.endTransmission(); // stop transmitting 
  }
  if (response.indexOf("/l5") != -1)
  {
    tone(8, 4978, 500);
  }
  
  //decelerate
  if (response.indexOf("/l6") != -1)
  {
      Wire.beginTransmission(9); // transmit to device #9 
      Wire.write(5); // sends x 
      Wire.endTransmission(); // stop transmitting 
  }
  
  //honk
  if (response.indexOf("/l7") != -1)
  {
      Wire.beginTransmission(9); // transmit to device #9 
      Wire.write(6); // sends x 
      Wire.endTransmission(); // stop transmitting 
  }
  
  //control servo motor
  if (response.indexOf("/l8") != -1)
  {
      Wire.beginTransmission(9); // transmit to device #9 
      Wire.write(7); // sends x 
      Wire.endTransmission(); // stop transmitting 
  }
  
  if (debug) 
  {
    Serial.print(response);
  }
  
  return response;
}
