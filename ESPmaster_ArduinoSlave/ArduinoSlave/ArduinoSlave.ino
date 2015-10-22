/* Sketch by Jose Manuel Loureiro 
 * Arduino slave
 * 
 * flash 5556 bytes (17% 328P)
 * RAM    263 bytes (12% 328P)


Recive char y convierte en tres int[3]=  int[0].int[1].int[2].
  *tb hay que mandar el último punto




                    int[0]  int[1]  int[2]
                    
digital write         1     pin       val
digital read          2     pin       --
analog write          3     pin       val
analog read           4     pin       --
configure pin         5     pin       confVal
reset arduino         6     --        --
 *podria ser tb     byte    byte      byte+=byte
 *pero no vale serial monitor ide




confVal
  1   digital in
  2   digital out start low
  3   digital out start high
  4   analog out (PWM)
  5   analog in
**faltaria añadir configuraciones para objetos


uso con IDE serial monitor 
5.13.2.   configura el pin 13 como salida digital que comienza su estado en low
6.0.0.    guarda conf en eeprom y resetea arduino
1.13.1.   enciende el led de pin 13
1.13.0.   apaga el led del pin 13


Integración con SS
Un ESP con el codigo de ss y sin declarar usart
y con serial.begin(9600);
    (...)
    logic(slot)
    if (mOutput==1){Serial.println(1.pin.1.);}
    if (mOutput==0){Serial.println(1.pin.0.);}


Falta:
- procesar petición de pinConf por parte de esp
- antes de resetear comparar si sirve la comfiguración actual
 
 */
#include <EEPROM.h>
#include <avr/wdt.h>//for software reset
/*
#define dw    1    //digital write
#define dr    2    //digital read
#define aw    3    //analog write
#define ar    4    //analog read
#define co    5    //configure pin
#define ReseT 6    //reset arduino
*/
byte pinConf[21];//0,1 serial(not used)
                //D2-D13
                //A1[14]-A7[20]
int ARV[8];//for analog reads
int AWV[6];//for analog writes


String inputString= "";         // a string to hold incoming data
int inputInt[3];
boolean stringComplete = false;  // whether the string is complete
byte index=0;
bool reseT;


void setup() {
  Serial.begin(9600);
  Serial.println("i am here");


//pin conf
  eepromRead();
  for (int i=2;i<21;i++) {
    if(pinConf[i]==1&&i<14){//digital in
      pinMode(i,INPUT);}
    if(pinConf[i]==2&&i<14){//digital out start low
      digitalWrite(i,LOW);
      pinMode(i,OUTPUT);}
    if(pinConf[i]==3&&i<14){//digital out start high
      digitalWrite(i,HIGH);
      pinMode(i,OUTPUT);}
    if(pinConf[i]==4&&(i==3||i==5||i==6||i==9||i==10||i==11)){//analog out
      pinMode(i,OUTPUT);}
    if(pinConf[i]==5&&i>13){//analog in
      pinMode(i,INPUT);}
  }
}


void loop() {


  serialEvent();
   
  if(millis()%15000==0){}//No quitar, sin no funciona
  /*  for (int i=2;i<21;i++) {
      Serial.print(i);
      Serial.print(": ");
      Serial.println(pinConf[i]);
    }
  }*/
  if (stringComplete) {
    for (int i=0;i<3;i++) {
      Serial.print(inputInt[i]);
      Serial.print(",");
    }//for
    Serial.println(":"); 
    switch(inputInt[0]){
      case 1://digital write
        if(1<inputInt[1]<14&&(inputInt[2]==0||inputInt[2]==1)){
          digitalWrite(inputInt[1],inputInt[2]);}
      break;
      case 2://digital read
        if(1<inputInt[1]<14){
          Serial.print("2.");
          Serial.print(inputInt[1]);
          Serial.print(".");
          Serial.print(digitalRead(inputInt[1]));
          Serial.println(".");}
      break;
      case 3://analog write
        if((inputInt[1]==(3||5||6||9||10||11))&&(0<=inputInt[2]<1024)){
          analogWrite(inputInt[1],inputInt[2]);}
      break;
      case 4://analog read
        if(13<inputInt[1]<21){
          Serial.print("4.");
          Serial.print(inputInt[1]);
          Serial.print(".");
          Serial.print(analogRead(inputInt[1]));
          Serial.println(".");}
      break;
      case 5://configure pin
        if((1<inputInt[1]<22)&&(0<inputInt[2]<6)){
          pinConf[inputInt[1]]=inputInt[2];}
      break;
      case 6://reaet
        software_Reboot();
      break;
      default:
      break;
    }//switch
    inputInt[0]=0;
    inputInt[1]=0;
    inputInt[2]=0;
    stringComplete = false;
  }//if stringcomplete
}




//=======================================================
void serialEvent() {


  while (Serial.available()) {
    char inChar = (char)Serial.read();
    if (inChar != '.') {
      inputString += inChar;
      }
    if (inChar == '.') {
      inputInt[index]=inputString.toInt();
      inputString = "";
      index++;
      }
    if (index == 3) {
      index=0;
      stringComplete = true;
    }
  }
}
void eepromRead(){
  for (int i=0;i<21;i++){
    pinConf[i]=EEPROM.read(i);
  }//for
}
void eepromUpdate(){
  for (int i=0;i<21;i++){
    EEPROM.update(i, pinConf[i]);
  }//for
}
void software_Reboot(){
  eepromUpdate();/////////////////
  wdt_enable(WDTO_15MS);
  while(1){}
}
