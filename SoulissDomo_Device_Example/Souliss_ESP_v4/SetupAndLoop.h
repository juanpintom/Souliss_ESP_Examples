#ifndef SetupAndLoop_h
#define SetupAndLoop_h

// ***************************************************************************
// *                         setupGeneral FUNCTION                           *
// ***************************************************************************
void setupGeneral(){
  
    EEPROM_CONFIG();
    SLOT_CONFIG();
    PINS_CONFIG();
    analogWriteFreq(500);
    analogWriteRange(255);
//**************************** SENSORS INITIALIZE *****************************
    if(DHT_SENSOR){
        dht.begin();
    }
    
    if(DALLAS_SENSOR){
        sensors.begin(); //Se inician los sensores DS18B20
    }
    
//**************************** SETUP SLOTS *****************************
    if(DHT_SENSOR){
        Set_Temperature(TEMPERATURE);
        Set_Humidity(HUMIDITY);
    }
    
    if(PWM_MODE || PIR_MODE){
        Set_DimmableLight(LEDPWM0);
        Set_DimmableLight(LEDPWM1);  
    }
    
    if(PWM_MODE){
        Set_DimmableLight(LEDPWM2);
    }
    
    if(PIR_MODE){
        Set_AutoLight(LED);
    }
    if(ALARM_MODE){
        Set_Antitheft_Main(ALARM);
    }
    
    if(RGB_MODE){
        Set_LED_Strip(LEDRGB);
    }        
    if(THERMOSTAT_MODE){
        Set_Thermostat(THERMOSTAT);
    
    }
    if(RELAY){
        Set_SimpleLight(RELAY0);
        Set_SimpleLight(RELAY1);
    }
    
    if(LDR_SENSOR){
        Set_T54(LDR);
    }
    
    if(DALLAS_SENSOR){
        Set_Temperature(DALLAS);
    }        

    if(BMP180){
        //Set_T51(PRESSURE0);
        Set_Pressure(PRESSURE0);
        Set_Temperature(BMP180TEMP);
    }
    
    if(CAPACITIVE && DEBUG_CAPSENSE){
        Set_T51(CAP0);
        Set_T51(CAP1);
        Set_T51(THRE);
    }
  
    if(RELAY){
        digitalWrite(RELAY0P,LOW);
        digitalWrite(RELAY1P,LOW);
        pinMode(RELAY0P, OUTPUT);                 
        pinMode(RELAY1P, OUTPUT);
    }
    
    if(PWM_MODE || PIR_MODE){
        pinMode(LEDPWMP0, OUTPUT);
        pinMode(LEDPWMP1, OUTPUT);        
    }
    
    if(PWM_MODE){
        pinMode(LEDPWMP2, OUTPUT);
    }
        
    if(PIR_MODE){
        pinMode(PIRP, INPUT);
        pinMode(LEDP, OUTPUT);
        digitalWrite(LEDP,LOW);
        
        //AutoMode Enabled at Start
        SetInput(LED, Souliss_T1n_AutoCmd);
        Logic_AutoLight(LED);
        nDigOut(LEDP, Souliss_T1n_Coil, LED);
    }
    
    if(RGB_MODE){
            pinMode(LEDRP, OUTPUT);
            pinMode(LEDGP, OUTPUT);
            pinMode(LEDBP, OUTPUT);
    }
    if(THERMOSTAT_MODE){
            pinMode(THERM_HEATERP, OUTPUT);
            pinMode(THERM_FAN1P, OUTPUT);
            pinMode(THERM_FAN1P, OUTPUT);  
            pinMode(THERM_FAN2P, OUTPUT);          
    }
        
        
    if(BMP180){
      //Wire.begin(SDAP,SCLP);
      delay(200);
        if (pressure.begin())
            LOG.print(F("BMP180 init success\r\n"));
        else
        {
            // Oops, something went wrong, this is usually a connection problem,
            // see the comments at the top of this sketch for the proper connections.
            LOG.print(F("BMP180 init fail\r\n"));
        }
    }
    
   

}

void fastGeneral(){
  
      FAST_50ms() {   // We process the logic and relevant input and output every 50 milliseconds
            if(RELAY){
                Logic_SimpleLight(RELAY0);
                DigOut(RELAY0P, Souliss_T1n_Coil,RELAY0);
            
                Logic_SimpleLight(RELAY1);
                DigOut(RELAY1P, Souliss_T1n_Coil,RELAY1);
                
            }

            if(PWM_MODE || PIR_MODE){
                if(CAPACITIVE){
                    Souliss_CapSense(LEDPWM0,Souliss_T1n_ToggleCmd,Souliss_T1n_BrightSwitch, CAP0P, cap_thresold, 1500);
                    Souliss_CapSense(LEDPWM1,Souliss_T1n_ToggleCmd,Souliss_T1n_BrightSwitch, CAP1P, cap_thresold, 1500);
                    if(DEBUG_CAPSENSE_ALL) LOG.println("");
                }                
                
                Logic_DimmableLight(LEDPWM0);                        
                //analogWrite(LEDPWMP0, mOutput(LEDPWM0+1)*4);
                analogWrite(LEDPWMP0, mOutput(LEDPWM0+1));                
                
                Logic_DimmableLight(LEDPWM1);                        
                //analogWrite(LEDPWMP1, mOutput(LEDPWM1+1)*4);
                analogWrite(LEDPWMP1, mOutput(LEDPWM1+1));
            }

            if(PIR_MODE){
                if( ALARM_MODE && mOutput(ALARM) == Souliss_T4n_Antitheft ){   //  IF Alarm is Armed
                   DigIn(PIRP,Souliss_T4n_Alarm,ALARM);                     //  Handle the PIR to the Antitheft               
                }else {                                                        //  IF Alarm isnt Armed
                   DigIn(PIRP,LIGHT_ON_CYCLE+1,LED);                       //  Handle the PIR to the Autolight    
                }
                if(ALARM_MODE) {
                  Logic_Antitheft_Main(ALARM);
                }
               
                Logic_AutoLight(LED);
                
                nDigOut(LEDP, Souliss_T1n_Coil,LED);
            }
            
            if(PWM_MODE){
                Logic_DimmableLight(LEDPWM2);                        
                //analogWrite(LEDPWMP2, mOutput(LEDPWM2+1)*4);
                analogWrite(LEDPWMP2, mOutput(LEDPWM2+1));
            }
            
            if(RGB_MODE){
                if(CAPACITIVE){
                    Souliss_CapSense(LEDRGB,Souliss_T1n_ToggleCmd,Souliss_T1n_BrightSwitch,CAP0P, cap_thresold, 1500);
                    Souliss_CapSense(LEDRGB,Souliss_T1n_ToggleCmd,Souliss_T1n_BrightSwitch,CAP1P, cap_thresold, 1500);
                }  
                Logic_LED_Strip(LEDRGB);
                //analogWrite(LEDRP, mOutput(LEDRGB+1)*4);
                //analogWrite(LEDGP, mOutput(LEDRGB+2)*4);
                //analogWrite(LEDBP, mOutput(LEDRGB+3)*4);
                analogWrite(LEDRP, mOutput(LEDRGB+1));
                analogWrite(LEDGP, mOutput(LEDRGB+2));
                analogWrite(LEDBP, mOutput(LEDRGB+3));
            }
            if(THERMOSTAT_MODE){
                Logic_Thermostat(THERMOSTAT);
            
                // Start the heater and the fans
                nDigOut(THERM_HEATERP, Souliss_T3n_HeatingOn, THERMOSTAT);   // Heater
                nDigOut(THERM_FAN1P, Souliss_T3n_FanOn1   , THERMOSTAT);   // Fan1
                nDigOut(THERM_FAN2P, Souliss_T3n_FanOn2   , THERMOSTAT);   // Fan2
                nDigOut(THERM_FAN3P, Souliss_T3n_FanOn3   , THERMOSTAT);   // Fan3
                
                // We are not handling the cooling mode, if enabled by the user, force it back
                // to disable
                if(mOutput(THERMOSTAT) & Souliss_T3n_CoolingOn)
                    mOutput(THERMOSTAT) &= ~Souliss_T3n_CoolingOn;
                }
                
        }
        
        FAST_110ms(){
          if(CAPACITIVE && DEBUG_CAPSENSE){
            float temp;
            temp = readCapacitivePin(CAP0P);
            if(temp>0) Souliss_ImportAnalog(memory_map, CAP0, &temp);       
            temp = readCapacitivePin(CAP1P);
            if(temp>0) Souliss_ImportAnalog(memory_map, CAP1, &temp);       
            temp = cap_thresold;
            Souliss_ImportAnalog(memory_map, THRE, &temp);       
          }          
        }

        FAST_510ms(){
           if(CAPACITIVE && DEBUG_CAPSENSE){           
              Read_T51(CAP0);
              Read_T51(CAP1);
              Read_T51(THRE);
           }
        }

        FAST_910ms()    {
            if(DALLAS){ 
                  // Acquire temperature from the microcontroller ADC
                  sensors.requestTemperatures(); //Prepara el sensor para la lectura
                  float dallas = sensors.getTempCByIndex(0);
                  if(DEBUG_DALLAS){
                    LOG.print("Dallas: ");
                    LOG.println(dallas);
                  }
                  String dallasstring = "dallas";
                  SendEmoncms("Dallas_Sensor", DALLAS);
                  Souliss_ImportAnalog(memory_map, DALLAS, &dallas);
                  if(THERMOSTAT_MODE){
                     Souliss_ImportAnalog(memory_map, THERMOSTAT+1, &dallas);  //IMPORTED FROM DALLAS SENSOR FOR NOW     
                  }
            } 
            
        }
        FAST_2110ms()
        {
            if(DHT_SENSOR){
                  Logic_Temperature(TEMPERATURE);
                  Logic_Humidity(HUMIDITY);
            }
            
            if(LDR_SENSOR){
                  Logic_T54(LDR);
            }
            
            if(DALLAS_SENSOR){
                  Logic_Temperature(DALLAS);
            }                  
            
            if(BMP180){
    //              Read_T51(PRESSURE0);
                  Logic_Pressure(PRESSURE0);  
                  Logic_Temperature(BMP180TEMP);
            }
        } 
        
        FAST_7110ms()
        { 
            if(LDR_SENSOR){
                float ldr_read = Souliss_GetLux(in, out, SIZEOF)*10.0;  //ORIGINAL
                if(DEBUG_GETLUX){
                    LOG.print("ldrlead: "); 
                    LOG.print(ldr_read);
                }
                if (ldr_read == 0) ldr_read = 0.01;
                Souliss_ImportAnalog(memory_map, LDR, &ldr_read);
            }
        }
        
        FAST_x10ms(300){
            //#if defined (MaCaco_DEBUG || VNET_DEBUG)
            /*  if(DHT_SENSOR){  
                LOG.print("Hum: "); 
                LOG.print(mOutputAsFloat(TEMPERATURE));
                LOG.print(" %\t");
                LOG.print("Temp: "); 
                LOG.print(mOutputAsFloat(HUMIDITY));
                LOG.print(" *C\t");
              }
              
              if(DALLAS_SENSOR){
                LOG.print("Dallas: ");
                LOG.print(mOutputAsFloat(DALLAS));
                LOG.print(" *C\t");             
              }
              
              if(LDR_SENSOR){
                LOG.print("Lux: ");
                LOG.print(mOutputAsFloat(LDR));
                LOG.print(" lux\t"); 
              }
              
              if(BMP180){
                LOG.print("Pressure: ");  
                LOG.print(mOutputAsFloat(PRESSURE0)); 
                LOG.print(" mb\t");  
                LOG.print("TempBMP180: ");  
                LOG.print(mOutputAsFloat(BMP180TEMP)); 
                LOG.print(" *C");           
              }
                LOG.print("\r\n");
            //#endif*/
        }

}

void slowGeneral(){
  
    SLOW_10s() {  // Read temperature and humidity from DHT every 110 seconds  
        if(RELAY){
            Timer_SimpleLight(RELAY0);
            Timer_SimpleLight(RELAY1);
        }
        
        if(PIR_MODE || PWM_MODE){
            Timer_DimmableLight(LEDPWM0);              
            Timer_DimmableLight(LEDPWM1);              
        }
        
        if(PIR_MODE){
            Timer_AutoLight(LED);
        }
        
        if(ALARM_MODE){
            Timer_Antitheft_Main(ALARM);
        }
        
        if(PWM_MODE){
            Timer_DimmableLight(LEDPWM2);                          
        }
        
        if(RGB_MODE){
            Timer_LED_Strip(LEDRGB);
        }
    }            
    SLOW_x10s(2) {

        if(DHT_SENSOR){
            Souliss_GetDHT(TEMPERATURE, HUMIDITY, true);  
        }
        
        if(BMP180){
            Souliss_GetPressure_BMP180(PRESSURE0,BMP180TEMP);	
        }    
        
    }

}

#endif
