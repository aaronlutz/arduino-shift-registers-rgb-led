//Pin connected to ST_CP of 74HC595
int latchPin = 8;
//Pin connected to SH_CP of 74HC595
int clockPin = 12;
//Pin connected to DS of 74HC595
int dataPin = 11;

// Enable (1) of Disable (0) Serial printing
int debugPrinting = 0;

// Number of shift registers (must be divisible by three)
// Number of LEDs is determined by the number of shift registers
int srCount = 6;
int ledCount = 18; // ledCount = srCount / 3 * 8
int pinCount = ledCount * 3;

// Increase  - reduces flicker AND possible LED colors
// Descrease - increases flicker AND possible LED colors
int colorCompressionFactor = 20;
int stepsPerCycle = 11; // = 255 / colorCompressionFactor;

// pallete[ledCount][3]
int pallete[18][3];

// cycle[stepsPerCycle][srCount]
int cycle[11][6];

void setup() {
  //Start Serial for debuging purposes
  Serial.begin(9600);

  //set pins to output because they are addressed in the main loop
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  // Turn off all LEDs
  clearShiftRegisters();

  // Load a color gradiant
  getGradient(1, pallete);

  // Convert it to a cycle
  convertToCycle(pallete, cycle, &Serial);

}

void loop() {
  singleCycle ();
  //color_bounce();
}

void clearShiftRegisters() {
  // 255 if the off value for an entire shift register
  int shiftOff[srCount];
  for (int x = 0; x < srCount; x++) {
    shiftOff[x] = 255;
  }

  // open latch, shift out shift register values, close latch
  digitalWrite(latchPin, 0);
  shiftOut(dataPin, clockPin, shiftOff, 6);
  digitalWrite(latchPin, 1);

}


void shiftOut(int myDataPin, int myClockPin, int myDataOut[], int arraySize) {
  //**************************************************************//
  //  Author  : Carlyn Maw, Tom Igoe                               //
  //  Date    : 25 Oct, 2006                                       //
  //  Version : 1.0                                                //
  //  Notes   : Code for using a 74HC595 Shift Register            //
  //          : to count from 0 to 255                             //
  //**************************************************************//

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
