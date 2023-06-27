/*
* config.h
*
* Configuration file for the NeopixelGame
*/


#define DEBUG_LEVEL DEBUG_INFO // Debug level

//////////////////////////////////////////
// IO
//////////////////////////////////////////
#define NUMPIXELS 60            // Number of pixels in the strip
#define PIN_NEOPIXEL 9          // Pin for the neopixel strip

#define PIN_BUTTON_FORWARD 2    // Pin for the forward button
#define PIN_BUTTON_BACKWARD 3   // Pin for the backward button
#define PIN_BUTTON_ATTACK 4     // Pin for the attack button
#define PIN_BUTTON_DEFEND 5     // Pin for the defend button

#define BUTTON_DEBOUNCE_TIME 50 // Debounce time for the buttons

//////////////////////////////////////////
// DEFAULTS
//////////////////////////////////////////
#define DEFAULT_ATACK_RANGE 2           // Default attack range 
#define DEFAULT_CHARGE_TIME 500         // Default charge time
#define DEFAULT_ATTACK_TIME 100         // Default attack time
#define DEFAULT_DEFEND_TIME 500         // Default defend time

#define DEATH_ANIMATION_MIN_TIME 500    // Minimum time for the death animation
#define DEATH_ANIMATION_MAX_TIME 2000   // Maximum time for the death animation
#define DEATH_ANIMATION_SIZE 4          // Size of the death animation

//////////////////////////////////////////
// PLAYER
//////////////////////////////////////////
#define PLAYER_ATTACK_RANGE DEFAULT_ATACK_RANGE     // Player attack range
#define PLAYER_CHARGE_TIME DEFAULT_CHARGE_TIME*1.5    // Player charge time
#define PLAYER_ATTACK_TIME DEFAULT_ATTACK_TIME      // Player attack time
#define PLAYER_DEFEND_TIME DEFAULT_DEFEND_TIME      // Player defend time

#define COLOR_PLAYER 0x0000FF           // Player color
#define COLOR_PLAYER_CHARGE 0x1a1a1a    // Player charge color
#define COLOR_PLAYER_ATTACK 0xFFFFFF    // Player attack color
#define COLOR_PLAYER_DEFEND 0x3300FF    // Player defend color
#define COLOR_PLAYER_DEATH 0x000026     // Player death color

//////////////////////////////////////////
// ENEMY
//////////////////////////////////////////
#define ENEMY_ATTACK_RANGE DEFAULT_ATACK_RANGE  // Enemy attack range
#define ENEMY_CHARGE_TIME DEFAULT_CHARGE_TIME   // Enemy charge time
#define ENEMY_ATTACK_TIME DEFAULT_ATTACK_TIME   // Enemy attack time
#define ENEMY_DEFEND_TIME DEFAULT_DEFEND_TIME   // Enemy defend time
#define ENEMY_ACTION_MIN_TIME 750               // Minimum time between the enemy actions
#define ENEMY_ACTION_MAX_TIME 2500              // Maximum time between the enemy actions
#define ENEMY_ATTACK_CHANCE 75                  // Chance of the enemy attacking vs defending (0-100)

#define COLOR_ENEMY 0xFF0000            // Enemy color
#define COLOR_ENEMY_CHARGE 0x4f0000     // Enemy charge color
#define COLOR_ENEMY_ATTACK 0xff8808     // Enemy attack color
#define COLOR_ENEMY_DEFEND 0xff4d4d     // Enemy defend color
#define COLOR_ENEMY_DEATH 0x260000      // Enemy death color

//////////////////////////////////////////
// ARCHER
//////////////////////////////////////////
#define ARCHER_CHARGE_TIME DEFAULT_CHARGE_TIME  // Archer charge time
#define ARCHER_ACTION_MIN_TIME 5000              // Minimum time between the archer actions
#define ARCHER_ACTION_MAX_TIME 20000            // Maximum time between the archer actions

#define ARROW_SPEED 10                           // Archer arrow speed px/s

#define COLOR_ARCHER 0x00FF00          // Archer color
#define COLOR_ARCHER_CHARGE 0x4d4d00   // Archer charge color
#define COLOR_ARCHER_DEATH 0x002600    // Archer death color

#define COLOR_ARROW 0x00FF00           // Arrow color

//////////////////////////////////////////
// LAVA
//////////////////////////////////////////
#define LAVA_OFF_MIN 1000           // Lava off time minimum
#define LAVA_OFF_MAX 5000           // Lava off time maximum
#define LAVA_ON_MIN 1000            // Lava on time minimum
#define LAVA_ON_MAX 5000            // Lava on time maximum

#define COLOR_LAVA_ON 0x5c4200    // Lava on color
#define COLOR_LAVA_OFF 0x241a00     // Lava off color








