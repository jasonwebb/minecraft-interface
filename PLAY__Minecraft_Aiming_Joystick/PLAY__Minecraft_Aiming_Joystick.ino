/*
 * Aiming joystick for real-life Minecraft interface
 * =================================================
 * 
 * Author:
 *  Jason Webb <zen.webb@gmail.com> - http://jason-webb.info
 * 
 * Description:
 *  Translates interactions with a classical potentiometer-based joystick into mouse
 *  movements using the USB HID class provided by Teensyduino, not unlike flight controls.
 *  
 *  Mouse will be moved quickly or slowly based on how far the joystick is tilted.
 */

// Pin definitions
#define horizontalPin A9
#define verticalPin A8
#define debug false

// Current position of potentiometers
int horizontalPosition, 
    verticalPosition;

// Amount of pixels to move the mouse 
int smallMovementAmount = 1,
    largeMovementAmount = 3;

void setup() {
  pinMode(horizontalPin, INPUT);
  pinMode(verticalPin, INPUT);
  
  Serial.begin(9600);
}

void loop() {
  // Get current position of potentiometers
  horizontalPosition = analogRead(horizontalPin);
  verticalPosition = analogRead(verticalPin);
  
  // Turn to the left --------------------------------------------------
  if(horizontalPosition > 600 && horizontalPosition < 900)
    Mouse.move(-smallMovementAmount, 0);
  else if(horizontalPosition > 900)
    Mouse.move(-largeMovementAmount, 0);
  
  // Turn to the right -------------------------------------------------
  else if(horizontalPosition < 480 && horizontalPosition > 200)
    Mouse.move(smallMovementAmount, 0);
  else if(horizontalPosition < 200)
    Mouse.move(largeMovementAmount, 0);
  
  // Look up -----------------------------------------------------------
  if(verticalPosition > 700 && verticalPosition < 900)
    Mouse.move(0, -smallMovementAmount);
  else if(verticalPosition > 900)
    Mouse.move(0, -largeMovementAmount);

  // Look down ---------------------------------------------------------
  else if(verticalPosition < 550 && verticalPosition > 200)
    Mouse.move(0, smallMovementAmount);
  else if(verticalPosition < 200)
    Mouse.move(0, largeMovementAmount);
    
    
  if(debug) {
    Serial.print(horizontalPosition);
    Serial.print(" ");
    Serial.println(verticalPosition);
  }
  
}
