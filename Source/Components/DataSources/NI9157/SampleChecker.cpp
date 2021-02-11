/**
 * @file SampleChecker.cpp
 * @brief Source file for class SampleChecker
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
 * the class SampleChecker (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <stdio.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "SampleChecker.h"
#include "AdvancedErrorManagement.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

SampleChecker::SampleChecker() :
        Object() {
    // Auto-generated constructor stub for SampleChecker
    // TODO Verify if manual additions are needed

    sampleSize = 0u;
    nFrameForSync = 0u;

}

SampleChecker::~SampleChecker() {
    // Auto-generated destructor stub for SampleChecker
    // TODO Verify if manual additions are needed
}

bool SampleChecker::Initialise(StructuredDataI &data) {

    bool ret = Object::Initialise(data);
    printf("Initialise beg SampleChecker\n");

    if (ret) {
        ret = data.Read("SampleSize", sampleSize);
        if (ret) {
            //read the number of frame to use to resync
            if (data.Read("NumOfFrameForSync", nFrameForSync)) {
                ret = (nFrameForSync > 0u);
                if (!ret) {
                    REPORT_ERROR(ErrorManagement::InitialisationError, "NumOfFrameForSync must be > 0");
                }
            }
            else {
                nFrameForSync = 1u;
            }
        }
        else {
            REPORT_ERROR(ErrorManagement::InitialisationError, "Please define SampleSize");
        }
    }
    printf("Initialise end SampleChecker\n");

    return ret;
}

uint8 SampleChecker::GetNumberOfFramesToSync() const{
    return nFrameForSync;
}

}
