/**
 * @file MemoryGate.cpp
 * @brief Source file for class MemoryGate
 * @date 23/07/2018
 * @author Giuseppe Ferro
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

 * @details This source file contains the definition of all the methods for
 * the class MemoryGate (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "MemoryGate.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

MemoryGate::MemoryGate() :
        ReferenceContainer(),
        resetTimeout() {
    mem = NULL_PTR(uint8*);
    memSize = 0u;
    numberOfBuffers = 2u;
    offsetStore = 0u;
    spinlocksRead = NULL_PTR(volatile int32 *);
    spinlocksWrite = NULL_PTR(FastPollingMutexSem *);
    whatIsNewestCounter = NULL_PTR(uint32 *);
    whatIsNewestGlobCounter = 0u;
}

MemoryGate::~MemoryGate() {
    if (mem != NULL_PTR(uint8*)) {
        delete[] mem;
    }

    if (spinlocksRead != NULL_PTR(volatile int32 *)) {
        delete[] spinlocksRead;
        spinlocksRead = NULL_PTR(volatile int32 *);
    }
    if (spinlocksWrite != NULL_PTR(FastPollingMutexSem *)) {
        delete[] spinlocksWrite;
        spinlocksWrite = NULL_PTR(FastPollingMutexSem *);

    }
    if (whatIsNewestCounter != NULL_PTR(uint32 *)) {
        delete[] whatIsNewestCounter;
        whatIsNewestCounter = NULL_PTR(uint32 *);
    }
}

bool MemoryGate::Initialise(StructuredDataI &data) {
    bool ret = ReferenceContainer::Initialise(data);
    if (ret) {
        if (!data.Read("NumberOfBuffers", numberOfBuffers)) {
            numberOfBuffers = 2u;
        }

        ret = (numberOfBuffers <= 64u) && (numberOfBuffers > 0u);
        if (!ret) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "The maximum allowed numberOfBuffers is 64");
        }

        if (ret) {
            if (spinlocksRead == NULL) {
                uint32 index = (numberOfBuffers);
                spinlocksRead = new volatile int32[index];
                ret = (spinlocksRead != NULL_PTR(volatile int32 *));
            }
            if (ret) {
                if (spinlocksWrite == NULL) {
                    uint32 index = (numberOfBuffers);
                    spinlocksWrite = new FastPollingMutexSem[index];
                    ret = (spinlocksWrite != NULL_PTR(FastPollingMutexSem *));
                }
            }
            if (ret) {

                if (whatIsNewestCounter == NULL) {
                    uint32 index = (numberOfBuffers);
                    whatIsNewestCounter = new uint32[index];
                    ret = (whatIsNewestCounter != NULL_PTR(uint32 *));
                }
            }

            if (ret) {
                for (uint32 i = 0u; i < numberOfBuffers; i++) {
                    /*lint -e{613} NULL pointer checked.*/
                    spinlocksRead[i] = 0;
                    /*lint -e{613} NULL pointer checked.*/
                    spinlocksWrite[i].Create(false);
                    /*lint -e{613} NULL pointer checked.*/
                    whatIsNewestCounter[i] = 0u;
                }
            }
        }

        if (ret) {
            uint32 resetTimeoutT;
            if (!data.Read("ResetMSecTimeout", resetTimeoutT)) {
                resetTimeout = TTInfiniteWait;
            }
            else {
                resetTimeout = resetTimeoutT;
            }

            if (data.Read("MemorySize", memSize)) {
                uint32 totMemSize = (memSize * numberOfBuffers);
                mem = new uint8[totMemSize];
                ret = MemoryOperationsHelper::Set(mem, '\0', memSize);
            }
            else {
                memSize = 0u;
            }
        }

    }

    return ret;
}

bool MemoryGate::SetMemorySize(uint32 size) {
    //same memory in read and write!
    bool ret = true;
    if (mem == NULL) {
        memSize = size;
        uint32 totMemSize = (memSize * numberOfBuffers);
        mem = new uint8[totMemSize];

        ret = MemoryOperationsHelper::Set(mem, '\0', memSize);
    }
    else {
        ret = (memSize == size);
        if (!ret) {
            REPORT_ERROR(ErrorManagement::FatalError, "Memory size mismatch (%d != %d). All the components must specify the same size", size, memSize);
        }
    }

    return ret;
}

bool MemoryGate::MemoryRead(uint8 * const bufferToFill) {
    //get input offset
    bool ok = false;
    uint64 checkedMask = 0ull;
    uint32 bufferIdx = 0u;

    for (uint32 k = 0u; (k < numberOfBuffers) && (!ok); k++) {
        //check the last written
        bufferIdx = 0u;
        uint32 max = 0u;
        for (uint32 h = 0u; (h < numberOfBuffers); h++) {
            /*lint -e{613} NULL pointer checked.*/
            if (spinlocksWrite[h].FastTryLock()) {
                /*lint -e{613} NULL pointer checked.*/
                uint32 tempMax = whatIsNewestCounter[h];
                /*lint -e{613} NULL pointer checked.*/
                spinlocksWrite[h].FastUnLock();
                if ((tempMax >= max) && (((1ull << h) & checkedMask) == 0u)) {
                    max = tempMax;
                    bufferIdx = h;
                }
            }
        }

        checkedMask |= (1ull << bufferIdx);
        ok = false;
        //try to lock the next buffer to write
        /*lint -e{613} NULL pointer checked.*/
        if (spinlocksWrite[bufferIdx].FastTryLock()) {
            /*lint -e{613} NULL pointer checked.*/
            Atomic::Increment(&spinlocksRead[bufferIdx]);
            /*lint -e{613} NULL pointer checked.*/
            spinlocksWrite[bufferIdx].FastUnLock();
            ok = true;
        }
    }
    //copy the memory to the data source buffer
    if (ok) {
        uint32 offset = (memSize * bufferIdx);
        /*lint -e{613} NULL pointer checked.*/
        ok = MemoryOperationsHelper::Copy(bufferToFill, &mem[offset], memSize);
    }

    if (ok) {
        /*lint -e{613} NULL pointer checked.*/
        Atomic::Decrement(&spinlocksRead[bufferIdx]);
        //REPORT_ERROR(ErrorManagement::Information, "buffer read=%d", bufferIdx);

    }

    return ok;
}

bool MemoryGate::MemoryWrite(const uint8 * const bufferToFlush) {

    bool ok = false;
    uint32 bufferIdx = 0u;
    uint64 checkedMask = 0ull;
    for (uint32 k = 0u; (k < numberOfBuffers) && (!ok); k++) {
        //check the oldest written
        uint32 min = 0xFFFFFFFFu;
        bufferIdx = 0u;
        for (uint32 h = 0u; (h < numberOfBuffers); h++) {
            uint32 index = h;
            /*lint -e{613} NULL pointer checked.*/
            uint32 tempMin = whatIsNewestCounter[index];
            if ((tempMin < min) && (((1ull << h) & checkedMask) == 0ull)) {
                min = tempMin;
                bufferIdx = h;
            }
        }
        checkedMask |= (1ull << bufferIdx);
        ok = false;
        //try to lock the next buffer to write
        /*lint -e{613} NULL pointer checked.*/
        if (spinlocksWrite[bufferIdx].FastTryLock()) {
            //no readers!
            /*lint -e{613} NULL pointer checked.*/
            if (spinlocksRead[bufferIdx] == 0) {
                ok = true;
            }
            else {
                /*lint -e{613} NULL pointer checked.*/
                spinlocksWrite[bufferIdx].FastUnLock();
            }
        }
    }

    //copy the memory to the data source buffer
    if (ok) {
        uint32 offset = (memSize * bufferIdx);
        /*lint -e{613} NULL pointer checked.*/
        ok = MemoryOperationsHelper::Copy(&mem[offset], bufferToFlush, memSize);
    }

    if (ok) {
        //REPORT_ERROR(ErrorManagement::Information, "buffer write=%d", bufferIdx);

        whatIsNewestGlobCounter++;
        /*lint -e{613} NULL pointer checked.*/
        whatIsNewestCounter[bufferIdx] = whatIsNewestGlobCounter;

        //overflow... subtract the smaller one
        if (whatIsNewestGlobCounter == 0u) {

            uint32 min = 0xFFFFFFFFu;
            for (uint32 h = 0u; (h < numberOfBuffers); h++) {
                if (h != bufferIdx) {
                    /*lint -e{613} NULL pointer checked.*/
                    if (whatIsNewestCounter[h] < min) {
                        /*lint -e{613} NULL pointer checked.*/
                        min = whatIsNewestCounter[h];
                    }
                }
            }

            for (uint32 h = 0u; (h < numberOfBuffers); h++) {
                if (h != bufferIdx) {
                    /*lint -e{613} NULL pointer checked.*/
                    bool blocked = spinlocksWrite[h].FastLock(resetTimeout);
                    if (blocked) {
                        /*lint -e{613} NULL pointer checked.*/
                        whatIsNewestCounter[h] -= min;
                        /*lint -e{613} NULL pointer checked.*/
                        spinlocksWrite[h].FastUnLock();
                    }
                }
            }

            whatIsNewestGlobCounter -= min;
            /*lint -e{613} NULL pointer checked.*/
            whatIsNewestCounter[bufferIdx] -= min;
        }
        /*lint -e{613} NULL pointer checked.*/
        spinlocksWrite[bufferIdx].FastUnLock();
    }
    return ok;
}
CLASS_REGISTER(MemoryGate, "1.0")

}
