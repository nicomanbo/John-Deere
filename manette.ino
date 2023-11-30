#include <math.h>
#include <L298N.h>
#include <SoftwareSerial.h>
#include <ArduinoBlue.h>
#include <Servo.h> // include servo library
Servo myservo;
Servo myservo2; // 
Servo servo1;

bool depart = false;
bool state = true;
bool state2 = true;
bool lastState = false;
int k = 0;
int pinTri = 11;
int pinTri2 = 10;
int pinDirection = 12;
int pos = 1500;
int valeur = 0;

int IN4 = 3;//PIN
int IN3 = 4;//PIN
int ENB =2;//PIN
const int ch1 = 8; //jaune
const int ch2 = 6;  //blanc
const int ch3 = 5; //orange
const int ch4 = 7; //brun
unsigned long throttle;
unsigned long direction;
L298N motor(ENB, IN3, IN4);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(ch1,INPUT);
  pinMode(ch2,INPUT);
  pinMode(ch3,INPUT);
  pinMode(ch4,INPUT);
  servo1.attach(pinDirection);
  delay(100);

}

void grainerouge() {
    myservo.attach(pinTri);
    pos = 1220;
    myservo.writeMicroseconds(pos);   // tell servo to go to position in variable 'pos'
    delay(700);     // waits 15ms for the servo to reach the position
    pos = 1850;
    myservo.writeMicroseconds(pos);              // tell servo to go to position in variable 'pos'
    //Serial.println(pos);
    delay(200);                       // waits 15ms for the servo to reach the position
    myservo.writeMicroseconds(1500);
    myservo.detach();
  }

void graineblanche() {
    myservo2.attach(pinTri2);
    pos = 1300;
    myservo2.writeMicroseconds(pos);   // tell servo to go to position in variable 'pos'
    delay(950);     // waits 15ms for the servo to reach the position
    pos = 1850;
    myservo2.writeMicroseconds(pos);              // tell servo to go to position in variable 'pos'
    //Serial.println(pos);
    delay(150);                       // waits 15ms for the servo to reach the position
    myservo2.writeMicroseconds(1500);
    myservo2.detach();
  }

void moteur(int x){
  if (x <= -5) {
    x = x - 20;
    motor.setSpeed(-1*x);
    motor.backward();
  }
  else if (x >= 5) {
    x = x + 20;
    motor.setSpeed(x);
    motor.forward();
  }
  else if (x < 5 && x > -5){
    motor.stop();
  }
}

void loop() {
  // DIRECTION AVEC UNE VALUE DE 950 A 1900 CHANGER EN ANGLE DE 142 A 52 DEGRÉ CAR IL EST DÉSAXER DE 7 DEGRÉ
  direction = pulseIn(ch1,HIGH);
  if (direction >= 800){
    int valeurDIR = map(direction, 950, 1900, 142, 52);
    if (valeurDIR >= 52 && valeurDIR <= 142){
      if (valeurDIR >= 92 && valeurDIR <= 102){
        servo1.write(97);
      }
      else {
        servo1.write(valeurDIR);
      }
    }
  }

  // TRACTION AVANT AVEC UNE VALUE DE 1000 A 1900 CHANGER EN PWM DE -155 A 155, SI LA VALEUR
  // EST IMMOBILE, ACTIVE LES BUTTONS SINON LE IF / ELSE ACTIVE LE MOTEUR PLUS BAS
  throttle = pulseIn(ch2,HIGH);
  valeur = map(throttle, 1000, 1990, -155, 155);
  // IF / ELSE
  if (valeur >= -5 && valeur <= 5 && depart == true){
    motor.stop();
    int reading1 = pulseIn(ch3, HIGH);
    // GRAINE ROUGE SELON LA VALEUR DE READING1 QUI CORRESPOND AU CH3
    if (reading1 >= 900 && reading1 <= 1000){
      state = true;
      if (state == lastState && k > 0){
        grainerouge();
        Serial.println("grainerouge");
        lastState = false;
      }
      }
    if (reading1 >= 1850 && reading1 <= 1950){
      k += 1;
      state = false;
      if (state == lastState){
        grainerouge();
        Serial.println("grainerouge");
        lastState = true;
      }
      }
    // GRAINE BLANCHE SELON LA VALEUR DE READING2 QUI CORRESPOND AU CH4
    int reading2 = pulseIn(ch4, HIGH);
    if (reading2 >= 1800 && reading2 <= 1950 && state2 == true){
      state2 = false;
    }
    if (reading2 >= 900 && reading2 <= 1000 && state2 == false){
      state2 = true;
      graineblanche();
      Serial.println("graineblanche");
    }
  }
  // TRACTION AVANT AVEC LA FONCTION MOTEUR QUI FAIT AVANCER OU RECULER
  else if (valeur >= -155 && valeur <= 155){
    depart = true;
     moteur(valeur);
  }
}
