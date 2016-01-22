#ifndef Functions_h
#define Functions_h

int valorPWM;

#define STORE_CUSTOM 430

//Autocalibrate Capacitive Sensors ON
#define AUTOCALIBRATE         1

boolean DEBUG_LOG           = 1;
boolean DEBUG_CAPSENSE      = 0;
boolean DEBUG_CAPSENSE_ALL  = 0;
boolean DEBUG_DHT           = 0;
boolean DEBUG_PRESSURE      = 1;
boolean DEBUG_GETLUX        = 0;
boolean DEBUG_DALLAS        = 0;
boolean DEBUG_EMONCMS       = 1;
boolean DEBUG_IR            = 1;
boolean DEBUG_IR_FULL       = 0;
boolean DEBUG_PLC           = 0;

#define DEBUG if(DEBUG_LOG)LOG

//INSIDE BUTTON ON GPIO0 VARIABLE:
boolean button0 = false;

//VALUES FOR CONFIGURATION STORED ON THE EEPROM.
byte L1;
byte L2;
byte L3;

byte S1;
byte S2;
byte S3;
byte S41;
byte S42;
byte S51;
byte S52;
byte S6;
byte L1PIR;
byte L2PIR;
byte L3PIR;

//byte byte0;   //STORE SENSOR CONFIGURATION
//byte byte1;   //STORE LIGHTS CONFIGURATION
//byte byte2;   //STORE OTHERS CONFIGURATION
byte OPTIONS1;   //STORE BOOLEAN OPTIONS
// BOOLEAN OPTIONS
boolean usartbridge = false;
boolean Send_Emon = true;
boolean IR_ENABLE = true;
boolean dht_type = true;  //FALSE = DHT11 , TRUE = DHT22
// ******************
byte OPTIONS2;   //STORE DEBUG OPTIONS
byte OPTIONS3;   //STORE DEBUG OPTIONS
boolean ALARM_MODE = false;
boolean cap_debug = false;
byte cap_thresold;
int ALTITUDE = 20;
byte dallas_qty = 1;


//S1 DEFINE
#define DALLAS_SENSOR 1 
//boolean DALLAS_SENSOR;  

//S2
#define DHT_SENSOR    1
//boolean DHT_SENSOR;

//S3
#define LDR_SENSOR    1
//boolean LDR_SENSOR;




//LIGHT MODES
//boolean ONOFF_MODE;
//boolean PULSE_MODE;
//boolean PWM_MODE;           
//boolean PIR_MODE;
//boolean RGB_MODE;
//boolean ALARM_MODE;
//boolean THERMOSTAT_MODE;

#define ONOFF_MODE    1
#define PWM_MODE      2    
#define RGB_MODE      3
#define PIR_MODE      4
#define PULSE_MODE    5

#define LIGHT_ON_CYCLE 10    		// Light ON for 10 cycles if triggered by a PIR sensor

//Enable this to use PINS 4, 5 for CAPACITIVE BUTTONS, RELAY1 and RELAY2 or on BMP180 pressure sensor (I2C)
//boolean CAPACITIVE;     
//boolean BUTTONS;
//boolean BUTTONS_PULLUP;
//boolean ALARM_ENDSTOP;
//boolean BUTTONS_2_STATE;
//
//boolean RELAY;          
//boolean BMP180;
//boolean PULSE_OUTPUT;
//boolean GARAGE_DOOR;
//boolean WINDOW_CURTAIN;
//boolean OPTO_AND_RELAY; 

#define CAPACITIVE        1     
#define BUTTONS           2
#define BUTTONS_PULLUP    3
#define ALARM_ENDSTOP     4
#define BUTTONS_2_STATE   5

#define RELAY             6         
#define PULSE_OUTPUT      7
#define PIR               8
#define BMP180            9
#define GARAGE_DOOR       10
#define WINDOW_CURTAIN    11 
#define OPTO_AND_RELAY    12
 
// Define the hold time of the outputs, by default the timer hold the relays for 16 times, so:
// 221x10x16ms that is about 35 seconds. Change the parameter inside FAST_x10ms() to change this time.
int WINDOW_TIMER = 211;  // MOVE THIS TO WEBCONFIG

int PULSE_TIMER = 1500;  // MOVE THIS TO WEBCONFIG

#include "PinsConfig.h"

//**********************  SLOTS VARIABLES  ***********************
byte ALARM;
byte TEMPERATURE;
byte HUMIDITY;
byte LED1; //byte LEDPWM0;
byte LED2; //byte LEDPWM1;
byte LED3; //byte LEDPWM2;
byte LED;
byte LEDRGB;
byte LDR;
byte DALLAS;
byte CAP0;
byte CAP1;
byte CAP2;
byte CAP3;
byte RELAY0;
byte RELAY1;
byte PRESSURE0;
byte BMP180TEMP;
byte THERMOSTAT;
byte PULSE0;
byte PULSE1;
byte T2X;         //To use with Garage and Curtain Mode
byte OPTO;        //To use with Opto + Relay Mode

float temp;       // Temporal Float Variable to store CAPx Readings

DHT dht11(DHTPIN, DHT11, 15);
DHT dht22(DHTPIN, DHT22, 15);

// Light calibration data
// out[] holds the values wanted in lux/10
#define SIZEOF 10
static const unsigned int out[] = { 1, 7, 30, 45, 65, 150, 300, 450, 2100, 13000};  // x10  //ULTIMO VALOR REFERENCIA
static const unsigned int in[]  = { 1, 100, 350, 430, 500, 680, 780, 950, 1005, 1024 };  // 0 - 1024

//#include <OneWire.h>
//#include <DallasTemperature.h>
OneWire ourWire(DALLASPIN); //Se establece el pin declarado como bus para la comunicaciÃ³n OneWire
DallasTemperature sensors(&ourWire); //Se instancia la librerÃ­a DallasTemperature

//SDA 5  SCL 4  PINS
//#include <SFE_BMP180.h>
//#include <Wire.h>
//#define ALTITUDE 20.0 // Altitude of reading location in meters
// You will need to create an SFE_BMP180 object, here called "pressure":
SFE_BMP180 pressure;

// ******************************************************************************************************************
// *************************************************  EMONCMS FUNCTION ***************************************************
// ******************************************************************************************************************
char serveremon[] = "emoncms.org";
char path[] = "/input/post.json?json=";
int port = 80; // port 80 is the default for HTTP
WiFiClient client;

String API = "";
String DeviceName = "Souliss";

void SendEmoncms(String inputstring, byte SLOT){
  
  float valueSLOT = mOutputAsFloat(SLOT);
  //LOG.println(valueSLOT);
  
  if (client.connect(serveremon, port))
  {
    if(DEBUG_EMONCMS){
      LOG.print("Emoncms: ");
      LOG.print(inputstring);
      LOG.print(": ");
      LOG.println(valueSLOT);
    }
      // Make a HTTP request:
    client.print("POST ");
    client.print(path);
    client.print(DeviceName);
    client.print("_");
    client.print(inputstring);
    client.print(":");
    client.print(valueSLOT);
    client.print("&apikey=");
    client.print(API);
    client.println(" HTTP/1.1");
    client.print("Host: ");
    client.println(serveremon);
    client.println("Connection: close");
    client.println();
  }
  else
  {
    // if you didn't get a connection to the server:
    if(DEBUG_EMONCMS){
      LOG.print("Emoncms: ");
      LOG.print(inputstring);
      LOG.print(": ");
      LOG.println("connection failed");
    }
  }
}


// ******************************************************************************************************************
// ***********************************************  SLOTS CONFIG ****************************************************
// ******************************************************************************************************************
void SLOT_CONFIG(){
  int NEXTSLOT = 0;
  LOG.println("SLOT CONFIG");  

  if(ALARM_MODE == 1 || S41 == ALARM_ENDSTOP || S42 == ALARM_ENDSTOP || S51 == ALARM_ENDSTOP || S52 == ALARM_ENDSTOP){
      ALARM = NEXTSLOT;
      NEXTSLOT = ALARM + 1;
      LOG.print("ALARM: "); LOG.println(ALARM);
  }
  
//  if(ALARM_MODE || ALARM_ENDSTOP){
//      ALARM = NEXTSLOT;
//      NEXTSLOT = ALARM + 1;
//      LOG.print("ALARM: "); LOG.println(ALARM);
//  }
  
  switch (S2) {  
        case 0:
            //NONE
            break;
        case 1:
            TEMPERATURE = NEXTSLOT;
            HUMIDITY = NEXTSLOT + 2;
            NEXTSLOT = HUMIDITY + 2;
            LOG.print("TEMP: "); LOG.println(TEMPERATURE);  
            LOG.print("HUMI: "); LOG.println(HUMIDITY);  
            break;
  }
        
  /*if(DHT_SENSOR){
      TEMPERATURE = NEXTSLOT;
      HUMIDITY = NEXTSLOT + 2;
      NEXTSLOT = HUMIDITY + 2;
      LOG.print("TEMP: "); LOG.println(TEMPERATURE);  
      LOG.print("HUMI: "); LOG.println(HUMIDITY);      
  }*/

  if(L1 == ONOFF_MODE || L1 == PIR_MODE || L1 == PULSE_MODE){
        LED1 = NEXTSLOT;
        NEXTSLOT = LED1 + 1;
        DEBUG.print("LED1: "); DEBUG.println(LED1);
  }else if(L1 == PWM_MODE){
        LED1 = NEXTSLOT;
        NEXTSLOT = LED1 + 2;
        DEBUG.print("LED1: PWM "); DEBUG.println(LED1);         
  }

  if(L2 == ONOFF_MODE || L2 == PIR_MODE || L2 == PULSE_MODE){
        LED2 = NEXTSLOT;
        NEXTSLOT = LED2 + 1;
        DEBUG.print("LED2: "); DEBUG.println(LED2);
  }else if(L2 == PWM_MODE){
        LED2 = NEXTSLOT;
        NEXTSLOT = LED2 + 2;
        DEBUG.print("LED2: PWM "); DEBUG.println(LED2);          
  }
  
  if(L3 == ONOFF_MODE || L3 == PIR_MODE || L3 == PULSE_MODE){
        LED3 = NEXTSLOT;
        NEXTSLOT = LED3 + 1;
        DEBUG.print("LED3: "); DEBUG.println(LED3);
  }else if(L3 == PWM_MODE){
        LED3 = NEXTSLOT;
        NEXTSLOT = LED3 + 2;
        DEBUG.print("LED3: PWM "); DEBUG.println(LED3);         
  } 
  // RGB MODE
  if(L1 == RGB_MODE && L2 == RGB_MODE && L3 == RGB_MODE){
        LEDRGB = NEXTSLOT;
        NEXTSLOT = LEDRGB + 4;
        DEBUG.print("LEDRGB: "); DEBUG.println(LEDRGB);  
  }

  /*
  if(ONOFF_MODE){
      LEDPWM0 = NEXTSLOT;
      LEDPWM1 = NEXTSLOT + 1;
      LEDPWM2 = NEXTSLOT + 2;
      NEXTSLOT = LEDPWM2 + 1;
      LOG.print("LEDONOFF0: "); LOG.println(LEDPWM0);  
      LOG.print("LEDONOFF1: "); LOG.println(LEDPWM1);  
      LOG.print("LEDONOFF2: "); LOG.println(LEDPWM2);       
  }
  if(PULSE_MODE){
      LEDPWM0 = NEXTSLOT;
      LEDPWM1 = NEXTSLOT + 1;
      LEDPWM2 = NEXTSLOT + 2;
      NEXTSLOT = LEDPWM2 + 1;
      LOG.print("LEDPULSE0: "); LOG.println(LEDPWM0);  
      LOG.print("LEDPULSE1: "); LOG.println(LEDPWM1);  
      LOG.print("LEDPULSE2: "); LOG.println(LEDPWM2);       
  }
  if(PWM_MODE || PIR_MODE){
      LEDPWM0 = NEXTSLOT;
      LEDPWM1 = NEXTSLOT + 2;
      NEXTSLOT = LEDPWM1 + 2;
      LOG.print("LEDPWM0: "); LOG.println(LEDPWM0);  
      LOG.print("LEDPWM1: "); LOG.println(LEDPWM1);  
  }
  if(PWM_MODE){
      LEDPWM2 = NEXTSLOT;
      NEXTSLOT = LEDPWM2 + 2;
      LOG.print("LEDPWM2: "); LOG.println(LEDPWM2);     
  }
  
  if(PIR_MODE){
      LED = NEXTSLOT;
      NEXTSLOT = LED + 1;
      LOG.print("LED: "); LOG.println(LED);       
  }
  
  if(RGB_MODE){
      LEDRGB = NEXTSLOT;
      NEXTSLOT = LEDRGB + 4;
      LOG.print("LEDRGB: "); LOG.println(LEDRGB);        
  }
  */

  switch (S3) {  
        case 0:
            //NONE
            break;
        case 1:
            LDR = NEXTSLOT;
            NEXTSLOT = LDR + 2;
            LOG.print("LDR: "); LOG.println(LDR);  
            break;
  }
  
  /*if(LDR_SENSOR){
      LDR = NEXTSLOT;
      NEXTSLOT = LDR + 2;
      LOG.print("LDR: "); LOG.println(LDR);        
  }*/

  switch (S1) {  
        case 0:
            //NONE
            break;
        case 1:
            DALLAS = NEXTSLOT;
            NEXTSLOT = DALLAS + (2 * dallas_qty);
            LOG.print("DALLAS: "); LOG.println(DALLAS);  
            break;
  }
  /*
  if(DALLAS_SENSOR){
      DALLAS = NEXTSLOT;
      NEXTSLOT = DALLAS + (2 * dallas_qty);
      LOG.print("DALLAS: "); LOG.println(DALLAS);
            
  }*/
  
  //GPIO 4-5 SLOT DEFINITIONS

  if(cap_debug){
      if(S41 == CAPACITIVE){
        CAP0 = NEXTSLOT;
        NEXTSLOT = CAP0 + 2;
        LOG.print("CAP0: "); LOG.println(CAP0); 
      }
      if(S42 == CAPACITIVE){
        CAP1 = NEXTSLOT;
        NEXTSLOT = CAP1 + 2;
        LOG.print("CAP1: "); LOG.println(CAP1); 
      }
      if(S51 == CAPACITIVE){
        CAP2 = NEXTSLOT;
        NEXTSLOT = CAP2 + 2;
        LOG.print("CAP2: "); LOG.println(CAP2); 
      }
      if(S52 == CAPACITIVE){
        CAP3 = NEXTSLOT;
        NEXTSLOT = CAP3 + 2;
        LOG.print("CAP3: "); LOG.println(CAP3); 
      } 
      if(S41 == CAPACITIVE || S42 == CAPACITIVE || S51 == CAPACITIVE || S52 == CAPACITIVE){
        THRE = NEXTSLOT;
        NEXTSLOT = THRE + 2;           
        LOG.print("THRE: "); LOG.println(THRE);
      }  
  }
//  switch (S4) {  
//        case 0:
//            //NONE
//            break;
//        case 1:
//            
//            break;
//        case 2:
//            CAP0 = NEXTSLOT; 
//            CAP1 = NEXTSLOT + 2;
//            THRE = NEXTSLOT + 4;
//            NEXTSLOT = THRE + 2;
//            LOG.print("CAP0: "); LOG.println(CAP0);   
//            LOG.print("CAP1: "); LOG.println(CAP1);             
//            LOG.print("THRE: "); LOG.println(THRE);                   
//            break;            
//  }
  /*if(CAPACITIVE && DEBUG_CAPSENSE){
      CAP0 = NEXTSLOT; 
      CAP1 = NEXTSLOT + 2;
      THRE = NEXTSLOT + 4;
      NEXTSLOT = THRE + 2;
      LOG.print("CAP0: "); LOG.println(CAP0);   
      LOG.print("CAP1: "); LOG.println(CAP1);             
      LOG.print("THRE: "); LOG.println(THRE);                   
  }*/
  if(S51 == RELAY){
    RELAY0 = NEXTSLOT;
    NEXTSLOT = RELAY0 + 1;
    LOG.print("RELAY0: "); LOG.println(RELAY0);
  }
  if(S52 == RELAY){
    RELAY1 = NEXTSLOT;
    NEXTSLOT = RELAY1 + 1;
    LOG.print("RELAY1: "); LOG.println(RELAY1);
  }
  if(S51 == PULSE_OUTPUT){
    PULSE0 = NEXTSLOT;
    NEXTSLOT = PULSE0 + 1;
    LOG.print("PULSE0: ");  LOG.println(PULSE0);
  }
  if(S52 == PULSE_OUTPUT){
    PULSE1 = NEXTSLOT;
    NEXTSLOT = PULSE1 + 1;
    LOG.print("PULSE1: ");  LOG.println(PULSE1);  
  }
//  switch (S51) {  
//        case 6:
//            RELAY0 = NEXTSLOT;
//            RELAY1 = NEXTSLOT + 1;
//            NEXTSLOT = RELAY1 + 1;
//            LOG.print("RELAY0: "); LOG.println(RELAY0);   
//            LOG.print("RELAY1: "); LOG.println(RELAY1);       
//            break;
//        case 7:
//            PULSE0 = NEXTSLOT;
//            PULSE1 = PULSE0 + 1;
//            NEXTSLOT = PULSE1 + 1;
//            LOG.print("PULSE0: ");  LOG.println(PULSE0); 
//            LOG.print("PULSE1: ");  LOG.println(PULSE1);                   
//            break;            
//                     
//  }
  switch (S52) {       
        case 9:
            PRESSURE0 = NEXTSLOT;  
            BMP180TEMP = NEXTSLOT + 2;
            NEXTSLOT = BMP180TEMP + 2;
            LOG.print("PRESSURE0: ");  LOG.println(PRESSURE0);   
            LOG.print("BMP180TEMP: "); LOG.println(BMP180TEMP);                   
            break;                        
        case 10:
            T2X = NEXTSLOT;
            NEXTSLOT = T2X + 1;
            LOG.print("GARAGE: ");  LOG.println(T2X);                    
            break; 
        case 11:
            T2X = NEXTSLOT;
            NEXTSLOT = T2X + 1;
            LOG.print("CURTAIN: ");  LOG.println(T2X);                    
            break;  
        case 12:
            OPTO = NEXTSLOT;
            NEXTSLOT = OPTO + 1;
            LOG.print("OPTO: ");  LOG.println(OPTO);                  
            break;                       
  }


  
  /*
  if(RELAY){
      RELAY0 = NEXTSLOT;
      RELAY1 = NEXTSLOT + 1;
      NEXTSLOT = RELAY1 + 1;
      LOG.print("RELAY0: "); LOG.println(RELAY0);   
      LOG.print("RELAY1: "); LOG.println(RELAY1);         
  }
  
  if(BMP180){
      PRESSURE0 = NEXTSLOT;  
      BMP180TEMP = NEXTSLOT + 2;
      NEXTSLOT = BMP180TEMP + 2;
      LOG.print("PRESSURE0: ");  LOG.println(PRESSURE0);   
      LOG.print("BMP180TEMP: "); LOG.println(BMP180TEMP);       
  } 
  if(PULSE_OUTPUT){
      PULSE0 = NEXTSLOT;
      PULSE1 = PULSE0 + 1;
      NEXTSLOT = PULSE1 + 1;
      LOG.print("PULSE0: ");  LOG.println(PULSE0); 
      LOG.print("PULSE1: ");  LOG.println(PULSE1); 
  }
  if(GARAGE_DOOR || WINDOW_CURTAIN){
      T2X = NEXTSLOT;
      NEXTSLOT = T2X + 1;
      LOG.print("T2X: ");  LOG.println(T2X); 
  }
  if(OPTO_AND_RELAY){
      OPTO = NEXTSLOT;
      NEXTSLOT = OPTO + 1;
      LOG.print("OPTO: ");  LOG.println(OPTO); 
  }*/
  
// ************************************** END OF SLOT_CONFIG() **************************
}

// ******************************************************************************************************************
// ***********************************************  EEPROM CONFIG ***************************************************
// ******************************************************************************************************************
//
//bool EEPROM_CONFIG(){
//    
//    //EEPROM CONFIGURATION READ.
//    DHT_SENSOR = false;
//    LDR_SENSOR = false;
//    DALLAS_SENSOR = false;
//    // DHT LDR DALLAS OPTIONS:
//    switch (byte0) {  
//        case 0:
//            //NONE
//            break;
//        case 1:
//            DHT_SENSOR = true;
//            break;
//        case 2:
//            LDR_SENSOR = true;
//            break;
//        case 3:
//            DALLAS_SENSOR = true;  
//            break;
//        case 4:
//            DHT_SENSOR = true;
//            LDR_SENSOR = true;
//            break;
//        case 5:
//            DHT_SENSOR = true;
//            DALLAS_SENSOR = true;  
//            break;
//        case 6:
//            LDR_SENSOR = true;
//            DALLAS_SENSOR = true;  
//            break;
//        case 7:
//            DHT_SENSOR = true;
//            LDR_SENSOR = true;
//            DALLAS_SENSOR = true;  
//            break;            
//    }
//    
//    LOG.print(DHT_SENSOR);
//    LOG.print(LDR_SENSOR);
//    LOG.print(DALLAS_SENSOR);
//    LOG.print(" DLD (DHT-LDR-DALLAS)");
//    LOG.print("\r\n");
//    
//    // PWM PIR RGB OPTIONS:
//    //switch (configuration[EEPROM_START+1]) {
//    ONOFF_MODE = false;
//    PULSE_MODE = false;
//    PWM_MODE = false;
//    PIR_MODE = false;
//    RGB_MODE = false;
//    ALARM_MODE = false;  
//    switch (byte1) {
//        case 0:
//            //NONE
//            break;
//		    case 1:
//            ONOFF_MODE = true;
//            break;
//        case 2:
//            PULSE_MODE = true;
//            break;	
//        case 3:
//            PWM_MODE = true;
//            break;
//        case 4:
//            PIR_MODE = true;
//            break;
//        case 5:
//            RGB_MODE = true;
//            break;
//        case 6:
//            PIR_MODE = true;
//            ALARM_MODE = true;
//            break;   
//    }
//	LOG.print(ONOFF_MODE);
//    LOG.print(PWM_MODE);
//    LOG.print(PIR_MODE);
//    LOG.print(RGB_MODE);
//    LOG.print(ALARM_MODE);
//    LOG.print(THERMOSTAT_MODE);
//    LOG.print(" OPPRAT (ONOFF-PWM-PIR-RGB-ALARM-THERMOSTAT)");
//    LOG.print("\r\n");
//    
//    // CAPACITIVE RELAY BMP180 OPTIONS
//    //switch (configuration[EEPROM_START+2]) {
//    CAPACITIVE = false;
//    RELAY = false;
//    BMP180 = false;
//    DEBUG_CAPSENSE = false;
//    BUTTONS = false;
//    BUTTONS_PULLUP = false;
//    ALARM_ENDSTOP = false;
//    BUTTONS_2_STATE = false;
//    PULSE_OUTPUT = false;
//    GARAGE_DOOR = false;
//    WINDOW_CURTAIN = false;
//    OPTO_AND_RELAY = false;
//    LOG.print("BYTE2: ");   
//    switch (byte2) { 
//        case 0:
//            DEBUG.print("NONE"); //NONE
//            break;
//        case 1:
//            CAPACITIVE = true;
//            DEBUG.print("CAPACITIVE");
//            break;
//        case 2:
//            RELAY = true;
//            DEBUG.print("RELAY");
//            break;
//        case 3:
//	    	    BMP180 = true;
//            DEBUG.print("BMP180");
//            break;
//        case 4:
//            CAPACITIVE = true;
//            DEBUG_CAPSENSE = true;
//            DEBUG.print("CAPACITIVE /W DEBUG");
//            break;    
//        case 5:
//            BUTTONS = true;
//            DEBUG.print("BUTTONS");
//            break; 
//        case 6:
//            BUTTONS_PULLUP = true;
//            DEBUG.print("BUTTONS_PULLUP");
//            break; 
//        case 7:
//            ALARM_ENDSTOP = true;
//            DEBUG.print("ALARM_ENDSTOP");
//            break; 
//        case 8:
//            BUTTONS_2_STATE = true;
//            DEBUG.print("BUTTONS_2_STATE");
//            break;
//        case 9:
//            PULSE_OUTPUT = true;
//            DEBUG.print("PULSE_OUTPUT");
//            break;
//        case 10:            
//            GARAGE_DOOR = true;
//            DEBUG.print("GARAGE_DOOR");
//            break;
//        case 11:            
//            WINDOW_CURTAIN = true;
//            DEBUG.print("WINDOW_CURTAIN");
//            break;
//        case 12:            
//            OPTO_AND_RELAY = true;
//            DEBUG.print("220V_OPTO_AND_RELAY");    
//            break;   
//    }
//    LOG.println("");
//    return 1;
//
//}

void WriteConfig_Slots() {
  LOG.println("Writing Config");
  bitWrite(OPTIONS1, 0, usartbridge);
  bitWrite(OPTIONS1, 1, IR_ENABLE);
  bitWrite(OPTIONS1, 2, dht_type);
  bitWrite(OPTIONS1, 3, Send_Emon);
  bitWrite(OPTIONS1, 4, DEBUG_LOG);
  bitWrite(OPTIONS1, 5, DEBUG_CAPSENSE);
  bitWrite(OPTIONS1, 6, DEBUG_CAPSENSE_ALL);
  bitWrite(OPTIONS1, 7, DEBUG_DHT);

  bitWrite(OPTIONS2, 0, DEBUG_PRESSURE);
  bitWrite(OPTIONS2, 1, DEBUG_GETLUX);
  bitWrite(OPTIONS2, 2, DEBUG_DALLAS);
  bitWrite(OPTIONS2, 3, DEBUG_EMONCMS);
  bitWrite(OPTIONS2, 4, DEBUG_IR);
  bitWrite(OPTIONS2, 5, DEBUG_IR_FULL);
  bitWrite(OPTIONS2, 6, DEBUG_PLC);
  //bitWrite(OPTIONS2, 7, DEBUG_PLC);

  bitWrite(OPTIONS3, 0, L1PIR);
  bitWrite(OPTIONS3, 1, L2PIR);
  bitWrite(OPTIONS3, 2, L3PIR);
  bitWrite(OPTIONS3, 3, ALARM_MODE);
  bitWrite(OPTIONS3, 4, cap_debug);
//  bitWrite(OPTIONS3, 5, DEBUG_IR_FULL);
//  bitWrite(OPTIONS3, 6, DEBUG_PLC);
//  bitWrite(OPTIONS3, 7, DEBUG_PLC);  
  
  
  EEPROM.write(STORE_CUSTOM,	  cap_thresold);
  EEPROM.write(STORE_CUSTOM+1, 	L1);
  EEPROM.write(STORE_CUSTOM+2, 	L2);
  EEPROM.write(STORE_CUSTOM+3, 	L3);
  EEPROM.write(STORE_CUSTOM+4,  S1);
  EEPROM.write(STORE_CUSTOM+5,  S2);
  EEPROM.write(STORE_CUSTOM+6,  S3);
  EEPROM.write(STORE_CUSTOM+7,  S41);
  EEPROM.write(STORE_CUSTOM+8,  S42);
  EEPROM.write(STORE_CUSTOM+9,  S51);
  EEPROM.write(STORE_CUSTOM+10,  S52);  
  EEPROM.write(STORE_CUSTOM+11,  S6);
  EEPROM.write(STORE_CUSTOM+12, OPTIONS1);
  EEPROM.write(STORE_CUSTOM+13, OPTIONS2);
  EEPROM.write(STORE_CUSTOM+14, OPTIONS3);
  EEPROM.write(STORE_CUSTOM+15, ALTITUDE/20);
  Store_String(STORE_CUSTOM+16, DeviceName);     //MAX 10 
  Store_String(STORE_CUSTOM+26,	API);      //MAX 32    480
  EEPROM.write(STORE_CUSTOM+59, dallas_qty);

  
  EEPROM.commit();
}

void ReadConfig_Slots()
{

	LOG.println(F("Reading Configuration"));
	cap_thresold = EEPROM.read(STORE_CUSTOM);
	L1 =        EEPROM.read(STORE_CUSTOM+1);
	L2 =        EEPROM.read(STORE_CUSTOM+2);
	L3 =        EEPROM.read(STORE_CUSTOM+3);
  S1 =        EEPROM.read(STORE_CUSTOM+4);
  S2 =        EEPROM.read(STORE_CUSTOM+5);
  S3 =        EEPROM.read(STORE_CUSTOM+6);
  S41 =       EEPROM.read(STORE_CUSTOM+7);
  S42 =       EEPROM.read(STORE_CUSTOM+8);
  S51 =       EEPROM.read(STORE_CUSTOM+9);
  S52 =       EEPROM.read(STORE_CUSTOM+10);
  S6 =        EEPROM.read(STORE_CUSTOM+11);
  OPTIONS1 =  EEPROM.read(STORE_CUSTOM+12);
  OPTIONS2 =  EEPROM.read(STORE_CUSTOM+13);
  OPTIONS3 =  EEPROM.read(STORE_CUSTOM+14);  
 	ALTITUDE = EEPROM.read(STORE_CUSTOM+15)*20;
  DeviceName = Return_String(STORE_CUSTOM+16,10);
	API = Return_String(STORE_CUSTOM+26,32);
	dallas_qty = EEPROM.read(STORE_CUSTOM+59);

  usartbridge = bitRead(OPTIONS1, 0);
  IR_ENABLE =   bitRead(OPTIONS1, 1);
  dht_type =    bitRead(OPTIONS1, 2);
  Send_Emon =   bitRead(OPTIONS1, 3);

  DEBUG_LOG =           bitRead(OPTIONS1, 4);
  DEBUG_CAPSENSE =      bitRead(OPTIONS1, 5);
  DEBUG_CAPSENSE_ALL =  bitRead(OPTIONS1, 6);
  DEBUG_DHT =           bitRead(OPTIONS1, 7);

  DEBUG_PRESSURE =  bitRead(OPTIONS2, 0);
  DEBUG_GETLUX =    bitRead(OPTIONS2, 1);
  DEBUG_DALLAS =    bitRead(OPTIONS2, 2);
  DEBUG_EMONCMS =   bitRead(OPTIONS2, 3);
  DEBUG_IR =        bitRead(OPTIONS2, 4);
  DEBUG_IR_FULL =   bitRead(OPTIONS2, 5);
  DEBUG_PLC =       bitRead(OPTIONS2, 6);
  //DEBUG_LOG =     bitRead(OPTIONS2, 7);

  L1PIR =         bitRead(OPTIONS3, 0);
  L2PIR =         bitRead(OPTIONS3, 1);
  L3PIR =         bitRead(OPTIONS3, 2);
  ALARM_MODE =    bitRead(OPTIONS3, 3);
  cap_debug =     bitRead(OPTIONS3, 4);
//  DEBUG_IR_FULL = bitRead(OPTIONS3, 5);
//  DEBUG_PLC =     bitRead(OPTIONS3, 6);
//  DEBUG_LOG =   bitRead(OPTIONS3, 7);
  
	//*** DEBUG TO SERIAL ***
	LOG.print(F("DeviceName: "));
	LOG.println(DeviceName);
	LOG.print(F("API: "));
	LOG.println(API);
	LOG.print(F("L1: "));
	LOG.println(L1);
  LOG.print(F("L1PIR: "));
  LOG.println(L1PIR);
  LOG.print(F("L2: "));
  LOG.println(L2);
  LOG.print(F("L2PIR: "));
  LOG.println(L2PIR);  
  LOG.print(F("L3: "));
  LOG.println(L3);
  LOG.print(F("L3PIR: "));
  LOG.println(L3PIR);  
  LOG.print(F("S1: "));
  LOG.println(S1);
  LOG.print(F("S2: "));
  LOG.println(S2);
  LOG.print(F("S3: "));
  LOG.println(S3);
  LOG.print(F("S41: "));
  LOG.println(S41);
  LOG.print(F("S42: "));
  LOG.println(S42);
  LOG.print(F("S51: "));
  LOG.println(S51);
  LOG.print(F("S52: "));
  LOG.println(S52);  
  LOG.print(F("S6: "));
  LOG.println(S6);
  LOG.print(F("OPTIONS1: "));
  LOG.println(OPTIONS1);
  LOG.print(F("OPTIONS2: "));
  LOG.println(OPTIONS2); 
  LOG.print(F("OPTIONS3: "));
  LOG.println(OPTIONS3);  
  LOG.print(F("usartbridge: "));
  LOG.println(usartbridge);
  LOG.print(F("IR_ENABLE: "));
  LOG.println(IR_ENABLE);
  LOG.print(F("dht_type: "));
  LOG.println(dht_type);
  LOG.print(F("Send Emon: "));
  LOG.println(Send_Emon);
  LOG.print(F("Dallas_qty: "));
  LOG.println(dallas_qty);
  LOG.print(F("cap_debug: "));
  LOG.println(cap_debug);  
  LOG.print(F("ALARM_MODE: "));
  LOG.println(ALARM_MODE);
}
// ******************************************************************************************************************
// *******************************************       setColor FUNCTION          *************************************
// ******************************************************************************************************************
void setColor(uint8_t slot, uint8_t R, uint8_t G, uint8_t B)
{
  mInput(slot+1)=R;
  mInput(slot+2)=G;
  mInput(slot+3)=B;
  mInput(slot)=Souliss_T1n_Set;
}

// ******************************************************************************************************************
// *************************************       setBrightColor FUNCTION          *************************************
// ******************************************************************************************************************
void setBrightColor(uint8_t slot, uint8_t color, uint8_t value_state)  //color:  Red=1 Green=2 Blue=3
{
  for (int i = 1; i<=3; i++)
  {
    if ( i == color ) 
    { 
      if (value_state == Souliss_T1n_BrightUp && mOutput(slot+color) +10 <= 254) { mInput(slot+color) = mOutput(slot+color) +10; }
      if (value_state == Souliss_T1n_BrightDown && mOutput(slot+color) -10 >= 0) { mInput(slot+color) = mOutput(slot+color) -10; }
    }
    else 
    {
      mInput(slot+i)=mOutput(slot+i);      
    }
  }
  mInput(slot)=Souliss_T1n_Set;
  
}

// ******************************************************************************************************************
// *******************************************       CAPACITIVO 1 PIN          **************************************
// ******************************************************************************************************************
uint8_t readCapacitivePin(int pinToMeasure) {
  pinMode(pinToMeasure, OUTPUT);
  digitalWrite(pinToMeasure, LOW);
  delay(1);
  // Prevent the timer IRQ from disturbing our measurement
  noInterrupts();
  // Make the pin an input with the internal pull-up on
  pinMode(pinToMeasure, INPUT_PULLUP);

  // Now see how long the pin to get pulled up. This manual unrolling of the loop
  // decreases the number of hardware cycles between each read of the pin,
  // thus increasing sensitivity.
  uint8_t cycles = 35;//17;
       if (digitalRead(pinToMeasure)) { cycles =  0;}
  else if (digitalRead(pinToMeasure)) { cycles =  1;}
  else if (digitalRead(pinToMeasure)) { cycles =  2;}
  else if (digitalRead(pinToMeasure)) { cycles =  3;}
  else if (digitalRead(pinToMeasure)) { cycles =  4;}
  else if (digitalRead(pinToMeasure)) { cycles =  5;}
  else if (digitalRead(pinToMeasure)) { cycles =  6;}
  else if (digitalRead(pinToMeasure)) { cycles =  7;}
  else if (digitalRead(pinToMeasure)) { cycles =  8;}
  else if (digitalRead(pinToMeasure)) { cycles =  9;}
  else if (digitalRead(pinToMeasure)) { cycles = 10;}
  else if (digitalRead(pinToMeasure)) { cycles = 11;}
  else if (digitalRead(pinToMeasure)) { cycles = 12;}
  else if (digitalRead(pinToMeasure)) { cycles = 13;}
  else if (digitalRead(pinToMeasure)) { cycles = 14;}
  else if (digitalRead(pinToMeasure)) { cycles = 15;}
  else if (digitalRead(pinToMeasure)) { cycles = 16;}
  else if (digitalRead(pinToMeasure)) { cycles = 17;}
  else if (digitalRead(pinToMeasure)) { cycles = 18;}
  else if (digitalRead(pinToMeasure)) { cycles = 19;}  
  else if (digitalRead(pinToMeasure)) { cycles = 20;}
  else if (digitalRead(pinToMeasure)) { cycles = 21;}
  else if (digitalRead(pinToMeasure)) { cycles = 22;}    
  else if (digitalRead(pinToMeasure)) { cycles = 23;}
  else if (digitalRead(pinToMeasure)) { cycles = 24;}
  else if (digitalRead(pinToMeasure)) { cycles = 25;}
  else if (digitalRead(pinToMeasure)) { cycles = 26;}
  else if (digitalRead(pinToMeasure)) { cycles = 27;}
  else if (digitalRead(pinToMeasure)) { cycles = 28;}
  else if (digitalRead(pinToMeasure)) { cycles = 29;}
  else if (digitalRead(pinToMeasure)) { cycles = 30;}
  else if (digitalRead(pinToMeasure)) { cycles = 31;}
  else if (digitalRead(pinToMeasure)) { cycles = 32;}
  else if (digitalRead(pinToMeasure)) { cycles = 33;}
  else if (digitalRead(pinToMeasure)) { cycles = 34;}
  
  
  
  // End of timing-critical section
  interrupts();

  // Discharge the pin again by setting it low and output
  //  It's important to leave the pins low if you want to 
  //  be able to touch more than 1 sensor at a time - if
  //  the sensor is left pulled high, when you touch
  //  two sensors, your body will transfer the charge between
  //  sensors.
  digitalWrite(pinToMeasure, LOW);
  pinMode(pinToMeasure, OUTPUT);

  return cycles;
}

unsigned long timer;

uint8_t Souliss_CapSense(uint8_t slot, uint8_t value, uint8_t value_hold, uint8_t pin, uint8_t thresold_value, int holdtime) {

    int cycles = readCapacitivePin(pin); 
    if(DEBUG_CAPSENSE_ALL){
          if(millis()%300==0) LOG.println("Pin\tSlotIn\tOut\tPWM\tcycles\tReturn");
          LOG.print(pin);
          LOG.print("\t");        
          LOG.print(InPin[pin]);
          LOG.print("\t");        
          LOG.print(mInput(slot));
          LOG.print("\t");        
          LOG.print(mOutput(slot));
          LOG.print("\t");
          LOG.print(mOutput(slot+1));
          LOG.print("\t");
          LOG.print(cycles); 
          LOG.print("\t");
          LOG.print(thresold_value); 
          LOG.print("\t");          
          LOG.print(abs(millis()-timer)); 
          LOG.print("\t");          

    }
  
    if(cycles > thresold_value && (InPin[pin] == 10 || InPin[pin] == 11)){
               InPin[pin] = PINSET;
    }
        
    if(cycles > thresold_value && (InPin[pin]==PINRESET))
	{
		timer = millis();								// Record time
		InPin[pin] = PINSET;
		
		return InPin[pin];
	}
	else if(cycles > thresold_value && (abs(millis()-timer) > holdtime) && (InPin[pin]==PINSET || InPin[pin]==PINACTIVE))
	{
	    if(AUTOCALIBRATE && (abs(millis()-timer) > 15000)){
                //config.cap_thresold = cycles + 4;
                //EEPROM.write(4, cycles + 4);
                //EEPROM.commit();
                if(DEBUG_CAPSENSE) { 
                  LOG.print("Autocalibrate: ");
                  //LOG.println(config.cap_thresold);
                }
                //return config.cap_thresold;
            }else{
                InPin[pin] = PINACTIVE;								// Stay there till pushbutton is released
      		if (memory_map)	memory_map[MaCaco_IN_s + slot] = value_hold;
                return value_hold;
            }
	}
    else if(cycles < thresold_value && (InPin[pin]==PINSET)) {
		InPin[pin] = 10;
        return InPin[pin];
	}
	else if(cycles < thresold_value && (InPin[pin]==PINACTIVE)) {
		InPin[pin] = 11;
        return InPin[pin];
    }
    else if(cycles < thresold_value && InPin[pin] == 10){
        if(mInput(slot) == Souliss_T1n_OffCmd || mInput(slot) == Souliss_T1n_OnCmd){
          	if(mOutput(slot+1) == 0) mOutput(slot) = Souliss_T1n_OffCoil;
            else                     mOutput(slot) = Souliss_T1n_OnCoil;
                memory_map[MaCaco_IN_s + slot] = Souliss_T1n_RstCmd;			// Reset
        }
        else memory_map[MaCaco_IN_s + slot] = value; 
        
        InPin[pin] = PINRESET;     
        return value;
    }
    else if(cycles < thresold_value && InPin[pin] == 11){
          InPin[pin] = PINRESET;
          if(mOutput(slot+1) == 0) mOutput(slot) = Souliss_T1n_OffCoil;
          else                     mOutput(slot) = Souliss_T1n_OnCoil;
          memory_map[MaCaco_IN_s + slot] = Souliss_T1n_RstCmd;			// Reset
    }          
	
    // return MaCaco_NODATACHANGED;

}

//***************************************************************************
// Calculate lux based on rawADC reading from LDR returns value in lux/10
//***************************************************************************
int Souliss_GetLux(const unsigned int* _in, const unsigned int* _out, byte size)
{
	//boolean DEBUG_GETLUX = 1;
        // take care the value is within range
	// val = constrain(val, _in[0], _in[size-1]);
        
	int val = analogRead(A0);
	delay(10);  //TODO: TEST WITH LOWER VALUES

  if(DEBUG_GETLUX){
    LOG.print(" AnalogRead: ");
    LOG.print(val);
    LOG.print("\r\n");
  }
  if (val <= _in[0]) return _out[0];
	if (val >= _in[size-1]) return _out[size-1];

	// search right interval
	byte pos = 1;  // _in[0] allready tested
	while(val > _in[pos]) pos++;

	// this will handle all exact "points" in the _in array
	if (val == _in[pos]) return _out[pos];

	// interpolate in the right segment for the rest
	return map(val, _in[pos-1], _in[pos], _out[pos-1], _out[pos]);
}

//***************************************************************************
//*                            DHT READING FUNCTION                         *
//***************************************************************************
void Souliss_GetDHT(uint8_t SLOT_TEMPERATURE, uint8_t SLOT_HUMIDITY, boolean Celsius){
    float h, t, f;  //Variables
    if(!dht_type){  
        h = dht11.readHumidity();
        // Read temperature as Celsius
        t = dht11.readTemperature();
        // Read temperature as Fahrenheit
        f = dht11.readTemperature(true);
    }
    else {
        h = dht22.readHumidity();
        // Read temperature as Celsius
        t = dht22.readTemperature();
        // Read temperature as Fahrenheit
        f = dht22.readTemperature(true);      
    }          
    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t) || isnan(f)) {
        if(DEBUG_DHT) LOG.print("Failed to read from DHT sensor!\r\n");
    }
    // Compute heat index
    // Must send in temp in Fahrenheit!
    //float hi = dht.computeHeatIndex(f, h);
    //float hic = (f - 32) / 1.8;
    //Souliss_ImportAnalog(memory_map, HEAT, &hic);
    if(DEBUG_DHT) {
      LOG.print("DHT Temp: ");
      LOG.print(t);
      LOG.print(" DHT Hum: ");
      LOG.println(h);
    }
    if(Celsius) Souliss_ImportAnalog(memory_map, SLOT_TEMPERATURE, &t);
    else        Souliss_ImportAnalog(memory_map, SLOT_TEMPERATURE, &f);
    
    Souliss_ImportAnalog(memory_map, SLOT_HUMIDITY, &h);	
          
}


// ***************************************************************************
// *                         BMP180 I2C READING FUNCTION                     *
// ***************************************************************************
float Souliss_GetPressure_BMP180(uint8_t SLOT_PRESSURE, uint8_t SLOT_TEMPERATURE){
  
  char status;
  double T,P,p0,a;

  // Loop here getting pressure readings every 10 seconds.

  // If you want sea-level-compensated pressure, as used in weather reports,
  // you will need to know the altitude at which your measurements are taken.
  // We're using a constant called ALTITUDE in this sketch:
  
  if(DEBUG_PRESSURE){
    LOG.print(F("provided altitude: "));
    LOG.print(ALTITUDE,0);
    LOG.print(F(" meters, "));
    LOG.print(ALTITUDE*3.28084,0);
    LOG.print(F(" feet\r\n"));
  }  
  // If you want to measure altitude, and not pressure, you will instead need
  // to provide a known baseline pressure. This is shown at the end of the sketch.

  // You must first get a temperature measurement to perform a pressure reading.
  
  // Start a temperature measurement:
  // If request is successful, the number of ms to wait is returned.
  // If request is unsuccessful, 0 is returned.

  status = pressure.startTemperature();
  if (status != 0)
  {
    // Wait for the measurement to complete:
    delay(status);

    // Retrieve the completed temperature measurement:
    // Note that the measurement is stored in the variable T.
    // Function returns 1 if successful, 0 if failure.

    status = pressure.getTemperature(T);
    if (status != 0)
    {
      if(DEBUG_PRESSURE){ 
        // Print out the measurement:
        LOG.print(F("temperature: "));
        LOG.print(T,2);
        LOG.print(F(" deg C, "));
        LOG.print((9.0/5.0)*T+32.0,2);
        LOG.print(F(" deg F\r\n"));
      }    
      // Start a pressure measurement:
      // The parameter is the oversampling setting, from 0 to 3 (highest res, longest wait).
      // If request is successful, the number of ms to wait is returned.
      // If request is unsuccessful, 0 is returned.

      status = pressure.startPressure(3);
      if (status != 0)
      {
        // Wait for the measurement to complete:
        delay(status);

        // Retrieve the completed pressure measurement:
        // Note that the measurement is stored in the variable P.
        // Note also that the function requires the previous temperature measurement (T).
        // (If temperature is stable, you can do one temperature measurement for a number of pressure measurements.)
        // Function returns 1 if successful, 0 if failure.

        status = pressure.getPressure(P,T);
        if (status != 0)
        {
          if(DEBUG_PRESSURE){
            // Print out the measurement:
            LOG.print(F("absolute pressure: "));
            LOG.print(P,2);
            LOG.print(F(" mb, "));
            LOG.print(P*0.0295333727,2);
            LOG.print(F(" inHg\r\n"));
          }
          // The pressure sensor returns abolute pressure, which varies with altitude.
          // To remove the effects of altitude, use the sealevel function and your current altitude.
          // This number is commonly used in weather reports.
          // Parameters: P = absolute pressure in mb, ALTITUDE = current altitude in m.
          // Result: p0 = sea-level compensated pressure in mb

          p0 = pressure.sealevel(P,ALTITUDE); // we're at 1655 meters (Boulder, CO)
          if(DEBUG_PRESSURE){
            LOG.print(F("relative (sea-level) pressure: "));
            LOG.print(p0,2);
            LOG.print(F(" mb, "));
            LOG.print(p0*0.0295333727,2);
            LOG.print(F(" inHg\r\n"));
          }
          // On the other hand, if you want to determine your altitude from the pressure reading,
          // use the altitude function along with a baseline pressure (sea-level or other).
          // Parameters: P = absolute pressure in mb, p0 = baseline pressure in mb.
          // Result: a = altitude in m.

          a = pressure.altitude(P,p0);
          if(DEBUG_PRESSURE){
            LOG.print(F("computed altitude: "));
            LOG.print(a,0);
            LOG.print(F(" meters, "));
            LOG.print(a*3.28084,0);
            LOG.print(F(" feet\r\n"));
          }
          float pressure = p0;
          float temperature = T; 
          Souliss_ImportAnalog(memory_map, SLOT_PRESSURE, &pressure);
          Souliss_ImportAnalog(memory_map, SLOT_TEMPERATURE, &temperature);
          return p0; 
        }
        else if(DEBUG_PRESSURE) LOG.print(F("error retrieving pressure measurement\n"));
      }
      else if(DEBUG_PRESSURE) LOG.print(F("error starting pressure measurement\n"));
    }
    else if(DEBUG_PRESSURE) LOG.print(F("error retrieving temperature measurement\n"));
  }
  else if(DEBUG_PRESSURE) LOG.print(F("error starting temperature measurement\n"));
 
}
boolean last_state;

uint8_t SoulissPLC_Read(uint8_t slot, uint8_t button_pin, uint8_t plc_pin, uint8_t relay_pin){
                           
            if(mInput(slot) == Souliss_T1n_OnCmd || mInput(slot) == Souliss_T1n_OffCmd){
                 if(DEBUG_PLC){ LOG.println("PLC Command Received: "); LOG.println(mInput(slot)); } 
                 digitalWrite(relay_pin, !digitalRead(relay_pin)); 
                 memory_map[MaCaco_IN_s + slot] = Souliss_T1n_RstCmd;
                 return 99;  //Return 99 when cmd received from Souliss App Just for test.
            }
  
            if(!digitalRead(button_pin) && InPin[button_pin] == PINRESET) { 
                 InPin[button_pin] = PINSET;
                 if(DEBUG_PLC) LOG.println(InPin[button_pin]);                 
                 return InPin[button_pin];
            }
            if(!digitalRead(button_pin) && InPin[button_pin] == PINSET) { 
                 InPin[button_pin] = PINACTIVE;
                 if(DEBUG_PLC) LOG.println(InPin[button_pin]);                 
                 return InPin[button_pin];
            }
            if(digitalRead(button_pin) && InPin[button_pin] == PINACTIVE) { 
                 InPin[button_pin] = PINRELEASED;
                 if(DEBUG_PLC) LOG.println(InPin[button_pin]);                 
                 return InPin[button_pin];
            }
            if(digitalRead(button_pin) && InPin[button_pin] == PINRELEASED) { 
                 digitalWrite(relay_pin, !digitalRead(relay_pin));
                 InPin[button_pin] = PINRESET;
                 if(DEBUG_PLC) LOG.println(InPin[button_pin]);                 
                 return InPin[button_pin];
            }
            
            
            if(!digitalRead(plc_pin)) {   
                memory_map[MaCaco_OUT_s + slot] = Souliss_T1n_OnCoil; 
                memory_map[MaCaco_IN_s + slot] = Souliss_T1n_RstCmd;                
                if(DEBUG_PLC) FAST_2110ms() { LOG.println("PLC ON"); }
                
            }
            else {
                memory_map[MaCaco_OUT_s + slot] = Souliss_T1n_OffCoil;
                memory_map[MaCaco_IN_s + slot] = Souliss_T1n_RstCmd;                
                if(DEBUG_PLC) FAST_2110ms() { LOG.println("PLC OFF"); }             
            }
}

#endif
