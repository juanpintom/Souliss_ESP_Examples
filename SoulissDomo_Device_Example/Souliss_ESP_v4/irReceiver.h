#ifndef irReceiver_h
#define irReceiver_h

int RECV_PIN = 2; //an IR detector/demodulatord is connected to GPIO pin 2
IRrecv irrecv(RECV_PIN);
decode_results results;

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

uint8_t Souliss_IrIn(long remote_button, uint8_t value_state1, uint8_t slot, decode_results *results) {

    if (results->value == remote_button)  
     {
        mInput(slot) = value_state1;
        results->value = 0;
        return value_state1;
     }
     else return 0;
}

//+=============================================================================
// Display IR code
//
void ircode (decode_results *results)
{
  // Panasonic has an Address
  if (results->decode_type == PANASONIC) {
    Serial.print(results->panasonicAddress, HEX);
    Serial.print(":");
  }

  // Print Code
  Serial.print(results->value, HEX);
  Serial.print(" ");
  Serial.print(results->value, DEC);

}

//+=============================================================================
// Display encoding type
//
void  encoding (decode_results *results)
{
  switch (results->decode_type) {
    default:
    case UNKNOWN:      Serial.print("UNKNOWN");       break ;
    case NEC:          Serial.print("NEC");           break ;
    case SONY:         Serial.print("SONY");          break ;
    case RC5:          Serial.print("RC5");           break ;
    case RC6:          Serial.print("RC6");           break ;
    case DISH:         Serial.print("DISH");          break ;
    case SHARP:        Serial.print("SHARP");         break ;
    case JVC:          Serial.print("JVC");           break ;
    case SANYO:        Serial.print("SANYO");         break ;
    case MITSUBISHI:   Serial.print("MITSUBISHI");    break ;
    case SAMSUNG:      Serial.print("SAMSUNG");       break ;
    case LG:           Serial.print("LG");            break ;
    case WHYNTER:      Serial.print("WHYNTER");       break ;
    case AIWA_RC_T501: Serial.print("AIWA_RC_T501");  break ;
    case PANASONIC:    Serial.print("PANASONIC");     break ;
  }
}

//+=============================================================================
// Dump out the decode_results structure.
//
void  dumpInfo (decode_results *results)
{
  // Show Encoding standard
  Serial.print("Encoding  : ");
  encoding(results);
  Serial.println("");

  // Show Code & length
  Serial.print("Code      : ");
  ircode(results);
  Serial.print(" (");
  Serial.print(results->bits, DEC);
  Serial.println(" bits)");
}

//+=============================================================================
// Dump out the decode_results structure.
//
void  dumpRaw (decode_results *results)
{
  // Print Raw data
  Serial.print("Timing[");
  Serial.print(results->rawlen-1, DEC);
  Serial.println("]: ");

  for (int i = 1;  i < results->rawlen;  i++) {
    unsigned long  x = results->rawbuf[i] * USECPERTICK;
    if (!(i & 1)) {  // even
      Serial.print("-");
      if (x < 1000)  Serial.print(" ") ;
      if (x < 100)   Serial.print(" ") ;
      Serial.print(x, DEC);
    } else {  // odd
      Serial.print("     ");
      Serial.print("+");
      if (x < 1000)  Serial.print(" ") ;
      if (x < 100)   Serial.print(" ") ;
      Serial.print(x, DEC);
      if (i < results->rawlen-1) Serial.print(", "); //',' not needed for last one
    }
    if (!(i % 8))  Serial.println("");
  }
  Serial.println("");                    // Newline
}

//+=============================================================================
// Dump out the decode_results structure.
//
void  dumpCode (decode_results *results)
{
  // Start declaration
  Serial.print("unsigned int  ");          // variable type
  Serial.print("rawData[");                // array name
  Serial.print(results->rawlen - 1, DEC);  // array size
  Serial.print("] = {");                   // Start declaration

  // Dump data
  for (int i = 1;  i < results->rawlen;  i++) {
    Serial.print(results->rawbuf[i] * USECPERTICK, DEC);
    if ( i < results->rawlen-1 ) Serial.print(","); // ',' not needed on last one
    if (!(i & 1))  Serial.print(" ");
  }

  // End declaration
  Serial.print("};");  // 

  // Comment
  Serial.print("  // ");
  encoding(results);
  Serial.print(" ");
  ircode(results);

  // Newline
  Serial.println("");

  // Now dump "known" codes
  if (results->decode_type != UNKNOWN) {

    // Some protocols have an address
    if (results->decode_type == PANASONIC) {
      Serial.print("unsigned int  addr = 0x");
      Serial.print(results->panasonicAddress, HEX);
      Serial.println(";");
    }

    // All protocols have data
    Serial.print("unsigned int  data = 0x");
    Serial.print(results->value, HEX);
    Serial.println(";");
  }
}

void readIR(){
    if (irrecv.decode(&results)) {  // Grab an IR code
      if(DEBUG_IR){
        dumpInfo(&results);           // Output the results
        //dumpRaw(&results);            // Output the results in RAW format
        //dumpCode(&results);           // Output the results as source code
        Serial.println("");           // Blank line between entries
      }
      irrecv.resume();              // Prepare for the next value
    }
}

#endif
