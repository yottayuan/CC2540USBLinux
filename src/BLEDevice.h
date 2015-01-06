/*
    Copyright (C) 2015  Robot Team - FTI - FPT Corp

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
 */
#ifndef BLEDEVICE_H
#define BLEDEVICE_H

#include <vector>
#include <sstream>
#include <stdio.h>
#include "Singleton.h"

#define ADDR_POS    0x08
#define RSSI_POS    0x0E

namespace framework {
class BLEDevice : public Singleton<BLEDevice>{
    friend class Singleton<BLEDevice>;
public:
    BLEDevice();
    BLEDevice(const char *data, int len);
    virtual ~BLEDevice();
    bool setAddressType(unsigned char type);
    bool setEventType(unsigned char type);
    bool setAddress(const char *addr);
    bool setRSSIValue(char value);
    bool setPwrValue(char value);
    bool setDeviceData(const char *data);

    bool getAddressType(unsigned char &type);
    bool getEventType(unsigned char &type);
    bool getRSSIValue(char &value);
    char *getAddress();
    bool getDeviceData(char *data);

    bool getDistance(float &value);
    bool calculateDistance();
private:
    unsigned char addr_type;
    unsigned char event_type;
    char rssi_value;
    char pwr_value;
    float distance;
    char *device_addr;
    char *device_data;

};

}
#endif
