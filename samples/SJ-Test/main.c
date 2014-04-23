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

#include <stdio.h>
#include <usbio.h>

int main(void)
{
    scanShowJockeyDevices();

    struct ShowJockeyDeviceNode *devices = getShowJockeyDevices();
    if(!devices)
        return -1;

    if(!openShowJockeyDevice(devices->device))
    {
       // unsigned char devMode = getShowJockeyDeviceMode(devices->device);

        unsigned char buffer[512];
        bzero(buffer, 512);

       // getShowJockeyDeviceBuffer(devices->device, buffer, 512);

        unsigned char *dmxdata = (unsigned char *)malloc(512);
        memset(dmxdata, 0, 512);


        unsigned char red = 5, green = 5, blue = 5, white=5, muster=5;
        while (1)
        {
            dmxdata[0] = red;
            dmxdata[1] = blue;
            dmxdata[2] = green;
            dmxdata[3] = white;

            red++;
            white++;
            blue++;
            green++;
            muster++;

            int trans = sendShowJockeyDeviceBuf(devices->device, dmxdata, 512);

            int recv = getShowJockeyDeviceBuffer(devices->device, buffer, 512);


            usleep(1000);
            //exit(0);
        }

    }

    clearShowJockeyDevices();

    return 0;
}

