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

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

class FilterGAMTestHelper: public MARTe::FilterGAM {
public:CLASS_REGISTER_DECLARATION()

    FilterGAMTestHelper(MARTe::uint32 elements = 10,
                        MARTe::uint32 samples = 1) {
        numberOfElements = elements;
        numberOfSamples = samples;
        if (numberOfElements > numberOfSamples) {
            byteSize = numberOfElements * sizeof(MARTe::float32);
        }
        else {
            byteSize = numberOfSamples * sizeof(MARTe::float32);
        }
        numH = NULL;
        denH = NULL;
        isInitialised = false;
    }

    virtual ~FilterGAMTestHelper() {

        if (numH != NULL) {
            delete[] numH;
        }
        if (denH != NULL) {
            delete[] denH;
        }

    }

    void* GetInputSignalsMemory() {
        return MARTe::GAM::GetInputSignalsMemory();
    }
    void* GetOutputSignalsMemory() {
        return MARTe::GAM::GetOutputSignalsMemory();
    }
    void* GetInputSignalsMemory(MARTe::uint32 idx) {
        return MARTe::GAM::GetInputSignalMemory(idx);
    }

    void* GetOutputSignalsMemory(MARTe::uint32 idx) {
        return MARTe::GAM::GetOutputSignalMemory(idx);
    }
    bool InitialiseNoFilter() {
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
            isInitialised = ret;
        }
        else {
            ret = false;
        }
        return ret;
    }
    bool InitialiseFilterFIR() {
        bool ret = true;
        if (isInitialised == false) {
            numH = new MARTe::float32[2];
            numH[0] = 0.5;
            numH[1] = 0.5;
            denH = new MARTe::float32[1];
            denH[0] = 1;
            MARTe::Vector<MARTe::float32> numVec(numH, 2);
            MARTe::Vector<MARTe::float32> denVec(denH, 1);
            ret &= config.Write("Num", numVec);
            ret &= config.Write("Den", denVec);
            ret &= config.Write("ResetInEachState", false);
            isInitialised = ret;
        }
        else {
            ret = false;
        }
        return ret;
    }

    bool InitialiseAVG10() {
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
            isInitialised = ret;
        }
        else {
            ret = false;
        }
        return ret;
    }

    bool InitialiseFilterFIR2() {
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
            isInitialised = ret;
        }
        else {
            ret = false;
        }
        return ret;
    }
    bool InitialiseFilterIIR() {
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
            bool resetInEachState = 0;
            ret &= config.Write("ResetInEachState", resetInEachState);
            isInitialised = ret;
        }
        else {
            ret = false;
        }
        return ret;
    }

    bool InitialiseFilterIIR2() {
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

    bool InitialiseConfigDataBaseSignal3() {
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

    bool InitialiseConfigDataBaseSignal4() {
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
}

FilterGAMTest::~FilterGAMTest() {
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
    ok &= (0 == gam.GetStaticGain(isInfinite));
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

bool FilterGAMTest::TestInitialise() {
    using namespace MARTe;
    FilterGAMTestHelper gam;
    gam.SetName("Test");
    bool ok = true;

    ok &= gam.InitialiseFilterFIR();
    ok &= gam.Initialise(gam.config);
    StreamString a;
    //example how to print a ConfigurationDatabase
    /*
     gam.config.MoveToRoot();
     printf("size of a %llu\n",a.Size());
     printf("%d\n",a.Printf("%!", gam.config));
     printf("size of a %llu\n",a.Size());
     printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
     printf("%s\n", a.Buffer());
     printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
     */
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
    bool isInfinite;
    ok &= (1 == gam.GetStaticGain(isInfinite));
    ok &= !isInfinite;
    ok &= (0 == gam.GetNumberOfSamples());
    ok &= (0 == gam.GetNumberOfSignals());
    ok &= !gam.GetResetInEachState();
    delete[] retNum;
    delete[] retDen;
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

bool FilterGAMTest::TestStaticGainFIR() {
    using namespace MARTe;
    FilterGAMTestHelper gam;
    gam.SetName("Test");
    bool ok = true;

    ok &= gam.InitialiseFilterFIR();
    ok &= gam.Initialise(gam.config);
    //Check static gain
    bool isInfinite = true;
    ok &= (1 == gam.GetStaticGain(isInfinite));
    ok &= !isInfinite;

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
    bool isInfinite = false;
    ok = (gam.GetStaticGain(isInfinite) == 0);
    ok &= isInfinite;
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

bool FilterGAMTest::TestSetupWrongInputSignalType() {
    using namespace MARTe;
    FilterGAMTestHelper gam;
    gam.SetName("Test");
    bool ok = true;

    ok &= gam.InitialiseFilterFIR();
    ok &= gam.Initialise(gam.config);

    ok &= gam.InitialiseConfigDataBaseSignal3();
    ok &= gam.SetConfiguredDatabase(gam.configSignals);

    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();

    ok &= gam.Setup();
    return !ok;
}

bool FilterGAMTest::TestSetupWrongOutputSignalType() {
    using namespace MARTe;
    FilterGAMTestHelper gam;
    gam.SetName("Test");
    bool ok = true;

    ok &= gam.InitialiseFilterFIR();
    ok &= gam.Initialise(gam.config);

    ok &= gam.InitialiseConfigDataBaseSignal4();
    ok &= gam.SetConfiguredDatabase(gam.configSignals);

    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();

    ok &= gam.Setup();

    return !ok;
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
    ok &= config.Write("ResetInEachState", false);
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
    ok &= config.Write("ResetInEachState", false);
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
    ok &= config.Write("ResetInEachState", false);
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
    ok &= config.Write("ResetInEachState", false);
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
    ok &= config.Write("ResetInEachState", false);
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
    ok &= config.Write("ResetInEachState", false);
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
    ok &= config.Write("ResetInEachState", false);
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
    ok &= config.Write("ResetInEachState", false);
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
    ok &= config.Write("ResetInEachState", false);
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
    ok &= config.Write("ResetInEachState", false);
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
    ok &= config.Write("ResetInEachState", false);
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
    ok &= config.Write("ResetInEachState", false);
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
    ok &= config.Write("ResetInEachState", false);
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
    ok &= config.Write("ResetInEachState", false);
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

bool FilterGAMTest::TestExecuteNoFilter() {
    using namespace MARTe;
    FilterGAMTestHelper gam;
    gam.SetName("Test");
    bool ok = true;

    ok &= gam.InitialiseNoFilter();
    ok &= gam.Initialise(gam.config);

    ok &= gam.InitialiseConfigDataBaseSignal1();
    ok &= gam.SetConfiguredDatabase(gam.configSignals);

    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();

    ok &= gam.Setup();

    float32 *gamMemoryIn = static_cast<float32*>(gam.GetInputSignalsMemory());
    float32 *gamMemoryOut = static_cast<float32*>(gam.GetOutputSignalsMemory());
    //assign inputs and outputs
    for (uint32 i = 0u; i < gam.numberOfElements; i++) {
        gamMemoryIn[i] = 1;
        gamMemoryOut[i] = 0;
    }
    if (ok) {
        gam.Execute();
    }
    for (uint32 i = 0u; i < gam.numberOfElements; i++) {
        ok &= (gamMemoryOut[i] == 1.0);
    }
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

    float32 *gamMemoryIn = static_cast<float32*>(gam.GetInputSignalsMemory());
    float32 *gamMemoryOut = static_cast<float32*>(gam.GetOutputSignalsMemory());
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

bool FilterGAMTest::TestExecuteFIRConstantInputInputElements1() {
    using namespace MARTe;
    FilterGAMTestHelper gam(1, 1);
    gam.SetName("Test");
    bool ok = true;

    ok &= gam.InitialiseFilterFIR();
    ok &= gam.Initialise(gam.config);

    ok &= gam.InitialiseConfigDataBaseSignal1();
    ok &= gam.SetConfiguredDatabase(gam.configSignals);

    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();

    ok &= gam.Setup();

    float32 *gamMemoryIn = static_cast<float32*>(gam.GetInputSignalsMemory());
    float32 *gamMemoryOut = static_cast<float32*>(gam.GetOutputSignalsMemory());
    //assign inputs and outputs
    for (uint32 i = 0u; i < gam.numberOfElements; i++) {
        gamMemoryIn[i] = 1;
        gamMemoryOut[i] = 0;
    }
    if (ok) { //Samples 1
        gam.Execute();
    }
    ok &= (gamMemoryOut[0] == 0.5);
    if (ok) { //Samples 2
        gam.Execute();
    }
    ok &= (gamMemoryOut[0] == gamMemoryOut[0]);
    if (ok) { //Samples 3
        gam.Execute();
    }
    ok &= (gamMemoryOut[0] == gamMemoryOut[0]);
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

    float32 *gamMemoryIn = static_cast<float32*>(gam.GetInputSignalsMemory());
    float32 *gamMemoryOut = static_cast<float32*>(gam.GetOutputSignalsMemory());

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

bool FilterGAMTest::TestExecuteFIRRampInputInputElements1() {
    using namespace MARTe;
    FilterGAMTestHelper gam(1, 1);
    gam.SetName("Test");
    bool ok = true;

    ok &= gam.InitialiseFilterFIR();
    ok &= gam.Initialise(gam.config);

    ok &= gam.InitialiseConfigDataBaseSignal1();
    ok &= gam.SetConfiguredDatabase(gam.configSignals);

    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();

    ok &= gam.Setup();

    float32 *gamMemoryIn = static_cast<float32*>(gam.GetInputSignalsMemory());
    float32 *gamMemoryOut = static_cast<float32*>(gam.GetOutputSignalsMemory());
    float32 rampInput = 0.0;
    //assign inputs and outputs
    for (uint32 i = 0u; i < gam.numberOfSamples; i++) {
        gamMemoryIn[i] = rampInput;
        gamMemoryOut[i] = 0;
    }
    if (ok) { //sample 0
        gam.Execute();
    }
    ok &= (gamMemoryOut[0] == 0);

    rampInput += 1.0;
    //assign inputs and outputs
    for (uint32 i = 0u; i < gam.numberOfSamples; i++) {
        gamMemoryIn[i] = rampInput;
    }
    if (ok) { //sample 1
        gam.Execute();
    }
    for (uint32 i = 1u; i < gam.numberOfSamples; i++) {
        ok &= (gamMemoryOut[i] == rampInput - 0.5);
    }
    rampInput += 1.0;
    //assign inputs and outputs
    for (uint32 i = 0u; i < gam.numberOfSamples; i++) {
        gamMemoryIn[i] = rampInput;
    }
    if (ok) { //sample 2
        gam.Execute();
    }
    for (uint32 i = 1u; i < gam.numberOfSamples; i++) {
        ok &= (gamMemoryOut[i] == rampInput - 0.5);
    }
    rampInput += 1.0;
    //assign inputs and outputs
    for (uint32 i = 0u; i < gam.numberOfSamples; i++) {
        gamMemoryIn[i] = rampInput;
    }
    if (ok) { //sample 3
        gam.Execute();
    }
    for (uint32 i = 1u; i < gam.numberOfSamples; i++) {
        ok &= (gamMemoryOut[i] == rampInput - 0.5);
    }
    return ok;
}

bool FilterGAMTest::TestExecuteAVG10RampInputInputElements1() {
    using namespace MARTe;
    FilterGAMTestHelper gam(1, 1);
    gam.SetName("Test");
    uint32 nOfTestSamples = 50;
    float32 *outputRef = new float32[nOfTestSamples];
    float32 rampVal = 1.0;
    float32 accum = 1.0 / 10.0;
    for (uint32 i = 0; i < 10; i++) {
        outputRef[i] = accum;
        rampVal += 1.0;
        accum += rampVal / 10.0;
    }
    for (uint32 i = 10; i < nOfTestSamples; i++) {
        outputRef[i] = outputRef[i - 1] + 1.0;
    }
    bool ok = true;

    ok &= gam.InitialiseAVG10();
    ok &= gam.Initialise(gam.config);

    ok &= gam.InitialiseConfigDataBaseSignal1();
    ok &= gam.SetConfiguredDatabase(gam.configSignals);

    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();

    ok &= gam.Setup();

    float32 *gamMemoryIn = static_cast<float32*>(gam.GetInputSignalsMemory());
    float32 *gamMemoryOut = static_cast<float32*>(gam.GetOutputSignalsMemory());
    gamMemoryIn[0] = 0.0;
    gamMemoryOut[0] = 0;
    uint32 idxFail = 0;
    for (uint32 i = 0u; (i < nOfTestSamples) && ok; i++) {
        gamMemoryIn[0] += 1.0;
        if (ok) { //sample 0
            gam.Execute();
        }
        ok = MyIsEqual(gamMemoryOut[0], outputRef[i]);
        if (!ok) {
            idxFail = i;
        }
    }
    if (idxFail > 0) {
        ok = false;
    }
    delete[] outputRef;
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

    float32 *gamMemoryIn = static_cast<float32*>(gam.GetInputSignalsMemory());
    float32 *gamMemoryOut = static_cast<float32*>(gam.GetOutputSignalsMemory());

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

    float32 *gamMemoryIn = static_cast<float32*>(gam.GetInputSignalsMemory());
    float32 *gamMemoryOut = static_cast<float32*>(gam.GetOutputSignalsMemory());
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

bool FilterGAMTest::TestExecuteFIRRampInput2DiffCoef() {
    using namespace MARTe;
    FilterGAMTestHelper gam;
    gam.SetName("Test");
    bool ok = true;

    ok &= gam.InitialiseFilterFIR2();
    ok &= gam.Initialise(gam.config);

    ok &= gam.InitialiseConfigDataBaseSignal1();
    ok &= gam.SetConfiguredDatabase(gam.configSignals);

    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();

    ok &= gam.Setup();

    float32 *gamMemoryIn = static_cast<float32*>(gam.GetInputSignalsMemory());
    float32 *gamMemoryOut = static_cast<float32*>(gam.GetOutputSignalsMemory());
    //assign inputs and outputs
    for (uint32 i = 0u; i < gam.numberOfElements; i++) {
        gamMemoryIn[i] = i;
        gamMemoryOut[i] = 0;
    }
    if (ok) {
        gam.Execute();
    }
    ok &= (gamMemoryOut[0] == 0);
    float32 refValue = 0;
    for (uint32 i = 1u; i < gam.numberOfElements; i++) {
        refValue = (i * 0.6F) + (i - 1) * 0.4F;
        ok &= IsEqual(gamMemoryOut[i], refValue);
    }
    for (uint32 i = 0u; i < gam.numberOfElements; i++) {
        gamMemoryIn[i] = i + 10;
        //gamMemoryOut[i] = 0;
    }
    gam.Execute();
    for (uint32 i = 1u; i < gam.numberOfElements; i++) {
        refValue = ((i + 10) * 0.6) + (i + 9) * 0.4;
        ok &= IsEqual(gamMemoryOut[i], refValue);
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

    float32 *gamMemoryIn = static_cast<float32*>(gam.GetInputSignalsMemory());
    float32 *gamMemoryOut = static_cast<float32*>(gam.GetOutputSignalsMemory());
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

bool FilterGAMTest::TestExecuteIIRInputRamp1InputElement() {
    using namespace MARTe;
    FilterGAMTestHelper gam(1, 1);
    gam.SetName("Test");
    uint32 nOfIterations = 10;
    float32 *expectedOutput = new float32[nOfIterations];
    expectedOutput[0] = 0;
    expectedOutput[1] = 0.1;
    expectedOutput[2] = 0.35;
    expectedOutput[3] = 0.805;
    expectedOutput[4] = 1.1275;
    expectedOutput[5] = 1.47525;
    expectedOutput[6] = 1.836875;
    expectedOutput[7] = 2.1865125;
    expectedOutput[8] = 2.53936875;
    expectedOutput[9] = 2.893013125;
    bool ok = true;

    ok &= gam.InitialiseFilterIIR2();
    ok &= gam.Initialise(gam.config);

    ok &= gam.InitialiseConfigDataBaseSignal1();
    ok &= gam.SetConfiguredDatabase(gam.configSignals);

    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();

    ok &= gam.Setup();

    float32 *gamMemoryIn = static_cast<float32*>(gam.GetInputSignalsMemory());
    float32 *gamMemoryOut = static_cast<float32*>(gam.GetOutputSignalsMemory());
    //assign inputs and outputs
    gamMemoryIn[0] = 0;
    gamMemoryOut[0] = 0;
    for (uint32 i = 0u; i < nOfIterations && ok; i++) {
        gam.Execute();
        ok &= MyIsEqual(gamMemoryOut[0], expectedOutput[i]);
        gamMemoryIn[0] = gamMemoryIn[0] + 1.0;
    }

    delete[] expectedOutput;
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

    float32 *gamMemoryIn = static_cast<float32*>(gam.GetInputSignalsMemory());
    float32 *gamMemoryOut = static_cast<float32*>(gam.GetOutputSignalsMemory());
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
    ok &= config.Write("ResetInEachState", false);
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

    float32 *gamMemoryIn = static_cast<float32*>(gam.GetInputSignalsMemory());
    float32 *gamMemoryOut = static_cast<float32*>(gam.GetOutputSignalsMemory());
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

    float32 *gamMemoryIn = static_cast<float32*>(gam.GetInputSignalsMemory());
    float32 *gamMemoryOut = static_cast<float32*>(gam.GetOutputSignalsMemory());

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

    float32 *gamMemoryIn0 = static_cast<float32*>(gam.GetInputSignalsMemory());
    float32 *gamMemoryOut0 = static_cast<float32*>(gam.GetOutputSignalsMemory());
    float32 *gamMemoryIn1 = static_cast<float32*>(gam.GetInputSignalsMemory(1));
    float32 *gamMemoryOut1 = static_cast<float32*>(gam.GetOutputSignalsMemory(1));
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

bool FilterGAMTest::TestAlwaysResetFIR() {
    using namespace MARTe;
    FilterGAMTestHelper gam;
    gam.SetName("Test");
    bool ok = true;
    //gam.config.Write("ResetInEachState", true); set the value to true, then the gam.InitialiseFilterFIR() set the value to false,
    //however the read stops at the first ResetInEachState
    ok &= gam.InitialiseFilterFIR();
    ok &= gam.config.Write("ResetInEachState", 1);
    ok &= gam.Initialise(gam.config);
    ok &= gam.InitialiseConfigDataBaseSignal2();
    ok &= gam.SetConfiguredDatabase(gam.configSignals);

    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();

    ok &= gam.Setup();

    float32 *gamMemoryIn0 = static_cast<float32*>(gam.GetInputSignalsMemory());
    float32 *gamMemoryOut0 = static_cast<float32*>(gam.GetOutputSignalsMemory());
    float32 *gamMemoryIn1 = static_cast<float32*>(gam.GetInputSignalsMemory(1));
    float32 *gamMemoryOut1 = static_cast<float32*>(gam.GetOutputSignalsMemory(1));
    //assign inputs and outputs
    for (uint32 i = 0u; i < gam.numberOfElements; i++) {
        gamMemoryIn0[i] = i;
        gamMemoryOut0[i] = 0;
        gamMemoryIn1[i] = 1;
        gamMemoryOut1[i] = 0;
    }
    if (ok) {
        gam.PrepareNextState("", "A");
        gam.Execute();
    }
    ok &= (gamMemoryOut0[0] == 0);
    ok &= (gamMemoryOut1[0] == 0.5);
    for (uint32 i = 1u; i < gam.numberOfElements; i++) {
        ok &= (gamMemoryOut0[i] == i - 0.5);
        ok &= (gamMemoryOut1[i] == 1);
    }
    for (uint32 i = 0u; i < gam.numberOfElements; i++) {
        gamMemoryIn0[i] = i;
        gamMemoryIn1[i] = 1;
        //gamMemoryOut[i] = 0;
    }
    gam.PrepareNextState("A", "B");
    gam.Execute();
    ok &= (gamMemoryOut0[0] == 0);
    ok &= (gamMemoryOut1[0] == 0.5);
    for (uint32 i = 1u; i < gam.numberOfElements; i++) {
        ok &= (gamMemoryOut0[i] == i - 0.5);
        ok &= (gamMemoryOut1[i] == 1);
    }
    return ok;
}

bool FilterGAMTest::TestAlwaysResetIIR() {
    using namespace MARTe;
    FilterGAMTestHelper gam;
    gam.SetName("Test");
    bool ok = true;
    //gam.config.Write("ResetInEachState", true); set the value to true, then the gam.InitialiseFilterFIR() set the value to false,
    //however the read stops at the first ResetInEachState
    ok &= gam.InitialiseFilterIIR();
    ok &= gam.config.Write("ResetInEachState", true);
    ok &= gam.Initialise(gam.config);
    ok &= gam.InitialiseConfigDataBaseSignal2();
    ok &= gam.SetConfiguredDatabase(gam.configSignals);

    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();

    ok &= gam.Setup();

    float32 *gamMemoryIn0 = static_cast<float32*>(gam.GetInputSignalsMemory());
    float32 *gamMemoryOut0 = static_cast<float32*>(gam.GetOutputSignalsMemory());
    float32 *gamMemoryIn1 = static_cast<float32*>(gam.GetInputSignalsMemory(1));
    float32 *gamMemoryOut1 = static_cast<float32*>(gam.GetOutputSignalsMemory(1));
    //assign inputs and outputs
    for (uint32 i = 0u; i < gam.numberOfElements; i++) {
        gamMemoryIn0[i] = 1.0F;
        gamMemoryOut0[i] = 0;
        gamMemoryIn1[i] = 1.0F;
        gamMemoryOut1[i] = 0;
    }
    if (ok) {
        gam.PrepareNextState("", "A");
        gam.Execute();
    }
    for (uint32 i = 1u; i < gam.numberOfElements; i++) {
        ok &= (gamMemoryOut0[i] == i + 1.0F);
        ok &= (gamMemoryOut1[i] == i + 1.0F);
    }
    for (uint32 i = 0u; i < gam.numberOfElements; i++) {
        gamMemoryIn0[i] = 1;
        gamMemoryIn1[i] = 1;
        //gamMemoryOut[i] = 0;
    }
    gam.PrepareNextState("A", "B");
    gam.Execute();
    for (uint32 i = 1u; i < gam.numberOfElements; i++) {
        ok &= (gamMemoryOut0[i] == i + 1.0F);
        ok &= (gamMemoryOut1[i] == i + 1.0F);
    }
    return ok;
}

bool FilterGAMTest::TestAlwaysResetMemoryNotInt() {
    using namespace MARTe;
    FilterGAMTestHelper gam;
    gam.SetName("Test");
    bool ok = true;
    //gam.config.Write("ResetInEachState", true); set the value to true, then the gam.InitialiseFilterFIR() set the value to false,
    //however the read stops at the first ResetInEachState
    ok &= gam.InitialiseFilterFIR();
    ok &= gam.config.Write("ResetInEachState", true);
    ok &= gam.Initialise(gam.config);
    ok &= gam.InitialiseConfigDataBaseSignal2();
    ok &= gam.SetConfiguredDatabase(gam.configSignals);

    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();

    float32 *gamMemoryIn0 = static_cast<float32*>(gam.GetInputSignalsMemory());
    float32 *gamMemoryOut0 = static_cast<float32*>(gam.GetOutputSignalsMemory());
    float32 *gamMemoryIn1 = static_cast<float32*>(gam.GetInputSignalsMemory(1));
    float32 *gamMemoryOut1 = static_cast<float32*>(gam.GetOutputSignalsMemory(1));
    //assign inputs and outputs
    for (uint32 i = 0u; i < gam.numberOfElements; i++) {
        gamMemoryIn0[i] = i;
        gamMemoryOut0[i] = 0;
        gamMemoryIn1[i] = 1;
        gamMemoryOut1[i] = 0;
    }
    if (ok) {
        ok &= !gam.PrepareNextState("", "A");

    }
    return ok;
}

bool FilterGAMTest::TestResetOnlyWhenRequired() {
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

    float32 *gamMemoryIn0 = static_cast<float32*>(gam.GetInputSignalsMemory());
    float32 *gamMemoryOut0 = static_cast<float32*>(gam.GetOutputSignalsMemory());
    float32 *gamMemoryIn1 = static_cast<float32*>(gam.GetInputSignalsMemory(1));
    float32 *gamMemoryOut1 = static_cast<float32*>(gam.GetOutputSignalsMemory(1));
    //assign inputs and outputs
    for (uint32 i = 0u; i < gam.numberOfElements; i++) {
        gamMemoryIn0[i] = i;
        gamMemoryOut0[i] = 0;
        gamMemoryIn1[i] = 1;
        gamMemoryOut1[i] = 0;
    }
    if (ok) {
        gam.PrepareNextState("", "A");
        gam.Execute();
    }
    ok &= (gamMemoryOut0[0] == 0);
    ok &= (gamMemoryOut1[0] == 0.5);
    for (uint32 i = 1u; i < gam.numberOfElements; i++) {
        ok &= (gamMemoryOut0[i] == i - 0.5);
        ok &= (gamMemoryOut1[i] == 1);
    }
    for (uint32 i = 0u; i < gam.numberOfElements; i++) {
        gamMemoryIn0[i] = i;
        gamMemoryIn1[i] = 1;
        //gamMemoryOut[i] = 0;
    }
    gam.PrepareNextState("A", "B");
    gam.Execute();
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

bool FilterGAMTest::TestResetOnlyWhenRequired2() {
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

    float32 *gamMemoryIn0 = static_cast<float32*>(gam.GetInputSignalsMemory());
    float32 *gamMemoryOut0 = static_cast<float32*>(gam.GetOutputSignalsMemory());
    float32 *gamMemoryIn1 = static_cast<float32*>(gam.GetInputSignalsMemory(1));
    float32 *gamMemoryOut1 = static_cast<float32*>(gam.GetOutputSignalsMemory(1));
    //assign inputs and outputs
    for (uint32 i = 0u; i < gam.numberOfElements; i++) {
        gamMemoryIn0[i] = i;
        gamMemoryOut0[i] = 0;
        gamMemoryIn1[i] = 1;
        gamMemoryOut1[i] = 0;
    }
    if (ok) {
        gam.PrepareNextState("", "A");
        gam.Execute();
    }
    ok &= (gamMemoryOut0[0] == 0);
    ok &= (gamMemoryOut1[0] == 0.5);
    for (uint32 i = 1u; i < gam.numberOfElements; i++) {
        ok &= (gamMemoryOut0[i] == i - 0.5);
        ok &= (gamMemoryOut1[i] == 1);
    }

    for (uint32 i = 0u; i < gam.numberOfElements; i++) {
        gamMemoryIn0[i] = i;
        gamMemoryIn1[i] = 1;
        //gamMemoryOut[i] = 0;
    }
    gam.PrepareNextState("B", "C");
    gam.Execute();
    ok &= (gamMemoryOut0[0] == 0);
    ok &= (gamMemoryOut1[0] == 0.5);
    for (uint32 i = 1u; i < gam.numberOfElements; i++) {
        ok &= (gamMemoryOut0[i] == i - 0.5);
        ok &= (gamMemoryOut1[i] == 1);
    }
    return ok;
}

bool FilterGAMTest::TestResetOnlyWhenRequired3() {
    using namespace MARTe;
    FilterGAMTestHelper gam;
    gam.SetName("Test");
    bool ok = true;

    ok &= gam.InitialiseFilterIIR();
    ok &= gam.Initialise(gam.config);
    ok &= gam.InitialiseConfigDataBaseSignal2();
    ok &= gam.SetConfiguredDatabase(gam.configSignals);

    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();

    ok &= gam.Setup();

    float32 *gamMemoryIn0 = static_cast<float32*>(gam.GetInputSignalsMemory());
    float32 *gamMemoryOut0 = static_cast<float32*>(gam.GetOutputSignalsMemory());
    float32 *gamMemoryIn1 = static_cast<float32*>(gam.GetInputSignalsMemory(1));
    float32 *gamMemoryOut1 = static_cast<float32*>(gam.GetOutputSignalsMemory(1));
    //assign inputs and outputs
    for (uint32 i = 0u; i < gam.numberOfElements; i++) {
        gamMemoryIn0[i] = 1;
        gamMemoryOut0[i] = 0;
        gamMemoryIn1[i] = 1;
        gamMemoryOut1[i] = 0;
    }
    if (ok) {
        gam.PrepareNextState("", "A");
        gam.Execute();
    }
    //Check output
    for (uint32 i = 0u; i < gam.numberOfElements; i++) {
        ok &= (gamMemoryOut0[i] == i + 1.0F);
        ok &= (gamMemoryOut1[i] == i + 1.0F);
    }

    for (uint32 i = 0u; i < gam.numberOfElements; i++) {
        gamMemoryIn0[i] = 1;
        gamMemoryIn1[i] = 1;
        //gamMemoryOut[i] = 0;
    }
    gam.PrepareNextState("B", "C");
    gam.Execute();
    for (uint32 i = 0u; i < gam.numberOfElements; i++) {
        ok &= (gamMemoryOut0[i] == i + 1.0F);
        ok &= (gamMemoryOut1[i] == i + 1.0F);
    }
    return ok;
}

bool FilterGAMTest::TestResetOnlyWhenRequiredMemoryNotInit() {
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

    float32 *gamMemoryIn0 = static_cast<float32*>(gam.GetInputSignalsMemory());
    float32 *gamMemoryOut0 = static_cast<float32*>(gam.GetOutputSignalsMemory());
    float32 *gamMemoryIn1 = static_cast<float32*>(gam.GetInputSignalsMemory(1));
    float32 *gamMemoryOut1 = static_cast<float32*>(gam.GetOutputSignalsMemory(1));
    //assign inputs and outputs
    for (uint32 i = 0u; i < gam.numberOfElements; i++) {
        gamMemoryIn0[i] = i;
        gamMemoryOut0[i] = 0;
        gamMemoryIn1[i] = 1;
        gamMemoryOut1[i] = 0;
    }
    if (ok) {
        ok &= gam.PrepareNextState("", "A");
        ok &= !gam.PrepareNextState("B", "C");
    }
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

