/**
 * @file TypeCastGAM.cpp
 * @brief Source file for class TypeCastGAM
 * @date 16/01/2017
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
 * the class TypeCastGAM (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"

#include "TypeCastGAM.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {
TypeCastGAM::TypeCastGAM() :
        GAM() {
    signalNumberOfElements = NULL;
    signalNumberOfSamples = NULL;
    inputSignalType = NULL;
    outputSignalType = NULL;
    numberOfSignals = 0u;
}

TypeCastGAM::~TypeCastGAM() {
    if (signalNumberOfElements != NULL) {
        delete[] signalNumberOfElements;
    }
    if (signalNumberOfSamples != NULL) {
        delete[] signalNumberOfSamples;
    }
    if (inputSignalType != NULL) {
        delete[] inputSignalType;
    }
    if (outputSignalType != NULL) {
        delete[] outputSignalType;
    }
}

bool TypeCastGAM::Setup() {
    bool ret = (GetNumberOfInputSignals() == GetNumberOfOutputSignals());
    if (!ret) {
        REPORT_ERROR(ErrorManagement::InitialisationError, "GetNumberOfInputSignals() != GetNumberOfOutputSignals()");
    }
    uint32 n;
    if (ret) {
        numberOfSignals = GetNumberOfInputSignals();
        signalNumberOfElements = new uint32[numberOfSignals];
        signalNumberOfSamples = new uint32[numberOfSignals];
        inputSignalType = new TypeDescriptor[numberOfSignals];
        outputSignalType = new TypeDescriptor[numberOfSignals];
    }

    for (n = 0u; (n < GetNumberOfInputSignals()) && (ret); n++) {
        uint32 inSamples = 1u;
        uint32 outSamples = 1u;
        uint32 idx = n;
        ret = GetSignalNumberOfSamples(InputSignals, idx, inSamples);
        if (ret) {
            ret = GetSignalNumberOfSamples(OutputSignals, idx, outSamples);
        }
        if (ret) {
            ret = (inSamples == outSamples);
            if (!ret) {
                REPORT_ERROR_PARAMETERS(ErrorManagement::InitialisationError,
                                        "GetSignalNumberOfSamples(InputSignals, %d) != GetSignalNumberOfSamples(OutputSignals, %d)", idx, idx)
            }
        }
        if (ret) {
            signalNumberOfSamples[idx] = inSamples;
        }

        uint32 inElements = 1u;
        uint32 outElements = 1u;

        if (ret) {
            ret = GetSignalNumberOfElements(InputSignals, idx, inElements);
        }
        if (ret) {
            ret = GetSignalNumberOfElements(OutputSignals, idx, outElements);
        }
        if (ret) {
            ret = (inElements == outElements);
            if (!ret) {
                REPORT_ERROR_PARAMETERS(ErrorManagement::InitialisationError,
                                        "GetSignalNumberOfElements(InputSignals, %d) != GetSignalNumberOfElements(OutputSignals, %d)", idx, idx)
            }
        }
        if (ret) {
            signalNumberOfElements[idx] = inElements;
        }

        TypeDescriptor inType = GetSignalType(InputSignals, idx);
        TypeDescriptor outType = GetSignalType(OutputSignals, idx);
        if (ret) {
            inputSignalType[idx] = inType;
            outputSignalType[idx] = outType;
        }
    }

    return ret;
}

bool TypeCastGAM::Execute() {
    uint32 i;
    uint32 s;
    uint32 n;
    //TODO finish all cases
    for (i = 0u; i < numberOfSignals; i++) {
        void *inputSignal = GetInputSignalMemory(i);
        void *outputSignal = GetOutputSignalMemory(i);
        if (inputSignalType[i] == UnsignedInteger16Bit) {
            if (outputSignalType[i] == UnsignedInteger32Bit) {
                for (s = 0u; s < signalNumberOfSamples[i]; s++) {
                    for (n = 0u; n < signalNumberOfElements[i]; n++) {
                        uint32 *dest = reinterpret_cast<uint32 *>(outputSignal);
                        uint16 *src = reinterpret_cast<uint16 *>(inputSignal);
                        dest[s * signalNumberOfElements[i] + n] = src[s * signalNumberOfElements[i] + n];
                    }
                }
            }
            else if (outputSignalType[i] == SignedInteger32Bit) {
                for (s = 0u; s < signalNumberOfSamples[i]; s++) {
                    for (n = 0u; n < signalNumberOfElements[i]; n++) {
                        int32 *dest = reinterpret_cast<int32 *>(outputSignal);
                        uint16 *src = reinterpret_cast<uint16 *>(inputSignal);
                        dest[s * signalNumberOfElements[i] + n] = src[s * signalNumberOfElements[i] + n];
                    }
                }
            }
            else if (outputSignalType[i] == Float32Bit) {
                for (s = 0u; s < signalNumberOfSamples[i]; s++) {
                    for (n = 0u; n < signalNumberOfElements[i]; n++) {
                        float32 *dest = reinterpret_cast<float32 *>(outputSignal);
                        uint16 *src = reinterpret_cast<uint16 *>(inputSignal);
                        dest[s * signalNumberOfElements[i] + n] = src[s * signalNumberOfElements[i] + n];
                    }
                }
            }
        }
        else if (inputSignalType[i] == SignedInteger16Bit) {
            if (outputSignalType[i] == UnsignedInteger32Bit) {
                for (s = 0u; s < signalNumberOfSamples[i]; s++) {
                    for (n = 0u; n < signalNumberOfElements[i]; n++) {
                        uint32 *dest = reinterpret_cast<uint32 *>(outputSignal);
                        int16 *src = reinterpret_cast<int16 *>(inputSignal);
                        dest[s * signalNumberOfElements[i] + n] = src[s * signalNumberOfElements[i] + n];
                    }
                }
            }
            else if (outputSignalType[i] == SignedInteger32Bit) {
                for (s = 0u; s < signalNumberOfSamples[i]; s++) {
                    for (n = 0u; n < signalNumberOfElements[i]; n++) {
                        int32 *dest = reinterpret_cast<int32 *>(outputSignal);
                        int16 *src = reinterpret_cast<int16 *>(inputSignal);
                        dest[s * signalNumberOfElements[i] + n] = src[s * signalNumberOfElements[i] + n];
                    }
                }
            }
            else if (outputSignalType[i] == Float32Bit) {
                for (s = 0u; s < signalNumberOfSamples[i]; s++) {
                    for (n = 0u; n < signalNumberOfElements[i]; n++) {
                        float32 *dest = reinterpret_cast<float32 *>(outputSignal);
                        int16 *src = reinterpret_cast<int16 *>(inputSignal);
                        dest[s * signalNumberOfElements[i] + n] = src[s * signalNumberOfElements[i] + n];
                    }
                }
            }
        }
    }

    return true;
}
CLASS_REGISTER(TypeCastGAM, "1.0")
}

