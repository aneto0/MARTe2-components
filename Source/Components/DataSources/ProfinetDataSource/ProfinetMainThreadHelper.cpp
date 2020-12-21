#include "ProfinetMainThreadHelper.h"

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
                workedFlags = entryPoint->MainThread(localFlag) ^ localFlag;

                //Clear only local flags which were reported as processed
                if(flagMutex.FastLock(timeout, sleepTimeSeconds)) {
                    eventFlag &= ~workedFlags;
                    flagMutex.FastUnLock();
                }

                eventSemaphore.Reset();
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
            flagMutex.FastUnLock();

            eventSemaphore.Post();
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

