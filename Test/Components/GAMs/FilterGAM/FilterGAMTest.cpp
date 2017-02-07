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
#include <limits>
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

FilterGAMTestHelper    (MARTe::uint32 elements=10, MARTe::uint32 samples=1) {
        numberOfElements = elements;
        numberOfSamples = samples;
        if(numberOfElements > numberOfSamples) {
            byteSize = numberOfElements * sizeof(MARTe::float32);
        }
        else {
            byteSize = numberOfSamples * sizeof(MARTe::float32);
        }
        numH = NULL;
        denH = NULL;
        isInitialised = false;
    }

    virtual ~FilterGAMTestHelper () {

        if(numH != NULL) {
            delete[] numH;
        }
        if(denH != NULL) {
            delete[] denH;
        }

    }

    void *GetInputSignalsMemory() {
        return MARTe::GAM::GetInputSignalsMemory();
    }
    void *GetOutputSignalsMemory() {
        return MARTe::GAM::GetOutputSignalsMemory();
    }
    void *GetInputSignalsMemory(MARTe::uint32 idx) {
        return MARTe::GAM::GetInputSignalMemory(idx);
    }

    void *GetOutputSignalsMemory(MARTe::uint32 idx) {
        return MARTe::GAM::GetOutputSignalMemory(idx);
    }
    bool InitialiseFilterFIR() {
        bool ret = true;
        if(isInitialised == false) {
            numH = new MARTe::float32[2];
            numH[0] = 0.5;
            numH[1] = 0.5;
            denH = new MARTe::float32[1];
            denH[0] = 1;
            MARTe::Vector<MARTe::float32> numVec(numH, 2);
            MARTe::Vector<MARTe::float32> denVec(denH, 1);
            ret &= config.Write("Num", numVec);
            ret &= config.Write("Den", denVec);
            isInitialised = ret;
        }
        else {
            ret = false;
        }
        return ret;
    }
    bool InitialiseFilterIIR() {
        bool ret = true;
        if(isInitialised == false) {
            numH = new MARTe::float32[1];
            numH[0] = 1;
            denH = new MARTe::float32[2];
            denH[0] = 1;
            denH[1] = -1;
            MARTe::Vector<MARTe::float32> numVec(numH, 1);
            MARTe::Vector<MARTe::float32> denVec(denH, 2);
            ret &= config.Write("Num", numVec);
            ret &= config.Write("Den", denVec);
            isInitialised = ret;
        }
        else {
            ret = false;
        }
        return ret;
    }

    bool IsInitialised() {
        return isInitialised;
    }
    bool InitialiseConfigDataBaseSignal1() {
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
        if(numberOfSamples > numberOfElements) {
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

    bool InitialiseConfigDataBaseSignal2() {
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
        if(numberOfSamples > numberOfElements) {
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
        if(numberOfSamples > numberOfElements) {
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
    MARTe::uint32 numberOfElements;
    MARTe::uint32 numberOfSamples;
    MARTe::uint32 byteSize;
    MARTe::ConfigurationDatabase configSignals;
    MARTe::ConfigurationDatabase config;
    MARTe::float32 *numH;
    MARTe::float32 *denH;
private:
    bool isInitialised;
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
    ok &= (0 == gam.GetNumberOfSamples());
    ok &= (0 == gam.GetNumberOfSignals());
    delete[] num;
    delete[] den;
    return ok;
}

bool FilterGAMTest::TestInitialise() {
    using namespace MARTe;
    FilterGAMTestHelper gam;
    gam.SetName("Test");
    bool ok = true;

    ok &= gam.InitialiseFilterFIR();
    ok &= gam.Initialise(gam.config);

    ok &= gam.GetNumberOfNumCoeff() == 2;
    ok &= gam.GetNumberOfDenCoeff() == 1;
    float32 *retNum = new float32[2];
    float32 *retDen = new float32[1];
    ok &= gam.GetNumCoeff(retNum);
    for (uint32 i = 0; i < 2; i++) {
        ok &= (retNum[i] == gam.numH[i]);
    }
    ok &= gam.GetDenCoeff(retDen);
    for (uint32 i = 0u; i < 1; i++) {
        ok &= (retDen[i] == gam.denH[i]);
    }
    ok &= (1 == gam.GetStaticGain());
    ok &= (0 == gam.GetNumberOfSamples());
    ok &= (0 == gam.GetNumberOfSignals());
    delete[] retNum;
    delete[] retDen;
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

bool FilterGAMTest::TestStaticGainFIR() {
    using namespace MARTe;
    FilterGAMTestHelper gam;
    gam.SetName("Test");
    bool ok = true;

    ok &= gam.InitialiseFilterFIR();
    ok &= gam.Initialise(gam.config);
    //Check static gain
    ok &= (1 == gam.GetStaticGain());

    return ok;
}

bool FilterGAMTest::TestStaticGainIIR() {
    using namespace MARTe;
    FilterGAMTestHelper gam;
    gam.SetName("Test");
    bool ok = true;

    ok &= gam.InitialiseFilterIIR();
    ok &= gam.Initialise(gam.config);
    //Check static gain
    ok &= (std::numeric_limits<float32>::infinity() == gam.GetStaticGain());
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
    bool ok = true;

    ok &= gam.InitialiseFilterFIR();
    ok &= gam.Initialise(gam.config);

    ok &= gam.InitialiseConfigDataBaseSignal1();
    ok &= gam.SetConfiguredDatabase(gam.configSignals);

    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();

    ok &= gam.Setup();

    //Verify post-conditions
    ok &= (gam.GetNumberOfSamples() == gam.numberOfElements);
    ok &= (gam.GetNumberOfSignals() == 1);
    return ok;
}

bool FilterGAMTest::TestSetupNoInputSignal() {
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
    return ok;
}

bool FilterGAMTest::TestSetupNoOutputSignal() {
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
    return ok;
}

bool FilterGAMTest::TestSetupNoInputSamples() {
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
    return ok;
}

bool FilterGAMTest::TestSetupNoOutputSamples() {
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
    return ok;
}

bool FilterGAMTest::TestSetupNoNumberOfElementsInput() {
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
    return ok;
}

bool FilterGAMTest::TestSetupNoNumberOfElementsOutput() {
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
    return ok;
}

bool FilterGAMTest::TestSetupNumberOfSamplesOutput2() {
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
    configSignals.Write("Samples", 1);

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
    bool ok = true;

    ok &= gam.InitialiseFilterFIR();
    ok &= gam.Initialise(gam.config);

    ok &= gam.InitialiseConfigDataBaseSignal1();
    ok &= gam.SetConfiguredDatabase(gam.configSignals);

    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();

    ok &= gam.Setup();

    float32 *gamMemoryIn = static_cast<float32 *>(gam.GetInputSignalsMemory());
    float32 *gamMemoryOut = static_cast<float32 *>(gam.GetOutputSignalsMemory());
    //assign inputs and outputs
    for (uint32 i = 0u; i < gam.numberOfElements; i++) {
        gamMemoryIn[i] = 1;
        gamMemoryOut[i] = 0;
    }
    if (ok) {
        gam.Execute();
    }
    ok &= (gamMemoryOut[0] == 0.5);
    for (uint32 i = 1u; i < gam.numberOfElements; i++) {
        ok &= (gamMemoryOut[i] == 1.0);
    }
    return ok;
}

bool FilterGAMTest::TestExecuteFIRRampInput() {
    using namespace MARTe;
    FilterGAMTestHelper gam;
    gam.SetName("Test");
    bool ok = true;

    ok &= gam.InitialiseFilterFIR();
    ok &= gam.Initialise(gam.config);

    ok &= gam.InitialiseConfigDataBaseSignal1();
    ok &= gam.SetConfiguredDatabase(gam.configSignals);

    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();

    ok &= gam.Setup();

    float32 *gamMemoryIn = static_cast<float32 *>(gam.GetInputSignalsMemory());
    float32 *gamMemoryOut = static_cast<float32 *>(gam.GetOutputSignalsMemory());

    //assign inputs and outputs
    for (uint32 i = 0u; i < gam.numberOfSamples; i++) {
        gamMemoryIn[i] = i;
        gamMemoryOut[i] = 0;
    }
    if (ok) {
        gam.Execute();
    }
    ok &= (gamMemoryOut[0] == 0);
    for (uint32 i = 1u; i < gam.numberOfSamples; i++) {
        ok &= (gamMemoryOut[i] == i - 0.5);
    }
    return ok;
}

bool FilterGAMTest::TestExecuteFIRConstantInput2() {
    using namespace MARTe;
    FilterGAMTestHelper gam;
    gam.SetName("Test");
    bool ok = true;

    ok &= gam.InitialiseFilterFIR();
    ok &= gam.Initialise(gam.config);

    ok &= gam.InitialiseConfigDataBaseSignal1();
    ok &= gam.SetConfiguredDatabase(gam.configSignals);

    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();

    ok &= gam.Setup();

    float32 *gamMemoryIn = static_cast<float32 *>(gam.GetInputSignalsMemory());
    float32 *gamMemoryOut = static_cast<float32 *>(gam.GetOutputSignalsMemory());

    //assign inputs and outputs
    for (uint32 i = 0u; i < gam.numberOfElements; i++) {
        gamMemoryIn[i] = 1;
        gamMemoryOut[i] = 0;
    }
    if (ok) {
        gam.Execute();
    }
    ok &= (gamMemoryOut[0] == 0.5);
    for (uint32 i = 1u; i < gam.numberOfElements; i++) {
        ok &= (gamMemoryOut[i] == 1.0);
    }
    gam.Execute();
    for (uint32 i = 0u; i < gam.numberOfElements; i++) {
        ok &= (gamMemoryOut[i] == 1.0);
    }

    return ok;
}

bool FilterGAMTest::TestExecuteFIRRampInput2() {
    using namespace MARTe;
    FilterGAMTestHelper gam;
    gam.SetName("Test");
    bool ok = true;

    ok &= gam.InitialiseFilterFIR();
    ok &= gam.Initialise(gam.config);

    ok &= gam.InitialiseConfigDataBaseSignal1();
    ok &= gam.SetConfiguredDatabase(gam.configSignals);

    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();

    ok &= gam.Setup();

    float32 *gamMemoryIn = static_cast<float32 *>(gam.GetInputSignalsMemory());
    float32 *gamMemoryOut = static_cast<float32 *>(gam.GetOutputSignalsMemory());
    //assign inputs and outputs
    for (uint32 i = 0u; i < gam.numberOfElements; i++) {
        gamMemoryIn[i] = i;
        gamMemoryOut[i] = 0;
    }
    if (ok) {
        gam.Execute();
    }
    ok &= (gamMemoryOut[0] == 0);
    for (uint32 i = 1u; i < gam.numberOfElements; i++) {
        ok &= (gamMemoryOut[i] == i - 0.5);
    }
    for (uint32 i = 0u; i < gam.numberOfElements; i++) {
        gamMemoryIn[i] = i + 10;
        //gamMemoryOut[i] = 0;
    }
    gam.Execute();
    for (uint32 i = 0u; i < gam.numberOfElements; i++) {
        ok &= (gamMemoryOut[i] == 10 + i - 0.5);
    }
    return ok;
}

bool FilterGAMTest::TestExecuteIIRConstant() {
    using namespace MARTe;
    FilterGAMTestHelper gam;
    gam.SetName("Test");
    bool ok = true;

    ok &= gam.InitialiseFilterIIR();
    ok &= gam.Initialise(gam.config);

    ok &= gam.InitialiseConfigDataBaseSignal1();
    ok &= gam.SetConfiguredDatabase(gam.configSignals);

    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();

    ok &= gam.Setup();

    float32 *gamMemoryIn = static_cast<float32 *>(gam.GetInputSignalsMemory());
    float32 *gamMemoryOut = static_cast<float32 *>(gam.GetOutputSignalsMemory());
    //assign inputs and outputs
    for (uint32 i = 0u; i < gam.numberOfElements; i++) {
        gamMemoryIn[i] = 1;
        gamMemoryOut[i] = 0;
    }
    if (ok) {
        gam.Execute();
    }
    ok &= (gamMemoryOut[0] == 1);
    for (uint32 i = 1u; i < gam.numberOfElements; i++) {
        ok &= (gamMemoryOut[i] == 1 + i);
    }
    return ok;
}

bool FilterGAMTest::TestExecuteIIRConstant2() {
    using namespace MARTe;
    FilterGAMTestHelper gam;
    gam.SetName("Test");
    bool ok = true;

    ok &= gam.InitialiseFilterIIR();
    ok &= gam.Initialise(gam.config);

    ok &= gam.InitialiseConfigDataBaseSignal1();
    ok &= gam.SetConfiguredDatabase(gam.configSignals);

    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();

    ok &= gam.Setup();

    float32 *gamMemoryIn = static_cast<float32 *>(gam.GetInputSignalsMemory());
    float32 *gamMemoryOut = static_cast<float32 *>(gam.GetOutputSignalsMemory());
    //assign inputs and outputs
    for (uint32 i = 0u; i < gam.numberOfElements; i++) {
        gamMemoryIn[i] = 1;
        gamMemoryOut[i] = 0;
    }
    if (ok) {
        gam.Execute();
    }
    ok &= (gamMemoryOut[0] == 1);
    for (uint32 i = 1u; i < gam.numberOfElements; i++) {
        ok &= (gamMemoryOut[i] == 1 + i);
    }
    gam.Execute();
    for (uint32 i = 0u; i < gam.numberOfElements; i++) {
        ok &= (gamMemoryOut[i] == 10 + 1 + i);
    }
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
    FilterGAMTestHelper gam(1, 10);
    gam.SetName("Test");
    bool ok = true;

    ok &= gam.InitialiseFilterFIR();
    ok &= gam.Initialise(gam.config);

    ok &= gam.InitialiseConfigDataBaseSignal1();

    ok &= gam.SetConfiguredDatabase(gam.configSignals);

    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();

    ok &= gam.Setup();

    float32 *gamMemoryIn = static_cast<float32 *>(gam.GetInputSignalsMemory());
    float32 *gamMemoryOut = static_cast<float32 *>(gam.GetOutputSignalsMemory());

    //assign inputs and outputs
    for (uint32 i = 0u; i < gam.numberOfSamples; i++) {
        gamMemoryIn[i] = i;
        gamMemoryOut[i] = 0;
    }
    if (ok) {
        gam.Execute();
    }
    ok &= (gamMemoryOut[0] == 0);
    for (uint32 i = 1u; i < gam.numberOfSamples; i++) {
        ok &= (gamMemoryOut[i] == i - 0.5);
    }
    for (uint32 i = 0u; i < gam.numberOfSamples; i++) {
        gamMemoryIn[i] = i + 10;
        //gamMemoryOut[i] = 0;
    }
    if (ok) {
        gam.Execute();
    }
    for (uint32 i = 0u; i < gam.numberOfSamples; i++) {
        ok &= (gamMemoryOut[i] == 10 + i - 0.5);
    }
    //Check size elements input or output (they are the same)
    ok &= (gam.GetNumberOfSamples() == 10);
    return ok;
}

bool FilterGAMTest::TestSetupSeveralSignals() {
    using namespace MARTe;
    FilterGAMTestHelper gam;
    gam.SetName("Test");
    bool ok = true;

    ok &= gam.InitialiseFilterFIR();
    ok &= gam.Initialise(gam.config);

    ok &= gam.InitialiseConfigDataBaseSignal2();
    ok &= gam.SetConfiguredDatabase(gam.configSignals);

    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();

    ok &= gam.Setup();
    ok &= (gam.GetNumberOfInputSignals() == 2);
    ok &= (gam.GetNumberOfSamples() == gam.numberOfElements);
    return ok;
}

bool FilterGAMTest::TestSetupSeveralSignalsDiffNumberOfInputElements() {
    using namespace MARTe;
    FilterGAMTestHelper gam;
    gam.SetName("Test");
    bool ok = true;

    ok &= gam.InitialiseFilterFIR();
    ok &= gam.Initialise(gam.config);

    uint32 numberOfElements = 10;
    uint32 numberOfSamples = 1;
    uint32 byteSize = numberOfElements * sizeof(float32);
    uint32 numberOfElements1 = 11;
    uint32 byteSize1 = numberOfElements1 * sizeof(float32);
    uint32 totalByteSize = byteSize + byteSize1;
    ConfigurationDatabase configSignals;

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
    ok &= configSignals.Write("NumberOfElements", numberOfElements1);
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
    ok &= configSignals.Write("NumberOfElements", numberOfElements);
    ok &= configSignals.Write("ByteSize", byteSize);

    ok &= configSignals.MoveAbsolute("Signals.OutputSignals");
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("QualifiedName", "OutputSignal2");
    ok &= configSignals.Write("DataSource", "TestDataSource");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.Write("NumberOfElements", numberOfElements1);
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
    ok &= gam.SetConfiguredDatabase(configSignals);

    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();

    ok &= !gam.Setup();

    return ok;
}


bool FilterGAMTest::TestSetupSeveralSignalsDiffNumberOfOutputElements() {
    using namespace MARTe;
    FilterGAMTestHelper gam;
    gam.SetName("Test");
    bool ok = true;

    ok &= gam.InitialiseFilterFIR();
    ok &= gam.Initialise(gam.config);

    uint32 numberOfElements = 10;
    uint32 numberOfSamples = 1;
    uint32 byteSize = numberOfElements * sizeof(float32);
    uint32 numberOfElements1 = 11;
    uint32 byteSize1 = numberOfElements1 * sizeof(float32);
    uint32 totalByteSize = byteSize + byteSize1;
    ConfigurationDatabase configSignals;

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
    ok &= configSignals.Write("NumberOfElements", numberOfElements);
    ok &= configSignals.Write("ByteSize", byteSize);

    ok &= configSignals.MoveAbsolute("Signals.OutputSignals");
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("QualifiedName", "OutputSignal2");
    ok &= configSignals.Write("DataSource", "TestDataSource");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.Write("NumberOfElements", numberOfElements1);
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
    ok &= gam.SetConfiguredDatabase(configSignals);

    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();

    ok &= !gam.Setup();

    return ok;
}


bool FilterGAMTest::TestSetupSeveralSignalsDiffInputSamples() {
    using namespace MARTe;
    FilterGAMTestHelper gam;
    gam.SetName("Test");
    bool ok = true;

    ok &= gam.InitialiseFilterFIR();
    ok &= gam.Initialise(gam.config);

    uint32 numberOfElements = 10;
    uint32 numberOfSamples = 1;
    uint32 byteSize = numberOfElements * sizeof(float32);
    uint32 numberOfElements1 = 11;
    uint32 numberOfSamples1 = 2;
    uint32 byteSize1 = numberOfElements1 * sizeof(float32);
    uint32 totalByteSize = byteSize + byteSize1;
    ConfigurationDatabase configSignals;

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
    ok &= configSignals.Write("NumberOfElements", numberOfElements);
    ok &= configSignals.Write("ByteSize", byteSize);

    ok &= configSignals.MoveAbsolute("Signals.OutputSignals");
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("QualifiedName", "OutputSignal2");
    ok &= configSignals.Write("DataSource", "TestDataSource");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.Write("NumberOfElements", numberOfElements);
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
    ok &= configSignals.Write("Samples", numberOfSamples1);

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
    ok &= gam.SetConfiguredDatabase(configSignals);

    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();

    ok &= !gam.Setup();

    return ok;
}

bool FilterGAMTest::TestExecuteSeveralSignalsFIR() {
    using namespace MARTe;
    FilterGAMTestHelper gam;
    gam.SetName("Test");
    bool ok = true;

    ok &= gam.InitialiseFilterFIR();
    ok &= gam.Initialise(gam.config);

    ok &= gam.InitialiseConfigDataBaseSignal2();
    ok &= gam.SetConfiguredDatabase(gam.configSignals);

    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();

    ok &= gam.Setup();

    float32 *gamMemoryIn0 = static_cast<float32 *>(gam.GetInputSignalsMemory());
    float32 *gamMemoryOut0 = static_cast<float32 *>(gam.GetOutputSignalsMemory());
    float32 *gamMemoryIn1 = static_cast<float32 *>(gam.GetInputSignalsMemory(1));
    float32 *gamMemoryOut1 = static_cast<float32 *>(gam.GetOutputSignalsMemory(1));
    //assign inputs and outputs
    for (uint32 i = 0u; i < gam.numberOfElements; i++) {
        gamMemoryIn0[i] = i;
        gamMemoryOut0[i] = 0;
        gamMemoryIn1[i] = 1;
        gamMemoryOut1[i] = 0;
    }
    if (ok) {
        gam.Execute();
    }
    ok &= (gamMemoryOut0[0] == 0);
    ok &= (gamMemoryOut1[0] == 0.5);
    for (uint32 i = 1u; i < gam.numberOfElements; i++) {
        ok &= (gamMemoryOut0[i] == i - 0.5);
        ok &= (gamMemoryOut1[i] == 1);
    }
    for (uint32 i = 0u; i < gam.numberOfElements; i++) {
        gamMemoryIn0[i] = i + 10;
        gamMemoryIn1[i] = 1;
        //gamMemoryOut[i] = 0;
    }
    gam.Execute();
    for (uint32 i = 0u; i < gam.numberOfElements; i++) {
        ok &= (gamMemoryOut0[i] == 10 + i - 0.5);
        ok &= (gamMemoryOut1[i] == 1);
    }
    return ok;
}

