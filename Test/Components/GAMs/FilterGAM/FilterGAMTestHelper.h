/**
 * @file FilterGAMTestHelper.h
 * @brief Header file for class FilterGAMTestHelper
 * @date 03/11/2023
 * @author Llorenc Capella
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

 * @details This header file contains the declaration of the class FilterGAMTestHelper
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef FILTERGAMTESTHELPER_H_
#define FILTERGAMTESTHELPER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "FilterGAM.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

class FilterGAMTestHelper: public FilterGAM {
//TODO Add the macro DLL_API to the class declaration (i.e. class DLL_API FilterGAMTestHelper)
public:
//    FilterGAMTestHelper();
    FilterGAMTestHelper(uint32 elements = 10,
                        uint32 samples = 1);
    virtual ~FilterGAMTestHelper();
    void* GetInputSignalsMemory();
    void* GetOutputSignalsMemory();
    void* GetInputSignalsMemory(MARTe::uint32 idx);
    void* GetOutputSignalsMemory(MARTe::uint32 idx);
    bool InitialiseNoFilter();

    bool InitialiseFilterFIR();

    bool InitialiseAVG10();

    bool InitialiseFilterFIR2();
    bool InitialiseFilterIIR();

    bool InitialiseFilterIIR2();

    bool IsInitialised();
    bool InitialiseConfigDataBaseSignal1();

    bool InitialiseConfigDataBaseSignal2();

    bool InitialiseConfigDataBaseSignal3();
    bool InitialiseConfigDataBaseSignal4();

    MARTe::uint32 numberOfElements;
    MARTe::uint32 numberOfSamples;
    MARTe::uint32 byteSize;
    MARTe::ConfigurationDatabase configSignals;
    MARTe::ConfigurationDatabase config;
    MARTe::float32 *numH;
    MARTe::float32 *denH;
private:
    StreamString GetTypeString(float32 var);
    StreamString GetTypeString(float64 var);
    StreamString GetTypeString(int32 var);
    StreamString GetTypeString(int64 var);
    bool isInitialised;
};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* TEST_COMPONENTS_GAMS_FILTERGAM_FILTERGAMTESTHELPER_H_ */

