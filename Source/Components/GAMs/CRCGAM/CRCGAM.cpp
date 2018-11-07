/**
 * @file CRCGAM.cpp
 * @brief Source file for class CRCGAM
 * @date 30 Oct 2018
 * @author Luca Porzio
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
 * the class CRCGAM (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "CRCGAM.h"
#include "AdvancedErrorManagement.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace {

}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

    CRCGAM::CRCGAM() : GAM() {
        inputData = NULL_PTR(uint8 *);
        outputData = NULL_PTR(void *);
        crcHelper = NULL_PTR(CRCHelper *);
        polynomial = 0x0u;
        initialCRCValue = 0x0u;
        isReflected = 0u;
        inputSize = 0u;
    }

    CRCGAM::~CRCGAM() {
        inputData = NULL_PTR(uint8 *);
        outputData = NULL_PTR(void *);
        crcHelper = NULL_PTR(CRCHelper *);
    }

    bool CRCGAM::Initialise(StructuredDataI &data) {
        bool ok = GAM::Initialise(data);
        if(ok) {
            ok = data.Read("Polynomial", polynomial);
            if(!ok) {
                REPORT_ERROR(ErrorManagement::InitialisationError, "Cannot read Polynomial value from configuration.");
            }
        }
        if(ok) {
            ok = data.Read("InitialValue", initialCRCValue);
            if(!ok) {
                REPORT_ERROR(ErrorManagement::InitialisationError, "Cannot read InitialValue from configuration.");
            }
        }
        if(ok) {
            ok = data.Read("Inverted", isReflected);
            if(!ok) {
                REPORT_ERROR(ErrorManagement::InitialisationError, "Cannot read Inverted value from configuration.");
            }
            if(isReflected > 1u) {
                REPORT_ERROR(ErrorManagement::InitialisationError, "Inverted option value must be 0 or 1. Now Inverted = %d", isReflected);
                ok = false;
            }
        }
        if(!ok) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "Error during Initialise.");
        }
        return ok;
    }

    bool CRCGAM::Setup() {
        //Must have at least 1 Input signal.
        bool ok = true;
        uint32 nInputSignals;
        nInputSignals = GetNumberOfInputSignals();
        if(nInputSignals < 1u) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "CRCGAM must have at least 1 input signal.");
            ok = false;
        }
        //The number of Output signals must be equal to 1.
        if(ok) {
            uint32 nOutputSignals;
            nOutputSignals = GetNumberOfOutputSignals();
            if(nOutputSignals != 1u) {
                REPORT_ERROR(ErrorManagement::InitialisationError, "nOutputSignals must be one. The current values is %u", nOutputSignals);
                ok = false;
            }
        }
        //The inputSize must be taken from InputSignals
        if(ok) {
            uint32 n;
            for (n = 0u; (n < GetNumberOfInputSignals()) && (ok); n++) {
                uint32 inByteSize = 0u;
                uint32 inSamples = 1u;
                ok = GetSignalByteSize(InputSignals, n, inByteSize);
                if (ok) {
                    ok = GetSignalNumberOfSamples(InputSignals, n, inSamples);
                } else {
                    REPORT_ERROR(ErrorManagement::InitialisationError, "Error evaluating InputSignals byte size.");
                }
                if (ok) {
                    inByteSize *= inSamples;
                    inputSize += inByteSize;
                } else {
                    REPORT_ERROR(ErrorManagement::InitialisationError, "Error getting Signal number of samples from InputSignals.");
                }
            }
        }
        //The output type must be a supported type.
        if(ok) {
            outputSignalType = GetSignalType(OutputSignals, 0u);
            if(outputSignalType == UnsignedInteger8Bit) {
                crcHelper = new CRCHelperT<uint8>();
                crcHelper->ComputeTable(&polynomial);
                REPORT_ERROR(ErrorManagement::Information, "Table computed!");
                outputData = reinterpret_cast<uint8*>(GetOutputSignalMemory(0u));
            } else if(outputSignalType == UnsignedInteger16Bit) {
                crcHelper = new CRCHelperT<uint16>();
                crcHelper->ComputeTable(&polynomial);
                REPORT_ERROR(ErrorManagement::Information, "Table computed!");
                outputData = reinterpret_cast<uint16*>(GetOutputSignalMemory(0u));
            } else if(outputSignalType == UnsignedInteger32Bit) {
                crcHelper = new CRCHelperT<uint32>();
                crcHelper->ComputeTable(&polynomial);
                REPORT_ERROR(ErrorManagement::Information, "Table computed!");
                outputData = reinterpret_cast<uint32*>(GetOutputSignalMemory(0u));
            } else {
                const char8 * const outputSignalTypeStr = TypeDescriptor::GetTypeNameFromTypeDescriptor(outputSignalType);
                REPORT_ERROR(ErrorManagement::InitialisationError, "The output signal type is %s which is not a supported type. "
                             "It must be uint8, uint16, uint32.", outputSignalTypeStr);
                ok = false;
            }
        }
        if(ok) {
            inputData = reinterpret_cast<uint8*>(GetInputSignalsMemory());
        }
        if(!ok) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "Error during GAM Setup.");
        }
        return ok;
    }

    bool CRCGAM::Execute() {

        bool inv = false;
        if(isReflected == 1u) {
            inv = true;
        }

        if(crcHelper != NULL_PTR(CRCHelper *)) {
            crcHelper->Compute(inputData, static_cast<int32>(inputSize), &initialCRCValue, inv, outputData);
        }

        return true;
    }

    CLASS_REGISTER(CRCGAM, "1.0")
}


