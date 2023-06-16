# Steppermotor 
## Bruk av steppermotor
 `Stepper.h` biblioteket følger med instalasjonen av Arduino IDE og indeholder to enkle funksjoner for bruk av stepermotoren. `setSpeed(long RPM)`hvilket setter hastigheten motoren skal rotere, og `step(int number_of_steps)`. I tilleg til disse funksjonenen inneholder den en constructor som kalles når klassen blir initialiser. `Stepper(int number_of_steps, int motor_pin_1, int motor_pin_2, int motor_pin_3, int motor_pin_4)` Denne brukes til å sette parameterene til motoren.

 ```cpp 
 #include <Stepper.h>

 #define STEPS_PER_REVOLUTION 200

 #define L293D_IN1_PIN 8
 #define L293D_IN2_PIN 9
 #define L293D_IN3_PIN 10
 #define L293D_IN4_PIN 11

 Stepper myStepper(STEPS_PER_REVOLUTION, L293D_IN1_PIN, L293D_IN2_PIN, L293D_IN3_PIN, L293D_IN4_PIN);

 void setup(){
    myStepper.setSpeed(100);
 }

 void loop(){
    // Rotate one revolution
    myStepper.step(STEPS_PER_REVOLUTION);
    delay(1000);
    // Rotate back
    myStepper.step(-STEPS_PER_REVOLUTION);
    delay(1000);
 }
 ```

![image](Schematic.png)

## oppgaveideer
* Sett opp motoren
* Speedcontroller med potmeter
* Kontroler avstand flyttet (typ heis, 3d printer el.) + gå til spesifik avstand?

## Funn
Maks hastighet ~200 rpm