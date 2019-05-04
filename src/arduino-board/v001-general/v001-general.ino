#define _TASK_SLEEP_ON_IDLE_RUN
#include <TaskScheduler.h>
#include <Wire.h>
#include <DHT11.h>
dht11 DHT11;
#define DHT11PIN 4
#define MOTION_DET 7
String DEVICE_NAME = "p01-uno-001";

int count_sent_data;
int count_recieve_commands;
int status_code = 0;
char write2wire[32];

void read_sensors();
void parse_command();

Scheduler runner;
// Callback methods prototypes
void t0Callback();
void t1Callback();
void t2Callback();
// Routine maintenance Task
Task t0(2000, TASK_FOREVER, &t0Callback, &runner, true);
// Tasks for actuator
//Task t1_alert(200, TASK_FOREVER, &t1Callback, &runner, true);
//Task t2_semaphore(1000, TASK_FOREVER, &t2Callback, &runner, false);

void requestEvent();
void receiveEvent(int howMany);

void log_msg(String priority, String msg) {
  /*doc.clear();
  doc["type"] = "log";
  doc["priority"] = priority;
  doc["msg"] = msg;
  doc["name"] = DEVICE_NAME;*/
}

void setup() {
  Serial.begin(9600); // start serial for debug
  while (!Serial) continue;
  Serial.println("fog-explorer-v001 v0.1");
  
  Wire.begin(8);  // join i2c bus with address 8
  Wire.onReceive(receiveEvent); //register receive event
  Wire.onRequest(requestEvent); //register request event

  pinMode(DHT11PIN, OUTPUT);  //digital 2 --> DHT11
  pinMode(MOTION_DET, INPUT); //digital 7 --> motion sensor
  
  delay(500);
  runner.startNow(); 
}

void loop() {
  runner.execute();
}

/*-----------------------------------------*/
void read_sensors() {
  String str = "";
  //LM35
  int val;
  int dat;
  val = analogRead(2); //analog pin2 --> LM35；
  dat = ((125 * val) >> 8); //加入light后需要参数修正，可能是电路问题
  str += String(dat);
  str += "#";
  //DHT1
  int chk = DHT11.read(DHT11PIN);
  str += String((float)DHT11.humidity);
  str += "#";
  str += String((float)DHT11.temperature);
  str += "#";
  //Light
  val = analogRead(1); //传感器接于模拟口1
  str += String(val);
  str += "#";
  //knob
  val = analogRead(0); //传感器接于模拟口0
  str += String(val);
  str += "#";
  //motion-detection
  val = digitalRead(MOTION_DET);
  str += String(val);
  str += "$*";
  Serial.println(str);
  strcpy(write2wire, str.c_str());
}

// function that executes whenever data is requested from i2c master
void requestEvent() {
  Wire.write(write2wire);  /*send string on request */
}

/*-----------------------------------------*/
// function that executes whenever data is received from i2c master
void receiveEvent(int howMany) {
  while (0 < Wire.available()) {
    char c = Wire.read();
    Serial.print(c);
  }
  Serial.println();
}

void t0Callback(){
 /* int x;
  Wire.beginTransmission(0x08); // transmit to device #8
  Wire.write("x is ");        // sends five bytes
  Wire.write(x);              // sends one byte
  Wire.endTransmission();    // stop transmitting
  
  if(1) { //0 == status_code
    log_msg("LOG_INFO","Start up.");
    //启动接收wire，等待命令
    status_code = 1;
  }*/
  read_sensors();
}



void parse_command() {
  
}
