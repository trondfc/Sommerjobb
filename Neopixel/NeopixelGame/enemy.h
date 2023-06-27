/*
* enemy.h
*
* Header file for the enemy class
* Creates an enemy object that can be used to display an enemy on a neopixel strip
*/
#pragma once
#include <Arduino.h>
#include "debug.h"  // include debug class

debug DebugEnemy(DEBUG_LEVEL, "enemy.h");    // create debug object for enemy class

// enemy class
class enemy
{
    public:
        enemy();                    // constructor, unused
        void start(int pos);        // start enemy
        void update();              // update enemy

        bool isColliding(int pos);  // check if a position is colliding with the enemy object
        bool isActive();            // check if enemy is active
        bool isHiting(int pos);     // check if enemy is hiting a position
        bool isDefending();         // check if enemy is defending

        int getPos();               // get enemy position
        void setPos(int pos);       // set enemy position
        int getSize();              // get enemy size
        uint32_t getColor();        // get enemy color

        bool hit();                 // hit enemy 

    private:

        void attack();                  // start enemy attack
        void updateAttack();            // update enemy attack
        void defend();                  // start enemy defend
        void updateDefend();            // update enemy defend
        void setColor(uint32_t color);  // set enemy color

        bool _active;                   // enemy active state
        int _pos;                       // enemy position
        int _size;                      // enemy size
        uint32_t _color;                // enemy color

        enum attackState                // attack state enum
        {
            ATTACK_STATE_NONE,          // no attack
            ATTACK_STATE_CHARGING,      // charging attack
            ATTACK_STATE_ATTACKING      // attacking
        };
        attackState _attacking;         // attack state
        unsigned long _attackStart;     // attack start time

        bool _defending;                // defending state
        unsigned long _defendStart;     // defend start time

        unsigned long _lastAction;      // last action time
        unsigned long _nextAction;      // next action time

};

// constructor
enemy::enemy()
{
}

// start enemy
void enemy::start(int pos)
{
    _pos = pos;
    _size = 1;
    _color = COLOR_ENEMY;
    _active = true;
}

// update enemy
void enemy::update()
{
    updateAttack(); // update attack
    updateDefend(); // update defend
    if(millis() - _lastAction > _nextAction){   // check if enemy should do an action
        _lastAction = millis();
        _nextAction = random(ENEMY_ACTION_MIN_TIME, ENEMY_ACTION_MAX_TIME); // set next action time
        int action = random(0, 101);  // get random action
        if(action <= ENEMY_ATTACK_CHANCE){
            attack();
        }else{
            defend();
        }
    }
}

// check if a position is colliding with the enemy object
bool enemy::isColliding(int pos)
{
    if (_active && _pos == pos)
    {
        return true;
    }
    return false;
}

// check if enemy is active
bool enemy::isActive()
{
    return _active;
}

// check if enemy is defending
bool enemy::isDefending()
{
    return _defending;
}

// get enemy position
int enemy::getPos()
{
    return _pos;
}

// get enemy size
int enemy::getSize()
{
    return _size;
}

// set enemy position
void enemy::setPos(int pos)
{
    _pos = pos;
}

// get enemy color
uint32_t enemy::getColor()
{
    return _color;
}

// hit enemy
bool enemy::hit()
{
    _active = false;
    _pos = -10;
    return true;
}

// set enemy color
void enemy::setColor(uint32_t color)
{
    _color = color;
}

// start enemy attack
void enemy::attack()
{
    DebugEnemy.println("Attacking", DEBUG_DEBUG);
    _attacking = ATTACK_STATE_CHARGING; // set attack state
    _size = PLAYER_ATTACK_RANGE;        // set attack size
    _attackStart = millis();            // set attack start time
    setColor(COLOR_ENEMY_CHARGE);       // set attack charge color
}

void enemy::updateAttack()
{
    if(_attacking == ATTACK_STATE_NONE){    // return if not attacking
        return;
    }
    // check if attack charge is done
    if(_attacking == ATTACK_STATE_CHARGING && millis() - _attackStart > ENEMY_CHARGE_TIME){
        DebugEnemy.println("Attack charged", DEBUG_DEBUG);
        _attacking = ATTACK_STATE_ATTACKING;    // set attack state
        _attackStart = millis();
        setColor(COLOR_ENEMY_ATTACK);           // set attack color
    }
    // check if attack is done
    if(_attacking == ATTACK_STATE_ATTACKING && millis() - _attackStart > ENEMY_ATTACK_TIME){
        DebugEnemy.println("Attack done", DEBUG_DEBUG);
        _size = 1;  // set size back to 1
        _attacking = ATTACK_STATE_NONE; // set attack state
        setColor(COLOR_ENEMY);        // set color back to normal
    }
}

// check if enemy is hiting a position
bool enemy::isHiting(int pos)
{
    if(!_active){       // return if not active
        return false;
    } else if (_attacking != ATTACK_STATE_ATTACKING && pos == _pos){
        return true;
    } else if (_attacking == ATTACK_STATE_ATTACKING && pos >= _pos - (_size-1) && pos <= _pos){
        return true;
    } else {
        return false;
    }
    
}

// start enemy defend
void enemy::defend()
{
    DebugEnemy.println("Defending", DEBUG_DEBUG);
    _defending = true;              // set defending state
    _defendStart = millis();
    setColor(COLOR_ENEMY_DEFEND);   // set defend color
}

// update enemy defend
void enemy::updateDefend()
{
    if(!_defending){    // return if not defending
        return;
    }
    if(millis() - _defendStart > ENEMY_DEFEND_TIME){    // check if defend time is done
        DebugEnemy.println("Done defending", DEBUG_DEBUG);
        _defending = false;     // set defending state
        setColor(COLOR_ENEMY);  // set color back to normal
    }
}