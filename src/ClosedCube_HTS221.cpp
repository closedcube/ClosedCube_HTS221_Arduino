/*

Arduino library for Arduino library for Sensirion Relative Humidity and Temperature Sensor
version 2020.6.15

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

#include <stdint.h>
#include "ClosedCube_HTS221.h"


ClosedCube::Sensor::HST221::HST221()
{
}

ClosedCube::Sensor::HST221::HST221(ClosedCube::Driver::I2CDevice device) : _sensor(device)
{
}

ClosedCube::Sensor::HST221::HST221(uint8_t address)
{
    _sensor.address(address);
}

void ClosedCube::Sensor::HST221::address(uint8_t address)
{
    _sensor.address(address);
}

uint8_t ClosedCube::Sensor::HST221::whoAmI() {
    return _sensor.readByteFromReg(0x0F,0);
}

void ClosedCube::Sensor::HST221::init() {
    _rh_H0 = _sensor.readByteFromReg(0x30) / 2.0;
	_rh_H1 = _sensor.readByteFromReg(0x31) / 2.0;

    _rh_H0_OUT = _sensor.readByteFromReg(0x37) << 8;
    _rh_H0_OUT |= _sensor.readByteFromReg(0x36);

	_rh_H1_OUT = _sensor.readByteFromReg(0x3B) << 8;
    _rh_H1_OUT |= _sensor.readByteFromReg(0x3A);

    uint8_t reg_T1_T0_msb = _sensor.readByteFromReg(0x35);   

    uint16_t T0_degC_x8 = (reg_T1_T0_msb & 0x3) << 8;
    T0_degC_x8 |= _sensor.readByteFromReg(0x32);

    uint16_t T1_degC_x8 = (reg_T1_T0_msb & 0xC) << 6;
    T1_degC_x8 |= _sensor.readByteFromReg(0x33);

    _t_T0 = T0_degC_x8 / 8.0; 
    _t_T1 = T1_degC_x8 / 8.0;

    _t_T0_OUT = _sensor.readByteFromReg(0x3D) << 8;
    _t_T0_OUT |= _sensor.readByteFromReg(0x3C);

    _t_T1_OUT = _sensor.readByteFromReg(0x3F) << 8;
    _t_T1_OUT |= _sensor.readByteFromReg(0x3E);

    _sensor.writeByteToReg(0x20, 0x84);
}

uint8_t ClosedCube::Sensor::HST221::oneShot() {
    uint8_t result = 0;

    uint8_t max_wait = 0xFF;
    while( (_sensor.readByteFromReg(0x21) << 7) != 0x00 ) {
        delay(1);
        --max_wait;
        if( max_wait == 0 ) {
            result = 1;
            break;
        }
    }

    if( result == 0 ) {
        _sensor.writeByteToReg(0x21,0x01);
        while( _sensor.readByteFromReg(0x27) != 0x03) {
            yield();
        }
    }

    return result;
}


float ClosedCube::Sensor::HST221::readT() {
    return readTemperature();
}

float ClosedCube::Sensor::HST221::readRH() {
    return readHumidity();
}

float ClosedCube::Sensor::HST221::readTemperature() {
    int16_t t_out = _sensor.readByteFromReg(0x2B) << 8;
    t_out |= _sensor.readByteFromReg(0x2A);
    
    return (_t_T0 + (t_out-_t_T0_OUT) * (_t_T1-_t_T0) / (_t_T1_OUT-_t_T0_OUT)); 
}

float ClosedCube::Sensor::HST221::readHumidity() {
    int16_t rh_H_OUT = (((int16_t)_sensor.readByteFromReg(0x29)) << 8) | ((int16_t)_sensor.readByteFromReg(0x28));
    
    return _rh_H0 + (rh_H_OUT-_rh_H0_OUT) * (_rh_H1-_rh_H0) / (_rh_H1_OUT-_rh_H0_OUT);
}
