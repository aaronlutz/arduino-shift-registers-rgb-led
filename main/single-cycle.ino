void singleCycle () {
  for (int step = 1; step < stepsPerCycle - 1; step++) {
    // latchPin LOW before ShiftOut, HIGH after
    digitalWrite(latchPin, 0);
    shiftOut(dataPin, clockPin, cycle[step], srCount);
    digitalWrite(latchPin, 1);
  }
}

void convertToCycle (int _colors[16][3], int _cycle[11][6], HardwareSerial *s) {
  //  Converts Array of integer RGB Values (one for each LED)
  //  into an array of bytes (one for each shift register for each step)
  //  and indefinetly shiftOut each step within the cycle

  // Notes to future self...
  /*
    Common Anode LEDs share their positive charge, with the three negative legs
    being unique to each color. To light up the red portion of the LED, the pin
    on the shift register for that leg of the LED needs to be LOW. Setting it
    LOW allows the current to pass through the LED. Setting it HIGH provides
    resitance and prevents current from passing through.

    Each shift register takes a byte and turns it into eight parellel bits.
    Each shift register has eight out pins.
    Each LED has three (red, green, and blue) pins.
    Eight LEDs need to be paired to three shift registers, don't leave any of
    the pins on the shift register unpaired.

    RGB Color values are defined between 0 and 255 for R, G, and B.
    The shift registers output a digital signal, so with them there are only
    nine possible states for the LED. Each LED pin can only be on or off.

      OFF - RGB(HIGH, HIGH, HIGH)
      WHITE - RGB(LOW, LOW, LOW)
      RED - RGB(LOW, HIGH, HIGH)
      GREEN - RGB(HIGH, LOW, HIGH)
      BLUE - RGB(HIGH, HIGH, LOW)
      TEAL RGB(HIGH, LOW, LOW)
      PINK RGB(LOW, HIGH, LOW)
      YELLOW RGB(LOW, LOW, HIGH)

    To use more than these 8 colors we need to use Pulse Wave Modulation (PWM)
    PWM is a way to mimic an analog signal with a digital one. A digital signal
    is discrete while analog is continous. Time is divided up time into major
    (cycle) and minor (step) units. To achieve RGB(53, 172, 233) the red pin of
    the LED needs to be LOW 21% of the time, the blue pin 67%, and the green pin
    91% of the time. So for the first 21% of the steps in the cycle the red pin
    will be set LOW, then for the remainder HIGH. At the Cycle level this
    averages to 21% on time.

    If the duration of the cycle is too slow, there will be a noticable flicker.
    The number of steps in each cycle needs to be modified until the flicker
    can't be seen. Changing the number of steps per cycle changes the number
    of unique LED colors. The number of steps is the resolution within each of
    the three color values. With 20 steps per cycle we have twice as many
    percieved values for red than with 10 steps per cycle.

    I initially planned on 255 steps per cycle, but the flicker was so bad that
    it wasn't even obvious that each LED was mimicking a color. Adding a variable
    called colorCompressionFactor divides both the initial 255 steps and the
    RGB values down to their compressed values.
  */



  // Define Variables
  int led, pin, step, this_pin;
  int pinValue, shiftRegister, srPin, srPinInverted;
  byte srValue;
  int pinValues[pinCount];
  int srBytes[stepsPerCycle][srCount];

  // 16 step color gradiant
  /*
    http://www.perbang.dk/rgbgradient/
    Color 1 = 35ACE9, Color 2 = BF001A
  */


  // Flatten rgbColors[][] into pinValues[]
  /*
    rgbColors[][] is the RGB value for each LEDs
    pinValues[] is # of steps each pin needs to be LOW (assuming 225 steps/cycle)
  */
  for (led = 0; led < ledCount; led++) {
    for (pin = 0; pin < 3; pin++) {
      this_pin = (led * 3) + pin;
      pinValues[this_pin] = _colors[led][pin] / colorCompressionFactor;
    }
  };

  // Create bytes to define each step in the cycle
  // one byte per shift register per step
  for (step = 0; step < stepsPerCycle; step++) {
    for (pin = 0; pin < pinCount; pin++) {

      // 0 - 255 value of current pin
      pinValue = pinValues[pin];

      // shift register, shift register pin this LED pin relates to
      shiftRegister = pin / 8;
      srPin = (pin) % 8;
      srPinInverted = abs( srPin - 7 );

      // At each shift registers first pin, reset the byte srValue to 11111111
      if (srPin == 0) {
        srValue = 255;
      }

      // If the pin is still LOW at this step, set the corresponding bit to 0
      if (pinValue > step) {
        srValue = srValue ^ (1 << srPinInverted);
      }

      // After the shift registers last pin, store srValue in srBytes
      if (srPin == 7) {
        _cycle[step][shiftRegister] = srValue;
      }

    }
  }

  // Print value if debugging
  if (debugPrinting == 1) {
    // Serial.println("--DEBUGGING------------------");
    //
    // Serial.print("stepsPerCycle = "); Serial.println(stepsPerCycle);
    //
    // // Print RGB Color Values
    // Serial.print("rgbColors {");
    // for (int led = 0; led < 16; led++)   {
    //   Serial.print("{");  Serial.print(rgbColors[led][0]);
    //   Serial.print(", "); Serial.print(rgbColors[led][1]);
    //   Serial.print(", "); Serial.print(rgbColors[led][2]);
    //   Serial.print("} ");
    // }
    // Serial.println("} "); Serial.println(' ');
    //
    // // Print the pinValues
    // Serial.print("pinValues  {");
    // for (int pin = 0; pin < 47; pin++)   {
    //   Serial.print(pinValues[pin]); Serial.print(", ");
    // }
    // Serial.print(pinValues[47]); Serial.println("} ");
    //
    //
    // // Print the Shift Register Byters
    // Serial.println("srBytes");
    // for (step = 0; step < stepsPerCycle; step++) {
    //   Serial.print(step);
    //   Serial.print(" { ");
    //   for (int r = 0; r < srCount; r++) {
    //     Serial.print(srBytes[step][r]);
    //     Serial.print(" ");
    //   }
    //   Serial.print(" }  { ");
    //   for (int r = 0; r < srCount; r++) {
    //     byte mask;
    //     byte data = srBytes[step][r];
    //     for (mask = 10000000; mask>0; mask >>= 1) { //iterate through bit mask
    //       if (data & mask){ // if bitwise AND resolves to true
    //         Serial.print(1); // send 1
    //       }
    //       else{ //if bitwise and resolves to false
    //         Serial.print(0); // send 0
    //       }
    //     }
    //     Serial.print(" ");
    //   }
    //   Serial.println(" }");
    // }
    // Serial.println("-----------------------------");
  }


}
