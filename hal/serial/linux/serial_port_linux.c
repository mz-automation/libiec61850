/*
 *  serial_port_linux.c
 *
 *  Copyright 2017 MZ Automation GmbH
 *
 *  This file is part of lib60870-C
 *
 *  lib60870-C is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  lib60870-C is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with lib60870-C.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  See COPYING file for the complete license text.
 */

#include "lib_memory.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/select.h>

#include "hal_serial.h"
#include "hal_time.h"

struct sSerialPort {
    char interfaceName[100];
    int fd;
    int baudRate;
    uint8_t dataBits;
    char parity;
    uint8_t stopBits;
    uint64_t lastSentTime;
    struct timeval timeout;
    SerialPortError lastError;
};


SerialPort
SerialPort_create(const char* interfaceName, int baudRate, uint8_t dataBits, char parity, uint8_t stopBits)
{
    SerialPort self = (SerialPort) GLOBAL_MALLOC(sizeof(struct sSerialPort));

    if (self != NULL) {
        self->fd = -1;
        self->baudRate = baudRate;
        self->dataBits = dataBits;
        self->stopBits = stopBits;
        self->parity = parity;
        self->lastSentTime = 0;
        self->timeout.tv_sec = 0;
        self->timeout.tv_usec = 100000; /* 100 ms */
        strncpy(self->interfaceName, interfaceName, 99);
        self->lastError = SERIAL_PORT_ERROR_NONE;
    }

    return self;
}

void
SerialPort_destroy(SerialPort self)
{
    if (self != NULL) {
        GLOBAL_FREEMEM(self);
    }
}

bool
SerialPort_open(SerialPort self)
{
    self->fd = open(self->interfaceName, O_RDWR | O_NOCTTY | O_NDELAY | O_EXCL);

    if (self->fd == -1) {
        self->lastError = SERIAL_PORT_ERROR_OPEN_FAILED;
        return false;
    }

    struct termios tios;
    speed_t baudrate;

    tcgetattr(self->fd, &tios);

    switch (self->baudRate) {
    case 110:
        baudrate = B110;
        break;
    case 300:
        baudrate = B300;
        break;
    case 600:
        baudrate = B600;
        break;
    case 1200:
        baudrate = B1200;
        break;
    case 2400:
        baudrate = B2400;
        break;
    case 4800:
        baudrate = B4800;
        break;
    case 9600:
        baudrate = B9600;
        break;
    case 19200:
        baudrate = B19200;
        break;
    case 38400:
        baudrate = B38400;
        break;
    case 57600:
        baudrate = B57600;
        break;
    case 115200:
        baudrate = B115200;
        break;
    default:
        baudrate = B9600;
        self->lastError = SERIAL_PORT_ERROR_INVALID_BAUDRATE;
    }

    /* Set baud rate */
    if ((cfsetispeed(&tios, baudrate) < 0) || (cfsetospeed(&tios, baudrate) < 0)) {
        close(self->fd);
        self->fd = -1;
        self->lastError = SERIAL_PORT_ERROR_INVALID_BAUDRATE;
        return false;
    }

    tios.c_cflag |= (CREAD | CLOCAL);

    /* Set data bits (5/6/7/8) */
    tios.c_cflag &= ~CSIZE;
    switch (self->dataBits) {
    case 5:
        tios.c_cflag |= CS5;
        break;
    case 6:
        tios.c_cflag |= CS6;
        break;
    case 7:
        tios.c_cflag |= CS7;
        break;
    case 8:
    default:
        tios.c_cflag |= CS8;
        break;
    }

    /* Set stop bits (1/2) */
    if (self->stopBits == 1)
        tios.c_cflag &=~ CSTOPB;
    else /* 2 */
        tios.c_cflag |= CSTOPB;

    if (self->parity == 'N') {
        tios.c_cflag &=~ PARENB;
    } else if (self->parity == 'E') {
        tios.c_cflag |= PARENB;
        tios.c_cflag &=~ PARODD;
    } else { /* 'O' */
        tios.c_cflag |= PARENB;
        tios.c_cflag |= PARODD;
    }

    tios.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

    if (self->parity == 'N') {
        tios.c_iflag &= ~INPCK;
    } else {
        tios.c_iflag |= INPCK;
    }

    tios.c_iflag &= ~(IXON | IXOFF | IXANY | ICRNL);
    tios.c_iflag |= IGNBRK; /* Set ignore break to allow 0xff characters */
    tios.c_iflag |= IGNPAR;
    tios.c_oflag &=~ OPOST;

    tios.c_cc[VMIN] = 0;
    tios.c_cc[VTIME] = 0;

    if (tcsetattr(self->fd, TCSANOW, &tios) < 0) {
        close(self->fd);
        self->fd = -1;
        self->lastError = SERIAL_PORT_ERROR_INVALID_ARGUMENT;

        return false;
    }

    return true;
}

void
SerialPort_close(SerialPort self)
{
    if (self->fd != -1) {
        close(self->fd);
        self->fd = 0;
    }
}

int
SerialPort_getBaudRate(SerialPort self)
{
    return self->baudRate;
}

void
SerialPort_discardInBuffer(SerialPort self)
{
    tcflush(self->fd, TCIOFLUSH);
}

void
SerialPort_setTimeout(SerialPort self, int timeout)
{
    self->timeout.tv_sec = timeout / 1000;
    self->timeout.tv_usec = (timeout % 1000) * 1000;
}

SerialPortError
SerialPort_getLastError(SerialPort self)
{
    return self->lastError;
}

int
SerialPort_readByte(SerialPort self)
{
    uint8_t buf[1];
    fd_set set;

    self->lastError = SERIAL_PORT_ERROR_NONE;

    FD_ZERO(&set);
    FD_SET(self->fd, &set);

    int ret = select(self->fd + 1, &set, NULL, NULL, &(self->timeout));

    if (ret == -1) {
        self->lastError = SERIAL_PORT_ERROR_UNKNOWN;
        return -1;
    }
    else if (ret == 0)
        return -1;
    else {
        read(self->fd, (char*) buf, 1);

        return (int) buf[0];
    }
}

int
SerialPort_write(SerialPort self, uint8_t* buffer, int startPos, int bufSize)
{
    /* TODO assure minimum line idle time? */

    self->lastError = SERIAL_PORT_ERROR_NONE;

    ssize_t result = write(self->fd, buffer + startPos, bufSize);

    tcdrain(self->fd);

    self->lastSentTime = Hal_getTimeInMs();

    return result;
}
