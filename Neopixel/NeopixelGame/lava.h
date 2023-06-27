/*
* lava.h
*
* Header file for lava class
* Creates a lava object that can be used to create lava
*/
#pragma once

#include <Arduino.h>
#include "debug.h"  // include debug class

debug DebugLava(DEBUG_LEVEL, "lava.h");  // create debug object for lava class

// lava class
class lava
{
    public:
        lava(); // constructor, unused
        void start(int pos, int size);  // start lava wit default on and off times
        void start(int pos,             // start lava with custom on and off times
                    int size,
                    int minOnTime,
                    int maxOnTime,
                    int minOffTime, 
                    int maxOffTime);
        void update();                  // update lava
        void remove();                  // remove lava

        bool isColliding(int pos);      // check if position is colliding with lava
        bool isActive();                // check if lava is active

        int getPos();                   // get lava position
        int getSize();                  // get lava size
        uint32_t getColor();            // get lava color

    private:
        void setColor(uint32_t color);  // set lava color

        bool _active;                   // lava  object active
        bool _on;                       // lava on
        int _pos;                       // lava position
        int _size;                      // lava size
        uint32_t _color;                // lava color

        int _minOnTime;                 // lava minimum on time
        int _maxOnTime;                 // lava maximum on time
        int _minOffTime;                // lava minimum off time
        int _maxOffTime;                // lava maximum off time

        unsigned long _turnedOn;        // time lava turned on
        int _onTime;                    // lava on time
        unsigned long _turnedOff;       // time lava turned off
        int _offTime;                   // lava off time
};

// constructor, unused
lava::lava()
{
}

// start lava wit default on and off times
void lava::start(int pos, int size)
{
    _pos = pos;
    _size = size;
    _color = COLOR_LAVA_ON;
    _active = true;

    _minOnTime = LAVA_ON_MIN;
    _maxOnTime = LAVA_ON_MAX;
    _minOffTime = LAVA_OFF_MIN;
    _maxOffTime = LAVA_OFF_MAX;
}

// start lava with custom on and off times
void lava::start(int pos, int size, int minOnTime, int maxOnTime, int minOffTime, int maxOffTime){
    _pos = pos;
    _size = size;
    _color = COLOR_LAVA_ON;
    _active = true;

    _minOnTime = minOnTime;
    _maxOnTime = maxOnTime;
    _minOffTime = minOffTime;
    _maxOffTime = maxOffTime;
}

// update lava
void lava::update()
{
    if(_active){    // only update if active
        if(millis() - _turnedOn > _onTime && _on){              // if lava is on and on-time has passed
            DebugLava.println("Lava off", DEBUG_DEBUG);
            _on = false;                                        // turn lava off
            setColor(COLOR_LAVA_OFF);                           // set lava color to off
            _turnedOff = millis();
            _offTime = random(_minOffTime, _maxOffTime);        // set off-time
        } else if(millis() - _turnedOff > _offTime && !_on){    // if lava is off and off-time has passed
            DebugLava.println("Lava on", DEBUG_DEBUG);
            _on = true;                                         // turn lava on
            setColor(COLOR_LAVA_ON);                            // set lava color to on
            _turnedOn = millis();
            _onTime = random(_minOnTime, _maxOnTime);           // set on-time
        }
    }
}

// remove lava
void lava::remove()
{
    _active = false;
    _on = false;
    _pos = -10; // move lava off map
}

// check if position is colliding with lava
bool lava::isColliding(int pos)
{
    if(!_on){            // if lava is off
        return false;
    }else if(pos >= _pos && pos < _pos + _size){ // if lava is on and position is in lava
        return true;
    }
    return false;
}

// check if lava is active
bool lava::isActive()
{
    return _active;     // return active state
}

// get lava position
int lava::getPos()
{
    return _pos;    // return lava start position
}

// get lava size
int lava::getSize()
{
    return _size;   // return lava size
}

// get lava color
uint32_t lava::getColor()
{
    return _color;  // return lava color
}

// set lava color
void lava::setColor(uint32_t color)
{
    _color = color; // set lava color
}

