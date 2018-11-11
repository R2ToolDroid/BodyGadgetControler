/// Board Leonardo
/// Programmer USBasp

///Automatische Gadgetwegung
///
///RC Signal an 10 gibt schaltimpulse
///RC Signal an 9 gibt Bewegung weiter
///4,5,6,7,8, A0(16) LED zur kontrolle
///2 und 3 I2C

// PCM Ports
// 0 = Servo Arm rechts auf ab
// 1 = Servo Hand
// 3 = Servo Arm Links auf ab
// 4 =
// 6 =
// 7 =
// 8 =
// 9 =
//10 =
//11 = P-1 Tür links
//12 = P-2 Tür links mitte
//13 = P-3 Kleine Tür oben rechts Charge Bay 
//14 = P-4 Kleine Tür unten rechts
//15 = P-5 Tür rechts

//### BUGFIX  Aktuell ist die Klappe PCM 12 falsch gesetzt!!!
//### Das muss Klappe 1 sein?


#include <Process.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

#define MIN_PULSE_WIDTH       500
//650
#define MAX_PULSE_WIDTH       2000
//2350
#define DEFAULT_PULSE_WIDTH   1500
//1500
#define FREQUENCY             50
//50

int sensorMode = 9;
int sensorRC = 10;
int Mode = 0;// 0=RandMove // 1=RCMove
int timer = 100; 
int thisPin = 0;
int thisKlappe = 1;
int ledPins[] = {
  4, 5, 6, 7, 8, 18
};       // an array of pin numbers to which LEDs are attached
int pinCount = 6; 
int ledState = LOW; 
int RCmode = 0;
int CheckMode = 0;
int active;
int signal;

         //Nummer  0   1  2  3  4  5
int KlappeAuf[] = {0,50,60,80,40,160};

         //Nummer  0   1  2   3   4  5
int KlappeZu[] = {0,170,170,160,160,50};

int gripper(){
    active = pulseIn(sensorMode,HIGH);
    while (active >= 1300) {
      active = pulseIn(sensorMode,HIGH);  
      signal = pulseIn(sensorRC,HIGH);
      Serial.print("Gripper Signal__");
      signal = signal/10;
      Serial.println (signal-20);
      Serial.print("Active Signal__");
      Serial.println (active);
      pwm.setPWM(1, 0, pulseWidth(signal-20));///Hand bewegung
    }
    pwm.setPWM(1, 0, pulseWidth(70));///Hand schließen
    return;
}

/// Alle Klappen auf und Zu
int Mode_0() {
   active = pulseIn(sensorMode,HIGH);

    while (active >= 1300){
      
    for (int thisPin = 0; thisPin < pinCount; thisPin++) {
        digitalWrite(ledPins[thisPin], HIGH);
        delay(timer);
        digitalWrite(ledPins[thisPin], LOW);
    }
    
    digitalWrite(ledPins[thisPin], HIGH);

    active = pulseIn(sensorMode,HIGH);

    for (int thisKlappe = 0; thisKlappe <= 5; thisKlappe++) {
        
         int port = thisKlappe +10;
         pwm.setPWM(port, 0, pulseWidth(KlappeAuf[thisKlappe]));//Klappe 1

         delay(100);

    }

    }

    
    for (int thisKlappe = 5; thisKlappe >= 0; thisKlappe--) {
        
         int port = thisKlappe +10;
         pwm.setPWM(port, 0, pulseWidth(KlappeZu[thisKlappe]));//Klappe 1
         delay(100);
    }

    
    Mode = 0;
    //Sysreset();
    return;
}


///Mode 1  .. Klappe rechts mit Arm
int Mode_1() {
    // loop from the lowest pin to the highest:
    for (int thisPin = 0; thisPin < pinCount; thisPin++) {
      digitalWrite(ledPins[thisPin], HIGH);
      delay(timer);
      digitalWrite(ledPins[thisPin], LOW);
    }
    //Sysreset();
    active = pulseIn(sensorMode,HIGH);    
    while (active >= 1300){
     // pwm.setPWM(11, 0, pulseWidth(70));///Klappe 1 auf
      pwm.setPWM(11, 0, pulseWidth(KlappeAuf[1]));//Klappe 1
      delay(1500);
      pwm.setPWM(0, 0, pulseWidth(160));///Arm 1 hoch
      active = pulseIn(sensorMode,HIGH);
      gripper();
    }
  
    pwm.setPWM(0, 0, pulseWidth(30));///Arm 1 zurück
    delay(2000);
    pwm.setPWM(11, 0, pulseWidth(KlappeZu[1]));//Klappe 1
    Mode = 1;
    return;
}

///Klappen Alle auf
int Mode_2() {
   active = pulseIn(sensorMode,HIGH);

    while (active >= 1300){
      
    for (int thisPin = 0; thisPin < pinCount; thisPin++) {
        digitalWrite(ledPins[thisPin], HIGH);
        delay(timer);
        digitalWrite(ledPins[thisPin], LOW);
    }
    
    digitalWrite(ledPins[thisPin], HIGH);

    active = pulseIn(sensorMode,HIGH);

    //pwm.setPWM(11, 0, pulseWidth(70));//Klappe 1
    //pwm.setPWM(12, 0, pulseWidth(60));//Klappe 2
    //pwm.setPWM(13, 0, pulseWidth(80));//Klappe 3
    //pwm.setPWM(14, 0, pulseWidth(60));//Klappe 4
    //pwm.setPWM(15, 0, pulseWidth(170));//Klappe 5

    for (int thisKlappe = 1; thisKlappe <= 5; thisKlappe++) {
        
         int port = thisKlappe +10;
         pwm.setPWM(port, 0, pulseWidth(KlappeAuf[thisKlappe]));//Klappe 1

        }

        delay(1200);

        pwm.setPWM(2, 0, pulseWidth(150));///Arm 2 hoch
         pwm.setPWM(0, 0, pulseWidth(150));///Arm 1 hoch
        
    }

    /////ARME AUF ????
    pwm.setPWM(0, 0, pulseWidth(20));///Arm 2 zurück
    pwm.setPWM(2, 0, pulseWidth(20));///Arm 2 zurück
    delay(2000);
    
    for (int thisKlappe = 1; thisKlappe <= 5; thisKlappe++) {
        
         int port = thisKlappe +10;
         pwm.setPWM(port, 0, pulseWidth(KlappeZu[thisKlappe]));//Klappe 1

    }
    return;
}


int Mode_3() {
   active = pulseIn(sensorMode,HIGH);
   while (active >= 1300){

    for (int thisPin = 0; thisPin < pinCount; thisPin++) {
        digitalWrite(ledPins[thisPin], HIGH);
        delay(timer);
        digitalWrite(ledPins[thisPin], LOW);
    }
    
    digitalWrite(ledPins[thisPin], HIGH);
   
    pwm.setPWM(13, 0, pulseWidth(KlappeAuf[3])); // AUf
     pwm.setPWM(14, 0, pulseWidth(KlappeAuf[4]));
   

   active = pulseIn(sensorMode,HIGH);
   } 
   
    pwm.setPWM(13, 0, pulseWidth(KlappeZu[3])); // ZU
     pwm.setPWM(14, 0, pulseWidth(KlappeZu[4]));
    //Sysreset();
    return;
}

///Mode 4  .. Klappe 4 klein
int Mode_4() {
    // loop from the lowest pin to the highest:
    active = pulseIn(sensorMode,HIGH);
   while (active >= 1300){

    for (int thisPin = 0; thisPin < pinCount; thisPin++) {
        digitalWrite(ledPins[thisPin], HIGH);
        delay(timer);
        digitalWrite(ledPins[thisPin], LOW);
    }
    
    
    
    pwm.setPWM(14, 0, pulseWidth(KlappeAuf[4]));
    active = pulseIn(sensorMode,HIGH);

   }
    delay(1000);
    pwm.setPWM(14, 0, pulseWidth(KlappeZu[4]));

   Mode = 4;
   return;
}



///Mode 5  .. Klappe 5 groß
int Mode_5() {
    // loop from the lowest pin to the highest:
    for (int thisPin = 0; thisPin < pinCount; thisPin++) {
      digitalWrite(ledPins[thisPin], HIGH);
      delay(timer);
      digitalWrite(ledPins[thisPin], LOW);
    }
    //Sysreset();
    active = pulseIn(sensorMode,HIGH);    
    while (active >= 1300){
     // pwm.setPWM(11, 0, pulseWidth(70));///Klappe 1 auf
      pwm.setPWM(15, 0, pulseWidth(KlappeAuf[5]));//Klappe 1
      delay(1500);
      pwm.setPWM(2, 0, pulseWidth(150));///Arm 2 hoch
      active = pulseIn(sensorMode,HIGH);
      
    }
  
    pwm.setPWM(2, 0, pulseWidth(30));///Arm 2 zurück
    delay(2000);
    pwm.setPWM(15, 0, pulseWidth(KlappeZu[5]));//Klappe 1
    Mode = 1;
    return;
}




int Sysreset(){

       pwm.setPWM(0, 0, pulseWidth(30));///Arm 1 zurück
       pwm.setPWM(2, 0, pulseWidth(30));///Arm 2 zurück
       delay(1500);
       

        pwm.setPWM(11, 0, pulseWidth(150));       

        //pwm.setPWM(12, 0, pulseWidth(KlappeAuf[1]));//Klappe 2
        
        delay(100);
      pwm.setPWM(11, 0, pulseWidth(170));///Klappe 1 zu (70 auf)
      delay(150);
        pwm.setPWM(12, 0, pulseWidth(140));    
        delay(100); 
      pwm.setPWM(12, 0, pulseWidth(170));///Klappe 2 zu (60 auf)
      delay(150);
        pwm.setPWM(13, 0, pulseWidth(150));
        delay(100); 
      pwm.setPWM(13, 0, pulseWidth(160));///Klappe 3 zu (80 auf)
      
      
      delay(150);
        pwm.setPWM(14, 0, pulseWidth(170)); 
        delay(100);  
      pwm.setPWM(14, 0, pulseWidth(160));///Klappe 4 zu (60 auf)
      
      
       delay(150);
        pwm.setPWM(15, 0, pulseWidth(7));   
        delay(100);
      pwm.setPWM(15, 0, pulseWidth(50));///Klappe 5 zu (170 auf)
      
}




void setup(){
  Serial.begin(9600);

  // set the digital pin as output:
 for (int thisPin = 0; thisPin < pinCount; thisPin++) {
    pinMode(ledPins[thisPin], OUTPUT);
  }

  Serial.print("16 channel Servo test!");
  pwm.begin();
  pwm.setPWMFreq(FREQUENCY);

 
    // if analog input pin 0 is unconnected, random analog
  // noise will cause the call to randomSeed() to generateB
  // different seed numbers each time the sketch runs.
  // randomSeed() will then shuffle the random function.
  randomSeed(analogRead(0));

  //pwm.setPWM(12, 0, pulseWidth(KlappeAuf[1]));//Klappe 2
  //delay (12000);
  //pwm.setPWM(12, 0, pulseWidth(KlappeZu[1]));//Klappe 2

  delay(3000);
  
  Sysreset();
  
}

int pulseWidth(int angle)
{
  int pulse_wide, analog_value;
  pulse_wide   = map(angle, 0, 180, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
  analog_value = int(float(pulse_wide) / 1000000 * FREQUENCY * 4096);
  Serial.println(analog_value);
  return analog_value;
}

int checkinput(){
  
     RCmode = pulseIn(sensorRC,HIGH);
     Serial.print("RC Mode v04 ");
     Serial.println(RCmode);
     
      if (RCmode >= 1500){
      Mode = Mode -1; 
      digitalWrite(ledPins[thisPin], LOW);
      }

      if (RCmode <= 1000){
      Mode = Mode +1; 
      digitalWrite(ledPins[thisPin], LOW);
      }
      
    return;
}


void loop() {

    CheckMode = pulseIn(sensorMode,HIGH);
    Serial.print("CheckMode ");
    Serial.println(CheckMode);
    checkinput();
     
    Serial.print("mode ");
    Serial.println(Mode);
    thisPin = Mode;
    digitalWrite(ledPins[thisPin], HIGH);

    if (Mode == 0 && CheckMode >= 1300){
         Mode_0();
     }

    if (Mode == 1 && CheckMode >= 1300){
         Mode_1();
     }
    if (Mode == 2 && CheckMode >= 1300){
         Mode_2();
     }
    if (Mode == 3 && CheckMode >= 1300){
         Mode_3();
     }
    if (Mode == 4 && CheckMode >= 1300){
         Mode_4();
     }

     if (Mode == 5 && CheckMode >= 1300){
         Mode_5();
     }

     
    if (Mode >= 6 || RCmode >=5000){
       Mode=-1;
     }
    if (Mode == -2) {
      Mode = 0;
     }
  
  delay(500);
  
}





