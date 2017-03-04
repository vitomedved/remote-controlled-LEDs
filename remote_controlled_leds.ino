/*
*********************************************************************************
*
*       CONNECTIONS TO ARUINO:
* yellow, red and green LED's to pins 11, 12, 13
* piezo speaker to pin A0
* IR sensor to pin 6
*
*********************************************************************************
*
* ar mp3 remote decimal codes for buttons(checked with Serial.print(); function):
* 16753245 - CH-: all LED's off
* 16736925 - CH: error sound
* 16769565 - CH+: all LED's on
* 16720605 - PREV: turn on only previous LED
* 16712445 - NEXT: turn on only next LED
* 16761405 - PLAY/PAUSE: error sound
* 16769055 - VOL-: turn on previous LED (if all LED are on then turn off first one, and so on)
* 16754775 - VOL+: turn on next LED (same logic as VOL-)
* 16748655 - EQ (): error sound
* 16738455 - 0: error sound
* 16750695 - 100+: error sound
* 16756815 - 200+: error sound
* 16724175 - 1: turn on yellow LED
* 16718055 - 2: turn on red LED
* 16743045 - 3: turn on green LED
* 16716015 - 4: error sound
* 16726215 - 5: error sound
* 16734885 - 6: error sound
* 16728765 - 7: error sound
* 16730805 - 8: error sound
* 16732845 - 9: error sound
*********************************************************************************
*/
#include <IRremote.h>
#include <IRremoteInt.h>
long btn;
//IR pin
const int IRpin = 6;
//piezo speaker pin
const int piezo = A0;
//LED pins
const int yellowLED = 11;
const int redLED = 12;
const int greenLED = 13;
//LED states
int yellowState = 0;
int redState = 0;
int greenState = 0;

//variables for buttons
const long btnChMinus = 16753245;
const long btnChPlus = 16769565;
const long btnPrev = 16720605;
const long btnNext = 16712445;
const long btnVolMinus = 16769055;
const long btnVolPlus = 16754775;
const long btn1 = 16724175;
const long btn2 = 16718055;
const long btn3 = 16743045;

IRrecv irrecv(IRpin);
decode_results results;

//functions for buttons
void btnPressed();
void allOff();
void allOn();
void singlePrev();
void singleNext();
void multiplePrev();
void multipleNext();
void lightYellow();
void lightRed();
void lightGreen();
void soundError();
void soundYellow();
void soundRed();
void soundGreen();
void soundOff();
void soundOn();
void soundMultiple();

void setup(){
  Serial.begin(9600);
  
  pinMode(IRpin, INPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);

  irrecv.enableIRIn();  
}

void loop(){
  
  if(irrecv.decode(&results)){
    btn = results.value;
    btnPressed();
    irrecv.resume();
  }
}
void btnPressed(){
    switch(btn){
        case btnChMinus:
            allOff();
            break;

        case btnChPlus:
            allOn();
            break;

        case btnPrev:
            singlePrev();
            break;

        case btnNext:
            singleNext();
            break;

        case btnVolMinus:
            multiplePrev();
            break;

        case btnVolPlus:
            multipleNext();
            break;

        case btn1:
            lightYellow();
            break;

        case btn2:
            lightRed();
            break;

        case btn3:
            lightGreen();
            break;
        default:
            soundError();
            break;
    }
}


void allOff(){
    digitalWrite(redLED, LOW);
    digitalWrite(yellowLED, LOW);
    digitalWrite(greenLED, LOW);
    yellowState = 0;
    greenState = 0;
    redState = 0;
    soundOff();
}

void allOn(){
    digitalWrite(redLED, HIGH);
    digitalWrite(yellowLED, HIGH);
    digitalWrite(greenLED, HIGH);
    yellowState = 1;
    greenState = 1;
    redState = 1;
    soundOn();
}
void singlePrev(){
    if((yellowState == 0 && redState == 0 && greenState == 0) || (yellowState == 1 && redState == 0 && greenState == 0)){
        digitalWrite(greenLED, HIGH);
        digitalWrite(yellowLED, LOW);
        yellowState = 0;
        greenState = 1;
        soundGreen();
    }else if(yellowState == 0 && redState == 0 && greenState == 1){
        digitalWrite(greenLED, LOW);
        digitalWrite(redLED, HIGH);
        redState = 1;
        greenState = 0;
        soundRed();
    }else if(yellowState == 0 && redState == 1 && greenState == 0){
        digitalWrite(redLED, LOW);
        digitalWrite(yellowLED, HIGH);
        redState = 0;
        yellowState = 1;
        soundYellow();
    }else{
        soundError();
    }

}

void singleNext(){
     if((yellowState == 0 && redState == 0 && greenState == 0) || (yellowState == 0 && redState == 0 && greenState == 1)){
        digitalWrite(greenLED, LOW);
        digitalWrite(yellowLED, HIGH);
        yellowState = 1;
        greenState = 0;
        soundYellow();
    }else if(yellowState == 0 && redState == 1 && greenState == 0){
        digitalWrite(redLED, LOW);
        digitalWrite(greenLED, HIGH);
        redState = 0;
        greenState = 1;
        soundGreen();
    }else if(yellowState == 1 && redState == 0 && greenState == 0){
        digitalWrite(redLED, HIGH);
        digitalWrite(yellowLED, LOW);
        redState = 1;
        yellowState = 0;
        soundRed();
    }else{
        soundError();
    }
}

void multiplePrev(){
    int states[] = {yellowState, redState, greenState};
    int leds[] = {yellowLED, redLED, greenLED};
    
    if(yellowState == 0 && redState == 0 && greenState == 0){
        soundError();
        return;
    }

    for(int j = 2; j >= 0; j--){
        if(states[j] == 1){
            digitalWrite(leds[j], LOW);
            states[j] = 0;
            break;
        }
    }

    soundMultiplePrev();

    yellowState = states[0];
    redState = states[1];
    greenState = states[2];
}

void multipleNext(){
    int states[] = {yellowState, redState, greenState};
    int leds[] = {yellowLED, redLED, greenLED};

    if(yellowState == 1 && redState == 1 && greenState == 1){
        soundError();
        return;
    }else if(yellowState == 0 && redState == 0 && greenState == 0){
        digitalWrite(yellowLED, HIGH);
        yellowState = 1;
        soundMultipleNext();
        return;
    }

    for(int j = 0; j < 3; j++){
        if(states[j] == 0){
            digitalWrite(leds[j], HIGH);
            states[j] = 1;
            break;
        }
    }

    soundMultipleNext();

    yellowState = states[0];
    redState = states[1];
    greenState = states[2];
}

void lightYellow(){
    digitalWrite(yellowLED, HIGH);
    yellowState = 1;
    tone(piezo, 261);
    delay(200);
    noTone(piezo);
}

void lightRed(){
    digitalWrite(redLED, HIGH);
    redState = 1;
    tone(piezo, 293);
    delay(200);
    noTone(piezo);
}

void lightGreen(){
    digitalWrite(greenLED, HIGH);
    greenState = 1;
    tone(piezo, 329);
    delay(200);
    noTone(piezo);
}

void soundError(){
    tone(piezo, 500);
    delay(200);
    noTone(piezo);
}

void soundOff(){
    tone(piezo, 310);
    delay(100);
    tone(piezo, 240);
    delay(100);
    noTone(piezo);
}

void soundOn(){
    tone(piezo, 420);
    delay(100);
    tone(piezo, 450);
    delay(100);
    noTone(piezo);
}

void soundGreen(){
    tone(piezo, 329);
    delay(200);
    noTone(piezo);
}

void soundRed(){
    tone(piezo, 293);
    delay(200);
    noTone(piezo);
}

void soundYellow(){
    tone(piezo, 261);
    delay(200);
    noTone(piezo);
}

void soundMultipleNext(){
    tone(piezo, 390);
    delay(200);
    noTone(piezo);
}

void soundMultiplePrev(){
    tone(piezo, 380);
    delay(200);
    noTone(piezo);
}
