/*
* arrow.h
*
* Header file for arrow class
* creates an arrow object for the neopixel game
*/
#pragma once

#include <Arduino.h>
#include "debug.h"  // include debug class

debug DebugArrow(DEBUG_LEVEL, "arrow.h");    // create debug object for arrow class

// arrow class
class arrow
{
    public:
        arrow();                        // constructor, unused
        void start(int pos);            // start the arrow at a position with default speed
        void start(int pos, int speed); // start the arrow at a position with custom speed
        void update();                  // update the arrow

        bool isColliding(int pos);      // check if the arrow is colliding with a position
        bool isActive();                // check if the arrow is active

        int getPos();                   // get the arrow's position
        void setPos(int pos);           // set the arrow's position
        uint32_t getColor();            // get the arrow's color

    private:
        void setColor(uint32_t color);  // set the arrow's color

        bool _active;                   // arrow active state
        int _pos;                       // arrow position
        int _speed;                     // arrow speed
        uint32_t _color;                // arrow color

        unsigned long _lastAction;      // arrow last action time
};

// constructor, unused
arrow::arrow()
{
}

// start the arrow at a position with default speed
void arrow::start(int pos)
{
    _active = true;
    _pos = pos;
    _speed = ARROW_SPEED;
    _color = COLOR_ARROW;
}

// start the arrow at a position with custom speed
void arrow::start(int pos, int speed)
{
    _active = true;
    _pos = pos;
    _speed = speed;
    _color = COLOR_ARROW;
}

// update the arrow
void arrow::update()
{
    if (_active)
    {
        if (millis() - _lastAction > (1000 / ARROW_SPEED))
        {
            _lastAction = millis();
            _pos--;
            DebugArrow.println("Arrow pos: " + String(_pos), DEBUG_DEBUG);
            if (_pos < 0)
            {
                _active = false;
                _pos = -10;
            }
        }
    }
}

// check if the arrow is colliding with a position
bool arrow::isColliding(int pos)
{
    if (_active)
    {
        // if the arrow is colliding with the position, deactivate the arrow and return true
        if (_pos == pos)    
        {
            _active = false;
            _pos = -10;
            return true;
        }
    }
    return false;
}

// check if the arrow is active
bool arrow::isActive()
{
    return _active;
}

// get the arrow's position
int arrow::getPos()
{
    return _pos;
}

// set the arrow's position
void arrow::setPos(int pos)
{
    _pos = pos;
}

// get the arrow's color
uint32_t arrow::getColor()
{
    return _color;
}

// set the arrow's color
void arrow::setColor(uint32_t color)
{
    _color = color;
}
