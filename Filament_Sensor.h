#ifndef FILAMENT_SENSOR_H
#define FILAMENT_SENSOR_H
#include "Arduino.h"
#include "OneWire.h"
#define READOUT_MEAN_N 10
#define INT_TIME 1000
#define M_TIME 500
#define SLOPE 0.34
#define INTERCEPT 0
#define OW_ADDR 0x69
//#define SERIAL_FILAMENT_DEBUG

class Filament_Sensor 
{
private:
    OneWire *ds;
    byte _pin;
    byte ADDR;
    byte readout_to_mean;
    byte addr[8];   // buffor to store OW data
    byte data[12];
    int integration_time;
    int measurement_time;
    float slope_coef;
    byte intercept;
public:
    Filament_Sensor(byte PIN);
    void Init();
    void set_measurement_time(int time);
    void set_integration_time(int time);
    void set_calibration_parameters(float slope,byte intercept);
    void set_readout_to_mean(byte n);
    void set_addr(byte addr);
    bool upload_config();
    bool readData();
	int readSpeed_X(bool raw = false);
    int readSpeed_Y(bool raw = false);
    bool set_resolution(byte X_res,byte Y_res);
    
};

#endif