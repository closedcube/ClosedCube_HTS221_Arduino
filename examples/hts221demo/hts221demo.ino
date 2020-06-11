/*

Example: hts221demo

Arduino library for Arduino library for Sensirion Relative Humidity and Temperature Sensor
version 2020.6.11

---

Copyright (c) 2018-2020, ClosedCube
All rights reserved.

Redistribution and use in source and binary forms, with or without 
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, 
   this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, 
   this list of conditions and the following disclaimer in the documentation and/or 
   other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors 
   may be used to endorse or promote products derived from this software
   without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR 
PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; 
OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#include "ClosedCube_HTS221.h"

#define UART_BAUD 9600
#define HTS221_I2C_ADDRESS 0x5F

ClosedCube::Sensor::HST221 hts221;

void setup() {
    Serial.begin(UART_BAUD);
    Wire.begin();

    Serial.println("ClosedCube HTS221 Arduino Demo");
    hts221.address(HTS221_I2C_ADDRESS);

    Serial.print("HTS221 Who Am I? 0x");    

    uint8_t whoIam = hts221.whoAmI();
    Serial.println(whoIam, HEX);

    if( whoIam != 0xBC ) {
        Serial.print("WARNING: Received ");
        Serial.print(whoIam);
        Serial.println(", but expected 0xBC");
    }

    hts221.init();

}

void loop() {
    hts221.oneShot();
    
    Serial.print("T=");
    Serial.print(hts221.readTemperature());
    Serial.print(" C; RH=");
    Serial.print(hts221.readHumidity());
    Serial.println(" %");

    delay(2000); // 2 seconds
}



