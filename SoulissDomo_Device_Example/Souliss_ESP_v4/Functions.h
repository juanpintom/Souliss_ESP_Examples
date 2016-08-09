#ifndef Functions_h
#define Functions_h

int valorPWM;

boolean Emonlib = 0;      

#define STORE_CUSTOM 430   // START BYTE ON EEPROM TO STORE CUSTOM VALUES.

//Autocalibrate Capacitive Sensors ON
#define AUTOCALIBRATE         1

// **********************************************************************
// **************************** DEBUG OPTIONS ***************************
// **********************************************************************

boolean DEBUG_LOG           = 1;  //CHANGED TO WEBCONFIG
boolean DEBUG_CAPSENSE      = 0;
boolean DEBUG_CAPSENSE_ALL  = 0;
boolean DEBUG_DHT           = 1;
boolean DEBUG_PRESSURE      = 1;
boolean DEBUG_GETLUX        = 1;
boolean DEBUG_DALLAS        = 1;
boolean DEBUG_EMONCMS       = 0;
boolean DEBUG_IR            = 1;
boolean DEBUG_IR_FULL       = 0;
boolean DEBUG_PLC           = 0;

#define DEBUG if(DEBUG_LOG)LOG

//INSIDE BUTTON ON GPIO0 VARIABLE:
boolean button0 = false;

// **********************************************************************
// ************************* CONFIG VARIABLES ***************************
// **********************************************************************
byte L1; //L1 OUTPUT --> LIGHT 1
byte L2; //L2 OUTPUT --> LIGHT 2
byte L3; //L3 OUTPUT --> LIGHT 3

byte S1; //S1 --> SENSOR 1 INPUT (DALLAS)
byte S2; //S2 --> SENSOR 2 INPUT (DHT)
byte S3; //S3 --> SENSOR 3 INPUT (LDR)
byte S41; //S4 --> SENSOR 41 INPUT/OUTPUT
byte S42; //S4 --> SENSOR 42 INPUT/OUTPUT
byte S51; //S5 --> SENSOR 51 INPUT/OUTPUT
byte S52; //S5 --> SENSOR 52 INPUT/OUTPUT
byte S6; //S6 --> IR REMOTE SENSOR 
byte L1PIR; //PIR ASSIGNED TO L1
byte L2PIR; //PIR ASSIGNED TO L2
byte L3PIR; //PIR ASSIGNED TO L3
boolean L1PIREN;//PIR ENABLED ON L1
boolean L2PIREN;//PIR ENABLED ON L1
boolean L3PIREN;//PIR ENABLED ON L1


// **********************************************************************
// ************************** BOOLEAN OPTIONS ***************************
// **********************************************************************

// -------------------------  OPTIONS 1 VARIABLE ------------------------
byte OPTIONS1;  

boolean usartbridge = false;
boolean Send_Emon = true;
boolean IR_ENABLE = true;
boolean dht_type = true;  //FALSE = DHT11 , TRUE = DHT22

// ---------------------  OPTIONS 2 AND 3 VARIABLES ---------------------
byte OPTIONS2;   //STORE DEBUG OPTIONS
byte OPTIONS3;   //STORE DEBUG OPTIONS
boolean ALARM_MODE = false;
boolean cap_debug = false;

// --------------------------  OTHER VARIABLES ---------------------------
byte cap_thresold;
int ALTITUDE = 20;
byte dallas_qty = 1;


// **********************************************************************
// ************************** #DEFINE OPTIONS ***************************
// **********************************************************************


// -------------------------- S1 #DEFINES ------------------------------
#define DALLAS_SENSOR 1

// -------------------------- S2 #DEFINES ------------------------------
#define DHT_SENSOR    1

// -------------------------- S3 #DEFINES ------------------------------
#define LDR_SENSOR    1

// ---------------------- L1 L2 L3 #DEFINES ----------------------------
#define ONOFF_MODE    1
#define PWM_MODE      2    
#define RGB_MODE      3
#define PIR_MODE      4
#define PULSE_MODE    5

// -------------------------- PIR LIGHT ON DEFINE ------------------------------
#define LIGHT_ON_CYCLE 10  //Light ON for 10 cycles if triggered by a PIR sensor


// ----------------------- S41 S42 S51 S52 #DEFINES ----------------------------
#define CAPACITIVE        1     
#define BUTTONS           2
#define BUTTONS_PULLUP    3
#define ALARM_ENDSTOP     4
#define BUTTONS_2_STATE   5

#define RELAY             6
#define INVRELAY          7         
#define PULSE_OUTPUT      8
#define TRIAC             9
#define PIR               10
#define BMP180            11
#define GARAGE_DOOR       12
#define WINDOW_CURTAIN    13 
#define OPTO_AND_RELAY    14
#define OLED              15


// **********************************************************************
// ******************** WINDOW TIMER #DEFINE OPTION *********************
// ********************************************************************** 
// Define the hold time of the outputs, by default the timer hold the relays for 16 times, so:
// 221x10x16ms that is about 35 seconds. Change the parameter inside FAST_x10ms() to change this time.
int WINDOW_TIMER = 211;  // MOVE THIS TO WEBCONFIG

 

// **********************************************************************
// ********************* PULSE TIMER #DEFINE OPTION *********************
// **********************************************************************
int PULSE_TIMER = 1500;  // MOVE THIS TO WEBCONFIG

 


// **********************************************************************
// ********************* INCLUDE PINSCONFIG.H ***************************
// **********************************************************************
#include "PinsConfig.h"

 

// **********************************************************************
// *********************** SLOTS VARIABLES ******************************
// **********************************************************************
byte ALARM;
byte TEMPERATURE;
byte HUMIDITY;
byte LED1; 
byte LED2; 
byte LED3; 
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


// ******************************************************************************************************************
// *************************************************  EMONCMS FUNCTION **********************************************
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
  float timetosend = millis();
  
  if (client.connect(serveremon, port))
  {
    if(DEBUG_EMONCMS){
      DEBUG.print("Emoncms: ");
      DEBUG.print(inputstring);
      DEBUG.print(": ");
      DEBUG.print(valueSLOT);
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
    if(DEBUG_EMONCMS) { 
      DEBUG.print(" Sent takes: "); 
      DEBUG.println(millis()-timetosend); 
    }
  }
  else
  {
    // if you didn't get a connection to the server:
    if(DEBUG_EMONCMS){
      DEBUG.print("Emoncms: ");
      DEBUG.print(inputstring);
      DEBUG.print(": ");
      DEBUG.println("connection failed");
    }
  }
}

// ******************************************************************************************************************
// ******************************************  THINGSPEAK FUNCTION **************************************************
// ******************************************************************************************************************

String apiKey = "2C9X7CQ2BH7N6E2O";

// ThingSpeak API
const char* serverTS = "api.thingspeak.com";

void SendThingspeak(byte SLOT1, byte SLOT2){
  
  float valueSLOT1 = mOutputAsFloat(SLOT1);
  float valueSLOT2 = mOutputAsFloat(SLOT2);
  float timetosend = millis();
  
  if (client.connect(serverTS,80)) {  //   "184.106.153.149" or api.thingspeak.com
    String postStr = apiKey;
           postStr +="&field1=";
           postStr += String(valueSLOT1);
           postStr +="&field2=";
           postStr += String(valueSLOT2);
           postStr += "\r\n\r\n";
 
     client.print("POST /update HTTP/1.1\n"); 
     client.print("Host: api.thingspeak.com\n"); 
     client.print("Connection: close\n"); 
     client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n"); 
     client.print("Content-Type: application/x-www-form-urlencoded\n"); 
     client.print("Content-Length: "); 
     client.print(postStr.length()); 
     client.print("\n\n"); 
     client.print(postStr);
           
 
     DEBUG.print("Temperature: ");
     DEBUG.print(valueSLOT1);
     DEBUG.print(" degrees Celcius Humidity: "); 
     DEBUG.print(valueSLOT2);
     DEBUG.print("% send to Thingspeak takes: ");
     DEBUG.println(millis()-timetosend);    
  }
  client.stop();
  
}

void SendThingspeakAll(){

  float field1 = mOutputAsFloat(TEMPERATURE);
  float field2 = mOutputAsFloat(HUMIDITY);
  float field3 = mOutputAsFloat(DALLAS);
  float field4 = mOutputAsFloat(LDR);
  float field5 = mOutputAsFloat(PRESSURE0);
  float field6 = mOutputAsFloat(BMP180TEMP);
  float timetosend = millis();
  
  if (client.connect(serverTS,80)) {  //   "184.106.153.149" or api.thingspeak.com
    String postStr = apiKey;
        if(S2 == DHT_SENSOR){
           postStr +="&field1=";
           postStr += String(field1);
           postStr +="&field2=";
           postStr += String(field2);
           DEBUG.print("Temperature: ");  DEBUG.print(field1); DEBUG.print("C | ");
           DEBUG.print("Humidity: ");     DEBUG.print(field2); DEBUG.print("% | ");
        }
        if(S1 == DALLAS_SENSOR){
           postStr +="&field3=";
           postStr += String(field3);
           DEBUG.print("Dallas: ");       DEBUG.print(field3); DEBUG.print("C | "); 
        }
        if(S3 == LDR_SENSOR){
           postStr +="&field4=";
           postStr += String(field4);
           DEBUG.print("LDR: ");          DEBUG.print(field4); DEBUG.print("lux | ");
        } 
        if(S51 == BMP180){
           postStr +="&field5=";
           postStr += String(field5);
           postStr +="&field6=";
           postStr += String(field6);
           DEBUG.print("Pressure: ");     DEBUG.print(field5); DEBUG.print("mb | ");
           DEBUG.print("BMP180TEMP: ");   DEBUG.print(field6); DEBUG.print("C ");
        }                 
           postStr += "\r\n\r\n";
 
     client.print("POST /update HTTP/1.1\n"); 
     client.print("Host: api.thingspeak.com\n"); 
     client.print("Connection: close\n"); 
     client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n"); 
     client.print("Content-Type: application/x-www-form-urlencoded\n"); 
     client.print("Content-Length: "); 
     client.print(postStr.length()); 
     client.print("\n\n"); 
     client.print(postStr);
     
     DEBUG.print("-> send to Thingspeak takes: ");
     DEBUG.println(millis()-timetosend); 
  }
  client.stop();
  
}


// ******************************************************************************************************************
// ***********************************************  SLOTS CONFIG ****************************************************
// ******************************************************************************************************************
void SLOT_CONFIG(){
  int NEXTSLOT = 0;
  DEBUG.println("SLOT CONFIG"); 

// --------------------------------- ALARM SLOTS --------------------------------
  if(ALARM_MODE == 1 || S41 == ALARM_ENDSTOP || S42 == ALARM_ENDSTOP || S51 == ALARM_ENDSTOP || S52 == ALARM_ENDSTOP){
      ALARM = NEXTSLOT;
      NEXTSLOT = ALARM + 1;
      DEBUG.print("ALARM: "); DEBUG.println(ALARM);
  }

// --------------------------------- S2 SLOTS -----------------------------------  
  switch (S2) {  
        case 0:
            //NONE
            break;
        case 1:
            TEMPERATURE = NEXTSLOT;
            HUMIDITY = NEXTSLOT + 2;
            NEXTSLOT = HUMIDITY + 2;
            DEBUG.print("TEMP: "); DEBUG.println(TEMPERATURE);  
            DEBUG.print("HUMI: "); DEBUG.println(HUMIDITY);  
            break;
  }


// --------------------------------- L1 SLOTS -----------------------------------        
  if(L1 == ONOFF_MODE || L1 == PIR_MODE || L1 == PULSE_MODE){
        LED1 = NEXTSLOT;
        NEXTSLOT = LED1 + 1;
        DEBUG.print("LED1: "); DEBUG.println(LED1);
  }else if(L1 == PWM_MODE){
        LED1 = NEXTSLOT;
        NEXTSLOT = LED1 + 2;
        DEBUG.print("LED1: PWM "); DEBUG.println(LED1);         
  }

// --------------------------------- L2 SLOTS -----------------------------------       

  if(L2 == ONOFF_MODE || L2 == PIR_MODE || L2 == PULSE_MODE){
        LED2 = NEXTSLOT;
        NEXTSLOT = LED2 + 1;
        DEBUG.print("LED2: "); DEBUG.println(LED2);
  }else if(L2 == PWM_MODE){
        LED2 = NEXTSLOT;
        NEXTSLOT = LED2 + 2;
        DEBUG.print("LED2: PWM "); DEBUG.println(LED2);          
  }
 
// --------------------------------- L3 SLOTS -----------------------------------         
  if(L3 == ONOFF_MODE || L3 == PIR_MODE || L3 == PULSE_MODE){
        LED3 = NEXTSLOT;
        NEXTSLOT = LED3 + 1;
        DEBUG.print("LED3: "); DEBUG.println(LED3);
  }else if(L3 == PWM_MODE){
        LED3 = NEXTSLOT;
        NEXTSLOT = LED3 + 2;
        DEBUG.print("LED3: PWM "); DEBUG.println(LED3);         
  }

// -------------------------------- RGB SLOT -----------------------------------        
  if(L1 == RGB_MODE && L2 == RGB_MODE && L3 == RGB_MODE){
        LEDRGB = NEXTSLOT;
        NEXTSLOT = LEDRGB + 4;
        DEBUG.print("LEDRGB: "); DEBUG.println(LEDRGB);  
  }

// --------------------------------- S3 SLOTS -----------------------------------        
  switch (S3) {  
        case 0:
            //NONE
            break;
        case 1:
            LDR = NEXTSLOT;
            NEXTSLOT = LDR + 2;
            DEBUG.print("LDR: "); DEBUG.println(LDR);  
            break;
  }

// --------------------------------- S1 SLOTS -----------------------------------        
  switch (S1) {  
        case 0:
            //NONE
            break;
        case 1:
            DALLAS = NEXTSLOT;
            NEXTSLOT = DALLAS + (2 * dallas_qty);
            DEBUG.print("DALLAS: "); DEBUG.println(DALLAS);  
            break;
  }
 

// --------------------------- CAPACITIVE DEBUG SLOTS ---------------------------        
  if(cap_debug){
      if(S41 == CAPACITIVE){
        CAP0 = NEXTSLOT;
        NEXTSLOT = CAP0 + 2;
        DEBUG.print("CAP0: "); DEBUG.println(CAP0); 
      }
      if(S42 == CAPACITIVE){
        CAP1 = NEXTSLOT;
        NEXTSLOT = CAP1 + 2;
        DEBUG.print("CAP1: "); DEBUG.println(CAP1); 
      }
      if(S51 == CAPACITIVE){
        CAP2 = NEXTSLOT;
        NEXTSLOT = CAP2 + 2;
        DEBUG.print("CAP2: "); DEBUG.println(CAP2); 
      }
      if(S52 == CAPACITIVE){
        CAP3 = NEXTSLOT;
        NEXTSLOT = CAP3 + 2;
        DEBUG.print("CAP3: "); DEBUG.println(CAP3); 
      } 
      if(S41 == CAPACITIVE || S42 == CAPACITIVE || S51 == CAPACITIVE || S52 == CAPACITIVE){
        THRE = NEXTSLOT;
        NEXTSLOT = THRE + 2;           
        DEBUG.print("THRE: "); DEBUG.println(THRE);
      }  
  }


// ------------------------- RELAY-PULSE-TRIAC SLOTS -----------------------------        
  if(S51 == RELAY || S51 == INVRELAY){
    RELAY0 = NEXTSLOT;
    NEXTSLOT = RELAY0 + 1;
    DEBUG.print("RELAY0: "); DEBUG.println(RELAY0);
  }
  if(S52 == RELAY || S51 == INVRELAY){
    RELAY1 = NEXTSLOT;
    NEXTSLOT = RELAY1 + 1;
    DEBUG.print("RELAY1: "); DEBUG.println(RELAY1);
  }
  if(S51 == PULSE_OUTPUT){
    PULSE0 = NEXTSLOT;
    NEXTSLOT = PULSE0 + 1;
    DEBUG.print("PULSE0: ");  DEBUG.println(PULSE0);
  }
  if(S52 == PULSE_OUTPUT){
    PULSE1 = NEXTSLOT;
    NEXTSLOT = PULSE1 + 1;
    DEBUG.print("PULSE1: ");  DEBUG.println(PULSE1);  
  }
  if(S51 == TRIAC){
    RELAY0 = NEXTSLOT;
    NEXTSLOT = RELAY0 + 2;
    DEBUG.print("TRIAC0: ");  DEBUG.println(RELAY0);
  }
  if(S52 == TRIAC){
    RELAY1 = NEXTSLOT;
    NEXTSLOT = RELAY1 + 2;
    DEBUG.print("TRIAC1: ");  DEBUG.println(RELAY1);  
  }

// --------------------------------- S52 SLOTS -----------------------------------        
  switch (S52) {       
        case BMP180:
            PRESSURE0 = NEXTSLOT;  
            BMP180TEMP = NEXTSLOT + 2;
            NEXTSLOT = BMP180TEMP + 2;
            DEBUG.print("PRESSURE0: ");  DEBUG.println(PRESSURE0);   
            DEBUG.print("BMP180TEMP: "); DEBUG.println(BMP180TEMP);                   
            break;                        
        case GARAGE_DOOR:
            T2X = NEXTSLOT;
            NEXTSLOT = T2X + 1;
            DEBUG.print("GARAGE: ");  DEBUG.println(T2X);                    
            break; 
        case WINDOW_CURTAIN:
            T2X = NEXTSLOT;
            NEXTSLOT = T2X + 1;
            DEBUG.print("CURTAIN: ");  DEBUG.println(T2X);                    
            break;  
        case OPTO_AND_RELAY:
            OPTO = NEXTSLOT;
            NEXTSLOT = OPTO + 1;
            DEBUG.print("OPTO: ");  DEBUG.println(OPTO);                  
            break;                       
  }

// ************************************** END OF SLOT_CONFIG() **************************
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

static unsigned long souliss_upTime;
static unsigned long souliss_time;
static unsigned long souliss_lastTime;
static unsigned long souliss_lastUpTime;

 

// ******************************************************************************************************************
// *************************************       DigIn3Action FUNCTION          ***************************************
// ******************************************************************************************************************
/*!
  Link an hardware pin to the shared memory map, active on rising edge
        or on long press hold.
  Identify three actions: press, hold and double press. Each action can
        perform an command for separate slot.
*/
/**************************************************************************/
U8 Souliss_DigIn3Action(U8 pin, U8 value_single, U8 value_long, U8 value_double, U8 *memory_map, U8 slot_single, U8 slot_long, U8 slot_double, U16 holdTime=1000, U16 doublePressTime=500)
{   
    int reading = dRead(pin);
    
    // measure time on button DOWN
    if(reading && (InPin[pin]==PINRESET))
    {
        InPin[pin] = PINSET;
        souliss_time = millis();
    souliss_lastTime = souliss_time;
        
        return MaCaco_NODATACHANGED;
    }
    // measure time on button UP
    else if (!reading && InPin[pin]==PINSET)
    {
        souliss_lastUpTime = souliss_upTime;
        souliss_upTime = millis();
    } 
    // reset time - third press will be single click again
    else if (!reading && InPin[pin]==PINRELEASED)
    {
        InPin[pin] = PINACTIVE;
        souliss_time = souliss_upTime = 0;
    } 
    // reset pin
    else if (!reading && InPin[pin]==PINACTIVE) 
    {
        InPin[pin] = PINRESET;
    }
    
    // HANDLING of BUTTON ACTIONS
    // single or double click
    if (!reading && InPin[pin]==PINSET) 
    {
        // double click
        if (abs(souliss_time - souliss_lastTime) < doublePressTime && 
            abs(souliss_upTime - souliss_lastUpTime) < doublePressTime)
        {
            InPin[pin] = PINRELEASED;
            if(memory_map)  memory_map[MaCaco_IN_s + slot_double] = value_double;
            return value_double;

        // single click
        } else {
            InPin[pin] = PINACTIVE;
            if(memory_map)  memory_map[MaCaco_IN_s + slot_single] = value_single;
            return value_single;
        }
    }
    // long click
    else if (reading && InPin[pin]==PINSET){
        if (abs(millis() - souliss_time) > holdTime) 
        {
            InPin[pin] = PINACTIVE;
            if(memory_map)  memory_map[MaCaco_IN_s + slot_long] = value_long;
            return value_long;
        }
    }
    
    return MaCaco_NODATACHANGED;
}


#endif

 
