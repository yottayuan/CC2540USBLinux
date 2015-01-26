
#include <iostream>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include "CC2540USBDongle.h"

using namespace std;
using namespace framework;

int main()
{
    //printf("Send init command\n");
    //CC2540USBDongle::Instance()->init();
    //printf("Send discovery command\n");
    CC2540USBDongle::Instance()->scan();
    while(true) {
        boost::this_thread::sleep(boost::posix_time::milliseconds(500));
//        bool status = false;
//        status = CC2540USBDongle::Instance()->isScanFinished();
//        if (status) {
//            CC2540USBDongle::Instance()->scan();
//        }
    }
}
