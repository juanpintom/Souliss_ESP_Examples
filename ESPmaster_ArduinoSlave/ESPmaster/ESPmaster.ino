/****************************************************
 *   Sketch by Jose Manuel Loureiro 
              ESP Self Wifi & Gateway, no router


            & Arduino Slave


*****************************************************/




//---------------------------------------------------
// 1.     Configure the framework
//---------------------------------------------------
#include "bconf/MCU_ESP8266.h"              
#include "conf/Gateway.h"
#include "conf/IPBroadcast.h"


#define SSID "Radio"
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
bool lastSLOT0;
bool lastSLOT1;
bool lastSLOT2;
bool lastSLOT3;


byte pinConf[22];//0,1 serial(not used)
                //D2-D13
                //A0[14]-A7[21]
String inputString= "";   // a string to hold incoming data
int inputInt[3];
bool stringComplete = false; // whether the string is complete
byte indeX=0;


int nVar[10];
bool dr[15];
int ar[8];
float floaT;


//=======================================================
void setup()
{
    Initialize();
    SetAPoint();//*********************SetAccessPoint**********//
    SetAsGateway(myvNet_dhcp); 
  //----------------------------------------------------
  //S2. Set vNet Address    
  //----------------------------------------------------
    SetAddress(0xAB01, 0xFF00, 0x0000);
  //----------------------------------------------------
  // S4. Set the typicals for this node
  //----------------------------------------------------
    Set_SimpleLight(SLOT0);        // Define a simple LED light logic
    Set_SimpleLight(SLOT1);        // Define a simple LED light logic
    Set_SimpleLight(SLOT2);        // Define a simple LED light logic
    Set_SimpleLight(SLOT3);        // Define a simple LED light logic
    Set_T51(SLOT4);


   Serial.begin(9600);


   delay(3000);   //wait arduino on
   Serial.println("8.0.0.");//clear arduino pinConf
   Serial.println("7.14.5.");//conf.A0.analog in
   Serial.println("7.3.3.");//configure.pin5.digOut start high
   Serial.println("7.4.3.");//pin4
   Serial.println("7.5.3.");
   Serial.println("7.6.3.");
   Serial.println("9.0.0.");//reset arduino if needed
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
            Serial.print(!mOutput(SLOT0));//! pq son reles invertidos(=lowDigOut)
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
        floaT=ar[0];
        ImportAnalog(SLOT4,&floaT);
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
      case 5:           //store nVar
      break;
      case 6:           //get nVar
        nVar[inputInt[1]]=inputInt[2];    
      break;
//*/
      case 7://configure pin
        if((1<inputInt[1]<22)&&(0<inputInt[2]<6)&&(pinConf[inputInt[1]]!=inputInt[2])){
          pinConf[inputInt[1]]=inputInt[2];}
      break;
      case 8:
        if (1<inputInt[1]<22){//read arduino
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


WiFi.softAP(_apname,_apname); //WiFi.softAP("ssid","password");
*/  
/**************************************************************************/ 
void SetAPoint()
{
  uint8_t i;
  uint8_t ipaddr[4];
  uint8_t gateway[4];   
  
/*  // Use a static access point name with a dynamic number
  char _time[9] = __TIME__;
  char _apname[18]= "Souliss_000000000";
  
  // Build the access point name
  for(i=0;i<8;i++)
    _apname[i+9] = _time[i];
*/  
  WiFi.mode(WIFI_AP);
//  WiFi.softAP(_apname,_apname);////********************************/////
  WiFi.softAP(SSID,PASS);


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
