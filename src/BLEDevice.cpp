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

#include "BLEDevice.h"
using namespace framework;

BLEDevice::BLEDevice() {

}

BLEDevice::BLEDevice(const char *data, int len) {
#if 0
    for(int i = 0x00; i<len; i++) {
        printf("%02x ", (unsigned char)data[i]);
    }
    printf("\n");
#endif
    setAddress(data);
    setRSSIValue((unsigned char) *(data+0x06));
    setDeviceData(data+0x07); // From data lenght
}

BLEDevice::~BLEDevice() {

}

bool BLEDevice::setAddressType(unsigned char type) {

}

bool BLEDevice::setEventType(unsigned char type) {

}

bool BLEDevice::setAddress(const char *addr) {
    if(device_addr != NULL) {
        delete [] device_addr;
    }
    device_addr = new char[0x06];
    for (int i = 0; i<0x06; i++) {
        device_addr[i] = addr[i];
    }
#if 0
    printf("setAddress: ");
    for(int i = 0x00; i<0x06; i++) {
        printf("%02x ", (unsigned char)device_addr[i]);
    }
    printf("\n");
#endif
    return true;
}

bool BLEDevice::setRSSIValue(char value) {
    rssi_value = value;
}

bool BLEDevice::setDeviceData(const char *data) {
    if(device_data != NULL) {
        delete [] device_data;
    }
    int data_len = data[0] + 1;
    device_data = new char[data_len];
    for (int i = 0; i<data_len; i++) {
        device_data[i] = data[i];
    }
#if 0
    printf("setDeviceData: ");
    for(int i = 0; i<data_len; i++) {
        printf("%02x ", (unsigned char)device_data[i]);
    }
    printf("\n");
#endif
    setPwrValue(device_data[data_len-1]);
    return true;
}

bool BLEDevice::getAddressType(unsigned char &type) {

}

bool BLEDevice::getEventType(unsigned char &type) {

}

bool BLEDevice::getRSSIValue(char &value) {
    value = rssi_value;
#if 0
    printf("getRSSIValue: %02x\n", (unsigned char)value);
#endif
    return true;
}

char *BLEDevice::getAddress() {
#if 0
    printf("getAddress: ");
    for(int i = 0x00; i<0x06; i++) {
        printf("%02x ", (unsigned char)device_addr[i]);
    }
    printf("\n");
#endif
    return device_addr;
}

bool BLEDevice::getDeviceData(char *data) {

}

bool BLEDevice::getDistance(float &value) {

}
bool BLEDevice::calculateDistance() {
    if ((pwr_value == 0) || (rssi_value == 0)) {
        distance = 0.0;
    } else {

}
    return true;
}

bool BLEDevice::setPwrValue(char value) {
    pwr_value = value;
#if 1
    printf("setPwrValue: %d\n", pwr_value);
#endif
    return true;
}

