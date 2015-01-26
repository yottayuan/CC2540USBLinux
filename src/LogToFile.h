#ifndef LOGTOFILE_H_
#define LOGTOFILE_H_

#include "Singleton.h"
#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>
#include <stdio.h>
#include <fstream>
#include <math.h>
using namespace std;

namespace framework {
class LogToFile: public Singleton<LogToFile> {
    friend class Singleton<LogToFile>;
public:
    LogToFile();
    virtual ~LogToFile();
    bool writeStringToFile(std::string str);
    bool writeVectorToFile(std::vector<std::string> data);
    std::string convertCharToHexString(char val);
    std::string convertCharToDecString(char val);
    std::string convertDoubleToString(double val);
private:
    std::string file_path;
    double calculateDistance(char txPower, char rssi);

};
}

#endif
