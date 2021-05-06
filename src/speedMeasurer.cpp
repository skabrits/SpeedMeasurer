/*
    speedMeasurer.cpp - Library for measuring speed with 2 bmp180 sensors.
    Created by Vsevolod Kabrits, May 3, 2021. Version 1.0.
    Released into the public domain.
    
       .                  .
       sda               scl
       |                  |
       | \              / |
       ------        ------ 2 breakout elements (reles or chips)
       |     \      /     |
       |       \  /       |
       |        /\        |
       |      /    \      |
       |    /        \    |
       |  /            \  |
       bmp180 - 1     bmp180 - 2
*/

#include <speedMeasurer.h>
#include <Wire.h>
#include <stdio.h>
#include <math.h>


SpeedMeasurer::SpeedMeasurer(int static_pressure_pin, int dynamic_pressure_pin) {
  _spin = static_pressure_pin;
  _dpin = dynamic_pressure_pin;
}


SpeedMeasurer::SpeedMeasurer(int static_pressure_pin, int dynamic_pressure_pin, int percision, bool rele = 1) {
  _spin = static_pressure_pin;
  _dpin = dynamic_pressure_pin;
  _percision = percision % 4;
  _rele = rele;
}


bool SpeedMeasurer::begin() {
  pinMode(_spin, OUTPUT);
  pinMode(_dpin, OUTPUT);
  s_t_flag = false;
  d_t_flag = false;
  s_p_flag = false;
  d_p_flag = false;
  s_on = false;
  d_on = false;
  return d2s() && s2d();
}


bool SpeedMeasurer::prepare() {
  byte s_t_s, s_t_m, s_p_s, s_p_m, d_t_s, d_t_m, d_p_s, d_p_m;
  
  s_t_s = startemp_s();
  delay(s_t_s + 10);
  s_t_m = getemp_s();
  T_s_basic = T_s;
  
  d_t_s = startemp_d();
  delay(d_t_s + 10);
  d_t_m = getemp_d();
  T_d_basic = T_d;

  s_p_s = startpressure_s();
  delay(s_p_s + 10);
  s_p_m = getpressure_s();
  P_s_basic = P_s;
  
  d_p_s = startpressure_d();
  delay(d_p_s + 10);
  d_p_m = getpressure_d();
  P_d_basic = P_d;

  T_s = -1000;
  T_d = -1000;
  P_s = -100;
  P_d = -100;
  T_basic = (T_s_basic + T_d_basic) / 2;
  P_basic = (P_s_basic + P_d_basic) / 2;

  return s_t_s && s_t_m && s_p_s && s_p_m && d_t_s && d_t_m && d_p_s && d_p_m;
}


byte SpeedMeasurer::startemp_s() {
  if (!d_t_flag && !d_p_flag && !s_t_flag && !s_p_flag) {
    d2s();
    s_t_flag = true;
    return s.startTemperature();
  } else {
    return 0;
  }
}


byte SpeedMeasurer::startemp_d() {
  if (!d_t_flag && !d_p_flag && !s_t_flag && !s_p_flag) {
    s2d();
    d_t_flag = true;
    return d.startTemperature();
  } else {
    return 0;
  }
}


bool SpeedMeasurer::getemp_s() {
  if (s_on && s_t_flag && !s_p_flag) {
    s_t_flag = false;
    return s.getTemperature(T_s);
  } else {
    return 0;
  }
}


bool SpeedMeasurer::getemp_d() {
  if (d_on && d_t_flag && !d_p_flag) {
    d_t_flag = false;
    return d.getTemperature(T_d);
  } else {
    return 0;
  }
}


byte SpeedMeasurer::startpressure_s() {
  if (!d_t_flag && !d_p_flag && !s_t_flag && !s_p_flag) {
    d2s();
    s_p_flag = true;
    return s.startPressure(_percision);
  } else {
    return 0;
  }
}


byte SpeedMeasurer::startpressure_d() {
  if (!d_t_flag && !d_p_flag && !s_t_flag && !s_p_flag) {
    s2d();
    d_p_flag = true;
    return d.startPressure(_percision);
  } else {
    return 0;
  }
}


bool SpeedMeasurer::getpressure_s() {
  if (s_on && !s_t_flag && s_p_flag && T_s != -1000) {
    s_p_flag = false;
    return s.getPressure(P_s, T_s);
  } else {
    return 0;
  }
}


bool SpeedMeasurer::getpressure_d() {
  if (d_on && !d_t_flag && d_p_flag && T_d != -1000) {
    d_p_flag = false;
    return d.getPressure(P_d, T_d);
  } else {
    return 0;
  }
}


double SpeedMeasurer::calc_velocity() {
  if(T_s != -1000 && T_d != -1000 && P_s != -100 && P_d != -100) {
    int v_2 = 2*(P_s - P_s_basic - P_d + P_d_basic)*8.314462*(T_d - T_d_basic + T_basic + 273.15)/((P_d - P_d_basic + P_basic)*0.029);
    v = v_2 / abs(v_2) * sqrt(abs(v_2));
  }
  return v;
}


bool SpeedMeasurer::d2s() {
  if (!d_t_flag && !d_p_flag) {
    digitalWrite(_dpin, LOW);
    digitalWrite(_spin, HIGH);
    if (_rele) {
      delay(7);
    }
    s_on = true;
    d_on = false;
    bool a = s.begin();
    return a;
  } else {
    return 0;
  }
}


bool SpeedMeasurer::s2d() {
  if (!s_t_flag && !s_p_flag) {
    digitalWrite(_spin, LOW);
    digitalWrite(_dpin, HIGH);
    if (_rele) {
      delay(7);
    }
    s_on = false;
    d_on = true;
    bool a = d.begin();
    return a;
  } else {
    return 0;
  }
}
