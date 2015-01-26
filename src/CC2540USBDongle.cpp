#include "CC2540USBDongle.h"
using namespace framework;

CC2540USBDongle::CC2540USBDongle() {
    if(serial == NULL) {
        //serial = new CallbackAsyncSerial("/dev/ttyACM0",115200);
        serial = new CallbackAsyncSerial("/dev/ttyUSB0",57600);
        serial->setCallback(boost::bind(&BLE::received,BLE::Instance(),_1,_2));
    }
}

CC2540USBDongle::~CC2540USBDongle() {
    serial->close();
    serial->clearCallback();
}

bool CC2540USBDongle::init() {
    std::vector<char> data;
    std::vector<char> cmd = BLE::Instance()->getGAPCommand((unsigned char)GAP_DEVICE_INITIALIZATION, data);
    printf("Write command\n");
    serial->write(cmd);
    bool status = false;
    status = BLE::Instance()->isModuleInitialized();
    boost::this_thread::sleep(boost::posix_time::milliseconds(100));
    while(!status) {
        status = BLE::Instance()->isModuleInitialized();
        boost::this_thread::sleep(boost::posix_time::milliseconds(100));
    }
    printf("Device is ready !\n");
    // set time for scan
    data.clear();
    data.push_back(0x03);
    data.push_back(0x02);
    data.push_back(0x00);
    data.push_back(0x0a);
    cmd.clear();
    cmd = BLE::Instance()->getGAPCommand((unsigned char) GAP_SET_PARAMETER, data);
#if defined (TEST_CODE)
    printf("Command:");
    for(int i = 0; i < cmd.size(); i++) {
        printf("%02x ", (unsigned char) cmd[i]);
    }
    printf("\n");
#endif
    serial->write(cmd);
    status = false;
    boost::this_thread::sleep(boost::posix_time::milliseconds(100));
    while(!status) {
        status = BLE::Instance()->isSendCMDCompleted();
    }
}

bool CC2540USBDongle::scan() {
    std::vector<char> data;
    std::vector<char> cmd = BLE::Instance()->getGAPCommand(GAP_DEVICE_DISCOVERY_REQUEST,data);
    //printf("Send discovery command\n");
    serial->write(cmd);
}

bool CC2540USBDongle::isScanFinished() {
    return BLE::Instance()->isScanFinished();
}

bool CC2540USBDongle::clearDevicesList() {
    return BLE::Instance()->clearDevicesList();
}
