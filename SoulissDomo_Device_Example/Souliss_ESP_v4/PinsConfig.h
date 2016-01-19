#ifndef PinsConfig_h
#define PinsConfig_h

//#define PCB
#define PCBrev2
//#define STRIPBOARD

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
byte BUT0P;
byte BUT1P;
byte THERM_HEATERP;
byte THERM_FAN1P;
byte THERM_FAN2P;
byte THERM_FAN3P;

// **************************************************************************

// Include and Configure DHT11 SENSOR
#ifdef PCBrev2    //PCB NUEVA REV2
  #define DHTPIN      16     // what pin we're connected to
#endif

#ifdef PCB    //PCB NUEVA
  #define DHTPIN      4//2//15     // what pin we're connected to
#endif

#ifdef STRIPBOARD //STRIPBOARD DE PRUEBAS
  #define DHTPIN      13         // what pin we're connected to
#endif

#ifdef PCBrev2    //PCB NUEVA
  #define DALLASPIN   14     //Se declara el pin donde se conectarÃ¡ la DATA
#endif

#ifdef PCB    //PCB NUEVA
  #define DALLASPIN   14     //Se declara el pin donde se conectarÃ¡ la DATA
#endif

#ifdef STRIPBOARD //STRIPBOARD DE PRUEBAS
  #define DALLASPIN   4     //Se declara el pin donde se conectarÃ¡ la DATA
#endif

#define RX 3
#define TX 1

// ******************************************************************************************************************
// *************************************************  PINS CONFIG ***************************************************
// ******************************************************************************************************************
void PINS_CONFIG(){

// *************************************************  PINS PCBrev2 ***************************************************
  #ifdef PCBrev2    //PCB NUEVA
    
    if(L1 == ONOFF_MODE || L1 == PULSE_MODE || L1 == PWM_MODE || L1 == PIR_MODE || L1 == ALARM_MODE){
        LEDPWMP0 = 13;
    }
    if(L2 == ONOFF_MODE || L2 == PULSE_MODE || L2 == PWM_MODE || L2 == PIR_MODE || L2 == ALARM_MODE){
        LEDPWMP1 = 12;
    }
    if(L3 == ONOFF_MODE || L3 == PULSE_MODE || L3 == PWM_MODE || L3 == PIR_MODE || L3 == ALARM_MODE){
        LEDPWMP2 = 15;
    } 

    if(L1 == PIR_MODE  || L1 == ALARM_MODE || L2 == PIR_MODE  || L2 == ALARM_MODE || L3 == PIR_MODE  || L3 == ALARM_MODE){
        PIRP = 3;       //CHANGED PIR TO RX PIN 3 (POWERED WITH 5V)   
    }
       
//    if(ONOFF_MODE || PULSE_MODE || PWM_MODE){
//        LEDPWMP0 = 13;//RX;//13;      //LED STRIP ON PIN //TX BLINK
//        LEDPWMP1 = 12;      //LED STRIP ON PIN 
//        LEDPWMP2 = 15;      //LED STRIP ON PIN 
//    }
//    if(PIR_MODE){
//        LEDPWMP0 = 13;//5;      //LED STRIP ON PIN 12
//        LEDPWMP1 = 12;//16;      //LED STRIP ON PIN 13
//        LEDP = 15;      //LED STRIP ON PIN 15
//        PIRP = 2;       //LED STRIP ON PIN 2
//    }
//    if(ALARM_MODE){
//        ALARMP = 2;
//    }
    if(L1 == RGB_MODE && L2 == RGB_MODE && L3 == RGB_MODE){
        LEDRP = 13;
        LEDGP = 12;
        LEDBP = 15;
    }
    
//    if(RGB_MODE){
//        LEDRP = 13;//5;      //LED STRIP ON PIN 12
//        LEDGP = 12;//16;      //LED STRIP ON PIN 13
//        LEDBP = 15;      //LED STRIP ON PIN 15
//    }
    
    //PIN OPTIONS FOR CAPACITIVE - RELAY OR BMP180

//**************************   REVISAAAAARR!!! *********************//

    if(S4 = 1 || S4 == 2){
        CAP0P = 4;//12;
        CAP1P = 5;//14;
    }
//    if(CAPACITIVE){
//        CAP0P = 4;//12;
//        CAP1P = 5;//14;
//    }
    if(S5 = 1 || S5 == 2 || S5 == 4 || S5 == 4){
        RELAY0P = 4;
        RELAY1P = 5;
    }
//    if(RELAY || PULSE_OUTPUT || GARAGE_DOOR || WINDOW_CURTAIN){
//        RELAY0P = 4;
//        RELAY1P = 5;
//    }
    if(S5 = 3){
        //SDA 5  SCL 4  PINS MUST CHANGE ON WIRE.H
        SCLP = 4;//12;
        SDAP = 5;//14;
    }
//    if(BMP180){
//        //SDA 5  SCL 4  PINS MUST CHANGE ON WIRE.H
//        SCLP = 4;//12;
//        SDAP = 5;//14;
//    }
    if(S4 = 3 || S4 == 4 || S4 == 5 || S4 == 6){
        BUT0P = 4;//TX; //TX FAIL RX OK
        BUT1P = 5;;
    }
//    if(BUTTONS || BUTTONS_PULLUP || ALARM_ENDSTOP || BUTTONS_2_STATE){
//        BUT0P = 4;//TX; //TX FAIL RX OK
//        BUT1P = 5;
//    }

    if(S5 = 6){
        BUT0P = 4;          //Used to OPTO IN
        RELAY1P = 5;        //Used to Relay  
    }
    
//    if(OPTO_AND_RELAY){
//        BUT0P = 4;          //Used to OPTO IN
//        RELAY1P = 5;        //Used to Relay
//    }
    
  #endif
// *************************************************  PINS PCB ***************************************************  
  #ifdef PCB    //PCB NUEVA

	  if(ONOFF_MODE || PULSE_MODE || PWM_MODE){
    	  LEDPWMP0 = 13;      //LED STRIP ON PIN 12
        LEDPWMP1 = 12;      //LED STRIP ON PIN 13
        LEDPWMP2 = 16;      //LED STRIP ON PIN 15
    }
    if(PIR_MODE){
        LEDPWMP0 = 13;//5;      //LED STRIP ON PIN 12
        LEDPWMP1 = 12;//16;      //LED STRIP ON PIN 13
        LEDP = 16;      //LED STRIP ON PIN 15
        PIRP = 0;       //LED STRIP ON PIN 2
    }
    if(ALARM_MODE){
        ALARMP = 0;
    }
    
    if(RGB_MODE){
        LEDRP = 13;      //LED STRIP ON PIN 12
        LEDGP = 12;      //LED STRIP ON PIN 13
        LEDBP = 16;      //LED STRIP ON PIN 15
    }
    
    //PIN OPTIONS FOR CAPACITIVE - RELAY OR BMP180
    if(CAPACITIVE){
        CAP0P = 4;
        CAP1P = 5;
    }
    
    if(RELAY || PULSE_OUTPUT || GARAGE_DOOR || WINDOW_CURTAIN){
        RELAY0P = 4;//12;
        RELAY1P = 5;//14;
    }
    if(BMP180){
        //SDA 5  SCL 4  PINS MUST CHANGE ON WIRE.H
        SCLP = 4;//12;
        SDAP = 5;//14;
    }
    if(BUTTONS || BUTTONS_PULLUP || ALARM_ENDSTOP || BUTTONS_2_STATE){
        BUT0P = 4;
        BUT1P = 5;
    }
    if(OPTO_AND_RELAY){
        BUT0P =   4;          //Used to OPTO IN
        RELAY1P = 5;        //Used to Relay
    }
  #endif
// *************************************************  PINS STRIPBOARD ***************************************************
  #ifdef STRIPBOARD    

    if(ONOFF_MODE || PULSE_MODE || PWM_MODE){
        LEDPWMP0 = 5;      //LED STRIP ON PIN 12
        LEDPWMP1 = 2;      //LED STRIP ON PIN 13
        LEDPWMP2 = 15;     //LED STRIP ON PIN 15
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
    if(THERMOSTAT_MODE){
        THERM_HEATERP = 5;
        THERM_FAN1P = 2;
        THERM_FAN2P = 15;
        THERM_FAN3P = 13;  //USING DHT PIN FOR TESTING PURPOSES 
    }
    
    //PIN OPTIONS FOR CAPACITIVE - RELAY OR BMP180
    if(CAPACITIVE){
        //SDA 5  SCL 4  PINS MUST CHANGE ON WIRE.H
        CAP0P = 12;
        CAP1P = 14;
    }
    
    if(RELAY || PULSE_OUTPUT || GARAGE_DOOR || WINDOW_CURTAIN){
        RELAY0P = 12;
        RELAY1P = 14;
    }
    if(BMP180){
        SCLP = 12;
        SDAP = 14;
    }
    if(BUTTONS || BUTTONS_PULLUP || ALARM_ENDSTOP || BUTTONS_2_STATE){
        BUT0P = 12;
        BUT1P = 14;
    }
    if(OPTO_AND_RELAY){
        BUT0P =   12;          //Used to OPTO IN
        RELAY1P = 14;        //Used to Relay
    }
  #endif

}

#endif
