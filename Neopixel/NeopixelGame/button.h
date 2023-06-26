/*
* button.h
*
* Header file for button class
* Creates a button object that can be used to check if a pull up button has been pressed or released
* includes debouncing
*/

#ifndef button_h
#define button_h

#include <Arduino.h>

// button class
class button
{
    public:
        button();                                       // constructor, unused
        void setupButton(int pin, int debounceTime);    // setup button, pin and debounce time
        void begin();                                   // begin button, set pin mode
        bool isDebounced();                             // check if button signal is debounced
        bool hasBeenPressed();                          // check if button has been pressed
        bool hasBeenReleased();                         // check if button has been released
        
        bool state;                                     // debounced state of button
    private:

        int _pin;                           // pin of button
        int _debounceTime;                  // time to debounce button
        bool _lastRead;                     // last read state of button
        bool _lastState;                    // last state of button
        unsigned long _lastDebounceTime;    // last time button was bounced
};

// constructor, unused
button::button()
{
}

// setup button, pin and debounce time
void button::setupButton(int pin, int debounceTime)
{
    _pin = pin;
    _debounceTime = debounceTime;
}

// begin button, set pin mode
void button::begin()
{
    pinMode(_pin, INPUT_PULLUP);
}

// check if button signal is debounced
bool button::isDebounced()
{
    bool reading = digitalRead(_pin);   // read button
    if(reading != _lastRead){           // if button state has changed
        _lastDebounceTime = millis();   // set last bounce time
    }
    _lastRead = reading;
    if((millis() - _lastDebounceTime) > _debounceTime){ // if button signal has been stable for longer than debounce time
            state = reading;
            return true;
    } else {
        return false;
    }
}

// check if button has been pressed
bool button::hasBeenPressed()
{
    if(isDebounced()){  // if button signal is debounced
        if(state == LOW && _lastState == HIGH){ // if button is pressed and last state was not pressed
            _lastState = state; // set last state to current state
            return true;    // return true
        }
    } else {
        _lastState = state; // set last state to current state
    }
    return false;   // return false
}

// check if button has been released
bool button::hasBeenReleased()
{
    if(isDebounced()){  // if button signal is debounced
        if(state == HIGH && _lastState == LOW){ // if button is released and last state was not released
            _lastState = state; // set last state to current state
            return true;    // return true
        }
    } else {
        _lastState = state; // set last state to current state
    }
    return false;   // return false
}



#endif