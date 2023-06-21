#include <Adafruit_NeoPixel.h>

#define NUMPIXELS 30 // Number of pixels in the strip
#define LED_PIN 6 // Pin for the NeoPixel strip

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);

#define BUTTON 2 // Pin for the left button

unsigned long lastTime = 0; // Time of the last frame
unsigned long lastButtonTime = 0;

int playerPos = 0; // Position of the player
int targetPos = random(1, NUMPIXELS-1); // Position of the target
int speed = 5; // Speed of the player, px/s
int score = 0; // Score of the player
int highscore = 0;

bool easyMode = true; // Whether the game is in easy mode

void setup(){
    Serial.begin(9600);

    // Set up button
    pinMode(BUTTON, INPUT_PULLUP);
    
    // Set up strip
    strip.begin();
    strip.show();
}
void loop(){
    if(millis() - lastTime > 1000/speed){ // Update the pixels based on the speed
        lastTime = millis();
        strip.clear();
        playerPos = (playerPos + 1) % NUMPIXELS; // Increment player's position with wraparound
        strip.setPixelColor(targetPos, strip.Color(255, 0, 0)); // Set the target pixel to red
        if(easyMode){   // If the game is in easy mode set the pixels next to the target to red
            strip.setPixelColor(targetPos + 1, strip.Color(255, 0, 0));
            strip.setPixelColor(targetPos - 1, strip.Color(255, 0, 0));
        }
        strip.setPixelColor(playerPos, strip.Color(0, 0, 255)); // Set the player pixel to blue
        strip.show();
    }
    if(millis() - lastButtonTime > 100 && digitalRead(BUTTON) == LOW){
      lastButtonTime = millis();
      buttonUpdate();
    }
}


void buttonUpdate(){
        if(playerPos == targetPos){ // If the player is on the target
            targetHit();
        } else if (easyMode && (playerPos == targetPos + 1 || playerPos == targetPos - 1)){ // If the player is at the target in easy mode
            targetHit();
        } else {
            blinkStrip(255, 0, 0, 2, 100); // Blink red
            showScore(3000); // Show the score for 3 seconds
            resetGame(); // Reset the game
            
        }
        if(score > highscore){ // Update the highscore
          highscore = score;
        }
}

// Called when the target is hit
void targetHit(){
    speed++; // Increment the speed
    score++; // Increment the score
    Serial.print("Score: "); // Print the score
    Serial.print(score);
    Serial.print("\t Higscore:");
    Serial.println(highscore);
    targetPos = random(1, NUMPIXELS-1); // Set a new target
    blinkStrip(0, 255, 0, 2, 100); // Blink green
}

// Blinks the strip a certain color a certain number of times
void blinkStrip(int r, int g, int b, int times, int time){
    for(int i = 0; i < times; i++){
        strip.fill(strip.Color(r, g, b));
        strip.show();
        delay(time);
        strip.clear();
        strip.show();
        delay(time);
    }
}

// Shows the score for a certain amount of time
void showScore(int time){
    strip.clear();
    for (int i = 0; i < score; i++)
    {
        strip.setPixelColor(i, strip.Color(255, 255, 255));
    }
    strip.show();
    delay(time);
}

// Resets the game
void resetGame(){
    playerPos = 0;
    targetPos = random(1, NUMPIXELS-1);
    speed = 5;
    score = 0;
    strip.clear();
    strip.show();
}