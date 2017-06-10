/*
 * Interactive dirt block for real-life Minecraft interface
 * =========================================================
 * 
 * Author:
 *  Jason Webb <zen.webb@gmail.com> - http://jason-webb.info
 * 
 * Description:
 *  Triggers mouse clicks based on events from a single piezo sensor 
 *  rigidly attached to the inside of a large wooden box.
 */

// Pin assignments
#define  piezoPin  A9

#define  bufferSize           10    // number of readings to take for each average
#define  triggerThreshold     500   // minimum value of piezo averages to count as a trigger
#define  triggerCooldownTime  7000  // number of clock cycles to wait before triggering again

// Rolling average variables
int buffer[bufferSize];
int total = 0;
int average = 0;

int bufferIndex = 0;

// Non-blocking cooldown counter
int cooldownCounter = 0;

boolean debug = false;

void setup() {
  // Set up all pins
  pinMode(piezoPin, INPUT);

  // Initialize buffer to be empty
  for(int i=0; i<bufferSize; i++) {
    buffer[i] = 0;
  }
  
  // Initialize serial connection for debugging
  Serial.begin(9600);
}

void loop() {
  readPiezos();

  // If cooldown has finished ...
  if(cooldownCounter == 0) {
    // Piezo event triggered
    if(average > triggerThreshold && average < 800) {
      Mouse.click();
      
      // Reset cooldown counter 
      cooldownCounter = triggerCooldownTime;
    }
  } else {
    cooldownCounter--;
  }

  // Output helpful information via Serial
  if(debug) {
    Serial.print("1=");
    Serial.print(average);
  }

}

/**********************************************************
 Obtain and smooth raw data from sensors, then calculate
 rolling average for processing
***********************************************************/
void readPiezos() {
  // Remove previous buffer entry at current position
  total -= buffer[bufferIndex];
  
  // Obtain new reading from sensor
  buffer[bufferIndex] = analogRead(piezoPin);
  
  // Append new reading to total
  total += buffer[bufferIndex];
  
  // Increment and wrap buffer index
  bufferIndex++;
  
  if(bufferIndex >= bufferSize)
    bufferIndex = 0;
  
  // Recalculate average
  average = total / bufferSize;
}
