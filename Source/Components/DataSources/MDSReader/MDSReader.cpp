/**
 * @file MDSReader.cpp
 * @brief Source file for class MDSReader
 * @date 22/09/2017
 * @author Llorenc Capella
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
 * the class MDSReader (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "AdvancedErrorManagement.h"
#include "MDSReader.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace {

}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

MDSReader::MDSReader() {
//Auto-generated constructor stub for MDSReader

//TODO Verify if manual additions are needed here
}

MDSReader::~MDSReader() {
//Auto-generated destructor stub for MDSReader

//TODO Verify if manual additions are needed here
}

bool MDSReader::Initialise(StructuredDataI& data) {

    return true;
}

bool MDSReader::Synchronise() {

    return true;
}

bool MDSReader::PrepareNextState(const char8 * const currentStateName,
                                 const char8 * const nextStateName) {
    return true;
}

bool MDSReader::AllocateMemory() {
    return true;
}

uint32 MDSReader::GetNumberOfMemoryBuffers() {
    return 1u;
}

bool MDSReader::GetSignalMemoryBuffer(const uint32 signalIdx,
                                      const uint32 bufferIdx,
                                      void *&signalAddress) {
    return 1u;
}

const char *MDSReader::GetBrokerName(StructuredDataI &data,
                              const SignalDirection direction) {
    return "";
}

bool MDSReader::GetInputBrokers(ReferenceContainer &inputBrokers,
                                const char8* const functionName,
                                void * const gamMemPtrs) {
    return 1u;
}

bool MDSReader::GetOutputBrokers(ReferenceContainer &outputBrokers,
                                 const char8* const functionName,
                                 void * const gamMemPtr) {
    return 1u;
}



CLASS_REGISTER(MDSReader, "1.0")
}

