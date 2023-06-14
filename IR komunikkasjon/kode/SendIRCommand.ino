/*
* Code for sending IR commands using the NEC protocol.
* Made for the Zumo 32U4 robot.
*
* The code uses the same hardware as the proximity sensors and can not be used at the same time.
* The proximity sensor code can be used as long as the IR LED is not used to send commands.
*/

#define PRINT_DEBUG // Uncomment to disable printing of debug messages

// setup debug printing macros
#ifdef PRINT_DEBUG
    #define DEBUG(msg) (Serial.println(msg))
    #define DEBUG_S(msg) (Serial.print(msg))
#else
    #define DEBUG(msg)
    #define DEBUG_S(msg)
#endif

#include <Zumo32U4IRPulses.h> // Library for controlling the IR LED

#define SUBCARIER_PERIOD 420 // The default frequency is 16000000 / (420 + 1) = 38.005 kHz
#define IR_BRIGHNESS 400 // 0-400, 0 is off, 400 is max brightness

#define DELAY_TIME 5000 // Time between sending commands in ms

#define DEVICE_ID 53 // ID of the device, used to identify the robot

// Enum for the direction of the IR LED
Zumo32U4IRPulses::Direction IR_DIRECTION = Zumo32U4IRPulses::Left;

void setup() {
  Serial.begin(9600);   // start serial for output
  delay(3000);          // wait for serial window conection, used for debuging
  DEBUG(F("START " __FILE__ " from " __DATE__ "\r\n")); // Print debug info
}

void loop() {
  //sendCommand(DEVICE_ID, random(0,250), IR_DIRECTION); // Send random command
  sendCommand(DEVICE_ID, random(0,3), IR_DIRECTION); // Send random command
  // change witch side of the robot the IR LED is sending from
  IR_DIRECTION = (IR_DIRECTION == Zumo32U4IRPulses::Left) ? Zumo32U4IRPulses::Right : Zumo32U4IRPulses::Left;
  delay(DELAY_TIME);
}

// Command for sending 8 bit command in acordance with the NEC protocol
void sendCommand(uint8_t deviceID, uint8_t command, Zumo32U4IRPulses::Direction direction) {
    // Send a start pulse. high for 9ms, low for 4.5ms
    DEBUG("StartPulse");
    startPulse(direction);
    delayMicroseconds(9000);
    Zumo32U4IRPulses::stop();
    delayMicroseconds(4500);

    // Send device ID.
    sendByte(deviceID, direction);

    // Send inverted device ID.
    sendByte(~deviceID, direction);

    // Send command.
    sendByte(command, direction);

    // Send inverted command.
    sendByte(~command, direction);

    // Send stop pulse. high for 562.5us, low for 562.5us
    DEBUG("StopPulse");
    startPulse(direction);
    delayMicroseconds(560);
    Zumo32U4IRPulses::stop();
    delayMicroseconds(560);
}

// Send 8 bit byte in acordance with the NEC protocol
void sendByte(uint8_t byte, Zumo32U4IRPulses::Direction direction){
    for (uint8_t i = 0; i < 8; i++) {   // iterate through the bits in the byte, LSB first
        uint8_t bit = (byte >> (i)) & 1;    // get the current bit
        if(bit == 1){   // if the bit is 1, send 1
            DEBUG_S("1");
            startPulse(direction);
            delayMicroseconds(560);
            Zumo32U4IRPulses::stop();
            delayMicroseconds(1690);
        } else {        // if the bit is 0, send 0
            DEBUG_S("0");
            startPulse(direction);
            delayMicroseconds(560);
            Zumo32U4IRPulses::stop();
            delayMicroseconds(560);
        }
    }
    DEBUG("");
}

// Start the IR pulse with settings for subcarrier
void startPulse(Zumo32U4IRPulses::Direction direction) {
    Zumo32U4IRPulses::start(direction, IR_BRIGHNESS, SUBCARIER_PERIOD); // Turn on IR LED int the specified direction with subcarrier
}
