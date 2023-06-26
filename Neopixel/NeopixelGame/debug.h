/*
* debug.h
*
* Header file for debug class
* Creates a debug object that can be used to print debug messages to serial
*/

#ifndef debug_h
#define debug_h

#include <Arduino.h>

// Define debug levels
#define DEBUG_OFF 0
#define DEBUG_INFO 1
#define DEBUG_DEBUG 2

// debug classes
class debug
{
private:
    int _debugLevel;    // debug level
    String _fileID;     // file ID
public:
    debug(int level, String fileID);            // constructor
    void begin(int baud);                       // begin serial
    void print(String message, int level);      // print message
    void println(String message, int level);    // print message with new line
};

// constructor
debug::debug(int level, String fileID)
{
    // set debug level and file ID
    _debugLevel = level;
    _fileID = fileID;
}

// begin serial
void debug::begin(int baud)
{
    Serial.begin(baud);
}

// print message
void debug::print(String message, int level)
{
    if(level <= _debugLevel){   // if current debug level is lower or equal to message debug level
        Serial.print(_fileID);  // print file ID
        Serial.print(": ");
        Serial.print(message);  // print message
    }
}

// print message with new line
void debug::println(String message, int level)
{
    if(level <= _debugLevel){       // if current debug level is lower or equal to message debug level
        Serial.print(_fileID);      // print file ID
        Serial.print(": ");
        Serial.println(message);    // print message
    }
}

#endif