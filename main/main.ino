//**************************************************************//
//  Name    : shiftOutCode, Dual Binary Counters                 //
//  Author  : Carlyn Maw, Tom Igoe                               //
//  Date    : 25 Oct, 2006                                       //
//  Version : 1.0                                                //
//  Notes   : Code for using a 74HC595 Shift Register            //
//          : to count from 0 to 255                             //
//**************************************************************//

//#include <color-bounce.ino>

//Pin connected to ST_CP of 74HC595
int latchPin = 8;
//Pin connected to SH_CP of 74HC595
int clockPin = 12;
//Pin connected to DS of 74HC595
int dataPin = 11;



void setup() {
  //Start Serial for debuging purposes
  Serial.begin(9600);
  //set pins to output because they are addressed in the main loop
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  Serial.begin(9600);


  clearShiftRegisters();

  int a = 0;
  a = a | (1 << 0);
  a = a | (1 << 1);
  a = a | (1 << 2);
  Serial.print(a);
}

void clearShiftRegisters() {

  int shiftOff[6] = {255, 255, 255, 255, 255, 255};

  digitalWrite(latchPin, 0);
    shiftOut(dataPin, clockPin, shiftOff, 6);
  digitalWrite(latchPin, 1);


}



void loop() {


  //color_bounce();
  pwm_single_led_constant_color(&Serial);
  //pwm_constant_color(&Serial);
  // for (int x = 0; x < 255; x++) {
  //   digitalWrite(latchPin, 0);
  //    shiftOut(dataPin, clockPin, 64);
  //    digitalWrite(latchPin, 1);
  //    delay(10);
  // }


}




void shiftOut(int myDataPin, int myClockPin, int myDataOut[], int arraySize) {
  // This shifts 8 bits out MSB first,
  //on the rising edge of the clock,
  //clock idles low

  //internal function setup
  int p=0;
  int r=0;


 //clear everything out just in case to
 //prepare shift register for bit shifting
  digitalWrite(myDataPin, 0);
  digitalWrite(myClockPin, 0);

  //for each bit in the byte myDataOut�
  //NOTICE THAT WE ARE COUNTING DOWN in our for loop
  //This means that %00000001 or "1" will go through such
  //that it will be pin Q0 that lights.
  for (r = 0; r < arraySize; r++)  {
    for (p = 7; p >= 0; p--)  {
      digitalWrite(myClockPin, 0);

      //if the value passed to myDataOut and a bitmask result
      // true then... so if we are at i=6 and our value is
      // %11010100 it would the code compares it to %01000000
      // and proceeds to set pinState to 1.
      if ( myDataOut[r] & (1 << p) ) {
        //Sets the pin to HIGH
        digitalWrite(myDataPin, 1);
      }
      else {
        //Sets the pin to LOW
        digitalWrite(myDataPin, 0);
      }

      //register shifts bits on upstroke of clock pin
      digitalWrite(myClockPin, 1);

    }
    //zero the data pin after shift to prevent bleed through
    digitalWrite(myDataPin, 0);
  }

  //stop shifting
  digitalWrite(myClockPin, 0);
}
