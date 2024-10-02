#ifndef SERIAL_HPP
#define SERIAL_HPP

#include <iostream>
#include <fcntl.h>      // Contains file controls like O_RDWR
#include <unistd.h>     // write(), read(), close()
#include <termios.h>    // Contains POSIX terminal control definitions
#include <cstring>

class Serial {
    const char* serialPort;
    //int serial_port = setupSerial(serialPort);

public:
    int setupSerial(const char *portName);
    void sendToS32K144(int serial_port, const std::string &data);
    std::string receiveFromS32K144(int serial_port);

    void Connect();
};

#endif