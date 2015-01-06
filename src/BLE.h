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
#ifndef BLE_H
#define BLE_H

#include <iostream>
#include <stdio.h>
#include <vector>
#include "HCIEvents.h"
#include "HCICodes.h"
#include "BLEDevice.h"

#define IRK_LEN                         0x10
#define IRK_POS                         0x0F
#define CSRK_LEN                        0x10
#define CSRK_POS                        0x1F
#define ADDR_LEN                        0x06
#define ADDR_POS                        0x06

#define DEVICE_DISCOVERY_STATUS_POS     0x05
#define DEVICE_INFOMATION_STATUS_POS    0x05

//#define DEBUG
using std::vector;
namespace framework {

class BLE :public Singleton<BLE> {
    friend class Singleton<BLE>;
public:
    BLE();
    virtual ~BLE();
    std::vector<char> getGAPCommand(unsigned char cmd, std::vector<char> data);
    std::vector<char> getGATTCommand(unsigned char cmd, std::vector<char> data);
    std::vector<char> getATTCommand(unsigned char cmd, std::vector<char> data);
    void received(const char *data, unsigned int len);
    bool getDongleAddress(std::vector<char> &addr);
    bool getModuleIRK(std::vector<char> &irk);
    bool getModuleCSRK(std::vector<char> &csrk);
    bool getModuleStatus(bool &status);
    bool getModuleStatus();
    bool getScanResultList(BLEDevice *list);

private:
    std::vector<char> dongle_address;
    unsigned char dongle_addr_type;
    std::vector<char> irk;
    std::vector<char> csrk;
    bool is_module_initialized;
    bool is_received;
    bool scan_finished;
    int number_of_device_found;
    std::vector<std::string> devices_found;
    BLEDevice *devices_list;


    bool setDongleAddress(const char *addr);
    bool setModuleStatus(bool status);

    bool setModuleIRK(const char *irk);
    bool setModuleCSRK(const char *csrk);

    bool processHCIEvents(const char *data, unsigned int len);
    bool processEventGAPDeviceDone(const char *data, unsigned int len);
    bool processEventGAPDeviceDiscovery(const char *data, unsigned int len);
    bool processEventGAPLinkEstablished(const char *data, unsigned int len);
    bool processEventGAPLinkTerminated(const char *data, unsigned int len);
    bool processEventGAPDeviceInfomation(const char *data, unsigned int len);
    bool processEventGAPHCIExtCommandStatus(const char *data, unsigned int len);
    bool processEventATTReadByTypeResponse(const char *data, unsigned int len);
    bool processEventATTWriteResponse(const char *data, unsigned int len);
    bool processEventATTHandleValueNotification(const char *data, unsigned int len);
    bool processEventNoMatch(const char *data, unsigned int len);

};

}

#endif
