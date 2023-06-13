#include <Stepper.h> // Using the Stepper library from Arduino IDE

const int stepsPerRevolution = 200; // Number of steps per revolution, checked against motors form Even

// initialize the stepper library on pins 8 through 11: (in1, in2, in3, in4)
Stepper myStepper(stepsPerRevolution, 8, 9, 10, 11);

int stepCount = 0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  // step one step:
  myStepper.step(1);
  Serial.print("steps:");
  Serial.println(stepCount);
  stepCount++;
  delay(500);
}