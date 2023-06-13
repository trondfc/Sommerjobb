/*
* Code for controlling a tol station based on the commands recived from a zumo car
* 0x01 is the command for opening the gate
* 0x02 is the command for closing the gate
* The ID of the car is also recived and checked against a list of authorised IDs
*/

#define PRINT_DEBUG  // Uncomment to disable printing of debug messages

// Setup debug printing macros
#ifdef PRINT_DEBUG
#define DEBUG(msg) (Serial.println(msg))
#else
#define DEBUG(msg)
#endif

#define DECODE_NEC          // Set protocol to NEC must be before #include <IRremote.hpp>
#define IR_RECEIVE_PIN 2    // Pin for IR receiver
#include <IRremote.hpp> // include IRremote library

#define SERVO_PIN 3 // Pin for servo (PWM pin)
#include <Servo.h> // include Servo library

Servo myservo;  // create servo object to control a servo

int authorisedIDs[] = {0,53}; // List of IDs that are authorised to open the gate

#define OPEN_GATE 0x01 // Command for opening the gate
#define CLOSE_GATE 0x02 // Command for closing the gate

void setup() {
    Serial.begin(9600);
    delay(3000);  // wait for serial window conection, used for debuging

    DEBUG(F("START " __FILE__ " from " __DATE__ "\r\nUsing library version " VERSION_IRREMOTE));

    // Start the receiver and if not 3. parameter specified, take LED_BUILTIN pin from the internal boards definition as default feedback LED
    IrReceiver.begin(IR_RECEIVE_PIN);

    myservo.attach(SERVO_PIN);  // attaches the servo on pin 3 to the servo object

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

        // Check if the ID is in the list of authorised IDs
        if(isAuthorised(IrReceiver.decodedIRData.address)){
            // Check what command was recived
            switch(IrReceiver.decodedIRData.command){
                case OPEN_GATE:
                    openGate();
                    break;
                case CLOSE_GATE:
                    closeGate();
                    break;
                default:
                    Serial.println("Unknown command");
                    break;
            }
        }
        else{
            Serial.println("Unauthorised ID");
        }
    }
}

// Checks if the ID is in the list of authorised IDs
bool isAuthorised(int ID){
    for(int i = 0; i < (sizeof(authorisedIDs)/sizeof(authorisedIDs[0])); i++){
        if(ID == authorisedIDs[i]){
            return true;
        }
    }
    return false;
}

void openGate(){
    Serial.println("Opening gate");
    myservo.write(90); // Open servo gate
}

void closeGate(){
    Serial.println("Closing gate");
    myservo.write(0); // Close servo gate
}