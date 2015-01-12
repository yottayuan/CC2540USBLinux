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
#include "BLE.h"
#include "HCICodes.h"
using namespace std;
using namespace boost;
using namespace framework;

BLE::BLE():module_initialized(false), send_cmd_completed(false), scan_finished(false){

}

BLE::~BLE() {

}

std::vector<char> BLE::getGAPCommand(unsigned char cmd, std::vector<char> data) {
    std::vector<char> output;
    send_cmd_completed = false;
    output.clear();
    output.push_back(COMMAND);
    switch (cmd) {
    case GAP_DEVICE_INITIALIZATION:
    {
        std::vector<char> irk(0x10, 0x00);
        std::vector<char> csrk(0x10, 0x00);
        std::vector<char> signal(0x0E, 0x00);
        unsigned char data_len = 0x30;
        signal[6] = 0x01;
        output.push_back(cmd); // GAP CMD
        output.push_back(0xFE);
        output.push_back(data_len);
        output.push_back(CENTRAL);
        output.push_back(0x03);
        output.insert(output.end(), irk.begin(), irk.end());
        output.insert(output.end(), csrk.begin(), csrk.end());
        output.insert(output.end(), signal.begin(), signal.end());
        break;
    }

    case GAP_DEVICE_DISCOVERY_REQUEST:
    {
        scan_finished = false;
        output.push_back(cmd); // GAP CMD
        output.push_back(0xFE);
        output.push_back(0x03); // Data length
        output.push_back(0x03); // Mode (all)
        output.push_back(0x01); // Enable name mode
        output.push_back(0x00); // Don't use white list
        break;
    }

    case GAP_ESTABLISH_LINK_REQUEST:
    {
        output.push_back(cmd); // GAP CMD
        output.push_back(0xFE);
        output.push_back(0x09); // Data length
        output.push_back(0x00); // Disable highDutyCycle
        output.push_back(0x00); // Don't use white list
        output.insert(output.end(), data.begin(), data.end()); // 1byte: AddrType, 6bytes: BinAddr
        break;
    }

    case GAP_TERMINATE_LINK_REQUEST:
    {
        output.push_back(cmd); // GAP CMD
        output.push_back(0xFE);
        output.push_back(0x03); // Data length
        //output.push_back(); // ConnHandle
        output.push_back(0x13); // Reason (Remote user terminated connection)
        break;
    }

    case GAP_SET_PARAMETER:
    {
        output.push_back(cmd); // GAP CMD
        output.push_back(0xFE);
        output.insert(output.end(),data.begin(), data.end());
        break;
    }

    default:
        break;
    }

    return output;
}

std::vector<char> BLE::getGATTCommand(unsigned char cmd, std::vector<char> data) {
    std::vector<char> output;
    send_cmd_completed = false;
    output.push_back(COMMAND);
    switch (cmd) {
    case GATT_DISCOVER_CHARACTERISTICS_BY_UUID:
    {
        output.push_back(cmd);
        output.insert(output.end(), data.begin(), data.end());
        break;
    }

    default:
        break;
    }
    return output;
}

void BLE::received(const char *data, unsigned int len) {
    //lock_guard<mutex> l(readQueueMutex);
#if defined (TEST_CODE)
    printf("============================================\n");
    for(int i = 0; i<len; i++) {
        printf("%02x:", (unsigned char)*(data+i));
        if((i%16 == 0)&& i!=0) printf("\n");
    }
    printf("\nDone !\n");
#endif
    received_data.clear();
    received_data.insert(received_data.begin(), data, data+len);
    processHCIEvents(received_data);
}

bool BLE::setDongleAddress(std::vector<char> addr) {
    this->dongle_address.clear();
    int end = ADDR_LEN + DEVICE_ADDR;
    for (int i = DEVICE_ADDR; i < end; i++) {
        this->dongle_address.push_back(addr[i]);
    }
#if defined (TEST_CODE)
    for (int i = 0; i < ADDR_LEN; i++) {
        printf("%02x ",(unsigned char)this->dongle_address[i]);
    }
    printf("\n");
#endif
    return true;
}

bool BLE::getDongleAddress(std::vector<char> &addr) {
    addr = this->dongle_address;
    return true;
}

bool BLE::isModuleInitialized() {
    return module_initialized;
}

bool BLE::isSendCMDCompleted() {
    return send_cmd_completed;
}

bool BLE::isScanFinished() {
    return scan_finished;
}

bool BLE::clearDevicesList() {
    devices_found.clear();
    return true;
}

bool BLE::isValidAddr(string addr) {
    return true;
}

bool BLE::isiBeaconDevice(std::vector<char> data) {
    // check data lenght
    if (data[0x0F] != 0x1E) { // Check lenght of this message
        return false;
    }

    char addr[6];
    for(int i = RSP_DEVICE_ADDR; i< RSP_DEVICE_RSSI; i++) {
        addr[i - RSP_DEVICE_ADDR] = data[i];
    }
    bool result = isValidAddr(std::string(addr));
    if (!result) {
        return false;
    }
    std::string device(addr);
    if (device.empty()) return false;
    // Check if device stored in this vector ?
    //for(std::vector<string>::iterator it = devices_found.begin(); it != devices_found.end(); it++ ) {
    std::size_t found = std::string::npos;
    int device_position = -1;
    for (int i = 0; i< devices_found.size(); i++) {
        //
        found = devices_found[i].find(device);
        if (found != std::string::npos) {
            device_position = i;
            break;
        }
    }
    device.append(1u,data[RSP_DEVICE_RSSI]);
    if (found != std::string::npos) {
#if 0
        printf("Before remove device:\n");
        for (int i = 0; i < devices_found.size(); i++) {
            printf("%s\n", devices_found[i].c_str());
        }
#endif
//        printf("Found current device in: %d, replace it !\n", device_position);
        devices_found.erase(devices_found.begin()+device_position); // remove old result
        devices_found.insert(devices_found.begin()+device_position,device); // instert new result
#if 0
        printf("After remove device:\n");
        for (int i = 0; i < devices_found.size(); i++) {
            printf("%s\n", devices_found[i].c_str());
        }
#endif
    } else {
        devices_found.push_back(device);
    }
    return true;
}

bool BLE::processHCIEvents(std::vector<char> data) {
    bool status = false;
    //lock_guard<mutex> l(readQueueMutex);
    if((unsigned char)data[0] == EVENT) {
#if defined (TEST_CODE)
        printf("============================================\n");
        printf("Event code: %02x\n",(unsigned char)data[1]);
#endif
        if((unsigned char)data[1] == VENDOR_SPECIFIC) {
#if defined (TEST_CODE)
            printf("Vendor Specific Event Code\n");
            printf("Data len: %d\n", data[2]);
            printf("Data Code: %02x%02x \n",(unsigned char)data[4], (unsigned char)data[3]);
#endif
            if((unsigned char)data[4] == 0x06) { // GAP event
#if defined (DEBUG_MSG)
                printf("GAP Event\n");
#endif
                if((unsigned char) data[3] == GAP_COMMAND_STATUS) {
#if defined (DEBUG_MSG)
                    printf("Got GAP Command Status\n");
#endif
                    if(data[5] == 0x00) {
#if defined (DEBUG_MSG)
                        printf("Send command sucessfully !\n");
#endif
                        send_cmd_completed = true;
                        processEventGAPHCIExtCommandStatus(data);
                    } else {
#if defined (DEBUG_MSG)
                        printf("Send command failed !\n");
#endif
                        send_cmd_completed = false;
                    }
                } else if ((unsigned char)data[3] == GAP_DEVICE_INIT_DONE) {
#if defined (DEBUG_MSG)
                    printf("GAP Device Init Done\n");
#endif
                    if(data[5] == 0x00) { //Success
#if defined (DEBUG_MSG)
                        printf("Device initialized and ready\n");
#endif
                        processEventGAPDeviceDone(data);
                    }
                } else if ((unsigned char) data[3] == GAP_DEVICE_INFOMATION) {
#if defined (DEBUG_MSG)
                    printf("GAP Device Infomation\n");
#endif
                    processEventGAPDeviceInfomation(data);
                } else if ((unsigned char) data[3] == GAP_DEVICE_DISCOVERY) {
#if defined (DEBUG_MSG)
                    printf("GAP Device Discovery\n");
#endif
                    processEventGAPDeviceDiscovery(data);
                }
            }
        } else if ((unsigned char)data[1] == LOW_ENERGY) {

        } else if ((unsigned char)data[1] == COMMAND_COMPLETE) {
#if defined (TEST_CODE)
            printf("Command complete !\n");
            printf("Data len: %d\n", data[2]);
            printf("Data Code: %02x%02x \n",(unsigned char)data[4], (unsigned char)data[3]);
            printf("Data: ");
            for (int var = 5; var < len; var++) {
                printf("%02x:",(unsigned char) data[var]);
            }
            printf("\n");
#endif

        } else {
            printf("Error, can't process this event: %02x\n",(unsigned char)data[1]);
        }
    } else {

    }
    return status;
}

bool BLE::processEventGAPDeviceDone(std::vector<char> data) {
    lock_guard<mutex> l(readQueueMutex);
#if defined (DEBUG_MSG)
    printf("processEventGAPDeviceDone()\n");
#endif
    setDongleAddress(data);
//    setModuleIRK(data+IRK_POS);
//    setModuleCSRK(data+CSRK_POS);
    module_initialized = true;
    return true;
}

bool BLE::processEventGAPDeviceInfomation(std::vector<char> data) {
    lock_guard<mutex> l(readQueueMutex);
    // 0x05: status
    // 0x06: event type
    //          0x00: Connectable undirected advertisement
    //          0x01: Connectable directed advertisement
    //          0x02: Discoverable undirected advertisement
    //          0x03: Non-connectable undirected advertisement
    //          0x04: Scan response
    // 0x07: addr type
    // 0x08: addr (6 bytes)
    // 0x0E: rssi
    // 0x0F: dataLen
    // 0x10: dataField
    if (data[DEVICE_INFOMATION_STATUS] == 0x00) {
//        BLEDevice device = BLEDevice();
//        device.setEventType((unsigned char)data[DEVICE_INFOMATION_STATUS_POS +1]);
//        device.setAddressType((unsigned char)data[DEVICE_INFOMATION_STATUS_POS +2]);
//        device.setAddress(data+DEVICE_INFOMATION_STATUS_POS +3);
//        device.setRSSIValue((unsigned char)data[data+DEVICE_INFOMATION_STATUS_POS +9]);
#if defined (TEST_CODE)
        printf("\nDevice: ");
        for(int i = 0; i <data.size(); i++) {
            printf("%02x ", (unsigned char)data[i]);
        }
        printf("\n");
#endif
        if(data[DEVICE_INFOMATION_STATUS+1] == 0x00) { // Connectable undirected advertisement
#if defined (TEST_CODE)
            printf("\nDevice: ");
            for(int i = 0; i <data.size(); i++) {
                printf("%02x ", (unsigned char)data[i]);
            }
            printf("\n");
#endif
            bool result = isiBeaconDevice(data);
            if (!result) return false;
        } else if (data[DEVICE_INFOMATION_STATUS+1] == 0x04) {


        }
    } else {

    }
    return true;
}

bool BLE::processEventGAPDeviceDiscovery(std::vector<char> data) {
    lock_guard<mutex> l(readQueueMutex);
    if(data[DEVICE_DISCOVERY_STATUS] == 0x00) {
        number_of_device_found = data[DEVICE_DISCOVERY_STATUS + 1];
        if(number_of_device_found == 0) {
//#if defined (DEBUG)
            printf("Device discovery done, no device found\n");
//#endif
        } else {
            printf("Device discovery done, found %d device(s)\n", number_of_device_found);
#if 1//defined (TEST_CODE)
            for(int i = 0; i< devices_found.size(); i++) {
                std::string device = devices_found[i];
                for(int j = 0; j < device.size(); j++) {
                    printf("%02x ", (unsigned char)device[j]);
                }
                printf("\n");
            }
#endif
            //devices_found.clear();
        }
    }
    scan_finished = true;
    return true;
}

bool BLE::processEventGAPLinkEstablished(std::vector<char> data) {
    lock_guard<mutex> l(readQueueMutex);
    return true;
}

bool BLE::processEventGAPLinkTerminated(std::vector<char> data) {
    lock_guard<mutex> l(readQueueMutex);
    return true;
}

bool BLE::processEventGAPHCIExtCommandStatus(std::vector<char> data) {
    lock_guard<mutex> l(readQueueMutex);
    return true;
}

bool BLE::processEventATTReadByTypeResponse(std::vector<char> data) {
    lock_guard<mutex> l(readQueueMutex);
    return true;
}

bool BLE::processEventATTWriteResponse(std::vector<char> data) {
    lock_guard<mutex> l(readQueueMutex);
    return true;
}

bool BLE::processEventATTHandleValueNotification(std::vector<char> data) {
    lock_guard<mutex> l(readQueueMutex);
    return true;
}

bool BLE::processEventNoMatch(std::vector<char> data) {
    lock_guard<mutex> l(readQueueMutex);
    return true;
}

double BLE::calculateDistance(char txPower, char rssi) {
    if (rssi == 0) {
        return -1.0; // if we cannot determine accuracy, return -1.
    }

    double ratio = rssi*1.0/txPower;
    if (ratio < 1.0) {
        return pow(ratio,10);
    }
    else {
        double accuracy =  (0.89976)*pow(ratio,7.7095) + 0.111;
        return accuracy;
    }
}
