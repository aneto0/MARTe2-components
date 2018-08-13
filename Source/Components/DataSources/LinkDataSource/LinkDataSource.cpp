/**
 * @file LinkDataSource.cpp
 * @brief Source file for class LinkDataSource
 * @date 06/07/2018
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
 * the class LinkDataSource (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "AdvancedErrorManagement.h"
#include "ConfigurationDatabase.h"
#include "LinkDataSource.h"
#include "ObjectRegistryDatabase.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

LinkDataSource::LinkDataSource() :
        MemoryDataSourceI() {
    isWriter = 0u;
}

LinkDataSource::~LinkDataSource() {

}

bool LinkDataSource::Initialise(StructuredDataI &data) {
    bool ret = MemoryDataSourceI::Initialise(data);
    if (ret) {
        ret = data.Read("Link", linkPath);
        if (!ret) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "Please specify Link path");
        }
    }
    if (ret) {
        ret = data.Read("IsWriter", isWriter);
        if (!ret) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "Please specify IsWriter");
        }
    }
    if (ret) {

        link = ObjectRegistryDatabase::Instance()->Find(linkPath.Buffer());
        ret = link.IsValid();
        if (!ret) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "The link %s is not valid", linkPath.Buffer());
        }
    }
    return ret;
}

/*lint -e{715} the data parameter is not required for this implementation.*/
const char8 *LinkDataSource::GetBrokerName(StructuredDataI &data, const SignalDirection direction) {

    const char8* brokerName = NULL_PTR(const char8 *);

    if (direction == InputSignals) {
        brokerName = "MemoryMapSynchronisedMultiBufferInputBroker";
    }

    if (direction == OutputSignals) {
        brokerName = "MemoryMapSynchronisedMultiBufferOutputBroker";
    }

    return brokerName;
}

bool LinkDataSource::AllocateMemory() {
    bool ret = MemoryDataSourceI::AllocateMemory();
    if (ret) {
        ret = link->SetMemorySize(totalMemorySize);
    }
    return ret;
}

bool LinkDataSource::Synchronise() {
    bool ret = true;
    if (isWriter > 0u) {
        ret = link->MemoryWrite(memory);
        if (!ret) {
            REPORT_ERROR(ErrorManagement::Warning, "Failed MemoryWrite(*): probably all the buffers are busy");
        }
    }
    else {
        ret = link->MemoryRead(memory);
        if (!ret) {
            REPORT_ERROR(ErrorManagement::Warning, "Failed MemoryRead(*): probably all the buffers are busy");
        }
    }
    return true;
}

/*lint -e{715} currentStateName and nextStateName are not referenced*/
bool LinkDataSource::PrepareNextState(const char8 * const currentStateName, const char8 * const nextStateName) {
    return true;
}

CLASS_REGISTER(LinkDataSource, "1.0")

}
