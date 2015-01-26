#include "LogToFile.h"

using namespace framework;
using namespace std;

LogToFile::LogToFile(): file_path("log.txt") {
}

LogToFile::~LogToFile() {

}

bool LogToFile::writeStringToFile(std::string str) {
    ofstream file;
    file.open(file_path.c_str(), ios::app|ios::ate );
    file << str;
    file.close();
    return true;
}

bool LogToFile::writeVectorToFile(std::vector<std::string> data) {
    // use this to write result of CC2540 scanning process
    std::string str = "";
    for (int i = 0; i< data.size(); i++) {
        std::string tmp = data[i];
        for (int j = 0; j<tmp.length(); j++) {
            if (j != 5) {
                if( j != 6) {
                   str+= convertCharToHexString(tmp[j]);
                   str+= ":";
                } else {
                    str+= convertCharToDecString(tmp[j]);
                    double d = calculateDistance(0xcd,tmp[j]);
                    str+= " ";
                    str+= convertDoubleToString(d);
                }
            } else {
                str+= convertCharToHexString(tmp[j]);
                str+= " ";
            }
        }
        str+= " ";
    }
    str+= "\n";
    printf("String to write: %s", str.c_str());
    writeStringToFile(str);
    return true;
}

std::string LogToFile::convertCharToHexString(char val) {
    char buff[10];
    int n = sprintf(buff,"%02x", (unsigned char)val);
    //printf("Hex_val:%s\n",buff);
    return std::string(buff);
}

std::string LogToFile::convertCharToDecString(char val) {
    char buff[10];
    int n = sprintf(buff,"%d", val);
    return std::string(buff);
}

std::string LogToFile::convertDoubleToString(double val) {
    char buff[10];
    int n = sprintf(buff,"%4.2f",val);
    return std::string(buff);
}

double LogToFile::calculateDistance(char txPower, char rssi) {
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
