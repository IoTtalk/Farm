#include <Bridge.h>

char pin13[2];
char valueStr[23];
int valueInt;

void setup() {
    pinMode(13,OUTPUT);
    
    pinMode(A0,INPUT);
    pinMode(A1,INPUT);
    pinMode(A2,INPUT);
    pinMode(A3,INPUT);
    pinMode(A4,INPUT);
    pinMode(A5,INPUT);
    pinMode(A6,INPUT);
    pinMode(A7,INPUT);

    Bridge.begin();   // Pins 0 and 1 should be avoided as they are used by the Bridge library.
}

void loop() {
    Bridge.get("Reg_done",  pin13, 2);
    digitalWrite(13, atoi(pin13));    

    char Moisture[]="Moisture1";
    for (int i=0; i<=3; i++){
        valueInt = analogRead(i);
        itoa(valueInt, valueStr, 10); 
        Moisture[8] = i + 49;
        Bridge.put(Moisture, valueStr);
        delay(500);
    }

    char pH[]="pH1";
    for (int i=4; i<=7; i++){
        valueInt = analogRead(i);
        itoa(valueInt, valueStr, 10); 
        pH[2] = i + 45;
        Bridge.put(pH, valueStr);
        delay(500);
    }    

   

    
}
