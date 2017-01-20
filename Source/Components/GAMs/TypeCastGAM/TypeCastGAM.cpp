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
#include "ConversionHelper.h"
#include "ConversionHelperT.h"
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
    numberOfSignals = 0u;
    conversionHelpers = NULL;
}

TypeCastGAM::~TypeCastGAM() {
    uint32 i;
    if (conversionHelpers != NULL) {
        for (i = 0u; i < numberOfSignals; i++) {
            delete conversionHelpers[i];
        }
        delete[] conversionHelpers;
    }
}

bool TypeCastGAM::Initialise(StructuredDataI & data) {
    bool ok = GAM::Initialise(data);
    if (ok) {
        ok = data.Copy(cdb);
    }
    return ok;
}

bool TypeCastGAM::Setup() {
    bool ret = (GetNumberOfInputSignals() == GetNumberOfOutputSignals());
    if (!ret) {
        REPORT_ERROR(ErrorManagement::InitialisationError, "GetNumberOfInputSignals() != GetNumberOfOutputSignals()");
    }
    uint32 n;
    uint32 signalNumberOfElements = 0u;
    uint32 signalNumberOfSamples = 0u;
    if (ret) {
        numberOfSignals = GetNumberOfInputSignals();
        conversionHelpers = new ConversionHelper*[numberOfSignals];
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
            signalNumberOfSamples = inSamples;
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
            signalNumberOfElements = inElements;
        }

        if (ret) {
            ret = cdb.MoveRelative("OutputSignals");
        }

        TypeDescriptor inputSignalType = GetSignalType(InputSignals, idx);
        TypeDescriptor outputSignalType = GetSignalType(OutputSignals, idx);
        if (ret) {
            if (inputSignalType == UnsignedInteger8Bit) {
                if (outputSignalType == UnsignedInteger8Bit) {
                    conversionHelpers[idx] = new ConversionHelperT<uint8, uint8>(GetInputSignalMemory(idx), GetOutputSignalMemory(idx));
                }
                else if (outputSignalType == SignedInteger8Bit) {
                    conversionHelpers[idx] = new ConversionHelperT<uint8, int8>(GetInputSignalMemory(idx), GetOutputSignalMemory(idx));
                }
                else if (outputSignalType == UnsignedInteger16Bit) {
                    conversionHelpers[idx] = new ConversionHelperT<uint8, uint16>(GetInputSignalMemory(idx), GetOutputSignalMemory(idx));
                }
                else if (outputSignalType == SignedInteger16Bit) {
                    conversionHelpers[idx] = new ConversionHelperT<uint8, int16>(GetInputSignalMemory(idx), GetOutputSignalMemory(idx));
                }
                else if (outputSignalType == UnsignedInteger32Bit) {
                    conversionHelpers[idx] = new ConversionHelperT<uint8, uint32>(GetInputSignalMemory(idx), GetOutputSignalMemory(idx));
                }
                else if (outputSignalType == SignedInteger32Bit) {
                    conversionHelpers[idx] = new ConversionHelperT<uint8, int32>(GetInputSignalMemory(idx), GetOutputSignalMemory(idx));
                }
                else if (outputSignalType == UnsignedInteger64Bit) {
                    conversionHelpers[idx] = new ConversionHelperT<uint8, uint64>(GetInputSignalMemory(idx), GetOutputSignalMemory(idx));
                }
                else if (outputSignalType == SignedInteger64Bit) {
                    conversionHelpers[idx] = new ConversionHelperT<uint8, int64>(GetInputSignalMemory(idx), GetOutputSignalMemory(idx));
                }
                else if (outputSignalType == Float32Bit) {
                    conversionHelpers[idx] = new ConversionHelperT<uint8, float32>(GetInputSignalMemory(idx), GetOutputSignalMemory(idx));
                }
                else if (outputSignalType == Float64Bit) {
                    conversionHelpers[idx] = new ConversionHelperT<uint8, float64>(GetInputSignalMemory(idx), GetOutputSignalMemory(idx));
                }
                else {
                    //TODO report error
                }
            }
            else if (inputSignalType == SignedInteger8Bit) {
                if (outputSignalType == UnsignedInteger8Bit) {
                    conversionHelpers[idx] = new ConversionHelperT<int8, uint8>(GetInputSignalMemory(idx), GetOutputSignalMemory(idx));
                }
                else if (outputSignalType == SignedInteger8Bit) {
                    conversionHelpers[idx] = new ConversionHelperT<int8, int8>(GetInputSignalMemory(idx), GetOutputSignalMemory(idx));
                }
                else if (outputSignalType == UnsignedInteger16Bit) {
                    conversionHelpers[idx] = new ConversionHelperT<int8, uint16>(GetInputSignalMemory(idx), GetOutputSignalMemory(idx));
                }
                else if (outputSignalType == SignedInteger16Bit) {
                    conversionHelpers[idx] = new ConversionHelperT<int8, int16>(GetInputSignalMemory(idx), GetOutputSignalMemory(idx));
                }
                else if (outputSignalType == UnsignedInteger32Bit) {
                    conversionHelpers[idx] = new ConversionHelperT<int8, uint32>(GetInputSignalMemory(idx), GetOutputSignalMemory(idx));
                }
                else if (outputSignalType == SignedInteger32Bit) {
                    conversionHelpers[idx] = new ConversionHelperT<int8, int32>(GetInputSignalMemory(idx), GetOutputSignalMemory(idx));
                }
                else if (outputSignalType == UnsignedInteger64Bit) {
                    conversionHelpers[idx] = new ConversionHelperT<int8, uint64>(GetInputSignalMemory(idx), GetOutputSignalMemory(idx));
                }
                else if (outputSignalType == SignedInteger64Bit) {
                    conversionHelpers[idx] = new ConversionHelperT<int8, int64>(GetInputSignalMemory(idx), GetOutputSignalMemory(idx));
                }
                else if (outputSignalType == Float32Bit) {
                    conversionHelpers[idx] = new ConversionHelperT<int8, float32>(GetInputSignalMemory(idx), GetOutputSignalMemory(idx));
                }
                else if (outputSignalType == Float64Bit) {
                    conversionHelpers[idx] = new ConversionHelperT<int8, float64>(GetInputSignalMemory(idx), GetOutputSignalMemory(idx));
                }
                else {
                    //TODO report error
                }
            }
            else if (inputSignalType == UnsignedInteger16Bit) {
                if (outputSignalType == UnsignedInteger8Bit) {
                    conversionHelpers[idx] = new ConversionHelperT<uint16, uint8>(GetInputSignalMemory(idx), GetOutputSignalMemory(idx));
                }
                else if (outputSignalType == SignedInteger8Bit) {
                    conversionHelpers[idx] = new ConversionHelperT<uint16, int8>(GetInputSignalMemory(idx), GetOutputSignalMemory(idx));
                }
                else if (outputSignalType == UnsignedInteger16Bit) {
                    conversionHelpers[idx] = new ConversionHelperT<uint16, uint16>(GetInputSignalMemory(idx), GetOutputSignalMemory(idx));
                }
                else if (outputSignalType == SignedInteger16Bit) {
                    conversionHelpers[idx] = new ConversionHelperT<uint16, int16>(GetInputSignalMemory(idx), GetOutputSignalMemory(idx));
                }
                else if (outputSignalType == UnsignedInteger32Bit) {
                    conversionHelpers[idx] = new ConversionHelperT<uint16, uint32>(GetInputSignalMemory(idx), GetOutputSignalMemory(idx));
                }
                else if (outputSignalType == SignedInteger32Bit) {
                    conversionHelpers[idx] = new ConversionHelperT<uint16, int32>(GetInputSignalMemory(idx), GetOutputSignalMemory(idx));
                }
                else if (outputSignalType == UnsignedInteger64Bit) {
                    conversionHelpers[idx] = new ConversionHelperT<uint16, uint64>(GetInputSignalMemory(idx), GetOutputSignalMemory(idx));
                }
                else if (outputSignalType == SignedInteger64Bit) {
                    conversionHelpers[idx] = new ConversionHelperT<uint16, int64>(GetInputSignalMemory(idx), GetOutputSignalMemory(idx));
                }
                else if (outputSignalType == Float32Bit) {
                    conversionHelpers[idx] = new ConversionHelperT<uint16, float32>(GetInputSignalMemory(idx), GetOutputSignalMemory(idx));
                }
                else if (outputSignalType == Float64Bit) {
                    conversionHelpers[idx] = new ConversionHelperT<uint16, float64>(GetInputSignalMemory(idx), GetOutputSignalMemory(idx));
                }
                else {
                    //TODO report error
                }
            }
            else if (inputSignalType == SignedInteger16Bit) {
                if (outputSignalType == UnsignedInteger8Bit) {
                    conversionHelpers[idx] = new ConversionHelperT<int16, uint8>(GetInputSignalMemory(idx), GetOutputSignalMemory(idx));
                }
                else if (outputSignalType == SignedInteger8Bit) {
                    conversionHelpers[idx] = new ConversionHelperT<int16, int8>(GetInputSignalMemory(idx), GetOutputSignalMemory(idx));
                }
                else if (outputSignalType == UnsignedInteger16Bit) {
                    conversionHelpers[idx] = new ConversionHelperT<int16, uint16>(GetInputSignalMemory(idx), GetOutputSignalMemory(idx));
                }
                else if (outputSignalType == SignedInteger16Bit) {
                    conversionHelpers[idx] = new ConversionHelperT<int16, int16>(GetInputSignalMemory(idx), GetOutputSignalMemory(idx));
                }
                else if (outputSignalType == UnsignedInteger32Bit) {
                    conversionHelpers[idx] = new ConversionHelperT<int16, uint32>(GetInputSignalMemory(idx), GetOutputSignalMemory(idx));
                }
                else if (outputSignalType == SignedInteger32Bit) {
                    conversionHelpers[idx] = new ConversionHelperT<int16, int32>(GetInputSignalMemory(idx), GetOutputSignalMemory(idx));
                }
                else if (outputSignalType == UnsignedInteger64Bit) {
                    conversionHelpers[idx] = new ConversionHelperT<int16, uint64>(GetInputSignalMemory(idx), GetOutputSignalMemory(idx));
                }
                else if (outputSignalType == SignedInteger64Bit) {
                    conversionHelpers[idx] = new ConversionHelperT<int16, int64>(GetInputSignalMemory(idx), GetOutputSignalMemory(idx));
                }
                else if (outputSignalType == Float32Bit) {
                    conversionHelpers[idx] = new ConversionHelperT<int16, float32>(GetInputSignalMemory(idx), GetOutputSignalMemory(idx));
                }
                else if (outputSignalType == Float64Bit) {
                    conversionHelpers[idx] = new ConversionHelperT<int16, float64>(GetInputSignalMemory(idx), GetOutputSignalMemory(idx));
                }
                else {
                    //TODO report error
                }
            }
            else if (inputSignalType == UnsignedInteger32Bit) {
                if (outputSignalType == UnsignedInteger8Bit) {
                    conversionHelpers[idx] = new ConversionHelperT<uint32, uint8>(GetInputSignalMemory(idx), GetOutputSignalMemory(idx));
                }
                else if (outputSignalType == SignedInteger8Bit) {
                    conversionHelpers[idx] = new ConversionHelperT<uint32, int8>(GetInputSignalMemory(idx), GetOutputSignalMemory(idx));
                }
                else if (outputSignalType == UnsignedInteger16Bit) {
                    conversionHelpers[idx] = new ConversionHelperT<uint32, uint16>(GetInputSignalMemory(idx), GetOutputSignalMemory(idx));
                }
                else if (outputSignalType == SignedInteger16Bit) {
                    conversionHelpers[idx] = new ConversionHelperT<uint32, int16>(GetInputSignalMemory(idx), GetOutputSignalMemory(idx));
                }
                else if (outputSignalType == UnsignedInteger32Bit) {
                    conversionHelpers[idx] = new ConversionHelperT<uint32, uint32>(GetInputSignalMemory(idx), GetOutputSignalMemory(idx));
                }
                else if (outputSignalType == SignedInteger32Bit) {
                    conversionHelpers[idx] = new ConversionHelperT<uint32, int32>(GetInputSignalMemory(idx), GetOutputSignalMemory(idx));
                }
                else if (outputSignalType == UnsignedInteger64Bit) {
                    conversionHelpers[idx] = new ConversionHelperT<uint32, uint64>(GetInputSignalMemory(idx), GetOutputSignalMemory(idx));
                }
                else if (outputSignalType == SignedInteger64Bit) {
                    conversionHelpers[idx] = new ConversionHelperT<uint32, int64>(GetInputSignalMemory(idx), GetOutputSignalMemory(idx));
                }
                else if (outputSignalType == Float32Bit) {
                    conversionHelpers[idx] = new ConversionHelperT<uint32, float32>(GetInputSignalMemory(idx), GetOutputSignalMemory(idx));
                }
                else if (outputSignalType == Float64Bit) {
                    conversionHelpers[idx] = new ConversionHelperT<uint32, float64>(GetInputSignalMemory(idx), GetOutputSignalMemory(idx));
                }
                else {
                    //TODO report error
                }
            }
            else if (inputSignalType == SignedInteger32Bit) {
                if (outputSignalType == UnsignedInteger8Bit) {
                    conversionHelpers[idx] = new ConversionHelperT<int32, uint8>(GetInputSignalMemory(idx), GetOutputSignalMemory(idx));
                }
                else if (outputSignalType == SignedInteger8Bit) {
                    conversionHelpers[idx] = new ConversionHelperT<int32, int8>(GetInputSignalMemory(idx), GetOutputSignalMemory(idx));
                }
                else if (outputSignalType == UnsignedInteger16Bit) {
                    conversionHelpers[idx] = new ConversionHelperT<int32, uint16>(GetInputSignalMemory(idx), GetOutputSignalMemory(idx));
                }
                else if (outputSignalType == SignedInteger16Bit) {
                    conversionHelpers[idx] = new ConversionHelperT<int32, int16>(GetInputSignalMemory(idx), GetOutputSignalMemory(idx));
                }
                else if (outputSignalType == UnsignedInteger32Bit) {
                    conversionHelpers[idx] = new ConversionHelperT<int32, uint32>(GetInputSignalMemory(idx), GetOutputSignalMemory(idx));
                }
                else if (outputSignalType == SignedInteger32Bit) {
                    conversionHelpers[idx] = new ConversionHelperT<int32, int32>(GetInputSignalMemory(idx), GetOutputSignalMemory(idx));
                }
                else if (outputSignalType == UnsignedInteger64Bit) {
                    conversionHelpers[idx] = new ConversionHelperT<int32, uint64>(GetInputSignalMemory(idx), GetOutputSignalMemory(idx));
                }
                else if (outputSignalType == SignedInteger64Bit) {
                    conversionHelpers[idx] = new ConversionHelperT<int32, int64>(GetInputSignalMemory(idx), GetOutputSignalMemory(idx));
                }
                else if (outputSignalType == Float32Bit) {
                    conversionHelpers[idx] = new ConversionHelperT<int32, float32>(GetInputSignalMemory(idx), GetOutputSignalMemory(idx));
                }
                else if (outputSignalType == Float64Bit) {
                    conversionHelpers[idx] = new ConversionHelperT<int32, float64>(GetInputSignalMemory(idx), GetOutputSignalMemory(idx));
                }
                else {
                    //TODO report error
                }
            }
            else if (inputSignalType == UnsignedInteger64Bit) {
                if (outputSignalType == UnsignedInteger8Bit) {
                    conversionHelpers[idx] = new ConversionHelperT<uint64, uint8>(GetInputSignalMemory(idx), GetOutputSignalMemory(idx));
                }
                else if (outputSignalType == SignedInteger8Bit) {
                    conversionHelpers[idx] = new ConversionHelperT<uint64, int8>(GetInputSignalMemory(idx), GetOutputSignalMemory(idx));
                }
                else if (outputSignalType == UnsignedInteger16Bit) {
                    conversionHelpers[idx] = new ConversionHelperT<uint64, uint16>(GetInputSignalMemory(idx), GetOutputSignalMemory(idx));
                }
                else if (outputSignalType == SignedInteger16Bit) {
                    conversionHelpers[idx] = new ConversionHelperT<uint64, int16>(GetInputSignalMemory(idx), GetOutputSignalMemory(idx));
                }
                else if (outputSignalType == UnsignedInteger32Bit) {
                    conversionHelpers[idx] = new ConversionHelperT<uint64, uint32>(GetInputSignalMemory(idx), GetOutputSignalMemory(idx));
                }
                else if (outputSignalType == SignedInteger32Bit) {
                    conversionHelpers[idx] = new ConversionHelperT<uint64, int32>(GetInputSignalMemory(idx), GetOutputSignalMemory(idx));
                }
                else if (outputSignalType == UnsignedInteger64Bit) {
                    conversionHelpers[idx] = new ConversionHelperT<uint64, uint64>(GetInputSignalMemory(idx), GetOutputSignalMemory(idx));
                }
                else if (outputSignalType == SignedInteger64Bit) {
                    conversionHelpers[idx] = new ConversionHelperT<uint64, int64>(GetInputSignalMemory(idx), GetOutputSignalMemory(idx));
                }
                else if (outputSignalType == Float32Bit) {
                    conversionHelpers[idx] = new ConversionHelperT<uint64, float32>(GetInputSignalMemory(idx), GetOutputSignalMemory(idx));
                }
                else if (outputSignalType == Float64Bit) {
                    conversionHelpers[idx] = new ConversionHelperT<uint64, float64>(GetInputSignalMemory(idx), GetOutputSignalMemory(idx));
                }
                else {
                    //TODO report error
                }
            }
            else if (inputSignalType == SignedInteger64Bit) {
                if (outputSignalType == UnsignedInteger8Bit) {
                    conversionHelpers[idx] = new ConversionHelperT<int64, uint8>(GetInputSignalMemory(idx), GetOutputSignalMemory(idx));
                }
                else if (outputSignalType == SignedInteger8Bit) {
                    conversionHelpers[idx] = new ConversionHelperT<int64, int8>(GetInputSignalMemory(idx), GetOutputSignalMemory(idx));
                }
                else if (outputSignalType == UnsignedInteger16Bit) {
                    conversionHelpers[idx] = new ConversionHelperT<int64, uint16>(GetInputSignalMemory(idx), GetOutputSignalMemory(idx));
                }
                else if (outputSignalType == SignedInteger16Bit) {
                    conversionHelpers[idx] = new ConversionHelperT<int64, int16>(GetInputSignalMemory(idx), GetOutputSignalMemory(idx));
                }
                else if (outputSignalType == UnsignedInteger32Bit) {
                    conversionHelpers[idx] = new ConversionHelperT<int64, uint32>(GetInputSignalMemory(idx), GetOutputSignalMemory(idx));
                }
                else if (outputSignalType == SignedInteger32Bit) {
                    conversionHelpers[idx] = new ConversionHelperT<int64, int32>(GetInputSignalMemory(idx), GetOutputSignalMemory(idx));
                }
                else if (outputSignalType == UnsignedInteger64Bit) {
                    conversionHelpers[idx] = new ConversionHelperT<int64, uint64>(GetInputSignalMemory(idx), GetOutputSignalMemory(idx));
                }
                else if (outputSignalType == SignedInteger64Bit) {
                    conversionHelpers[idx] = new ConversionHelperT<int64, int64>(GetInputSignalMemory(idx), GetOutputSignalMemory(idx));
                }
                else if (outputSignalType == Float32Bit) {
                    conversionHelpers[idx] = new ConversionHelperT<int64, float32>(GetInputSignalMemory(idx), GetOutputSignalMemory(idx));
                }
                else if (outputSignalType == Float64Bit) {
                    conversionHelpers[idx] = new ConversionHelperT<int64, float64>(GetInputSignalMemory(idx), GetOutputSignalMemory(idx));
                }
                else {
                    //TODO report error
                }
            }
            else if (inputSignalType == Float32Bit) {
                if (outputSignalType == UnsignedInteger8Bit) {
                    conversionHelpers[idx] = new ConversionHelperT<float32, uint8>(GetInputSignalMemory(idx), GetOutputSignalMemory(idx));
                }
                else if (outputSignalType == SignedInteger8Bit) {
                    conversionHelpers[idx] = new ConversionHelperT<float32, int8>(GetInputSignalMemory(idx), GetOutputSignalMemory(idx));
                }
                else if (outputSignalType == UnsignedInteger16Bit) {
                    conversionHelpers[idx] = new ConversionHelperT<float32, uint16>(GetInputSignalMemory(idx), GetOutputSignalMemory(idx));
                }
                else if (outputSignalType == SignedInteger16Bit) {
                    conversionHelpers[idx] = new ConversionHelperT<float32, int16>(GetInputSignalMemory(idx), GetOutputSignalMemory(idx));
                }
                else if (outputSignalType == UnsignedInteger32Bit) {
                    conversionHelpers[idx] = new ConversionHelperT<float32, uint32>(GetInputSignalMemory(idx), GetOutputSignalMemory(idx));
                }
                else if (outputSignalType == SignedInteger32Bit) {
                    conversionHelpers[idx] = new ConversionHelperT<float32, int32>(GetInputSignalMemory(idx), GetOutputSignalMemory(idx));
                }
                else if (outputSignalType == UnsignedInteger64Bit) {
                    conversionHelpers[idx] = new ConversionHelperT<float32, uint64>(GetInputSignalMemory(idx), GetOutputSignalMemory(idx));
                }
                else if (outputSignalType == SignedInteger64Bit) {
                    conversionHelpers[idx] = new ConversionHelperT<float32, int64>(GetInputSignalMemory(idx), GetOutputSignalMemory(idx));
                }
                else if (outputSignalType == Float32Bit) {
                    conversionHelpers[idx] = new ConversionHelperT<float32, float32>(GetInputSignalMemory(idx), GetOutputSignalMemory(idx));
                }
                else if (outputSignalType == Float64Bit) {
                    conversionHelpers[idx] = new ConversionHelperT<float32, float64>(GetInputSignalMemory(idx), GetOutputSignalMemory(idx));
                }
                else {
                    //TODO report error
                }
            }
            else if (inputSignalType == Float64Bit) {
                if (outputSignalType == UnsignedInteger8Bit) {
                    conversionHelpers[idx] = new ConversionHelperT<float64, uint8>(GetInputSignalMemory(idx), GetOutputSignalMemory(idx));
                }
                else if (outputSignalType == SignedInteger8Bit) {
                    conversionHelpers[idx] = new ConversionHelperT<float64, int8>(GetInputSignalMemory(idx), GetOutputSignalMemory(idx));
                }
                else if (outputSignalType == UnsignedInteger16Bit) {
                    conversionHelpers[idx] = new ConversionHelperT<float64, uint16>(GetInputSignalMemory(idx), GetOutputSignalMemory(idx));
                }
                else if (outputSignalType == SignedInteger16Bit) {
                    conversionHelpers[idx] = new ConversionHelperT<float64, int16>(GetInputSignalMemory(idx), GetOutputSignalMemory(idx));
                }
                else if (outputSignalType == UnsignedInteger32Bit) {
                    conversionHelpers[idx] = new ConversionHelperT<float64, uint32>(GetInputSignalMemory(idx), GetOutputSignalMemory(idx));
                }
                else if (outputSignalType == SignedInteger32Bit) {
                    conversionHelpers[idx] = new ConversionHelperT<float64, int32>(GetInputSignalMemory(idx), GetOutputSignalMemory(idx));
                }
                else if (outputSignalType == UnsignedInteger64Bit) {
                    conversionHelpers[idx] = new ConversionHelperT<float64, uint64>(GetInputSignalMemory(idx), GetOutputSignalMemory(idx));
                }
                else if (outputSignalType == SignedInteger64Bit) {
                    conversionHelpers[idx] = new ConversionHelperT<float64, int64>(GetInputSignalMemory(idx), GetOutputSignalMemory(idx));
                }
                else if (outputSignalType == Float32Bit) {
                    conversionHelpers[idx] = new ConversionHelperT<float64, float32>(GetInputSignalMemory(idx), GetOutputSignalMemory(idx));
                }
                else if (outputSignalType == Float64Bit) {
                    conversionHelpers[idx] = new ConversionHelperT<float64, float64>(GetInputSignalMemory(idx), GetOutputSignalMemory(idx));
                }
                else {
                    //TODO report error
                }
            }
            else {
                //TODO report error
            }
            if (conversionHelpers[idx] != NULL_PTR(ConversionHelper *)) {
                conversionHelpers[idx]->SetNumberOfElements(signalNumberOfElements);
                conversionHelpers[idx]->SetNumberOfSamples(signalNumberOfSamples);
                if (ret) {
                    StreamString signalName = cdb.GetChildName(idx);
                    ret = cdb.MoveRelative(signalName.Buffer());
                }
                if (ret) {
                    conversionHelpers[idx]->LoadGain(cdb);
                    ret = cdb.MoveToAncestor(1u);
                }
            }
        }
    }

    return ret;
}

bool TypeCastGAM::Execute() {
    uint32 i;
    //TODO finish all cases
    for (i = 0u; i < numberOfSignals; i++) {
        conversionHelpers[i]->Convert();
    }

    return true;
}
CLASS_REGISTER(TypeCastGAM, "1.0")
}

