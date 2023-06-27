/*
* deathAnimation.h
*
* Header file for deathAnimation.cpp
* Creates a deathAnimation object that can be used to display a death animation on a neopixel strip
*/

#pragma once

#include <Arduino.h>
#include "debug.h"  // include debug class

debug DebugDeathAnimation(DEBUG_LEVEL, "deathAnimation.h"); // create debug object for deathAnimation class

// deathAnimation class
class deathAnimation
{
    public:
        deathAnimation();                               // constructor, unused
        void start(int pos, uint32_t color);            // start death animation
        void update();                                  // update death animation
        bool isActive();                                // check if death animation is active           
        int getPos();                                   // get death animation position
        uint32_t pixelColors[DEATH_ANIMATION_SIZE];     // death animation pixel colors

    private:
        void setColor(uint32_t color);                  // set death animation color
        uint32_t randomColourIntensety(uint32_t color); // randomize color intensety

        unsigned long _endMillis[DEATH_ANIMATION_SIZE]; // death animation end times

        bool _active;                                   // death animation active state   
        int _pos;                                       // death animation position
        unsigned long _startMillis;                     // death animation start time

};

// constructor, unused
deathAnimation::deathAnimation()
{
}

// start death animation
void deathAnimation::start(int pos, uint32_t color)
{
    DebugDeathAnimation.println("Death animation started at " + String(pos) + " with color " + String(color), DEBUG_DEBUG);
    _pos = pos - ((DEATH_ANIMATION_SIZE) / 2);  // center death animation around position
    for(int i = 0; i < DEATH_ANIMATION_SIZE; i++){  // randomize color intensety for each pixel
        pixelColors[i] = randomColourIntensety(color);
        _endMillis[i] = random(DEATH_ANIMATION_MIN_TIME, DEATH_ANIMATION_MAX_TIME); // randomize end time for each pixel
    }
    _active = true;
    DebugDeathAnimation.println(String(pixelColors[0]), DEBUG_DEBUG);
    _startMillis = millis();
}

// update death animation
void deathAnimation::update()
{
    if(_active){
        for(int i = 0; i < DEATH_ANIMATION_SIZE; i++){      // loop through each pixel
            if(millis() - _startMillis > _endMillis[i]){    // if pixel end time is reached
                pixelColors[i] = 0;                         // turn pixel off
            }
        }
        bool allOff = false;
        for(int i = 0; i < DEATH_ANIMATION_SIZE; i++){      // check if all pixels are off
            if(pixelColors[i] != 0){    
                allOff = false;
                break;
            }
            allOff = true;
        }
        if(allOff){
            DebugDeathAnimation.println("Death animation ended", DEBUG_DEBUG);
            _active = false;
        }
    }
}

// check if death animation is active
bool deathAnimation::isActive()
{
    return _active;
}

// get death animation position
int deathAnimation::getPos()
{
    return _pos;
}

// randomize color intensety
uint32_t deathAnimation::randomColourIntensety(uint32_t color)
{
    // seperate color into red, green and blue, and randomize intensety for each color between 50% and 150%
    uint32_t red = ((color >> 16) & 0xFF) * (float(random(50, 150)) / 100.0);
    uint32_t green = ((color >> 8) & 0xFF) * (float(random(50, 150)) / 100.0);
    uint32_t blue = (color & 0xFF) * (float(random(50, 150)) / 100.0);

    // cap color at 255
    if(red > 255){
        red = 255;
    }
    if(green > 255){
        green = 255;
    }
    if(blue > 255){
        blue = 255;
    }
    DebugDeathAnimation.println("Random color: " + String(red) + ", " + String(green) + ", " + String(blue), DEBUG_DEBUG);

    // combine colors into one uint32_t
    uint32_t output = (red << 16) | (green << 8) | blue;
    DebugDeathAnimation.println("Random color output: " + String(output), DEBUG_DEBUG);
    return output;
}