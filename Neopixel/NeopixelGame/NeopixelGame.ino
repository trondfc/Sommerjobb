#include "FastLED.h"

#include "config.h"
#include "debug.h"
#include "button.h"
#include "enemy.h"
#include "lava.h"
#include "player.h"
#include "deathAnimation.h"


CRGB leds[NUMPIXELS];

//Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, PIN_NEOPIXEL, NEO_GRB + NEO_KHZ800);

debug Debug(DEBUG_LEVEL, "NeopixelGame");

player Player1(PIN_BUTTON_FORWARD, PIN_BUTTON_BACKWARD, PIN_BUTTON_ATTACK, PIN_BUTTON_DEFEND, 30);

#define LAVA_AMOUNT 4
lava Lava[LAVA_AMOUNT];

#define ENEMY_AMOUNT 5
enemy Enemys[ENEMY_AMOUNT];

#define DEATH_ANIMATION_AMOUNT 2
deathAnimation DeathAnimations[DEATH_ANIMATION_AMOUNT];

void setup(){
    Debug.begin(9600);
    Debug.println("Starting up", DEBUG_INFO);

    FastLED.addLeds<WS2811, PIN_NEOPIXEL, GBR>(leds, NUMPIXELS).setCorrection( TypicalLEDStrip );

    Player1.begin();

    Lava[0].start(55, 1);
    Lava[1].start(56, 1, 2000, 5000, 200, 1000);
    Lava[2].start(57, 1);
    Lava[3].start(58, 1, 500, 2000, 2000, 5000);

    Enemys[0].start(5);
    Enemys[1].start(10);
    Enemys[2].start(15);
    Enemys[3].start(20);
    Enemys[4].start(25);

}

void loop(){
    FastLED.clear();
    Player1.update();


    for(int x = 0; x < LAVA_AMOUNT; x++){
        if(Lava[x].isActive()){
            Lava[x].update();
            if(Lava[x].isColliding(Player1.getPos())){
                startDeathAnimation(Player1.getPos(), COLOR_PLAYER_DEATH);
                Player1.hit();
            }
            for (int i = 0; i < Lava[x].getSize(); i++)
            {
                setPixel(Lava[x].getPos() + i, Lava[x].getColor());
            }
        }
    }

    for(int x = 0; x < DEATH_ANIMATION_AMOUNT; x++){
        if(DeathAnimations[x].isActive()){
            DeathAnimations[x].update();
            for(int i = 0; i < DEATH_ANIMATION_SIZE; i++){
                setPixel(DeathAnimations[x].getPos() + i, DeathAnimations[x].pixelColors[i]);
            }
        }
    }

    for(int x = 0; x < ENEMY_AMOUNT; x++){
        if(Enemys[x].isActive()){
            Enemys[x].update();
            if(Enemys[x].isColliding(Player1.getPos())){
                startDeathAnimation(Player1.getPos(), COLOR_PLAYER_DEATH);
                Player1.hit();
            }
            if(Enemys[x].isHiting(Player1.getPos())){
                if(!Player1.isDefending()){
                    startDeathAnimation(Player1.getPos(), COLOR_PLAYER_DEATH);
                    Player1.hit();
                }
            }
            if(Player1.isHiting(Enemys[x].getPos())){
                if(!Enemys[x].isDefending()){
                    startDeathAnimation(Enemys[x].getPos(), COLOR_ENEMY_DEATH);
                    Enemys[x].hit();
                }
            
            }
            for(int i = 0; i < Enemys[x].getSize(); i++){
                setPixel(Enemys[x].getPos() - i, Enemys[x].getColor());
            }
        }
    }

    for(int i = 0; i < Player1.getSize(); i++){
        setPixel(Player1.getPos() + i, Player1.getColor());
    }
    
    FastLED.show();

}

void setPixel(int Pixel, uint32_t color) {
  byte red = color & 0xFF;
  byte green = (color >> 8) & 0xFF;
  byte blue = (color >> 16) & 0xFF;
   // FastLED
   leds[Pixel].r = red;
   leds[Pixel].g = green;
   leds[Pixel].b = blue;
}

void startDeathAnimation(int pos, uint32_t color){
    for(int x = 0; x < DEATH_ANIMATION_AMOUNT; x++){
        if(!DeathAnimations[x].isActive()){
            DeathAnimations[x].start(pos, color);
            return;
        }
    }
}