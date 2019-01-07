/**
 * @file OPCUADSInput.cpp
 * @brief Source file for class OPCUADSInput
 * @date Nov 16, 2018 TODO Verify the value and format of the date
 * @author lporzio TODO Verify the name and format of the author
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
 * the class OPCUADSInput (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "OPCUADSInput.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

OPCUADSInput::OPCUADSInput() :
        DataSourceI(),
        EmbeddedServiceMethodBinderI(),
        executor(*this) {
    clients = NULL_PTR(OPCUAClientWrapper *);
    numberOfClients = 0u;
    paths = NULL_PTR(StreamString *);
    namespaceIndexes = NULL_PTR(uint32 *);
}

OPCUADSInput::~OPCUADSInput() {
    delete paths;
    if (!executor.Stop()) {
        if (!executor.Stop()) {
            REPORT_ERROR(ErrorManagement::FatalError, "Could not stop MultiThreadService.");
        }
    }
}

bool OPCUADSInput::Initialise(StructuredDataI & data) {
    bool ok = DataSourceI::Initialise(data);
    if (ok) {
        ok = data.MoveRelative("Signals");
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Could not move to the Signals section");
        }
        if (ok) {
            ok = signalsDatabase.MoveRelative("Signals");
        }
        /* Do not allow to add signals in run-time */
        if (ok) {
            ok = signalsDatabase.Write("Locked", 1u);
            numberOfClients = (signalsDatabase.GetNumberOfChildren() - 1u);

            /* Checking the signal type. If it's a structured type we need to call the function to get the structure */
            bool isStructured;
            paths = new StreamString[numberOfClients];
            namespaceIndexes = new uint32[numberOfClients];
            for (uint32 i = 0u; (i < numberOfClients) && (ok); i++) {
                /* TEST */
                TypeDescriptor td = GetSignalType(i);
                isStructured = td.isStructuredData;
                if (isStructured) {
                    /* Here we need to resolve the Structure until we find a base type
                     * Then we need to append the names of the member we found to the paths array
                     * Increment the number of clients according to the number of base type member we found
                     */
                }
                /* END TEST */
                ok = signalsDatabase.MoveRelative(signalsDatabase.GetChildName(i));
                if (ok) {
                    ok = signalsDatabase.Read("Path", paths[i]);
                    if (!ok) {
                        REPORT_ERROR(ErrorManagement::ParametersError, "Cannot read the Path attribute from signal %d", i);
                    }
                }
                if (ok) {
                    ok = signalsDatabase.Read("NamespaceIndex", namespaceIndexes[i]);
                    if (!ok) {
                        REPORT_ERROR(ErrorManagement::ParametersError, "Cannot read the NamespaceIndex attribute from signal %d", i);
                    }
                }
                ok = signalsDatabase.MoveToAncestor(1u);
            }
        }
        if (ok) {
            if (numberOfClients > 0u) {
                clients = new OPCUAClientWrapper[numberOfClients];
            }
        }
        if (ok) {
            ok = signalsDatabase.MoveToAncestor(1u);
        }
    }
    if (ok) {
        ok = data.MoveToAncestor(1u);
    }
    if (ok) {
        uint32 i;
        for (i = 0u; i < numberOfClients; i++) {
            clients[i].Connect();
        }
    }
    if (!ok) {
        REPORT_ERROR(ErrorManagement::ParametersError, "Error during Initialise!");
    }
    return ok;
}

bool OPCUADSInput::SetConfiguredDatabase(StructuredDataI & data) {
    bool ok = DataSourceI::SetConfiguredDatabase(data);
    uint32 nOfSignals = GetNumberOfSignals();
    if (ok) {
        ok = (nOfSignals > 0u);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "At least one signal shall be defined");
        }
    }
    if (ok) {
        ok = (executor.Start() == ErrorManagement::NoError);
    }
    return ok;
}

bool OPCUADSInput::AllocateMemory() {
    return true;
}

uint32 OPCUADSInput::GetNumberOfMemoryBuffers() {
    return 1u;
}

bool OPCUADSInput::GetSignalMemoryBuffer(const uint32 signalIdx,
                                         const uint32 bufferIdx,
                                         void *&signalAddress) {
    StreamString opcDisplayName;
    bool ok = GetSignalName(signalIdx, opcDisplayName);
    REPORT_ERROR(ErrorManagement::Information, "Searching for signal [%s]", opcDisplayName.Buffer());
    if (ok) {
        ok = clients[signalIdx].BrowseAddressSpace(namespaceIndexes[signalIdx], paths[signalIdx]);
        if (ok) {
            ok = clients[signalIdx].GetSignalMemory(signalAddress);
        }
    }
    if (ok) {
        ok = (signalAddress != NULL_PTR(void *));
    }
    if (ok) {

    }
    return ok;
}

const char8 * OPCUADSInput::GetBrokerName(StructuredDataI &data,
                                          const SignalDirection direction) {
    const char8* brokerName = "";
    if (direction == InputSignals) {
        brokerName = "MemoryMapInputBroker";
    }
    return brokerName;
}

bool OPCUADSInput::PrepareNextState(const char8 * const currentStateName,
                                    const char8 * const nextStateName) {
    return true;
}

ErrorManagement::ErrorType OPCUADSInput::Execute(ExecutionInfo & info) {
    ErrorManagement::ErrorType err = ErrorManagement::NoError;
    if (info.GetStage() != ExecutionInfo::BadTerminationStage) {
        err.communicationError = !clients[info.GetThreadNumber()].Monitor();
        Sleep::MSec(500);
    }
    return err;
}

bool OPCUADSInput::Synchronise() {
    return false;
}

CLASS_REGISTER(OPCUADSInput, "1.0");

}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

