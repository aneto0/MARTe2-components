/**
 * @file FilterGAM.cpp
 * @brief Source file for class FilterGAM
 * @date Jan 30, 2017 TODO Verify the value and format of the date
 * @author codac TODO Verify the name and format of the author
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
 * the class FilterGAM (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "FilterGAM.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {
FilterGAM::FilterGAM() {
    num = NULL_PTR(float32 *);
    den = NULL_PTR(float32 *);
    numberOfNumCoeff = 0;
    numberOfDenCoeff = 0;
    lastInputs = NULL_PTR(float32 *);
    lastOutputs = NULL_PTR(float32 *);
    staticGain = 0;
}

FilterGAM::~FilterGAM() {
    if (num != NULL_PTR(float32 *)) {
        delete[] num;
    }
    if (den != NULL_PTR(float32 *)) {
        delete[] den;
    }
    if (lastInputs != NULL_PTR(float32 *)) {
        delete[] lastInputs;
    }
    if (lastOutputs != NULL_PTR(float32 *)) {
        delete[] lastOutputs;
    }
}

bool FilterGAM::Initialise(StructuredDataI& data) {
    AnyType functionsArray = data.GetType("Num");
    bool ret = true;
    bool ok = (functionsArray.GetDataPointer() != NULL);
    numberOfNumCoeff = 0u;
    if (ok) {
        numberOfNumCoeff = functionsArray.GetNumberOfElements(0u);
        ok = (numberOfNumCoeff > 0u);
    }
    if (ok) {
        num = new float32[numberOfNumCoeff];
        Vector<float32> numVector(num, numberOfNumCoeff);
        ok = (data.Read("Num", numVector));
        if (!ok) {
            REPORT_ERROR_PARAMETERS(ErrorManagement::InitialisationError, "Error reading numerator %s", GetName())
        }
    }
    else {
        REPORT_ERROR_PARAMETERS(ErrorManagement::InitialisationError, "No numerator defined %s", GetName())
    }
    ret &= ok;

    AnyType functionsArray1 = data.GetType("Den");
    //ok is reused in the denominator initialization.
    ok = (functionsArray1.GetDataPointer() != NULL);
    numberOfDenCoeff = 0u;
    if (ok) {
        numberOfDenCoeff = functionsArray1.GetNumberOfElements(0u);
        ok &= (numberOfDenCoeff > 0u);
    }
    if (ok) {
        den = new float32[numberOfDenCoeff];
        Vector<float32> denVector(den, numberOfDenCoeff);
        data.Read("Den", denVector);
        if (!ok) {
            REPORT_ERROR_PARAMETERS(ErrorManagement::InitialisationError, "Error reading denominator %s", GetName());
        }
        else {
            ok &= CheckNormalisation();
            if (!ok) {
                REPORT_ERROR_PARAMETERS(ErrorManagement::InitialisationError,
                                        "The coefficients of the filter must be normalised before being introduced into the GAM %s", GetName());
            }
        }
    }
    else {
        REPORT_ERROR_PARAMETERS(ErrorManagement::InitialisationError, "No denominator defined %s", GetName())
    }
    //Update the ret value
    ret &= ok;
    if (ret) {
        float32 sumNumerator = 0;
        for (uint32 i = 0; i < numberOfNumCoeff; i++) {
            sumNumerator += num[i];
        }
        float32 sumDenominator = 0;
        for (uint32 i = 0; i < numberOfDenCoeff; i++) {
            sumDenominator += den[i];
        }
        staticGain = sumNumerator / sumDenominator;
        //initialise lastInputs & lastOutputs
        lastInputs = new float32[numberOfNumCoeff - 1u];
        lastOutputs = new float32[numberOfDenCoeff - 1u];
        for (uint32 i = 0; i < numberOfDenCoeff - 1u; i++) {
            lastOutputs[i] = 0.0;
        }
        for (uint32 i = 0; i < numberOfNumCoeff - 1u; i++) {
            lastInputs[i] = 0.0;
        }
    }
    return ret;
}

bool FilterGAM::Setup() {
    return true;
}

bool FilterGAM::Execute() {
    /*
     int32 n = 0;
     float32 accumulator = 0;
     while (n < numberOfSamples) {
     accumulator = 0;
     //sum inputs
     for (int32 k = 0; k < numberOfNumCoeff; k++) {
     if (n >= k) {
     accumulator += input[n - k] * num[k];
     }
     else {
     accumulator += lastInputs[k - n - 1] * num[k];
     }
     }
     //Sum outputs
     for (int32 k = 1; k < numberOfDenCoeff; k++) {
     if (n >= k) {
     accumulator -= output[n - k] * den[k];
     }
     else {
     accumulator -= lastOutputs[k - n - 1] * den[k];
     }
     }
     output[n] = accumulator;
     n++;
     }

     //update the last values
     for (int32 k = 0; k < numberOfNumCoeff - 1; k++) {
     lastInputs[k] = input[numberOfSamples - 1 - k];
     }
     for (int32 k = 0; k < numberOfDenCoeff - 1; k++) {
     lastOutputs[k] = output[numberOfSamples - 1 - k];
     }
     */
    return true;
}

}

