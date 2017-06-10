/*
 * Jump pad for real-life Minecraft interface
 * ==========================================
 * 
 * Author:
 *  Jason Webb <zen.webb@gmail.com> - http://jason-webb.info
 * 
 * Description:
 *  Triggers space bar keypresses based on events from two piezo 
 *  sensors rigidly mounted underneath a flexible plywood sheet.
 */

// Pin assignments
#define  leftPiezoPin     A9
#define  rightPiezoPin    A8

#define  bufferSize           10    // number of readings to take for each average
#define  triggerThreshold     550   // minimum value of piezo averages to count as a trigger
#define  triggerCooldownTime  10000  // number of clock cycles to wait before triggering again
#define  keyHoldTime          100   // number of clock cycles to hold key for when triggered

// Rolling average variables
int leftBuffer[bufferSize], rightBuffer[bufferSize];
int leftTotal = 0, rightTotal = 0;
int leftAverage = 0, rightAverage = 0;
int totalAverage = 0;
int bufferIndex = 0;

// Non-blocking cooldown counter
int cooldownCounter = 0;

boolean debug = true;

void setup() {
  // Set up all pins
  pinMode(leftPiezoPin, INPUT);
  pinMode(rightPiezoPin, INPUT);

  // Initialize buffers to be empty
  for(int i=0; i<bufferSize; i++) {
    leftBuffer[i] = 0;
    rightBuffer[i] = 0; 
  }
  
  // Initialize serial connection for debugging
  Serial.begin(9600);
}

void loop() {
  readPiezos();

  // Piezo trigger cooldown ----------------------------------------------
  if(cooldownCounter == 0) {
    // Piezo event triggered
    if(totalAverage > triggerThreshold) {
      // Press Space key
      Keyboard.set_key1(KEY_SPACE);
      Keyboard.send_now();
      
      delay(keyHoldTime);    // hold for a bit
      
      // Release Space key
      Keyboard.set_key1(0);
      Keyboard.send_now();
      
      if(debug)
        Serial.println("JUMP triggered");
      
      // Reset cooldown counter 
      cooldownCounter = triggerCooldownTime;
    }
    
  } else {
    cooldownCounter--;
  }

  // Output helpful information via Serial -------------------------------
  if(debug) {
    Serial.print("L = ");
    Serial.print(leftAverage);
    Serial.print(", R = ");
    Serial.print(rightAverage);
    Serial.print(", AVG = ");
    Serial.println(totalAverage);
  }
}

/**********************************************************
 Obtain and smooth raw data from sensors, then calculate
 rolling average for processing
***********************************************************/
void readPiezos() {
  // Remove previous buffer entries
  leftTotal -= leftBuffer[bufferIndex];
  rightTotal -= rightBuffer[bufferIndex];
  
  // Obtain new readings from sensors
  leftBuffer[bufferIndex] = analogRead(leftPiezoPin);
  rightBuffer[bufferIndex] = analogRead(rightPiezoPin);
  
  // Append new readings to totals
  leftTotal += leftBuffer[bufferIndex];
  rightTotal += rightBuffer[bufferIndex];
  
  // Increment and wrap buffer index
  bufferIndex++;
  
  if(bufferIndex >= bufferSize)
    bufferIndex = 0;
  
  // Finding rolling average for each sensor  
  leftAverage = leftTotal / bufferSize;
  rightAverage = rightTotal / bufferSize;
  
  // Get average of both sensors
  totalAverage = (leftAverage + rightAverage) / 2;
}
