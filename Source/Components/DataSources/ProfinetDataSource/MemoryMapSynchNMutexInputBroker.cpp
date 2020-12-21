#include "MemoryMapSynchNMutexInputBroker.h"

namespace MARTe {

    MemoryMapSynchNMutexInputBroker::MemoryMapSynchNMutexInputBroker() : MemoryMapInputBroker() {

    }
    
    MemoryMapSynchNMutexInputBroker::~MemoryMapSynchNMutexInputBroker() {

    }

    bool MemoryMapSynchNMutexInputBroker::Execute() {
        bool returnValue = true;

        if(dataSource != NULL_PTR(DataSourceI *)) {
            ISynchronisableInput *synchDataSource = dynamic_cast<ISynchronisableInput*>(dataSource);
            if(synchDataSource != NULL_PTR(ISynchronisableInput*)) {
                returnValue = synchDataSource->SynchroniseInput();
                if(returnValue) {
                    returnValue = MemoryMapInputBroker::Execute();
                    bool unlockResult = dataSource->TerminateInputCopy(0u, 0u, 0u);
                    returnValue = returnValue && unlockResult;
                }
            }
            else {
                returnValue = false;
            }
        }

        return returnValue;
    }

    CLASS_REGISTER(MemoryMapSynchNMutexInputBroker, "1.0")
}