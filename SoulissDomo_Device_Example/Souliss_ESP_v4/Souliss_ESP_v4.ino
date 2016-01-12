/**************************************************************************
   Souliss - Web Configuration
    
    This example demonstrate a complete web configuration of ESP8266 based
	nodes, the node starts as access point and allow though a web interface
	the configuration of IP and Souliss parameters.

	This example is only supported on ESP8266.
 
***************************************************************************/

//#define SERIALPORT_INSKETCH
//  #define LOG Serial
 
// ************************* IR LIBRARY ***********************************
// To use the IR Functions you need to add this library to your Arduino libraries:
// https://github.com/markszabo/IRremoteESP8266/

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


// ************************* TELNET DEBUG ******************************
#define MAX_SRV_CLIENTS 1
WiFiServer telnet(23);
WiFiClient serverClients[MAX_SRV_CLIENTS];

#define SERIALPORT_INSKETCH
#define LOG serverClients[0]
//#define LOG Serial
// ***************************  SOULISS  LIBRARIES ***************************
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




 
/*#include "Adafruit_IO_Client.h"
//#define AIO_KEY    "...your Adafruit IO key value ..."
#define AIO_KEY    "65b1e1717e6d08e3f58768c1de502d5363d6a64e"
Adafruit_IO_Client aio = Adafruit_IO_Client(client, AIO_KEY);
Adafruit_IO_Feed testFeed = aio.getFeed("esptestfeed");
unsigned int count = 0;
*/

#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    "juanpintom"
#define AIO_KEY         "7d5d2fa70f894f9aade434d0814f5c3ad13cfdd8"

/************ Global State (you don't need to change this!) ******************/

// Store the MQTT server, username, and password in flash memory.
// This is required for using the Adafruit MQTT library.
const char MQTT_SERVER[] PROGMEM    = AIO_SERVER;
const char MQTT_USERNAME[] PROGMEM  = AIO_USERNAME;
const char MQTT_PASSWORD[] PROGMEM  = AIO_KEY;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, MQTT_SERVER, AIO_SERVERPORT, MQTT_USERNAME, MQTT_PASSWORD);

/****************************** Feeds ***************************************/

// Setup a feed called 'photocell' for publishing.
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>
const char PHOTOCELL_FEED[] PROGMEM = AIO_USERNAME "/feeds/photocell";
Adafruit_MQTT_Publish photocell = Adafruit_MQTT_Publish(&mqtt, PHOTOCELL_FEED);

// Setup a feed called 'onoff' for subscribing to changes.
const char ONOFF_FEED[] PROGMEM = AIO_USERNAME "/feeds/onoff";
Adafruit_MQTT_Subscribe onoffbutton = Adafruit_MQTT_Subscribe(&mqtt, ONOFF_FEED);

/*************************** Sketch Code ************************************/

// Bug workaround for Arduino 1.6.6, it seems to need a function declaration
// for some reason (only affects ESP8266, likely an arduino-builder bug).
void MQTT_connect();

//****************************************************************************
//************************** SETUP FUNCTION **********************************
//****************************************************************************

void setup()
{
    //LOG.begin(115200);
    Serial.begin(115200);
    Initialize();
    
    //************************************** TELNET SETUP *************************
    telnet.begin();
    telnet.setNoDelay(true);
    Serial.print("Start: "); Serial.println(millis());
    while(Telnet_Loop() || millis() > 10000){ Serial.println(10000 - millis()); }
    

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
    /*if(DEBUG_PRESSURE){
      LOG.print("ALTITUD: ");
      LOG.println(ALTITUDE);
    }
    
    
    LOG.print("EEPROM GW: ");
    LOG.println(EEPROM.read(STORE__GATEWAY_s));*/
    
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



    //aio.begin();
    mqtt.subscribe(&onoffbutton);
    
    if(IR_ENABLE){
      DEBUG.println("IR_START");
      irrecv.enableIRIn();  // Start the receiver
    }
}

uint32_t x=0;

void loop()
{  
    if(IR_ENABLE){
    	readIR();
    }
    runWebServer();
        
    EXECUTEFAST() {                     
        UPDATEFAST();   
        FAST_x10ms(5){
            //MQTT_Loop();
        }
        FAST_x10ms(500){
            //MQTT_Loop_Slow();
            Telnet_Loop();
        }        
        fastGeneral();
         
        // Run communication as Gateway or Peer
        if (IsRuntimeGateway())
            FAST_GatewayComms(); 
        else 
            FAST_PeerComms();   
    }

    EXECUTESLOW() {
        UPDATESLOW();
        /*SLOW_x10s(1) { 
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
        } */
        slowGeneral();
        
        // If running as Peer
        if (!IsRuntimeGateway())
            SLOW_PeerJoin();
    } 
    //OTA_Process();
}  

boolean Telnet_Loop(){
  uint8_t i;
  //check if there are any new clients
  if (telnet.hasClient()){
    for(i = 0; i < MAX_SRV_CLIENTS; i++){
      //find free/disconnected spot
      if (!serverClients[i] || !serverClients[i].connected()){
        if(serverClients[i]) serverClients[i].stop();
        serverClients[i] = telnet.available();
        //Serial1.print("New client: "); Serial1.print(i);
        return 1;
        continue;
      }
    }
    //no free/disconnected spot so reject
    WiFiClient serverClient = telnet.available();
    serverClient.stop();
  }
  return 0;
  //check clients for data
  /*for(i = 0; i < MAX_SRV_CLIENTS; i++){
  //  if (serverClients[i] && serverClients[i].connected()){
  //    if(serverClients[i].available()){
  //      //get data from the telnet client and push it to the UART
  //      while(serverClients[i].available()) Serial.write(serverClients[i].read());
  //    }
  //  }
  //}
  //check UART for data
  if(Serial.available()){
    size_t len = Serial.available();
    uint8_t sbuf[len];
    Serial.readBytes(sbuf, len);
    //push UART data to all connected telnet clients
    for(i = 0; i < MAX_SRV_CLIENTS; i++){
      if (serverClients[i] && serverClients[i].connected()){
        serverClients[i].write(sbuf, len);
        delay(1);
      }
    }
  }*/
  
  /*for(i = 0; i < MAX_SRV_CLIENTS; i++){
      if (serverClients[i] && serverClients[i].connected()){
        serverClients[i].println(millis());
        delay(1);
      }
  }*/
  
  
}  

void MQTT_Loop(){
  MQTT_connect();

  // this is our 'wait for incoming subscription packets' busy subloop
  Adafruit_MQTT_Subscribe *subscription;
  //LOG.println(F("Start Suscription: "));
  while ((subscription = mqtt.readSubscription(1000))) {
    if (subscription == &onoffbutton) {
      LOG.print(F("Got: "));
      LOG.println((char *)onoffbutton.lastread);
    }
  }
}
void MQTT_Loop_Slow(){
  // ping the server to keep the mqtt connection alive
  if(! mqtt.ping()) {
    mqtt.disconnect();
  }
  
  // Now we can publish stuff!
  LOG.print(F("\nSending photocell val "));
  LOG.print(x);
  LOG.print("...");
  if (! photocell.publish(x++)) {
    LOG.println(F("Failed"));
  } else {
    LOG.println(F("OK!"));
  }
  
}
// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  LOG.print("Connecting to MQTT... ");

  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       LOG.println(mqtt.connectErrorString(ret));
       LOG.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
  }
  LOG.println("MQTT Connected!");
}

