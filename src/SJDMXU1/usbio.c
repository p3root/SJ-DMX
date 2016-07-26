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


#include "usbio.h"

#define LIB_VERSION "0.1"

#define VENDOR 0x0483
#define PRODUCT 0x57fe

struct ShowJockeyDeviceNode *_devices = 0;
int _deviceCount = 0;

const char *getDllVersion()
{
    return LIB_VERSION;
}

int removeAllDevices()
{
    if(_devices)
    {
        struct ShowJockeyDeviceNode *cur = _devices;
        while(cur->next != 0)
        {
            struct ShowJockeyDeviceNode *next = cur->next;
            cur = next->next;
            free(next);
        }
    }
    return 0;
}


int scanShowJockeyDevices()
{
    removeAllDevices();

    int di, ii, ai, ei, device_count;
    struct libusb_device_descriptor device_descriptor;
    struct libusb_device **device_list;
    struct libusb_config_descriptor *device_config;

    if (libusb_init(0)) {
        return 0;
    }

    device_count = libusb_get_device_list(0, &device_list);
    if (device_count <= 0) {
        return 0;
    }
    struct ShowJockeyDeviceNode* devices = 0;
    struct ShowJockeyDeviceNode* cur = 0;

    for (di = 0; di < device_count; di++)
    {
        if (!libusb_get_device_descriptor(device_list[di], &device_descriptor))
        {
            if (device_descriptor.idVendor == VENDOR && device_descriptor.idProduct == PRODUCT)
            {
                if(cur == 0)
                {
                    cur = (struct ShowJockeyDeviceNode*)malloc(sizeof(struct ShowJockeyDeviceNode));
                    cur->next = 0;
                }
                else
                {
                    cur = cur->next;
                    cur = (struct ShowJockeyDeviceNode*)malloc(sizeof(struct ShowJockeyDeviceNode));
                    cur->next = 0;
                }

                if(devices == 0)
                    devices = cur;

                cur->device->open = 0;
                //  cur->device->_handle = 0;
                cur->device = (struct ShowJockeyDevice*)malloc(sizeof(struct ShowJockeyDevice));
                cur->device->busNumber = libusb_get_bus_number(device_list[di]);
                cur->device->portNumber = libusb_get_device_address(device_list[di]);
                //cur->next = (struct ShowJockeyDeviceNode*)malloc(sizeof(struct ShowJockeyDeviceNode));

                _deviceCount++;

                if (!libusb_get_config_descriptor(device_list[di], 0, &device_config))
                {
                    for (ii = 0; ii < device_config->bNumInterfaces; ii++)
                    {
                        for (ai = 0; ai < device_config->interface[ii].num_altsetting; ai++)
                        {
                            for (ei = 0; ei < device_config->interface[ii].altsetting[ai].bNumEndpoints; ei++)
                            {
                                int attributes = device_config->interface[ii].altsetting[ai].endpoint[ei].bmAttributes;

                                if(attributes == LIBUSB_TRANSFER_TYPE_BULK)
                                {
                                    int endpoint = device_config->interface[ii].altsetting[ai].endpoint[ei].bEndpointAddress;

                                    int input = (device_config->interface[ii].altsetting[ai].endpoint[ei].bEndpointAddress & LIBUSB_ENDPOINT_DIR_MASK) == LIBUSB_ENDPOINT_IN;
                                    if(input)
                                    {
                                        cur->device->pipeIn = endpoint;
                                        cur->device->maxPacketSizeIn = device_config->interface[ii].altsetting[ai].endpoint[ei].wMaxPacketSize;
                                        continue;
                                    }
                                    else
                                    {
                                        cur->device->pipeOut = endpoint;
                                        cur->device->maxPacketSizeOut = device_config->interface[ii].altsetting[ai].endpoint[ei].wMaxPacketSize;
                                        continue;
                                    }
                                }
                            }
                        }
                    }
                }

            }
        }
    }


    libusb_free_device_list(device_list, 1);

    _devices = devices;

    return 0;
}

int getShowJockeyDeviceCount()
{
    return _deviceCount;
}


struct ShowJockeyDeviceNode *getShowJockeyDevices()
{
    return _devices;
}


int openShowJockeyDevice(struct ShowJockeyDevice *dev)
{
    if(!dev)
        return 1;

    int device_count, di;
    struct libusb_device_descriptor device_descriptor;
    struct libusb_device **device_list;

    device_count = libusb_get_device_list(0, &device_list);
    if (device_count <= 0) {
        return 1;
    }

    for (di = 0; di < device_count; di++)
    {
        if (!libusb_get_device_descriptor(device_list[di], &device_descriptor))
        {
            if (device_descriptor.idVendor == VENDOR && device_descriptor.idProduct == PRODUCT
                    && libusb_get_bus_number(device_list[di]) == dev->busNumber
                    && libusb_get_device_address(device_list[di]) == dev->portNumber)
            {
                int opened = libusb_open(device_list[di], &dev->_handle);
                if(!opened)
                {
                    libusb_free_device_list(device_list, 1);
                    dev->open = 1;
                    return 0;
                }
            }
        }
    }

    libusb_free_device_list(device_list, 1);
    return 1;
}


void closeShowJockeyDevice(struct ShowJockeyDevice *dev)
{
    libusb_close(dev->_handle);
    dev->open = 0;
}

int bulkSync(struct ShowJockeyDevice* dev, int endpoint, int maxPacketSize, unsigned char *buffer, size_t size)
{
    if(!dev || !dev->_handle)
        return -1;

    if((int)size <= maxPacketSize)
    {
        int transferred = -1;
        libusb_bulk_transfer(dev->_handle, endpoint, buffer, size, &transferred, 3000);

        return transferred;
    }
    else
    {
        int retVal = 1;
        unsigned char *buf = buffer;
        int nPacket = maxPacketSize;
        int nLeft = (int)size;
        int transferred = -1;
        int transferredAll = 0;

        while(1)
        {
            if(nLeft <= 0)
                break;

            retVal = libusb_bulk_transfer(dev->_handle, endpoint, buffer, nPacket, &transferred, 3000);

            if(retVal)
                break;

            buf += transferred;
            nLeft -= transferred;
            nPacket = maxPacketSize;
            transferredAll += transferred;
        }

        if(transferredAll == (int)size)
            return transferredAll;

    }

    return -1;
}

unsigned char getShowJockeyDeviceMode(struct ShowJockeyDevice *dev)
{
    if(!dev)
        return 1;
    if(!dev->open)
        return 1;

    unsigned char cmd[3] = {0xff, 0xff, 0x00};

    if(bulkSync(dev, dev->pipeOut, dev->maxPacketSizeOut, cmd, 3))
    {
        usleep(3000);

        if(bulkSync(dev, dev->pipeIn, dev->maxPacketSizeIn, cmd, 3))
            return cmd[2];
    }
    return 0;

}


int getShowJockeyDeviceBuffer(struct ShowJockeyDevice *dev, unsigned char *buffer, size_t size)
{
    if(!dev)
        return -1;
    if(!dev->open)
        return -1;

    unsigned char cmd[3] = {0xff, 0xff, 0x01};

    if(!bulkSync(dev, dev->pipeOut, dev->maxPacketSizeOut, cmd, 3))
    {
        return -1;
    }

    usleep(3000);

    unsigned char readBuf[size];
    bzero(readBuf, size);

    if(!bulkSync(dev, dev->pipeIn, dev->maxPacketSizeIn, readBuf, size))
    {
        return -1;
    }

    memcpy(buffer, &readBuf, size);

    return size;

}


int sendShowJockeyDeviceBuf(struct ShowJockeyDevice *dev, unsigned char *buffer, size_t size)
{
    if(!dev)
        return -1;
    if(!dev->open)
        return -1;

    int retVal = 0;
    int leftWriteSize = (int)size;
    unsigned short alreadyWrittenSize = 0;
    int writeSize = 0;
    int maxPacketSizeOut = dev->maxPacketSizeOut;

    unsigned char *writeBuffer = buffer;

    unsigned char *bulkBuffer = (unsigned char*)malloc(maxPacketSizeOut);

    int bulkDataSize = maxPacketSizeOut - 2;

    while(leftWriteSize > 0) {
        memset(bulkBuffer, 0, maxPacketSizeOut);
        memcpy(bulkBuffer, &alreadyWrittenSize, 2);

        if(leftWriteSize > bulkDataSize) {
            writeSize = bulkDataSize;
        }
        else {
            writeSize = leftWriteSize;
        }

        memcpy((bulkBuffer+2), writeBuffer, writeSize);

        retVal = bulkSync(dev, dev->pipeOut, maxPacketSizeOut, bulkBuffer, writeSize+2);

        if(retVal < 0)
        {
            return retVal;
        }
        leftWriteSize -= writeSize;
        alreadyWrittenSize += writeSize;
        writeBuffer += writeSize;
    }

    if(bulkBuffer)
    {
        free(bulkBuffer);
        bulkBuffer = 0;
    }

    return alreadyWrittenSize;
}


void clearShowJockeyDevices()
{
    if(!_devices)
        return;

    struct ShowJockeyDeviceNode *node = _devices;

    while(node != 0)
    {
         struct ShowJockeyDeviceNode *nextNode = node->next;

        if(node->device->open)
        {
            closeShowJockeyDevice(node->device);
        }

        free(node->device);
        free(node);

        node = nextNode;
    }
}
