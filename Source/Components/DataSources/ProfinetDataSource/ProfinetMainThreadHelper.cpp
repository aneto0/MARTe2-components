/**
 * @file ProfinetMainThreadHelper.cpp
 * @brief Source file for class ProfinetMainThreadHelper 
 * @date 15/01/2021
 * @author Giuseppe Avon
 *
 * @copyright Copyright 2015 F4E | European Joint Undertaking for ITER and
 * the Development of Fusion Energy ('Fusion for Energy').
 * Licensed under the EUPL, Version 1.1 or - as soon they will be approved
 * by the European Commission - subsequent versions of the EUPL (the "Licence")
 * You may not use this work except in compliance with the Licence.
 * You may obtain a copy of the Licence at: http://ec.europa.eu/idabc/eupl
 *
 * @warning Unless required by applicable law or agreed to in writing, 
 * software distributed under the Licence is distributed on an "AS IS"
 * basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
 * or implied. See the Licence permissions and limitations under the Licence.

 * @details This header file contains the declaration of the class ProfinetMainThreadHelper
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "ProfinetMainThreadHelper.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

    ProfinetMainThreadHelper::ProfinetMainThreadHelper() :  Object(),
                                                            EmbeddedServiceMethodBinderT<ProfinetMainThreadHelper>(*this, &ProfinetMainThreadHelper::ThreadCallback),
                                                            service(*this) {
        //flagMutex.Create(false);
        
        //eventSemaphore.Create();
        //eventSemaphore.Reset();
    }

    bool ProfinetMainThreadHelper::Initialise(StructuredDataI &data) {
        bool returnValue = Object::Initialise(data);
	if(returnValue) {
	   flagMutex.Create(false);
	}

	if(returnValue) {
	    returnValue = eventSemaphore.Create();
	    if(!returnValue) {
		REPORT_ERROR(ErrorManagement::FatalError, "Error during event semaphore creation");
	    }
	}

	if(returnValue) {
	    returnValue = eventSemaphore.Reset();
	    if(!returnValue) {
		REPORT_ERROR(ErrorManagement::FatalError, "Error during event semaphore reset");
	    }
	}
	
        if(returnValue) {
            returnValue = service.Initialise(data);
        }

        return returnValue;
    }

    ProfinetMainThreadHelper::~ProfinetMainThreadHelper() {
        entryPoint = NULL_PTR(IMainThreadEntryPoint*);
    }

    //lint -e{830,830,830,830,830,830,830,830,1764} Parameter is not under this class control
    ErrorManagement::ErrorType ProfinetMainThreadHelper::ThreadCallback(ExecutionInfo &info) {
        ErrorManagement::ErrorType returnValue = ErrorManagement::NoError;

        if(info.GetStage() == ExecutionInfo::MainStage) {
            //Stand still, awaiting for an event to be raised
	    if(eventSemaphore.Wait(timeout) == ErrorManagement::NoError) {
                uint16 localFlag = 0u;
                uint16 workedFlags = 0u;

                //Freeze current flag status locally
                //lint -e{838} localFlag is used below in the bitwise XOR operation
		localFlag = eventFlag;

                //Run a cycle and tell apart which flags were processed (do not assume the MainThread can clear all flags in a single call)
                if(entryPoint != NULL_PTR(IMainThreadEntryPoint*)) {
                    workedFlags = entryPoint->MainThread(localFlag) ^ localFlag;
                }                

                //Clear only local flags which were reported as processed
		if(flagMutex.FastLock(timeout, sleepTimeSeconds) == ErrorManagement::NoError) {
                    eventFlag &= ~workedFlags;
                    bool resetStatus = eventSemaphore.Reset();
		    if(resetStatus) {
			//REPORT_ERROR(ErrorManagement::Information, "Event semaphore reset failed");
		    }
                    flagMutex.FastUnLock();
                }
            }
        }
        else if(info.GetStage() == ExecutionInfo::StartupStage) {
            
        }
        else if(info.GetStage() == ExecutionInfo::TerminationStage) {
            
        }
        else if(info.GetStage() == ExecutionInfo::BadTerminationStage) {
            
        }
        else if(info.GetStage() == ExecutionInfo::AsyncTerminationStage) {
            
        }
	else {
	    REPORT_ERROR(ErrorManagement::ParametersError, "Thread is in an unhandled stage");
	}
        
        return returnValue;
    }

    //lint -e{952} 
    bool ProfinetMainThreadHelper::SetEntryPoint(IMainThreadEntryPoint *entryPointParam) {
        bool returnValue = false;
        if(entryPointParam != NULL_PTR(IMainThreadEntryPoint*)) {
            this->entryPoint = entryPointParam;
            returnValue = true;
        }
        return returnValue;
    }

    void ProfinetMainThreadHelper::Start() {
        if(service.Start() != ErrorManagement::NoError) {
	    REPORT_ERROR(ErrorManagement::FatalError, "Cannot start Main Thread Helper");
	}
    }

    void ProfinetMainThreadHelper::Stop() {
        if(service.Stop() != ErrorManagement::NoError) {
	    REPORT_ERROR(ErrorManagement::FatalError, "Cannot stop Main Thread Helper");
	}
    }

    void ProfinetMainThreadHelper::NotifyEvent(const ProfinetDataSourceEventType eventType) {
        if(flagMutex.FastLock(timeout, sleepTimeSeconds) == ErrorManagement::NoError) {
	    //lint -e{641,9114,9117,9119,9130} Event Type is an enum which is thought as a flag and its values are 2^n
            eventFlag |= eventType;
            if(!eventSemaphore.Post()) {
		REPORT_ERROR(ErrorManagement::Information, "Barrier lowering failed");
	    }
            flagMutex.FastUnLock();
        }
	else {
	    REPORT_ERROR(ErrorManagement::Information, "Unable to obtain fast lock");
	}
    }

    void ProfinetMainThreadHelper::SetPeriodicInterval(const float32 seconds) {
        //Setting the spinlock sleep time to 1/5 of the whole interval period, 
        //sampling occupation status far above the Nyquist condition
        //thus releasing CPU in between the polling cycles
        sleepTimeSeconds = static_cast<float32>(seconds / 5.0F);
    }

    void ProfinetMainThreadHelper::SetTimeout(const float32 seconds) {
        timeout.SetTimeoutSec(static_cast<float64>(seconds));
    }

    CLASS_REGISTER(ProfinetMainThreadHelper, "1.0")
}

