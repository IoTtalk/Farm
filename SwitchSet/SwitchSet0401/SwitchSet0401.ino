#include <Bridge.h>

char pin13[2];
char StrBuf[50];
char valueStr[23];
int valueInt;

char incomming[11]={'\0'};
int previous_incomming_Switch_O[] = {99,99,99,99,99,99,99,99,99,99};

void setup() {
    pinMode(13,OUTPUT);
    pinMode(2,OUTPUT);
    digitalWrite(2, HIGH);    
    pinMode(3,OUTPUT);
    digitalWrite(3, HIGH);
    pinMode(4,OUTPUT);
    digitalWrite(4, HIGH);    
    pinMode(5,OUTPUT);    
    digitalWrite(5, HIGH);
    pinMode(6,OUTPUT);    
    digitalWrite(6, HIGH);
    pinMode(7,OUTPUT);
    digitalWrite(7, HIGH);
    pinMode(8,OUTPUT);    
    digitalWrite(8, HIGH);
    pinMode(9,OUTPUT);        
    digitalWrite(9, HIGH);
    pinMode(10,OUTPUT);        
    digitalWrite(10, HIGH);
    pinMode(11,OUTPUT);        
    digitalWrite(11, HIGH);

    Bridge.begin();   // Pins 0 and 1 should be avoided as they are used by the Bridge library.
}

String StringTemp = "";
void loop() {
    Bridge.get("Reg_done",  pin13, 2);
    digitalWrite(13, atoi(pin13));    
   
    for(int i=1; i<=10; i++){
        StringTemp = "incomming_Switch-O" + String(i);
        StringTemp.toCharArray(StrBuf, StringTemp.length()+1);
        Bridge.get(StrBuf,  incomming, 10);
        if (atoi(incomming) != previous_incomming_Switch_O[i-1]){
            previous_incomming_Switch_O[i-1] = atoi(incomming);
            StringTemp = "Switch-O" + String(i);
            StringTemp.toCharArray(StrBuf, StringTemp.length()+1);
            Bridge.get(StrBuf,  valueStr, 5);
            valueStr[4]='\0';
            valueInt = atoi(valueStr);
            if (valueInt > 0) valueInt=0; else valueInt=1;  // on/off reverse
            digitalWrite(i+1, valueInt);
        }
        delay(0.5);
    }
    
}
