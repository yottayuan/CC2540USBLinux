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
#include <boost/thread.hpp>
#include "HCIEvents.h"
#include "HCICodes.h"
#include "BLEDevice.h"
#include <math.h>

#define IRK_LEN                         0x10
#define IRK                             0x0F
#define CSRK_LEN                        0x10
#define CSRK                            0x1F
#define ADDR_LEN                        0x06
#define DEVICE_ADDR                     0x06

#define DEVICE_DISCOVERY_STATUS         0x05
#define DEVICE_INFOMATION_STATUS        0x05
#define RSP_DEVICE_ADDR                 0x08
#define RSP_DEVICE_RSSI                 0x0E
#define RSP_DEVICE_DATA_LEN             0x0F
#define RSP_DEVICE_DATA_FIELD           0x10

//#define DEBUG_MSG
//#define TEST_CODE
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
    bool isModuleInitialized();
    bool isSendCMDCompleted();
    bool isScanFinished();
    bool getScanResultList(BLEDevice *list);
    bool clearDevicesList();
    bool isiBeaconDevice(std::vector<char> data);
    bool isValidAddr(std::string addr);

private:
    std::vector<char> dongle_address;
    bool module_initialized;
    bool scan_finished;
    bool send_cmd_completed;
    int number_of_device_found;
    std::vector<std::string> devices_found;

    std::vector<char> received_data;
    boost::mutex readQueueMutex;

    bool setDongleAddress(std::vector<char> addr);

    bool processHCIEvents(std::vector<char> data);
    bool processEventGAPDeviceDone(std::vector<char> data);
    bool processEventGAPDeviceInfomation(std::vector<char> data);
    bool processEventGAPDeviceDiscovery(std::vector<char> data);
    bool processEventGAPLinkEstablished(std::vector<char> data);
    bool processEventGAPLinkTerminated(std::vector<char> data);
    bool processEventGAPHCIExtCommandStatus(std::vector<char> data);
    bool processEventATTReadByTypeResponse(std::vector<char> data);
    bool processEventATTWriteResponse(std::vector<char> data);
    bool processEventATTHandleValueNotification(std::vector<char> data);
    bool processEventNoMatch(std::vector<char> data);
    double calculateDistance(char txPower, char rssi);

};

}

#endif
