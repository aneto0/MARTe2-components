/**
 * @file FilterGAMTest.cpp
 * @brief Source file for class FilterGAMTest
 * @date 30/01/2017
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
 * the class FilterGAMTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include "math.h"
/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ConfigurationDatabase.h"
#include "FilterGAM.h"
#include "FilterGAMTest.h"
#include "Vector.h"
#include "stdio.h"
#include "FilterGAMTestHelper.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {
FilterGAMTest::FilterGAMTest() {
}

FilterGAMTest::~FilterGAMTest() {
}

bool FilterGAMTest::TestInitialise_GAMInitialiseFail() {
    FilterGAM gam;
    ConfigurationDatabase cfg;
    cfg.CreateRelative("+LLCN");
    cfg.MoveToRoot();
    bool ok = !gam.Initialise(cfg);
    return ok;
}

bool FilterGAMTest::TestInitialise_ZeroInputSignals() {
    FilterGAM gam;
    ConfigurationDatabase cfg;
    cfg.CreateRelative("InputSignals");
    cfg.MoveToRoot();
    bool ok = !gam.Initialise(cfg);
    return ok;
}

bool FilterGAMTest::TestInitialise_UnsupportedInputType() {
    FilterGAM gam;
    ConfigurationDatabase cfg;
    cfg.CreateAbsolute("InputSignals.Input0");
    cfg.Write("Type", "InvalidType");
    cfg.MoveToRoot();
    bool ok = !gam.Initialise(cfg);
    return ok;
}

bool FilterGAMTest::TestInitialise_WrongFilterCoeff() {
    using namespace MARTe;
    FilterGAM gam;
    gam.SetName("Test");
    bool ok = true;
    ConfigurationDatabase config;
    float32 *num = new float32[2];
    float32 *den = new float32[1];
    num[0] = 1;
    num[1] = 2;
    den[0] = 0;
    Vector<float32> numVec(num, 2);
    Vector<float32> denVec(den, 1);
    ok &= config.Write("Num", numVec);
    ok &= config.Write("Den", denVec);
    ok &= config.CreateRelative("InputSignals");
    ok &= config.CreateRelative("Input0");
    ok &= config.MoveToRoot();
    ok &= !gam.Initialise(config);
    delete[] num;
    delete[] den;
    return ok;
}

bool FilterGAMTest::TestInitialise_WrongRestInEachState() {
    using namespace MARTe;
    FilterGAM gam;
    gam.SetName("Test");
    bool ok = true;
    ConfigurationDatabase config;
    float32 *num = new float32[2];
    float32 *den = new float32[1];
    num[0] = 1;
    num[1] = 2;
    den[0] = 1;
    Vector<float32> numVec(num, 2);
    Vector<float32> denVec(den, 1);
    ok &= config.Write("ResetInEachState", 2);
    ok &= config.Write("Num", numVec);
    ok &= config.Write("Den", denVec);
    ok &= config.CreateRelative("InputSignals");
    ok &= config.CreateRelative("Input0");
    ok &= config.MoveToRoot();
    ok &= !gam.Initialise(config);
    delete[] num;
    delete[] den;
    return ok;
}

bool FilterGAMTest::TestInitialise_CheckNormalisationBeforeInitialise() {
    using namespace MARTe;
    FilterGAMTestHelper<float32> gam;
    gam.SetName("Test");
    bool ok = !gam.CheckNormalisation();
    return ok;
}

bool FilterGAMTest::TestInitialise_NoNum() {
    FilterGAM gam;
    ConfigurationDatabase cfg;
    cfg.CreateAbsolute("InputSignals.Input0");
    cfg.Write("Type", "float64");
    cfg.MoveToRoot();
    bool ok = !gam.Initialise(cfg);
    return ok;
}

bool FilterGAMTest::TestConstructor() {
    using namespace MARTe;
    FilterGAM gam;
    uint32 sizeMem = 3u;
    float32 *num = new float32[sizeMem];
    float32 *den = new float32[sizeMem];
    bool ok = true;

    for (uint32 i = 0; i < sizeMem; i++) {
        num[i] = 0;
        den[i] = 0;
    }
//try to get the num and denominator vales of the filter
    ok &= !gam.GetNumCoeff(num);
    ok &= !gam.GetDenCoeff(den);
//Check that the values are not changed
    for (uint32 i = 0; i < sizeMem; i++) {
        ok &= (num[i] == 0);
        ok &= (den[i] == 0);
    }
    bool isInfinite;
//Check that the static gain is 0
    float32 gainReturn;
    gam.GetStaticGain(isInfinite, gainReturn);
    ok &= 0.0 == gainReturn;
    ok &= !isInfinite;
//Check that the number of numerator coefficients is 0
    ok &= (0 == gam.GetNumberOfNumCoeff());
// Idem denominator
    ok &= (0 == gam.GetNumberOfDenCoeff());
//Check normalisation
    ok &= !gam.CheckNormalisation();
    ok &= (0 == gam.GetNumberOfSamples());
    ok &= (0 == gam.GetNumberOfSignals());
//The constructor initialise resetInEachState = true
    ok &= gam.GetResetInEachState();
    delete[] num;
    delete[] den;
    return ok;
}

bool FilterGAMTest::TestInitialiseNoResetInEachState() {
    using namespace MARTe;
    FilterGAM gam;
    gam.SetName("Test");
    bool ok = true;
    ConfigurationDatabase config;
    float32 *num = new float32[2];
    float32 *den = new float32[1];
    den[0] = 1;
    Vector<float32> numVec(num, 2);
    Vector<float32> denVec(den, 1);
    ok &= config.Write("Num", numVec);
    ok &= config.Write("Den", denVec);
    ok &= config.CreateRelative("InputSignals");
    ok &= config.CreateRelative("Input0");
    ok &= config.MoveToRoot();
    ok &= gam.Initialise(config);
    ok &= gam.GetResetInEachState();
    delete[] num;
    delete[] den;
    return ok;
}

bool FilterGAMTest::TestInitialiseWrongResetInEachState() {
    using namespace MARTe;
    FilterGAM gam;
    gam.SetName("Test");
    bool ok = true;
    ConfigurationDatabase config;
    float32 *num = new float32[2];
    float32 *den = new float32[1];
    den[0] = 1;
    Vector<float32> numVec(num, 2);
    Vector<float32> denVec(den, 1);
    ok &= config.Write("Num", numVec);
    ok &= config.Write("Den", denVec);
    ok &= config.Write("ResetInEachState", 3);
    ok &= !gam.Initialise(config);
    delete[] num;
    delete[] den;
    return ok;
}

bool FilterGAMTest::TestInitialiseNoNum() {
    using namespace MARTe;
    FilterGAM gam;
    gam.SetName("Test");
    bool ok = true;
    ConfigurationDatabase config;
    float32 *num = new float32[2];
    float32 *den = new float32[1];
    den[0] = 1;
    Vector<float32> numVec(num, 2);
    Vector<float32> denVec(den, 1);
    ok &= config.Write("otherNames", numVec);
    ok &= config.Write("Den", denVec);
    ok &= config.CreateAbsolute("InputSignals.Input0");
    ok &= config.MoveToRoot();
    ok &= gam.Initialise(config);
    delete[] num;
    delete[] den;
    return !ok;
}

bool FilterGAMTest::TestInitialiseWrongNumType() {
    using namespace MARTe;
    FilterGAM gam;
    gam.SetName("Test");
    bool ok = true;
    ConfigurationDatabase config;
    float32 *num = new float32[2];
    float32 *den = new float32[1];
    den[0] = 1;
    Vector<float32> numVec(num, 0);
    Vector<float32> denVec(den, 1);
    ok &= config.Write("Num", ok);
    ok &= config.Write("Den", denVec);
    ok &= config.CreateAbsolute("InputSignals.Input0");
    ok &= config.MoveToRoot();
    ok &= gam.Initialise(config);
    delete[] num;
    delete[] den;
    return !ok;
}

bool FilterGAMTest::TestInitialiseNoDen() {
    using namespace MARTe;
    FilterGAM gam;
    gam.SetName("Test");
    ConfigurationDatabase config;
    uint32 numberOfNumCoeff = 2;
    float32 *num = new float32[numberOfNumCoeff];
    num[0] = 0.5;
    num[1] = 0.5;
    Vector<float32> numVec(num, numberOfNumCoeff);
    bool ok = config.Write("Num", numVec);
    ok &= config.CreateAbsolute("InputSignals.Input0");
    ok &= config.MoveToRoot();
    ok &= gam.Initialise(config);
    ok &= (0 == gam.GetNumberOfDenCoeff());
    delete[] num;
    return !ok;
}

bool FilterGAMTest::TestInitialiseWrongDenType() {
    using namespace MARTe;
    FilterGAM gam;
    gam.SetName("Test");
    bool ok = true;
    ConfigurationDatabase config;
    float32 *num = new float32[2];
    float32 *den = new float32[1];
    den[0] = 1;
    Vector<float32> numVec(num, 2);
    Vector<float32> denVec(den, 1);
    ok &= config.Write("Num", numVec);
    ok &= config.Write("Den", ok);
    ok &= gam.Initialise(config);
    delete[] num;
    delete[] den;
    return !ok;
}

bool FilterGAMTest::TestFailNormalise() {
    using namespace MARTe;
    FilterGAM gam;
    gam.SetName("Test");
    bool ok = true;
    ConfigurationDatabase config;
    uint32 numberOfNumCoeff = 2;
    uint32 numberOfDenCoeff = 1;
    float32 *num = new float32[numberOfNumCoeff];
    float32 *den = new float32[numberOfDenCoeff];
    num[0] = 0.5;
    num[1] = 0.5;
    den[0] = 2;
    Vector<float32> numVec(num, numberOfNumCoeff);
    Vector<float32> denVec(den, numberOfDenCoeff);
    ok &= config.Write("Num", numVec);
    ok &= config.Write("Den", denVec);
    ok &= config.CreateAbsolute("InputSignals.Input0");
    ok &= config.MoveToRoot();
    ok &= !gam.Initialise(config);
    ok &= !gam.CheckNormalisation();
    delete[] num;
    delete[] den;
    return ok;
}

bool FilterGAMTest::TestSetupNoInputSignal() {
    using namespace MARTe;
    FilterGAM gam;
    gam.SetName("Test");
    ConfigurationDatabase config;
    float32 *num = new float32[2];
    num[0] = 0.5;
    num[1] = 0.5;
    float32 *den = new float32[1];
    den[0] = 1;
    Vector<float32> numVec(num, 2);
    Vector<float32> denVec(den, 1);
    bool ok = config.Write("Num", numVec);
    ok &= config.Write("Den", denVec);
    ok &= config.Write("ResetInEachState", false);
    ok &= config.CreateRelative("InputSignals");
    ok &= config.CreateRelative("Input0");
    ok &= config.MoveToRoot();
    ok &= gam.Initialise(config);
//    uint32 numberOfSamples = 1;
    uint32 numberOfElements = 10;
    uint32 byteSize = numberOfElements * sizeof(float32);
    uint32 totalByteSize = byteSize;
    ConfigurationDatabase configSignals;
    /*
     configSignals.CreateAbsolute("Signals.InputSignals");
     configSignals.CreateRelative("0");
     configSignals.Write("QualifiedName", "InputSignal1");
     configSignals.Write("DataSource", "TestDataSource");
     configSignals.Write("Type", "float32");
     configSignals.Write("NumberOfDimensions", 1);
     configSignals.Write("NumberOfElements", numberOfElements);
     configSignals.Write("ByteSize", byteSize);
     */
    configSignals.MoveToAncestor(1u);
    configSignals.Write("ByteSize", totalByteSize);

    configSignals.MoveToRoot();
    configSignals.CreateAbsolute("Signals.OutputSignals");
    configSignals.CreateRelative("0");
    configSignals.Write("QualifiedName", "OutputSignal1");
    configSignals.Write("DataSource", "TestDataSource");
    configSignals.Write("Type", "float32");
    configSignals.Write("NumberOfDimensions", 1);
    configSignals.Write("NumberOfElements", numberOfElements);
    configSignals.Write("ByteSize", byteSize);

    configSignals.MoveToAncestor(1u);
    configSignals.Write("ByteSize", totalByteSize);
    /*
     configSignals.MoveToRoot();
     configSignals.CreateAbsolute("Memory.InputSignals");
     configSignals.CreateRelative("0");
     configSignals.Write("DataSource", "TestDataSource");
     configSignals.CreateRelative("Signals");
     configSignals.CreateRelative("0");
     configSignals.Write("Samples", 1);
     */
    configSignals.CreateAbsolute("Memory.OutputSignals");
    configSignals.CreateRelative("0");
    configSignals.Write("DataSource", "TestDataSource");
    configSignals.CreateRelative("Signals");
    configSignals.CreateRelative("0");
    configSignals.Write("Samples", 1);

    configSignals.MoveToRoot();

    ok &= gam.SetConfiguredDatabase(configSignals);
    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();

    ok &= !gam.Setup();
    delete[] num;
    delete[] den;
    return ok;
}

bool FilterGAMTest::TestSetupNoOutputSignal() {
    using namespace MARTe;
    FilterGAM gam;
    gam.SetName("Test");
    ConfigurationDatabase config;
    float32 *num = new float32[2];
    num[0] = 0.5;
    num[1] = 0.5;
    float32 *den = new float32[1];
    den[0] = 1;
    Vector<float32> numVec(num, 2);
    Vector<float32> denVec(den, 1);
    bool ok = config.Write("Num", numVec);
    ok &= config.Write("Den", denVec);
    ok &= config.Write("ResetInEachState", false);
    ok &= config.CreateRelative("InputSignals");
    ok &= config.CreateRelative("Input0");
    ok &= config.MoveToRoot();
    ok &= gam.Initialise(config);
    ok &= config.Write("ResetInEachState", false);
//    uint32 numberOfSamples = 1;
    uint32 numberOfElements = 10;
    uint32 byteSize = numberOfElements * sizeof(float32);
    uint32 totalByteSize = byteSize;
    ConfigurationDatabase configSignals;
    configSignals.CreateAbsolute("Signals.InputSignals");
    configSignals.CreateRelative("0");
    configSignals.Write("QualifiedName", "InputSignal1");
    configSignals.Write("DataSource", "TestDataSource");
    configSignals.Write("Type", "float32");
    configSignals.Write("NumberOfDimensions", 1);
    configSignals.Write("NumberOfElements", numberOfElements);
    configSignals.Write("ByteSize", byteSize);

    configSignals.MoveToAncestor(1u);
    configSignals.Write("ByteSize", totalByteSize);
    /*
     configSignals.MoveToRoot();
     configSignals.CreateAbsolute("Signals.OutputSignals");
     configSignals.CreateRelative("0");
     configSignals.Write("QualifiedName", "OutputSignal1");
     configSignals.Write("DataSource", "TestDataSource");
     configSignals.Write("Type", "float32");
     configSignals.Write("NumberOfDimensions", 1);
     configSignals.Write("NumberOfElements", numberOfElements);
     configSignals.Write("ByteSize", byteSize);

     configSignals.MoveToAncestor(1u);
     configSignals.Write("ByteSize", totalByteSize);
     */

    configSignals.MoveToRoot();

    configSignals.CreateAbsolute("Memory.InputSignals");
    configSignals.CreateRelative("0");
    configSignals.Write("DataSource", "TestDataSource");
    configSignals.CreateRelative("Signals");
    configSignals.CreateRelative("0");
    configSignals.Write("Samples", 1);
    /*
     configSignals.CreateAbsolute("Memory.OutputSignals");
     configSignals.CreateRelative("0");
     configSignals.Write("DataSource", "TestDataSource");
     configSignals.CreateRelative("Signals");
     configSignals.CreateRelative("0");
     configSignals.Write("Samples", 1);
     */
    configSignals.MoveToRoot();

    ok &= gam.SetConfiguredDatabase(configSignals);
    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();

    ok &= !gam.Setup();
    delete[] num;
    delete[] den;
    return ok;
}

bool FilterGAMTest::TestSetupNoInputSamples() {
    using namespace MARTe;
    FilterGAM gam;
    gam.SetName("Test");
    ConfigurationDatabase config;
    float32 *num = new float32[2];
    num[0] = 0.5;
    num[1] = 0.5;
    float32 *den = new float32[1];
    den[0] = 1;
    Vector<float32> numVec(num, 2);
    Vector<float32> denVec(den, 1);
    bool ok = config.Write("Num", numVec);
    ok &= config.Write("Den", denVec);
    ok &= config.Write("ResetInEachState", false);
    ok &= config.CreateRelative("InputSignals");
    ok &= config.CreateRelative("Input0");
    ok &= config.MoveToRoot();
    ok &= gam.Initialise(config);
//    uint32 numberOfSamples = 1;
    uint32 numberOfElements = 10;
    uint32 byteSize = numberOfElements * sizeof(float32);
    uint32 totalByteSize = byteSize;
    ConfigurationDatabase configSignals;
    configSignals.CreateAbsolute("Signals.InputSignals");
    configSignals.CreateRelative("0");
    configSignals.Write("QualifiedName", "InputSignal1");
    configSignals.Write("DataSource", "TestDataSource");
    configSignals.Write("Type", "float32");
    configSignals.Write("NumberOfDimensions", 1);
    configSignals.Write("NumberOfElements", numberOfElements);
    configSignals.Write("ByteSize", byteSize);

    configSignals.MoveToAncestor(1u);
    configSignals.Write("ByteSize", totalByteSize);

    configSignals.MoveToRoot();
    configSignals.CreateAbsolute("Signals.OutputSignals");
    configSignals.CreateRelative("0");
    configSignals.Write("QualifiedName", "OutputSignal1");
    configSignals.Write("DataSource", "TestDataSource");
    configSignals.Write("Type", "float32");
    configSignals.Write("NumberOfDimensions", 1);
    configSignals.Write("NumberOfElements", numberOfElements);
    configSignals.Write("ByteSize", byteSize);

    configSignals.MoveToAncestor(1u);
    configSignals.Write("ByteSize", totalByteSize);

    configSignals.MoveToRoot();
    /*
     configSignals.CreateAbsolute("Memory.InputSignals");
     configSignals.CreateRelative("0");
     configSignals.Write("DataSource", "TestDataSource");
     configSignals.CreateRelative("Signals");
     configSignals.CreateRelative("0");
     configSignals.Write("Samples", 1);

     configSignals.CreateAbsolute("Memory.OutputSignals");
     configSignals.CreateRelative("0");
     configSignals.Write("DataSource", "TestDataSource");
     configSignals.CreateRelative("Signals");
     configSignals.CreateRelative("0");
     configSignals.Write("Samples", 1);
     */
    configSignals.MoveToRoot();

    ok &= gam.SetConfiguredDatabase(configSignals);
    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();

    ok &= !gam.Setup();
    delete[] num;
    delete[] den;
    return ok;
}

bool FilterGAMTest::TestSetupNoOutputSamples() {
    using namespace MARTe;
    FilterGAM gam;
    gam.SetName("Test");
    ConfigurationDatabase config;
    float32 *num = new float32[2];
    num[0] = 0.5;
    num[1] = 0.5;
    float32 *den = new float32[1];
    den[0] = 1;
    Vector<float32> numVec(num, 2);
    Vector<float32> denVec(den, 1);
    bool ok = config.Write("Num", numVec);
    ok &= config.Write("Den", denVec);
    ok &= config.Write("ResetInEachState", false);
    ok &= config.CreateRelative("InputSignals");
    ok &= config.CreateRelative("Input0");
    ok &= config.MoveToRoot();
    ok &= gam.Initialise(config);
//    uint32 numberOfSamples = 1;
    uint32 numberOfElements = 10;
    uint32 byteSize = numberOfElements * sizeof(float32);
    uint32 totalByteSize = byteSize;
    ConfigurationDatabase configSignals;
    configSignals.CreateAbsolute("Signals.InputSignals");
    configSignals.CreateRelative("0");
    configSignals.Write("QualifiedName", "InputSignal1");
    configSignals.Write("DataSource", "TestDataSource");
    configSignals.Write("Type", "float32");
    configSignals.Write("NumberOfDimensions", 1);
    configSignals.Write("NumberOfElements", numberOfElements);
    configSignals.Write("ByteSize", byteSize);

    configSignals.MoveToAncestor(1u);
    configSignals.Write("ByteSize", totalByteSize);

    configSignals.MoveToRoot();
    configSignals.CreateAbsolute("Signals.OutputSignals");
    configSignals.CreateRelative("0");
    configSignals.Write("QualifiedName", "OutputSignal1");
    configSignals.Write("DataSource", "TestDataSource");
    configSignals.Write("Type", "float32");
    configSignals.Write("NumberOfDimensions", 1);
    configSignals.Write("NumberOfElements", numberOfElements);
    configSignals.Write("ByteSize", byteSize);

    configSignals.MoveToAncestor(1u);
    configSignals.Write("ByteSize", totalByteSize);

    configSignals.MoveToRoot();

    configSignals.CreateAbsolute("Memory.InputSignals");
    configSignals.CreateRelative("0");
    configSignals.Write("DataSource", "TestDataSource");
    configSignals.CreateRelative("Signals");
    configSignals.CreateRelative("0");
    configSignals.Write("Samples", 1);
    /*
     configSignals.CreateAbsolute("Memory.OutputSignals");
     configSignals.CreateRelative("0");
     configSignals.Write("DataSource", "TestDataSource");
     configSignals.CreateRelative("Signals");
     configSignals.CreateRelative("0");
     configSignals.Write("Samples", 1);
     */
    configSignals.MoveToRoot();

    ok &= gam.SetConfiguredDatabase(configSignals);
    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();

    ok &= !gam.Setup();
    delete[] num;
    delete[] den;
    return ok;
}

bool FilterGAMTest::TestSetupNoNumberOfElementsInput() {
    using namespace MARTe;
    FilterGAM gam;
    gam.SetName("Test");
    ConfigurationDatabase config;
    float32 *num = new float32[2];
    num[0] = 0.5;
    num[1] = 0.5;
    float32 *den = new float32[1];
    den[0] = 1;
    Vector<float32> numVec(num, 2);
    Vector<float32> denVec(den, 1);
    bool ok = config.Write("Num", numVec);
    ok &= config.Write("Den", denVec);
    ok &= config.Write("ResetInEachState", false);
    ok &= config.CreateRelative("InputSignals");
    ok &= config.CreateRelative("Input0");
    ok &= config.MoveToRoot();
    ok &= gam.Initialise(config);
    uint32 numberOfSamples = 1;
    uint32 numberOfElements = 10;
    uint32 byteSize = numberOfElements * sizeof(float32);
    uint32 totalByteSize = byteSize;
    ConfigurationDatabase configSignals;
    configSignals.CreateAbsolute("Signals.InputSignals");
    configSignals.CreateRelative("0");
    configSignals.Write("QualifiedName", "InputSignal1");
    configSignals.Write("DataSource", "TestDataSource");
    configSignals.Write("Type", "float32");
    configSignals.Write("NumberOfDimensions", 1);
    configSignals.Write("ByteSize", byteSize);

    configSignals.MoveToAncestor(1u);
    configSignals.Write("ByteSize", totalByteSize);

    configSignals.MoveToRoot();
    configSignals.CreateAbsolute("Signals.OutputSignals");
    configSignals.CreateRelative("0");
    configSignals.Write("QualifiedName", "OutputSignal1");
    configSignals.Write("DataSource", "TestDataSource");
    configSignals.Write("Type", "float32");
    configSignals.Write("NumberOfDimensions", 1);
    configSignals.Write("NumberOfElements", numberOfElements);
    configSignals.Write("ByteSize", byteSize);

    configSignals.MoveToAncestor(1u);
    configSignals.Write("ByteSize", totalByteSize);

    configSignals.MoveToRoot();

    configSignals.CreateAbsolute("Memory.InputSignals");
    configSignals.CreateRelative("0");
    configSignals.Write("DataSource", "TestDataSource");
    configSignals.CreateRelative("Signals");
    configSignals.CreateRelative("0");
    configSignals.Write("Samples", numberOfSamples);

    configSignals.CreateAbsolute("Memory.OutputSignals");
    configSignals.CreateRelative("0");
    configSignals.Write("DataSource", "TestDataSource");
    configSignals.CreateRelative("Signals");
    configSignals.CreateRelative("0");
    configSignals.Write("Samples", 1);

    configSignals.MoveToRoot();

    ok &= gam.SetConfiguredDatabase(configSignals);
    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();

    ok &= !gam.Setup();
    delete[] num;
    delete[] den;
    return ok;
}

bool FilterGAMTest::TestSetupNoNumberOfElementsOutput() {
    using namespace MARTe;
    FilterGAM gam;
    gam.SetName("Test");
    ConfigurationDatabase config;
    float32 *num = new float32[2];
    num[0] = 0.5;
    num[1] = 0.5;
    float32 *den = new float32[1];
    den[0] = 1;
    Vector<float32> numVec(num, 2);
    Vector<float32> denVec(den, 1);
    bool ok = config.Write("Num", numVec);
    ok &= config.Write("Den", denVec);
    ok &= config.Write("ResetInEachState", false);
    ok &= config.CreateRelative("InputSignals");
    ok &= config.CreateRelative("Input0");
    ok &= config.MoveToRoot();
    ok &= gam.Initialise(config);
    uint32 numberOfSamples = 1;
    uint32 numberOfElements = 10;
    uint32 byteSize = numberOfElements * sizeof(float32);
    uint32 totalByteSize = byteSize;
    ConfigurationDatabase configSignals;
    configSignals.CreateAbsolute("Signals.InputSignals");
    configSignals.CreateRelative("0");
    configSignals.Write("QualifiedName", "InputSignal1");
    configSignals.Write("DataSource", "TestDataSource");
    configSignals.Write("Type", "float32");
    configSignals.Write("NumberOfDimensions", 1);
    configSignals.Write("NumberOfElements", numberOfElements);
    configSignals.Write("ByteSize", byteSize);

    configSignals.MoveToAncestor(1u);
    configSignals.Write("ByteSize", totalByteSize);

    configSignals.MoveToRoot();
    configSignals.CreateAbsolute("Signals.OutputSignals");
    configSignals.CreateRelative("0");
    configSignals.Write("QualifiedName", "OutputSignal1");
    configSignals.Write("DataSource", "TestDataSource");
    configSignals.Write("Type", "float32");
    configSignals.Write("NumberOfDimensions", 1);
    configSignals.Write("ByteSize", byteSize);

    configSignals.MoveToAncestor(1u);
    configSignals.Write("ByteSize", totalByteSize);

    configSignals.MoveToRoot();

    configSignals.CreateAbsolute("Memory.InputSignals");
    configSignals.CreateRelative("0");
    configSignals.Write("DataSource", "TestDataSource");
    configSignals.CreateRelative("Signals");
    configSignals.CreateRelative("0");
    configSignals.Write("Samples", numberOfSamples);

    configSignals.CreateAbsolute("Memory.OutputSignals");
    configSignals.CreateRelative("0");
    configSignals.Write("DataSource", "TestDataSource");
    configSignals.CreateRelative("Signals");
    configSignals.CreateRelative("0");
    configSignals.Write("Samples", 1);

    configSignals.MoveToRoot();

    ok &= gam.SetConfiguredDatabase(configSignals);
    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();

    ok &= !gam.Setup();
    delete[] num;
    delete[] den;
    return ok;
}

bool FilterGAMTest::TestSetupNumberOfSamplesOutput2() {
    using namespace MARTe;
    FilterGAM gam;
    gam.SetName("Test");
    ConfigurationDatabase config;
    float32 *num = new float32[2];
    num[0] = 0.5;
    num[1] = 0.5;
    float32 *den = new float32[1];
    den[0] = 1;
    Vector<float32> numVec(num, 2);
    Vector<float32> denVec(den, 1);
    bool ok = config.Write("Num", numVec);
    ok &= config.Write("Den", denVec);
    ok &= config.Write("ResetInEachState", false);
    ok &= config.CreateRelative("InputSignals");
    ok &= config.CreateRelative("Input0");
    ok &= config.MoveToRoot();
    ok &= gam.Initialise(config);
    uint32 numberOfSamples = 1;
    uint32 numberOfElements = 10;
    uint32 byteSize = numberOfElements * sizeof(float32);
    uint32 totalByteSize = byteSize;
    ConfigurationDatabase configSignals;
    configSignals.CreateAbsolute("Signals.InputSignals");
    configSignals.CreateRelative("0");
    configSignals.Write("QualifiedName", "InputSignal1");
    configSignals.Write("DataSource", "TestDataSource");
    configSignals.Write("Type", "float32");
    configSignals.Write("NumberOfDimensions", 1);
    configSignals.Write("NumberOfElements", numberOfElements);
    configSignals.Write("ByteSize", byteSize);

    configSignals.MoveToAncestor(1u);
    configSignals.Write("ByteSize", totalByteSize);

    configSignals.MoveToRoot();
    configSignals.CreateAbsolute("Signals.OutputSignals");
    configSignals.CreateRelative("0");
    configSignals.Write("QualifiedName", "OutputSignal1");
    configSignals.Write("DataSource", "TestDataSource");
    configSignals.Write("Type", "float32");
    configSignals.Write("NumberOfDimensions", 1);
    configSignals.Write("NumberOfElements", numberOfElements);
    configSignals.Write("ByteSize", byteSize);

    configSignals.MoveToAncestor(1u);
    configSignals.Write("ByteSize", totalByteSize);

    configSignals.MoveToRoot();

    configSignals.CreateAbsolute("Memory.InputSignals");
    configSignals.CreateRelative("0");
    configSignals.Write("DataSource", "TestDataSource");
    configSignals.CreateRelative("Signals");
    configSignals.CreateRelative("0");
    configSignals.Write("Samples", numberOfSamples);

    configSignals.CreateAbsolute("Memory.OutputSignals");
    configSignals.CreateRelative("0");
    configSignals.Write("DataSource", "TestDataSource");
    configSignals.CreateRelative("Signals");
    configSignals.CreateRelative("0");
//Wrong number of output samples
    configSignals.Write("Samples", 2);

    configSignals.MoveToRoot();

    ok &= gam.SetConfiguredDatabase(configSignals);
    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();

    ok &= !gam.Setup();
    delete[] num;
    delete[] den;
    return ok;
}

bool FilterGAMTest::TestSetup0NumberOfElements() {
    using namespace MARTe;
    FilterGAM gam;
    gam.SetName("Test");
    ConfigurationDatabase config;
    float32 *num = new float32[2];
    num[0] = 0.5;
    num[1] = 0.5;
    float32 *den = new float32[1];
    den[0] = 1;
    Vector<float32> numVec(num, 2);
    Vector<float32> denVec(den, 1);
    bool ok = config.Write("Num", numVec);
    ok &= config.Write("Den", denVec);
    ok &= config.Write("ResetInEachState", false);
    ok &= config.CreateRelative("InputSignals");
    ok &= config.CreateRelative("Input0");
    ok &= config.MoveToRoot();
    ok &= gam.Initialise(config);
    uint32 numberOfSamples = 1;
    uint32 numberOfElements = 0;
//It fake the byteSize, but otherwise memory allocation fails..
    uint32 byteSize = 1 * sizeof(float32);
    uint32 totalByteSize = byteSize;
    ConfigurationDatabase configSignals;
    configSignals.CreateAbsolute("Signals.InputSignals");
    configSignals.CreateRelative("0");
    configSignals.Write("QualifiedName", "InputSignal1");
    configSignals.Write("DataSource", "TestDataSource");
    configSignals.Write("Type", "float32");
    configSignals.Write("NumberOfDimensions", 1);
    configSignals.Write("NumberOfElements", numberOfElements);
    configSignals.Write("ByteSize", byteSize);

    configSignals.MoveToAncestor(1u);
    configSignals.Write("ByteSize", totalByteSize);

    configSignals.MoveToRoot();
    configSignals.CreateAbsolute("Signals.OutputSignals");
    configSignals.CreateRelative("0");
    configSignals.Write("QualifiedName", "OutputSignal1");
    configSignals.Write("DataSource", "TestDataSource");
    configSignals.Write("Type", "float32");
    configSignals.Write("NumberOfDimensions", 1);
    configSignals.Write("NumberOfElements", numberOfElements);
    configSignals.Write("ByteSize", byteSize);

    configSignals.MoveToAncestor(1u);
    configSignals.Write("ByteSize", totalByteSize);

    configSignals.MoveToRoot();

    configSignals.CreateAbsolute("Memory.InputSignals");
    configSignals.CreateRelative("0");
    configSignals.Write("DataSource", "TestDataSource");
    configSignals.CreateRelative("Signals");
    configSignals.CreateRelative("0");
    configSignals.Write("Samples", numberOfSamples);

    configSignals.CreateAbsolute("Memory.OutputSignals");
    configSignals.CreateRelative("0");
    configSignals.Write("DataSource", "TestDataSource");
    configSignals.CreateRelative("Signals");
    configSignals.CreateRelative("0");
    configSignals.Write("Samples", 1);

    configSignals.MoveToRoot();

    ok &= gam.SetConfiguredDatabase(configSignals);
    gam.AllocateInputSignalsMemory();
    gam.AllocateOutputSignalsMemory();
    ok &= !gam.Setup();
    delete[] num;
    delete[] den;
    return ok;
}

bool FilterGAMTest::TestSetupFailNumberOfSamples() {
    using namespace MARTe;
    FilterGAM gam;
    gam.SetName("Test");
    ConfigurationDatabase config;
    float32 *num = new float32[2];
    num[0] = 0.5;
    num[1] = 0.5;
    float32 *den = new float32[1];
    den[0] = 1;
    Vector<float32> numVec(num, 2);
    Vector<float32> denVec(den, 1);
    bool ok = config.Write("Num", numVec);
    ok &= config.Write("Den", denVec);
    ok &= config.Write("ResetInEachState", false);
    ok &= config.CreateRelative("InputSignals");
    ok &= config.CreateRelative("Input0");
    ok &= config.MoveToRoot();
    ok &= gam.Initialise(config);
    uint32 numberOfSamples = 2;
    uint32 numberOfElements = 9;
    uint32 byteSize = numberOfElements * sizeof(float32);
    uint32 totalByteSize = byteSize;
    ConfigurationDatabase configSignals;
    configSignals.CreateAbsolute("Signals.InputSignals");
    configSignals.CreateRelative("0");
    configSignals.Write("QualifiedName", "InputSignal1");
    configSignals.Write("DataSource", "TestDataSource");
    configSignals.Write("Type", "float32");
    configSignals.Write("NumberOfDimensions", 1);
    configSignals.Write("NumberOfElements", numberOfElements);
    configSignals.Write("ByteSize", byteSize);

    configSignals.MoveToAncestor(1u);
    configSignals.Write("ByteSize", totalByteSize);

    configSignals.MoveToRoot();
    configSignals.CreateAbsolute("Signals.OutputSignals");
    configSignals.CreateRelative("0");
    configSignals.Write("QualifiedName", "OutputSignal1");
    configSignals.Write("DataSource", "TestDataSource");
    configSignals.Write("Type", "float32");
    configSignals.Write("NumberOfDimensions", 1);
    configSignals.Write("NumberOfElements", numberOfElements);
    configSignals.Write("ByteSize", byteSize);

    configSignals.MoveToAncestor(1u);
    configSignals.Write("ByteSize", totalByteSize);

    configSignals.MoveToRoot();

    configSignals.CreateAbsolute("Memory.InputSignals");
    configSignals.CreateRelative("0");
    configSignals.Write("DataSource", "TestDataSource");
    configSignals.CreateRelative("Signals");
    configSignals.CreateRelative("0");
    configSignals.Write("Samples", numberOfSamples);

    configSignals.CreateAbsolute("Memory.OutputSignals");
    configSignals.CreateRelative("0");
    configSignals.Write("DataSource", "TestDataSource");
    configSignals.CreateRelative("Signals");
    configSignals.CreateRelative("0");
    configSignals.Write("Samples", 1);
    configSignals.MoveToRoot();
    gam.SetConfiguredDatabase(configSignals);
    gam.AllocateInputSignalsMemory();
    gam.AllocateOutputSignalsMemory();
    ok &= !gam.Setup();
    delete[] num;
    delete[] den;
    return ok;
}

bool FilterGAMTest::TestSetupDifferentInputOutputSamples() {
    using namespace MARTe;
    FilterGAM gam;
    gam.SetName("Test");
    ConfigurationDatabase config;
    float32 *num = new float32[2];
    num[0] = 0.5;
    num[1] = 0.5;
    float32 *den = new float32[1];
    den[0] = 1;
    Vector<float32> numVec(num, 2);
    Vector<float32> denVec(den, 1);
    bool ok = config.Write("Num", numVec);
    ok &= config.Write("Den", denVec);
    ok &= config.Write("ResetInEachState", false);
    ok &= config.CreateRelative("InputSignals");
    ok &= config.CreateRelative("Input0");
    ok &= config.MoveToRoot();
    ok &= gam.Initialise(config);
    uint32 numberOfSamples = 1;
    uint32 numberOfElements = 9;
    uint32 byteSize = numberOfElements * sizeof(float32);
    uint32 totalByteSize = byteSize;
    ConfigurationDatabase configSignals;
    configSignals.CreateAbsolute("Signals.InputSignals");
    configSignals.CreateRelative("0");
    configSignals.Write("QualifiedName", "InputSignal1");
    configSignals.Write("DataSource", "TestDataSource");
    configSignals.Write("Type", "float32");
    configSignals.Write("NumberOfDimensions", 1);
    configSignals.Write("NumberOfElements", numberOfElements);
    configSignals.Write("ByteSize", byteSize);

    configSignals.MoveToAncestor(1u);
    configSignals.Write("ByteSize", totalByteSize);

    configSignals.MoveToRoot();
    configSignals.CreateAbsolute("Signals.OutputSignals");
    configSignals.CreateRelative("0");
    configSignals.Write("QualifiedName", "OutputSignal1");
    configSignals.Write("DataSource", "TestDataSource");
    configSignals.Write("Type", "float32");
    configSignals.Write("NumberOfDimensions", 1);
    configSignals.Write("NumberOfElements", numberOfElements + 1);
    configSignals.Write("ByteSize", byteSize);

    configSignals.MoveToAncestor(1u);
    configSignals.Write("ByteSize", totalByteSize);

    configSignals.MoveToRoot();

    configSignals.CreateAbsolute("Memory.InputSignals");
    configSignals.CreateRelative("0");
    configSignals.Write("DataSource", "TestDataSource");
    configSignals.CreateRelative("Signals");
    configSignals.CreateRelative("0");
    configSignals.Write("Samples", numberOfSamples);

    configSignals.CreateAbsolute("Memory.OutputSignals");
    configSignals.CreateRelative("0");
    configSignals.Write("DataSource", "TestDataSource");
    configSignals.CreateRelative("Signals");
    configSignals.CreateRelative("0");
    configSignals.Write("Samples", numberOfSamples);
    configSignals.MoveToRoot();
    gam.SetConfiguredDatabase(configSignals);
    gam.AllocateInputSignalsMemory();
    gam.AllocateOutputSignalsMemory();
    ok &= !gam.Setup();
    delete[] num;
    delete[] den;
    return ok;
}

bool FilterGAMTest::TestSetupNoInputDimension() {
    using namespace MARTe;
    FilterGAM gam;
    gam.SetName("Test");
    ConfigurationDatabase config;
    float32 *num = new float32[2];
    num[0] = 0.5;
    num[1] = 0.5;
    float32 *den = new float32[1];
    den[0] = 1;
    Vector<float32> numVec(num, 2);
    Vector<float32> denVec(den, 1);
    bool ok = config.Write("Num", numVec);
    ok &= config.Write("Den", denVec);
    ok &= config.Write("ResetInEachState", false);
    ok &= config.CreateRelative("InputSignals");
    ok &= config.CreateRelative("Input0");
    ok &= config.MoveToRoot();
    ok &= gam.Initialise(config);
    uint32 numberOfSamples = 1;
    uint32 numberOfElements = 9;
    uint32 byteSize = numberOfElements * sizeof(float32);
    uint32 totalByteSize = byteSize;
    ConfigurationDatabase configSignals;
    configSignals.CreateAbsolute("Signals.InputSignals");
    configSignals.CreateRelative("0");
    configSignals.Write("QualifiedName", "InputSignal1");
    configSignals.Write("DataSource", "TestDataSource");
    configSignals.Write("Type", "float32");
//configSignals.Write("NumberOfDimensions", 1);
    configSignals.Write("NumberOfElements", numberOfElements);
    configSignals.Write("ByteSize", byteSize);

    configSignals.MoveToAncestor(1u);
    configSignals.Write("ByteSize", totalByteSize);

    configSignals.MoveToRoot();
    configSignals.CreateAbsolute("Signals.OutputSignals");
    configSignals.CreateRelative("0");
    configSignals.Write("QualifiedName", "OutputSignal1");
    configSignals.Write("DataSource", "TestDataSource");
    configSignals.Write("Type", "float32");
    configSignals.Write("NumberOfDimensions", 1);
    configSignals.Write("NumberOfElements", numberOfElements);
    configSignals.Write("ByteSize", byteSize);

    configSignals.MoveToAncestor(1u);
    configSignals.Write("ByteSize", totalByteSize);

    configSignals.MoveToRoot();

    configSignals.CreateAbsolute("Memory.InputSignals");
    configSignals.CreateRelative("0");
    configSignals.Write("DataSource", "TestDataSource");
    configSignals.CreateRelative("Signals");
    configSignals.CreateRelative("0");
    configSignals.Write("Samples", numberOfSamples);

    configSignals.CreateAbsolute("Memory.OutputSignals");
    configSignals.CreateRelative("0");
    configSignals.Write("DataSource", "TestDataSource");
    configSignals.CreateRelative("Signals");
    configSignals.CreateRelative("0");
    configSignals.Write("Samples", numberOfSamples);
    configSignals.MoveToRoot();
    gam.SetConfiguredDatabase(configSignals);
    gam.AllocateInputSignalsMemory();
    gam.AllocateOutputSignalsMemory();
    ok &= !gam.Setup();
    delete[] num;
    delete[] den;
    return ok;
}

bool FilterGAMTest::TestSetupWrongInputDimension() {
    using namespace MARTe;
    FilterGAM gam;
    gam.SetName("Test");
    ConfigurationDatabase config;
    float32 *num = new float32[2];
    num[0] = 0.5;
    num[1] = 0.5;
    float32 *den = new float32[1];
    den[0] = 1;
    Vector<float32> numVec(num, 2);
    Vector<float32> denVec(den, 1);
    bool ok = config.Write("Num", numVec);
    ok &= config.Write("Den", denVec);
    ok &= config.Write("ResetInEachState", false);
    ok &= config.CreateRelative("InputSignals");
    ok &= config.CreateRelative("Input0");
    ok &= config.MoveToRoot();
    ok &= gam.Initialise(config);
    uint32 numberOfSamples = 1;
    uint32 numberOfElements = 9;
    uint32 byteSize = numberOfElements * sizeof(float32);
    uint32 totalByteSize = byteSize;
    ConfigurationDatabase configSignals;
    configSignals.CreateAbsolute("Signals.InputSignals");
    configSignals.CreateRelative("0");
    configSignals.Write("QualifiedName", "InputSignal1");
    configSignals.Write("DataSource", "TestDataSource");
    configSignals.Write("Type", "float32");
    configSignals.Write("NumberOfDimensions", 23);
    configSignals.Write("NumberOfElements", numberOfElements);
    configSignals.Write("ByteSize", byteSize);

    configSignals.MoveToAncestor(1u);
    configSignals.Write("ByteSize", totalByteSize);

    configSignals.MoveToRoot();
    configSignals.CreateAbsolute("Signals.OutputSignals");
    configSignals.CreateRelative("0");
    configSignals.Write("QualifiedName", "OutputSignal1");
    configSignals.Write("DataSource", "TestDataSource");
    configSignals.Write("Type", "float32");
    configSignals.Write("NumberOfDimensions", 1);
    configSignals.Write("NumberOfElements", numberOfElements);
    configSignals.Write("ByteSize", byteSize);

    configSignals.MoveToAncestor(1u);
    configSignals.Write("ByteSize", totalByteSize);

    configSignals.MoveToRoot();

    configSignals.CreateAbsolute("Memory.InputSignals");
    configSignals.CreateRelative("0");
    configSignals.Write("DataSource", "TestDataSource");
    configSignals.CreateRelative("Signals");
    configSignals.CreateRelative("0");
    configSignals.Write("Samples", numberOfSamples);

    configSignals.CreateAbsolute("Memory.OutputSignals");
    configSignals.CreateRelative("0");
    configSignals.Write("DataSource", "TestDataSource");
    configSignals.CreateRelative("Signals");
    configSignals.CreateRelative("0");
    configSignals.Write("Samples", numberOfSamples);
    configSignals.MoveToRoot();
    gam.SetConfiguredDatabase(configSignals);
    gam.AllocateInputSignalsMemory();
    gam.AllocateOutputSignalsMemory();
    ok &= !gam.Setup();
    delete[] num;
    delete[] den;
    return ok;
}
bool FilterGAMTest::TestSetupNoOutputDimension() {
    using namespace MARTe;
    FilterGAM gam;
    gam.SetName("Test");
    ConfigurationDatabase config;
    float32 *num = new float32[2];
    num[0] = 0.5;
    num[1] = 0.5;
    float32 *den = new float32[1];
    den[0] = 1;
    Vector<float32> numVec(num, 2);
    Vector<float32> denVec(den, 1);
    bool ok = config.Write("Num", numVec);
    ok &= config.Write("Den", denVec);
    ok &= config.Write("ResetInEachState", false);
    ok &= config.CreateRelative("InputSignals");
    ok &= config.CreateRelative("Input0");
    ok &= config.MoveToRoot();
    ok &= gam.Initialise(config);
    uint32 numberOfSamples = 1;
    uint32 numberOfElements = 9;
    uint32 byteSize = numberOfElements * sizeof(float32);
    uint32 totalByteSize = byteSize;
    ConfigurationDatabase configSignals;
    configSignals.CreateAbsolute("Signals.InputSignals");
    configSignals.CreateRelative("0");
    configSignals.Write("QualifiedName", "InputSignal1");
    configSignals.Write("DataSource", "TestDataSource");
    configSignals.Write("Type", "float32");
    configSignals.Write("NumberOfDimensions", 1);
    configSignals.Write("NumberOfElements", numberOfElements);
    configSignals.Write("ByteSize", byteSize);

    configSignals.MoveToAncestor(1u);
    configSignals.Write("ByteSize", totalByteSize);

    configSignals.MoveToRoot();
    configSignals.CreateAbsolute("Signals.OutputSignals");
    configSignals.CreateRelative("0");
    configSignals.Write("QualifiedName", "OutputSignal1");
    configSignals.Write("DataSource", "TestDataSource");
    configSignals.Write("Type", "float32");
//configSignals.Write("NumberOfDimensions", 1);
    configSignals.Write("NumberOfElements", numberOfElements);
    configSignals.Write("ByteSize", byteSize);

    configSignals.MoveToAncestor(1u);
    configSignals.Write("ByteSize", totalByteSize);

    configSignals.MoveToRoot();

    configSignals.CreateAbsolute("Memory.InputSignals");
    configSignals.CreateRelative("0");
    configSignals.Write("DataSource", "TestDataSource");
    configSignals.CreateRelative("Signals");
    configSignals.CreateRelative("0");
    configSignals.Write("Samples", numberOfSamples);

    configSignals.CreateAbsolute("Memory.OutputSignals");
    configSignals.CreateRelative("0");
    configSignals.Write("DataSource", "TestDataSource");
    configSignals.CreateRelative("Signals");
    configSignals.CreateRelative("0");
    configSignals.Write("Samples", numberOfSamples);
    configSignals.MoveToRoot();
    gam.SetConfiguredDatabase(configSignals);
    gam.AllocateInputSignalsMemory();
    gam.AllocateOutputSignalsMemory();
    ok &= !gam.Setup();
    delete[] num;
    delete[] den;
    return ok;
}

bool FilterGAMTest::TestSetupWrongOutputDimension() {
    using namespace MARTe;
    FilterGAM gam;
    gam.SetName("Test");
    ConfigurationDatabase config;
    float32 *num = new float32[2];
    num[0] = 0.5;
    num[1] = 0.5;
    float32 *den = new float32[1];
    den[0] = 1;
    Vector<float32> numVec(num, 2);
    Vector<float32> denVec(den, 1);
    bool ok = config.Write("Num", numVec);
    ok &= config.Write("Den", denVec);
    ok &= config.Write("ResetInEachState", false);
    ok &= config.CreateRelative("InputSignals");
    ok &= config.CreateRelative("Input0");
    ok &= config.MoveToRoot();
    ok &= gam.Initialise(config);
    uint32 numberOfSamples = 1;
    uint32 numberOfElements = 9;
    uint32 byteSize = numberOfElements * sizeof(float32);
    uint32 totalByteSize = byteSize;
    ConfigurationDatabase configSignals;
    configSignals.CreateAbsolute("Signals.InputSignals");
    configSignals.CreateRelative("0");
    configSignals.Write("QualifiedName", "InputSignal1");
    configSignals.Write("DataSource", "TestDataSource");
    configSignals.Write("Type", "float32");
    configSignals.Write("NumberOfDimensions", 1);
    configSignals.Write("NumberOfElements", numberOfElements);
    configSignals.Write("ByteSize", byteSize);

    configSignals.MoveToAncestor(1u);
    configSignals.Write("ByteSize", totalByteSize);

    configSignals.MoveToRoot();
    configSignals.CreateAbsolute("Signals.OutputSignals");
    configSignals.CreateRelative("0");
    configSignals.Write("QualifiedName", "OutputSignal1");
    configSignals.Write("DataSource", "TestDataSource");
    configSignals.Write("Type", "float32");
    configSignals.Write("NumberOfDimensions", 11);
    configSignals.Write("NumberOfElements", numberOfElements);
    configSignals.Write("ByteSize", byteSize);

    configSignals.MoveToAncestor(1u);
    configSignals.Write("ByteSize", totalByteSize);

    configSignals.MoveToRoot();

    configSignals.CreateAbsolute("Memory.InputSignals");
    configSignals.CreateRelative("0");
    configSignals.Write("DataSource", "TestDataSource");
    configSignals.CreateRelative("Signals");
    configSignals.CreateRelative("0");
    configSignals.Write("Samples", numberOfSamples);

    configSignals.CreateAbsolute("Memory.OutputSignals");
    configSignals.CreateRelative("0");
    configSignals.Write("DataSource", "TestDataSource");
    configSignals.CreateRelative("Signals");
    configSignals.CreateRelative("0");
    configSignals.Write("Samples", numberOfSamples);
    configSignals.MoveToRoot();
    gam.SetConfiguredDatabase(configSignals);
    gam.AllocateInputSignalsMemory();
    gam.AllocateOutputSignalsMemory();
    ok &= !gam.Setup();
    delete[] num;
    delete[] den;
    return ok;
}

bool FilterGAMTest::MyIsEqual(MARTe::float32 f1,
                              MARTe::float32 f2) {
    MARTe::float32 epsilon = 1e-5;
    MARTe::float32 diff = f1 - f2;
    bool ret;
    if (diff < 0) {
        diff = -diff;
    }
    ret = diff < epsilon;
    return ret;
}
bool FilterGAMTest::MyIsEqual(MARTe::float64 f1,
                              MARTe::float64 f2) {
    MARTe::float64 epsilon = 1e-5;
    MARTe::float64 diff = f1 - f2;
    bool ret;
    if (diff < 0) {
        diff = -diff;
    }
    ret = diff < epsilon;
    return ret;
}
bool FilterGAMTest::MyIsEqual(MARTe::int32 f1,
                              MARTe::int32 f2) {
    return f1 == f2;
}
bool FilterGAMTest::MyIsEqual(MARTe::int64 f1,
                              MARTe::int64 f2) {

    return f1 == f2;
}

}
