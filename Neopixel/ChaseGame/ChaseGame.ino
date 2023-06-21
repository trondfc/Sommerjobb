#include <Adafruit_NeoPixel.h>

#define NUMPIXELS 30 // Number of pixels in the strip
#define LED_PIN 6 // Pin for the NeoPixel strip

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);

#define BUTTON_1 2 // Pin for the left button
#define BUTTON_2 3 // Pin for the right button

#define UPDATE_DELAY 100 // Delay between updates, ms

#define SCORE_TO_WIN 5 // Score needed to win

int player1Pos = 0; // Position of player 1
int player2Pos = NUMPIXELS/2; // Position of player 2

int player1Score = 0; // Score of player 1
int player2Score = 0; // Score of player 2

unsigned long lastTime = 0; // Time of the last frame

void setup(){
    Serial.begin(9600);

    // Set up buttons with interrupts
    pinMode(BUTTON_1, INPUT_PULLUP);
    pinMode(BUTTON_2, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(BUTTON_1), button1Update, FALLING);
    attachInterrupt(digitalPinToInterrupt(BUTTON_2), button2Update, FALLING);

    strip.begin();
    strip.clear();
}

void loop(){
    if(millis() - lastTime > UPDATE_DELAY){ // Update the pixels every UPDATE_DELAY ms
        lastTime = millis();
        strip.clear();
        strip.setPixelColor(player1Pos, strip.Color(0, 0, 255));
        strip.setPixelColor(player2Pos, strip.Color(0, 255, 0));
        strip.show();
    }
}

// ISR for button 1
void button1Update(){
    player1Pos = (player1Pos + 1) % NUMPIXELS; // Increment player 1's position with wraparound
    if(player1Pos == player2Pos){ // If the players are in the same position
        player1Score++; // Increment player 1's score
        displayScore();
        if (player1Score == SCORE_TO_WIN){ // If player 1 has won
            blinkStrip(0, 0, 255, 10, 100); // Blink the strip blue
            player1Score = 0;
            player2Score = 0;
        } else {
            blinkStrip(0, 0, 255, 2, 100);
        }
        resetGame(); // Reset the game
    }
}

// ISR for button 2
void button2Update(){
    player2Pos = (player2Pos + 1) % NUMPIXELS; // Increment player 2's position with wraparound
    if(player1Pos == player2Pos){ // If the players are in the same position
        player2Score++; // Increment player 2's score
        displayScore();
        if (player2Score == SCORE_TO_WIN){ // If player 2 has won
            blinkStrip(0, 0, 255, 10, 100); // Blink the strip green
            player1Score = 0;
            player2Score = 0;
        } else {
            blinkStrip(0, 255, 0, 2, 100);
        }
        resetGame(); // Reset the game
    }
}

// Display the score on the strip
void displayScore(){
    strip.clear();
    for(int i = 0; i < player1Score; i++){ 
        strip.setPixelColor(i, strip.Color(0, 0, 255));
    }
    for(int i = NUMPIXELS - 1; i > NUMPIXELS - 1 - player2Score; i--){
        strip.setPixelColor(i, strip.Color(0, 255, 0));
    }
    strip.show();
    delay(2000);
}

// Reset the game
void resetGame(){
    player1Pos = 0;
    player2Pos = NUMPIXELS/2;
}

// Blink the strip a certain color a certain number of times with a certain delay
void blinkStrip(uint8_t R, uint8_t G, uint8_t B, int times, int delayTime){
    for(int i = 0; i < times; i++) {
        strip.fill(strip.Color(R, G, B), 0, NUMPIXELS);
        strip.show();
        delay(delayTime);
        strip.clear();
        strip.show();
        delay(delayTime);
    }
}