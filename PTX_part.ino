#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define CE_Pin 9
#define CSN_PIN 10
#define PIPE_CNT 3

RF24 radio(CE_PIN, CSN_PIN);
LiquidCrystal_I2C lcd(0x27, 16, 2);// I2C주소값 
int iRecvValue[3]= {0};
int iTemp[PIPE_CNT]={0};
int iHumis[PIPE_CNT]={0};
unit64_t_address[PIPE_CNT]={0xF0F0cccc11, 0xF0F0F0F012, 0xF0F0F0F013};

const int iNodeNum=1;
int iSendValue[3]={0};
unsigned long snd_time=millis();
unsigned long cur_time=millis();
char recv_flag='N';

/*
  AnalogReadSerial

  Reads an analog input on pin 0, prints the result to the Serial Monitor.
  Graphical representation is available using Serial Plotter (Tools > Serial Plotter menu).
  Attach the center pin of a potentiometer to pin A0, and the outside pins to +5V and ground.

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/AnalogReadSerial
*/

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  radio.begin();
  radio.setPALevel(RF24_PA_MIN);//모든 통신 파이프 오픈
  radio.openReadingPipe(0, address[iNodeNum]);
  radio.stopListening();
  iSendValue[2]=iNodeNum;
  pinMode(LEDPIN, OUTPUT);
  digitalWrite(LEDPIN, LOW);
}

// the loop routine runs over and over again forever:
void loop() {
  cur_time-millis();

  if(cur_time-snd_time>3000){
    DHT.read11(DHTPIN);
    
  }
  byte pipeNum = 0;
  while(radio.available(&pipeNum)){
    Serial.println("Here");
    radio.read(&iRecvValue, sizeof(iRecvValue));
    iTemp[iRecvValue[2]]=iRecvValue[0];
    delay(200);
    sendRecvOK(pipeNum);
  }

  for(int i=0; i<2; i++){
    lcd.clear();
    lcd.home();
    String line1 ="R"+ String(i*2+1)+":"+ String(iTemp[i*2]);
    lcd.print(line1);

    if(i!=1){
      String line2 = "R"+ String(i*2+2)+":"+ String(iTemp[i*2+1]);
      lcd.setCursor(0,1);
      lcd.print(line2);
    }
    delay(1000);
  }        // delay in between reads for stability
}
