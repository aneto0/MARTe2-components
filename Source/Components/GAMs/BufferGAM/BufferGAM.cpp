/**
 * @file BufferGAM.cpp
 * @brief Source file for class BufferGAM
 * @date 6 Aug 2016
 * @author andre
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
 * the class BufferGAM (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "BufferGAM.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {
BufferGAM::BufferGAM() :
        GAM() {
    totalSignalsByteSize = 0u;
}

bool BufferGAM::Setup() {
    bool ret = (GetNumberOfInputSignals() == GetNumberOfOutputSignals());
    if (!ret) {
        REPORT_ERROR(ErrorManagement::InitialisationError, "GetNumberOfInputSignals() != GetNumberOfOutputSignals()");
    }
    uint32 n;
    totalSignalsByteSize = 0u;
    for (n = 0u; (n < GetNumberOfInputSignals()) && (ret); n++) {
        uint32 inByteSize = 0u;
        uint32 outByteSize = 0u;
        uint32 idx = n;
        ret = GetSignalByteSize(InputSignals, idx, inByteSize);
        if (ret) {
            totalSignalsByteSize += inByteSize;
            ret = GetSignalByteSize(OutputSignals, idx, outByteSize);
        }
        if (ret) {
            ret = (inByteSize == outByteSize);
            if (!ret) {
                REPORT_ERROR_PARAMETERS(ErrorManagement::InitialisationError, "GetSignalByteSize(InputSignals, %d) != GetSignalByteSize(OutputSignals, %d)", idx, idx)
            }
        }

        TypeDescriptor inType = GetSignalType(InputSignals, idx);
        TypeDescriptor outType = GetSignalType(OutputSignals, idx);
        if(ret) {
            ret = (inType == outType);
            if (!ret) {
                REPORT_ERROR_PARAMETERS(ErrorManagement::InitialisationError, "GetSignalType(InputSignals, %d) != GetSignalType(OutputSignals, %d)", idx, idx)
            }
        }
    }

    return ret;
}

bool BufferGAM::Execute() {
    return MemoryOperationsHelper::Copy(GetOutputSignalsMemory(), GetInputSignalsMemory(), totalSignalsByteSize);
}
CLASS_REGISTER(BufferGAM, "1.0")
}

