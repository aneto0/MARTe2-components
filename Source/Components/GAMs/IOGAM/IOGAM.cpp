/**
 * @file IOGAM.cpp
 * @brief Source file for class IOGAM
 * @date 06/08/2016
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
 * the class IOGAM (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "IOGAM.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {
IOGAM::IOGAM() :
        GAM() {
    totalSignalsByteSize = 0u;
}

IOGAM::~IOGAM() {

}

bool IOGAM::Setup() {
    bool ret = true;
    uint32 n;
    uint32 inTotalSignalsByteSize = 0u;
    for (n = 0u; (n < GetNumberOfInputSignals()) && (ret); n++) {
        uint32 inByteSize = 0u;
        uint32 inSamples = 1u;
        ret = GetSignalByteSize(InputSignals, n, inByteSize);
        if (ret) {
            ret = GetSignalNumberOfSamples(InputSignals, n, inSamples);
        }
        if (ret) {
            inByteSize *= inSamples;
            inTotalSignalsByteSize += inByteSize;
        }
    }
    uint32 outTotalSignalsByteSize = 0u;
    for (n = 0u; (n < GetNumberOfOutputSignals()) && (ret); n++) {
        uint32 outByteSize = 0u;
        uint32 outSamples = 1u;
        ret = GetSignalByteSize(OutputSignals, n, outByteSize);
        if (ret) {
            ret = GetSignalNumberOfSamples(OutputSignals, n, outSamples);
        }
        if (ret) {
            outByteSize *= outSamples;
            outTotalSignalsByteSize += outByteSize;
        }
    }
    if (ret) {
        ret = (inTotalSignalsByteSize == outTotalSignalsByteSize);
        if (!ret) {
            REPORT_ERROR(ErrorManagement::InitialisationError,
                         "GetSignalByteSize(InputSignals): %d != GetSignalByteSize(OutputSignals): %d",
                         inTotalSignalsByteSize, outTotalSignalsByteSize);
        }
    }
    if (ret) {
        totalSignalsByteSize = outTotalSignalsByteSize;
    }

    return ret;
}

bool IOGAM::Execute() {
    return MemoryOperationsHelper::Copy(GetOutputSignalsMemory(), GetInputSignalsMemory(), totalSignalsByteSize);
}
CLASS_REGISTER(IOGAM, "1.0")
}

