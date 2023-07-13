// Define the pins for the 7 segment display
#define SEG_A 6
#define SEG_B 7
#define SEG_C 8
#define SEG_D 9
#define SEG_E 10
#define SEG_F 11
#define SEG_G 12
#define SEG_DP 13

// Define the pins for the 4 displays
#define SEG_1 2
#define SEG_2 3
#define SEG_3 4
#define SEG_4 5

// Define the numbers
const bool numbers[10][8] = {
    {1, 1, 1, 1, 1, 1, 0, 0}, // 0
    {0, 1, 1, 0, 0, 0, 0, 0}, // 1
    {1, 1, 0, 1, 1, 0, 1, 0}, // 2
    {1, 1, 1, 1, 0, 0, 1, 0}, // 3
    {0, 1, 1, 0, 0, 1, 1, 0}, // 4
    {1, 0, 1, 1, 0, 1, 1, 0}, // 5
    {1, 0, 1, 1, 1, 1, 1, 0}, // 6
    {1, 1, 1, 0, 0, 0, 0, 0}, // 7
    {1, 1, 1, 1, 1, 1, 1, 0}, // 8
    {1, 1, 1, 1, 0, 1, 1, 0}  // 9
};

// Variable to store the output
bool output[4][8] = {
    {0, 0, 0, 0, 0, 0, 0, 0}, // 1
    {0, 0, 0, 0, 0, 0, 0, 0}, // 2
    {0, 0, 0, 0, 0, 0, 0, 0}, // 3
    {0, 0, 0, 0, 0, 0, 0, 0}  // 4
};

// last time the display was updated
unsigned long lastTime = 0;

// counting variable
int count = 0;

// Setup the pins
void setup(){
    for(int i = 2; i <= 13; i++){
        pinMode(i, OUTPUT);
    }
    for(int i = 0; i < 4; i++){
        setNumber(0, i);
    }
}

void loop(){
    update();
    if(millis() - lastTime > 1000){
        lastTime = millis();
        count++;
        setNumber(count % 10, 3);           // set the last digit
        setNumber((count / 10) % 10, 2);    // set the second digit
        setNumber((count / 100) % 10, 1);   // set the third digit
        setNumber((count / 1000) % 10, 0);  // set the fourth digit
    }

}

// Store the number in the output array
void setNumber(int number, int segment){
    for(int i = 0; i < 8; i++){
        output[segment][i] = !numbers[number][i];
    }
}

// Show the number on the display
void showNumber(int segment){
    for(int i = 0; i < 8; i++){
        digitalWrite(i + 6, output[segment][i]);
    }
}

// Set the segment
void setSegment(int segment){
    digitalWrite(SEG_1, LOW);
    digitalWrite(SEG_2, LOW);
    digitalWrite(SEG_3, LOW);
    digitalWrite(SEG_4, LOW);
    digitalWrite(segment, HIGH);
}

// Update the display
void update(){
    setSegment(SEG_1);
    showNumber(0);
    delay(5);
    setSegment(SEG_2);
    showNumber(1);
    delay(5);
    setSegment(SEG_3);
    showNumber(2);
    delay(5);
    setSegment(SEG_4);
    showNumber(3);
    delay(5);
}