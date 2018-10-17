/**
 * @file    ApplicationStackControlResource.h
 * @brief   mbed CoAP Endpoint application stack control resource
 * @author  Doug Anson
 * @version 1.0
 * @see
 *
 * Copyright (c) 2018
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

#ifndef __APPLICATION_STACK_CONTROL_RESOURCE_H__
#define __APPLICATION_STACK_CONTROL_RESOURCE_H__

// Base class
#include "mbed-connector-interface/DynamicResource.h"

// Tunables
#define APPLICATION_DATA_SIZE 		256

// resources 
static char _application_data[APPLICATION_DATA_SIZE+1];
static bool _application_stack_on = false;

// Exposed methods to main()

// Get the Application Data (could be used for StackB Auth Creds...)
extern "C" char *getData() {
     if (strlen(_application_data) > 0) {
          return (char *)_application_data;
     }
     return NULL;
}

// Determine if the StackB is ON or OFF... 
extern "C" bool appStackOn() {
     return _application_stack_on;
}

/** 
  * ApplicationStackControlResource class
  */
class ApplicationStackControlResource : public DynamicResource
{

public:
    /**
     * Default constructor
     * @param logger input logger instance for this resource
     * @param obj_name input the sample object name
     * @param res_name input the sample resource name
     * @param observable input the resource is Observable (default: FALSE)
     */
    ApplicationStackControlResource(const Logger *logger,const char *obj_name,const char *res_name,const bool observable = false) : DynamicResource(logger,obj_name,res_name,"AppStackControl",M2MBase::PUT_POST_ALLOWED,observable) {
	memset(_application_data,0,APPLICATION_DATA_SIZE+1);
    }

    /**
     * Put the value of the AppControl Resource
     */
    virtual void put(const string value) {
	int length = value.length();
	if (length > APPLICATION_DATA_SIZE) {
	    length = APPLICATION_DATA_SIZE;
	}
	memset(_application_data,0,APPLICATION_DATA_SIZE+1);
	strncpy(_application_data,value.c_str(),length);
    }
    
    /**
     * Post the value of the Sample Resource
     */
    virtual void post(void *args) {
	// just toggle the appStackOn value...
	_application_stack_on = !_application_stack_on;
    }
};

#endif // __APPLICATION_STACK_CONTROL_RESOURCE_H__
