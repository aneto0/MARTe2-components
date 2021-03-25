/**
 * @file ProfinetTimerHelper.cpp
 * @brief Source file for class ProfinetTimerHelper
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

 * @details This header file contains the declaration of the class ProfinetTimerHelper
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "ProfinetTimerHelper.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

    ProfinetTimerHelper::ProfinetTimerHelper() :    Object(),
                                                    EmbeddedServiceMethodBinderT<ProfinetTimerHelper>(*this, &ProfinetTimerHelper::ThreadCallback),
                                                    service(*this) {
        REPORT_ERROR(ErrorManagement::Information, "Timer Helper created");                                                
    }

    bool ProfinetTimerHelper::Initialise(StructuredDataI &data) {
        bool returnValue = Object::Initialise(data);

        if(returnValue) {
            returnValue = service.Initialise(data);
        }

        return returnValue;
    }

    ProfinetTimerHelper::~ProfinetTimerHelper() {
        entryPoint = NULL_PTR(ITimerEntryPoint*);
    }

    //lint -e{830, 1764} Function prototype is derived from upper type
    ErrorManagement::ErrorType ProfinetTimerHelper::ThreadCallback(ExecutionInfo &info) {
        if(timerInterval >= lastInterval) {
            Sleep::Sec(static_cast<float32>(timerInterval - lastInterval));
        }
        uint64 elapsedStart = HighResolutionTimer::Counter();

        ErrorManagement::ErrorType returnValue = ErrorManagement::NoError;

        if(info.GetStage() == ExecutionInfo::MainStage) {
            if(entryPoint != NULL_PTR(ITimerEntryPoint*)) {
                entryPoint->TimerTick();
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
	    REPORT_ERROR(ErrorManagement::ParametersError, "Unknown thread callback execution stage");
	}

        lastInterval = HighResolutionTimer::TicksToTime(HighResolutionTimer::Counter(), elapsedStart);

        return returnValue;
    }   

    //lint -e{830,952}
    bool ProfinetTimerHelper::SetEntryPoint(ITimerEntryPoint *entryPointParam) {
        bool returnValue = false;
        if(entryPointParam != NULL_PTR(ITimerEntryPoint*)) {
            this->entryPoint = entryPointParam;
            returnValue = true;
        }
        return returnValue;
    }

    void ProfinetTimerHelper::Start() {
        if(!service.Start()) {
	    REPORT_ERROR(ErrorManagement::FatalError, "Failure to start service");
	}
    }

    void ProfinetTimerHelper::Stop() {
        if(!service.Stop()) {
	    REPORT_ERROR(ErrorManagement::FatalError, "Failure to stop service");
	}
    }

    void ProfinetTimerHelper::SetTimerInterval(const float64 intervalInSeconds) {
        timerInterval = intervalInSeconds;
    }
    

    CLASS_REGISTER(ProfinetTimerHelper, "1.0")
}
