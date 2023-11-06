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

template<typename T>
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

    bool InitialiseFilterIntegrator();

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
    T *numH;
    T *denH;
    StreamString GetTypeString(float32 var);
    StreamString GetTypeString(float64 var);
    StreamString GetTypeString(int32 var);
    StreamString GetTypeString(int64 var);
private:

    bool isInitialised;
};

template<typename T>
FilterGAMTestHelper<T>::FilterGAMTestHelper(uint32 elements,
                                            uint32 samples) {
    numberOfElements = elements;
    numberOfSamples = samples;
    if (numberOfElements > numberOfSamples) {
        byteSize = numberOfElements * sizeof(T);
    }
    else {
        byteSize = numberOfSamples * sizeof(T);
    }
    numH = NULL;
    denH = NULL;
    isInitialised = false;
}
template<typename T>
FilterGAMTestHelper<T>::~FilterGAMTestHelper() {

    if (numH != NULL) {
        delete[] numH;
    }
    if (denH != NULL) {
        delete[] denH;
    }

}
template<typename T>
void* FilterGAMTestHelper<T>::GetInputSignalsMemory() {
    return MARTe::GAM::GetInputSignalsMemory();
}
template<typename T>
void* FilterGAMTestHelper<T>::GetOutputSignalsMemory() {
    return MARTe::GAM::GetOutputSignalsMemory();
}
template<typename T>
void* FilterGAMTestHelper<T>::GetInputSignalsMemory(MARTe::uint32 idx) {
    return MARTe::GAM::GetInputSignalMemory(idx);
}
template<typename T>
void* FilterGAMTestHelper<T>::GetOutputSignalsMemory(MARTe::uint32 idx) {
    return MARTe::GAM::GetOutputSignalMemory(idx);
}
template<typename T>
bool FilterGAMTestHelper<T>::InitialiseNoFilter() {
    bool ret = true;
    if (isInitialised == false) {
        numH = new MARTe::float32[1];
        numH[0] = 1.0;
        denH = new MARTe::float32[1];
        denH[0] = 1.0;
        MARTe::Vector<MARTe::float32> numVec(numH, 1);
        MARTe::Vector<MARTe::float32> denVec(denH, 1);
        ret &= config.Write("Num", numVec);
        ret &= config.Write("Den", denVec);
        ret &= config.Write("ResetInEachState", false);
        ret &= config.CreateRelative("InputSignals");
        ret &= config.CreateRelative("Input0");
        ret &= config.MoveToRoot();
        isInitialised = ret;
    }
    else {
        ret = false;
    }
    return ret;
}
template<typename T>
bool FilterGAMTestHelper<T>::InitialiseAVG10() {
    bool ret = true;
    if (isInitialised == false) {
        MARTe::uint32 nOfNumerators = 10;
        numH = new T[nOfNumerators];
        T coeff = 1.0;
        for (MARTe::uint32 i = 0u; i < nOfNumerators; i++) {
            numH[i] = coeff;
        }
        denH = new T[1];
        denH[0] = 1.0;
        MARTe::Vector<T> numVec(numH, nOfNumerators);
        MARTe::Vector<T> denVec(denH, 1);
        ret &= config.Write("Num", numVec);
        ret &= config.Write("Den", denVec);
        ret &= config.Write("ResetInEachState", false);
        ret &= config.CreateRelative("InputSignals");
        ret &= config.CreateRelative("Input0");
        T auxVar = 0;
        StreamString auxTypeStr = GetTypeString(auxVar);
        ret &= config.Write("Type", auxTypeStr.Buffer());
        ret &= config.MoveToRoot();
        isInitialised = ret;
    }
    else {
        ret = false;
    }
    return ret;
}
template<typename T>
bool FilterGAMTestHelper<T>::InitialiseFilterFIR2() {
    bool ret = true;
    if (isInitialised == false) {
        numH = new T[2];
        numH[0] = 6;
        numH[1] = 4;
        denH = new T[1];
        denH[0] = 1;
        MARTe::Vector<T> numVec(numH, 2);
        MARTe::Vector<T> denVec(denH, 1);
        ret &= config.Write("Num", numVec);
        ret &= config.Write("Den", denVec);
        bool resetInEachState = 0;
        ret &= config.Write("ResetInEachState", resetInEachState);
        ret &= config.CreateRelative("InputSignals");
        ret &= config.CreateRelative("Input0");
        T auxVar = 0;
        StreamString auxTypeStr = GetTypeString(auxVar);
        ret &= config.Write("Type", auxTypeStr.Buffer());
        ret &= config.MoveToRoot();
        isInitialised = ret;
    }
    else {
        ret = false;
    }
    return ret;
}
template<typename T>
bool FilterGAMTestHelper<T>::InitialiseFilterIIR() {
    bool ret = true;
    if (isInitialised == false) {
        numH = new T[1];
        numH[0] = 1;
        denH = new T[2];
        denH[0] = 1;
        denH[1] = -1;
        MARTe::Vector<T> numVec(numH, 1);
        MARTe::Vector<T> denVec(denH, 2);
        ret &= config.Write("Num", numVec);
        ret &= config.Write("Den", denVec);
        ret &= config.CreateRelative("InputSignals");
        ret &= config.CreateRelative("Input0");
        T auxVar = 0;
        StreamString auxTypeStr = GetTypeString(auxVar);
        ret &= config.Write("Type", auxTypeStr.Buffer());
        ret &= config.MoveToRoot();
        bool resetInEachState = 0;
        ret &= config.Write("ResetInEachState", resetInEachState);
        isInitialised = ret;
    }
    else {
        ret = false;
    }
    return ret;
}
template<typename T>
bool FilterGAMTestHelper<T>::InitialiseFilterIIR2() {
    bool ret = true;
    if (isInitialised == false) {
        numH = new T[3];
        numH[0] = 1;
        numH[1] = 2;
        numH[2] = 3;
        denH = new T[3];
        denH[0] = 1;
        denH[1] = 5;
        denH[2] = 2;
        MARTe::Vector<T> numVec(numH, 3);
        MARTe::Vector<T> denVec(denH, 3);
        ret &= config.Write("Num", numVec);
        ret &= config.Write("Den", denVec);
        bool resetInEachState = 0;
        ret &= config.Write("ResetInEachState", resetInEachState);
        ret &= config.CreateRelative("InputSignals");
        ret &= config.CreateRelative("Input0");
        T auxVar = 0;
        StreamString auxTypeStr = GetTypeString(auxVar);
        ret &= config.Write("Type", auxTypeStr.Buffer());
        ret &= config.MoveToRoot();
        isInitialised = ret;
    }
    else {
        ret = false;
    }
    return ret;
}

template<typename T>
bool FilterGAMTestHelper<T>::InitialiseFilterIntegrator() {
    bool ret = true;
    if (isInitialised == false) {
        numH = new T[1];
        numH[0] = 1;
        denH = new T[2];
        denH[0] = 1;
        denH[1] = -1;
        Vector<T> numVec(numH, 1);
        Vector<T> denVec(denH, 2);
        bool ok = config.Write("Num", numVec);
        ok = config.Write("Den", denVec);
        ok &= config.Write("ResetInEachState", false);
        ok &= config.CreateRelative("InputSignals");
        ok &= config.CreateRelative("Input0");
        T auxVar = 0;
        StreamString auxTypeStr = GetTypeString(auxVar);
        ret &= config.Write("Type", auxTypeStr.Buffer());
        ok &= config.MoveToRoot();
    }
    else {
        ret = false;
    }
    return ret;
}

template<typename T>
bool FilterGAMTestHelper<T>::IsInitialised() {
    return isInitialised;
}
template<typename T>
bool FilterGAMTestHelper<T>::InitialiseConfigDataBaseSignal2() {
    using namespace MARTe;
    bool ok = true;

    uint32 numberOfSignals = 2;
    uint32 totalByteSize = byteSize * numberOfSignals;
    ok &= configSignals.CreateAbsolute("Signals.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("QualifiedName", "InputSignal1");
    ok &= configSignals.Write("DataSource", "TestDataSource");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.Write("NumberOfElements", numberOfElements);
    ok &= configSignals.Write("ByteSize", byteSize);

    ok &= configSignals.MoveAbsolute("Signals.InputSignals");
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("QualifiedName", "InputSignal2");
    ok &= configSignals.Write("DataSource", "TestDataSource");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.Write("NumberOfElements", numberOfElements);
    ok &= configSignals.Write("ByteSize", byteSize);

    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", totalByteSize);

    ok &= configSignals.MoveToRoot();
    ok &= configSignals.CreateAbsolute("Signals.OutputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("QualifiedName", "OutputSignal1");
    ok &= configSignals.Write("DataSource", "TestDataSource");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.Write("NumberOfDimensions", 1);
    //The Filter GAM only support numberOfSamples 1 and numberOfElements>=1
    if (numberOfSamples > numberOfElements) {
        ok &= configSignals.Write("NumberOfElements", numberOfSamples);
    }
    else {
        ok &= configSignals.Write("NumberOfElements", numberOfElements);
    }
    ok &= configSignals.Write("ByteSize", byteSize);

    ok &= configSignals.MoveAbsolute("Signals.OutputSignals");
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("QualifiedName", "OutputSignal2");
    ok &= configSignals.Write("DataSource", "TestDataSource");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.Write("NumberOfDimensions", 1);
    //The Filter GAM only support numberOfSamples 1 and numberOfElements>=1
    if (numberOfSamples > numberOfElements) {
        ok &= configSignals.Write("NumberOfElements", numberOfSamples);
    }
    else {
        ok &= configSignals.Write("NumberOfElements", numberOfElements);
    }
    ok &= configSignals.Write("ByteSize", byteSize);

    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", totalByteSize);

    ok &= configSignals.MoveToRoot();

    ok &= configSignals.CreateAbsolute("Memory.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("DataSource", "TestDataSource");
    ok &= configSignals.CreateRelative("Signals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Samples", numberOfSamples);

    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("Samples", numberOfSamples);

    ok &= configSignals.CreateAbsolute("Memory.OutputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("DataSource", "TestDataSource");
    ok &= configSignals.CreateRelative("Signals");
    ok &= configSignals.CreateRelative("0");
    //The Filter GAM only support numberOfSamples 1 and numberOfElements>=1
    ok &= configSignals.Write("Samples", 1);

    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("Samples", 1);

    ok &= configSignals.MoveToRoot();
    return ok;
}

template<typename T>
bool FilterGAMTestHelper<T>::InitialiseConfigDataBaseSignal3() {
    bool ok = true;
    MARTe::uint32 totalByteSize = byteSize;
    ok &= configSignals.CreateAbsolute("Signals.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("QualifiedName", "InputSignal1");
    ok &= configSignals.Write("DataSource", "TestDataSource");
    ok &= configSignals.Write("Type", "uint32");
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.Write("NumberOfElements", numberOfElements);
    ok &= configSignals.Write("ByteSize", byteSize);

    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", totalByteSize);

    ok &= configSignals.MoveToRoot();
    ok &= configSignals.CreateAbsolute("Signals.OutputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("QualifiedName", "OutputSignal1");
    ok &= configSignals.Write("DataSource", "TestDataSource");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.Write("NumberOfDimensions", 1);
    if (numberOfSamples > numberOfElements) {
        ok &= configSignals.Write("NumberOfElements", numberOfSamples);
    }
    else {
        ok &= configSignals.Write("NumberOfElements", numberOfElements);
    }
    ok &= configSignals.Write("ByteSize", byteSize);

    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", totalByteSize);

    ok &= configSignals.MoveToRoot();

    ok &= configSignals.CreateAbsolute("Memory.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("DataSource", "TestDataSource");
    ok &= configSignals.CreateRelative("Signals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Samples", numberOfSamples);

    ok &= configSignals.CreateAbsolute("Memory.OutputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("DataSource", "TestDataSource");
    ok &= configSignals.CreateRelative("Signals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Samples", 1);

    ok &= configSignals.MoveToRoot();
    return ok;
}
template<typename T>
bool FilterGAMTestHelper<T>::InitialiseConfigDataBaseSignal4() {
    bool ok = true;
    MARTe::uint32 totalByteSize = byteSize;
    ok &= configSignals.CreateAbsolute("Signals.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("QualifiedName", "InputSignal1");
    ok &= configSignals.Write("DataSource", "TestDataSource");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.Write("NumberOfElements", numberOfElements);
    ok &= configSignals.Write("ByteSize", byteSize);

    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", totalByteSize);

    ok &= configSignals.MoveToRoot();
    ok &= configSignals.CreateAbsolute("Signals.OutputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("QualifiedName", "OutputSignal1");
    ok &= configSignals.Write("DataSource", "TestDataSource");
    ok &= configSignals.Write("Type", "uint32");
    ok &= configSignals.Write("NumberOfDimensions", 1);
    if (numberOfSamples > numberOfElements) {
        ok &= configSignals.Write("NumberOfElements", numberOfSamples);
    }
    else {
        ok &= configSignals.Write("NumberOfElements", numberOfElements);
    }
    ok &= configSignals.Write("ByteSize", byteSize);

    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", totalByteSize);

    ok &= configSignals.MoveToRoot();

    ok &= configSignals.CreateAbsolute("Memory.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("DataSource", "TestDataSource");
    ok &= configSignals.CreateRelative("Signals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Samples", numberOfSamples);

    ok &= configSignals.CreateAbsolute("Memory.OutputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("DataSource", "TestDataSource");
    ok &= configSignals.CreateRelative("Signals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Samples", 1);

    ok &= configSignals.MoveToRoot();
    return ok;
}
template<typename T>
StreamString FilterGAMTestHelper<T>::GetTypeString(float32 var) {
    StreamString aux = "float32";
    return aux;
}
template<typename T>
StreamString FilterGAMTestHelper<T>::GetTypeString(float64 var) {
    StreamString aux = "float64";
    return aux;
}
template<typename T>
StreamString FilterGAMTestHelper<T>::GetTypeString(int32 var) {
    StreamString aux = "int32";
    return aux;
}
template<typename T>
StreamString FilterGAMTestHelper<T>::GetTypeString(int64 var) {
    StreamString aux = "int64";
    return aux;
}

template<typename T>
bool FilterGAMTestHelper<T>::InitialiseFilterFIR() {
    bool ret = true;
    if (isInitialised == false) {
        numH = new T[2];
        numH[0] = 1.0;
        numH[1] = 1.0;
        denH = new T[1];
        denH[0] = 1;
        MARTe::Vector<T> numVec(numH, 2);
        MARTe::Vector<T> denVec(denH, 1);
        ret &= config.Write("Num", numVec);
        ret &= config.Write("Den", denVec);
        ret &= config.Write("ResetInEachState", false);
        ret &= config.CreateRelative("InputSignals");
        ret &= config.CreateRelative("Input0");
        T auxVar = 0;
        StreamString auxTypeStr = GetTypeString(auxVar);
        ret &= config.Write("Type", auxTypeStr.Buffer());
        ret &= config.MoveToRoot();
        isInitialised = ret;
    }
    else {
        ret = false;
    }
    return ret;
}
template<typename T>
bool FilterGAMTestHelper<T>::InitialiseConfigDataBaseSignal1() {
    bool ok = true;
    MARTe::uint32 totalByteSize = byteSize;
    ok &= configSignals.CreateAbsolute("Signals.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("QualifiedName", "InputSignal1");
    ok &= configSignals.Write("DataSource", "TestDataSource");
    T auxVar = 0;
    StreamString typeString = GetTypeString(auxVar);
    ok &= configSignals.Write("Type", typeString.Buffer());
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.Write("NumberOfElements", numberOfElements);
    ok &= configSignals.Write("ByteSize", byteSize);

    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", totalByteSize);

    ok &= configSignals.MoveToRoot();
    ok &= configSignals.CreateAbsolute("Signals.OutputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("QualifiedName", "OutputSignal1");
    ok &= configSignals.Write("DataSource", "TestDataSource");
    ok &= configSignals.Write("Type", typeString.Buffer());
    ok &= configSignals.Write("NumberOfDimensions", 1);
    if (numberOfSamples > numberOfElements) {
        ok &= configSignals.Write("NumberOfElements", numberOfSamples);
    }
    else {
        ok &= configSignals.Write("NumberOfElements", numberOfElements);
    }
    ok &= configSignals.Write("ByteSize", byteSize);

    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", totalByteSize);

    ok &= configSignals.MoveToRoot();

    ok &= configSignals.CreateAbsolute("Memory.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("DataSource", "TestDataSource");
    ok &= configSignals.CreateRelative("Signals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Samples", numberOfSamples);

    ok &= configSignals.CreateAbsolute("Memory.OutputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("DataSource", "TestDataSource");
    ok &= configSignals.CreateRelative("Signals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Samples", 1);

    ok &= configSignals.MoveToRoot();
    return ok;
}
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* TEST_COMPONENTS_GAMS_FILTERGAM_FILTERGAMTESTHELPER_H_ */

