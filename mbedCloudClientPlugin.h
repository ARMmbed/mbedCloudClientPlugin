/*
 * Copyright (c) 2018 ARM Limited. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __MBED_CLOUD_CLIENT_PLUGIN_H__
#define __MBED_CLOUD_CLIENT_PLUGIN_H__

// ************************************************************************
// pulled from https://github.com/ARMmbed/mbed-cloud-sample
// ************************************************************************

// Choose a LED color (for FOTA illustration) - only ONE may be uncommented or compile errors occur!
#define USE_BLUE_LED			true
//#define USE_RED_LED			true
//#define USE_GREEN_LED			true

// Enable/Disable the DeviceManager
#define ENABLE_DEVICE_MANAGER           true    

// Passphrase to supply for data management authentication
#define MY_DM_PASSPHRASE                "arm1234"

// Include security.h
#include "security.h"

// mbed Endpoint Network
#include "mbed-connector-interface/mbedEndpointNetwork.h"

// Logger
#include "mbed-connector-interface/Logger.h"
Logger logger(&pc);

// Our Device Management Authenticator (trivial passphrase authenticator used)
#include "mbed-connector-interface/PassphraseAuthenticator.h"
PassphraseAuthenticator authenticator(&logger,MY_DM_PASSPHRASE);

// Our Device Management Responder
#include "mbed-connector-interface/DeviceManagementResponder.h"

// Our Device Manager
#include "mbed-connector-interface/DeviceManager.h"

// Monotonic Counter Resource
#include "mbed-endpoint-resources/MonotonicCounterResource.h"
MonotonicCounterResource sample_counter(&logger,"123","4567",true);     // "true" -> resource is observable

// Light Sensor Resource
#include "mbed-endpoint-resources/LightResource.h"
LightResource light(&logger,"311","5850");                            

// Application Stack Control Resource
#include "mbed-endpoint-resources/ApplicationStackControlResource.h"
ApplicationStackControlResource app_control(&logger,"311","5600");

// forward references
extern "C" void mcc_platform_init_connection(void);
extern "C" void *mcc_platform_get_network_interface(void);

// called from the Endpoint::start() below to create resources and the endpoint internals...
Connector::Options *configure_endpoint(Connector::OptionsBuilder &config)
{    
    // Build the endpoint configuration parameters
    logger.log("Endpoint::main (%s): customizing endpoint configuration...",net_get_type());
    return config                 
        // set the endpoint type (from mbed_cloud_client_user_config.h) 
        .setEndpointType(MBED_CLOUD_CLIENT_ENDPOINT_TYPE)
        
        // Light Resource
        .addResource(&light)                    // not observable
        
        // Monotonic Counter Resource
        .addResource(&sample_counter,10000)     // observe every 10 seconds

        // Application Stack Control Example Resource
	.addResource(&app_control)		// not observable
                   
        // finalize the configuration...
        .build();
}

// initialize mbedConnectorInterface client
void init_mbed_cloud_client_plugin() {
    // mbed Cloud setup
    DeviceManager *device_manager = NULL;
    if (ENABLE_DEVICE_MANAGER) {
        // Allocate the Device Management processor 
        DeviceManagementResponder *dm_processor = new DeviceManagementResponder(&logger,&authenticator);
        device_manager = new DeviceManager(&logger,dm_processor,MBED_CLOUD_CLIENT_ENDPOINT_TYPE);
    }
    
    // we have to plumb our network first
    Connector::Endpoint::plumbNetwork(device_manager);
}

// start mbedConnectorInterface's primary event loop
void start_mbed_cloud_client_plugin_event_loop() {
    Connector::Endpoint::start();
}

#endif // __MBED_CLOUD_CLIENT_PLUGIN_H__
