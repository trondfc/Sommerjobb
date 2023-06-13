#include <Stepper.h> // Using the Stepper library from Arduino IDE

const int stepsPerRevolution = 200;  // Number of steps per revolution, checked against motors form Even

// initialize the stepper library on pins 8 through 11: (in1, in2, in3, in4)
Stepper myStepper(stepsPerRevolution, 8, 9, 10, 11);

void setup() {
  // set the speed at 60 rpm:
  myStepper.setSpeed(60);
  // initialize the serial port:
  Serial.begin(9600);
}

void loop() {
  // step one revolution  in one direction:
  Serial.println("clockwise");
  myStepper.step(stepsPerRevolution);
  delay(500);

  // step one revolution in the other direction:
  Serial.println("counterclockwise");
  myStepper.step(-stepsPerRevolution);
  delay(500);
}