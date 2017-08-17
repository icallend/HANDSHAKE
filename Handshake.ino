/*
    HANDSHAKE
    Ian Benjamin Callender
    arduino uno + ellumiglow mega + 2xcapacitive touch sensors
    12/2016
*/


/* EL VARIABLES */
boolean resetFlag = false;  // reset flag
boolean cmdReady = false;  // command ready flag
boolean invOn = false;  // inverter state variable

/* CAPTOUCH VARIABLES */
const int TOUCH_BUTTON[] = {5, 7};  // Input pins for touch states
int buttonState[] = {0, 0};  // Variable for reading buttons

/* GENERAL VARS */
int delayTime = 100;
int randBounds[] = {50,125};


void setup() {
  // Configure button pins as inputs
  for (int i = 0; i < 2; i++) {
    pinMode(TOUCH_BUTTON[i], INPUT);
  }

  Serial.begin(250000);
}


void loop() {

  if (!resetFlag) {
    delay(1000);
    Serial.println("!\r\n");  // reset mega
    resetFlag = true;  // thus, run once
  }
  delayTime = 100; // reset, if after flicker

  // Read the state of the capacitive touch boards
  for (int i = 0; i < 2; i++) {
    buttonState[i] = digitalRead(TOUCH_BUTTON[i]);
  }


  // FOR DEBUGGING
  //cmdReady = true;


  // If touches are detected...
  if ( (buttonState[0] == HIGH) && (buttonState[1] == HIGH) ) {
    //Serial.print("H/H");
    setInv(true);

  } else if ( (buttonState[0] == LOW) && (buttonState[1] == LOW) ) {
    //Serial.print("L/L");
    setInv(false);

  } else {
    //Serial.print("H/L");
    //do something funky

    setInv(true);
    delay(random(randBounds[0], randBounds[1]));

    setInv(false);  // make sure this doesn't end with light on
    delayTime = random(randBounds[0], randBounds[1]);
  }

  delay(delayTime);
}


void setInv(boolean toState) {
  if (toState != invOn) {  // forget it, if the states match

    serialEvent();

    if (cmdReady) {
      cmdReady = false;  // because we're about to submit a command, and might return (thus calling serialEvent, setting cmdReady to true) instantaneously

      switch (toState) {
        case 0:
          Serial.print("set.inv.off 1\r\n");
          invOn = false;
          break;
        case 1:
          Serial.print("set.inv.on 1\r\n");
          invOn = true;
          break;
        default:
          break;
      }
    }

  }
}


// called by arduino whenever data is available
void serialEvent() {
  while (Serial.available()) {  // if nothing to read, there's nothing there, nothing will happen. that's why there's no set false

    if ( ((char)Serial.read()) == '>') {  // if ready for a new command (ie "cmd>")
      cmdReady = true;
    };

  };
}

