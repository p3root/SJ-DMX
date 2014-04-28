SJ-DMX
=======
ShowJockey DMX Adapter lib

A linux library to use USB DMX Adapter from ShowJockey (http://www.showjockey.com/).

Usage
======
You need automake tools to build this library. Just run
```
./autogen.sh
make
make install
```

Scan for ShowJockey USB Devices
```
 scanShowJockeyDevices();
```
Get the list of ShowJockey USB Devices

```
struct ShowJockeyDeviceNode *devices = getShowJockeyDevices();
```
Open the ShowJockey device
```
openShowJockeyDevice(devices->device);
```
Write to the ShowJockey device
```
unsigned char *dmxdata = (unsigned char *)malloc(512);
memset(dmxdata, 0, 512);
sendShowJockeyDeviceBuf(devices->device, dmxdata, 512);
```

Thanks
=======
Special thanks to ShowJockey. They give me the source code of their own MAC OS driver, and gave me the permission to publish my code as an open source project.

License
=======
Copyright (c) 2013-2014, p3root - Patrik Pfaffenbauer (patrik.pfaffenbauer@p3.co.at) All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

Neither the name of the organisation nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
