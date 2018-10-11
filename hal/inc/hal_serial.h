/*
 *  hal_serial.h
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

#ifndef SRC_IEC60870_LINK_LAYER_SERIAL_PORT_H_
#define SRC_IEC60870_LINK_LAYER_SERIAL_PORT_H_

#include "hal_base.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \file hal_serial.h
 * \brief Abstraction layer for serial ports.
 * Has to be implemented for the serial link layer of CS 101.
 */

/*! \addtogroup hal Platform (Hardware/OS) abstraction layer
   *
   *  @{
   */

/**
 * @defgroup HAL_SERIAL Access to serial interfaces
 *
 *  Serial interface abstraction layer. This functions have to be implemented to
 *  port lib60870 to new platforms when the serial link layers are required.
 *
 * @{
 */

typedef struct sSerialPort* SerialPort;

typedef enum {
    SERIAL_PORT_ERROR_NONE = 0,
    SERIAL_PORT_ERROR_INVALID_ARGUMENT = 1,
    SERIAL_PORT_ERROR_INVALID_BAUDRATE = 2,
    SERIAL_PORT_ERROR_OPEN_FAILED = 3,
    SERIAL_PORT_ERROR_UNKNOWN = 99
} SerialPortError;

/**
 * \brief Create a new SerialPort instance
 *
 * \param interfaceName identifier or name of the serial interface (e.g. "/dev/ttyS1" or "COM4")
 * \param baudRate the baud rate in baud (e.g. 9600)
 * \param dataBits the number of data bits (usually 8)
 * \param parity defines what kind of parity to use ('E' - even parity, 'O' - odd parity, 'N' - no parity)
 * \param stopBits the number of stop buts (usually 1)
 *
 * \return the new SerialPort instance
 */
PAL_API SerialPort
SerialPort_create(const char* interfaceName, int baudRate, uint8_t dataBits, char parity, uint8_t stopBits);

/**
 * \brief Destroy the SerialPort instance and release all resources
 */
PAL_API void
SerialPort_destroy(SerialPort self);

/**
 * \brief Open the serial interface
 *
 * \return true in case of success, false otherwise (use \ref SerialPort_getLastError for a detailed error code)
 */
PAL_API bool
SerialPort_open(SerialPort self);

/**
 * \brief Close (release) the serial interface
 */
PAL_API void
SerialPort_close(SerialPort self);

/**
 * \brief Get the baudrate used by the serial interface
 *
 * \return the baud rate in baud
 */
PAL_API int
SerialPort_getBaudRate(SerialPort self);

/**
 * \brief Set the timeout used for message reception
 *
 * \param timeout the timeout value in ms.
 */
PAL_API void
SerialPort_setTimeout(SerialPort self, int timeout);

/**
 * \brief Discard all data in the input buffer of the serial interface
 */
PAL_API void
SerialPort_discardInBuffer(SerialPort self);

/**
 * \brief Read a byte from the interface
 *
 * \return number of read bytes of -1 in case of an error
 */
PAL_API int
SerialPort_readByte(SerialPort self);

/**
 * \brief Write the number of bytes from the buffer to the serial interface
 *
 * \param buffer the buffer containing the data to write
 * \param startPos start position in the buffer of the data to write
 * \param numberOfBytes number of bytes to write
 *
 * \return number of bytes written, or -1 in case of an error
 */
PAL_API int
SerialPort_write(SerialPort self, uint8_t* buffer, int startPos, int numberOfBytes);

/**
 * \brief Get the error code of the last operation
 */
PAL_API SerialPortError
SerialPort_getLastError(SerialPort self);

/*! @} */

/*! @} */

#ifdef __cplusplus
}
#endif


#endif /* SRC_IEC60870_LINK_LAYER_SERIAL_PORT_H_ */
