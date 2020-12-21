#include "MemoryMapSynchNMutexOutputBroker.h"

namespace MARTe {

    MemoryMapSynchNMutexOutputBroker::MemoryMapSynchNMutexOutputBroker() : MemoryMapOutputBroker() {

    }
    
    MemoryMapSynchNMutexOutputBroker::~MemoryMapSynchNMutexOutputBroker() {

    }

    bool MemoryMapSynchNMutexOutputBroker::Execute() {
        bool returnValue = true;

        if(dataSource != NULL_PTR(DataSourceI *)) {
            ISynchronisableOutput *synchDataSource = dynamic_cast<ISynchronisableOutput*>(dataSource);
            if(synchDataSource != NULL_PTR(ISynchronisableOutput*)) {
                returnValue = synchDataSource->SynchroniseOutput();
                if(returnValue) {
                    returnValue = MemoryMapOutputBroker::Execute();
                    bool unlockResult = dataSource->TerminateOutputCopy(0u, 0u, 0u);
                    returnValue = returnValue && unlockResult;
                }
            }
            else {
                returnValue = false;
            }
        }

        return returnValue;
    }

    CLASS_REGISTER(MemoryMapSynchNMutexOutputBroker, "1.0")
}