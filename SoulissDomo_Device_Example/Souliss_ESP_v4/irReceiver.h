#ifndef irReceiver_h
#define irReceiver_h

int RECV_PIN = 2; //an IR detector/demodulatord is connected to GPIO pin 2
IRrecv irrecv(RECV_PIN);
decode_results results;

bool Souliss_IrIn(long remote_button, uint8_t value_state1, uint8_t slot, decode_results *results) {

    if (results->value == remote_button)  
     {
        if(DEBUG_IR) { LOG.print("Souliss_IrIN| Code: "); LOG.print(remote_button); LOG.print(" Cmd: "); LOG.print(value_state1); LOG.print(" Slot: "); LOG.println(slot); }
        if(slot != 254) mInput(slot) = value_state1;
        results->value = 0;
        return true;
     }
     else return false;
}

//+=============================================================================
// Display IR code
//
void ircode (decode_results *results)
{
  // Panasonic has an Address
  if (results->decode_type == PANASONIC) {
    LOG.print(results->panasonicAddress, HEX);
    LOG.print(":");
  }

  // Print Code
  LOG.print(results->value, HEX);
}

void ircodeDEC (decode_results *results)
{

  LOG.print(results->value, DEC);

}

//+=============================================================================
// Display encoding type
//
void  encoding (decode_results *results)
{
  switch (results->decode_type) {
    default:
    case UNKNOWN:      LOG.print("UNKNOWN");       break ;
    case NEC:          LOG.print("NEC");           break ;
    case SONY:         LOG.print("SONY");          break ;
    case RC5:          LOG.print("RC5");           break ;
    case RC6:          LOG.print("RC6");           break ;
    case DISH:         LOG.print("DISH");          break ;
    case SHARP:        LOG.print("SHARP");         break ;
    case JVC:          LOG.print("JVC");           break ;
    case SANYO:        LOG.print("SANYO");         break ;
    case MITSUBISHI:   LOG.print("MITSUBISHI");    break ;
    case SAMSUNG:      LOG.print("SAMSUNG");       break ;
    case LG:           LOG.print("LG");            break ;
    case WHYNTER:      LOG.print("WHYNTER");       break ;
    case AIWA_RC_T501: LOG.print("AIWA_RC_T501");  break ;
    case PANASONIC:    LOG.print("PANASONIC");     break ;
  }
}

//+=============================================================================
// Dump out the decode_results structure.
//
void  dumpInfo (decode_results *results)
{
  // Show Encoding standard
  LOG.print("Encoding  : ");
  encoding(results);
  LOG.println("");

  // Show Code & length
  LOG.print("Code      : ");
  ircode(results);
  LOG.print(" (");
  LOG.print(results->bits, DEC);
  LOG.println(" bits)");
}

//+=============================================================================
// Dump out the decode_results structure.
//
void  dumpRaw (decode_results *results)
{
  // Print Raw data
  LOG.print("Timing[");
  LOG.print(results->rawlen-1, DEC);
  LOG.println("]: ");

  for (int i = 1;  i < results->rawlen;  i++) {
    unsigned long  x = results->rawbuf[i] * USECPERTICK;
    if (!(i & 1)) {  // even
      LOG.print("-");
      if (x < 1000)  LOG.print(" ") ;
      if (x < 100)   LOG.print(" ") ;
      LOG.print(x, DEC);
    } else {  // odd
      LOG.print("     ");
      LOG.print("+");
      if (x < 1000)  LOG.print(" ") ;
      if (x < 100)   LOG.print(" ") ;
      LOG.print(x, DEC);
      if (i < results->rawlen-1) LOG.print(", "); //',' not needed for last one
    }
    if (!(i % 8))  LOG.println("");
  }
  LOG.println("");                    // Newline
}

//+=============================================================================
// Dump out the decode_results structure.
//
void  dumpCode (decode_results *results)
{
  // Start declaration
  LOG.print("unsigned int  ");          // variable type
  LOG.print("rawData[");                // array name
  LOG.print(results->rawlen - 1, DEC);  // array size
  LOG.print("] = {");                   // Start declaration

  // Dump data
  for (int i = 1;  i < results->rawlen;  i++) {
    LOG.print(results->rawbuf[i] * USECPERTICK, DEC);
    if ( i < results->rawlen-1 ) LOG.print(","); // ',' not needed on last one
    if (!(i & 1))  LOG.print(" ");
  }

  // End declaration
  LOG.print("};");  // 

  // Comment
  LOG.print("  // ");
  encoding(results);
  LOG.print(" ");
  ircode(results);

  // Newline
  LOG.println("");

  // Now dump "known" codes
  if (results->decode_type != UNKNOWN) {

    // Some protocols have an address
    if (results->decode_type == PANASONIC) {
      LOG.print("unsigned int  addr = 0x");
      LOG.print(results->panasonicAddress, HEX);
      LOG.println(";");
    }

    // All protocols have data
    LOG.print("unsigned int  data = 0x");
    LOG.print(results->value, HEX);
    LOG.println(";");
  }
}

void readIR(){
    if (irrecv.decode(&results)) {  // Grab an IR code
      if(DEBUG_IR){
        LOG.print(F("Code: "));
        ircodeDEC(&results);
        LOG.println("");
        if(DEBUG_IR_FULL){
          dumpInfo(&results);           // Output the results
          dumpRaw(&results);            // Output the results in RAW format
          dumpCode(&results);           // Output the results as source code
          LOG.println("");
        }  
                   // Blank line between entries
      }
      irrecv.resume();              // Prepare for the next value
    }
}

#endif
