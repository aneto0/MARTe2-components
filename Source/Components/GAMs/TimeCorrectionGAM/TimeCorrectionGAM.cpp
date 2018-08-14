/**
 * @file TimeCorrectionGAM.cpp
 * @brief Source file for class TimeCorrectionGAM
 * @date 07/06/2018
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
 * the class TimeCorrectionGAM (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "AdvancedErrorManagement.h"
#include "TimeCorrectionGAM.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

TimeCorrectionGAM::TimeCorrectionGAM() :
        GAM() {
    expectedDelta = 0ull;
    deltaTolerance = 0ull;
    filterGain = 0.F;
    estimatedDelta = 0.;
    inputTime = NULL_PTR(uint64 *);
    correctedTime = NULL_PTR(uint64 *);
    corrected = NULL_PTR(uint8 *);
    lastValidTime = 0ull;
    iterationCounter = 0u;
}

TimeCorrectionGAM::~TimeCorrectionGAM() {
    inputTime = NULL_PTR(uint64 *);
    correctedTime = NULL_PTR(uint64 *);
    corrected = NULL_PTR(uint8 *);
}

bool TimeCorrectionGAM::Initialise(StructuredDataI & data) {
    bool ret = GAM::Initialise(data);
    if (ret) {
        ret = data.Read("ExpectedDelta", expectedDelta);
        if (!ret) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "ExpectedDelta shall be defined");
        }
    }

    if (ret) {
        ret = data.Read("DeltaTolerance", deltaTolerance);
        if (!ret) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "DeltaTolerance shall be defined");
        }
    }

    if (ret) {
        ret = data.Read("FilterGain", filterGain);
        if (!ret) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "FilterGain shall be defined");
        }
        if (ret) {
            ret = ((filterGain > 0.) && (filterGain < 1.));
            if (ret) {
                REPORT_ERROR(ErrorManagement::InitialisationError, "FilterGain must be in [0, 1]");
            }
        }
    }
    return ret;
}

bool TimeCorrectionGAM::Setup() {
    bool ret = (GetNumberOfInputSignals() == 1u);

    if (ret) {
        uint32 nOfInputSignals = GetNumberOfOutputSignals();
        uint32 nOfOutputSignals = GetNumberOfOutputSignals();
        ret = ((nOfInputSignals > 0u) && (nOfOutputSignals <= 2u));
        if (ret) {
            REPORT_ERROR(ErrorManagement::FatalError, "This Function allows only one or two output signals");
        }

    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "This Function allows only one input signal");
    }

    //check the signals are uint64 type
    if (ret) {
        TypeDescriptor td = GetSignalType(InputSignals, 0u);
        ret = (td == UnsignedInteger64Bit);
        if (ret) {
            REPORT_ERROR(ErrorManagement::FatalError, "The input time signal type must be uint64");
        }
    }
    if (ret) {
        TypeDescriptor td = GetSignalType(OutputSignals, 0u);
        ret = (td == UnsignedInteger64Bit);
        if (ret) {
            REPORT_ERROR(ErrorManagement::FatalError, "The corrected time signal (0) type must be uint64");
        }
    }

    if (ret) {
        if (GetNumberOfOutputSignals() > 1u) {
            TypeDescriptor td = GetSignalType(OutputSignals, 1u);
            ret = (td == UnsignedInteger8Bit);
            if (ret) {
                REPORT_ERROR(ErrorManagement::FatalError, "The corrected flag signal (1) type must be uint8");
            }
        }
    }

    //check that the number of elements is 1
    if (ret) {
        uint32 numberOfElements = 0u;
        ret = GetSignalNumberOfElements(InputSignals, 0u, numberOfElements);
        if (ret) {
            ret = (numberOfElements == 1u);
            if (ret) {
                REPORT_ERROR(ErrorManagement::FatalError, "The input signal type must be scalar");
            }
        }
    }
    if (ret) {

        uint32 numberOfElements = 0u;
        ret = GetSignalNumberOfElements(OutputSignals, 0u, numberOfElements);
        if (ret) {
            ret = (numberOfElements == 1u);
            if (ret) {
                REPORT_ERROR(ErrorManagement::FatalError, "The corrected time signal (0) must be scalar");
            }
        }
    }
    if (ret) {
        if (GetNumberOfOutputSignals() > 1u) {

            uint32 numberOfElements = 0u;
            ret = GetSignalNumberOfElements(OutputSignals, 1u, numberOfElements);
            if (ret) {
                ret = (numberOfElements == 1u);
                if (ret) {
                    REPORT_ERROR(ErrorManagement::FatalError, "The corrected flag signal (1) must be scalar");
                }
            }
        }
    }
    if (ret) {
        inputTime = reinterpret_cast<uint64 *>(GetInputSignalMemory(0u));
        correctedTime = reinterpret_cast<uint64 *>(GetOutputSignalMemory(0u));
        if (GetNumberOfOutputSignals() > 1u) {
            corrected = reinterpret_cast<uint8 *>(GetOutputSignalMemory(1u));
        }
        estimatedDelta = static_cast<float64>(expectedDelta);
    }
    return ret;
}

/*lint -e{613} pointer checked before during the Setup*/
bool TimeCorrectionGAM::Execute() {
    //a good value
    if (iterationCounter > 0u) {
        uint64 delta = (*inputTime) - lastValidTime;
        if (((delta - expectedDelta) < deltaTolerance) || ((expectedDelta - delta) < deltaTolerance)) {
            estimatedDelta = static_cast<float64>(((1.0 - static_cast<float64>(filterGain)) * estimatedDelta) + (static_cast<float64>(filterGain) * static_cast<float64>(delta)));
            (*correctedTime) = (*inputTime);
            if (corrected != NULL_PTR(uint8*)) {
                (*corrected = 0u);
            }
        }
        //something wrong... correct the value
        else {
            float64 lastValidTimeF = static_cast<float64>(lastValidTime) + estimatedDelta;
            (*correctedTime) = static_cast<uint64>(lastValidTimeF);
            if ((estimatedDelta - static_cast<float64>(static_cast<uint64>(estimatedDelta))) > 0.5) {
                (*correctedTime)++;
            }

            if (corrected != NULL_PTR(uint8*)) {
                (*corrected = 1u);
            }
        }
        lastValidTime = (*correctedTime);
    }
    else {
        (*correctedTime) = (*inputTime);
        lastValidTime = (*inputTime);
        iterationCounter++;
    }
    //REPORT_ERROR(ErrorManagement::FatalError, "estimated delta = %f", estimatedDelta);

    return true;
}
CLASS_REGISTER(TimeCorrectionGAM, "1.0")
}

