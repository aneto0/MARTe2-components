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
        flagMutex.Create(false);
        
        eventSemaphore.Create();
        eventSemaphore.Reset();
    }

    bool ProfinetMainThreadHelper::Initialise(StructuredDataI &data) {
        bool returnValue = Object::Initialise(data);
        if(returnValue) {
            returnValue = service.Initialise(data);
        }

        return returnValue;
    }

    ProfinetMainThreadHelper::~ProfinetMainThreadHelper() {
        entryPoint = NULL_PTR(IMainThreadEntryPoint*);
    }

    ErrorManagement::ErrorType ProfinetMainThreadHelper::ThreadCallback(ExecutionInfo &info) {
        ErrorManagement::ErrorType returnValue;

        if(info.GetStage() == ExecutionInfo::MainStage) {
            //Stand still, awaiting for an event to be raised
            if(eventSemaphore.Wait(timeout)) {
                uint16 localFlag = 0;
                uint16 workedFlags = 0;

                //Freeze current flag status locally
                localFlag = eventFlag;

                //Run a cycle and tell apart which flags were processed (do not assume the MainThread can clear all flags in a single call)
                if(entryPoint != NULL_PTR(IMainThreadEntryPoint*)) {
                    workedFlags = entryPoint->MainThread(localFlag) ^ localFlag;
                }                

                //Clear only local flags which were reported as processed
                if(flagMutex.FastLock(timeout, sleepTimeSeconds)) {
                    eventFlag &= ~workedFlags;
                    eventSemaphore.Reset();
                    flagMutex.FastUnLock();
                }
            }
            returnValue = ErrorManagement::Completed;
        }
        else if(info.GetStage() == ExecutionInfo::StartupStage) {
            
        }
        else if(info.GetStage() == ExecutionInfo::TerminationStage) {
            
        }
        else if(info.GetStage() == ExecutionInfo::BadTerminationStage) {
            
        }
        else if(info.GetStage() == ExecutionInfo::AsyncTerminationStage) {
            
        }
        
        return returnValue;
    }

    bool ProfinetMainThreadHelper::SetEntryPoint(IMainThreadEntryPoint *entryPoint) {
        bool returnValue = false;
        if(entryPoint != NULL_PTR(IMainThreadEntryPoint*)) {
            this->entryPoint = entryPoint;
            returnValue = true;
        }
        return returnValue;
    }

    void ProfinetMainThreadHelper::Start() {
        service.Start();
    }

    void ProfinetMainThreadHelper::Stop() {
        service.Stop();
    }

    void ProfinetMainThreadHelper::NotifyEvent(ProfinetDataSourceEventType eventType) {
        if(flagMutex.FastLock(timeout, sleepTimeSeconds)) {
            eventFlag |= eventType;
            eventSemaphore.Post();
            flagMutex.FastUnLock();
        }
    }

    void ProfinetMainThreadHelper::SetPeriodicInterval(float64 seconds) {
        //Setting the spinlock sleep time to 1/5 of the whole interval period, 
        //sampling occupation status far above the Nyquist condition
        //thus releasing CPU in between the polling cycles
        sleepTimeSeconds = seconds / 5.0d;
    }

    void ProfinetMainThreadHelper::SetTimeout(float64 seconds) {
        timeout.SetTimeoutSec(seconds);
    }

    CLASS_REGISTER(ProfinetMainThreadHelper, "1.0")
}

