/*
* Program to messure lap time for cars passing a break beam
* The program uses a IR reciver to recive a ID from the car
* in order to track the lap time for multiple cars
*/
#define DECODE_NEC  // We want to decode NEC protocol

#define IR_RECEIVE_PIN 2  // Pin for IR receiver
#define IR_SEND_PIN 3   // Pin for IR sender

#include <IRremote.hpp>

#define DEVICE_ID 200  // ID of the device, used to identify the sender

#define COMMAND_IDENTIFY 0xF0     // Command for telling the robot to identify itself
#define COMMAND_TOL_STATION 0xF1  // Command for telling the robot it's at a tol station

#define COMMAND_OPEN 0x01  // Command for telling the tol station to open the gate
#define COMMAND_IDENTIFYING 0x02 // Command for answering the identify command

#define BREAK_BEAM_SENSOR A0  // Pin for break beam sensor
#define REFERENCE_PIN A1      // Pin for the refrence potmeter

#define ANSER_TIMEOUT 500  // Time between transmissions

#define BREAKBEEK_TIMEOUT 1000 // Time between each break beam detection, the car neads time to pass the beam

enum state { ON, OFF }; // Enum for timer state
state timerState = ON;  // Set initial state to OFF

int IDList[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 53 };  // List of car IDs
unsigned long timeList[10] = { 0 };                  // List of times for each car

unsigned long lastBreak = 0;
bool oldState = false;

int lastCarID; // ID of the last car to pass the beam

bool awaitingAnswer = false;
unsigned long lastTransmission = 0;

void setup() {
  Serial.begin(9600);
  delay(3000);

  // Just to know which program is running on my Arduino
  Serial.println(F("START " __FILE__ " from " __DATE__ "\r\nUsing library version " VERSION_IRREMOTE));

  // Print startup message
  Serial.print(F("Send IR signals at pin "));
  Serial.println(IR_SEND_PIN);
  Serial.print(F("Ready to receive IR signals of protocols: "));
  printActiveIRProtocols(&Serial);
  Serial.print(F("at pin "));
  Serial.println(IR_RECEIVE_PIN);

  // Start  IR receiver and sender
  IrReceiver.begin(IR_RECEIVE_PIN);
  IrSender.begin(DISABLE_LED_FEEDBACK);
}

void loop() {
  unsigned long timeNow = millis();
  if (timeNow - lastTransmission > ANSER_TIMEOUT) { // If it's time to send a new command
    IrSender.sendNEC(DEVICE_ID, COMMAND_IDENTIFY, 0);
    lastTransmission = timeNow;
    awaitingAnswer = true;
  }
  if (IrReceiver.decode()) {  // If we have a new IR message
    //IrReceiver.printIRResultShort(&Serial); // Print a short summary of received data

    if (IrReceiver.decodedIRData.protocol == UNKNOWN) {
      Serial.println(F("Received noise or an unknown (or not yet enabled) protocol"));
      // We have an unknown protocol here, print more info
      IrReceiver.printIRResultRawFormatted(&Serial, true);
    }
    Serial.println();
    IrReceiver.resume();  // Enable receiving of the next value

    if (awaitingAnswer && IrReceiver.decodedIRData.address != DEVICE_ID) { // If we are waiting for an answer and the answer is not from ourself
      // Print the answer
      Serial.println("Got answer");
      Serial.print("The car ID is:");
      Serial.println(IrReceiver.decodedIRData.address);

      Serial.print("Recived command is:");
      Serial.println(IrReceiver.decodedIRData.command);
      awaitingAnswer = false;
      // If the answer is an identifying command, update the last car ID
      if(IrReceiver.decodedIRData.command == COMMAND_IDENTIFYING){
        lastCarID = IrReceiver.decodedIRData.address;
      }
    }
  }
  updateBreakBeam();    // check if the break beam is broken
}

void updateBreakBeam() {
  unsigned long timeNow = millis();
  if (analogRead(BREAK_BEAM_SENSOR) < analogRead(REFERENCE_PIN)) {
    oldState = false;
    return;  // Noting detected, return
  } else if (timerState == OFF) {
    return;  // Timer is off, return
  } else if (!oldState && (timeNow - lastBreak > BREAKBEEK_TIMEOUT)) { // If the beam is broken and the debounce time has passed
    oldState = true;
    lastBreak = timeNow;
    Serial.println("Beam broken");
    printTiming();  // Print timing
  }
}

/*
* Function for printing the lap time for the last id recived
*/
void printTiming() {
  if (getIDIndex(lastCarID) != 255) {  // test for invalid id
    unsigned long timeNow = millis();  // Get current time
    Serial.print("Time for car: ");    // Print ID
    Serial.print(lastCarID);
    Serial.print(" is: ");                                                   // Print time
    Serial.println(float(timeNow - timeList[getIDIndex(lastCarID)]) / 1000.0, 1);  // pirnt time as seconds with 1 desimal.
    timeList[getIDIndex(lastCarID)] = timeNow;                               // Update time
  }
}

/*
* Function for getting the index of a ID in the IDList
* Returns index of ID if found, else returns 255
*/
uint8_t getIDIndex(uint8_t ID) {
  for (int i = 0; i < 10; i++) {
    if (ID == IDList[i]) {
      //Serial.println("ID found");
      return i;
    }
  }
  //Serial.println("ID not found");
  return 255;  // ID not found return 255
}