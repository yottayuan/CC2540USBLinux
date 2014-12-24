#ifndef BLEDEVICE_H
#define BLEDEVICE_H

#include <vector>
#include "Singleton.h"

namespace framework {
class BLEDevice : public Singleton<BLEDevice>{
    friend class Singleton<BLEDevice>;
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

}
#endif
