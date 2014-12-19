
#include <iostream>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include "AsyncSerial.h"
#include "BLE.h"
//#define  INITIALIZED

using namespace std;

int main()
{
    BLE cc2540;
    cc2540 = BLE();
    CallbackAsyncSerial serial("/dev/ttyACM0",57600);

	//Bind the received() member function of the foo instance,
	//_1 and _2 are parameter forwarding placeholders
    serial.setCallback(boost::bind(&BLE::received,cc2540,_1,_2));
    std::vector<char> data;
    std::vector<char> cmd = cc2540.getGAPCommand((unsigned char)GAP_DEVICE_INITIALIZATION, data);
    //serial.writeString("Hello world\n");
//    for(int i = 0; i<cmd.size(); i++) {
//        printf("%02x:", (unsigned char)cmd[i]);
//        if((i%16 == 0)&& i!=0) printf("\n");
//    }
//    printf("\n");
#if !defined (INITIALIZED)
    serial.write(cmd);
#endif
    printf("Send init command\n");
#if !defined (INITIALIZED)
    bool *status;
    *status = false;
    while(!(*status)) {
        //printf("Module status: %d\n", cc2540.isModuleInitialized());
        *status = cc2540.getModuleStatus();
    }
#endif
    printf("Send discovery command\n");
    cmd.clear();
    cmd = cc2540.getGAPCommand(GAP_DEVICE_DISCOVERY_REQUEST,data);
    serial.write(cmd);
    for(int i = 0; i<cmd.size(); i++) {
        printf("%02x:", (unsigned char)cmd[i]);
        if((i%16 == 0)&& i!=0) printf("\n");
    }
    printf("\n");
    while(true);
    boost::this_thread::sleep(boost::posix_time::seconds(3));
//    while(true) {
//        if(event.isModuleInitialized()) {
//            cmd.clear();
//            cmd = cc2540.getGAPCommand(GAP_DEVICE_DISCOVERY_REQUEST,data);
//            serial.write(cmd);
//        }
//    }
	serial.clearCallback();
}
