/*
* Porgram for Zumo robot to receive a command from a remote and answer it
*/

#define PRINT_DEBUG  // Uncomment to disable printing of debug messages

#ifdef PRINT_DEBUG
#define DEBUG(msg) (Serial.println(msg))
#define DEBUG_S(msg) (Serial.print(msg))
#else
#define DEBUG(msg)
#define DEBUG_S(msg)
#endif

#define DECODE_NEC

#include <IRremote.hpp>
#include <Zumo32U4IRPulses.h>

#define SUBCARIER_PERIOD 420  // The default frequency is 16000000 / (420 + 1) = 38.005 kHz
#define IR_BRIGHNESS 100        // 0-400, 0 is off, 400 is max brightness

#define IR_RECEIVE_PIN 4    // receive on right proximity sensor

#define DEVICE_ID 2  // ID of the device, used to identify the robot

Zumo32U4IRPulses::Direction IR_DIRECTION = Zumo32U4IRPulses::Right; // Direction of the IR LED

const int authorisedDevices[] = { 200, 201 }; // List of authorised devices

#define COMMAND_IDENTIFY 0xF0       // Command for telling the robot to identify itself
#define COMMAND_TOL_STATION 0xF1    // Command for telling the robot it's at a tol station

#define COMMAND_OPEN 0x01   // Command for telling the tol station to open the gate
#define COMMAND_IDENTIFYING 0x02

#define ANSWER_DELAY 30 // Delay before answering a command, needed for the IR receiver to be ready

unsigned long recivedTime = 0;
bool newData = false;

void setup() {
  Serial.begin(9600);
  delay(3000);  // wait for serial window conection, used for debuging

  DEBUG(F("START " __FILE__ " from " __DATE__ "\r\nUsing library version " VERSION_IRREMOTE));

  // Start the receiver
  IrReceiver.begin(IR_RECEIVE_PIN);

  Serial.print(F("Ready to receive IR signals of protocols: "));
  printActiveIRProtocols(&Serial);
  Serial.print(F("at pin "));
  Serial.println(IR_RECEIVE_PIN);
}

void loop() {

  if (IrReceiver.decode()) {
    Serial.println("loop");

    //IrReceiver.printIRResultShort(&Serial);

    if (IrReceiver.decodedIRData.protocol == UNKNOWN) {
      Serial.println(F("Received noise or an unknown (or not yet enabled) protocol"));
      // We have an unknown protocol here, print more info
      IrReceiver.printIRResultRawFormatted(&Serial, true);
    }
    Serial.println();
    Serial.print("Recived command ");
    Serial.print(IrReceiver.decodedIRData.command);
    Serial.print(" from ");
    Serial.println(IrReceiver.decodedIRData.address);

    recivedTime = millis();
    newData = true;

    IrReceiver.start();  // Enable receiving of the next value
    
  }
  if (millis() - recivedTime > ANSWER_DELAY && newData == true) {
    handleCommand(IrReceiver.decodedIRData.address, IrReceiver.decodedIRData.command);
    newData = false;
    IrReceiver.start();  // Enable receiving of the next value
  }
}

// functon for handling received commands
void handleCommand(uint16_t senderID, uint16_t command) {
  if (isAuthorised(senderID)) {
    if (command == COMMAND_IDENTIFY) {
      DEBUG("Identify");
      sendCommand(DEVICE_ID, COMMAND_IDENTIFYING, IR_DIRECTION);   // Send device ID and no command
    } else if (command == COMMAND_TOL_STATION) {
      DEBUG("Tol station");
      sendCommand(DEVICE_ID, COMMAND_OPEN, IR_DIRECTION); //Send open command
    }
  }
}

// Function for checking if the sender is an authorised device
bool isAuthorised(uint16_t senderID) {
  for (int i = 0; i < (sizeof(authorisedDevices) / sizeof(authorisedDevices[0])); i++) {
    if (senderID == authorisedDevices[i]) {
      DEBUG("Is authorised");
      return true;
      ;
    }
  }
  DEBUG("Not authorised");
  return false;
}

// Function for sending 8 bit command in acordance with the NEC protocol
void sendCommand(uint8_t deviceID, uint8_t command, Zumo32U4IRPulses::Direction direction) {
  DEBUG_S("Sending : ");
  DEBUG_S(command);
  DEBUG_S(" From : ");
  DEBUG(deviceID);
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
void sendByte(uint8_t byte, Zumo32U4IRPulses::Direction direction) {
  for (uint8_t i = 0; i < 8; i++) {   // iterate through the bits in the byte, LSB first
    uint8_t bit = (byte >> (i)) & 1;  // get the current bit
    if (bit == 1) {                   // if the bit is 1, send 1
      DEBUG_S("1");
      startPulse(direction);
      delayMicroseconds(560);
      Zumo32U4IRPulses::stop();
      delayMicroseconds(1690);
    } else {  // if the bit is 0, send 0
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
  Zumo32U4IRPulses::start(direction, IR_BRIGHNESS, SUBCARIER_PERIOD);  // Turn on IR LED int the specified direction with subcarrier
}
