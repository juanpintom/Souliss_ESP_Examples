#ifndef irButtons_h
#define irButtons_h


/******************************** BUTTONS DEFINITIONS *************************/
//This values are long values returned by IRremote library, to get your values enable DEBUG and see results on Serial Port
#define b0  16748655
#define b1  16758855
#define b2  16775175
#define b3  16756815
#define b4  16750695
#define b5  16767015
#define b6  16746615
#define b7  16754775
#define b8  16771095
#define b9  16730295
#define b10 16738455
#define b11 16757325
#define b12 16712445
#define b13 16724685
#define b14 16720095
#define b15 16711935
#define b16 16732335
#define b17 16742535
#define b18 16740495
#define b19 16734375
#define b20 16726215
#define b21 16722135
#define b22 16773135
#define b23 16724175
#define b_Repeat  4294967295

/************************************* COLORES ************************/
#define Rojo          0xFF,0x00,0x00
#define Verde         0x00,0xFF,0x00
#define Azul          0x00,0x00,0xFF
#define Naranja       0xFF,0x5F,0x00
#define Amarillo      0xFF,0xFF,0x00
#define AzulCielo     0x00,0xFF,0xFF
#define AzulOscuro    0x00,0x5F,0xFF
#define Morado        0xFF,0x00,0xFF
#define MoradoSuave   0xFF,0x5F,0xFF
#define Blanco        0xFF,0xFF,0xFF

// ************************** COLORS FOR setBrightColor FUNCTION *******************
#define Red           1
#define Green         2
#define Blue          3

// **********************************  FUNCTION CODES    ****************************
#define ir_ONOFF      1
#define ir_PWM        2
#define ir_RGB        3

byte remote_mode = 0;
                       
/*********************** BUTTON ASSIGNATION COLORS CALL FUNCTION *********************/                        
void irButtons(byte function){  

  // ************************************************************************* 
  // *************************** ir_ONOFF MODE *******************************
  // *************************************************************************
  if(function == ir_ONOFF || function == ir_PWM){
    // ************************    OFF ALL    *****************************    
      if(Souliss_IrIn(b2, 0, 254, &results)){
        mInput(LED1) = Souliss_T1n_OffCmd;
        mInput(LED2) = Souliss_T1n_OffCmd; 
        mInput(LED3) = Souliss_T1n_OffCmd;   
      }
    // ************************    ON ALL    ******************************* 
      if(Souliss_IrIn(b3, 0, 254, &results)){
        
        mInput(LED1) = Souliss_T1n_OnCmd;
        mInput(LED2) = Souliss_T1n_OnCmd; 
        mInput(LED3) = Souliss_T1n_OnCmd; 
      }
    // ************************    TOGGLES    *******************************
      Souliss_IrIn(b11, Souliss_T1n_ToggleCmd, LED1, &results);
      Souliss_IrIn(b15, Souliss_T1n_ToggleCmd, LED2, &results);
      Souliss_IrIn(b19, Souliss_T1n_ToggleCmd, LED3, &results);
      
    // ************************    TIMERS     *******************************
      Souliss_IrIn(b10, Souliss_T1n_Timed_StdVal, LED1, &results);
      Souliss_IrIn(b14, Souliss_T1n_Timed_StdVal, LED2, &results);
      Souliss_IrIn(b18, Souliss_T1n_Timed_StdVal, LED3, &results);
      
  }
// ************************************************************************* 
// ************************************************************************* 

// ************************************************************************* 
// ***************************** ir_PWM MODE *******************************
// *************************************************************************
  if(function == ir_PWM){
    // *********************************************************************
    // **************    BRIGHT UP - DOWN MODE BUTTONS    ******************
    // *********************************************************************
    // ************************    SELECT MODE    **************************
      if(Souliss_IrIn(b23, 0, 0, &results)) 
      { 
         if (remote_mode < 3 ) remote_mode++;
         else remote_mode=0;
         if(DEBUG_IR){ LOG.print("Remote Mode: "); LOG.println(remote_mode); }
      }
   // *******************   BRIGHT CONTROL   *******************************   
      switch (remote_mode) {
          case 0:
              Souliss_IrIn(b0, Souliss_T1n_BrightUp,   LED1, &results);
              Souliss_IrIn(b1, Souliss_T1n_BrightDown, LED1, &results);
              break;
          case 1:
              Souliss_IrIn(b0, Souliss_T1n_BrightUp,   LED2, &results);
              Souliss_IrIn(b1, Souliss_T1n_BrightDown, LED2, &results);
              break;
          case 2:
              Souliss_IrIn(b0, Souliss_T1n_BrightUp,   LED3, &results);
              Souliss_IrIn(b1, Souliss_T1n_BrightDown, LED3, &results);
              break;
          case 3:
        // ************************    BRIGHT UP ALL    ************************
              if(Souliss_IrIn(b0, 0, 254, &results)){
                  mInput(LED1) = Souliss_T1n_BrightUp;
                  mInput(LED2) = Souliss_T1n_BrightUp; 
                  mInput(LED3) = Souliss_T1n_BrightUp; 
              }
        // **********************    BRIGHT DOWN ALL    ************************      
              if(Souliss_IrIn(b1, 0, 254, &results)){
                  mInput(LED1) = Souliss_T1n_BrightDown;
                  mInput(LED2) = Souliss_T1n_BrightDown; 
                  mInput(LED3) = Souliss_T1n_BrightDown;
              }
              break;
      }

      // *********************************************************************
      // ********    BRIGHT UP - DOWN INDIVIDUAL BUTTONS    ******************
      // *********************************************************************

    
      // ************************    BRIGHT UP ALL    ************************
          if(Souliss_IrIn(b4, 0, 254, &results)){
              mInput(LED1) = Souliss_T1n_BrightUp;
              mInput(LED2) = Souliss_T1n_BrightUp; 
              mInput(LED3) = Souliss_T1n_BrightUp; 
          }
      // **********************    BRIGHT DOWN ALL    ************************      
          if(Souliss_IrIn(b1, 0, 254, &results)){
              mInput(LED1) = Souliss_T1n_BrightDown;
              mInput(LED2) = Souliss_T1n_BrightDown; 
              mInput(LED3) = Souliss_T1n_BrightDown;
          }
      // *****************    BRIGHT UP - DOWN BUTTONS    ************************       
          Souliss_IrIn(b8,  Souliss_T1n_BrightUp,   LED1, &results);
          Souliss_IrIn(b9,  Souliss_T1n_BrightDown, LED1, &results);  
          Souliss_IrIn(b12, Souliss_T1n_BrightUp,   LED2, &results);
          Souliss_IrIn(b13, Souliss_T1n_BrightDown, LED2, &results); 
          Souliss_IrIn(b16, Souliss_T1n_BrightUp,   LED3, &results);
          Souliss_IrIn(b17, Souliss_T1n_BrightDown, LED3, &results);       
  }
// ************************************************************************* 
// *************************************************************************  

// ************************************************************************* 
// ***************************** ir_RGB MODE *******************************
// *************************************************************************    
   
  if(function == ir_RGB){
    // *********** COLORES **************
    /*
    #define Rojo          0xFF,0x00,0x00
    #define Verde         0x00,0xFF,0x00
    #define Azul          0x00,0x00,0xFF
    #define Naranja       0xFF,0x5F,0x00
    #define Amarillo      0xFF,0xFF,0x00
    #define AzulCielo     0x00,0xFF,0xFF
    #define AzulOscuro    0x00,0x5F,0xFF
    #define Morado        0xFF,0x00,0xFF
    #define MoradoSuave   0xFF,0x5F,0xFF
    #define Blanco        0xFF,0xFF,0xFF
    */
    Souliss_IrIn(b0, Souliss_T1n_BrightUp, LEDRGB, &results);
    Souliss_IrIn(b1, Souliss_T1n_BrightDown, LEDRGB, &results);
          
    if(Souliss_IrIn(b23, 0, 0, &results)) 
    { 
         if (remote_mode == 0) remote_mode=1;
         else remote_mode=0;
    }
    
    if(Souliss_IrIn(b16, 0, LEDRGB, &results)) 
    { 
        if (remote_mode == 0 ) setColor(LEDRGB, Naranja);
        else setBrightColor(LEDRGB, Red, Souliss_T1n_BrightUp);
    }
    if(Souliss_IrIn(b17, 0, LEDRGB, &results)) 
    { 
        if (remote_mode == 0) setColor(LEDRGB, Azul);
        else setBrightColor(LEDRGB, Green, Souliss_T1n_BrightUp);    
    }
    if(Souliss_IrIn(b18, 0, LEDRGB, &results)) 
    { 
        if (remote_mode == 0 ) setColor(LEDRGB, Morado);       
        else setBrightColor(LEDRGB, Blue, Souliss_T1n_BrightUp);                            
    }
    if(Souliss_IrIn(b20, 0, LEDRGB, &results)) 
    { 
        if (remote_mode == 0 ) setColor(LEDRGB, Amarillo);   
        else setBrightColor(LEDRGB, Red, Souliss_T1n_BrightDown);                         
    }
    if(Souliss_IrIn(b21, 0, LEDRGB, &results)) 
    { 
        if (remote_mode == 0 ) setColor(LEDRGB, Blanco);                            
        else setBrightColor(LEDRGB, Green, Souliss_T1n_BrightDown);
      
    }
    if(Souliss_IrIn(b22, 0, LEDRGB, &results)) 
    { 
        if (remote_mode == 0 ) setColor(LEDRGB, MoradoSuave);                      
        else setBrightColor(LEDRGB, Blue, Souliss_T1n_BrightDown);                            
    }
  }
// ************************************************************************* 
// ************************************************************************* 

}

#endif
                      
