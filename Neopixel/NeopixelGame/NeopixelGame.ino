/*
* Neopixel game
*
* A dungeon crawler game for a neopixel strip inspired by the Twang project
* https://github.com/bdring/TWANG/tree/master
*
* The game is controlled by 4 buttons, forward, backward, attack and defend
* The player has to handle enemies, archers and lava to survive
*
* Enemies can attack and defend and has a random chance of doing either
* is defeated when the player hits them with an attack while they aren't defending
*
* Archers can shoot arrows at the player, witch the player has to defend against
* The player can attack the archers to defeat them
*
* Lava is a hazard that the player has to avoid
* Tuns the lava on and off at random intervals
* cant be defeated and has to be avoided
*
*
* Configuration of the game can be done in the config.h file
*
*
* Created by: Trond F. C.
* Date: 2023-06-27
*/

#include "FastLED.h"

// Include the neopixel game classes
#include "config.h"
#include "debug.h"
#include "button.h"
#include "enemy.h"
#include "lava.h"
#include "player.h"
#include "archer.h"
#include "arrow.h"
#include "deathAnimation.h"


CRGB leds[NUMPIXELS];   // neopixel array

debug Debug(DEBUG_LEVEL, "NeopixelGame");   // create debug object for main class

// create player object with button pins and health
player Player1(PIN_BUTTON_FORWARD, PIN_BUTTON_BACKWARD, PIN_BUTTON_ATTACK, PIN_BUTTON_DEFEND, 30);

// create lava objects
#define LAVA_AMOUNT 4
lava Lava[LAVA_AMOUNT];

// create enemy objects
#define ENEMY_AMOUNT 5
enemy Enemys[ENEMY_AMOUNT];

// create death animation objects
#define DEATH_ANIMATION_AMOUNT 2
deathAnimation DeathAnimations[DEATH_ANIMATION_AMOUNT];

// create archer and arrow objects
// Artchers makes the game a lot harder, be carful to not add to many
#define ARCHER_AMOUNT 2
archer Archers[ARCHER_AMOUNT];
arrow Arrows[ARCHER_AMOUNT];

void setup(){
    Debug.begin(9600);  // start serial debug
    Debug.println("Starting up", DEBUG_INFO);   // print startup message

    // initialize neopixel strip
    FastLED.addLeds<WS2811, PIN_NEOPIXEL, GBR>(leds, NUMPIXELS).setCorrection( TypicalLEDStrip );

    // initialize player
    Player1.begin();

    /*
    * Initialize lava
    *
    * Lava at position 5 with a size of 3
    * Lava at position 20 with a size of 2, custom on/off times
    * Lava at position 35 with a size of 6
    * Lava at position 47 with a size of 4, custom on/off times
    */
    Lava[0].start(5, 3);    
    Lava[1].start(20, 2, 2000, 5000, 500, 1000);
    Lava[2].start(35, 6);
    Lava[3].start(47, 4, 500, 2000, 2000, 5000);

    /*
    * Initialize enemies
    *
    * Enemy at position 10, 14, 25, 43, 45
    */    
    Enemys[0].start(10);
    Enemys[1].start(14);
    Enemys[2].start(25);
    Enemys[3].start(43);
    Enemys[4].start(45);

    /*
    * Initialize archers
    *
    * Archer at position 30, custom atack times
    * Archer at position 55
    */
    Archers[0].start(30, ARCHER_ACTION_MIN_TIME*2, ARCHER_ACTION_MAX_TIME*2);
    Archers[1].start(55);

}

void loop(){
    FastLED.clear();    // Clear all pixel data

    Player1.update();   // update player

    // process all death animation objects
    for(int x = 0; x < DEATH_ANIMATION_AMOUNT; x++){    // loop through all death animation objects
        if(DeathAnimations[x].isActive()){  // only process active death animations
            DeathAnimations[x].update();    // update death animation
            // draw all pixels of the death animation
            for(int i = 0; i < DEATH_ANIMATION_SIZE; i++){
                setPixel(DeathAnimations[x].getPos() + i, DeathAnimations[x].pixelColors[i]);
            }
        }
    }

    // process all lava objects
    for(int x = 0; x < LAVA_AMOUNT; x++){   // loop through all lava objects
        if(Lava[x].isActive()){ // only process active lava objects
            Lava[x].update();   // update lava
            if(Lava[x].isColliding(Player1.getPos())){  // check if player is colliding with lava
                // start death animation at player position with player death color
                // and hit player
                startDeathAnimation(Player1.getPos(), COLOR_PLAYER_DEATH);
                Player1.hit();
            }
            // draw all pixels of the lava
            for (int i = 0; i < Lava[x].getSize(); i++)
            {
                setPixel(Lava[x].getPos() + i, Lava[x].getColor());
            }
        }
    }

    // process all enemy objects
    for(int x = 0; x < ENEMY_AMOUNT; x++){      // loop through all enemy objects
        if(Enemys[x].isActive()){   // only process active enemy objects
            Enemys[x].update();     // update enemy
            if(Enemys[x].isColliding(Player1.getPos())){    // check if player is colliding with enemy
                // start death animation at player position with player death color
                // and hit player
                startDeathAnimation(Player1.getPos(), COLOR_PLAYER_DEATH);
                Player1.hit();
            }
            if(Enemys[x].isHiting(Player1.getPos())){   // check if enemy is hitting player
                if(!Player1.isDefending()){             // check if player is not defending
                    // start death animation at player position with player death color
                    // and hit player
                    startDeathAnimation(Player1.getPos(), COLOR_PLAYER_DEATH);
                    Player1.hit();
                }
            }
            if(Player1.isHiting(Enemys[x].getPos())){   // check if player is hitting enemy
                if(!Enemys[x].isDefending()){           // check if enemy is not defending
                    // start death animation at enemy position with enemy death color
                    // and hit enemy
                    startDeathAnimation(Enemys[x].getPos(), COLOR_ENEMY_DEATH);
                    Enemys[x].hit();
                }
            
            }
            // draw all pixels of the enemy
            for(int i = 0; i < Enemys[x].getSize(); i++){
                setPixel(Enemys[x].getPos() - i, Enemys[x].getColor());
            }
        }
    }

    // process all archer and arrow objects
    for(int x = 0; x < ARCHER_AMOUNT; x++){    // loop through all archer and arrow objects
        if(Archers[x].isActive()){      // only process active archer objects
            Archers[x].update();        // update archer
            if(Archers[x].isColliding(Player1.getPos())){   // check if player is colliding with archer
                // start death animation at player position with player death color
                // and hit player
                startDeathAnimation(Player1.getPos(), COLOR_PLAYER_DEATH);
                Player1.hit();
            }
            if(Archers[x].isShooting()){    // check if archer is shooting
                Arrows[x].start(Archers[x].getPos());   // start arrow at archer position
            }
            if(Player1.isHiting(Archers[x].getPos())){  // check if player is hitting archer
                // start death animation at archer position with archer death color
                // and hit archer
                startDeathAnimation(Archers[x].getPos(), COLOR_ARCHER_DEATH);
                Archers[x].hit();
            }
            // draw all pixels of the archer
            setPixel(Archers[x].getPos(), Archers[x].getColor());
        }
        if(Arrows[x].isActive()){   // only process active arrow objects
            Arrows[x].update();     // update arrow
            if(Arrows[x].isColliding(Player1.getPos())){    // check if player is colliding with arrow
                if(!Player1.isDefending()){                 // check if player is not defending
                    // start death animation at player position with player death color
                    // and hit player
                    startDeathAnimation(Player1.getPos(), COLOR_PLAYER_DEATH);
                    Player1.hit();
                }
            }
            // draw all pixels of the arrow
            setPixel(Arrows[x].getPos(), Arrows[x].getColor());
        }
    }

    // draw all pixels of the player
    for(int i = 0; i < Player1.getSize(); i++){
        setPixel(Player1.getPos() + i, Player1.getColor());
    }
    
    FastLED.show(); // show all pixel data

}

// Function to set a pixel to a color
void setPixel(int Pixel, uint32_t color) {
    // Split incoming color into RGB values
    byte red = color & 0xFF;
    byte green = (color >> 8) & 0xFF;
    byte blue = (color >> 16) & 0xFF;
    // set pixel to color
    leds[Pixel].r = red;
    leds[Pixel].g = green;
    leds[Pixel].b = blue;
}

// Function to start a death animation
// finds a inactive death animation object and starts it
void startDeathAnimation(int pos, uint32_t color){
    for(int x = 0; x < DEATH_ANIMATION_AMOUNT; x++){
        if(!DeathAnimations[x].isActive()){
            DeathAnimations[x].start(pos, color);
            return;
        }
    }
}