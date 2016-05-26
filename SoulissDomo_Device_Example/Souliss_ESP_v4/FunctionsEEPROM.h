#ifndef FunctionsEEPROM_h
#define FunctionsEEPROM_h

void deleteEEPROM(){
  // **** FUNCTION TO DELETE JUST ADDRESSES (MORE THAN 5sec) or ALL THE EEPROM DATA (MORE THAN 10sec) *** 
//  STILL DISABLED, TESTING
    //EEPROM.begin(512);
    DEBUG.println("");
    DEBUG.println("Time to Reset");
    delay(1000);
    long previous = millis();
    pinMode(0, INPUT);
    if(!digitalRead(0)) DEBUG.println("GPIO0 PRESSED!");
    pinMode(15, OUTPUT);
    while(!digitalRead(0)){
      if(millis() < previous + 5000){
        DEBUG.print("Deleting Addresses in: ");
        DEBUG.println(5000 - (millis() - previous));
        digitalWrite(13, !digitalRead(13));
        delay(500);
      }else{
        for(int i = STORE__ADDR_s; i <= STORE__PADDR_f; i++){
          EEPROM.write(i,0);
        }
        EEPROM.commit();
        DEBUG.println("Address Deleted");
        // DELETE EEPROM IF GPIO STILL PRESSED
        if(millis() < previous + 10000){
          DEBUG.print("Deleting Custom Data in: ");
          DEBUG.println(10000 - (millis() - previous));
          delay(500);
        }else{
          //for(int i = STORE_CUSTOM; i <= 512; i++){
          for(int i = 0; i <= 512; i++){  
            EEPROM.write(i,255);
          }
        EEPROM.commit();
        DEBUG.println("Custom Data Deleted");
        digitalWrite(13, !digitalRead(13));
        delay(200);
        }
      }
    } 
}


void WriteConfig_Slots() {
  DEBUG.println("Writing Config");
  bitWrite(OPTIONS1, 0, usartbridge);
  bitWrite(OPTIONS1, 1, IR_ENABLE);
  bitWrite(OPTIONS1, 2, dht_type);
  bitWrite(OPTIONS1, 3, Send_Emon);
  bitWrite(OPTIONS1, 4, DEBUG_LOG);
//  bitWrite(OPTIONS1, 5, DEBUG_CAPSENSE);
//  bitWrite(OPTIONS1, 6, DEBUG_CAPSENSE_ALL);
//  bitWrite(OPTIONS1, 7, DEBUG_DHT);
//
//  bitWrite(OPTIONS2, 0, DEBUG_PRESSURE);
//  bitWrite(OPTIONS2, 1, DEBUG_GETLUX);
//  bitWrite(OPTIONS2, 2, DEBUG_DALLAS);
//  bitWrite(OPTIONS2, 3, DEBUG_EMONCMS);
//  bitWrite(OPTIONS2, 4, DEBUG_IR);
//  bitWrite(OPTIONS2, 5, DEBUG_IR_FULL);
//  bitWrite(OPTIONS2, 6, DEBUG_PLC);
  //bitWrite(OPTIONS2, 7, DEBUG_PLC);

  if(L1PIR == 0){
    L1PIREN = 0;
    bitWrite(OPTIONS3, 0, L1PIREN);
    bitWrite(OPTIONS3, 1, 0);  
  } else if(L1PIR == 1){
      L1PIREN = 1;
      bitWrite(OPTIONS3, 0, L1PIREN);
      bitWrite(OPTIONS3, 1, 0);  
    } else if(L1PIR == 2){
      L1PIREN = 1;
      bitWrite(OPTIONS3, 0, L1PIREN);
      bitWrite(OPTIONS3, 1, 1);  
    }
  if(L2PIR == 0){
    L2PIREN = 0;
    bitWrite(OPTIONS3, 2, L2PIREN);
    bitWrite(OPTIONS3, 3, 0);  
  } else if(L2PIR == 1){
      L2PIREN = 1;
      bitWrite(OPTIONS3, 2, L2PIREN);
      bitWrite(OPTIONS3, 3, 0);  
    } else if(L2PIR == 2){
      L2PIREN = 1;
      bitWrite(OPTIONS3, 2, L2PIREN);
      bitWrite(OPTIONS3, 3, 1);  
    }
  if(L3PIR == 0){
    L3PIREN = 0;
    bitWrite(OPTIONS3, 4, L3PIREN);
    bitWrite(OPTIONS3, 5, 0);  
  } else if(L3PIR == 1){
      L3PIREN = 1;
      bitWrite(OPTIONS3, 4, L3PIREN);
      bitWrite(OPTIONS3, 5, 0);  
    } else if(L3PIR == 2){
      L3PIREN = 1;
      bitWrite(OPTIONS3, 4, L3PIREN);
      bitWrite(OPTIONS3, 5, 1);  
    } 
    
  L1PIREN =       bitRead(OPTIONS3, 0);
  L1PIR =         bitRead(OPTIONS3, 1);
  L2PIREN =       bitRead(OPTIONS3, 2);
  L2PIR =         bitRead(OPTIONS3, 3);
  L3PIREN =       bitRead(OPTIONS3, 4);
  L3PIR =         bitRead(OPTIONS3, 5);
  
  DEBUG.print("PIR_Write:" );
  DEBUG.print(L1PIREN);
  DEBUG.print(L1PIR);
  DEBUG.print(L2PIREN);
  DEBUG.print(L2PIR);
  DEBUG.print(L3PIREN);
  DEBUG.println(L3PIR);
//       
//  bitWrite(OPTIONS3, 0, L1PIREN);
//  bitWrite(OPTIONS3, 1, L1PIR);
//  bitWrite(OPTIONS3, 2, L2PIREN);
//  bitWrite(OPTIONS3, 3, L2PIR);
//  bitWrite(OPTIONS3, 4, L3PIREN);
//  bitWrite(OPTIONS3, 5, L3PIR);
  bitWrite(OPTIONS3, 6, ALARM_MODE);
  bitWrite(OPTIONS3, 7, cap_debug);
      
//  bitWrite(OPTIONS3, 0, L1PIR);
//  bitWrite(OPTIONS3, 1, L2PIR);
//  bitWrite(OPTIONS3, 2, L3PIR);
//  bitWrite(OPTIONS3, 3, ALARM_MODE);
//  bitWrite(OPTIONS3, 4, cap_debug);
//  bitWrite(OPTIONS3, 5, DEBUG_IR_FULL);
//  bitWrite(OPTIONS3, 6, DEBUG_PLC);
//  bitWrite(OPTIONS3, 7, DEBUG_PLC);  
  
  
  EEPROM.write(STORE_CUSTOM,    cap_thresold);
  EEPROM.write(STORE_CUSTOM+1,  L1);
  EEPROM.write(STORE_CUSTOM+2,  L2);
  EEPROM.write(STORE_CUSTOM+3,  L3);
  EEPROM.write(STORE_CUSTOM+4,  S1);
  EEPROM.write(STORE_CUSTOM+5,  S2);
  EEPROM.write(STORE_CUSTOM+6,  S3);
  EEPROM.write(STORE_CUSTOM+7,  S41);
  EEPROM.write(STORE_CUSTOM+8,  S42);
  EEPROM.write(STORE_CUSTOM+9,  S51);
  EEPROM.write(STORE_CUSTOM+10,  S52);  
  EEPROM.write(STORE_CUSTOM+11,  S6);
  EEPROM.write(STORE_CUSTOM+12, OPTIONS1);
  EEPROM.write(STORE_CUSTOM+13, OPTIONS2);
  EEPROM.write(STORE_CUSTOM+14, OPTIONS3);
  EEPROM.write(STORE_CUSTOM+15, ALTITUDE/20);
  Store_String(STORE_CUSTOM+16, DeviceName);     //MAX 10 
  Store_String(STORE_CUSTOM+26, API);      //MAX 32    480
  EEPROM.write(STORE_CUSTOM+59, dallas_qty);
  EEPROM.commit();
  //EEPROM.end();
}

void ReadConfig_Slots()
{

  //EEPROM.begin(512);
  cap_thresold = EEPROM.read(STORE_CUSTOM);
  L1 =        EEPROM.read(STORE_CUSTOM+1);
  L2 =        EEPROM.read(STORE_CUSTOM+2);
  L3 =        EEPROM.read(STORE_CUSTOM+3);
  S1 =        EEPROM.read(STORE_CUSTOM+4);
  S2 =        EEPROM.read(STORE_CUSTOM+5);
  S3 =        EEPROM.read(STORE_CUSTOM+6);
  S41 =       EEPROM.read(STORE_CUSTOM+7);
  S42 =       EEPROM.read(STORE_CUSTOM+8);
  S51 =       EEPROM.read(STORE_CUSTOM+9);
  S52 =       EEPROM.read(STORE_CUSTOM+10);
  S6 =        EEPROM.read(STORE_CUSTOM+11);
  OPTIONS1 =  EEPROM.read(STORE_CUSTOM+12);
  OPTIONS2 =  EEPROM.read(STORE_CUSTOM+13);
  OPTIONS3 =  EEPROM.read(STORE_CUSTOM+14);  
  ALTITUDE = EEPROM.read(STORE_CUSTOM+15)*20;
  DeviceName = Return_String(STORE_CUSTOM+16,10);
  API = Return_String(STORE_CUSTOM+26,32);
  dallas_qty = EEPROM.read(STORE_CUSTOM+59);
  
  usartbridge = bitRead(OPTIONS1, 0);
  IR_ENABLE =   bitRead(OPTIONS1, 1);
  dht_type =    bitRead(OPTIONS1, 2);
  Send_Emon =   bitRead(OPTIONS1, 3);
  
  DEBUG_LOG =           bitRead(OPTIONS1, 4);

  //DEBUG_LOG = 1;   //FORCE DEBUG ENABLED IN CASE OF TOTAL FAIL
  
//  DEBUG_CAPSENSE =      bitRead(OPTIONS1, 5);
//  DEBUG_CAPSENSE_ALL =  bitRead(OPTIONS1, 6);
//  DEBUG_DHT =           bitRead(OPTIONS1, 7);
//
//  DEBUG_PRESSURE =  bitRead(OPTIONS2, 0);
//  DEBUG_GETLUX =    bitRead(OPTIONS2, 1);
//  DEBUG_DALLAS =    bitRead(OPTIONS2, 2);
//  DEBUG_EMONCMS =   bitRead(OPTIONS2, 3);
//  DEBUG_IR =        bitRead(OPTIONS2, 4);
//  DEBUG_IR_FULL =   bitRead(OPTIONS2, 5);
//  DEBUG_PLC =       bitRead(OPTIONS2, 6);
//  //DEBUG_LOG =     bitRead(OPTIONS2, 7);

  
  L1PIREN =       bitRead(OPTIONS3, 0);
  L1PIR =         bitRead(OPTIONS3, 1);
  L2PIREN =       bitRead(OPTIONS3, 2);
  L2PIR =         bitRead(OPTIONS3, 3);
  L3PIREN =       bitRead(OPTIONS3, 4);
  L3PIR =         bitRead(OPTIONS3, 5);
  ALARM_MODE =    bitRead(OPTIONS3, 6);
  cap_debug =     bitRead(OPTIONS3, 7);
  
  if(!L1PIREN) L1PIR = 0;
  else if(!L1PIR) L1PIR = 1;
  else L1PIR = 2;

  if(!L2PIREN) L2PIR = 0;
  else if(!L2PIR) L2PIR = 1;
  else L2PIR = 2;

  if(!L3PIREN) L3PIR = 0;
  else if(!L3PIR) L3PIR = 1;
  else L3PIR = 2;
     
}

void showConfig(){
  DEBUG.println(F("Reading Configuration"));
  DEBUG.print("PIR_Read:");
  DEBUG.print(L1PIREN);
  DEBUG.print(L1PIR);
  DEBUG.print(L2PIREN);
  DEBUG.print(L2PIR);
  DEBUG.print(L3PIREN);
  DEBUG.println(L3PIR);
  
  //*** DEBUG TO SERIAL ***
  DEBUG.print(F("DeviceName: "));
  DEBUG.println(DeviceName);
  DEBUG.print(F("API: "));
  DEBUG.println(API);
  DEBUG.print(F("L1: "));
  DEBUG.println(L1);
  DEBUG.print(F("L1PIR: "));
  DEBUG.println(L1PIR);
  DEBUG.print(F("L2: "));
  DEBUG.println(L2);
  DEBUG.print(F("L2PIR: "));
  DEBUG.println(L2PIR);  
  DEBUG.print(F("L3: "));
  DEBUG.println(L3);
  DEBUG.print(F("L3PIR: "));
  DEBUG.println(L3PIR);  
  DEBUG.print(F("S1: "));
  DEBUG.println(S1);
  DEBUG.print(F("S2: "));
  DEBUG.println(S2);
  DEBUG.print(F("S3: "));
  DEBUG.println(S3);
  DEBUG.print(F("S41: "));
  DEBUG.println(S41);
  DEBUG.print(F("S42: "));
  DEBUG.println(S42);
  DEBUG.print(F("S51: "));
  DEBUG.println(S51);
  DEBUG.print(F("S52: "));
  DEBUG.println(S52);  
  DEBUG.print(F("S6: "));
  DEBUG.println(S6);
  DEBUG.print(F("OPTIONS1: "));
  DEBUG.println(OPTIONS1);
  DEBUG.print(F("OPTIONS2: "));
  DEBUG.println(OPTIONS2); 
  DEBUG.print(F("OPTIONS3: "));
  DEBUG.println(OPTIONS3);  
  DEBUG.print(F("usartbridge: "));
  DEBUG.println(usartbridge);
  DEBUG.print(F("IR_ENABLE: "));
  DEBUG.println(IR_ENABLE);
  DEBUG.print(F("dht_type: "));
  DEBUG.println(dht_type);
  DEBUG.print(F("Send Emon: "));
  DEBUG.println(Send_Emon);
  DEBUG.print(F("Dallas_qty: "));
  DEBUG.println(dallas_qty);
  DEBUG.print(F("cap_debug: "));
  DEBUG.println(cap_debug);  
  DEBUG.print(F("cap_thresold: "));
  DEBUG.println(cap_thresold);
  DEBUG.print(F("ALARM_MODE: "));
  DEBUG.println(ALARM_MODE);
  DEBUG.print(F("DEBUG_LOG: "));
  DEBUG.println(DEBUG_LOG); 
  
}

boolean defaultValues(){
  if(L1 == 255 && dallas_qty == 255){
    cap_thresold = 5;
    L1 = 2;
    L2 = 2;
    L3 = 2;
    S1 = 0;
    S2 = 0;
    S3 = 0;
    S41 = 0;
    S42 = 0;
    S51 = 0;
    S52 = 0;
    S6 = 0;
    
    ALTITUDE = 1;
    DeviceName = "";
    API = "";
    dallas_qty = 1;
  
    usartbridge = 0;
    IR_ENABLE = 0;
    dht_type = 1;
    Send_Emon = 0;
    
    L1PIREN = 0;
    L1PIR =   0;
    L2PIREN = 0;
    L2PIR =   0;
    L3PIREN = 0;
    L3PIR =   0;
    ALARM_MODE = 0;
    DEBUG_LOG = 1;
    cap_debug = 0;  
    WriteConfig_Slots();
    ReadConfig_Slots();
    return 1;
  } 
  return 0;
}

byte STATE;

void StoreSTATE() {

  bitWrite(STATE, 0, mOutput(LED1));
  bitWrite(STATE, 1, mOutput(LED2));
  bitWrite(STATE, 2, mOutput(LED3));
  bitWrite(STATE, 3, mOutput(RELAY0));
  bitWrite(STATE, 4, mOutput(RELAY1));
  //bitWrite(STATE, 5, mOutput(PULSE0));
  //bitWrite(STATE, 6, mOutput(PULSE0));
  bitWrite(STATE, 7, mOutput(OPTO));
  
  EEPROM.write(STORE_CUSTOM+60,STATE);
  EEPROM.commit();

}

void ReadSTATE() {

  STATE = EEPROM.read(STORE_CUSTOM+60);
  mOutput(LED1) =    bitRead(STATE, 0);
  mOutput(LED2) =    bitRead(STATE, 1);
  mOutput(LED3) =    bitRead(STATE, 2);
  mOutput(RELAY0) = bitRead(STATE, 3);
  mOutput(RELAY1) = bitRead(STATE, 4);
  //mOutput(PULSE0) = bitRead(STATE, 5);
  //mOutput(PULSE0) = bitRead(STATE, 6);
  mOutput(OPTO)   =   bitRead(STATE, 7);

}

#endif
