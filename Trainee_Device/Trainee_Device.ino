#include <SPI.h>
#include <LoRa.h>
#include <Nextion.h>
#define LED1   A0
#define LED2   A1
#define LED3   A2     //A2
#define ACKBUT A3
#define BUZZER 5

enum decimal {
  NO,
  CURRENT,
  SET
};

int gas[3]={0,0,0}; //CH4 O2 CO
decimal gasPoint[3];

char val[50] = {0};

byte localAddress = 0xBB;     // address of this device
byte destination = 0xBC;
unsigned long rssiMillis;

unsigned long startMillis;
unsigned long currentMillis;
const unsigned long period = 500 ; //the period of blinking LEDs/buzzer

boolean  oldSwitch  =   LOW;
boolean  newSwitch  =   LOW; // toogle switch

boolean  alarmFlag1 =   false; //buzzer on when true
boolean  alarmFlag2 =   false;
boolean  ackflag    =   false; //stop buzzer
boolean  priorityCheck  = false;

void serialEnd() {
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);
}

void sendData(){
  if(millis() > rssiMillis + 1000){
      serialEnd()
      LoRa.beginPacket();
      LoRa.write(destination);
      LoRa.endPacket();
      rssiMillis = 0;
      rssiMillis = millis();
  }
}

void setup() {
  Serial.begin(9600);
  initializePins();
  startMillis=millis();
  rssiMillis = millis();

  while (!Serial);
  //Serial.println("LoRa Receiver");
  LoRa.setPins(10, 9, 2);
  //LoRa.setSPIFrequency(8E6);
  if (!LoRa.begin(868E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  nexInit();
}

void printData(){
  serialEnd();
  for(int i = 0; i < 3; i++){
    if (gasPoint[i] == SET) {
      int t1 = gas[i] / 10;
      int t2 = gas[i] - (gas[i] / 10) * 10;
      sprintf(val, "gas%ivalue.txt=\"%i,%i\"", i+1, t1, t2);
      Serial.print(val);
      serialEnd();
    }
    else {
      sprintf(val, "gas%ivalue.txt=\"%i\"", i+1, gas[i]);
      Serial.print(val);
      serialEnd();
    }
  }
}


void loop() {
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
    Serial.print("Received packet '");

    // read packet
    while (LoRa.available()) {
      if(LoRa.read() == localAddress){
          gas[0] = word(LoRa.read(), LoRa.read());
          gasPoint[0] = LoRa.read();
          gas[1] = word(LoRa.read(), LoRa.read());
          gasPoint[1] = LoRa.read();
          gas[2] = word(LoRa.read(), LoRa.read());
          gasPoint[2] = LoRa.read();
          //counter = LoRa.read();
          printData();
          //LoRa.endPacket();
          // clear ackflag
           ackflag=false;
      }
      //Serial.print((char)LoRa.read());
    }

    // print RSSI of packet
    sprintf(val, "lora.txt=\"%i\"", LoRa.packetRssi());
    Serial.print(val);
    serialEnd();
    //Serial.println(LoRa.packetRssi());
    //debugging();
    sendData();
  }

  toggleSwtich();
  if(ackflag==false){gasConcentration(0,gas[0]); // gasConcentration(gasType,gasConcentration)
                      if(alarmFlag1==true) {priorityCheck=true;}
                     if(alarmFlag2==false){gasConcentration(1,gas[1]);
                       priority();
                       if(alarmFlag2==false && priorityCheck==true) {alarmFlag1=true;}
                      if(alarmFlag1==false && alarmFlag2==false) {gasConcentration(2,gas[2]);
                        priority();
                      }
                    }
                     alarm();
  }
  else if (ackflag==true){stopAlarm();}
}

void gasConcentration(int gasType,int gasCon){   //info about gasType and gas concentration
  /*gasType Gas  : max   A1  A2  unit
        0   CH4  : 100   10  20  %
        3   IBUT : 2000  100 200 ppm
        1   O2   : 25    19  23  % lower than 19 higher than 23 is not okay
        2   CO   : 500   20  100 ppm
  */
     if(gasType==0)       {setAlarm(gasCon,10,20); //explosive
     gas[0]=gasCon;}
     else if(gasType==1)  { setAlarmO2(gasCon,19,23); //O2
     gas[1]=gasCon;}
     else if(gasType==2)  { setAlarm(gasCon,20,100); // CO
     gas[2]=gasCon;}
     /*else if(gasType==3)  {setAlarm(gasCon,20,100);
     amountOfGas[3]=gasCon;}*/
  }

void setAlarm( int gasCon, int A1, int A2){  //choose between alarm 1 and alarm 2 and set alarm flag
   if(gasCon>=A1 && gasCon<A2){
      alarmFlag2=false;
      alarmFlag1=true;}
    else if (gasCon>=A2){
        alarmFlag2=true;
        alarmFlag1=false;
        }
    else {
        alarmFlag2=false;
        alarmFlag1=false;
      }
}

void setAlarmO2( int gasCon, int A1, int A2){  //choose between alarm 1 and alarm 2 and set alarm flag
   if(gasCon<=A1){
      alarmFlag2=false;
      alarmFlag1=true;}
    else if (gasCon>=A2){
        alarmFlag2=true;
        alarmFlag1=false;
        }
    else {
        alarmFlag2=false;
        alarmFlag1=false;
      }
}

void alarm(){
  currentMillis = millis(); //get the current time (number of milliseconds since the program started)
  if(currentMillis-startMillis >= period)
  {
    if(alarmFlag1==true || alarmFlag2 ==true){
    digitalWrite(LED1,!digitalRead(LED1));    //blinking LEds
    digitalWrite(LED2,!digitalRead(LED2));
    digitalWrite(LED3,!digitalRead(LED3));
    if(alarmFlag1==true && alarmFlag2==false ){tone(BUZZER,261,250 );}
    if(alarmFlag2==true && alarmFlag1==false) {tone(BUZZER,440,250);}
    startMillis=currentMillis;
    }
    else {stopAlarm();}
  }
}

void stopAlarm(){
  noTone(BUZZER);
  digitalWrite(LED1,LOW);
  digitalWrite(LED2,LOW);
  digitalWrite(LED3,LOW);
}

void toggleSwtich(){
  newSwitch=digitalRead(ACKBUT);
  if(newSwitch !=oldSwitch){
      if(newSwitch==HIGH){
          if(ackflag==false) {ackflag=true;}
          else               {ackflag=false;}
        }
    }
    oldSwitch=newSwitch;
}

void initializePins() {
  pinMode(LED1,  OUTPUT);
  pinMode(LED2,  OUTPUT);
  pinMode(LED3,  OUTPUT);
  pinMode(BUZZER,OUTPUT);
  pinMode(ACKBUT,INPUT);
}
void priority() {
  if(alarmFlag2==true){
    alarmFlag1=false;
    alarmFlag2=true;
  }
}
void debugging(){
  if(!ackflag){
    sprintf(val, "ack==false");
    Serial.println(val);
    Serial.println(alarmFlag1);
    Serial.println(alarmFlag2);
  }
  else{sprintf(val, "ack==true");
    Serial.println(val); }
  serialEnd();
}
