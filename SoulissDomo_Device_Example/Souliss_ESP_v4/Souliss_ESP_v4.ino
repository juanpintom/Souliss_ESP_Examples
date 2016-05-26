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

// EmonLibrary examples openenergymonitor.org, Licence GNU GPL V3

//#include "EmonLib.h"                   // Include Emon Library
//EnergyMonitor emon1;                   // Create an instance

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

////// ***************************  OLED  LIBRARY ***************************
////#include <Wire.h>
#include "SSD1306.h"
#include "SSD1306Ui.h"
#include "images.h"

// Initialize the oled display for address 0x3c
// sda-pin=14 and sdc-pin=12
SSD1306   display(0x3c, 5, 4);
SSD1306Ui ui     ( &display );

// this array keeps function pointers to all frames
// frames are the single views that slide from right to left
bool (*frames[])(SSD1306 *display, SSD1306UiState* state, int x, int y) = { drawFrame1, drawFrame2, drawFrame3, drawFrame4 };

// how many frames are there?
int frameCount = 4;

bool (*overlays[])(SSD1306 *display, SSD1306UiState* state)             = { msOverlay };
int overlaysCount = 1;

// ***************************  NTP  LIBRARIES ***************************

#include <ntp.h>
//#include <WiFiUdp.h>
#include <Time.h>
time_t getNTPtime(void);
NTP NTPclient;
#define CET +1

// ***************************  DEBUG OPTIONS   ***************************

#define SERIALPORT_INSKETCH
//#define LOG serverClients[0]
#define LOG Serial

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
#include "FunctionsEEPROM.h"
#include "FunctionsSENSORS.h"
#include "irReceiver.h"
#include "irButtons.h"
#include "SetupAndLoop.h"

#include "Page_General.h"
 
ESP8266HTTPUpdateServer httpUpdater;

/*#include "Adafruit_IO_Client.h"
//#define AIO_KEY    "...your Adafruit IO key value ..."
#define AIO_KEY    "65b1e1717e6d08e3f58768c1de502d5363d6a64e"
Adafruit_IO_Client aio = Adafruit_IO_Client(client, AIO_KEY);
Adafruit_IO_Feed testFeed = aio.getFeed("esptestfeed");
unsigned int count = 0;
*/

/*
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

// ************************* Adafruit.io Setup ********************************

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    "juanpintom"
#define AIO_KEY         "7d5d2fa70f894f9aade434d0814f5c3ad13cfdd8"

// ************ Global State (you don't need to change this!) *****************

// Store the MQTT server, username, and password in flash memory.
// This is required for using the Adafruit MQTT library.
const char MQTT_SERVER[] PROGMEM    = AIO_SERVER;
const char MQTT_USERNAME[] PROGMEM  = AIO_USERNAME;
const char MQTT_PASSWORD[] PROGMEM  = AIO_KEY;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, MQTT_SERVER, AIO_SERVERPORT, MQTT_USERNAME, MQTT_PASSWORD);

// ****************************** Feeds *************************************

// Setup a feed called 'photocell' for publishing.
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>
const char PHOTOCELL_FEED[] PROGMEM = AIO_USERNAME "/feeds/photocell";
Adafruit_MQTT_Publish photocell = Adafruit_MQTT_Publish(&mqtt, PHOTOCELL_FEED);

// Setup a feed called 'onoff' for subscribing to changes.
const char ONOFF_FEED[] PROGMEM = AIO_USERNAME "/feeds/onoff";
Adafruit_MQTT_Subscribe onoffbutton = Adafruit_MQTT_Subscribe(&mqtt, ONOFF_FEED);

// *************************** Sketch Code **********************************

// Bug workaround for Arduino 1.6.6, it seems to need a function declaration
// for some reason (only affects ESP8266, likely an arduino-builder bug).
void MQTT_connect();
*/

WiFiUDP Udp;
IPAddress remote_ip(192, 168, 1, 12); // you PC network address
#define UDP_PORT 1111
  
#define DEBUG_UDP(msg) Udp.beginPacket(remote_ip, UDP_PORT);\
  Udp.write(msg);\
  Udp.write("\r\n");\
  Udp.endPacket();

//****************************************************************************
//************************** SETUP FUNCTION **********************************
//****************************************************************************

void setup()
{
    Udp.begin(UDP_PORT);
    
    //LOG.begin(115200);
    EEPROM.begin(512);   //TENGO COMENTADO EL EEPROM.begin DE SOULISS EN STORE.CPP
    ReadConfig_Slots();
    DEBUG.begin(115200);
    DEBUG.println(""); DEBUG.println("Node Started V.1.0.2");
    showConfig();
    //ReadConfig_Slots();

    
    
    // **************************************  OLED SETUP *********************
    if(DEBUG_LOG && (S51 == OLED || S51 == BMP180)) I2CScanner();
    if(S51 == OLED) OLEDsetup();
  
    //emon1.current(0, 10);             // Current: input pin, calibration.

    Initialize();
    deleteEEPROM();
    
    server.on ( "/", send_general_html  );
    server.on ( "/general.html", send_general_html  );
    server.on ( "/admin/generalvalues", send_general_configuration_values_html);

    httpUpdater.setup(&server);
    MDNS.addService("http", "tcp", 80);
    
    //ReadConfig_Slots();
    EEPROM.begin(512);
    // Read the IP configuration from the EEPROM, if not available start
    // the node as access point
    if(!ReadIPConfiguration()) 
	  {	
  		// Start the node as access point with a configuration WebServer
  		SetAccessPoint();
  		startWebServer();
      if(S51 == OLED) drawNOWIFI();
      DEBUG.print("defaultValues: ");
      DEBUG.println(defaultValues());
  		// We have nothing more than the WebServer for the configuration
  		// to run, once configured the node will quit this.
  		while(1)
  		{
  			yield();
  			runWebServer(); 
  		}
  
	  }
    if(S51 == OLED) drawSouliss();
    
    if (IsRuntimeGateway())
    {
        // Connect to the WiFi network and get an address from DHCP     
        DEBUG.println("Gateway Mode");        
        SetAsGateway(myvNet_dhcp);       // Set this node as gateway for SoulissApp  
        SetAddressingServer();
   //NODOS PREDEFINIDOS PARA MI!!
        //SetAsPeerNode(0x00C7, 1);    // NODO 644 HABITACION OCIO IP199
        //SetAsPeerNode(0x00C9, 2);   // NODO ENTRADA IP201
       //NO USADOS
        //SetAsPeerNode(0x00CA, 3);    // NODO DORMIR IP202
        //SetAsPeerNode(0x00CB, 4);    // NODO SALON IP203
        //SetAsPeerNode(0x00CC, 5);    // NODO ESCALERA IP204
        
    }
    else 
    {
        // This board request an address to the gateway at runtime, no need
        // to configure any parameter here.
        DEBUG.println("Peer Mode");
        SetDynamicAddressing();  
        GetAddress();
        
    }
    
    setupGeneral();
    startWebServer();
    DEBUG.print("vNet Media : ");
    DEBUG.println (vNet_GetAddress(vNet_MyMedia()),HEX);
    DEBUG.print("vNet 1 : ");
    DEBUG.print(vNet_GetAddress(1),HEX);
    DEBUG.print(" | IP: ");
    DEBUG.println(vNet_GetAddress(1),DEC);
    DEBUG.print("vNet 2 : ");
    DEBUG.println (vNet_GetAddress(2),HEX);
    DEBUG.print("vNet 3 : ");
    DEBUG.println (vNet_GetAddress(3),HEX);
    DEBUG.print("vNet 4 : ");
    DEBUG.println (vNet_GetAddress(4),HEX);
    DEBUG.print("vNet 5 : ");
    DEBUG.println (vNet_GetAddress(5),HEX);

    DEBUG.print("STORE__SIZE: ");
    DEBUG.println(STORE__SIZE);
    //OTA_Init(); 
    
//    httpUpdater.setup(&server);
//    MDNS.addService("http", "tcp", 80);



    //aio.begin();
    //mqtt.subscribe(&onoffbutton);
// **************************************  NTP SETUP *********************
    NTPclient.begin("time-a.timefreq.bldrdoc.gov", CET);
    setSyncInterval(SECS_PER_HOUR);
    setSyncProvider(getNTPtime);
// **************************************  IR SETUP *********************
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
        FAST_x10ms(10){
            //MQTT_Loop();
              if(S51 == OLED) int remainingTimeBudget = ui.update();
              //ReadPCF();
       //  PFC8591 READS
//              LOG.print("CH0 ");
//              adc_value = getADC(PCF8591_ADC_CH0); //Channel 3 is the pot
//              LOG.print(adc_value);
//
//              LOG.print("\r CH1 ");
//              adc_value = getADC(PCF8591_ADC_CH1); //Channel 3 is the pot
//              LOG.print(adc_value);
//
//              LOG.print("\r CH2 ");
//              adc_value = getADC(PCF8591_ADC_CH2); //Channel 3 is the pot
//              LOG.print(adc_value);
//              
//              LOG.print("\r CH3 ");
//              adc_value = getADC(PCF8591_ADC_CH3); //Channel 3 is the pot
//              LOG.println(adc_value);
        }
//        FAST_x10ms(20){
//          if(Emonlib){
//            double Irms = calcIrms(2048);
//            //double Irms = emon1.calcIrms(1480);  // Calculate Irms only
//            LOG.print(" ");
//            LOG.print(Irms*230.0);         // Apparent power
//            LOG.print(" ");
//            LOG.println(Irms);          // Irms 
//          }
//        }
//        FAST_x10ms(500){
//            //MQTT_Loop_Slow();
//            
//        }        
        //Serial.println("FAST");
          FAST_x10ms(100){
              DEBUG_UDP("Test"); 
          }
        fastGeneral();
         
        // Run communication as Gateway or Peer
        if (IsRuntimeGateway())
            FAST_GatewayComms(); 
        else 
            FAST_PeerComms();   
    }

    EXECUTESLOW() {
        //Serial.println("SLOW");
        UPDATESLOW();       
        slowGeneral();
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
        
        // If running as Peer
        if (!IsRuntimeGateway())
            SLOW_PeerJoin();
    } 
  //}
}  

// ******************************************************************************************************************
// **********************************************  MQTT FUNCTIONS ***************************************************
// ******************************************************************************************************************
/*
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
}*/

// ******************************************************************************************************************
// **********************************************  OLED FUNCTIONS ***************************************************
// ******************************************************************************************************************


void OLEDsetup(){

  ui.setTargetFPS(30);

  ui.setActiveSymbole(activeSymbole);
  ui.setInactiveSymbole(inactiveSymbole);

  // You can change this to
  // TOP, LEFT, BOTTOM, RIGHT
  ui.setIndicatorPosition(BOTTOM);

  // Defines where the first frame is located in the bar.
  ui.setIndicatorDirection(LEFT_RIGHT);

  // You can change the transition that is used
  // SLIDE_LEFT, SLIDE_RIGHT, SLIDE_TOP, SLIDE_DOWN
  ui.setFrameAnimation(SLIDE_LEFT);

  // Add frames
  ui.setFrames(frames, frameCount);

  // Add overlays
  ui.setOverlays(overlays, overlaysCount);

  // Inital UI takes care of initalising the display too.
  ui.init();

  display.flipScreenVertically();
  drawStart();
  
}


bool msOverlay(SSD1306 *display, SSD1306UiState* state) {
  display->setTextAlignment(TEXT_ALIGN_RIGHT);
  display->setFont(ArialMT_Plain_10);
//  display->drawString(128, 0, String(millis()));
  String minu, segu;
  if(minute()<10) minu = "0" + String(minute()); else minu = String(minute());
  if(second()<10) segu = "0" + String(second()); else segu = String(second());
  String FechaHora = String(hour())  + ":" + minu  + "." + segu + " " + String(day()) + "/" + String(month()) + "/" + String(year());
  display->drawString(128, 0, FechaHora);
  return true;
}

bool drawFrame1(SSD1306 *display, SSD1306UiState* state, int x, int y) {
  // draw an xbm image.
  // Please note that everything that should be transitioned
  // needs to be drawn relative to x and y
  display->setFont(ArialMT_Plain_10);
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->drawString(0 + x, 11 + y, String(WiFi.SSID()));
  // if this frame need to be refreshed at the targetFPS you need to
  // return true
  display->drawXbm(x + 34, y + 14, WiFi_Logo_width, WiFi_Logo_height, WiFi_Logo_bits);
  return false;
}

bool drawFrame2(SSD1306 *display, SSD1306UiState* state, int x, int y) {
  // Demonstrates the 3 included default sizes. The fonts come from SSD1306Fonts.h file
  // Besides the default fonts there will be a program to convert TrueType fonts into this format
//  display->setTextAlignment(TEXT_ALIGN_LEFT);
//  display->setFont(ArialMT_Plain_10);
//  display->drawString(0 + x, 10 + y, "Arial 10");
//
//  display->setFont(ArialMT_Plain_16);
//  display->drawString(0 + x, 20 + y, "Arial 16");
//
//  display->setFont(ArialMT_Plain_24);
//  display->drawString(0 + x, 34 + y, "Arial 24");
//  display->drawXbm(128 - thermometer24_widthPixels, 10, thermometer24_widthPixels, thermometer24_heightPixels, thermometer24_bits);
  display->drawXbm(x + (128 - ((humidity24_widthPixels+thermometer24_widthPixels)/2)), y + 10, thermometer24_widthPixels, thermometer24_heightPixels, thermometer24_bits);
  display->drawXbm(x + (128 - humidity24_widthPixels), y + 34, humidity24_widthPixels, humidity24_heightPixels, humidity24_bits);
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->setFont(ArialMT_Plain_10);
  //display->drawString(64 + x, 10 + y, "Temperature - Humidity");

  display->setFont(ArialMT_Plain_24);
//  display->drawString(64 + x, 20 + y, String(mOutputAsFloat(TEMPERATURE)));
  display->drawString(64 + x, 10 + y, String(mOutputAsFloat(TEMPERATURE))+"ºC");

  display->setFont(ArialMT_Plain_24);
  display->drawString(64 + x, 34 + y, String(mOutputAsFloat(HUMIDITY))+"%");
  return false;
}

bool drawFrame3(SSD1306 *display, SSD1306UiState* state, int x, int y) {
//  // Text alignment demo
//  display->setFont(ArialMT_Plain_10);
//
//  // The coordinates define the left starting point of the text
//  display->setTextAlignment(TEXT_ALIGN_LEFT);
//  display->drawString(0 + x, 11 + y, "Left aligned (0,10)");
//
//  // The coordinates define the center of the text
//  display->setTextAlignment(TEXT_ALIGN_CENTER);
//  //display->drawString(64 + x, 22, "Center aligned (64,22)");  
//  display->drawString(64 + x, 22, String(mOutputAsFloat(DALLAS)));
//  // The coordinates define the right end of the text
//  display->setTextAlignment(TEXT_ALIGN_RIGHT);
//  display->drawString(128 + x, 33, "Right aligned (128,33)");

  display->drawXbm(x + (128 - thermometer24_widthPixels), y + 22, thermometer24_widthPixels, thermometer24_heightPixels, thermometer24_bits);
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->setFont(ArialMT_Plain_16);
  display->drawString(64 + x, 11, "Temperature");
  // The coordinates define the center of the text
  display->setFont(ArialMT_Plain_24); 
  display->drawString(64 + x, 22, String(mOutputAsFloat(DALLAS)));
  

  return false;
}

bool drawFrame4(SSD1306 *display, SSD1306UiState* state, int x, int y) {
  // Demo for drawStringMaxWidth:
  // with the third parameter you can define the width after which words will be wrapped.
  // Currently only spaces and "-" are allowed for wrapping
//  display->setTextAlignment(TEXT_ALIGN_LEFT);
//  display->setFont(ArialMT_Plain_10);
//  display->drawStringMaxWidth(0 + x, 10 + y, 128, "Lorem ");
  display->drawXbm(x + (128 - lux24_widthPixels), y + 22, lux24_widthPixels, lux24_heightPixels, lux24_bits);
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->setFont(ArialMT_Plain_16);
  display->drawString(64 + x, 11, "Luminosity");
  // The coordinates define the center of the text
  display->setFont(ArialMT_Plain_24); 
  display->drawString(64 + x, 22, String(mOutputAsFloat(LDR)));
  
  return false;
}

bool drawStart() {
  // draw an xbm image.
  // Please note that everything that should be transitioned
  // needs to be drawn relative to x and y
  Serial.println("DrawStart");
  display.displayOn();
  display.clear();
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(0, 0, "Connecting to WiFi...");
  // if this frame need to be refreshed at the targetFPS you need to
  // return true
  display.drawXbm(34, 14, WiFi_Logo_width, WiFi_Logo_height, WiFi_Logo_bits);
  //display.drawXbm((128-souliss_logo_widthPixels)/2, 14, souliss_logo_widthPixels, souliss_logo_heightPixels, souliss_logo_bits);
  display.display();
  return false;
}

bool drawNOWIFI() {
  // draw an xbm image.
  // Please note that everything that should be transitioned
  // needs to be drawn relative to x and y
  DEBUG.println("DrawNOWIFI");
  display.displayOn();
  display.clear();
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  //display.drawString(0, 11, "NO WIFI CONFIGURED, Enter to 192.168.4.1");
  display.drawStringMaxWidth(0, 0, 128, "NO WIFI CONFIGURED, Enter to 192.168.4.1");
  // if this frame need to be refreshed at the targetFPS you need to
  // return true
  //display.drawXbm(34, 14, WiFi_Logo_width, WiFi_Logo_height, WiFi_Logo_bits);
  display.display();
  return false;
}

bool drawSouliss() {
  // draw an xbm image.
  // Please note that everything that should be transitioned
  // needs to be drawn relative to x and y
  DEBUG.println("DrawSouliss");
  display.displayOn();
  display.clear();
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(0, 0, "Starting Souliss...");
  // if this frame need to be refreshed at the targetFPS you need to
  // return true
  display.drawXbm((128-souliss_logo_widthPixels)/2, 14, souliss_logo_widthPixels, souliss_logo_heightPixels, souliss_logo_bits);
  display.display();
  return false;
}

void I2CScanner(){

  Wire.begin(5,4);
  DEBUG.println ();
  DEBUG.println ("I2C scanner. Scanning ...");
  byte count = 0;
  for (byte i = 1; i < 120; i++)
  {
    Wire.beginTransmission (i);
    if (Wire.endTransmission () == 0)
      {
      DEBUG.print ("Found address: ");
      DEBUG.print (i, DEC);
      DEBUG.print (" (0x");
      DEBUG.print (i, HEX);
      DEBUG.println (")");
      count++;
      delay (1);  // maybe unneeded?
      } // end of good response
  } // end of for loop
  DEBUG.println ("Done.");
  DEBUG.print ("Found ");
  DEBUG.print (count, DEC);
  DEBUG.println (" device(s).");
  delay(1000);
}


// ******************************************************************************************************************
// *************************************************  CLOCK FUNCTION ***************************************************
// ******************************************************************************************************************

time_t getNTPtime(void)
{
  return NTPclient.getNtpTime();
}

