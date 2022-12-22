#include <Arduino.h>
#include <TimerOne.h>

#define CAR_PIEZO A0
#define PED_PIEZO A1

const int PED_RED = 6;
const int PED_GREEN = 5;
const int MAIN_RED = 4;
const int MAIN_YELLOW = 3;
const int MAIN_GREEN = 2;
const int CYCLES_BLINK = 6;
volatile bool allowCross = false;
volatile bool green = false;
volatile bool waitingPed = false;
unsigned long latestCross;
unsigned long arrivalPed;
unsigned long latestCar;

void handleChecks() {
	if(analogRead(PED_PIEZO) > 900 && !waitingPed && millis() - latestCross > 3000) {
		waitingPed = true;
		arrivalPed = millis();
	}
	
	if(analogRead(CAR_PIEZO) > 900) {
		Serial.println("Carro detectado");
		latestCar = millis();
	}
}

void turnRed() {
  digitalWrite(MAIN_GREEN, LOW);
  digitalWrite(MAIN_YELLOW, HIGH);
  delay(1500);
  digitalWrite(MAIN_YELLOW, LOW);
  digitalWrite(MAIN_RED, HIGH);
  delay(100);
  digitalWrite(PED_RED, LOW);
  delay(100);
  digitalWrite(PED_GREEN, HIGH);
}

void blinkPedRed() {
  digitalWrite(PED_GREEN, LOW);

  for (int i = 0; i < CYCLES_BLINK; i++) {
    digitalWrite(PED_RED, HIGH);
    delay(150);
    digitalWrite(PED_RED, LOW);
    delay(150);
  }
}

void turnGreen() {
  blinkPedRed();
  delay(100);
  digitalWrite(MAIN_RED, LOW);
  digitalWrite(PED_RED, HIGH);
  delay(200);
  digitalWrite(MAIN_GREEN, HIGH);
}

void setup() {
	pinMode(PED_RED, OUTPUT);
	pinMode(PED_GREEN, OUTPUT);
	pinMode(MAIN_GREEN, OUTPUT);
	pinMode(MAIN_YELLOW, OUTPUT);
	pinMode(MAIN_GREEN, OUTPUT);
	digitalWrite(PED_GREEN, LOW);
	digitalWrite(PED_RED, LOW);
	digitalWrite(MAIN_RED, LOW);
	digitalWrite(MAIN_YELLOW, LOW);
	digitalWrite(MAIN_GREEN, LOW);
	pinMode(CAR_PIEZO, INPUT);
	pinMode(PED_PIEZO, INPUT);
	Serial.begin(9600);

	Serial.println("Inicializando...");
	Serial.println("Por favor aguarde.");
	while (analogRead(CAR_PIEZO) > 800 || analogRead(PED_PIEZO) > 800) {

	}

	Timer1.initialize(100000);
	Timer1.attachInterrupt(handleChecks);
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

/*
const int PED_RED = 2;
const int PED_GREEN = 3;
const int MAIN_RED = 4;
const int MAIN_YELLOW = 5;
const int MAIN_GREEN = 6;
const int CYCLES_BLINK = 6;
volatile bool red = false;
volatile bool green = false;
volatile bool waitingPed = false;
unsigned long arrivalPed;
unsigned long latestCar;

void setup() {
  // put your setup code here, to run once:
  pinMode(PED_RED, OUTPUT);
  pinMode(PED_GREEN, OUTPUT);
  pinMode(MAIN_GREEN, OUTPUT);
  pinMode(MAIN_YELLOW, OUTPUT);
  pinMode(MAIN_GREEN, OUTPUT);
  digitalWrite(PED_GREEN, LOW);
  digitalWrite(PED_RED, LOW);
  digitalWrite(MAIN_RED, LOW);
  digitalWrite(MAIN_YELLOW, LOW);
  digitalWrite(MAIN_GREEN, LOW);
  
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  turnRed();
  delay(3000);
  turnGreen();
  delay(5000);
  
  if (!green) {
  	turnGreen();
    green = true;
    red = false;
  }

  Serial.println(analogRead(CAR_PIEZO));
  
  if(analogRead(PED_PIEZO) > 450 && !waitingPed) {
    waitingPed = true;
	arrivalPed = millis();
  }
  
  if(analogRead(CAR_PIEZO) > 450) {
  	latestCar = millis();
  }
  
  if (waitingPed && (arrivalPed - latestCar > 5000 || arrivalPed - millis() > 8000)) {
  	turnRed();
    red = true;
    green = false;
    waitingPed = false;
  }
}

void turnRed() {
  digitalWrite(MAIN_GREEN, LOW);
  digitalWrite(MAIN_YELLOW, HIGH);
  delay(1500);
  digitalWrite(MAIN_YELLOW, LOW);
  digitalWrite(MAIN_RED, HIGH);
  delay(100);
  digitalWrite(PED_RED, LOW);
  delay(100);
  digitalWrite(PED_GREEN, HIGH);
}

void turnGreen() {
  blinkPedRed();
  delay(100);
  digitalWrite(MAIN_RED, LOW);
  digitalWrite(PED_RED, HIGH);
  delay(200);
  digitalWrite(MAIN_GREEN, HIGH);
}

void blinkPedRed() {
  digitalWrite(PED_GREEN, LOW);

  for (int i = 0; i < CYCLES_BLINK; i++) {
    digitalWrite(PED_RED, HIGH);
    delay(150);
    digitalWrite(PED_RED, LOW);
    delay(150);
  }
}
*/