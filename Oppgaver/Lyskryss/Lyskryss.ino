
// Define pins
#define DIR1RED 2
#define DIR1YELLOW 3
#define DIR1GREEN 4

#define DIR2RED 5
#define DIR2YELLOW 6
#define DIR2GREEN 7

// Define time between light changes
#define LIGHT_TIME 2000
unsigned long lastTime = 0;

// Define the light states
enum lightState{RED, YELLOW, GREEN};
lightState dir1_lightState = RED;
lightState dir2_lightState = GREEN;

// Define the direction of the lights
bool dir1_towardsGreen = true;
bool dir2_towardsGreen = false;

void setup(){
  Serial.begin(9600);

  // Set the pins as outputs
  pinMode(DIR1RED, OUTPUT);
  pinMode(DIR1YELLOW, OUTPUT);
  pinMode(DIR1GREEN, OUTPUT);
  pinMode(DIR2RED, OUTPUT);
  pinMode(DIR2YELLOW, OUTPUT);
  pinMode(DIR2GREEN, OUTPUT);
}

void loop(){
  // Check if it is time to change the lights
  if(millis() - lastTime > LIGHT_TIME){
    lastTime = millis();
    // Direction 1
    switch (dir1_lightState) {
      case RED: // If the state is red
        // Turn on the red light and turn off the others
        digitalWrite(DIR1RED, HIGH);
        digitalWrite(DIR1YELLOW, LOW);
        digitalWrite(DIR1GREEN, LOW);
        // Set the next state to yellow and set the direction to towards green
        dir1_lightState = YELLOW;
        dir1_towardsGreen = true;
        break;
      case YELLOW:  // If the state is yellow
        // Turn on the yellow light and turn off the green
        digitalWrite(DIR1YELLOW, HIGH);
        digitalWrite(DIR1GREEN, LOW);
        if(dir1_towardsGreen){  // If the direction is towards green
          // Turn on the red light, and set the next state to green
          digitalWrite(DIR1RED, HIGH);
          dir1_lightState = GREEN;
        } else { // If the direction is towards red
          // Turn off the red light, and set the next state to red
          digitalWrite(DIR1RED, LOW);
          dir1_lightState = RED;
        }
        break;
      case GREEN:   // If the state is green
        // Turn on the green light and turn off the others
        digitalWrite(DIR1RED, LOW);
        digitalWrite(DIR1YELLOW, LOW);
        digitalWrite(DIR1GREEN, HIGH);
        // Set the next state to yellow and set the direction to towards red
        dir1_lightState = YELLOW;
        dir1_towardsGreen = false;
        break;
    }

    // Direction 2
    switch (dir2_lightState) {
      case RED: // If the state is red
        // Turn on the red light and turn off the others
        digitalWrite(DIR2RED, HIGH);
        digitalWrite(DIR2YELLOW, LOW);
        digitalWrite(DIR2GREEN, LOW);
        // Set the nest state to yellow and set the direction to towards green
        dir2_lightState = YELLOW;
        dir2_towardsGreen = true;
        break;
      case YELLOW:  // If the state is yellow
        // Turn on the yellow light and turn off the green
        digitalWrite(DIR2YELLOW, HIGH);
        digitalWrite(DIR2GREEN, LOW);
        if(dir2_towardsGreen){  // If the direction is towards green
          // Turn on the red light, and set the next state to green
          digitalWrite(DIR2RED, HIGH);
          dir2_lightState = GREEN;
        } else {    // If the direction is towards red
          // Turn off the red light, and set the next state to red
          digitalWrite(DIR2RED, LOW);
          dir2_lightState = RED;
        }
        break;
      case GREEN:   // If the state is green
        // Turn on the green light and turn off the others
        digitalWrite(DIR2RED, LOW);
        digitalWrite(DIR2YELLOW, LOW);
        digitalWrite(DIR2GREEN, HIGH);
        // Set the next state to yellow and set the direction to towards red
        dir2_lightState = YELLOW;
        dir2_towardsGreen = false;
        break;
    }
  }
}