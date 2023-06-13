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

#define SUBCARIER_PERIOD 420 // The default frequency is 16000000 / (420 + 1) = 38.005 kHz
#define IR_BRIGHNESS 400 // 0-400, 0 is off, 400 is max brightness

#define DELAY_TIME 5000 // Time between sending commands in ms

#define DEVICE_ID 53 // ID of the device, used to identify the robot

bool IR_DIRECTION = false; // LOW -> LEFT, HIGH -> RIGHT

void setup() {
  Serial.begin(9600);   // start serial for output
  delay(3000);          // wait for serial window conection, used for debuging
  DEBUG(F("START " __FILE__ " from " __DATE__ "\r\n")); // Print debug info
}

void loop() {
  sendCommand(DEVICE_ID, random(0,250), IR_DIRECTION); // Send random command
  IR_DIRECTION = !IR_DIRECTION; // change witch side of the robot the IR LED is sending from
  delay(DELAY_TIME);
}

// Command for sending 8 bit command in acordance with the NEC protocol
void sendCommand(uint8_t deviceID, uint8_t command, bool direction) {
    // Send a start pulse. high for 9ms, low for 4.5ms
    DEBUG("StartPulse");
    startPulse(direction);
    delayMicroseconds(9000);
    stopPulse();
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
    stopPulse();
    delayMicroseconds(560);
}

// Send 8 bit byte in acordance with the NEC protocol
void sendByte(uint8_t byte, bool direction){
    for (uint8_t i = 0; i < 8; i++) {   // iterate through the bits in the byte, LSB first
        uint8_t bit = (byte >> (i)) & 1;    // get the current bit
        if(bit == 1){   // if the bit is 1, send 1
            DEBUG_S("1");
            startPulse(direction);
            delayMicroseconds(560);
            stopPulse();
            delayMicroseconds(1690);
        } else {        // if the bit is 0, send 0
            DEBUG_S("0");
            startPulse(direction);
            delayMicroseconds(560);
            stopPulse();
            delayMicroseconds(560);
        }
    }
    DEBUG("");
}

// Start the IR pulse with settings for subcarrier
void startPulse(bool direction) {
    startCarrier(direction, IR_BRIGHNESS, SUBCARIER_PERIOD); // Turn on IR LED int the specified direction with subcarrier
}

// Stolen form https://github.com/pololu/zumo-32u4-arduino-library/blob/master/src/Zumo32U4IRPulses.cpp
void startCarrier(bool direction, uint16_t brightness, uint16_t period)
{
    // Disable Timer 3's interrupts.  This should be done first because another
    // library might be using the timer and its ISR might be modifying timer
    // registers.
    TIMSK3 = 0;

    // Make sure brightness is not larger than period because then the compare
    // match would never happen and the pulse count would always be zero.
    if (brightness > period)
    {
        brightness = period;
    }

    // Set the PWM pin to be an input temporarily.  Otherwise, when we configure
    // the COM3A<1:0> bits, the OC03A signal might be high from previous
    // activity of the timer and result in a glitch on the pin.
    PORTC &= ~(1 << 6);
    DDRC &= ~(1 << 6);

    // Put the timer into a known state that should not cause any trouble while
    // we are reconfiguring it.
    // COM3A<1:0> = 10 : Clear OC3A on match, set at top.
    TCCR3A = (1 << COM3A1);
    TCCR3B = 0;

    // Simulate a compare match, which makes the OC3A PWM signal (which is not
    // connected to the I/O pin yet) go low.  We must do this after configuring
    // COM3A<1:0>.
    TCCR3C = (1 << FOC3A);

    // Make the PWM pin be an output.  The OC03A signal will control its value.
    DDRC |= (1 << 6);

    // Drive PF6/A1 high or low to select which LEDs to use.
    if (direction)
    {
        // Right
        PORTF |= (1 << 6);
    }
    else
    {
        // Left
        PORTF &= ~(1 << 6);
    }
    DDRF |= (1 << 6);

    // Set frequency/period.
    ICR3 = period;

    // Set the count to be one less than ICR3 so that the new duty cycle
    // will take effect very soon.
    TCNT3 = period - 1;

    // Set the duty cycle.
    OCR3A = brightness;

    // Start the timer.  It will start running once the clock source bits
    // in TCCR3B are set.
    //
    // COM3A<1:0> = 10 : Set OC3A on match, clear at top.
    // WGM3<3:0> = 1110 : Fast PWM, with ICR3 as the TOP.
    // CS3<3:0> = 001 : Internal clock with no prescaler
    TCCR3A = (1 << COM3A1) | (1 << WGM31);
    TCCR3B = (1 << WGM33) | (1 << WGM32) | (1 << CS30);
}

void stopPulse()
{
    // Prepare the PWM pin to drive low.  We don't want to just set it as an
    // input because then it might decay from high to low gradually and the
    // LEDs would not turn off immediately.
    PORTC &= ~(1 << 6);
    DDRC |= (1 << 6);

    // Disconnect the PWM signal from the pin, causing it to drive low.  We must
    // do this before stopping the timer to avoid glitches.
    TCCR3A = (1 << WGM31);

    // Turn off the timer.
    TCCR3B = 0;

    // Restore the timer's default settings to help avoid compatibilty issues
    // with other libraries.
    TIMSK3 = 0;
    TCCR3A = 0;
    OCR3A = 0;
    ICR3 = 0;
    TCNT3 = 0;

    // Change the IR LED direction pin (A1) back to an input so it
    // can be used for measuring the battery level.
    DDRF &= ~(1 << 6);
    PORTF &= ~(1 << 6);
}
