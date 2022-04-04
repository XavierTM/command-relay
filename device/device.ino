



// AT+UART_DEF=9600,8,1,0,0


#include <SoftwareSerial.h>


#define ESP_RX 3
#define ESP_TX 4
#define GSM_RX 5
#define GSM_TX 6
#define BUTTON_PIN 7
#define BUZZER_PIN 8

#define DEVICE_NO 1

#define HOST "xavisoft.co.zw"
#define PORT "8080"
#define SSID "Netro Blue Valley"
#define WIFI_PWD "@Malkon#One!"
#define PHONE_NO "+263772801761"

SoftwareSerial esp(ESP_RX, ESP_TX);
SoftwareSerial gsm(GSM_RX, GSM_TX);



void clearGsm() {

	// Serial.print("GSM: ");

	while(gsm.available())
		gsm.read();
}



bool checkCommand() {


	esp.begin(9600);

	clearEsp();

	esp.setTimeout(1000);
	esp.println("AT+CIPSTART=\"TCP\",\"" + String(HOST) + "\"," + String(PORT));
	
	if (!esp.find("CONNECT")) {
		Serial.println("Not connected!");
		esp.end();
		return false;
	}


	clearEsp();


	esp.setTimeout(100);
	esp.println("AT+CIPSEND=33");
	
	if (!esp.find("OK")) {
		Serial.println("FAILED TO START SENDING.");
		esp.end();
		return false;
	}


	esp.setTimeout(2000);
	esp.println("GET /check-command/" +  String(DEVICE_NO) + " HTTP/1.1");
	esp.println();

	if (!esp.find("||true||")) {
		esp.end();
		return false;
	}

	esp.println("AT+CIPCLOSE");
	clearEsp();

	esp.end();

	return true;


}



void ring() {

	Serial.println("Ringing...");

	tone(BUZZER_PIN, 1000); // Send 1KHz sound signal...
	delay(700);        // ...for 1 sec
	noTone(BUZZER_PIN);     // Stop sound...
	delay(300);

	tone(BUZZER_PIN, 1000); // Send 1KHz sound signal...
	delay(700);        // ...for 1 sec
	noTone(BUZZER_PIN);     // Stop sound...

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


void sendMessage() {

	gsm.begin(9600);


	clearGsm();


	gsm.println("AT+CMGF=1");
	delay(1000);
	gsm.println("AT+CMGS=\"" + String(PHONE_NO) + "\"\r");
	// gsm.println("AT+CMGS=\"+263777901802\"\r");
	delay(1000);
	gsm.println("The button is pressed."); // text to be sent
	delay(100);
	gsm.println((char)26); //ASII Code for CTRL+Z
	delay(1000);

	clearGsm();

	gsm.end();
	
}


void clearEsp() {

	// Serial.println("ESP: ");

	while(esp.available())
		esp.read();

}


void sendCommand() {

	esp.begin(9600);

	clearEsp();

	Serial.println("Sending...");

	esp.setTimeout(1000);
	esp.println("AT+CIPSTART=\"TCP\",\"" + String(HOST) + "\"," + String(PORT));
	
	
	if (!esp.find("CONNECT")) {
		Serial.println("Not connected!");
		esp.end();
		return;
	}


	clearEsp();


	esp.setTimeout(100);
	esp.println("AT+CIPSEND=32");
	
	if (!esp.find("OK")) {
		Serial.println("FAILED TO START SENDING.");
		esp.end();
		return;
	}

	int device_no;

	if (DEVICE_NO == 1)
		device_no = 2;
	else
		device_no = 1;


	esp.println("GET /post-command/" + String(device_no) + " HTTP/1.1");
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
	esp.println("AT+CWJAP=\"" + String(SSID) + "\",\"" + String(WIFI_PWD) + "\"");
	delay(1000);


	// // enable multiple connections
	// esp.println("AT+CIPMUX=1");
	// delay(2000);

	esp.end();

	pinMode(BUTTON_PIN, INPUT_PULLUP);


	// delay(2000);
	// sendMessage();
	// Serial.println('message sent.');

	// gsm.begin(9600);

}




void loop () {

	if (isButtonPressed()) {
		sendCommand();

		if (DEVICE_NO == 1) {
			Serial.println("Calling sendMessage");
			sendMessage();
		}
	}


	if (checkCommand()) {
		ring();
	}

}


// void loop() {
	

// 	// ring();

// 	// delay(1000);

// 	// while (Serial.available())
// 	// 	gsm.write(Serial.read());


// 	// while (gsm.available())
// 	// 	Serial.write(gsm.read());


// 	delay(2000);
// 	sendMessage();

// 	while (true);


// }