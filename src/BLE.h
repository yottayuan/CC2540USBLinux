#ifndef BLE_H
#define BLE_H

#include <iostream>
#include <stdio.h>
#include <vector>
#include "HCIEvents.h"
#include "HCICodes.h"

#define IRK_LEN                         0x10
#define IRK_POS                         0x0F
#define CSRK_LEN                        0x10
#define CSRK_POS                        0x1F
#define ADDR_LEN                        0x06
#define ADDR_POS                        0x06

#define DEVICE_DISCOVERY_STATUS_POS     0x05
#define DEVICE_INFOMATION_STATUS_POS    0x05

#define DEBUG
using std::vector;

class BLE {
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
    bool *getModuleStatus();

private:
    std::vector<char> dongle_address;
    unsigned char dongle_addr_type;
    std::vector<char> irk;
    std::vector<char> csrk;
    bool is_module_initialized;
    bool is_received;
    int number_of_device_found;
    std::vector<BLEDevice> devices_found;


    bool setDongleAddress(const char *addr);

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

class BLEDevice {
public:
    BLEDevice();
    virtual ~BLEDevice();
    bool setAddressType(unsigned char &type);
    bool setEventType(unsigned char &type);
    bool setAddress(const char *addr);
    bool setRSSIValue(unsigned char &value);
    bool setDeviceData(const char *data);

    bool getAddressType(unsigned char &type);
    bool getEventType(unsigned char &type);
    bool getRSSIValue(unsigned char &value);
    bool getAddress(std::vector<char> &addr);
    bool getDeviceData(std::vector<char> &data);
private:
    unsigned char addr_type;
    unsigned char event_type;
    unsigned char rssi_value;
    std::vector<char> addr;
    std::vector<char> device_data;

};

#endif
