#include <Arduino.h>

#define CAR_INTERRUPT 0 // Pin 2
#define PED_INTERRUPT 1 // Pin 3

// Defining used pins
const int CAR_PLATE = 2;
const int PED_PLATE = 3;
const int PED_RED = 8;
const int PED_GREEN = 7;
const int MAIN_RED = 6;
const int MAIN_YELLOW = 5;
const int MAIN_GREEN = 4;

const int CYCLES_BLINK = 6;

volatile bool allowCross = false;
volatile bool green = false;
volatile bool waitingPed = false;

// Time related variables
unsigned long latestCross;
unsigned long arrivalPed;
unsigned long latestCar;

void handleCar() {
	if (digitalRead(CAR_PLATE) == HIGH) {
		Serial.println("Carro detectado");
		latestCar = millis();
	}
	else {
		// Possible addition to calculate speed
	}
}

void handlePedestrian() {
	if (digitalRead(PED_PLATE) == LOW && !waitingPed && millis() - latestCross > 3000) {
		Serial.println("Pedestre detectado.");
		waitingPed = true;
		arrivalPed = millis();
	}
}

void turnRed() {
  	digitalWrite(MAIN_GREEN, HIGH);
  	digitalWrite(MAIN_YELLOW, LOW);
  	delay(1500);
  	digitalWrite(MAIN_YELLOW, HIGH);
  	digitalWrite(MAIN_RED, LOW);
  	delay(100);
  	digitalWrite(PED_RED, HIGH);
  	delay(100);
  	digitalWrite(PED_GREEN, LOW);
}

void blinkPedRed() {
  	digitalWrite(PED_GREEN, HIGH);

  	for (int i = 0; i < CYCLES_BLINK; i++) {
    	digitalWrite(PED_RED, LOW);
    	delay(150);
    	digitalWrite(PED_RED, HIGH);
    	delay(150);
  	}
}

void turnGreen() {
  	blinkPedRed();
  	delay(100);
  	digitalWrite(MAIN_RED, HIGH);
  	digitalWrite(PED_RED, LOW);
  	delay(200);
  	digitalWrite(MAIN_GREEN, LOW);
}

void setup() {

	pinMode(PED_RED, OUTPUT);
	pinMode(PED_GREEN, OUTPUT);
	pinMode(MAIN_GREEN, OUTPUT);
	pinMode(MAIN_YELLOW, OUTPUT);
	pinMode(MAIN_RED, OUTPUT);
	pinMode(CAR_PLATE, INPUT_PULLUP);
	pinMode(PED_PLATE, INPUT_PULLUP);

	// Defined to change to make speed checks possible
	attachInterrupt(CAR_INTERRUPT, handleCar, CHANGE);

	attachInterrupt(PED_INTERRUPT, handlePedestrian, FALLING);

	// All LEDs start turned off
	// LEDs are connected directly to the 5V output
	digitalWrite(PED_GREEN, HIGH);
	digitalWrite(PED_RED, HIGH);
	digitalWrite(MAIN_RED, HIGH);
	digitalWrite(MAIN_YELLOW, HIGH);
	digitalWrite(MAIN_GREEN, HIGH);
	Serial.begin(9600);
}

void loop() {
	if (!green) {
		turnGreen();
		green = true;
	}
	
	if (waitingPed) {
		if (arrivalPed > latestCar && millis() - latestCar > 2000)
			allowCross = true;
		else if (arrivalPed < latestCar && millis() - latestCar > 5000)
			allowCross = true;
		else if (millis() - arrivalPed > 8000)
			allowCross = true;

		if (allowCross) {
			turnRed();
			green = false;
			delay(5000);
      waitingPed = false;
      latestCross = millis();
      allowCross = false;
		} 
	}
}