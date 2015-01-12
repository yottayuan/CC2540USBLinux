#ifndef CC2540USBDONGLE_H
#define CC2540USBDONGLE_H
#include "AsyncSerial.h"
#include "BLE.h"
namespace framework {
class CC2540USBDongle :public Singleton<CC2540USBDongle>{
    friend class Singleton<CC2540USBDongle>;
public:
    CC2540USBDongle();
    virtual ~CC2540USBDongle();
    bool init();
    bool scan();
    bool isScanFinished();
    bool clearDevicesList();
private:
    CallbackAsyncSerial *serial;
    //BLE *ble;
};
}


#endif
