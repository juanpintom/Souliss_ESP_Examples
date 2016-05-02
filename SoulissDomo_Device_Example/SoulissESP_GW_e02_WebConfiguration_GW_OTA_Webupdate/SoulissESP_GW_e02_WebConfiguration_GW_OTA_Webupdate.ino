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
   
#define MYLED               0
uint8_t GPIO_POUT = 15;  //GPIO15

#define LOG Serial

//OTA_Setup();
ESP8266HTTPUpdateServer httpUpdater;

void setup()
{
    Serial.begin(115200);    
    Initialize();
    if(!digitalRead(13)){
        //EEPROM.begin(512);
        // write a 0 to all 512 bytes of the EEPROM
        for (int i = STORE__PADDR_s; i < STORE__PADDR_f; i++)
          EEPROM.write(i, 0);

        // turn the LED on when we're done
        pinMode(15, OUTPUT);
        digitalWrite(15, HIGH);
        Serial.println("NODES DELETED");
        EEPROM.commit();
        //EEPROM.end();
        delay(3000);
    } 
    
    if(!digitalRead(12)){
        //EEPROM.begin(512);
        // write a 0 to all 512 bytes of the EEPROM
        for (int i = 0; i < 512; i++)
          EEPROM.write(i, 0);

        // turn the LED on when we're done
        pinMode(15, OUTPUT);
        digitalWrite(15, HIGH);
        Serial.println("EEPROM DELETED");
        EEPROM.commit();
        //EEPROM.end();
        delay(3000);
    } 
    
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

    if (IsRuntimeGateway())
    {
        // Connect to the WiFi network and get an address from DHCP      
        Serial.println("GW MODE");        
        SetAsGateway(myvNet_dhcp);       // Set this node as gateway for SoulissApp  
        SetAddressingServer();
        //SetAddress(0xAB00, 0xFF00, 0xAB01);
        RoutingTable(0xAB00, myvNet_dhcp, 0);
        SetAsPeerNode(0x00C7, 1);	  // NODO 644 HABITACION OCIO IP199
        SetAsPeerNode(0x00C9, 2);	  // NODO ENTRADA IP201
        SetAsPeerNode(0x00CA, 3);    // NODO DORMIR IP202
        SetAsPeerNode(0x00CB, 4);    // NODO SALON IP203
        SetAsPeerNode(0x00CC, 5);    // NODO ESCALERA IP204
        
        
        //SetAsPeerNode(0x00CA, 3);
//        SetAsPeerNode(0x00BD, 4);
        
    }
    else 
    {
        // This board request an address to the gateway at runtime, no need
        // to configure any parameter here.
        Serial.println("PEER MODE");
        SetDynamicAddressing();  
        //SetAddress(0xAB05, 0xFF00, 0xAB01);
        //SetAddress(0x00BD, 0xFF00, 0x00C8);
        GetAddress();
    }
    
    Set_DimmableLight(MYLED);
    startWebServer();
    
    LOG.print("vNet Media : ");
    LOG.println (vNet_GetAddress(vNet_MyMedia()),HEX);
    LOG.print("vNet 1 : ");
    LOG.println (vNet_GetAddress(1),HEX);
    LOG.print("vNet 2 : ");
    LOG.println (vNet_GetAddress(2),HEX);
    LOG.print("vNet 3 : ");
    LOG.println (vNet_GetAddress(3),HEX);
    LOG.print("vNet 4 : ");
    LOG.println (vNet_GetAddress(4),HEX);
    LOG.print("vNet 5 : ");
    LOG.println (vNet_GetAddress(5),HEX);
    
    //OTA_Init(); 
    httpUpdater.setup(&server);
    MDNS.addService("http", "tcp", 80);
}

void loop()
{  
    runWebServer();
    yield();
    EXECUTEFAST() {                     
        UPDATEFAST();   
        
        FAST_50ms() {   // We process the logic and relevant input and output every 50 milliseconds
             Logic_DimmableLight(MYLED);                        
             analogWrite(GPIO_POUT, mOutput(MYLED+1)*4);
        }
      
        // Run communication as Gateway or Peer
        if (IsRuntimeGateway())
            FAST_GatewayComms(); 
        else 
            FAST_PeerComms();   
    }

    EXECUTESLOW() {
        UPDATESLOW();

        SLOW_10s() {  // Read temperature and humidity from DHT every 110 seconds  
                Timer_DimmableLight(MYLED);   
        }
        
        // If running as Peer
        if (!IsRuntimeGateway())
            SLOW_PeerJoin();
    } 
    //OTA_Process();
}    
