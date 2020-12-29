#include "ProfinetTimerHelper.h"

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

    ErrorManagement::ErrorType ProfinetTimerHelper::ThreadCallback(ExecutionInfo &info) {
        if(timerInterval >= lastInterval) {
            Sleep::Sec(timerInterval - lastInterval);
        }
        uint64 elapsedStart = HighResolutionTimer::Counter();

        ErrorManagement::ErrorType returnValue;

        if(info.GetStage() == ExecutionInfo::MainStage) {
            if(entryPoint != NULL_PTR(ITimerEntryPoint*)) {
                entryPoint->TimerTick();
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

        lastInterval = HighResolutionTimer::TicksToTime(HighResolutionTimer::Counter(), elapsedStart);

        return returnValue;
    }   

    bool ProfinetTimerHelper::SetEntryPoint(ITimerEntryPoint *entryPoint) {
        bool returnValue = false;
        if(entryPoint != NULL_PTR(ITimerEntryPoint*)) {
            this->entryPoint = entryPoint;
            returnValue = true;
        }
        return returnValue;
    }

    void ProfinetTimerHelper::Start() {
        service.Start();
    }

    void ProfinetTimerHelper::Stop() {
        service.Stop();
    }

    void ProfinetTimerHelper::SetTimerInterval(float64 intervalInSeconds) {
        timerInterval = intervalInSeconds;
    }
    

    CLASS_REGISTER(ProfinetTimerHelper, "1.0")
}