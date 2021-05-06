/*
    speedMeasurer.h - Library for measuring speed with 2 bmp180 sensors.
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

#ifndef speedMeasurer_h
#define speedMeasurer_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>
#include <SFE_BMP180.h>

class SpeedMeasurer
{
  public:

    double T_s = -1000, T_d = -1000, P_s = -100, P_d = -100, v = -100, T_s_basic = 0, T_d_basic = 0, P_s_basic = 0, P_d_basic = 0, T_basic = 0, P_basic = 0;
    
    SpeedMeasurer(int static_pressure_pin, int dynamic_pressure_pin);
      // inits pito tube on 2 controling pins (for 1 arduino to switch devices)

    SpeedMeasurer(int static_pressure_pin, int dynamic_pressure_pin, int percision, bool rele = true);
      // percision parameter - quality of measurment, 0 - fast, 3 - percise
      // rele - should wait for breakout when switching or not
    
    bool begin();
      // returns 1 if both sensors initialized sucsessfully, 0 otherwise

    bool prepare();
      // calibrates sensors
      // returns 1 if calibration was sucsessful, 0 otherwise

    
    byte startemp_s();
      // starts tempreture measurment on static pressure sensor
      // returns num of ms to wait until parameter is measured, 0 if failed to connect (on bmp 180 usually 5 ms)
    
    byte startemp_d();
      // starts tempreture measurment on dynamic pressure sensor
      // returns num of ms to wait until parameter is measured, 0 if failed to connect (on bmp 180 usually 5 ms)\


    bool getemp_s();
      // writes static sensor temperature to T_s variable
      // returns 1 if operation was sucsessful, 0 otherwise
    
    bool getemp_d();
      // writes dynamic sensor temperature to T_d variable
      // returns 1 if operation was sucsessful, 0 otherwise

    byte startpressure_s();
      // starts pressure measurment on static pressure sensor
      // returns num of ms to wait until parameter is measured, 0 if failed to connect (on bmp 180 usually 26 ms)
    
    byte startpressure_d();
      // starts pressure measurment on dynamic pressure sensor
      // returns num of ms to wait until parameter is measured, 0 if failed to connect (on bmp 180 usually 26 ms)


    bool getpressure_s();
      // writes static sensor pressure to P_s variable
      // returns 1 if operation was sucsessful, 0 otherwise
    
    bool getpressure_d();
      // writes dynamic sensor pressure to P_d variable
      // returns 1 if operation was sucsessful, 0 otherwise

    double calc_velocity();
      // calculates velocity and returns it
    
  private:
  
    bool s_t_flag = false, d_t_flag = false, s_p_flag = false, d_p_flag = false, s_on = false, d_on = false, _rele = true;
    int _spin, _dpin, _percision = 2;
    SFE_BMP180 s, d;
    
    bool d2s();
      // activates static sensor and diactivates dynamic
    bool s2d();
      // activates dynamic sensor and diactivates static
};

#endif
