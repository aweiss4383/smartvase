#include "Qduino.h"
#include "Wire.h"

fuelGauge lipo;

const int ledPin1 = 3;
const int ledPin2 = 5;
const int ledPin3 = 6;
const int ledPin4 = 9;

const int needWater = 4;


void setup() {
  //  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
  lipo.setup();

}

void loop() {
  //  analogWrite(A5,1024);
  //  Serial.println("moisture is:");
  //  Serial.println(analogRead(A5));
  int moisture = map(analogRead(A5), 0, 1024, 1 , 10);
  int volts = map(analogRead(A5), 0, 1024, 0, 10);
  if (lipo.chargePercentage() >= 30) {
    if (moisture <= needWater) {
      for (int i = 3; i <= 150; i += 3) {
        analogWrite(ledPin1, i);
        analogWrite(ledPin2, i);
        analogWrite(ledPin3, i);
        analogWrite(ledPin4, i);
        delay(40);
      }
      for (int i = 150; i >= 3; i -= 3) {
        analogWrite(ledPin1, i);
        analogWrite(ledPin2, i);
        analogWrite(ledPin3, i);
        analogWrite(ledPin4, i);
        delay(40);
      }
    } else {
      analogWrite(ledPin1, 0);
      analogWrite(ledPin2, 0);
      analogWrite(ledPin3, 0);
      analogWrite(ledPin4, 0);
      delay(10000);
    }
  } else if (lipo.chargePercentage() < 30) {
    for (int i = 3; i <= 150; i += 3) {
      analogWrite(ledPin1, i);
      analogWrite(ledPin2, i);
      analogWrite(ledPin3, i);
      analogWrite(ledPin4, i);
      delay(5);
    }
    for (int i = 150; i >= 3; i -= 3) {
      analogWrite(ledPin1, i);
      analogWrite(ledPin2, i);
      analogWrite(ledPin3, i);
      analogWrite(ledPin4, i);
      delay(5);
    }
  }
  Serial.println(moisture);
  String bat = String(lipo.chargePercentage());
  Serial.println(bat + "%");

  //  digitalWrite(ledPin,HIGH);
  //  delay(1000);
  //  digitalWrite(ledPin,LOW);
  //  delay(1000);
  

}
