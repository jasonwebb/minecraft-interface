/*
 * Direction movement pad for real-life Minecraft interface
 * =========================================================
 * 
 * Author:
 *  Jason Webb <zen.webb@gmail.com> - http://jason-webb.info
 * 
 * Description:
 *  Triggers WASD key keypresses based on events detected
 *  through pairs of piezo sensors (one pair per direction).
 */

// Pin assignments
#define  upPiezo1Pin     A9
#define  upPiezo2Pin     A8
#define  leftPiezo1Pin   A7
#define  leftPiezo2Pin   A6
#define  downPiezo1Pin   A5
#define  downPiezo2Pin   A4
#define  rightPiezo1Pin  A3
#define  rightPiezo2Pin  A2

// Number of samples to take for each analog piezo reading
#define  bufferSize  10

// Per-direction trigger thresholds
#define  leftTriggerThreshold   700
#define  rightTriggerThreshold  700
#define  upTriggerThreshold     700
#define  downTriggerThreshold   700

// Per-direction cooldown amounts
#define  leftTriggerCooldownTime   1000
#define  rightTriggerCooldownTime  1000
#define  upTriggerCooldownTime     1000
#define  downTriggerCooldownTime   1000

// Number of clock cycles to hold keys on for
#define  keyHoldTime  400

// Rolling averages ------------------------------------------------
// Left
int leftBuffer1[bufferSize], leftBuffer2[bufferSize];
int leftTotal1 = 0, leftTotal2 = 0;
int leftAverage1 = 0, leftAverage2 = 0;
int leftTotalAverage = 0;

// Right
int rightBuffer1[bufferSize], rightBuffer2[bufferSize];
int rightTotal1 = 0, rightTotal2 = 0;
int rightAverage1 = 0, rightAverage2 = 0;
int rightTotalAverage = 0;

// Up
int upBuffer1[bufferSize], upBuffer2[bufferSize];
int upTotal1 = 0, upTotal2 = 0;
int upAverage1 = 0, upAverage2 = 0;
int upTotalAverage = 0;

// Down
int downBuffer1[bufferSize], downBuffer2[bufferSize];
int downTotal1 = 0, downTotal2 = 0;
int downAverage1 = 0, downAverage2 = 0;
int downTotalAverage = 0;

// Global index for all rolling averages
int bufferIndex = 0;

// Non-blocking cooldown counters
int leftCooldownCounter = 0, rightCooldownCounter = 0, upCooldownCounter = 0, downCooldownCounter = 0;

boolean debug = true;

void setup() {
  // Set up all pins
  pinMode(leftPiezo1Pin, INPUT);
  pinMode(leftPiezo2Pin, INPUT);
  pinMode(rightPiezo1Pin, INPUT);
  pinMode(rightPiezo2Pin, INPUT);
  pinMode(upPiezo1Pin, INPUT);
  pinMode(upPiezo2Pin, INPUT);
  pinMode(downPiezo1Pin, INPUT);
  pinMode(downPiezo2Pin, INPUT);
  
  // Initialize buffers to be empty
  for(int i=0; i<bufferSize; i++) {
    leftBuffer1[i] = 0;
    leftBuffer2[i] = 0;
    rightBuffer1[i] = 0;
    rightBuffer2[i] = 0;
    upBuffer1[i] = 0;
    upBuffer2[i] = 0;
    downBuffer1[i] = 0;
    downBuffer2[i] = 0;
  }
  
  // Initialize serial connection for debugging
  Serial.begin(9600);
}

void loop() {
  // Get latest readings from all piezo sensors
  readLeftPiezos();
  readRightPiezos();
  readUpPiezos();
  readDownPiezos();

  // Left triggered ------------------------------------------------
  if(leftCooldownCounter == 0) {
    if(leftTotalAverage > leftTriggerThreshold) {
      // Press A key
      Keyboard.set_key1(KEY_A);
      Keyboard.send_now();
      
      delay(keyHoldTime);    // hold for a bit
      
      // Release A key
      Keyboard.set_key1(0);
      Keyboard.send_now();
      
      if(debug)
        Serial.println("LEFT detected");
      
      leftCooldownCounter = leftTriggerCooldownTime;
    }
  } else {
    leftCooldownCounter--;
  }
  
  // Right triggered ------------------------------------------------
  if(rightCooldownCounter == 0) {
    if(rightTotalAverage > rightTriggerThreshold) {
      // Press D key
      Keyboard.set_key1(KEY_D);
      Keyboard.send_now();
      
      delay(keyHoldTime);    // hold for a bit
      
      // Release D key
      Keyboard.set_key1(0);
      Keyboard.send_now();
      
      if(debug)
        Serial.println("RIGHT detected");
      
      rightCooldownCounter = rightTriggerCooldownTime;
    }
  } else {
    rightCooldownCounter--;
  }
  
  // Up triggered ------------------------------------------------
  if(upCooldownCounter == 0) {
    if(upTotalAverage > upTriggerThreshold) {
      // Press W key
      Keyboard.set_key1(KEY_W);
      Keyboard.send_now();
      
      delay(keyHoldTime);    // hold for a bit
      
      // Release W key
      Keyboard.set_key1(0);
      Keyboard.send_now();
      
      if(debug)
        Serial.println("UP detected");
      
      upCooldownCounter = upTriggerCooldownTime;
    }
  } else {
    upCooldownCounter--;
  }
  
  // Down triggered ------------------------------------------------
  if(downCooldownCounter == 0) {
    if(downTotalAverage > downTriggerThreshold) {
      // Press S key
      Keyboard.set_key1(KEY_S);
      Keyboard.send_now();
      
      delay(keyHoldTime);    // hold for a bit
      
      // Release S key
      Keyboard.set_key1(0);
      Keyboard.send_now();
      
      if(debug)
        Serial.println("DOWN detected");
      
      downCooldownCounter = downTriggerCooldownTime;
    }
  } else {
    downCooldownCounter--;
  }
  
  // Increment and wrap global buffer index ------------------------
  bufferIndex++;
  
  if(bufferIndex >= bufferSize)
    bufferIndex = 0;  

  // Output useful information to Serial ---------------------------
  if(debug) {
    Serial.print("L=");
    Serial.print(leftTotalAverage);
    Serial.print(", R=");
    Serial.print(rightTotalAverage);
    Serial.print(", U=");
    Serial.print(upTotalAverage);
    Serial.print(", D=");
    Serial.println(downTotalAverage);
  }
  
}

/**************************************************************
 Obtain readings and update rolling average for left piezos
***************************************************************/
void readLeftPiezos() {
  // Remove previous buffer entries
  leftTotal1 -= leftBuffer1[bufferIndex];
  leftTotal2 -= leftBuffer2[bufferIndex];
  
  // Obtain new readings from sensors
  leftBuffer1[bufferIndex] = analogRead(leftPiezo1Pin);
  leftBuffer2[bufferIndex] = analogRead(leftPiezo2Pin);
  
  // Append new readings to totals
  leftTotal1 += leftBuffer1[bufferIndex];
  leftTotal2 += leftBuffer2[bufferIndex];
  
  // Finding rolling average for each sensor  
  leftAverage1 = leftTotal1 / bufferSize;
  leftAverage2 = leftTotal2 / bufferSize;
  
  // Get average of both sensors
  leftTotalAverage = (leftAverage1 + leftAverage2) / 2;
}

/**************************************************************
 Obtain readings and update rolling average for right piezos
***************************************************************/
void readRightPiezos() {
  // Remove previous buffer entries
  rightTotal1 -= rightBuffer1[bufferIndex];
  rightTotal2 -= rightBuffer2[bufferIndex];
  
  // Obtain new readings from sensors
  rightBuffer1[bufferIndex] = analogRead(rightPiezo1Pin);
  rightBuffer2[bufferIndex] = analogRead(rightPiezo2Pin);
  
  // Append new readings to totals
  rightTotal1 += rightBuffer1[bufferIndex];
  rightTotal2 += rightBuffer2[bufferIndex];
  
  // Finding rolling average for each sensor  
  rightAverage1 = rightTotal1 / bufferSize;
  rightAverage2 = rightTotal2 / bufferSize;
  
  // Get average of both sensors
  rightTotalAverage = (rightAverage1 + rightAverage2) / 2;
}

/**************************************************************
 Obtain readings and update rolling average for up piezos
***************************************************************/
void readUpPiezos() {
  // Remove previous buffer entries
  upTotal1 -= upBuffer1[bufferIndex];
  upTotal2 -= upBuffer2[bufferIndex];
  
  // Obtain new readings from sensors
  upBuffer1[bufferIndex] = analogRead(upPiezo1Pin);
  upBuffer2[bufferIndex] = analogRead(upPiezo2Pin);
  
  // Append new readings to totals
  upTotal1 += upBuffer1[bufferIndex];
  upTotal2 += upBuffer2[bufferIndex];
  
  // Finding rolling average for each sensor  
  upAverage1 = upTotal1 / bufferSize;
  upAverage2 = upTotal2 / bufferSize;
  
  // Get average of both sensors
  upTotalAverage = (upAverage1 + upAverage2) / 2;
}

/**************************************************************
 Obtain readings and update rolling average for down piezos
***************************************************************/
void readDownPiezos() {
  // Remove previous buffer entries
  downTotal1 -= downBuffer1[bufferIndex];
  downTotal2 -= downBuffer2[bufferIndex];
  
  // Obtain new readings from sensors
  downBuffer1[bufferIndex] = analogRead(downPiezo1Pin);
  downBuffer2[bufferIndex] = analogRead(downPiezo2Pin);
  
  // Append new readings to totals
  downTotal1 += downBuffer1[bufferIndex];
  downTotal2 += downBuffer2[bufferIndex];
  
  // Finding rolling average for each sensor  
  downAverage1 = downTotal1 / bufferSize;
  downAverage2 = downTotal2 / bufferSize;
  
  // Get average of both sensors
  downTotalAverage = (downAverage1 + downAverage2) / 2;
}
