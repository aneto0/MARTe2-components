/**
 * @file DANStream.cpp
 * @brief Source file for class DANStream
 * @date Apr 4, 2017
 * @author aneto
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
 * the class DANStream (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include "dan/dan_DataCore.h"
#include "dan/dan_Source.h"

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "CompilerTypes.h"
#include "DANSource.h"
#include "DANStream.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

DANStream::DANStream(TypeDescriptor tdIn, StreamString baseNameIn, uint32 danBufferMultiplierIn) {
    td = tdIn;
    typeSize = td.numberOfBits / 8u;
    nOfSignals = 0u;
    signalList = NULL_PTR(char8 **);
    blockMemory = NULL_PTR(char8 *);
    danSource = NULL_PTR(dan_Source);
    baseName = baseNameIn;
    danBufferMultiplier = danBufferMultiplierIn;
}

DANStream::~DANStream() {
    if (signalList != NULL_PTR(void **)) {
        delete[] signalList;
    }
    if ((DANSource::GetDANDataCore() != NULL_PTR(dan_DataCore)) && (danSource != NULL_PTR(dan_Source))) {
        dan_publisher_unpublishSource(DANSource::GetDANDataCore(), danSource);
    }
}

TypeDescriptor DANStream::GetType() {
    return td;
}

bool DANStream::PutData(uint64 timeStamp) {
    uint32 s;
    bool ok = true;
    for (s = 0u; (s < nOfSignals) && (ok); s++) {
        ok = MemoryOperationsHelper::Copy(&blockMemory[s * typeSize], signalList[s], typeSize);
    }
    if (ok) {
        ok = (dan_publisher_putDataBlock(danSource, timeStamp, blockMemory, nOfSignals * typeSize, NULL_PTR(char8 *)) == 0u);
    }
    return ok;
}

bool DANStream::OpenStream(float64 samplingRate) {
    return (dan_publisher_openStream(danSource, samplingRate, 0u) == 0u);
}

bool DANStream::CloseStream() {
    return (dan_publisher_closeStream(danSource) == 0u);
}

void DANStream::Finalise() {
    blockMemory = GlobalObjectsDatabase::Instance()->GetStandardHeap()->Malloc(nOfSignals * typeSize);
    StreamString danSourceName;
    danSourceName.Printf("%s_%s", baseName.Buffer(), TypeDescriptor::GetTypeNameFromTypeDescriptor(td));
    uint32 danBufferSize = nOfSignals * typeSize * danBufferMultiplier;
    danSource = dan_publisher_publishSource_withDAQBuffer(DANSource::GetDANDataCore(), danSourceName.Buffer(), danBufferSize);
}

void DANStream::AddSignal(void * const signalIn) {
    void ** newSignalList = new void*[nOfSignals + 1u];
    uint32 i;
    for (i = 0u; i < nOfSignals; i++) {
        newSignalList[i] = signalList[i];
    }
    if (nOfSignals > 0u) {
        delete[] signalList;
    }
    signalList = newSignalList;
    signalList[nOfSignals] = signalIn;
    nOfSignals++;
}

}
