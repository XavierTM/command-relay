


// AT+UART_DEF=9600,8,1,0,0


#include <SoftwareSerial.h>


#define RX 3
#define TX 4
#define BUTTON_PIN 7
#define BUZZER_PIN 8;

SoftwareSerial esp(RX, TX);


void ring() {

	Serial.println("Ringing...");

	tone(buzzer, 1000); // Send 1KHz sound signal...
	delay(700);        // ...for 1 sec
	noTone(buzzer);     // Stop sound...
	delay(300);

	tone(buzzer, 1000); // Send 1KHz sound signal...
	delay(700);        // ...for 1 sec
	noTone(buzzer);     // Stop sound...

}


bool isButtonPressed() {

	if (digitalRead(BUTTON_PIN))
		return false;

	// debouncing
	delay(10);

	if (digitalRead(BUTTON_PIN))
		return false;

	return true;

}


void clearEsp() {
	while(esp.available())
		esp.read();
}


bool checkCommand() {

	clearEsp();

	esp.setTimeout(1000);
	esp.println("AT+CIPSTART=\"TCP\",\"192.168.0.100\",8080");
	
	if (!esp.find("CONNECT")) {
		Serial.println("Not connected!");
		return false;
	}


	clearEsp();


	esp.setTimeout(100);
	esp.println("AT+CIPSEND=31");
	
	if (!esp.find("OK")) {
		Serial.println("FAILED TO START SENDING.");
		return false;
	}


	esp.setTimeout(2000);
	esp.println("GET /check-command HTTP/1.1");
	esp.println();

	if (!esp.find("||true||"))
		return false;

	esp.println("AT+CIPCLOSE");
	clearEsp();

	return true;


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


	// pin modes
	pinMode(BUTTON_PIN, INPUT_PULLUP);
	pinMode(BUZZER_PIN, OUTPUT);

}




void loop () {

	if (checkCommand()) {
		ring();
	}

}


// void loop() {
	

// 	while (Serial.available())
// 		esp.write(Serial.read());


// 	while (esp.available())
// 		Serial.write(esp.read());


// }