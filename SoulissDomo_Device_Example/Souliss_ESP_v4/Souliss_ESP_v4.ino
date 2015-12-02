/**************************************************************************
   Souliss - Web Configuration
    
    This example demonstrate a complete web configuration of ESP8266 based
	nodes, the node starts as access point and allow though a web interface
	the configuration of IP and Souliss parameters.

	This example is only supported on ESP8266.
 
***************************************************************************/
#define MaCaco_DEBUG_INSKETCH
  #define MaCaco_DEBUG   1

#define VNET_DEBUG_INSKETCH
  #define VNET_DEBUG    1

#define SOULISS_DEBUG_INSKETCH
  #define SOULISS_DEBUG   1

#include "DHT.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include <SFE_BMP180.h>
#include <Wire.h>

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPUpdateServer.h>
#include <EEPROM.h>
#include <WiFiUdp.h>

// Configure the Souliss framework
#include "bconf/MCU_ESP8266.h"              // Load the code directly on the ESP8266
#include "conf/RuntimeGateway.h"            // This node is a Peer and can became a Gateway at runtime
#include "conf/DynamicAddressing.h"         // Use dynamically assigned addresses
#include "conf/WEBCONFinterface.h"          // Enable the WebConfig interface

#include "Souliss.h"
#include "Functions.h"
#include "SetupAndLoop.h"

#include "Page_General.h"

OTA_Setup();  
ESP8266HTTPUpdateServer httpUpdater;

void setup()
{
    Serial.begin(115200);
    Initialize();
    EEPROM.begin(512);
    
    server.on ( "/", send_general_html  );
    server.on ( "/general.html", send_general_html  );
    server.on ( "/admin/generalvalues", send_general_configuration_values_html);
    
    // Read the IP configuration from the EEPROM, if not available start
    // the node as access point
    if(!ReadIPConfiguration()) 
	{	
		// Start the node as access point with a configuration WebServer
		SetAccessPoint();
		startWebServer();

		// We have nothing more than the WebServer for the configuration
		// to run, once configured the node will quit this.
		while(1)
		{
			yield();
			runWebServer(); 
		}
  
	}
    ReadConfig_Slots();
    if(DEBUG_PRESSURE){
      LOG.print("ALTITUD: ");
      LOG.println(ALTITUDE);
    }
    
    
    Serial.print("EEPROM GW: ");
    Serial.println(EEPROM.read(STORE__GATEWAY_s));
    
    if (IsRuntimeGateway())
    {
        // Connect to the WiFi network and get an address from DHCP     
        Serial.println("Gateway Mode");        
        SetAsGateway(myvNet_dhcp);       // Set this node as gateway for SoulissApp  
        SetAddressingServer();
        
    }
    else 
    {
        // This board request an address to the gateway at runtime, no need
        // to configure any parameter here.
        Serial.println("Peer Mode");
        SetDynamicAddressing();  
        GetAddress();
        
    }
    setupGeneral();
    startWebServer();
    LOG.print("vNet Media : ");
    LOG.println (vNet_GetAddress(vNet_MyMedia()),HEX);
    LOG.print("vNet 1 : ");
    LOG.print(vNet_GetAddress(1),HEX);
    LOG.print(" | IP: ");
    LOG.println(vNet_GetAddress(1),DEC);
    LOG.print("vNet 2 : ");
    LOG.println (vNet_GetAddress(2),HEX);
    LOG.print("vNet 3 : ");
    LOG.println (vNet_GetAddress(3),HEX);
    LOG.print("vNet 4 : ");
    LOG.println (vNet_GetAddress(4),HEX);
    LOG.print("vNet 5 : ");
    LOG.println (vNet_GetAddress(5),HEX);

    OTA_Init(); 
    
    httpUpdater.setup(&server);
    MDNS.addService("http", "tcp", 80);
}

void loop()
{  
    runWebServer();
    
    EXECUTEFAST() {                     
        UPDATEFAST();   
        
        fastGeneral();
         
        // Run communication as Gateway or Peer
        if (IsRuntimeGateway())
            FAST_GatewayComms(); 
        else 
            FAST_PeerComms();   
    }

    EXECUTESLOW() {
        UPDATESLOW();

        slowGeneral();
        
        // If running as Peer
        if (!IsRuntimeGateway())
            SLOW_PeerJoin();
    } 
    OTA_Process();
}    



