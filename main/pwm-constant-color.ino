

void pwm_constant_color (HardwareSerial *s) {
  int ledCount = 16;
  int srCount = 6;
  int stepsPerCycle = 255;
  int pinCount = ledCount * 3;
  s->println('test');

  // 16 step color gradiant
  // http://www.perbang.dk/rgbgradient/
  // Color 1 = 35ACE9
  // Color 2 = BF001A
  int rgbColors[ledCount][3] = {
      {53, 172, 233},
      {45, 138, 230},
      {44, 104, 227},
      {40, 69, 224},
      {39, 36, 221},
      {66, 33, 219},
      {94, 29, 216},
      {122, 25, 213},
      {151, 22, 210},
      {180, 18, 207},
      {205, 15, 200},
      {202, 12, 166},
      {199, 9, 131},
      {196, 5, 96},
      {193, 2, 61},
      {191, 0, 25}};

  // flatten rgbColors into 1 dimensional array
  int pinValues[pinCount];

  // loop through rgbColors and populate pinValues
  // l for LED, p for PIN
  int currentPin;
  for (int l = 0; l < ledCount; l++) {
    for (int p = 0; p < 3; p++) {
      currentPin = (l * 3) + p;
      pinValues[currentPin] = rgbColors[l][p];
    }
  };

  // hold 1 or 0 for each pin for each step of the cycle
  int cyclePlan[stepsPerCycle][pinCount];
  int srBytes[stepsPerCycle][srCount];
  // p is pin (0 - 47) [48 = 3led pins * 16leds = 8sr pins * 6sr]
  // rpi is register pin index (0 - 7)
  int p;
  byte pByte;

// 0-255 value of pin
  int pinVal;
  // Step specific 0-1 value of pin
  int pinStepBit;

  for (int st = 0; st < stepsPerCycle; st++) {
    for (int r = 0; r < srCount; r++) {

      int srVal = 0; // int for sr, will perform bitmath on this

      for (int rpi = 0; rpi < 8; rpi++) {
        // current pin position
        p = (r * srCount) + rpi;
        // s->print(p);

        // retrieve 0 - 255 value of current pin
        pinVal = pinValues[p];

        // pinStepBit = 1 while pinVal < current Step
        // pinStepBit = 0 once pinStep >= current Step
        if (pinVal < st) {
          srVal = srVal | (1 << rpi);
        }
      }
      // s->println(srVal);
      srBytes[st][r] = srVal;
    }
  };

  for (int xxx = 0; xxx < 100; xxx++) {
    for (int st = 0; st < stepsPerCycle; st++) {
      digitalWrite(latchPin, 0);

      for (int r = 0; r < srCount; r++) {
        shiftOut(dataPin, clockPin, srBytes[st][r]);

      }

      digitalWrite(latchPin, 1);

    }
  }
}
