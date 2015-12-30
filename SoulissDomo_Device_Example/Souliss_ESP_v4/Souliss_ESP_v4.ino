/**************************************************************************
   Souliss - Web Configuration
    
    This example demonstrate a complete web configuration of ESP8266 based
	nodes, the node starts as access point and allow though a web interface
	the configuration of IP and Souliss parameters.

	This example is only supported on ESP8266.
 
***************************************************************************/

<<<<<<< HEAD
#define SERIALPORT_INSKETCH
  #define LOG Serial
 
=======
// ************************* IR LIBRARY ***********************************
// To use the IR Functions you need to add this library to your Arduino libraries:
// https://github.com/markszabo/IRremoteESP8266/

>>>>>>> origin/master
#include <IRremoteESP8266.h>

// ************************* SOULISS DEBUG LINES  ***********************************
// Enable or disable Souliss debug
#define MaCaco_DEBUG_INSKETCH
  #define MaCaco_DEBUG   1

#define VNET_DEBUG_INSKETCH
  #define VNET_DEBUG    1

#define SOULISS_DEBUG_INSKETCH
  #define SOULISS_DEBUG   1

/*#define USART_LOG LOG.print
#define USART_DEBUG_INSKETCH
  #define USART_DEBUG     1

#define USARTBAUDRATE_INSKETCH
# define USART_BAUD9k6        1
# define USART_BAUD19k2       0
# define USART_BAUD57k6       0
# define USART_BAUD115k2      0
# define USART_BAUD256k       0  */
// *************

// *************************** SENSORS LIBRARIES *************************

#include "DHT.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include <SFE_BMP180.h>
#include <Wire.h>

// ***************************  ESP  LIBRARIES ***************************

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPUpdateServer.h>
#include <EEPROM.h>
#include <WiFiUdp.h>

<<<<<<< HEAD


=======
// ***************************  SOULISS  LIBRARIES ***************************
>>>>>>> origin/master
// Configure the Souliss framework
#include "bconf/MCU_ESP8266.h"              // Load the code directly on the ESP8266
//#include "conf/SuperNode.h"
//#include "conf/usart.h"                     // USART 
#include "conf/RuntimeGateway.h"            // This node is a Peer and can became a Gateway at runtime
#include "conf/DynamicAddressing.h"         // Use dynamically assigned addresses
#include "conf/WEBCONFinterface.h"          // Enable the WebConfig interface

#include "Souliss.h"
#include "Functions.h"
#include "irReceiver.h"
#include "irButtons.h"
#include "SetupAndLoop.h"

#include "Page_General.h"

//OTA_Setup();  
ESP8266HTTPUpdateServer httpUpdater;

<<<<<<< HEAD
#include "Adafruit_IO_Client.h"
//#define AIO_KEY    "...your Adafruit IO key value ..."
#define AIO_KEY    "65b1e1717e6d08e3f58768c1de502d5363d6a64e"
Adafruit_IO_Client aio = Adafruit_IO_Client(client, AIO_KEY);
Adafruit_IO_Feed testFeed = aio.getFeed("esptestfeed");
unsigned int count = 0;
=======
>>>>>>> origin/master

void setup()
{
    LOG.begin(115200);
    if(IR_ENABLE){
    	irrecv.enableIRIn();  // Start the receiver
    }
    
    Initialize();
// **** FUNCTION TO DELETE JUST ADDRESSES (MORE THAN 5sec) or ALL THE EEPROM DATA (MORE THAN 10sec) *** 
//  STILL DISABLED, TESTING
    EEPROM.begin(512);
    LOG.println("");
    LOG.println("Time to Reset");
    delay(1000);
    long previous = millis();
    pinMode(0, INPUT);
    if(!digitalRead(0)) LOG.println("GPIO0 PRESSED!");
    while(!digitalRead(0)){
      if(millis() < previous + 5000){
        LOG.print("Deleting Addresses in: ");
        LOG.println(5000 - (millis() - previous));
        delay(500);
      }else{
        for(int i = STORE__ADDR_s; i <= STORE__PADDR_f; i++){
          //EEPROM.write(i,0);
        }
        EEPROM.commit();
        LOG.println("Address Deleted");
        // DELETE EEPROM IF GPIO STILL PRESSED
        if(millis() < previous + 10000){
          LOG.print("Deleting EEPROM in: ");
          LOG.println(10000 - (millis() - previous));
          delay(500);
        }else{
          for(int i = 0; i <= 512; i++){
            EEPROM.write(i,255);
        }
        EEPROM.commit();
        LOG.println("EEPROM Deleted");
        }
      }
    }
    
    //SetAddress(0xAB04,0x00FF,0xAB01);
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
    
    
    LOG.print("EEPROM GW: ");
    LOG.println(EEPROM.read(STORE__GATEWAY_s));
    
    if (IsRuntimeGateway())
    {
        // Connect to the WiFi network and get an address from DHCP     
        LOG.println("Gateway Mode");        
        SetAsGateway(myvNet_dhcp);       // Set this node as gateway for SoulissApp  
        SetAddressingServer();
        
    }
    else 
    {
        // This board request an address to the gateway at runtime, no need
        // to configure any parameter here.
        LOG.println("Peer Mode");
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

    LOG.print("STORE__SIZE: ");
    LOG.println(STORE__SIZE);
    //OTA_Init(); 
    
    httpUpdater.setup(&server);
    MDNS.addService("http", "tcp", 80);

    aio.begin();
}



void loop()
{  
<<<<<<< HEAD

=======
    if(IR_ENABLE){
    	readIR();
    }
    
>>>>>>> origin/master
    runWebServer();

    readIR();
    
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
        SLOW_x10s(1) { 
            count += 1;
            if (testFeed.send(count)) {
              LOG.print(F("Wrote value to feed: ")); LOG.println(count, DEC);
            }
            else {
              LOG.println(F("Error writing value to feed!"));
            }
        }   
        SLOW_x10s(3){
            FeedData latest = testFeed.receive();
            if (latest.isValid()) {
              Serial.print(F("Received value from feed: ")); Serial.println(latest);
              // By default the received feed data item has a string value, however you
              // can use the following functions to attempt to convert it to a numeric
              // value like an int or float.  Each function returns a boolean that indicates
              // if the conversion succeeded, and takes as a parameter by reference the
              // output value.
              int i;
              if (latest.intValue(&i)) {
                Serial.print(F("Value as an int: ")); Serial.println(i, DEC);
              }
              // Other functions that you can use include:
              //  latest.uintValue() (unsigned int)
              //  latest.longValue() (long)
              //  latest.ulongValue() (unsigned long)
              //  latest.floatValue() (float)
              //  latest.doubleValue() (double)
            }
            else {
              Serial.print(F("Failed to receive the latest feed value!"));
            }
        } 
        slowGeneral();
        
        // If running as Peer
        if (!IsRuntimeGateway())
            SLOW_PeerJoin();
    } 
    //OTA_Process();
}    



