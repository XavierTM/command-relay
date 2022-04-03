


// AT+UART_DEF=9600,8,1,0,0


#include <SoftwareSerial.h>


#define ESP_RX 3
#define ESP_TX 4
#define GSM_RX 5
#define GSM_TX 6
#define BUTTON_PIN 7

SoftwareSerial esp(ESP_RX, ESP_TX);
SoftwareSerial gms(GSM_RX, GSM_TX)


bool isButtonPressed() {

	if (digitalRead(BUTTON_PIN))
		return false;

	// debouncing
	delay(10);

	if (digitalRead(BUTTON_PIN))
		return false;

	return true;

}


void sendMessage() {

	gsm.begin(9600);


	gsm.println("AT+CMGF=1");
	delay(1000);
	gsm.println("AT+CMGS=\"+263774613020\"\r");
	delay(1000);
	gsm.println("The button is pressed."); // text to be sent
	delay(100);
	gsm.println((char)26); //ASII Code for CTRL+Z
	delay(1000);

	gsm.end();
	
}


void clearEsp() {
	while(esp.available())
		esp.read();
}


void sendCommand() {

	esp.begin(9600);

	clearEsp();

	Serial.println("Sending...");

	esp.setTimeout(1000);
	esp.println("AT+CIPSTART=\"TCP\",\"192.168.0.100\",8080");
	
	if (!esp.find("CONNECT")) {
		Serial.println("Not connected!");
		esp.end();
		return;
	}


	clearEsp();


	esp.setTimeout(100);
	esp.println("AT+CIPSEND=30");
	
	if (!esp.find("OK")) {
		Serial.println("FAILED TO START SENDING.");
		esp.end();
		return;
	}

	esp.println("GET /post-command HTTP/1.1");
	esp.println();

	delay(1000);

	esp.println("AT+CIPCLOSE");


	clearEsp();

	Serial.println("Command send.");

	esp.end();

}


void setup() {

	Serial.begin(9600);
	esp.begin(9600);
	delay(2000);

	// set mode
	esp.println("AT+CWMODE=1");
	delay(2000);

	// connect
	esp.println("AT+CWJAP=\"Netro Blue Valley\",\"@Malkon#One!\"");
	delay(1000);


	// // enable multiple connections
	// esp.println("AT+CIPMUX=1");
	// delay(2000);

	esp.end();

	pinMode(BUTTON_PIN, INPUT_PULLUP);

}




void loop () {

	if (isButtonPressed()) {
		sendCommand();
	}

}


// void loop() {
	

// 	while (Serial.available())
// 		esp.write(Serial.read());


// 	while (esp.available())
// 		Serial.write(esp.read());


// }