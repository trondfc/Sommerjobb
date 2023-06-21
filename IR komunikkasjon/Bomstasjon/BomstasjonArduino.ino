/*
* Program to send IR out IR command that it is a toll station
* and open the gate when it recives a command to open from the car
*/
#define DECODE_NEC  // We want to decode NEC protocol

// Pins for IR receiver and transmitter
#define IR_RECEIVE_PIN      2
#define IR_SEND_PIN         3

#include <IRremote.hpp>

#include <Servo.h>

Servo myservo;  // create servo object to control a servo

bool open = false;          // variable to store the gate state
unsigned long timeOpned;    // variable to store the time the gate was opened
#define OPEN_TIME 5000      // Time the gate is open

#define DEVICE_ID 200 // ID of the device, used to identify the sender

#define COMMAND_IDENTIFY 0xF0    // Command for telling the robot to identify itself
#define COMMAND_TOL_STATION 0xF1 // Command for telling the robot it's at a tol station

#define COMMAND_OPEN 0x01   // Command for telling the tol station to open the gate

#define SEND_INTERVAL 500 // Time between transmissions

bool awaitingAnswer = false;
unsigned long lastTransmission = 0;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
  myservo.write(0); // close the gate

  Serial.begin(9600);
  delay(3000);

  // Just to know which program is running on my Arduino
  Serial.println(F("START " __FILE__ " from " __DATE__ "\r\nUsing library version " VERSION_IRREMOTE));
  Serial.print(F("Send IR signals at pin "));
  Serial.println(IR_SEND_PIN);
  Serial.print(F("Ready to receive IR signals of protocols: "));
  printActiveIRProtocols(&Serial);
  Serial.print(F("at pin "));
  Serial.println(IR_RECEIVE_PIN);

  // Start the IR receiver and sender
  IrReceiver.begin(IR_RECEIVE_PIN);
  IrSender.begin(DISABLE_LED_FEEDBACK);  
}

void loop() {
  unsigned long timeNow = millis();
  if (timeNow - lastTransmission > SEND_INTERVAL) {     // Send IR signal every SEND_INTERVAL
    IrSender.sendNEC(DEVICE_ID, COMMAND_TOL_STATION, 0);
    lastTransmission = timeNow;
    awaitingAnswer = true;
  }
    if (IrReceiver.decode()) {  // If a new code is received from the IR receiver

      //IrReceiver.printIRResultShort(&Serial); // Print a short summary of received data


      if (IrReceiver.decodedIRData.protocol == UNKNOWN) {
        Serial.println(F("Received noise or an unknown (or not yet enabled) protocol"));
        // We have an unknown protocol here, print more info
        IrReceiver.printIRResultRawFormatted(&Serial, true);
      }
      Serial.println();
      IrReceiver.resume();  // Enable receiving of the next value

      if (awaitingAnswer && IrReceiver.decodedIRData.address != DEVICE_ID) { // If we are waiting for an answer and the answer is not from ourself
        Serial.println("Got answer");
        Serial.print("The car ID is:");
        Serial.println(IrReceiver.decodedIRData.address);

        Serial.print("Recived command is:");
        Serial.println(IrReceiver.decodedIRData.command);
        awaitingAnswer = false;
        if (IrReceiver.decodedIRData.command == COMMAND_OPEN) { // If the command is to open the gate
          Serial.print("opening");
          myservo.write(100);
          open = true;
          timeOpned = millis();
        }
      }
    }
    if(millis() - timeOpned > OPEN_TIME && open == true){ // If the gate has been open for OPEN_TIME 
      Serial.println("closing");
      myservo.write(0);
      open = false;

    }
}
