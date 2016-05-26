#ifndef FunctionsSENSORS_h
#define FunctionsSENSORS_h

DHT dht11(DHTPIN, DHT11, 15);
DHT dht22(DHTPIN, DHT22, 15);

// Light calibration data
// out[] holds the values wanted in lux/10
#define SIZEOF 30
static const unsigned int out[] =   { 0,   1,  2,   5,    7,   10,   15,   20,   30,   40,   50,   60,   70,   80,  100,  150,  200,  250,  350,  450,  600,   800, 1200, 1600, 2000, 2500, 3000, 4000, 5000, 6350};  // x10  
static const unsigned int in[]  =   { 1,  30, 80, 200,  275,  350,  430,  490,  590,  650,  720,  750,  775,  790,  800,  825,  880,  890,  940,  955,  970,  1000, 1003, 1006, 1010, 1013, 1015, 1018, 1021, 1024};

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
// **************************      PCF8591 Module Analog to Digital test program. ***********************************
// ******************************************************************************************************************
//Essentially, this tests the I2C communications to the chip.
//The chip address is 0x90.
//
//#include <Wire.h>
#define PCF8591 (0x90 >> 1)      // Device address = 0       
#define PCF8591_DAC_ENABLE 0x40
#define PCF8591_ADC_CH0 0x40
#define PCF8591_ADC_CH1 0x41
#define PCF8591_ADC_CH2 0x42
#define PCF8591_ADC_CH3 0x43
byte adc_value;

byte getADC(byte config)
{
   Wire.beginTransmission(PCF8591);
   Wire.write(config);
   Wire.endTransmission();
   Wire.requestFrom((int) PCF8591,2);
   while (Wire.available()) 
 {
   adc_value = Wire.read(); //This needs two reads to get the value.
   adc_value = Wire.read();
 }
 return adc_value;
}


#define ADC0 0x00 // control bytes for reading individual ADCs
#define ADC1 0x01
#define ADC2 0x02
#define ADC3 0x03
byte value0, value1, value2, value3;

void ReadPCF()
{
   Wire.beginTransmission(PCF8591); // wake up PCF8591
   Wire.write(ADC0); // control byte - read ADC0
   Wire.endTransmission(); // end tranmission
   Wire.requestFrom(PCF8591, 2);
   value0=Wire.read();
   value0=Wire.read();
   Wire.beginTransmission(PCF8591); // wake up PCF8591
   Wire.write(ADC1); // control byte - read ADC1
   Wire.endTransmission(); // end tranmission
   Wire.requestFrom(PCF8591, 2);
   value1=Wire.read();
   value1=Wire.read();
   Wire.beginTransmission(PCF8591); // wake up PCF8591
   Wire.write(ADC2); // control byte - read ADC2
   Wire.endTransmission(); // end tranmission
   Wire.requestFrom(PCF8591, 2);
   value2=Wire.read();
   value2=Wire.read();
   Wire.beginTransmission(PCF8591); // wake up PCF8591
   Wire.write(ADC3); // control byte - read ADC3
   Wire.endTransmission(); // end tranmission
   Wire.requestFrom(PCF8591, 2);
   value3=Wire.read();
   value3=Wire.read();
   LOG.print(value0); LOG.print(" ");
   LOG.print(value1); LOG.print(" ");
   LOG.print(value2); LOG.print(" ");
   LOG.print(value3); LOG.print(" ");
   LOG.println();
}

// ******************************************************************************************************************
// *************************************************  Energy Meassure FUNCTION ***************************************************
// ******************************************************************************************************************

double offsetI;
double filteredI;
double sqI,sumI;
int16_t sampleI;
double Irms;

double squareRoot(double fg)  
{
  double n = fg / 2.0;
  double lstX = 0.0;
  while (n != lstX)
  {
    lstX = n;
    n = (n + fg / n) / 2.0;
  }
  return n;
}

double calcIrms(unsigned int Number_of_Samples)
{
  /* Be sure to update this value based on the IC and the gain settings! */
  float multiplier = 0.125F;    /* ADS1115 @ +/- 4.096V gain (16-bit results) */
  for (unsigned int n = 0; n < Number_of_Samples; n++)
  {
    //sampleI = ads.readADC_Differential_0_1();
    sampleI = analogRead(A0);

    // Digital low pass filter extracts the 2.5 V or 1.65 V dc offset, 
  //  then subtract this - signal is now centered on 0 counts.
    offsetI = (offsetI + (sampleI-offsetI)/1024);
    filteredI = sampleI - offsetI;
    //filteredI = sampleI * multiplier;

    // Root-mean-square method current
    // 1) square current values
    sqI = filteredI * filteredI;
    // 2) sum 
    sumI += sqI;
  }
  LOG.print("A0: ");
  LOG.print (sampleI);
  Irms = squareRoot(sumI / Number_of_Samples)*multiplier; 

  //Reset accumulators
  sumI = 0;
//--------------------------------------------------------------------------------------       
 
  return Irms;
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


unsigned long timer;

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
      LOG.print(abs(millis()-timer)); 
      LOG.print("\t");  
    }
  
    if(cycles > thresold_value && (InPin[pin] == 10 || InPin[pin] == 11)){
       InPin[pin] = PINSET;
    }
        
    if(cycles > thresold_value && (InPin[pin]==PINRESET))
    {
      timer = millis();               // Record time
      InPin[pin] = PINSET;      
      return InPin[pin];
    }
    else if(cycles > thresold_value && (abs(millis()-timer) > holdtime) && (InPin[pin]==PINSET || InPin[pin]==PINACTIVE))
    {
      if(AUTOCALIBRATE && (abs(millis()-timer) > 15000)){
        //config.cap_thresold = cycles + 4;
        //EEPROM.write(4, cycles + 4);
        //EEPROM.commit();
        if(DEBUG_CAPSENSE) { 
          LOG.print("Autocalibrate: ");
          //LOG.println(config.cap_thresold);
        }
        //return config.cap_thresold;
      }else{
        InPin[pin] = PINACTIVE;               
        if (memory_map) memory_map[MaCaco_IN_s + slot] = value_hold;
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
          memory_map[MaCaco_IN_s + slot] = Souliss_T1n_RstCmd;      // Reset
      }
      else memory_map[MaCaco_IN_s + slot] = value; 
      
      InPin[pin] = PINRESET;     
      return value;
    }
    else if(cycles < thresold_value && InPin[pin] == 11){
      InPin[pin] = PINRESET;
      if(mOutput(slot+1) == 0) mOutput(slot) = Souliss_T1n_OffCoil;
      else                     mOutput(slot) = Souliss_T1n_OnCoil;
      memory_map[MaCaco_IN_s + slot] = Souliss_T1n_RstCmd;      // Reset
    }          
  
    // return MaCaco_NODATACHANGED;

}

//***************************************************************************
// Calculate lux based on rawADC reading from LDR returns value in lux/10
//***************************************************************************
float Souliss_GetLux(const unsigned int* _in, const unsigned int* _out, byte size)
{
  //boolean DEBUG_GETLUX = 1;
        // take care the value is within range
  // val = constrain(val, _in[0], _in[size-1]);
        
  int val = analogRead(A0);
  delay(10);  //TODO: TEST WITH LOWER VALUES

  if(DEBUG_GETLUX){
    LOG.print("AnalogRead: ");
    LOG.println(val);
  }
  if (val < 30){
    if (val > 25) return 0.8;
    if (val > 20) return 0.6;
    if (val > 15) return 0.4;
    if (val > 12) return 0.1;
    if (val > 9) return 0.05;
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
    float h, t, f;  //Variables
    if(!dht_type){  
        h = dht11.readHumidity();
        // Read temperature as Celsius
        t = dht11.readTemperature();
        // Read temperature as Fahrenheit
        f = dht11.readTemperature(true);
    }
    else {
        h = dht22.readHumidity();
        // Read temperature as Celsius
        t = dht22.readTemperature();
        // Read temperature as Fahrenheit
        f = dht22.readTemperature(true);      
    }          
    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t) || isnan(f)) {
        if(DEBUG_DHT) LOG.print("Failed to read from DHT sensor!\r\n");
    }
    // Compute heat index
    // Must send in temp in Fahrenheit!
    //float hi = dht.computeHeatIndex(f, h);
    //float hic = (f - 32) / 1.8;
    //Souliss_ImportAnalog(memory_map, HEAT, &hic);
    if(DEBUG_DHT) {
      LOG.print("DHT Temp: ");
      LOG.print(t);
      LOG.print(" DHT Hum: ");
      LOG.println(h);
    }
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
boolean last_state;

uint8_t SoulissPLC_Read(uint8_t slot, uint8_t button_pin, uint8_t plc_pin, uint8_t relay_pin){
                           
            if(mInput(slot) == Souliss_T1n_OnCmd || mInput(slot) == Souliss_T1n_OffCmd){
                 if(DEBUG_PLC){ LOG.println("PLC Command Received: "); LOG.println(mInput(slot)); } 
                 digitalWrite(relay_pin, !digitalRead(relay_pin)); 
                 memory_map[MaCaco_IN_s + slot] = Souliss_T1n_RstCmd;
                 return 99;  //Return 99 when cmd received from Souliss App Just for test.
            }
  
            if(!digitalRead(button_pin) && InPin[button_pin] == PINRESET) { 
                 InPin[button_pin] = PINSET;
                 if(DEBUG_PLC) LOG.println(InPin[button_pin]);                 
                 return InPin[button_pin];
            }
            if(!digitalRead(button_pin) && InPin[button_pin] == PINSET) { 
                 InPin[button_pin] = PINACTIVE;
                 if(DEBUG_PLC) LOG.println(InPin[button_pin]);                 
                 return InPin[button_pin];
            }
            if(digitalRead(button_pin) && InPin[button_pin] == PINACTIVE) { 
                 InPin[button_pin] = PINRELEASED;
                 if(DEBUG_PLC) LOG.println(InPin[button_pin]);                 
                 return InPin[button_pin];
            }
            if(digitalRead(button_pin) && InPin[button_pin] == PINRELEASED) { 
                 digitalWrite(relay_pin, !digitalRead(relay_pin));
                 InPin[button_pin] = PINRESET;
                 if(DEBUG_PLC) LOG.println(InPin[button_pin]);                 
                 return InPin[button_pin];
            }
            
            
            if(!digitalRead(plc_pin)) {   
                memory_map[MaCaco_OUT_s + slot] = Souliss_T1n_OnCoil; 
                memory_map[MaCaco_IN_s + slot] = Souliss_T1n_RstCmd;                
                if(DEBUG_PLC) FAST_2110ms() { LOG.println("PLC ON"); }
                
            }
            else {
                memory_map[MaCaco_OUT_s + slot] = Souliss_T1n_OffCoil;
                memory_map[MaCaco_IN_s + slot] = Souliss_T1n_RstCmd;                
                if(DEBUG_PLC) FAST_2110ms() { LOG.println("PLC OFF"); }             
            }
}

#endif
