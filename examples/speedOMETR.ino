#include <SpeedMeasurer>
#include <Wire.h>
#include <math.h>


SpeedMeasurer sm(5,6,3);

int phase, com;
byte del_time;
bool timer_on = false, start = true;
unsigned long current_time, start_time;

void setup() {
  Serial.begin(9600);
  Serial.println(sm.begin() ? "Initialized sucsesfully" : "Failed to init");
  Serial.println(sm.prepare() ? "Preparation sucsesfully" : "Failed to prepare");
  phase = 0;
}

void loop() {
  if (Serial.available() > 1) {
    com = Serial.parseInt();
    Serial.println("hi " + String(com));
    if (com == 0) {
      start = false;
      phase = 0;
      timer_on = false;
    } else if (com == 2) {
      Serial.println(sm.prepare() ? "Preparation sucsesfully" : "Failed to prepare");
      start = true;
      phase = 0;
      timer_on = false;
    } else {
      start = true;
      phase = 0;
      timer_on = false;
    }
  }

  if (start) {
    if(!timer_on) {
      start_time = millis();
      timer_on = !timer_on;
    }
    
    if(phase == 0) {
      del_time = sm.startemp_s();
      Serial.println("Static tempreture delay time: " + String(del_time) + " ms");
      current_time = millis();
      phase++;
    }

    if(phase == 1) {
//      Serial.println(start);
//      Serial.println(millis() - current_time);
      if(millis() - current_time > del_time ) {
        Serial.println(sm.getemp_s() ? "Measured static temperature sucsesfully" : "Failed to measure static temperature");
        phase++;
      }
    }
  
    if(phase == 2) {
      del_time = sm.startemp_d();
      Serial.println("Dynamic tempreture delay time: " + String(del_time) + " ms");
      current_time = millis();
      phase++;
    }

    if(phase == 3) {
      if(millis() - current_time > del_time) {
        Serial.println(sm.getemp_d() ? "Measured dynamic temperature sucsesfully" : "Failed to measure dynamic temperature");
        phase++;
      }
    }
  
    if(phase == 4) {
      del_time = sm.startpressure_s();
      Serial.println("Static pressure delay time: " + String(del_time) + " ms");
      current_time = millis();
      phase++;
    }

    if(phase == 5) {
      if(millis() - current_time > del_time) {
        Serial.println(sm.getpressure_s() ? "Measured static pressure sucsesfully" : "Failed to measure static pressure");
        phase++;
      }
    }
  
    if(phase == 6) {
      del_time = sm.startpressure_d();
      Serial.println("Dynamic pressure delay time: " + String(del_time) + " ms");
      current_time = millis();
      phase++;
    }

    if(phase == 7) {
      if(millis() - current_time > del_time) {
        Serial.println(sm.getpressure_d() ? "Measured dynamic pressure sucsesfully" : "Failed to measure dynamic pressure");
  
        Serial.println("Static tempreture: " + String(sm.T_s) + " C; " + "Dynamic tempreture: " + String(sm.T_d) + " C; " + "Static pressure: " + String(sm.P_s*100) + " Pa; " + "Dynamic pressure: " + String(sm.P_d*100) + " Pa");
        Serial.println("Basic static tempreture: " + String(sm.T_s_basic) + " C; " + "Basic dynamic tempreture: " + String(sm.T_d_basic) + " C; " + "Basic static pressure: " + String(sm.P_s_basic*100) + " Pa; " + "Basic dynamic pressure: " + String(sm.P_d_basic*100) + " Pa");
        Serial.println("Basic tempreture: " + String(sm.T_basic) + " C; " + "Static pressure: " + String(sm.P_basic*100) + " Pa");
        Serial.println("Velocity: " + String(sm.calc_velocity()) + " m/s");
        Serial.println("Overall time: " + String(millis() - start_time) + " ms");
        phase++;
      }
    }
  }
}
