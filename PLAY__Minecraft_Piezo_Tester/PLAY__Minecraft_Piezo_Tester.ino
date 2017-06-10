#define  piezo1Pin  A9
#define  piezo2Pin  A8

#define  bufferSize  10

int buffer1[bufferSize], buffer2[bufferSize];
int total1 = 0, total2 = 0;
int average1 = 0, average2 = 0;
int totalAverage = 0;

int bufferIndex = 0;

void setup() {
  pinMode(piezo1Pin, INPUT);
  pinMode(piezo2Pin, INPUT);

  // Initialize buffers to be empty
  for(int i=0; i<bufferSize; i++) {
    buffer1[i] = 0;
    buffer2[i] = 0;
  }
  
  // Initialize serial connection for debugging
  Serial.begin(115200);
}

void loop() {
  readPiezos();

  Serial.print(average1);
  Serial.print(" ");
  Serial.print(average2);
  Serial.print(" ");
  Serial.println(totalAverage);
}

void readPiezos() {
  // Remove previous buffer entries
  total1 -= buffer1[bufferIndex];
  total2 -= buffer2[bufferIndex];
  
  // Obtain new readings from sensors
  buffer1[bufferIndex] = analogRead(piezo1Pin);
  buffer2[bufferIndex] = analogRead(piezo2Pin);
  
  // Append new readings to totals
  total1 += buffer1[bufferIndex];
  total2 += buffer2[bufferIndex];
  
  // Finding rolling average for each sensor  
  average1 = total1 / bufferSize;
  average2 = total2 / bufferSize;
  
  // Get average of both sensors
  totalAverage = (average1 + average2) / 2;
}
