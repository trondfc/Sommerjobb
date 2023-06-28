/*
* archer.h
*
* Header file for archer class
* creates an archer object for the neopixel game
*/

#pragma once

#include <Arduino.h>
#include "debug.h"  // include debug class

debug DebugArcher(DEBUG_LEVEL, "archer.h");    // create debug object for enemy class

// archer class
class archer
{
    public:
        archer();   // constructor, unused
        void start(int pos);    // start the archer at a position with default attack times
        void start(int pos, int minAttackTime, int maxAttackTime);  // start the archer at a position with custom attack times
        void remove();  // remove the archer
        void update();  // update the archer

        bool isColliding(int pos);  // check if the archer is colliding with a position
        bool isActive();            // check if the archer is active
        bool isShooting();          // check if the archer is shooting

        int getPos();               // get the archer's position
        void setPos(int pos);       // set the archer's position
        uint32_t getColor();        // get the archer's color

        bool hit();                 // hit the archer

    private:
        void attack();                  // start the archer's attack
        void updateAttack();            // update the archer's attack
        void setColor(uint32_t color);  // set the archer's color

        bool _active;               // archer active state
        bool _attacking;            // archer attacking state
        bool _shooting;             // archer shooting state
        int _pos;                   // archer position
        uint32_t _color;            // archer color

        int _minAttackTime;         // archer minimum attack delay 
        int _maxAttackTime;         // archer maximum attack delay

        unsigned long _attackStart; // archer attack start time
        unsigned long _lastAction;  // archer last action time
        unsigned long _nextAction;  // archer next action time

};

// constructor, unused
archer::archer()
{
}

// start the archer at a position with default attack times
void archer::start(int pos)
{
    _active = true;
    _pos = pos;
    _color = COLOR_ARCHER;
    _minAttackTime = ARCHER_ACTION_MIN_TIME;
    _maxAttackTime = ARCHER_ACTION_MAX_TIME;
    _nextAction = random(_minAttackTime, _maxAttackTime);
}

// remove the archer
void archer::remove(){
    _active = false;
    _pos = -10;
}

// start the archer at a position with custom attack times
void archer::start(int pos, int minAttackTime, int maxAttackTime)
{
    _active = true;
    _pos = pos;
    _color = COLOR_ARCHER;
    _minAttackTime = minAttackTime;
    _maxAttackTime = maxAttackTime;
    _nextAction = random(_minAttackTime, _maxAttackTime);
}

// update the archer
void archer::update()
{
    if (_active)
    {
        updateAttack(); // update the archer's attack
        // check if the archer should attack again
        if (millis() - _lastAction > _nextAction)
        {
            _lastAction = millis();
            _nextAction = random(_minAttackTime, _maxAttackTime);

            attack();
        }
    }
} 

// check if the archer is colliding with a position
bool archer::isColliding(int pos)
{
    return _active && _pos == pos;
}

// check if the archer is active
bool archer::isActive()
{
    return _active;
}

// get the archer's position
int archer::getPos()
{
    return _pos;
}

// set the archer's position
void archer::setPos(int pos)
{
    _pos = pos;
}

// get the archer's color
uint32_t archer::getColor()
{
    return _color;
}

// hit the archer
bool archer::hit()
{
    _active = false;    // deactivate the archer
    _pos = -10;         // move the archer off screen
    return true;
}

// start the archer's attack
void archer::attack()
{
    _color = COLOR_ARCHER_CHARGE;   // set the archer's color to charging
    _attacking = true;              // set the archer's attacking state to true
    _attackStart = millis();        // set the archer's attack start time
    DebugArcher.println("Archer is charging", DEBUG_DEBUG);
}

// update the archer's attack
void archer::updateAttack()
{
    if(!_attacking) // if the archer is not attacking, return
    {
        return;
    }
    // check if the archer is done charging
    if(millis() - _attackStart > ARCHER_CHARGE_TIME)    
    {
        DebugArcher.println("Archer is shooting", DEBUG_DEBUG);
        _attacking = false;
        _shooting = true;       // set the archer's shooting state to true
    }
}

// check if the archer is ready to shoot
bool archer::isShooting()
{
    if(_shooting)   // if the archer is shooting, return true
    {
        DebugArcher.println("shooting", DEBUG_DEBUG);
        _shooting = false;      // set the archer's shooting state to false
        _color = COLOR_ARCHER;  // set the archer's color to normal
        return true;
    }
    return false;
}

// set the archer's color
void archer::setColor(uint32_t color)
{
    _color = color;
}
