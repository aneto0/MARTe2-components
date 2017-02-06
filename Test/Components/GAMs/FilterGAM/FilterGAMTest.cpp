/**
 * @file FilterGAMTest.cpp
 * @brief Source file for class FilterGAMTest
 * @date Jan 30, 2017 TODO Verify the value and format of the date
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

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ConfigurationDatabase.h"
#include "FilterGAM.h"
#include "FilterGAMTest.h"
#include "Vector.h"
#include "stdio.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

class FilterGAMTestHelper: public MARTe::FilterGAM {
public:
    CLASS_REGISTER_DECLARATION()

FilterGAMTestHelper    () {

    }

    virtual ~FilterGAMTestHelper () {

    }

    void *GetInputSignalsMemory() {
        return MARTe::GAM::GetInputSignalsMemory();
    }

    void *GetOutputSignalsMemory() {
        return MARTe::GAM::GetOutputSignalsMemory();
    }
};
CLASS_REGISTER(FilterGAMTestHelper, "1.0")

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

FilterGAMTest::FilterGAMTest() {
//Auto-generated constructor stub for FilterGAMTest

//TODO Verify if manual additions are needed here
}

FilterGAMTest::~FilterGAMTest() {
//Auto-generated destructor stub for FilterGAMTest

//TODO Verify if manual additions are needed here
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

//Check that the static gain is 0
    ok &= (0 == gam.GetStaticGain());
//Check that the number of numerator coefficients is 0
    ok &= (0 == gam.GetNumberOfNumCoeff());
// Idem denominator
    ok &= (0 == gam.GetNumberOfDenCoeff());
//Check normalisation
    ok &= !gam.CheckNormalisation();

    return ok;
}

bool FilterGAMTest::TestInitialise() {
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
    ok &= gam.Initialise(config);
    delete[] num;
    delete[] den;
    return ok;
}

bool FilterGAMTest::TestNumberOfNumCoeff() {
    using namespace MARTe;
    FilterGAM gam;
    gam.SetName("Test");
    ConfigurationDatabase config;
    uint32 numberOfNumCoeff = 2;
    uint32 numberOfDenCoeff = 1;
    float32 *num = new float32[numberOfNumCoeff];
    num[0] = 0.5;
    num[1] = 0.5;
    Vector<float32> numVec(num, numberOfNumCoeff);

    float32 *den = new float32[numberOfDenCoeff];
    den[0] = 1;
    Vector<float32> denVec(den, numberOfDenCoeff);

    bool ok = config.Write("Num", numVec);
    ok &= config.Write("Den", denVec);
    ok &= gam.Initialise(config);
    ok &= (gam.GetNumberOfNumCoeff() == (numberOfNumCoeff));
    delete[] num;
    delete[] den;
    return ok;
}

bool FilterGAMTest::TestValuesOfNumCoeff() {
    using namespace MARTe;
    FilterGAM gam;
    gam.SetName("Test");
    ConfigurationDatabase config;
    uint32 numberOfNumCoeff = 2;
    uint32 numberOfDenCoeff = 1;
    float32 *num = new float32[numberOfNumCoeff];
    num[0] = 0.5;
    num[1] = 0.5;
    Vector<float32> numVec(num, numberOfNumCoeff);

    float32 *den = new float32[numberOfDenCoeff];
    den[0] = 1;
    Vector<float32> denVec(den, numberOfDenCoeff);

    bool ok = config.Write("Num", numVec);
    ok &= config.Write("Den", denVec);
    ok &= gam.Initialise(config);
    float32 *numRet = new float32[numberOfNumCoeff];
    gam.GetNumCoeff(numRet);
    for (uint32 i = 0; i < numberOfNumCoeff; i++) {
        ok &= (num[i] == numRet[i]);
    }
    delete[] num;
    delete[] numRet;
    delete[] den;
    return ok;
}

bool FilterGAMTest::TestInitialiseNum0() {
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
    ok &= gam.Initialise(config);
    delete[] num;
    delete[] den;
    return !ok;
}

bool FilterGAMTest::TestNumberOfDenCoeff() {
    using namespace MARTe;
    FilterGAM gam;
    gam.SetName("Test");
    ConfigurationDatabase config;
    uint32 numberOfNumCoeff = 2;
    uint32 numberOfDenCoeff = 1;
    float32 *num = new float32[numberOfNumCoeff];
    num[0] = 0.5;
    num[1] = 0.5;
    Vector<float32> numVec(num, numberOfNumCoeff);

    float32 *den = new float32[numberOfDenCoeff];
    den[0] = 1;
    Vector<float32> denVec(den, numberOfDenCoeff);

    bool ok = config.Write("Num", numVec);
    ok &= config.Write("Den", denVec);
    ok &= gam.Initialise(config);
    ok &= (gam.GetNumberOfDenCoeff() == (numberOfDenCoeff));
    delete[] num;
    delete[] den;
    return ok;
}

bool FilterGAMTest::TestValuesOfDenCoeff() {
    using namespace MARTe;
    FilterGAM gam;
    gam.SetName("Test");
    ConfigurationDatabase config;
    uint32 numberOfNumCoeff = 2;
    uint32 numberOfDenCoeff = 2;
    float32 *num = new float32[numberOfNumCoeff];
    num[0] = 0.5;
    num[1] = 0.5;
    Vector<float32> numVec(num, numberOfNumCoeff);

    float32 *den = new float32[numberOfDenCoeff];
    den[0] = 1;
    den[1] = 2;
    Vector<float32> denVec(den, numberOfDenCoeff);

    float32 *denRet = new float32[numberOfDenCoeff];
    bool ok = config.Write("Num", numVec);
    ok &= config.Write("Den", denVec);
    ok &= gam.Initialise(config);
    gam.GetDenCoeff(denRet);
    for (uint32 i = 0; i < numberOfDenCoeff; i++) {
        ok &= (den[i] == denRet[i]);
    }
    delete[] num;
    delete[] den;
    delete[] denRet;
    return ok;
}

bool FilterGAMTest::TestInitialiseDen0() {
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
    ok &= gam.Initialise(config);
    ok &= (0 == gam.GetNumberOfDenCoeff());
    delete[] num;
    return !ok;
}

bool FilterGAMTest::TestStaticGain() {
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
    den[0] = 1;
    Vector<float32> numVec(num, numberOfNumCoeff);
    Vector<float32> denVec(den, numberOfDenCoeff);
    ok &= config.Write("Num", numVec);
    ok &= config.Write("Den", denVec);
    ok &= gam.Initialise(config);
    ok &= (1 == gam.GetStaticGain());
    delete[] num;
    delete[] den;
    return ok;
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
    ok &= !gam.Initialise(config);
    ok &= !gam.CheckNormalisation();
    delete[] num;
    delete[] den;
    return ok;
}

bool FilterGAMTest::TestSetup() {
    using namespace MARTe;
    FilterGAMTestHelper gam;
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
    ok &= gam.Initialise(config);
    uint32 numberOfElements = 10;
    uint32 numberOfSamples = 1;
    uint32 byteSize = numberOfElements * sizeof(float32);
    uint32 totalByteSize = byteSize;
    ConfigurationDatabase configSignals;
    configSignals.CreateAbsolute("Signals.InputSignals");
    configSignals.CreateRelative("0");
    configSignals.Write("QualifiedName", "InputSignal1");
    configSignals.Write("DataSource", "TestDataSource");
    configSignals.Write("Type", "float32");
    configSignals.Write("NumberOfDimensions", 1);
    configSignals.Write("Samples", numberOfSamples);
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
    configSignals.Write("Samples", numberOfSamples);
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
    ok &= gam.Setup();
    return ok;
}

bool FilterGAMTest::TestSetupFailInputs() {
    using namespace MARTe;
    FilterGAMTestHelper gam;
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
    ok &= gam.Initialise(config);
    uint32 numberOfSamples = 1;
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
     configSignals.Write("Samples", numberOfSamples);
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
    configSignals.Write("Samples", numberOfSamples);
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
    return ok;
}

bool FilterGAMTest::TestSetupFailOutputs() {
    using namespace MARTe;
    FilterGAMTestHelper gam;
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
    configSignals.Write("Samples", numberOfSamples);
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
     configSignals.Write("Samples", numberOfSamples);
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
    return ok;
}

bool FilterGAMTest::TestSetup0NumberOfElements() {
    using namespace MARTe;
    FilterGAMTestHelper gam;
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
    configSignals.Write("Samples", numberOfSamples);

    configSignals.MoveToRoot();

    ok &= gam.SetConfiguredDatabase(configSignals);
    gam.AllocateInputSignalsMemory();
    gam.AllocateOutputSignalsMemory();
    ok &= !gam.Setup();
    return ok;
}

bool FilterGAMTest::TestSetupFailNumberOfSamples() {
    using namespace MARTe;
    FilterGAMTestHelper gam;
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
    return ok;
}

bool FilterGAMTest::TestSetupDifferentInputOutputSamples() {
    using namespace MARTe;
    FilterGAMTestHelper gam;
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
    return ok;
}

bool FilterGAMTest::TestExecuteFIRConstantInput() {
    using namespace MARTe;
    FilterGAMTestHelper gam;
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
    ok = config.Write("Den", denVec);
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
    configSignals.Write("Samples", numberOfSamples);

    configSignals.MoveToRoot();

    ok &= gam.SetConfiguredDatabase(configSignals);
    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();
    ok &= gam.Setup();

    float32 *gamMemoryIn = static_cast<float32 *>(gam.GetInputSignalsMemory());
    float32 *gamMemoryOut = static_cast<float32 *>(gam.GetOutputSignalsMemory());
    //assign inputs and outputs
    for (uint32 i = 0u; i < numberOfElements; i++) {
        gamMemoryIn[i] = 1;
        gamMemoryOut[i] = 0;
    }
    if (ok) {
        gam.Execute();
    }
    ok &= (gamMemoryOut[0] == 0.5);
    for (uint32 i = 1u; i < numberOfElements; i++) {
        ok &= (gamMemoryOut[i] == 1.0);
    }

    delete[] num;
    delete[] den;
    return ok;
}

bool FilterGAMTest::TestExecuteFIRRampInput() {
    using namespace MARTe;
    FilterGAMTestHelper gam;
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
    ok = config.Write("Den", denVec);
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
    configSignals.Write("Samples", numberOfSamples);

    configSignals.MoveToRoot();

    ok &= gam.SetConfiguredDatabase(configSignals);
    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();
    ok &= gam.Setup();

    float32 *gamMemoryIn = static_cast<float32 *>(gam.GetInputSignalsMemory());
    float32 *gamMemoryOut = static_cast<float32 *>(gam.GetOutputSignalsMemory());
    //assign inputs and outputs
    for (uint32 i = 0u; i < numberOfSamples; i++) {
        gamMemoryIn[i] = i;
        gamMemoryOut[i] = 0;
    }
    if (ok) {
        gam.Execute();
    }
    ok &= (gamMemoryOut[0] == 0);
    for (uint32 i = 1u; i < numberOfSamples; i++) {
        ok &= (gamMemoryOut[i] == i - 0.5);
    }

    delete[] num;
    delete[] den;
    return ok;
}

bool FilterGAMTest::TestExecuteFIRConstantInput2() {
    using namespace MARTe;
    FilterGAMTestHelper gam;
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
    ok = config.Write("Den", denVec);
    ok &= gam.Initialise(config);
    uint32 numberOfElements = 10;
    uint32 numberOfSamples = 1;
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
    configSignals.Write("Samples", numberOfSamples);

    configSignals.MoveToRoot();

    ok &= gam.SetConfiguredDatabase(configSignals);
    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();
    ok &= gam.Setup();

    float32 *gamMemoryIn = static_cast<float32 *>(gam.GetInputSignalsMemory());
    float32 *gamMemoryOut = static_cast<float32 *>(gam.GetOutputSignalsMemory());
    //assign inputs and outputs
    for (uint32 i = 0u; i < numberOfElements; i++) {
        gamMemoryIn[i] = 1;
        gamMemoryOut[i] = 0;
    }
    if (ok) {
        gam.Execute();
    }
    ok &= (gamMemoryOut[0] == 0.5);
    for (uint32 i = 1u; i < numberOfElements; i++) {
        ok &= (gamMemoryOut[i] == 1.0);
    }
    gam.Execute();
    for (uint32 i = 0u; i < numberOfElements; i++) {
        ok &= (gamMemoryOut[i] == 1.0);
    }
    delete[] num;
    delete[] den;
    return ok;
}

bool FilterGAMTest::TestExecuteFIRRampInput2() {
    using namespace MARTe;
    FilterGAMTestHelper gam;
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
    ok = config.Write("Den", denVec);
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
    configSignals.Write("Samples", numberOfSamples);

    configSignals.MoveToRoot();

    ok &= gam.SetConfiguredDatabase(configSignals);
    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();
    ok &= gam.Setup();

    float32 *gamMemoryIn = static_cast<float32 *>(gam.GetInputSignalsMemory());
    float32 *gamMemoryOut = static_cast<float32 *>(gam.GetOutputSignalsMemory());
    //assign inputs and outputs
    for (uint32 i = 0u; i < numberOfElements; i++) {
        gamMemoryIn[i] = i;
        gamMemoryOut[i] = 0;
    }
    if (ok) {
        gam.Execute();
    }
    ok &= (gamMemoryOut[0] == 0);
    for (uint32 i = 1u; i < numberOfElements; i++) {
        ok &= (gamMemoryOut[i] == i - 0.5);
    }
    for (uint32 i = 0u; i < numberOfElements; i++) {
        gamMemoryIn[i] = i + 10;
        //gamMemoryOut[i] = 0;
    }
    gam.Execute();
    for (uint32 i = 0u; i < numberOfElements; i++) {
        ok &= (gamMemoryOut[i] == 10 + i - 0.5);
    }

    delete[] num;
    delete[] den;
    return ok;
}

bool FilterGAMTest::TestExecuteIIRConstant() {
    using namespace MARTe;
    FilterGAMTestHelper gam;
    gam.SetName("Test");
    ConfigurationDatabase config;
    float32 *num = new float32[1];
    num[0] = 1;
    float32 *den = new float32[2];
    den[0] = 1;
    den[1] = -1;
    Vector<float32> numVec(num, 1);
    Vector<float32> denVec(den, 2);
    bool ok = config.Write("Num", numVec);
    ok = config.Write("Den", denVec);
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
    configSignals.Write("Samples", numberOfSamples);

    configSignals.MoveToRoot();

    ok &= gam.SetConfiguredDatabase(configSignals);
    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();
    ok &= gam.Setup();

    float32 *gamMemoryIn = static_cast<float32 *>(gam.GetInputSignalsMemory());
    float32 *gamMemoryOut = static_cast<float32 *>(gam.GetOutputSignalsMemory());
    //assign inputs and outputs
    for (uint32 i = 0u; i < numberOfElements; i++) {
        gamMemoryIn[i] = 1;
        gamMemoryOut[i] = 0;
    }
    if (ok) {
        gam.Execute();
    }
    ok &= (gamMemoryOut[0] == 1);
    for (uint32 i = 1u; i < numberOfElements; i++) {
        ok &= (gamMemoryOut[i] == 1 + i);
    }
    delete[] num;
    delete[] den;
    return ok;
}

bool FilterGAMTest::TestExecuteIIRConstant2() {
    using namespace MARTe;
    FilterGAMTestHelper gam;
    gam.SetName("Test");
    ConfigurationDatabase config;
    float32 *num = new float32[1];
    num[0] = 1;
    float32 *den = new float32[2];
    den[0] = 1;
    den[1] = -1;
    Vector<float32> numVec(num, 1);
    Vector<float32> denVec(den, 2);
    bool ok = config.Write("Num", numVec);
    ok = config.Write("Den", denVec);
    ok &= gam.Initialise(config);
    uint32 numberOfElements = 10;
    uint32 numberOfSamples = 1;
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
    configSignals.Write("Samples", numberOfSamples);

    configSignals.MoveToRoot();

    ok &= gam.SetConfiguredDatabase(configSignals);
    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();
    ok &= gam.Setup();

    float32 *gamMemoryIn = static_cast<float32 *>(gam.GetInputSignalsMemory());
    float32 *gamMemoryOut = static_cast<float32 *>(gam.GetOutputSignalsMemory());
    //assign inputs and outputs
    for (uint32 i = 0u; i < numberOfElements; i++) {
        gamMemoryIn[i] = 1;
        gamMemoryOut[i] = 0;
    }
    if (ok) {
        gam.Execute();
    }
    ok &= (gamMemoryOut[0] == 1);
    for (uint32 i = 1u; i < numberOfElements; i++) {
        ok &= (gamMemoryOut[i] == 1 + i);
    }
    gam.Execute();
    for (uint32 i = 0u; i < numberOfElements; i++) {
        ok &= (gamMemoryOut[i] == 10 + 1 + i);
    }
    delete[] num;
    delete[] den;
    return ok;
}

bool FilterGAMTest::TestExecuteIIRConstantDimArray1() {
    using namespace MARTe;
    FilterGAMTestHelper gam;
    gam.SetName("Test");
    ConfigurationDatabase config;
    float32 *num = new float32[1];
    num[0] = 1;
    float32 *den = new float32[2];
    den[0] = 1;
    den[1] = -1;
    Vector<float32> numVec(num, 1);
    Vector<float32> denVec(den, 2);
    bool ok = config.Write("Num", numVec);
    ok = config.Write("Den", denVec);
    ok &= gam.Initialise(config);
    uint32 numberOfElements = 1;
    uint32 numberOfSamples = 1;
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
    configSignals.Write("Samples", numberOfSamples);

    configSignals.MoveToRoot();

    ok &= gam.SetConfiguredDatabase(configSignals);
    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();
    ok &= gam.Setup();

    float32 *gamMemoryIn = static_cast<float32 *>(gam.GetInputSignalsMemory());
    float32 *gamMemoryOut = static_cast<float32 *>(gam.GetOutputSignalsMemory());
    //assign inputs and outputs
    for (uint32 i = 0u; i < numberOfElements; i++) {
        gamMemoryIn[i] = 1;
        gamMemoryOut[i] = 0;
    }
    if (ok) {
        gam.Execute();
    }
    ok &= (gamMemoryOut[0] == 1);

    ok &= (gamMemoryOut[0] == 1);
    //Call execute 9 extra times just to filter the signal, which has dimension = 1,
    uint32 refValue = 1u;
    for (uint32 n = 0; n < 9; n++) {
        gam.Execute();
        refValue++;
        ok &= (gamMemoryOut[0] == refValue);
    }
    delete[] num;
    delete[] den;
    return ok;
}

bool FilterGAMTest::TestExecuteElements1Samples10() {
    using namespace MARTe;
    FilterGAMTestHelper gam;
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
    ok = config.Write("Den", denVec);
    ok &= gam.Initialise(config);

    uint32 numberOfSamples = 10;
    uint32 numberOfElements = 1;
    uint32 byteSize = numberOfSamples * sizeof(float32);
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
    //The Filter GAM only support numberOfSamples 1 and numberOfElements>=1
    configSignals.Write("NumberOfElements", numberOfSamples);
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
    //The Filter GAM only support numberOfSamples 1 and numberOfElements>=1
    configSignals.Write("Samples", 1);

    configSignals.MoveToRoot();

    ok &= gam.SetConfiguredDatabase(configSignals);
    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();
    ok &= gam.Setup();

    float32 *gamMemoryIn = static_cast<float32 *>(gam.GetInputSignalsMemory());
    float32 *gamMemoryOut = static_cast<float32 *>(gam.GetOutputSignalsMemory());
    //assign inputs and outputs
    for (uint32 i = 0u; i < numberOfSamples; i++) {
        gamMemoryIn[i] = i;
        gamMemoryOut[i] = 0;
    }
    if (ok) {
        gam.Execute();
    }
    ok &= (gamMemoryOut[0] == 0);
    for (uint32 i = 1u; i < numberOfSamples; i++) {
        ok &= (gamMemoryOut[i] == i - 0.5);
    }
    for (uint32 i = 0u; i < numberOfSamples; i++) {
        gamMemoryIn[i] = i + 10;
        //gamMemoryOut[i] = 0;
    }
    if(ok){
        gam.Execute();
    }
    for (uint32 i = 0u; i < numberOfSamples; i++) {
        ok &= (gamMemoryOut[i] == 10 + i - 0.5);
    }

    delete[] num;
    delete[] den;
    return ok;
}


