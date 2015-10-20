#ifndef Functions_h
#define Functions_h

int valorPWM;

#define LOG Serial

//#define PCB
#define STRIPBOARD

//Autocalibrate Capacitive Sensors ON
#define AUTOCALIBRATE         1

boolean DEBUG_CAPSENSE = 0;
boolean DEBUG_CAPSENSE_ALL = 0;
boolean DEBUG_PRESSURE = 0;
boolean DEBUG_GETLUX   = 0;
boolean DEBUG_DALLAS   = 1;

//COPIED TO Souliss webconfig.h
byte byte0;
byte byte1;
byte byte2;
byte cap_thresold;
int ALTITUDE = 20;

//DHT Sensor on PIN 16
boolean DHT_SENSOR;

//LDR Sensor on PIN ADC
boolean LDR_SENSOR;

//DALLAS Sensor on PIN 14
boolean DALLAS_SENSOR;   

//PWM PIN 15 or PIR (PIR Sensor on PIN 2) 
boolean PWM_MODE;           
boolean PIR_MODE;
boolean RGB_MODE;
boolean ALARM_MODE;

#define LIGHT_ON_CYCLE 10    		// Light ON for 10 cycles if triggered by a PIR sensor

//Enable this to use PINS 4, 5 for CAPACITIVE BUTTONS, RELAY1 and RELAY2 or on BMP180 pressure sensor (I2C)
boolean CAPACITIVE;     
boolean RELAY;          
boolean BMP180;       


//**********************  SLOTS VARIABLES  ***********************
byte ALARM;
byte TEMPERATURE;
byte HUMIDITY;
byte LEDPWM0;
byte LEDPWM1;
byte LEDPWM2;
byte LED;
byte LEDRGB;
byte LDR;
byte DALLAS;
byte CAP0;
byte CAP1;
byte RELAY0;
byte RELAY1;
byte PRESSURE0;
byte BMP180TEMP;

//**********************  PIN VARIABLES  *************************
byte ALARMP;
byte LEDPWMP0;
byte LEDPWMP1;
byte LEDPWMP2;
byte LEDP;
byte PIRP;
byte LEDRP;
byte LEDGP;
byte LEDBP;
byte CAP0P;
byte CAP1P;
byte THRE;
byte RELAY0P;
byte RELAY1P;
byte SDAP;
byte SCLP;

// **************************************************************************

// Include and Configure DHT11 SENSOR
#ifdef PCB    //PCB NUEVA
  #define DHTPIN      16     // what pin we're connected to
#endif

#ifdef STRIPBOARD //STRIPBOARD DE PRUEBAS
  #define DHTPIN      13         // what pin we're connected to
#endif

//#include "DHT.h"
#define DHTTYPE DHT11   // DHT 11 
DHT dht(DHTPIN, DHTTYPE, 15);

// Light calibration data
// out[] holds the values wanted in lux/10
#define SIZEOF 10
static const unsigned int out[] = { 1, 7, 30, 45, 65, 150, 300, 450, 2100, 13000};  // x10  //ULTIMO VALOR REFERENCIA
static const unsigned int in[]  = { 1, 100, 350, 430, 500, 680, 780, 950, 1005, 1024 };  // 0 - 1024

#ifdef PCB    //PCB NUEVA
  #define DALLASPIN   14     //Se declara el pin donde se conectarÃ¡ la DATA
#endif

#ifdef STRIPBOARD //STRIPBOARD DE PRUEBAS
  #define DALLASPIN   4     //Se declara el pin donde se conectarÃ¡ la DATA
#endif


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
char input[] = "testesp";
char apikey[] = "xxxxxxxxxxxxxxxxxxxxxxxxx";
int port = 80; // port 80 is the default for HTTP
WiFiClient client;

void SendEmoncms(byte SLOT){
  
  float value = mOutputAsFloat(SLOT);
  LOG.println(value);
  
  if (client.connect(serveremon, port))
  {
    Serial.println("connected");
    // Make a HTTP request:
    client.print("POST ");
    client.print(path);
    client.print(input);
    client.print(":");
    client.print(value);
    client.print("&apikey=");
    client.print(apikey);
    client.println(" HTTP/1.1");
    client.print("Host: ");
    client.println(serveremon);
    client.println("Connection: close");
    client.println();
  }
  else
  {
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
  }
}



// ******************************************************************************************************************
// *************************************************  PINS CONFIG ***************************************************
// ******************************************************************************************************************
void PINS_CONFIG(){
  #ifdef PCB    //PCB NUEVA
    if(DHT_SENSOR){
        //DHTPIN = 16;     // what pin we're connected to
    }
    if(PWM_MODE){
        LEDPWMP0 = 12;      //LED STRIP ON PIN 12
        LEDPWMP1 = 13;      //LED STRIP ON PIN 13
        LEDPWMP2 = 15;      //LED STRIP ON PIN 15
    }
    
    if(PIR_MODE){
        LEDPWMP0 = 12;//5;      //LED STRIP ON PIN 12
        LEDPWMP1 = 13;//16;      //LED STRIP ON PIN 13
        LEDP = 15;      //LED STRIP ON PIN 15
        PIRP = 2;       //LED STRIP ON PIN 2
    }
    if(ALARM_MODE){
        ALARMP = 2;
    }
    
    if(RGB_MODE){
        LEDRP = 12;//5;      //LED STRIP ON PIN 12
        LEDGP = 13;//16;      //LED STRIP ON PIN 13
        LEDBP = 15;      //LED STRIP ON PIN 15
    }
    
    //PIN OPTIONS FOR CAPACITIVE - RELAY OR BMP180
    if(CAPACITIVE){
        //SDA 5  SCL 4  PINS MUST CHANGE ON WIRE.H
        CAP0P = 4;//12;
        CAP1P = 5;//14;
    }
    
    if(RELAY){
        RELAY0P = 4;//12;
        RELAY1P = 5;//14;
    }
    if(BMP180){
        SCLP = 4;//12;
        SDAP = 5;//14;
    }
  #endif

  #ifdef STRIPBOARD    //PCB NUEVA
    if(DHT_SENSOR){
        //DHTPIN = 16;     // what pin we're connected to
    }
    if(PWM_MODE){
        LEDPWMP0 = 5;      //LED STRIP ON PIN 12
        LEDPWMP1 = 2;//16;      //LED STRIP ON PIN 13
        LEDPWMP2 = 15;      //LED STRIP ON PIN 15
    }
    
    if(PIR_MODE){
        LEDPWMP0 = 5;      //LED STRIP ON PIN 12
        LEDPWMP1 = 2;//16;      //LED STRIP ON PIN 13
        LEDP = 15;      //LED STRIP ON PIN 15
        PIRP = 16;//2;       //LED STRIP ON PIN 2
    }
    if(ALARM_MODE){
        ALARMP = 16;//2;
    }    
    
    if(RGB_MODE){
        LEDRP = 5;      //LED STRIP ON PIN 12
        LEDGP = 2;//16;      //LED STRIP ON PIN 13
        LEDBP = 15;      //LED STRIP ON PIN 15
    }
    
    //PIN OPTIONS FOR CAPACITIVE - RELAY OR BMP180
    if(CAPACITIVE){
        //SDA 5  SCL 4  PINS MUST CHANGE ON WIRE.H
        CAP0P = 12;
        CAP1P = 14;
    }
    
    if(RELAY){
        RELAY0P = 12;
        RELAY1P = 14;
    }
    if(BMP180){
        SCLP = 12;
        SDAP = 14;
    }
  #endif

}
// ******************************************************************************************************************
// ***********************************************  SLOTS CONFIG ****************************************************
// ******************************************************************************************************************
void SLOT_CONFIG(){
  int NEXTSLOT = 0;
  LOG.println("SLOT CONFIG");  
  
  if(ALARM_MODE){
      ALARM = NEXTSLOT;
      NEXTSLOT = ALARM + 1;
      LOG.print("ALARM: ");
      LOG.println(ALARM);
  }
  
  if(DHT_SENSOR){
      TEMPERATURE = NEXTSLOT;
      HUMIDITY = NEXTSLOT + 2;
      NEXTSLOT = HUMIDITY + 2;
      LOG.print("TEMP: ");
      LOG.println(TEMPERATURE);  
      LOG.print("HUMI: ");
      LOG.println(HUMIDITY);      
  }
  
  if(PWM_MODE || PIR_MODE){
      LEDPWM0 = NEXTSLOT;
      LEDPWM1 = NEXTSLOT + 2;
      NEXTSLOT = LEDPWM1 + 2;
      LOG.print("LEDPWM0: ");
      LOG.println(LEDPWM0);  
      LOG.print("LEDPWM1: ");
      LOG.println(LEDPWM1);      
  }
  if(PWM_MODE){
      LEDPWM2 = NEXTSLOT;
      NEXTSLOT = LEDPWM2 + 2;
      LOG.print("LEDPWM2: ");
      LOG.println(LEDPWM2);     
  }
  
  if(PIR_MODE){
      LED = NEXTSLOT;
      NEXTSLOT = LED + 1;
      LOG.print("LED: ");
      LOG.println(LED);       
  }
  
  if(RGB_MODE){
      LEDRGB = NEXTSLOT;
      NEXTSLOT = LEDRGB + 4;
      LOG.print("LEDRGB: ");
      LOG.println(LEDRGB);        
  }
  
  if(LDR_SENSOR){
      LDR = NEXTSLOT;
      NEXTSLOT = LDR + 2;
      LOG.print("LDR: ");
      LOG.println(LDR);        
  }
  
  if(DALLAS_SENSOR){
      DALLAS = NEXTSLOT;
      NEXTSLOT = DALLAS + 2;
      LOG.print("DALLAS: ");
      LOG.println(DALLAS);        
  }
  
  //GPIO 4-5 SLOT DEFINITIONS
  
  if(CAPACITIVE && DEBUG_CAPSENSE){
      CAP0 = NEXTSLOT; 
      CAP1 = NEXTSLOT + 2;
      THRE = NEXTSLOT + 4;
      NEXTSLOT = THRE + 2;
      LOG.print("CAP0: ");
      LOG.println(CAP0);   
      LOG.print("CAP1: ");
      LOG.println(CAP1);             
      LOG.print("THRE: ");
      LOG.println(THRE);                   
  }
  
  if(RELAY){
      RELAY0 = NEXTSLOT;
      RELAY1 = NEXTSLOT + 1;
      NEXTSLOT = RELAY1 + 1;
      LOG.print("RELAY0: ");
      LOG.println(RELAY0);   
      LOG.print("RELAY1: ");
      LOG.println(RELAY1);         
  }
  
  if(BMP180){
      PRESSURE0 = NEXTSLOT;  
      BMP180TEMP = NEXTSLOT + 2;
      NEXTSLOT = BMP180TEMP + 2;
      LOG.print("PRESSURE0: ");
      LOG.println(PRESSURE0);   
      LOG.print("BMP180TEMP: ");
      LOG.println(BMP180TEMP);       
  }  
}

// ******************************************************************************************************************
// ***********************************************  EEPROM CONFIG ***************************************************
// ******************************************************************************************************************

bool EEPROM_CONFIG(){
    
    //EEPROM CONFIGURATION READ.

    // DHT LDR DALLAS OPTIONS:
    switch (byte0) {  
        case 0:
            DHT_SENSOR = false;
            LDR_SENSOR = false;
            DALLAS_SENSOR = false;  
            break;
        case 1:
            DHT_SENSOR = true;
            LDR_SENSOR = false;
            DALLAS_SENSOR = false;  
            break;
        case 2:
            DHT_SENSOR = false;
            LDR_SENSOR = true;
            DALLAS_SENSOR = false;  
            break;
        case 3:
            DHT_SENSOR = false;
            LDR_SENSOR = false;
            DALLAS_SENSOR = true;  
            break;
        case 4:
            DHT_SENSOR = true;
            LDR_SENSOR = true;
            DALLAS_SENSOR = false;  
            break;
        case 5:
            DHT_SENSOR = true;
            LDR_SENSOR = false;
            DALLAS_SENSOR = true;  
            break;
        case 6:
            DHT_SENSOR = false;
            LDR_SENSOR = true;
            DALLAS_SENSOR = true;  
            break;
        case 7:
            DHT_SENSOR = true;
            LDR_SENSOR = true;
            DALLAS_SENSOR = true;  
            break;            
    }
    
    LOG.print(DHT_SENSOR);
    LOG.print(LDR_SENSOR);
    LOG.print(DALLAS_SENSOR);
    LOG.print(" DLD (DHT-LDR-DALLAS)");
    LOG.print("\r\n");
    
    // PWM PIR RGB OPTIONS:
    //switch (configuration[EEPROM_START+1]) {  
    switch (byte1) {
        case 0:
            PWM_MODE = false;
            PIR_MODE = false;
            RGB_MODE = false;
            ALARM_MODE = false;
            break;
        case 1:
            PWM_MODE = true;
            PIR_MODE = false;
            RGB_MODE = false;
            ALARM_MODE = false;
            break;
        case 2:
            PWM_MODE = false;
            PIR_MODE = true;
            RGB_MODE = false;
            ALARM_MODE = false;
            break;
        case 3:
            PWM_MODE = false;
            PIR_MODE = false;
            RGB_MODE = true;
            ALARM_MODE = false;
            break;
        case 4:
            PWM_MODE = false;
            PIR_MODE = true;
            RGB_MODE = false;
            ALARM_MODE = true;
            break;
    }
    LOG.print(PWM_MODE);
    LOG.print(PIR_MODE);
    LOG.print(RGB_MODE);
    LOG.print(ALARM_MODE);
    LOG.print(" PPR (PWM-PIR-RGB-ALARM)");
    LOG.print("\r\n");
    
    // CAPACITIVE RELAY BMP180 OPTIONS
    //switch (configuration[EEPROM_START+2]) {
    switch (byte2) { 
        case 0:
            CAPACITIVE = false;
            RELAY = false;
            BMP180 = false;
            
            break;
        case 1:
            CAPACITIVE = true;
            RELAY = false;
            BMP180 = false;
            DEBUG_CAPSENSE = false;
            break;
        case 2:
            CAPACITIVE = false;
            RELAY = true;
            BMP180 = false;
            break;
        case 3:
            CAPACITIVE = false;
            RELAY = false;
            BMP180 = true;
            break;
        case 4:
            CAPACITIVE = true;
            RELAY = false;
            BMP180 = false;
            DEBUG_CAPSENSE = true;
            break;    
            
    }
    LOG.print(CAPACITIVE);
    LOG.print(RELAY);
    LOG.print(BMP180);
    LOG.print(" CRB (CAP-RELAY-BMP180)");
    LOG.print("\r\n");
    
    return 1;

}

void WriteConfig_Slots()
{

	LOG.println("Writing Config");
        EEPROM.write(500,cap_thresold);
	EEPROM.write(501,byte0);
	EEPROM.write(502,byte1);
	EEPROM.write(503,byte2);
        EEPROM.write(504, ALTITUDE/20);
        EEPROM.commit();
	
}
void ReadConfig_Slots()
{

	LOG.println(F("Reading Configuration"));
	cap_thresold = EEPROM.read(500);
	byte0 = EEPROM.read(501);
	byte1 = EEPROM.read(502);
	byte2 = EEPROM.read(503);
        ALTITUDE = EEPROM.read(504)*20;
		
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
          LOG.print(abs(millis()-time)); 
          LOG.print("\t");          

    }
  
    if(cycles > thresold_value && (InPin[pin] == 10 || InPin[pin] == 11)){
               InPin[pin] = PINSET;
    }
        
    if(cycles > thresold_value && (InPin[pin]==PINRESET))
	{
		time = millis();								// Record time
		InPin[pin] = PINSET;
		
		return InPin[pin];
	}
	else if(cycles > thresold_value && (abs(millis()-time) > holdtime) && (InPin[pin]==PINSET || InPin[pin]==PINACTIVE))
	{
	    if(AUTOCALIBRATE && (abs(millis()-time) > 15000)){
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
        float h = dht.readHumidity();
        // Read temperature as Celsius
        float t = dht.readTemperature();
        // Read temperature as Fahrenheit
        float f = dht.readTemperature(true);
              
        // Check if any reads failed and exit early (to try again).
        if (isnan(h) || isnan(t) || isnan(f)) {
            LOG.print("Failed to read from DHT sensor!\r\n");
        }
        // Compute heat index
        // Must send in temp in Fahrenheit!
        //float hi = dht.computeHeatIndex(f, h);
        //float hic = (f - 32) / 1.8;
        //Souliss_ImportAnalog(memory_map, HEAT, &hic);
  	    
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

#endif
