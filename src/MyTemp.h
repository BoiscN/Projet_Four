/*
 * MyTemp.h
 *
 *  Created on: 05.05.2022
 *      Author: Nicolas
 */

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <Dht_U.h>

#ifndef MyTemp_H
#define MyTemp_H

class MyTemp
{
private:
    DHT_Unified *dht;
public:
    MyTemp();
    ~MyTemp();
    float getTemp();
    void init();
};

#endif