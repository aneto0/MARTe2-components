/**
 * @file MarkerBitChecker.cpp
 * @brief Source file for class MarkerBitChecker
 * @date 11/02/2021
 * @author Giuseppe Ferro
 * @author Pedro Lourenco
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
 *
 * @details This source file contains the definition of all the methods for
 * the class MarkerBitChecker (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <stdio.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "MarkerBitChecker.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

MarkerBitChecker::MarkerBitChecker() :
        SampleChecker() {
    // Auto-generated constructor stub for MarkerBitChecker
    // TODO Verify if manual additions are needed

    resetBitMask = 0ull;
    bitMask = 0ull;
}

MarkerBitChecker::~MarkerBitChecker() {
    // Auto-generated destructor stub for MarkerBitChecker
    // TODO Verify if manual additions are needed
}

bool MarkerBitChecker::Initialise(StructuredDataI &data) {
    
    bool ret = SampleChecker::Initialise(data);
    printf("Initialise beg MarkerBitChecker\n");

    if (ret) {
        ret = (nFrameForSync == 1u);
        if (ret) {
            ret = data.Read("MarkerBitMask", bitMask);
            if (!ret) {
                REPORT_ERROR(ErrorManagement::InitialisationError, "Please define MarkerBit");
            }

            if (ret) {
                if (!data.Read("ResetBitMask", resetBitMask)) {
                    resetBitMask = bitMask;
                }
            }

        }
        else {
            REPORT_ERROR(ErrorManagement::InitialisationError, "NumOfFrameForSync must be equal to 1");
        }
    }
    printf("Initialise end MarkerBitChecker\n");

    return ret;
}

/*lint -e{952} -e{578} parameter 'sample' not declared as const*/
bool MarkerBitChecker::Check(uint8 *sample,
                             bool &write) {
    write = true;
    uint64 temp = 0ull;
    bool ret = MemoryOperationsHelper::Copy(&temp, sample, sampleSize);
    if (ret) {
        ret = ((temp & bitMask) != 0ull);
    }
    if (ret) {
        temp &= ~(resetBitMask);
        ret = MemoryOperationsHelper::Copy(sample, &temp, sampleSize);
    }

    return ret;
}

/*lint -e{952} -e{578} parameters 'frames' and 'sizeToRead' not declared as const*/
bool MarkerBitChecker::Synchronise(uint8 *frames,
                                   uint32 sizeToRead,
                                   uint32 &idx,
                                   bool &write) {

    idx = 0u;
    write = true;
    bool ret = false;

    while (idx < sizeToRead) {
        ret = Check(&frames[idx], write);
        if (ret) {
            break;
        }
        idx += sampleSize;
    }

    return ret;
}

CLASS_REGISTER(MarkerBitChecker, "1.0");

}
