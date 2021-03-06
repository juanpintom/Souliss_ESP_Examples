/*  Download WifiManager from https://github.com/tzapu/WiFiManager  */
/*
  With this Example you don't need to configure SSID and Password before compile and load the sketch. 
  If your node doesn't connect to your WiFi or isn't configured yet you will see an open WiFi network called "Souliss", 
  connect to the wifi and open on your browser this address: http://192.168.4.1  and configure your WiFi, 
  then you can open your Souliss Android App and enjoy! :)
    *(Chrome on Android not supported)
*/
#include <WiFiClient.h>
#include <EEPROM.h>
#include <ESP8266mDNS.h>
#include <WiFiManager.h>

WiFiManager wifi(0);
#define DEBUG 
/**************************************************************************
    Souliss - Hello World for Expressif ESP8266
    
    This is the basic example, create a software push-button on Android
    using SoulissApp (get it from Play Store).  
    
    Load this code on ESP8266 board using the porting of the Arduino core
    for this platform.
        
***************************************************************************/
#define MaCaco_DEBUG_INSKETCH
#define MaCaco_DEBUG  		1

#define VNET_DEBUG_INSKETCH
#define VNET_DEBUG  		1

// Configure the framework
#include "bconf/MCU_ESP8266.h"              // Load the code directly on the ESP8266
#include "conf/Gateway.h"                   // The main node is the Gateway, we have just one node
#include "conf/IPBroadcast.h"

// **** Define the WiFi name and password ****
//#define WIFICONF_INSKETCH
//#define WiFi_SSID               "mywifi"
//#define WiFi_Password           "mypassword"    

// Include framework code and libraries
#include <ESP8266WiFi.h>
//#include <EEPROM.h>
#include "Souliss.h"

// This identify the number of the LED logic
#define MYLEDLOGIC          0               

// **** Define here the right pin for your ESP module **** 
#define	OUTPUTPIN			5

void setup()
{   
    Serial.begin(115200);
    wifi.autoConnect("Souliss");
    WiFi.mode(WIFI_STA);
    Initialize();

    // Connect to the WiFi network and get an address from DHCP
    GetIPAddress();                           
    SetAsGateway(myvNet_dhcp);       // Set this node as gateway for SoulissApp  

    // This is the vNet address for this node, used to communicate with other
	// nodes in your Souliss network
    SetAddress(0xAB01, 0xFF00, 0x0000);
    SetAsPeerNode(0xAB02, 1);
	
    Set_SimpleLight(MYLEDLOGIC);        // Define a simple LED light logic
	
    pinMode(OUTPUTPIN, OUTPUT);         // Use pin as output 
}

void loop()
{ 
    // Here we start to play
    EXECUTEFAST() {                     
        UPDATEFAST();   
        
        FAST_50ms() {   // We process the logic and relevant input and output every 50 milliseconds
            Logic_SimpleLight(MYLEDLOGIC);
            DigOut(OUTPUTPIN, Souliss_T1n_Coil,MYLEDLOGIC);
        } 
              
        // Here we handle here the communication with Android
        FAST_GatewayComms();                                        
    }
} 
