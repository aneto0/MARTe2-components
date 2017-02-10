/**
 * @file MDSWriter.cpp
 * @brief Source file for class MDSWriter
 * @date 10/02/2017
 * @author Andre Neto
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
 * the class MDSWriter (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "MDSWriter.h"
#include "MemoryMapTriggerOutputBroker.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

MDSWriter::MDSWriter() {
}

MDSWriter::~MDSWriter() {
}

bool MDSWriter::AllocateMemory() {
    return true;
}

uint32 MDSWriter::GetNumberOfMemoryBuffers() {
    return 1u;
}

bool MDSWriter::GetSignalMemoryBuffer(const uint32 signalIdx, const uint32 bufferIdx, void*& signalAddress) {
    return true;
}

const char8* MDSWriter::GetBrokerName(StructuredDataI& data, const SignalDirection direction) {
    return "MemoryMapTriggerOutputBroker";
}

bool MDSWriter::GetInputBrokers(ReferenceContainer& inputBrokers, const char8* const functionName, void* const gamMemPtr) {
    return false;
}

bool MDSWriter::GetOutputBrokers(ReferenceContainer& outputBrokers, const char8* const functionName, void* const gamMemPtr) {
    ReferenceT<MemoryMapTriggerOutputBroker> broker;
    return true;
}

bool MDSWriter::Synchronise() {
    return true;
}

bool MDSWriter::PrepareNextState(const char8* const currentStateName, const char8* const nextStateName) {
    return true;
}

bool MDSWriter::Initialise(StructuredDataI& data) {
    return DataSourceI::Initialise(data);
}

bool MDSWriter::SetConfiguredDatabase(StructuredDataI& data) {
    return DataSourceI::Initialise(data);
}

CLASS_REGISTER(MDSWriter, "1.0")

}
