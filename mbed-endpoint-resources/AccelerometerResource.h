/**
 * @file    AccelerometerResource.h
 * @brief   mbed CoAP Endpoint Accelerometer sensor resource supporting CoAP GET
 * @author  Doug Anson
 * @version 1.0
 * @see
 *
 * Copyright (c) 2014
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __ACCELEROMETER_RESOURCE_H__
#define __ACCELEROMETER_RESOURCE_H__

// Base class
#include "mbed-connector-interface/DynamicResource.h"

// Include libraries for different sensors
#include "FXOS8700Q.h" 

// Accelerometer
I2C i2c(I2C_SDA, I2C_SCL);
FXOS8700QAccelerometer acc(i2c, FXOS8700CQ_SLAVE_ADDR1);
FXOS8700QMagnetometer mag(i2c, FXOS8700CQ_SLAVE_ADDR1);

/** 
 * AccelerometerResource class
 */
class AccelerometerResource : public DynamicResource
{   
public:
    /**
    Default constructor
    @param logger input logger instance for this resource
    @param obj_name input the Accelerometer Object name
    @param res_name input the Accelerometer Resource name
    @param observable input the resource is Observable (default: FALSE)
    */
    AccelerometerResource(const Logger *logger,const char *obj_name,const char *res_name,const bool observable = false) : DynamicResource(logger,obj_name,res_name,"Accelerometer",M2MBase::GET_ALLOWED,observable) {
    }
    
    /**
    Get the value of the Accelerometer sensor
    @returns string containing the acclerometer sensor value
    */
    virtual string get() {
        // rest of GET() method
        char buffer[256] = "";
        motion_data_units_t acc_data, mag_data;
        
        // Read the sensor values; see https://os.mbed.com/components/FXOS8700Q/
        acc.getAxis(acc_data);
        mag.getAxis(mag_data);
                    
        // fill the value of the accelerometer
        sprintf(buffer,"{\"Xacc\":%5.2f,\"Yacc\":%5.2f,\"Zacc\":%5.2f,\"Xmag\":%5.2f,\"Ymag\":%5.2f,\"Zmag\":%5.2f}", 
    			acc_data.x, acc_data.y, acc_data.z, mag_data.x, mag_data.y, mag_data.z);
        return string(buffer);
    }
};

#endif // __ACCELEROMETER_RESOURCE_H__
