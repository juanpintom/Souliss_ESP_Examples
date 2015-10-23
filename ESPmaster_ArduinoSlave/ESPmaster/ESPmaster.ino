/****************************************************
 *   Sketch by Jose Manuel Loureiro and Juan Pinto
              ESP Self Wifi & Gateway, no router
              & Arduino Slave

*****************************************************/
//---------------------------------------------------
// 1.     Configure the framework
//---------------------------------------------------
#include "bconf/MCU_ESP8266.h"              
#include "conf/Gateway.h"
#include "conf/IPBroadcast.h"


#define XSSID "Radio"
#define PAS  "AAAAAAAA"  
//---------------------------------------------------
// 2.     Include framework code and libraries
//---------------------------------------------------
#include "Souliss.h"
#include <ESP8266WiFi.h>
#include <EEPROM.h>
#include <WiFiUdp.h>
//----------------------------------------------------
// 4. Slots
//----------------------------------------------------
#define SLOT0             0
#define SLOT1             1
#define SLOT2             2
#define SLOT3             3
#define SLOT4             4//&5
//----------------------------------------------------
// 6. Comandos
//----------------------------------------------------
bool lastSLOT[4];
bool lastSLOT0;
bool lastSLOT1;
bool lastSLOT2;
bool lastSLOT3;

// **************************** DEFINES ***********************
/* CHULETA DE COMANDOS
Esp send:                  int[0]  int[1]  int[2]
                    
digital write         1     pin       value
digital read          2     pin       0(else)
analog write          3     pin       value
analog read           4     pin       0(else)
store variable        5     nVar      value
get  variable         6     nvar      0(else)
configure pin         7     pin       confVal
send eeprom values    8     0         0(else)
clear pinConf         8     1         0(else)
reset arduino         9     0(else)   0(esle) */

#define digital_write       1
#define digital_read        2  //x.0
#define analog_write        3
#define analog_read         4  //x.0
#define store_variable      5
#define get_variable        6  //x.0
#define configure_pin       7
#define send_eeprom_values  8.0.0
#define clear_pinConf       8.1.0
#define reset_arduino       9.0.0

bool Arduino(byte command, byte variable = 255, byte option = 255){
  if(command == send_eeprom_values || command == clear_pinConf || command == reset_arduino) {
    Serial.println(command);
    return 1;
  }
    
  if(command == digital_write || command == analog_write || command == store_variable || command == configure_pin){
    if(variable != 255 && option != 255){
      Serial.print(command);Serial.print(".");
      Serial.print(variable);Serial.print(".");
      Serial.print(option);Serial.print(".");
      Serial.println("");
      return 1;
    }
  }
  if(command == digital_read || command == analog_read || command == get_variable){
    if(variable != 255)
      Serial.print(command);Serial.print(".");
      Serial.print(variable);Serial.print(".");
      Serial.print("0);Serial.print(".");
      Serial.println("");
      return 1;
    }
  }
  return 0;
}

byte pinConf[14];//0,1 serial(not used)
                //D2-D13
String inputString= "";   // a string to hold incoming data
int inputInt[3];
bool stringComplete = false; // whether the string is complete
byte indeX=0;
///* if no needed
int nVar[10];
//*/
bool dr[14];
float ar[8];

//=======================================================
void setup()
{
    Initialize();
    SetAPoint();//**************SetAccessPoint*********//
    SetAsGateway(myvNet_dhcp); 
  //----------------------------------------------------
  //S2. Set vNet Address    
  //----------------------------------------------------
    SetAddress(0xAB01, 0xFF00, 0x0000);
  //----------------------------------------------------
  // S4. Set the typicals for this node
  //----------------------------------------------------
    Set_SimpleLight(SLOT0); 
    Set_SimpleLight(SLOT1); 
    Set_SimpleLight(SLOT2);  
    Set_SimpleLight(SLOT3); 
    Set_T51(SLOT4);

   Serial.begin(9600);

   delay(3000);   //wait arduino on
   Serial.println("8.0.0.");//clear arduino pinConf
                            //?ever needed
   Serial.println("7.14.5.");//conf.A0.analog in
   Serial.println("7.3.3."); //conf.pin5.digOut start high
   Serial.println("7.4.3."); //pin4
   Serial.println("7.5.3.");
   Serial.println("7.6.3.");
   Serial.println("9.0.0."); //reset arduino if needed
   delay(4000); //wait arduino after reset
}

//=======================================================
void loop()
{  
  serialEvent();
  EXECUTEFAST() {                     
    UPDATEFAST();   
  
      FAST_50ms() {   
        Logic_SimpleLight(SLOT0);
          if(mOutput(SLOT0)!=lastSLOT0){
            Serial.print("1.6.");
            Serial.print(!mOutput(SLOT0));//! Relay(=lowDigOut)
            Serial.println(".");
            lastSLOT0=mOutput(SLOT0);}
        Logic_SimpleLight(SLOT1);        
          if(mOutput(SLOT1)!=lastSLOT1){
            Serial.print("1.5.");
            Serial.print(!mOutput(SLOT1));
            Serial.println(".");
            lastSLOT1=mOutput(SLOT1);}
        Logic_SimpleLight(SLOT2); 
          if(mOutput(SLOT2)!=lastSLOT2){
            Serial.print("1.4.");
            Serial.print(!mOutput(SLOT2));
            Serial.println(".");
            lastSLOT2=mOutput(SLOT2);}
        Logic_SimpleLight(SLOT3);
          if(mOutput(SLOT3)!=lastSLOT3){
            Serial.print("1.3.");
            Serial.print(!mOutput(SLOT3));
            Serial.println(".");
            lastSLOT3=mOutput(SLOT3);}
        }

      FAST_50ms() {
        Read_T51(SLOT4);
        Serial.print("4.14.0.");
        ImportAnalog(SLOT4,&ar[0]);
        }
      FAST_GatewayComms(); 
    }
    EXECUTESLOW() {
        UPDATESLOW();
    } 
  if (stringComplete) {
    switch(inputInt[0]){
      case 2:             //digital read
        if(1<inputInt[1]<14){
          dr[inputInt[1]]=inputInt[2];}
      break;
      case 4:             //analog read
        if(13<inputInt[1]<22){
          ar[inputInt[1]-14]=inputInt[2];}
      break;
///* if no need
      case 6:           //get nVar
        nVar[inputInt[1]]=inputInt[2];    
      break;
//*/
      case 8:
        if (1<inputInt[1]<14){//read arduino pinConf
          pinConf[inputInt[1]]=inputInt[2];    
        }
      break;
      default:
      break;
    }//switch
    inputInt[0]=0;
    inputInt[1]=0;
    inputInt[2]=0;
    stringComplete = false;
  }//if stringcomplete
}  
//=======================================================
void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    if (inChar != '.') {
      inputString += inChar;
      }
    if (inChar == '.') {
      inputInt[indeX]=inputString.toInt();
      inputString = "";
      indeX++;
      }
    if (indeX == 3) {
      indeX=0;
      stringComplete = true;
    }
  }//while
}
/**************************************************************************
/*!
  Start the node as an access point 
  
  from SSLibrary/Base/NetworkSetup.ccp lin250+
*/  
/**************************************************************************/ 
void SetAPoint()
{
  uint8_t i;
  uint8_t ipaddr[4];
  uint8_t gateway[4];   
  
  WiFi.mode(WIFI_AP);
  WiFi.softAP(XSSID,PASS);

  IPAddress lIP  = WiFi.softAPIP();
  
  for(i=0;i<4;i++)
  {
    ipaddr[i]  = lIP[i];
    gateway[i] = lIP[i];
  } 

  myvNet_dhcp = ipaddr[3];
  
  for(i=0; i<4; i++)
  {
    if(DEFAULT_BASEIPADDRESS)   DEFAULT_BASEIPADDRESS[i]=ipaddr[i];
    if(DEFAULT_GATEWAY)     DEFAULT_GATEWAY[i] = gateway[i];
  }
  
  U16 vNet_address = (U16)ipaddr[i-1];      
  DEFAULT_BASEIPADDRESS[i-1]=0;     
  Souliss_SetAddress(vNet_address, DYNAMICADDR_SUBNETMASK, 0);  
}
