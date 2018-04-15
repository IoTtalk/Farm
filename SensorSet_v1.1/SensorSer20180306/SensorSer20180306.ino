#include "IRRecvCounter.h"
#include <Wire.h>
#include "Seeed_BME280.h"
#include "SoftwareSerial.h"
#include <T6603.h>
#include <Bridge.h>


//for Bridge
char D13[2];
char valueStr[23];
int valueInt=0;
char incomming[11]={'\0'};
int previous_incomming_LED=999;

//風速計 A0
const int windPin = A0;
float windSensorValue = 0.0;

//土壤濕度FDS100  A1
//const int soilMoisturePin = A1;
//float soilMoisture = 0.0;

//UV  A1, A2, A3
const int UVpin1 = A1;
const int UVpin2 = A2;
const int UVpin3 = A3;
float UVsensorValue = 0.0;

//BME 大氣壓   //D2(SDA), D3(SCL)
BME280 bme280;   
float Temperature=0.0;
uint32_t AtmosphericPressure=0, Humidity=0;

//蟲罐  D4, D5, D6
CIRRecvCounter g_IRRecvD4(4, DEF_TYPE_DIGITAL);
CIRRecvCounter g_IRRecvD5(5, DEF_TYPE_DIGITAL);
CIRRecvCounter g_IRRecvD6(6, DEF_TYPE_DIGITAL);
long Count1 = 0;
long Count2 = 0;
long Count3 = 0;

//雨量計
const int RainPin = 7;    //interupt4   #D3(interrupt0), D2(interrupt1), D0(interrupt2), D1(interrupt3) and D7(interrupt4).
float RainMeter = 0.0;

//LED
const int LEDpin = 8;  //use D8 to control LED

//CO2
T6603 sensor;
const uint8_t rxPin = 9;
const uint8_t txPin = 10;
int CO2;


void blink(){//rain
  RainMeter += 0.2;
}

void setup() {
    Serial.begin(115200);

    pinMode(windPin,INPUT);          //風速計            A0
    //pinMode(soilMoisturePin,INPUT);  //土壤濕度 FDS100   A1 
    pinMode(UVpin1,INPUT);            //UV                A1
    pinMode(UVpin2,INPUT);            //UV                A2
    pinMode(UVpin3,INPUT);            //UV                A3

    Bridge.begin();   // D0 and D1 are used by the Bridge library.
 
    //BME 大氣壓 D2, D3
    if(!bme280.init()) Serial.println("Sensor Error!");

    //蟲罐  D4, D5, D6  <--- 上方蟲罐建構子內已經初始化

    //雨量計 D7
    pinMode(RainPin , INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(RainPin), blink, RISING);

    //LED 開關 D8
    pinMode(LEDpin, OUTPUT);

    //CO2 D9, D10
    sensor.begin(rxPin, txPin);  

    //IoTtalk successful registration notification
    pinMode(13,OUTPUT);
}

long cycleTimestamp = millis();
void loop() {
    Bridge.get("Reg_done",  D13, 2);
    digitalWrite(13, atoi(D13));    
      
    //風速計 A0
    windSensorValue = float(analogRead(windPin));
    if (windSensorValue < 81.84) windSensorValue =0.0;
    else windSensorValue =  (windSensorValue-81.84) * 0.09897;
    dtostrf(windSensorValue, 10, 5, valueStr);
    Bridge.put("WindSpeed", valueStr); 
   
   /* 
    //土壤濕度FDS100  A1
    soilMoisture = float(analogRead(soilMoisturePin)) / 1023 * 100;         // is it correct?????
    Serial.print("Soil moisture: ");    Serial.print(soilMoisture);   Serial.println(" %");
    dtostrf(soilMoisture, 7, 2, valueStr);    
    Bridge.put("SoilMoisture", valueStr); 
    */

    //UV sensor A1
    UVsensorValue = float(analogRead(UVpin1));           // is it correct?????
    Serial.print("UV1: ");    Serial.println(UVsensorValue);   // unit: Voltage
    dtostrf(UVsensorValue, 7, 2, valueStr);    
    Bridge.put("UV1", valueStr); 
    
    //UV sensor A2
    UVsensorValue = float(analogRead(UVpin2));           // is it correct?????
    Serial.print("UV2: ");    Serial.println(UVsensorValue);   // unit: Voltage
    dtostrf(UVsensorValue, 7, 2, valueStr);    
    Bridge.put("UV2", valueStr); 

    //UV sensor A3
    UVsensorValue = float(analogRead(UVpin3));           // is it correct?????
    Serial.print("UV3: ");    Serial.println(UVsensorValue);   // unit: Voltage
    dtostrf(UVsensorValue, 7, 2, valueStr);    
    Bridge.put("UV3", valueStr);     


    //BME 大氣壓   D2, D3
    Temperature = bme280.getTemperature();
    AtmosphericPressure = bme280.getPressure();
    Humidity = bme280.getHumidity();
    Serial.print("Temp: ");        Serial.print(Temperature);    Serial.println(" C");// unit: C
    Serial.print("Pressure: ");    Serial.print(AtmosphericPressure);    Serial.println(" Pa");     // unit: Pa
    Serial.print("Altitude: ");    Serial.print(bme280.calcAltitude(AtmosphericPressure));    Serial.println(" m");  // unit: meter
    Serial.print("Humidity: ");    Serial.print(Humidity);    Serial.println(" %"); // unit:%
    dtostrf(Temperature, 6, 2, valueStr);
    Bridge.put("Temperature", valueStr); 
    dtostrf(AtmosphericPressure, 8, 0, valueStr);
    Bridge.put("AtPressure", valueStr);     
    dtostrf(Humidity, 8, 0, valueStr);
    Bridge.put("Humidity", valueStr);        

    //雨量計 D7 interrupt4   
    Serial.print("RainMeter: ");    Serial.println(RainMeter);   // unit:mm
    dtostrf(RainMeter, 6, 2, valueStr);
    Bridge.put("RainMeter", valueStr);        

    //LED開關 D8
    Bridge.get("incomming_LED",  incomming, 2);
    if (atoi(incomming)!=previous_incomming_LED){
        previous_incomming_LED=atoi(incomming);
        Bridge.get("LED", valueStr, 5);
        valueStr[4]='\0';
        valueInt = atoi(valueStr);
        if (valueInt >= 1) valueInt=1; else valueInt=0;        
        digitalWrite(LEDpin, valueInt);
    }        
    Serial.print("LED state: ");    Serial.println(valueInt);

    
    //CO2  rxPin=D9, txPin=D10
    CO2 = sensor.get_co2();
    Serial.print("CO2: ");    Serial.println(CO2);
    itoa(CO2, valueStr, 10);
    Bridge.put("CO2", valueStr);            

    //捕蟲罐  D4, D5, D6
    while (millis()-cycleTimestamp <= 2000){
        g_IRRecvD4.IRRecvCount();
        g_IRRecvD5.IRRecvCount();
        g_IRRecvD6.IRRecvCount();
    }    
    Count1 = g_IRRecvD4.getCount();
    Count2 = g_IRRecvD5.getCount();
    Count3 = g_IRRecvD6.getCount();
    Serial.print("Bugs counter: ");    Serial.print(Count1); Serial.print(", ");    Serial.print(Count2); Serial.print(", ");    Serial.println(Count3);
    itoa((Count1+Count2+Count3), valueStr, 10);
    Bridge.put("Bugs", valueStr);            
    cycleTimestamp = millis();

    Serial.println("=========================================================================");
}
