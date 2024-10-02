#include "Serial.hpp"

#include "Volume.hpp"
#include "Player.hpp"

// Setup serial port
int Serial::setupSerial(const char *portName) {
    int serial_port = open(portName, O_RDWR);

    if (serial_port < 0) {
        std::cerr << "Error " << errno << " opening " << portName << ": " << strerror(errno) << std::endl;
        return -1;
    }

    struct termios tty;

    if (tcgetattr(serial_port, &tty) != 0) {
        std::cerr << "Error " << errno << " from tcgetattr: " << strerror(errno) << std::endl;
        return -1;
    }

    tty.c_cflag &= ~PARENB;         // No parity bit
    tty.c_cflag &= ~CSTOPB;         // 1 stop bit
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;             // 8 bits per byte
    tty.c_cflag &= ~CRTSCTS;        // Disable RTS/CTS hardware flow control
    tty.c_cflag |= CREAD | CLOCAL;  // Turn on read & ignore control lines (CLOCAL)

    tty.c_lflag &= ~ICANON;
    tty.c_lflag &= ~ECHO;           // Disable echo
    tty.c_lflag &= ~ECHOE;          // Disable erasure
    tty.c_lflag &= ~ECHONL;         // Disable new-line echo
    tty.c_lflag &= ~ISIG;           // Disable interpretation of INTR, QUIT and SUSP

    tty.c_iflag &= ~(IXON | IXOFF | IXANY);  // Turn off software flow control
    tty.c_iflag &= ~(ICRNL | INLCR);         // Disable special handling of carriage returns and line feeds

    tty.c_oflag &= ~OPOST;          // Prevent special interpretation of output bytes (e.g. newline chars)

    // Set the baud rate to 9600
    cfsetispeed(&tty, B9600);
    cfsetospeed(&tty, B9600);

    // Save tty settings
    if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
        std::cerr << "Error " << errno << " from tcsetattr: " << strerror(errno) << std::endl;
        return -1;
    }

    return serial_port;
}

// Send data to the S32K144
void Serial::sendToS32K144(int serial_port, const std::string &data) {
    write(serial_port, data.c_str(), data.size());
    // write played_time
    // write duration
    // write title
}

// Receive data from the S32K144
std::string Serial::receiveFromS32K144(int serial_port) {
    char buffer[256];
    memset(&buffer, '\0', sizeof(buffer));
    int num_bytes = read(serial_port, &buffer, sizeof(buffer));

    if (num_bytes < 0) {
        std::cerr << "Error reading from serial port: " << strerror(errno) << std::endl;
    }

    return std::string(buffer);
}

void Serial::Connect() {
    int serial_port = setupSerial(serialPort);

    if (serial_port < 0) {
        return;
    }

    Player& player = Player::getInstance();

    std::string comnmand;
    while (true) {
        // Receive command form S32K144
        comnmand = receiveFromS32K144(serial_port);
        if (comnmand == "VOLUP") {
            player.getVolume().upVolume();
        } else if (comnmand == "VOLDOWN") {
            player.getVolume().downVolume();
        }
        // Send playback info to S32K144
        
    }
}