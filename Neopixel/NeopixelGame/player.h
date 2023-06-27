/*
* player.h
*
* Header file for player class
* Creates a player object that can be used to display a player on a neopixel strip
*/

#pragma once

#include <Arduino.h>
#include "button.h"         // include button class
#include "debug.h"          // include debug class

debug DebugPlayer(DEBUG_LEVEL, "player.h");   // create debug object for player class

// player class
class player
{
    public:
        // constructor, takes 4 button pins and number of lives
        player(int forwardPin, int backwardPin, int attackPin, int defendPin, int lives);
        void begin();   // begin player
        void update();  // update player

        int getPos();           // get player position
        void setPos(int pos);   // set player position
        int getSize();          // get player size
        uint32_t getColor();    // get player color
        int getLives();         // get player lives

        bool hit();             // hit player
        bool isHiting(int pos); // check if player is hiting a position
        bool isDefending();     // check if player is defending

    private:
        void setColor(uint32_t color);  // set player color
        void moveForward();             // move player forward
        void moveBackward();            // move player backward
        void attack();                  // start player attack
        void defend();                  // start player defend

        void updateDefend();            // update player defend
        void updateAttack();            // update player attack

        button _forwardButton;          // forward button
        button _backwardButton;         // backward button
        button _attackButton;           // attack button
        button _defendButton;           // defend button

        enum attackState                // attack state enum
        {
            ATTACK_STATE_NONE,          // no attack
            ATTACK_STATE_CHARGING,      // charging attack
            ATTACK_STATE_ATTACKING      // attacking
        };
        attackState _attacking;         // attack state
        unsigned long _attackStart;     // attack start time

        int _pos;                       // player position
        int _size;                      // player size
        int _lives;                     // player lives
        bool _alive;                    // player alive state
        bool _defending;                // player defending state
        unsigned long _defendStart;     // defend start time
       
        uint32_t _color;                // player color

};

// constructor, takes 4 button pins and number of lives
player::player(int forwardPin, int backwardPin, int attackPin, int defendPin, int lives)
{
    // setup buttons
    _forwardButton.setupButton(forwardPin, BUTTON_DEBOUNCE_TIME);   
    _backwardButton.setupButton(backwardPin, BUTTON_DEBOUNCE_TIME);
    _attackButton.setupButton(attackPin, BUTTON_DEBOUNCE_TIME);
    _defendButton.setupButton(defendPin, BUTTON_DEBOUNCE_TIME);
    _pos = 0;               // set player position
    _size = 1;              // set player size
    _lives = lives;         // set player lives
    _alive = true;          // set player alive state
    _color = COLOR_PLAYER;  // set player color
}

// begin player
void player::begin()
{
    // begin buttons
    _forwardButton.begin();
    _backwardButton.begin();
    _attackButton.begin();
    _defendButton.begin();
}

// update player
void player::update()
{   
    // check if _forwardButton has been pressed and player is not defending or attacking
    if (_forwardButton.hasBeenPressed() && !_defending && _attacking == ATTACK_STATE_NONE)
    {
        moveForward();  // move player forward
    }
    // check if _backwardButton has been pressed and player is not defending or attacking
    if (_backwardButton.hasBeenPressed()&& !_defending && _attacking == ATTACK_STATE_NONE)
    {
        moveBackward(); // move player backward
    }
    // check if _attackButton has been pressed and player is not defending or attacking
    if (_attackButton.hasBeenPressed()&& !_defending && _attacking == ATTACK_STATE_NONE)
    {
        attack();   // start attack
    }
    // check if _defendButton has been pressed and player is not defending
    if (_defendButton.hasBeenPressed()&& !_defending)
    {
        _attacking = ATTACK_STATE_NONE; // stop attack
        _size = 1;                      // set player size to 1
        defend();                       // start defend            
    }
    updateDefend();     // update defend
    updateAttack();     // update attack
}

// get player position
int player::getPos()
{
    return _pos;
}

// set player position
void player::setPos(int pos)
{
    _pos = pos;
}

// get player size
int player::getSize()
{
    return _size;
}

// get player color
uint32_t player::getColor()
{
    return _color;
}

// get player lives
int player::getLives()
{
    return _lives;
}

// hit player
bool player::hit()
{
    _lives--;           // decrement lives
    _pos = 0;           // set player position to 0
    DebugPlayer.println("Player hit, lives: " + String(_lives), DEBUG_INFO);
    if (_lives <= 0)    // check if player has no lives left
    {
        DebugPlayer.println("Player dead", DEBUG_INFO);
        _alive = false; // set player alive state to false
        _pos = -10;     // move player off screen
    }
    return true;
}

// check if player is hiting a position
bool player::isHiting(int pos)
{
    if(_attacking != ATTACK_STATE_ATTACKING){   // return false if player is not attacking
        return false;
    }
    if(pos >= _pos && pos <= _pos + _size){    // check if position is within players attack range
        return true;
    } else {
        return false;
    }
}

// check if player is defending
bool player::isDefending()
{
    return _defending;
}

// set player color
void player::setColor(uint32_t color)
{
    _color = color;
}

// move player forward
void player::moveForward()
{
    DebugPlayer.print("Moving forward, pos is:", DEBUG_DEBUG);
    DebugPlayer.println(String(_pos), DEBUG_DEBUG);
    if (_pos < NUMPIXELS - 1)   // check if player is not at the end of the strip
    {
        _pos++;
    }
}

// move player backward
void player::moveBackward()
{
    DebugPlayer.print("Moving backward, pos is:", DEBUG_DEBUG);
    DebugPlayer.println(String(_pos), DEBUG_DEBUG);
    if (_pos > 0)   // check if player is not at the start of the strip
    {
        _pos--;
    }
}

// start attack
void player::attack()
{
    DebugPlayer.println("Attacking", DEBUG_DEBUG);
    _attacking = ATTACK_STATE_CHARGING;     // set attack state to charging
    _size = PLAYER_ATTACK_RANGE;            // set player size to attack range
    _attackStart = millis();                // set attack start time
    setColor(COLOR_PLAYER_CHARGE);          // set player color to attack charge color
}

// update attack
void player::updateAttack()
{
    if(_attacking == ATTACK_STATE_NONE){    // return if player is not attacking
        return;
    }
    // check if attack is charged
    if(_attacking == ATTACK_STATE_CHARGING && millis() - _attackStart > PLAYER_CHARGE_TIME){
        DebugPlayer.println("Attack charged", DEBUG_DEBUG);
        _attacking = ATTACK_STATE_ATTACKING;    // set attack state to attacking
        _attackStart = millis();
        setColor(COLOR_PLAYER_ATTACK);          // set player color to attack color
    }
    // check if attack is done
    if(_attacking == ATTACK_STATE_ATTACKING && millis() - _attackStart > PLAYER_ATTACK_TIME){
        DebugPlayer.println("Attack done", DEBUG_DEBUG);
        _size = 1;                      // set player size back to 1
        _attacking = ATTACK_STATE_NONE; // set attack state to none
        setColor(COLOR_PLAYER);         // set player color to normal color
    }
}

// start defend
void player::defend()
{
    DebugPlayer.println("Defending", DEBUG_DEBUG);
    _defending = true;              // set defending state to true
    _defendStart = millis();        // set defend start time
    setColor(COLOR_PLAYER_DEFEND);  // set player color to defend color
}

// update defend
void player::updateDefend()
{
    if(!_defending){        // return if player is not defending
        return;
    }
    // check if defend is done
    if(millis() - _defendStart > PLAYER_DEFEND_TIME){
        DebugPlayer.println("Done defending", DEBUG_DEBUG);
        _defending = false;         // set defending state to false
        setColor(COLOR_PLAYER);     // set player color to normal color
    }
}