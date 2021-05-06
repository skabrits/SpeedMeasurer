#include <SpeedMeasurer.h>
#include <Wire.h>
#include <math.h>

SpeedMeasurer sm(5,6,3);

int phase, com, cou;
byte del_time;
bool timer_on = false, start = true;
unsigned long current_time, start_time;
double velos[10] = {0,0,0,0,0,0,0,0,0,0};

void setup() {
  Serial.begin(9600);
  Serial.println(sm.begin() ? "Initialized sucsesfully" : "Failed to init");
  Serial.println(sm.prepare() ? "Preparation sucsesfully" : "Failed to prepare");
  phase = 0;
  cou = 0;
}

void loop() {
  if (Serial.available() > 1) {
    com = Serial.parseInt();
    Serial.println("hi " + String(com));
    if (com == 0) {
      start = false;
    } else if (com == 2) {
      Serial.println(sm.prepare() ? "Preparation sucsesfully" : "Failed to prepare");
      start = true;
    } else {
      start = true;
      
    }
  }

  if (start) {
    velos[cou % 10] = speed_calc();
  }
  cou++;
  double sv = 0;
  for(int i = 0; i < 10; i++) {
    sv += velos[i];
  }
  Serial.println(sv/10);
}

double speed_calc() {
  del_time = sm.startemp_s();
  delay(del_time+10);
  sm.getemp_s();
  del_time = sm.startemp_d();
  delay(del_time+10);
  sm.getemp_d();
  del_time = sm.startpressure_s();
  delay(del_time+10);
  sm.getpressure_s();
  del_time = sm.startpressure_d();
  delay(del_time+10);
  sm.getpressure_d();
  
  return sm.calc_velocity();
}
