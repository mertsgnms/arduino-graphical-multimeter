#include <Wire.h>
#include <Adafruit_INA219.h>
#include <U8g2lib.h>    
U8G2_SH1107_128X128_1_HW_I2C screen(U8G2_R0,U8X8_PIN_NONE);
const int xStart = 35;     
const int xEnd   = 127;    
const int graphWidth = xEnd - xStart + 1;
float value;
int reading;
int valueACV;
float scaledACV;
float valueDCV;
float scaledDCV;
#define DCVpin A1
#define ACVpin A7
float current_mA = 0;
Adafruit_INA219 ina219;
#define controlPin 6
#define activateDCVm 8
#define activateCurrentm 4
int graph[graphWidth];
unsigned long lastGraphUpdate = 0;
const int graphInterval = 1;
const int AC_midpoint = 512;   
int acPeak = 0;                
int acPeakSample = 0;          
unsigned long lastPeakReset = 0;
const unsigned long peakWindow = 20;   
int acPeakPos = 0;
int acPeakNeg = 0;
int acPeakPos_display = 0;          
int acPeakNeg_display = 0;          

void setup() {
  screen.begin();
  screen.setFont(u8g2_font_5x8_mr);
  pinMode(DCVpin, INPUT);
  
  ina219.begin();
  ina219.setCalibration_16V_400mA();
  pinMode(controlPin, OUTPUT);
  pinMode(activateDCVm, INPUT);
  pinMode(activateCurrentm, INPUT);
  digitalWrite(controlPin, HIGH);

}
void loop() {
  unsigned long now = micros();

  
  if (now - lastGraphUpdate >= graphInterval * 1000) {
    lastGraphUpdate = now;

    BufferGraph();   
  }
  static int drawSkip = 0;
  if (drawSkip++ >= 2) {      
    drawSkip = 0;

    screen.firstPage();
    do {
      DrawAxis();
      DrawGraph();
    } while (screen.nextPage());
  }
  if (millis() - lastPeakReset >= peakWindow) {
    acPeakPos_display = acPeakPos;
    acPeakNeg_display = acPeakNeg;
    
    acPeakPos = 0;
    acPeakNeg = 0;

    lastPeakReset = millis();
  } 
}

float measureDCV(){
  reading = analogRead(DCVpin);
  valueDCV = float(reading) / 1023;
  valueDCV = valueDCV * 5;
  scaledDCV = valueDCV * 20.36;
  return scaledDCV;
}

float measureCurrent(){
  current_mA = ina219.getCurrent_mA();
  return current_mA;
}

int measureACV(){
  reading = analogRead(ACVpin);
  return reading;
}
uint8_t checkPin(){
  if(digitalRead(activateDCVm) == 1){
    return 1;
  }
  else if(digitalRead(activateCurrentm) == 1){
    return 2;
  }
  else{
    return 3;
  }
}

void DrawAxis(){
  screen.drawLine(0, 0, 0, 127);
  String read;
  if(checkPin()== 1){
    screen.drawStr(5, 10, "DCV");
    screen.drawStr(5,50 ,"V" );
    read = String(value);
  }
  else if(checkPin()== 2){
    screen.drawStr(5, 10, "CUR");
    screen.drawStr(5,50 ,"mA" );
    read = String(value);
  }
  else if(checkPin()== 3){
    screen.drawStr(5, 10, "ACV");
    screen.drawStr(5,50 ,"V" );

    float peakPosV = acPeakPos_display * (5.0/1023.0) * 20.0;
    float peakNegV = acPeakNeg_display * (5.0/1023.0) * 20.0;

    read = String(peakPosV,2);
  }
  
  screen.drawStr(5,30 , read.c_str() );
    
}

void DrawGraph(){
  for(int i=1; i<graphWidth; i++){
    int x1 = xStart + (i-1);
    int x2 = xStart + i;
    screen.drawLine(x1, 127 - graph[i-1], x2, 127 -graph[i]);
  }
}
void BufferGraph(){
  int graphvalue;
  uint8_t mode = checkPin();
  if(mode==1){
    value = measureDCV();
    graphvalue = map(value,0,100,0,127);
  }
  else if(mode == 2){
    value = measureCurrent();
    graphvalue = map(value,0,400,0,64);
    graphvalue = graphvalue +63;
  }

  else if(mode == 3){
    valueACV = measureACV();
  
  int centered = valueACV - 512;     

    
    graphvalue = map(centered, -512, 512, 127, 0);

    
    if (centered > acPeakPos) acPeakPos = centered;
    if (centered < acPeakNeg) acPeakNeg = centered;
    }
  

  
  for(int i=0; i<graphWidth-1; i++){
    graph[i] = graph[i+1];
  }
  graph[graphWidth-1] = graphvalue;
}