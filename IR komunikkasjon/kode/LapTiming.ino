/*
* Code for reciving timing zumo cars around a track
* The car is identified by the ID sent by the IR transmitter
* The time is measured by a break beam sensor
* The code supports up to 10 cars
*
* Used IR protocol: NEC
*/

//#define PRINT_DEBUG  // Uncomment to disable printing of debug messages

// Setup debug printing macros
#ifdef PRINT_DEBUG
#define DEBUG(msg) (Serial.println(msg))
#else
#define DEBUG(msg)
#endif


#define DECODE_NEC          // Set IR protocol to NEC, must be before #include <IRremote.hpp>
#include <IRremote.hpp> // include IRremote library

#define IR_RECEIVE_PIN 2    // Pin for IR receiver

#define BREAK_BEAM_LED 13   // Pin for break beam LED
#define BREAK_BEAM_SENSOR A0 // Pin for break beam sensor
#define BREAK_BEAM_THRESHOLD 700 // Threshold for break beam sensor
#define BREAK_DEBOUNCE 500

enum state { // Enum for the state of 
    ON,
    OFF
};
state timerState = ON; // Set initial state to OFF

int IDList[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 53}; // List of car IDs
unsigned long timeList[10] = {0}; // List for ceapting the time of the cars

unsigned long lastBreak = 0; // Variable for storing the time of the last break, used for debouncing
bool oldState = false; // Variable for edge detection of the break beam sensor

void setup() {
    Serial.begin(9600);
    delay(3000);  // wait for serial window conection, used for debuging

    DEBUG(F("START " __FILE__ " from " __DATE__ "\r\nUsing library version " VERSION_IRREMOTE));

    // Start the receiver and if not 3. parameter specified, take LED_BUILTIN pin from the internal boards definition as default feedback LED
    IrReceiver.begin(IR_RECEIVE_PIN);

    pinMode(BREAK_BEAM_LED, OUTPUT); // Set break beam LED pin to output
    pinMode(BREAK_BEAM_SENSOR, INPUT); // Set break beam sensor pin to input

    digitalWrite(BREAK_BEAM_LED, HIGH); // Set break beam LED to high

    Serial.print(F("Ready to receive IR signals of protocols: "));
    printActiveIRProtocols(&Serial);
    Serial.print(F("at pin "));
    Serial.println(IR_RECEIVE_PIN);
}

void loop() {
    /*
    * Check if received data is available and if yes, try to decode it.
    * Decoded result is in the IrReceiver.decodedIRData structure.
    */
    if (IrReceiver.decode()) {

        //IrReceiver.printIRResultShort(&Serial); // Print a short summary of received data

        if (IrReceiver.decodedIRData.protocol == UNKNOWN) {
        Serial.println(F("Received noise or an unknown (or not yet enabled) protocol"));
        // We have an unknown protocol here, print more info
        IrReceiver.printIRResultRawFormatted(&Serial, true);
        }
        Serial.println();
        /*
        * !!!Important!!! Enable receiving of the next value,
        * since receiving has stopped after the end of the current received data packet.
        */
        IrReceiver.resume();  // Enable receiving of the next value

        // Print the recived ID and command
        Serial.print("Recived command:");
        Serial.print(IrReceiver.decodedIRData.command);
        Serial.print(" from car:");
        Serial.println(IrReceiver.decodedIRData.address);
    }
    updateBreakBeam();
}

/*
* Function for checking if the break beam is broken
* If the beam is broken, the time of the identified car is printed
*/
void updateBreakBeam() {
  unsigned long timeNow = millis();
    if (analogRead(BREAK_BEAM_SENSOR) > BREAK_BEAM_THRESHOLD) {
        oldState = false;
        return; // Noting detected, return
    } else if (timerState == OFF){
        return; // Timer is off, return
    } else if (!oldState && (timeNow - lastBreak > BREAK_DEBOUNCE)){ 
        oldState = true;
        lastBreak = timeNow;
        DEBUG("Beam broken");
        printTiming(); // Print timing 
    }
}

/*
* Function for printing the lap time for the last id recived
*/
void printTiming(){
    unsigned long timeNow = millis(); // Get current time
    Serial.print("Time for car: "); // Print ID
    Serial.print(IrReceiver.decodedIRData.address);
    Serial.print(" is: "); // Print time
    Serial.println(timeNow - timeList[getIDIndex(IrReceiver.decodedIRData.address)]);
    timeList[getIDIndex(IrReceiver.decodedIRData.address)] = timeNow; // Update time
}

/*
* Function for getting the index of a ID in the IDList
* Returns index of ID if found, else returns 255
*/
uint8_t getIDIndex(uint8_t ID){
    for(int i = 0; i < 10; i++){
        if(ID == IDList[i]){
            DEBUG("ID found");
            return i;
        }
    }
    DEBUG("ID not found");
    return 255; // ID not found return 255
}