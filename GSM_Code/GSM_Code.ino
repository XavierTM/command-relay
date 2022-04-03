#include<SoftwareSerial.h>

SoftwareSerial mySerial(10,11); // RX and TX

void setup() 
{
 mySerial.begin(9600); // baudrate for the gsm
 Serial.begin(9600);

}

void sendmessageOne()
{
  mySerial.println("AT+CMGF=1");
  delay(1000);
  mySerial.println("AT+CMGS=\"+263774613020\"\r");
  delay(1000);
  mySerial.println("Good Morning"); // text to be sent
  delay(100);
  mySerial.println((char)26); //ASII Code for CTRL+Z
  delay(1000);
}

void sendmessageTwo()
{
  mySerial.println("AT+CMGF=1");
  delay(1000);
  mySerial.println("AT+CMGS=\"+263774613020\"\r");
  delay(1000);
  mySerial.println("Good Morning"); // change this text to required text
  delay(100);
  mySerial.println((char)26); //ASII Code for CTRL+Z
  delay(1000);
}

void sendmessageThree()
{
  mySerial.println("AT+CMGF=1");
  delay(1000);
  mySerial.println("AT+CMGS=\"+263774613020\"\r");
  delay(1000);
  mySerial.println("Good Bye"); // text to be sent
  delay(100);
  mySerial.println((char)26); //ASII Code for CTRL+Z
  delay(1000);
}


void receiveSMS()
{
  mySerial.println("AT+CMGD=1,4");
  delay(1000);
  mySerial.println("ATE0");
  delay(1000);
  mySerial.println("AT+CMGF=1");
  delay(1000);
  mySerial.println("AT+CNMI=3,3,0,0,0"); // AT Command to receive a live SMS
  delay(1000);
}

void loop() 
{
 sendmessageOne();
  delay(2000); //delay 2s
  sendmessageTwo();
  delay(10000); //delay 10s
  sendmessageThree();
  delay(1000); //delay 1s
if (Serial.available()>0)
   switch(Serial.read())
  {
    case 'r':
      Serial.println("ready....");
      receiveSMS();
      break;
  }
 
 if (mySerial.available()>0)
   Serial.write(mySerial.read()); 
}
