/*
 *  Copyright (c) 2013-2014, p3root - Patrik Pfaffenbauer (patrik.pfaffenbauer@p3.co.at)
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification,
 *  are permitted provided that the following conditions are met:
 *
 *    Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright notice, this
 *    list of conditions and the following disclaimer in the documentation and/or
 *    other materials provided with the distribution.
 *
 *    Neither the name of the organisation nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 *  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 *  ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#ifndef USBIO_H
#define USBIO_H

#include <libusb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifdef  __cplusplus
# define USBIO_BEGIN_DECLS  extern "C" {
# define USBIO_END_DECLS    }
#else
# define USBIO_BEGIN_DECLS
# define USBIO_END_DECLS
#endif

USBIO_BEGIN_DECLS

struct ShowJockeyDevice {
    unsigned char portNumber;
    unsigned char busNumber;
    unsigned char pipeIn;
    unsigned char pipeOut;
    unsigned short maxPacketSizeIn;
    unsigned short maxPacketSizeOut;
    int open;
    libusb_device_handle *_handle;

};

struct ShowJockeyDeviceNode {
        struct ShowJockeyDevice *device;
        struct ShowJockeyDeviceNode *next;
};

///
/// \brief getDllVersion
/// \return returns the assembly version
///
const char *getDllVersion();

///
/// \brief Scans for all ShowJockey devices
/// \return > 1 for success, othwerwise 0 for an error
///
int scanShowJockeyDevices();

///
/// \brief Clears the memory
///
void clearShowJockeyDevices();

///
/// \brief Gets the amount of deivces
/// \return  The amount of the devices
///
int getShowJockeyDeviceCount();

///
/// \brief Returns the list of found devices
/// \return
///
struct ShowJockeyDeviceNode *getShowJockeyDevices();

///
/// \brief Opens the ShowJockey USB Device
/// \param Device
/// \return 0 if success, 1 if an error occured
///
int openShowJockeyDevice(struct ShowJockeyDevice *dev);

///
/// \brief closeShowJockeyDevice
/// \param dev
///
void closeShowJockeyDevice(struct ShowJockeyDevice *dev);

///
/// \brief getShowJockeyDeviceMode
/// \param dev
/// \return
///
unsigned char getShowJockeyDeviceMode(struct ShowJockeyDevice *dev);

///
/// \brief getShowJockeyDeviceBuffer
/// \param dev
/// \param buffer
/// \param size
/// \return
///
int getShowJockeyDeviceBuffer(struct ShowJockeyDevice *dev, unsigned char *buffer, size_t size);

///
/// \brief sendShowJockeyDeviceBuf
/// \param dev
/// \param buffer
/// \param size
/// \return
///
int sendShowJockeyDeviceBuf(struct ShowJockeyDevice *dev, unsigned char *buffer, size_t size);

USBIO_END_DECLS

#endif // USBIO_H
