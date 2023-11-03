/**
 * @file FilterGAMTestHelper.cpp
 * @brief Source file for class FilterGAMTestHelper
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

 * @details This source file contains the definition of all the methods for
 * the class FilterGAMTestHelper (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "FilterGAMTestHelper.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace {

}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

//FilterGAMTestHelper::FilterGAMTestHelper() {
//    numberOfElements = 0;
//    numberOfSamples = 0;
//    byteSize = 0;
//    numH = NULL;
//    denH = NULL;
//    isInitialised = false;
//}

FilterGAMTestHelper::FilterGAMTestHelper(uint32 elements,
                                         uint32 samples) {
    numberOfElements = elements;
    numberOfSamples = samples;
    if (numberOfElements > numberOfSamples) {
        byteSize = numberOfElements * sizeof(float32);
    }
    else {
        byteSize = numberOfSamples * sizeof(float32);
    }
    numH = NULL;
    denH = NULL;
    isInitialised = false;
}

FilterGAMTestHelper::~FilterGAMTestHelper() {

    if (numH != NULL) {
        delete[] numH;
    }
    if (denH != NULL) {
        delete[] denH;
    }

}

void* FilterGAMTestHelper::GetInputSignalsMemory() {
    return MARTe::GAM::GetInputSignalsMemory();
}
void* FilterGAMTestHelper::GetOutputSignalsMemory() {
    return MARTe::GAM::GetOutputSignalsMemory();
}
void* FilterGAMTestHelper::GetInputSignalsMemory(MARTe::uint32 idx) {
    return MARTe::GAM::GetInputSignalMemory(idx);
}

void* FilterGAMTestHelper::GetOutputSignalsMemory(MARTe::uint32 idx) {
    return MARTe::GAM::GetOutputSignalMemory(idx);
}

bool FilterGAMTestHelper::InitialiseNoFilter() {
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

bool FilterGAMTestHelper::InitialiseAVG10() {
    bool ret = true;
    if (isInitialised == false) {
        MARTe::uint32 nOfNumerators = 10;
        numH = new MARTe::float32[nOfNumerators];
        MARTe::float32 coeff = 1.0 / nOfNumerators;
        for (MARTe::uint32 i = 0u; i < nOfNumerators; i++) {
            numH[i] = coeff;
        }
        denH = new MARTe::float32[1];
        denH[0] = 1.0;
        MARTe::Vector<MARTe::float32> numVec(numH, nOfNumerators);
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

bool FilterGAMTestHelper::InitialiseFilterFIR2() {
    bool ret = true;
    if (isInitialised == false) {
        numH = new MARTe::float32[2];
        numH[0] = 0.6;
        numH[1] = 0.4;
        denH = new MARTe::float32[1];
        denH[0] = 1;
        MARTe::Vector<MARTe::float32> numVec(numH, 2);
        MARTe::Vector<MARTe::float32> denVec(denH, 1);
        ret &= config.Write("Num", numVec);
        ret &= config.Write("Den", denVec);
        bool resetInEachState = 0;
        ret &= config.Write("ResetInEachState", resetInEachState);
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
bool FilterGAMTestHelper::InitialiseFilterIIR() {
    bool ret = true;
    if (isInitialised == false) {
        numH = new MARTe::float32[1];
        numH[0] = 1;
        denH = new MARTe::float32[2];
        denH[0] = 1;
        denH[1] = -1;
        MARTe::Vector<MARTe::float32> numVec(numH, 1);
        MARTe::Vector<MARTe::float32> denVec(denH, 2);
        ret &= config.Write("Num", numVec);
        ret &= config.Write("Den", denVec);
        ret &= config.CreateRelative("InputSignals");
        ret &= config.CreateRelative("Input0");
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

bool FilterGAMTestHelper::InitialiseFilterIIR2() {
    bool ret = true;
    if (isInitialised == false) {
        numH = new MARTe::float32[3];
        numH[0] = 0.1;
        numH[1] = 0.2;
        numH[2] = 0.3;
        denH = new MARTe::float32[3];
        denH[0] = 1;
        denH[1] = 0.5;
        denH[2] = 0.2;
        MARTe::Vector<MARTe::float32> numVec(numH, 3);
        MARTe::Vector<MARTe::float32> denVec(denH, 3);
        ret &= config.Write("Num", numVec);
        ret &= config.Write("Den", denVec);
        bool resetInEachState = 0;
        ret &= config.Write("ResetInEachState", resetInEachState);
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

bool FilterGAMTestHelper::IsInitialised() {
    return isInitialised;
}

bool FilterGAMTestHelper::InitialiseConfigDataBaseSignal2() {
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

bool FilterGAMTestHelper::InitialiseFilterFIR() {
    bool ret = true;
    if (isInitialised == false) {
        numH = new float32[2];
        numH[0] = 0.5;
        numH[1] = 0.5;
        denH = new float32[1];
        denH[0] = 1;
        MARTe::Vector<float32> numVec(numH, 2);
        MARTe::Vector<float32> denVec(denH, 1);
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

bool FilterGAMTestHelper::InitialiseConfigDataBaseSignal1() {
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

bool FilterGAMTestHelper::InitialiseConfigDataBaseSignal3() {
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

bool FilterGAMTestHelper::InitialiseConfigDataBaseSignal4() {
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

StreamString FilterGAMTestHelper::GetTypeString(float32 var) {
    StreamString aux = "float32";
    return aux;
}
StreamString FilterGAMTestHelper::GetTypeString(float64 var) {
    StreamString aux = "float64";
    return aux;
}
StreamString FilterGAMTestHelper::GetTypeString(int32 var) {
    StreamString aux = "int32";
    return aux;
}
StreamString FilterGAMTestHelper::GetTypeString(int64 var) {
    StreamString aux = "int64";
    return aux;
}

}

