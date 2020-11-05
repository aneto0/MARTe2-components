/**
 * @file PIDGAMTest.cpp
 * @brief Source file for class PIDGAMTest
 * @date 22/10/2017
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
 * the class PIDGAMTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include "stdio.h"
/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "PIDGAMTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace {

}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {
class PIDGAMTestHelper: public PIDGAM {
public:
    CLASS_REGISTER_DECLARATION()PIDGAMTestHelper(float64 kpDef = 1.0,
            float64 kiDef = 1.2,
            float64 kdDef = 1.3,
            float64 sampleTimeDef = 0.001,
            float64 maxOutputDef = 0x1.FFFFFFFFFFFFFp1023,
            float64 minOutputDef = -0x1.FFFFFFFFFFFFFp1023,
            uint32 noe = 1) {
        kp = kpDef;
        ki = kiDef;
        kd = kdDef;
        maxOutput = maxOutputDef;
        minOutput = minOutputDef;
        sampleTime = sampleTimeDef;
        numberOfElements = noe;
    }

    void *GetInputSignalsMemory() {
        return GAM::GetInputSignalsMemory();
    }
    void *GetOutputSignalsMemory() {
        return GAM::GetOutputSignalsMemory();
    }
    void *GetInputSignalsMemory(uint32 idx) {
        return GAM::GetInputSignalMemory(idx);
    }

    void *GetOutputSignalsMemory(uint32 idx) {
        return GAM::GetOutputSignalMemory(idx);
    }

    bool HelperInitialise() {
        bool ok;
        ok = config.Write("Kp", kp);
        ok = config.Write("Ki", ki);
        ok = config.Write("Kd", kd);
        ok &= config.Write("SampleTime", sampleTime);
        ok &= config.Write("MaxOutput", maxOutput);
        ok &= config.Write("MinOutput", minOutput);
        return ok;
    }
    bool HelperSetup1() {
        bool ok;
        uint32 numberOfInputSignals = 1;
        uint32 byteSizePerSignal = numberOfElements * sizeof(float64);
        uint32 totalInputBytes = byteSizePerSignal*numberOfInputSignals;
        uint32 totalOutputBytes = byteSizePerSignal;

        ok = configSignals.CreateAbsolute("Signals.InputSignals");
        ok &= configSignals.CreateRelative("0");
        ok &= configSignals.Write("NumberOfElements", 1);
        ok &= configSignals.Write("DataSource", "Reference");
        ok &= configSignals.Write("NumberOfDimensions", 1);
        ok &= configSignals.Write("Type", "float64");
        ok &= configSignals.Write("ByteSize", byteSizePerSignal);
        ok &= configSignals.MoveAbsolute("Signals.InputSignals");
        ok &= configSignals.Write("ByteSize", totalInputBytes);

        ok &= configSignals.MoveToRoot();
        ok &= configSignals.CreateAbsolute("Memory.InputSignals");
        ok &= configSignals.CreateRelative("0");
        ok &= configSignals.Write("DataSource", "Reference");
        ok &= configSignals.CreateRelative("Signals");
        ok &= configSignals.CreateRelative("0");
        ok &= configSignals.Write("Samples", 1);

        ok &= configSignals.MoveToRoot();
        ok &= configSignals.CreateAbsolute("Signals.OutputSignals");
        ok &= configSignals.CreateRelative("0");
        ok &= configSignals.Write("NumberOfElements", 1);
        ok &= configSignals.Write("DataSource", "Reference");
        ok &= configSignals.Write("NumberOfDimensions", 1);
        ok &= configSignals.Write("Type", "float64");
        ok &= configSignals.Write("ByteSize", byteSizePerSignal);
        ok &= configSignals.MoveAbsolute("Signals.OutputSignals");
        ok &= configSignals.Write("ByteSize", totalOutputBytes);

        ok &= configSignals.MoveToRoot();
        ok &= configSignals.CreateAbsolute("Memory.OutputSignals");
        ok &= configSignals.CreateRelative("0");
        ok &= configSignals.Write("DataSource", "Reference");
        ok &= configSignals.CreateRelative("Signals");
        ok &= configSignals.CreateRelative("0");
        ok &= configSignals.Write("Samples", 1);

        ok &= configSignals.MoveToRoot();
        return ok;
    }

    bool HelperSetup2() {
        bool ok;
        uint32 numberOfInputSignals = 2;
        uint32 byteSizePerSignal = numberOfElements * sizeof(float64);
        uint32 totalInputBytes = byteSizePerSignal*numberOfInputSignals;
        uint32 totalOutputBytes = byteSizePerSignal;

        ok = configSignals.CreateAbsolute("Signals.InputSignals");
        ok &= configSignals.CreateRelative("0");
        ok &= configSignals.Write("NumberOfElements", 1);
        ok &= configSignals.Write("DataSource", "Reference");
        ok &= configSignals.Write("NumberOfDimensions", 1);
        ok &= configSignals.Write("Type", "float64");
        ok &= configSignals.Write("ByteSize", byteSizePerSignal);
        ok &= configSignals.MoveAbsolute("Signals.InputSignals");
        ok &= configSignals.CreateRelative("1");
        ok &= configSignals.Write("NumberOfElements", 1);
        ok &= configSignals.Write("DataSource", "Measurement");
        ok &= configSignals.Write("NumberOfDimensions", 1);
        ok &= configSignals.Write("Type", "float64");
        ok &= configSignals.Write("ByteSize", byteSizePerSignal);
        ok &= configSignals.MoveToAncestor(1u);
        ok &= configSignals.Write("ByteSize", totalInputBytes);

        ok &= configSignals.MoveToRoot();
        ok &= configSignals.CreateAbsolute("Memory.InputSignals");
        ok &= configSignals.CreateRelative("0");
        ok &= configSignals.Write("DataSource", "Reference");
        ok &= configSignals.CreateRelative("Signals");
        ok &= configSignals.CreateRelative("0");
        ok &= configSignals.Write("Samples", 1);
        ok &= configSignals.MoveToAncestor(3u);
        ok &= configSignals.CreateRelative("1");
        ok &= configSignals.Write("DataSource", "Measurement");
        ok &= configSignals.CreateRelative("Signals");
        ok &= configSignals.CreateRelative("1");
        ok &= configSignals.Write("Samples", 1);

        ok &= configSignals.MoveToRoot();
        ok &= configSignals.CreateAbsolute("Signals.OutputSignals");
        ok &= configSignals.CreateRelative("0");
        ok &= configSignals.Write("NumberOfElements", 1);
        ok &= configSignals.Write("DataSource", "Reference");
        ok &= configSignals.Write("NumberOfDimensions", 1);
        ok &= configSignals.Write("Type", "float64");
        ok &= configSignals.Write("ByteSize", byteSizePerSignal);
        ok &= configSignals.MoveAbsolute("Signals.OutputSignals");
        ok &= configSignals.Write("ByteSize", totalOutputBytes);

        ok &= configSignals.MoveToRoot();
        ok &= configSignals.CreateAbsolute("Memory.OutputSignals");
        ok &= configSignals.CreateRelative("0");
        ok &= configSignals.Write("DataSource", "Reference");
        ok &= configSignals.CreateRelative("Signals");
        ok &= configSignals.CreateRelative("0");
        ok &= configSignals.Write("Samples", 1);

        ok &= configSignals.MoveToRoot();
        return ok;
    }

    bool IsEqualLargerMargins(float64 f1,
                              float64 f2) {
        float64 *min = reinterpret_cast<float64*>(const_cast<uint64*>(&EPSILON_FLOAT64));
        float64 minLarger = *min * 2;
        return ((f1 - f2) < (minLarger)) && ((f1 - f2) > -(minLarger));
    }

    ConfigurationDatabase config;
    ConfigurationDatabase configSignals;
private :
    float64 kp;
    float64 ki;
    float64 kd;
    float64 sampleTime;
    float64 maxOutput;
    float64 minOutput;
    uint32 numberOfElements;

};
CLASS_REGISTER(PIDGAMTestHelper, "1.0")

PIDGAMTest::PIDGAMTest() {
//Auto-generated constructor stub for PIDGAMTest

//TODO Verify if manual additions are needed here
}

PIDGAMTest::~PIDGAMTest() {
//Auto-generated destructor stub for PIDGAMTest

//TODO Verify if manual additions are needed here
}

bool PIDGAMTest::TestInitialiseMissingKpKiKd() {
    PIDGAM gam;
    bool ret;
    ConfigurationDatabase config;
    ret = !gam.Initialise(config);
    return ret;
}

bool PIDGAMTest::TestInitialiseMissingSampleTime() {
    PIDGAM gam;
    bool ret;
    ConfigurationDatabase config;
    float64 kp = 1.1;
    config.Write("Kp", kp);
    ret = !gam.Initialise(config);
    return ret;
}

bool PIDGAMTest::TestInitialiseWrongSampleTime() {
    PIDGAM gam;
    bool ret;
    ConfigurationDatabase config;
    float64 kp = 1.1;
    config.Write("Kp", kp);
    float64 sampleTime = 0.0;
    config.Write("SampleTime", sampleTime);
    ret = !gam.Initialise(config);
    return ret;
}

bool PIDGAMTest::TestInitialiseMissingUpperSaturationLimit() {
    PIDGAM gam;
    bool ret;
    ConfigurationDatabase config;
    float64 kp = 1.1;
    config.Write("Kp", kp);
    float64 sampleTime = 0.001;
    config.Write("SampleTime", sampleTime);
    ret = gam.Initialise(config);
    return ret;
}

bool PIDGAMTest::TestInitialiseMissingLowerSaturationLimit() {
    PIDGAM gam;
    bool ret;
    ConfigurationDatabase config;
    float64 kp = 1.1;
    config.Write("Kp", kp);
    float64 sampleTime = 0.001;
    config.Write("SampleTime", sampleTime);
    float64 maxOutput = 1.5;
    config.Write("MaxOutput", maxOutput);
    ret = gam.Initialise(config);
    return ret;
}

bool PIDGAMTest::TestInitialiseWrongSaturationLimits() {
    PIDGAM gam;
    bool ret;
    ConfigurationDatabase config;
    float64 kp = 1.1;
    config.Write("Kp", kp);
    float64 sampleTime = 0.001;
    config.Write("SampleTime", sampleTime);
    float64 maxOutput = 1.5;
    config.Write("MaxOutput", maxOutput);
    float64 minOutput = 1.7;
    config.Write("MinOutput", minOutput);
    ret = !gam.Initialise(config);
    return ret;
}

bool PIDGAMTest::TestInitialiseWrongSaturationLimits2() {
    PIDGAM gam;
    bool ret;
    ConfigurationDatabase config;
    float64 kp = 1.1;
    config.Write("Kp", kp);
    float64 sampleTime = 0.001;
    config.Write("SampleTime", sampleTime);
    float64 maxOutput = 1.5;
    config.Write("MaxOutput", maxOutput);
    float64 minOutput = 1.5;
    config.Write("MinOutput", minOutput);
    ret = !gam.Initialise(config);
    return ret;
}

bool PIDGAMTest::TestInitialise() {
    bool ret;
    PIDGAMTestHelper gam;
    ret = gam.HelperInitialise();
    ret &= gam.Initialise(gam.config);
    return ret;
}

bool PIDGAMTest::TestSetupWrongNumberOfInputs() {
    bool ret;
    PIDGAMTestHelper gam;
    ret = gam.HelperInitialise();
    ret &= gam.Initialise(gam.config);
    ret &= !gam.Setup();
    return ret;
}

bool PIDGAMTest::TestSetupWrongNumberOfInputs2() {
    bool ret;
    PIDGAMTestHelper gam;
    ret = gam.HelperInitialise();
    ret &= gam.Initialise(gam.config);

    ConfigurationDatabase configSignals;
    ret &= configSignals.CreateAbsolute("Signals.InputSignals");
    ret &= configSignals.CreateRelative("0");

    ret &= configSignals.MoveAbsolute("Signals.InputSignals");
    ret &= configSignals.CreateRelative("1");

    ret &= configSignals.MoveAbsolute("Signals.InputSignals");
    ret &= configSignals.CreateRelative("2");

    ret &= configSignals.MoveToAncestor(1u);
    ret &= configSignals.Write("ByteSize", 16);

    ret &= configSignals.MoveToRoot();

    /*
     //example how to print a ConfigurationDatabase
     StreamString a;
     printf("size of a %llu\n", a.Size());
     printf("%d\n", a.Printf("%!", configSignals));
     printf("size of a %llu\n", a.Size());
     printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
     printf("%s\n", a.Buffer());
     printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
     */

    ret &= gam.SetConfiguredDatabase(configSignals);
    if (ret) {
        ret &= !gam.Setup();
    }
    return ret;
}

bool PIDGAMTest::TestSetupWrongNumberOfOutputs() {
    bool ret;
    PIDGAMTestHelper gam;
    ret = gam.HelperInitialise();
    ret &= gam.Initialise(gam.config);

    ConfigurationDatabase configSignals;
    ret &= configSignals.CreateAbsolute("Signals.InputSignals");
    ret &= configSignals.CreateRelative("0");

    ret &= configSignals.MoveAbsolute("Signals.InputSignals");
    ret &= configSignals.CreateRelative("1");

    ret &= configSignals.MoveToAncestor(1u);
    ret &= configSignals.Write("ByteSize", 16);

    ret &= configSignals.MoveToRoot();

    ret &= gam.SetConfiguredDatabase(configSignals);
    if (ret) {
        ret &= !gam.Setup();
    }
    return ret;
}

bool PIDGAMTest::TestSetupWrongNumberOfOutputs2() {
    bool ret;
    PIDGAMTestHelper gam;
    ret = gam.HelperInitialise();
    ret &= gam.Initialise(gam.config);

    ConfigurationDatabase configSignals;
    ret &= configSignals.CreateAbsolute("Signals.InputSignals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.MoveAbsolute("Signals.InputSignals");
    ret &= configSignals.CreateRelative("1");
    ret &= configSignals.MoveToAncestor(1u);
    ret &= configSignals.Write("ByteSize", 16);

    ret &= configSignals.MoveToRoot();
    ret &= configSignals.CreateAbsolute("Signals.OutputSignals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.MoveAbsolute("Signals.OutputSignals");
    ret &= configSignals.CreateRelative("1");
    ret &= configSignals.MoveToAncestor(1u);
    ret &= configSignals.Write("ByteSize", 16);

    ret &= configSignals.MoveToRoot();

    ret &= gam.SetConfiguredDatabase(configSignals);
    if (ret) {
        ret &= !gam.Setup();
    }
    return ret;
}

bool PIDGAMTest::TestSetupNoInputElementsReference() {
    bool ret;
    PIDGAMTestHelper gam;
    ret = gam.HelperInitialise();
    ret &= gam.Initialise(gam.config);

    ConfigurationDatabase configSignals;
    ret &= configSignals.CreateAbsolute("Signals.InputSignals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.MoveAbsolute("Signals.InputSignals");
    ret &= configSignals.CreateRelative("1");
    ret &= configSignals.MoveToAncestor(1u);
    ret &= configSignals.Write("ByteSize", 16);

    ret &= configSignals.MoveToRoot();
    ret &= configSignals.CreateAbsolute("Signals.OutputSignals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.MoveAbsolute("Signals.OutputSignals");
    ret &= configSignals.Write("ByteSize", 16);

    ret &= configSignals.MoveToRoot();

    ret &= gam.SetConfiguredDatabase(configSignals);
    if (ret) {
        ret &= !gam.Setup();
    }
    return ret;
}

bool PIDGAMTest::TestSetupWrongInputElementsReference() {
    bool ret;
    PIDGAMTestHelper gam;
    ret = gam.HelperInitialise();
    ret &= gam.Initialise(gam.config);

    ConfigurationDatabase configSignals;
    ret &= configSignals.CreateAbsolute("Signals.InputSignals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.Write("NumberOfElements", 0); //here is the wrong value
    ret &= configSignals.MoveAbsolute("Signals.InputSignals");
    ret &= configSignals.CreateRelative("1");
    ret &= configSignals.MoveToAncestor(1u);
    ret &= configSignals.Write("ByteSize", 16);

    ret &= configSignals.MoveToRoot();
    ret &= configSignals.CreateAbsolute("Signals.OutputSignals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.MoveAbsolute("Signals.OutputSignals");
    ret &= configSignals.Write("ByteSize", 16);

    ret &= configSignals.MoveToRoot();

    ret &= gam.SetConfiguredDatabase(configSignals);
    if (ret) {
        ret &= !gam.Setup();
    }
    return ret;
}

bool PIDGAMTest::TestSetupNoInputElementsMeasurement() {
    bool ret;
    PIDGAMTestHelper gam;
    ret = gam.HelperInitialise();
    ret &= gam.Initialise(gam.config);

    ConfigurationDatabase configSignals;
    ret &= configSignals.CreateAbsolute("Signals.InputSignals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.Write("NumberOfElements", 1);
    ret &= configSignals.MoveAbsolute("Signals.InputSignals");
    ret &= configSignals.CreateRelative("1");
    ret &= configSignals.MoveToAncestor(1u);
    ret &= configSignals.Write("ByteSize", 16);

    ret &= configSignals.MoveToRoot();
    ret &= configSignals.CreateAbsolute("Signals.OutputSignals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.MoveAbsolute("Signals.OutputSignals");
    ret &= configSignals.Write("ByteSize", 16);

    ret &= configSignals.MoveToRoot();

    ret &= gam.SetConfiguredDatabase(configSignals);
    if (ret) {
        ret &= !gam.Setup();
    }
    return ret;
}

bool PIDGAMTest::TestSetupWrongInputElementsMeasurement() {
    bool ret;
    PIDGAMTestHelper gam;
    ret = gam.HelperInitialise();
    ret &= gam.Initialise(gam.config);

    ConfigurationDatabase configSignals;
    ret &= configSignals.CreateAbsolute("Signals.InputSignals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.Write("NumberOfElements", 1);
    ret &= configSignals.MoveAbsolute("Signals.InputSignals");
    ret &= configSignals.CreateRelative("1");
    ret &= configSignals.Write("NumberOfElements", 0); //here is the wrong value

    ret &= configSignals.MoveToAncestor(1u);
    ret &= configSignals.Write("ByteSize", 16);

    ret &= configSignals.MoveToRoot();
    ret &= configSignals.CreateAbsolute("Signals.OutputSignals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.MoveAbsolute("Signals.OutputSignals");
    ret &= configSignals.Write("ByteSize", 16);

    ret &= configSignals.MoveToRoot();

    ret &= gam.SetConfiguredDatabase(configSignals);
    if (ret) {
        ret &= !gam.Setup();
    }
    return ret;
}

bool PIDGAMTest::TestSetupNoOutputElements() {
    bool ret;
    PIDGAMTestHelper gam;
    ret = gam.HelperInitialise();
    ret &= gam.Initialise(gam.config);

    ConfigurationDatabase configSignals;
    ret &= configSignals.CreateAbsolute("Signals.InputSignals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.Write("NumberOfElements", 1);
    ret &= configSignals.MoveAbsolute("Signals.InputSignals");
    ret &= configSignals.CreateRelative("1");
    ret &= configSignals.Write("NumberOfElements", 1);

    ret &= configSignals.MoveToAncestor(1u);
    ret &= configSignals.Write("ByteSize", 16);

    ret &= configSignals.MoveToRoot();
    ret &= configSignals.CreateAbsolute("Signals.OutputSignals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.MoveAbsolute("Signals.OutputSignals");
    ret &= configSignals.Write("ByteSize", 16);

    ret &= configSignals.MoveToRoot();

    ret &= gam.SetConfiguredDatabase(configSignals);
    if (ret) {
        ret &= !gam.Setup();
    }
    return ret;
}

bool PIDGAMTest::TestSetupWrongOutputElements() {
    bool ret;
    PIDGAMTestHelper gam;
    ret = gam.HelperInitialise();
    ret &= gam.Initialise(gam.config);

    ConfigurationDatabase configSignals;
    ret &= configSignals.CreateAbsolute("Signals.InputSignals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.Write("NumberOfElements", 1);
    ret &= configSignals.MoveAbsolute("Signals.InputSignals");
    ret &= configSignals.CreateRelative("1");
    ret &= configSignals.Write("NumberOfElements", 1);

    ret &= configSignals.MoveToAncestor(1u);
    ret &= configSignals.Write("ByteSize", 16);

    ret &= configSignals.MoveToRoot();
    ret &= configSignals.CreateAbsolute("Signals.OutputSignals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.Write("NumberOfElements", 0); //wrong value
    ret &= configSignals.MoveAbsolute("Signals.OutputSignals");
    ret &= configSignals.Write("ByteSize", 16);

    ret &= configSignals.MoveToRoot();
    ret &= gam.SetConfiguredDatabase(configSignals);
    if (ret) {
        ret &= !gam.Setup();
    }
    return ret;
}

bool PIDGAMTest::TestSetupNoInputSamplesReference() {
    bool ret;
    PIDGAMTestHelper gam;
    ret = gam.HelperInitialise();
    ret &= gam.Initialise(gam.config);

    ConfigurationDatabase configSignals;
    ret &= configSignals.CreateAbsolute("Signals.InputSignals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.Write("NumberOfElements", 1);
    ret &= configSignals.MoveAbsolute("Signals.InputSignals");
    ret &= configSignals.CreateRelative("1");
    ret &= configSignals.Write("NumberOfElements", 1);

    ret &= configSignals.MoveToAncestor(1u);
    ret &= configSignals.Write("ByteSize", 16);

    ret &= configSignals.MoveToRoot();
    ret &= configSignals.CreateAbsolute("Signals.OutputSignals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.Write("NumberOfElements", 1);
    ret &= configSignals.MoveAbsolute("Signals.OutputSignals");
    ret &= configSignals.Write("ByteSize", 16);

    ret &= configSignals.MoveToRoot();

    ret &= gam.SetConfiguredDatabase(configSignals);
    if (ret) {
        ret &= !gam.Setup();
    }
    return ret;
}

bool PIDGAMTest::TestSetupWrongInputSamplesReference() {
    bool ret;
    PIDGAMTestHelper gam;
    ret = gam.HelperInitialise();
    ret &= gam.Initialise(gam.config);

    ConfigurationDatabase configSignals;
    ret &= configSignals.CreateAbsolute("Signals.InputSignals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.Write("NumberOfElements", 1);
    ret &= configSignals.Write("DataSource", "TestDataSource");
    ret &= configSignals.MoveAbsolute("Signals.InputSignals");
    ret &= configSignals.CreateRelative("1");
    ret &= configSignals.Write("NumberOfElements", 1);
    ret &= configSignals.MoveToAncestor(1u);
    ret &= configSignals.Write("ByteSize", 16);

    ret &= configSignals.MoveToRoot();
    ret &= configSignals.CreateAbsolute("Memory.InputSignals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.Write("DataSource", "TestDataSource");
    ret &= configSignals.CreateRelative("Signals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.Write("Samples", 12);

    ret &= configSignals.MoveToRoot();
    ret &= configSignals.CreateAbsolute("Signals.OutputSignals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.Write("NumberOfElements", 1);
    ret &= configSignals.MoveAbsolute("Signals.OutputSignals");
    ret &= configSignals.Write("ByteSize", 16);

    ret &= configSignals.MoveToRoot();

    ret &= gam.SetConfiguredDatabase(configSignals);
    if (ret) {
        ret &= !gam.Setup();
    }
    return ret;
}

bool PIDGAMTest::TestSetupNoInputSamplesMeasurement() {
    bool ret;
    PIDGAMTestHelper gam;
    ret = gam.HelperInitialise();
    ret &= gam.Initialise(gam.config);

    ConfigurationDatabase configSignals;
    ret &= configSignals.CreateAbsolute("Signals.InputSignals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.Write("NumberOfElements", 1);
    ret &= configSignals.Write("DataSource", "TestDataSource");
    ret &= configSignals.MoveAbsolute("Signals.InputSignals");
    ret &= configSignals.CreateRelative("1");
    ret &= configSignals.Write("NumberOfElements", 1);
    ret &= configSignals.MoveToAncestor(1u);
    ret &= configSignals.Write("ByteSize", 16);

    ret &= configSignals.MoveToRoot();
    ret &= configSignals.CreateAbsolute("Memory.InputSignals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.Write("DataSource", "TestDataSource");
    ret &= configSignals.CreateRelative("Signals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.Write("Samples", 1);

    ret &= configSignals.MoveToRoot();
    ret &= configSignals.CreateAbsolute("Signals.OutputSignals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.Write("NumberOfElements", 1);
    ret &= configSignals.MoveAbsolute("Signals.OutputSignals");
    ret &= configSignals.Write("ByteSize", 16);

    ret &= configSignals.MoveToRoot();

    ret &= gam.SetConfiguredDatabase(configSignals);
    if (ret) {
        ret &= !gam.Setup();
    }
    return ret;
}

bool PIDGAMTest::TestSetupWrongInputSamplesMeasurement() {
    bool ret;
    PIDGAMTestHelper gam;
    ret = gam.HelperInitialise();
    ret &= gam.Initialise(gam.config);

    ConfigurationDatabase configSignals;
    ret &= configSignals.CreateAbsolute("Signals.InputSignals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.Write("NumberOfElements", 1);
    ret &= configSignals.Write("DataSource", "TestDataSource");
    ret &= configSignals.MoveAbsolute("Signals.InputSignals");
    ret &= configSignals.CreateRelative("1");
    ret &= configSignals.Write("NumberOfElements", 1);
    ret &= configSignals.Write("DataSource", "TestDataSource1");
    ret &= configSignals.MoveToAncestor(1u);
    ret &= configSignals.Write("ByteSize", 16);

    ret &= configSignals.MoveToRoot();
    ret &= configSignals.CreateAbsolute("Memory.InputSignals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.Write("DataSource", "TestDataSource");
    ret &= configSignals.CreateRelative("Signals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.Write("Samples", 1);
    ret &= configSignals.MoveToAncestor(3u);
    ret &= configSignals.CreateRelative("1");
    ret &= configSignals.Write("DataSource", "TestDataSource1");
    ret &= configSignals.CreateRelative("Signals");
    ret &= configSignals.CreateRelative("1");
    ret &= configSignals.Write("Samples", 23);

    ret &= configSignals.MoveToRoot();
    ret &= configSignals.CreateAbsolute("Signals.OutputSignals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.Write("NumberOfElements", 1);
    ret &= configSignals.MoveAbsolute("Signals.OutputSignals");
    ret &= configSignals.Write("ByteSize", 16);

    ret &= configSignals.MoveToRoot();

    ret &= gam.SetConfiguredDatabase(configSignals);
    if (ret) {
        ret &= !gam.Setup();
    }
    return ret;
}

bool PIDGAMTest::TestSetupNoOutputSamples() {
    bool ret;
    PIDGAMTestHelper gam;
    ret = gam.HelperInitialise();
    ret &= gam.Initialise(gam.config);

    ConfigurationDatabase configSignals;
    ret &= configSignals.CreateAbsolute("Signals.InputSignals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.Write("NumberOfElements", 1);
    ret &= configSignals.Write("DataSource", "TestDataSource");
    ret &= configSignals.MoveAbsolute("Signals.InputSignals");
    ret &= configSignals.CreateRelative("1");
    ret &= configSignals.Write("NumberOfElements", 1);
    ret &= configSignals.Write("DataSource", "TestDataSource1");
    ret &= configSignals.MoveToAncestor(1u);
    ret &= configSignals.Write("ByteSize", 16);

    ret &= configSignals.MoveToRoot();
    ret &= configSignals.CreateAbsolute("Memory.InputSignals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.Write("DataSource", "TestDataSource");
    ret &= configSignals.CreateRelative("Signals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.Write("Samples", 1);
    ret &= configSignals.MoveToAncestor(3u);
    ret &= configSignals.CreateRelative("1");
    ret &= configSignals.Write("DataSource", "TestDataSource1");
    ret &= configSignals.CreateRelative("Signals");
    ret &= configSignals.CreateRelative("1");
    ret &= configSignals.Write("Samples", 1);

    ret &= configSignals.MoveToRoot();
    ret &= configSignals.CreateAbsolute("Signals.OutputSignals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.Write("NumberOfElements", 1);
    ret &= configSignals.MoveAbsolute("Signals.OutputSignals");
    ret &= configSignals.Write("ByteSize", 16);

    ret &= configSignals.MoveToRoot();

    ret &= gam.SetConfiguredDatabase(configSignals);
    if (ret) {
        ret &= !gam.Setup();
    }
    return ret;
}

bool PIDGAMTest::TestSetupWrongOutputSamples() {
    bool ret;
    PIDGAMTestHelper gam;
    ret = gam.HelperInitialise();
    ret &= gam.Initialise(gam.config);

    ConfigurationDatabase configSignals;
    ret &= configSignals.CreateAbsolute("Signals.InputSignals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.Write("NumberOfElements", 1);
    ret &= configSignals.Write("DataSource", "Reference");
    ret &= configSignals.MoveAbsolute("Signals.InputSignals");
    ret &= configSignals.CreateRelative("1");
    ret &= configSignals.Write("NumberOfElements", 1);
    ret &= configSignals.Write("DataSource", "Measurement");
    ret &= configSignals.MoveToAncestor(1u);
    ret &= configSignals.Write("ByteSize", 16);

    ret &= configSignals.MoveToRoot();
    ret &= configSignals.CreateAbsolute("Memory.InputSignals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.Write("DataSource", "Reference");
    ret &= configSignals.CreateRelative("Signals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.Write("Samples", 1);
    ret &= configSignals.MoveToAncestor(3u);
    ret &= configSignals.CreateRelative("1");
    ret &= configSignals.Write("DataSource", "Measurement");
    ret &= configSignals.CreateRelative("Signals");
    ret &= configSignals.CreateRelative("1");
    ret &= configSignals.Write("Samples", 1);

    ret &= configSignals.MoveToRoot();
    ret &= configSignals.CreateAbsolute("Signals.OutputSignals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.Write("NumberOfElements", 1);
    ret &= configSignals.Write("DataSource", "Reference");
    ret &= configSignals.MoveAbsolute("Signals.OutputSignals");
    ret &= configSignals.Write("ByteSize", 16);

    ret &= configSignals.MoveToRoot();
    ret &= configSignals.CreateAbsolute("Memory.OutputSignals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.Write("DataSource", "Reference");
    ret &= configSignals.CreateRelative("Signals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.Write("Samples", 15);

    ret &= configSignals.MoveToRoot();
    /*
     //example how to print a ConfigurationDatabase
     StreamString a;
     printf("size of a %llu\n", a.Size());
     printf("%d\n", a.Printf("%!", configSignals));
     printf("size of a %llu\n", a.Size());
     printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
     printf("%s\n", a.Buffer());
     printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
     */
    ret &= gam.SetConfiguredDatabase(configSignals);
    if (ret) {
        ret &= !gam.Setup();
    }
    return ret;
}

bool PIDGAMTest::TestSetupNoInputDimensionReference() {
    bool ret;
    PIDGAMTestHelper gam;
    ret = gam.HelperInitialise();
    ret &= gam.Initialise(gam.config);

    ConfigurationDatabase configSignals;
    ret &= configSignals.CreateAbsolute("Signals.InputSignals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.Write("NumberOfElements", 1);
    ret &= configSignals.Write("DataSource", "Reference");
    ret &= configSignals.MoveAbsolute("Signals.InputSignals");
    ret &= configSignals.CreateRelative("1");
    ret &= configSignals.Write("NumberOfElements", 1);
    ret &= configSignals.Write("DataSource", "Measurement");
    ret &= configSignals.MoveToAncestor(1u);
    ret &= configSignals.Write("ByteSize", 16);

    ret &= configSignals.MoveToRoot();
    ret &= configSignals.CreateAbsolute("Memory.InputSignals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.Write("DataSource", "Reference");
    ret &= configSignals.CreateRelative("Signals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.Write("Samples", 1);
    ret &= configSignals.MoveToAncestor(3u);
    ret &= configSignals.CreateRelative("1");
    ret &= configSignals.Write("DataSource", "Measurement");
    ret &= configSignals.CreateRelative("Signals");
    ret &= configSignals.CreateRelative("1");
    ret &= configSignals.Write("Samples", 1);

    ret &= configSignals.MoveToRoot();
    ret &= configSignals.CreateAbsolute("Signals.OutputSignals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.Write("NumberOfElements", 1);
    ret &= configSignals.Write("DataSource", "Reference");
    ret &= configSignals.MoveAbsolute("Signals.OutputSignals");
    ret &= configSignals.Write("ByteSize", 16);

    ret &= configSignals.MoveToRoot();
    ret &= configSignals.CreateAbsolute("Memory.OutputSignals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.Write("DataSource", "Reference");
    ret &= configSignals.CreateRelative("Signals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.Write("Samples", 1);

    ret &= configSignals.MoveToRoot();

    ret &= gam.SetConfiguredDatabase(configSignals);
    if (ret) {
        ret &= !gam.Setup();
    }
    return ret;
}

bool PIDGAMTest::TestSetupWrongInputDimensionReference() {
    bool ret;
    PIDGAMTestHelper gam;
    ret = gam.HelperInitialise();
    ret &= gam.Initialise(gam.config);

    ConfigurationDatabase configSignals;
    ret &= configSignals.CreateAbsolute("Signals.InputSignals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.Write("NumberOfElements", 1);
    ret &= configSignals.Write("DataSource", "Reference");
    ret &= configSignals.Write("NumberOfDimensions", 11); //Wrong value
    ret &= configSignals.MoveAbsolute("Signals.InputSignals");
    ret &= configSignals.CreateRelative("1");
    ret &= configSignals.Write("NumberOfElements", 1);
    ret &= configSignals.Write("DataSource", "Measurement");
    ret &= configSignals.MoveToAncestor(1u);
    ret &= configSignals.Write("ByteSize", 16);

    ret &= configSignals.MoveToRoot();
    ret &= configSignals.CreateAbsolute("Memory.InputSignals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.Write("DataSource", "Reference");
    ret &= configSignals.CreateRelative("Signals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.Write("Samples", 1);
    ret &= configSignals.MoveToAncestor(3u);
    ret &= configSignals.CreateRelative("1");
    ret &= configSignals.Write("DataSource", "Measurement");
    ret &= configSignals.CreateRelative("Signals");
    ret &= configSignals.CreateRelative("1");
    ret &= configSignals.Write("Samples", 1);

    ret &= configSignals.MoveToRoot();
    ret &= configSignals.CreateAbsolute("Signals.OutputSignals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.Write("NumberOfElements", 1);
    ret &= configSignals.Write("DataSource", "Reference");
    ret &= configSignals.MoveAbsolute("Signals.OutputSignals");
    ret &= configSignals.Write("ByteSize", 16);

    ret &= configSignals.MoveToRoot();
    ret &= configSignals.CreateAbsolute("Memory.OutputSignals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.Write("DataSource", "Reference");
    ret &= configSignals.CreateRelative("Signals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.Write("Samples", 1);

    ret &= configSignals.MoveToRoot();

    ret &= gam.SetConfiguredDatabase(configSignals);
    if (ret) {
        ret &= !gam.Setup();
    }
    return ret;
}

bool PIDGAMTest::TestSetupNoInputDimensionMeasurement() {
    bool ret;
    PIDGAMTestHelper gam;
    ret = gam.HelperInitialise();
    ret &= gam.Initialise(gam.config);

    ConfigurationDatabase configSignals;
    ret &= configSignals.CreateAbsolute("Signals.InputSignals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.Write("NumberOfElements", 1);
    ret &= configSignals.Write("DataSource", "Reference");
    ret &= configSignals.Write("NumberOfDimensions", 1);
    ret &= configSignals.MoveAbsolute("Signals.InputSignals");
    ret &= configSignals.CreateRelative("1");
    ret &= configSignals.Write("NumberOfElements", 1);
    ret &= configSignals.Write("DataSource", "Measurement");
    ret &= configSignals.MoveToAncestor(1u);
    ret &= configSignals.Write("ByteSize", 16);

    ret &= configSignals.MoveToRoot();
    ret &= configSignals.CreateAbsolute("Memory.InputSignals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.Write("DataSource", "Reference");
    ret &= configSignals.CreateRelative("Signals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.Write("Samples", 1);
    ret &= configSignals.MoveToAncestor(3u);
    ret &= configSignals.CreateRelative("1");
    ret &= configSignals.Write("DataSource", "Measurement");
    ret &= configSignals.CreateRelative("Signals");
    ret &= configSignals.CreateRelative("1");
    ret &= configSignals.Write("Samples", 1);

    ret &= configSignals.MoveToRoot();
    ret &= configSignals.CreateAbsolute("Signals.OutputSignals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.Write("NumberOfElements", 1);
    ret &= configSignals.Write("DataSource", "Reference");
    ret &= configSignals.MoveAbsolute("Signals.OutputSignals");
    ret &= configSignals.Write("ByteSize", 16);

    ret &= configSignals.MoveToRoot();
    ret &= configSignals.CreateAbsolute("Memory.OutputSignals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.Write("DataSource", "Reference");
    ret &= configSignals.CreateRelative("Signals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.Write("Samples", 1);

    ret &= configSignals.MoveToRoot();

    ret &= gam.SetConfiguredDatabase(configSignals);
    if (ret) {
        ret &= !gam.Setup();
    }
    return ret;
}

bool PIDGAMTest::TestSetupWrongInputDimensionMeasurement() {
    bool ret;
    PIDGAMTestHelper gam;
    ret = gam.HelperInitialise();
    ret &= gam.Initialise(gam.config);

    ConfigurationDatabase configSignals;
    ret &= configSignals.CreateAbsolute("Signals.InputSignals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.Write("NumberOfElements", 1);
    ret &= configSignals.Write("DataSource", "Reference");
    ret &= configSignals.Write("NumberOfDimensions", 1);
    ret &= configSignals.MoveAbsolute("Signals.InputSignals");
    ret &= configSignals.CreateRelative("1");
    ret &= configSignals.Write("NumberOfElements", 1);
    ret &= configSignals.Write("DataSource", "Measurement");
    ret &= configSignals.Write("NumberOfDimensions", 7); //Wrong value
    ret &= configSignals.MoveToAncestor(1u);
    ret &= configSignals.Write("ByteSize", 16);

    ret &= configSignals.MoveToRoot();
    ret &= configSignals.CreateAbsolute("Memory.InputSignals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.Write("DataSource", "Reference");
    ret &= configSignals.CreateRelative("Signals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.Write("Samples", 1);
    ret &= configSignals.MoveToAncestor(3u);
    ret &= configSignals.CreateRelative("1");
    ret &= configSignals.Write("DataSource", "Measurement");
    ret &= configSignals.CreateRelative("Signals");
    ret &= configSignals.CreateRelative("1");
    ret &= configSignals.Write("Samples", 1);

    ret &= configSignals.MoveToRoot();
    ret &= configSignals.CreateAbsolute("Signals.OutputSignals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.Write("NumberOfElements", 1);
    ret &= configSignals.Write("DataSource", "Reference");
    ret &= configSignals.MoveAbsolute("Signals.OutputSignals");
    ret &= configSignals.Write("ByteSize", 16);

    ret &= configSignals.MoveToRoot();
    ret &= configSignals.CreateAbsolute("Memory.OutputSignals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.Write("DataSource", "Reference");
    ret &= configSignals.CreateRelative("Signals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.Write("Samples", 1);

    ret &= configSignals.MoveToRoot();

    ret &= gam.SetConfiguredDatabase(configSignals);
    if (ret) {
        ret &= !gam.Setup();
    }
    return ret;
}

bool PIDGAMTest::TestSetupNoOutputDimension() {
    bool ret;
    PIDGAMTestHelper gam;
    ret = gam.HelperInitialise();
    ret &= gam.Initialise(gam.config);

    ConfigurationDatabase configSignals;
    ret &= configSignals.CreateAbsolute("Signals.InputSignals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.Write("NumberOfElements", 1);
    ret &= configSignals.Write("DataSource", "Reference");
    ret &= configSignals.Write("NumberOfDimensions", 1);
    ret &= configSignals.MoveAbsolute("Signals.InputSignals");
    ret &= configSignals.CreateRelative("1");
    ret &= configSignals.Write("NumberOfElements", 1);
    ret &= configSignals.Write("DataSource", "Measurement");
    ret &= configSignals.Write("NumberOfDimensions", 1);
    ret &= configSignals.MoveToAncestor(1u);
    ret &= configSignals.Write("ByteSize", 16);

    ret &= configSignals.MoveToRoot();
    ret &= configSignals.CreateAbsolute("Memory.InputSignals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.Write("DataSource", "Reference");
    ret &= configSignals.CreateRelative("Signals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.Write("Samples", 1);
    ret &= configSignals.MoveToAncestor(3u);
    ret &= configSignals.CreateRelative("1");
    ret &= configSignals.Write("DataSource", "Measurement");
    ret &= configSignals.CreateRelative("Signals");
    ret &= configSignals.CreateRelative("1");
    ret &= configSignals.Write("Samples", 1);

    ret &= configSignals.MoveToRoot();
    ret &= configSignals.CreateAbsolute("Signals.OutputSignals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.Write("NumberOfElements", 1);
    ret &= configSignals.Write("DataSource", "Reference");
    ret &= configSignals.MoveAbsolute("Signals.OutputSignals");
    ret &= configSignals.Write("ByteSize", 16);

    ret &= configSignals.MoveToRoot();
    ret &= configSignals.CreateAbsolute("Memory.OutputSignals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.Write("DataSource", "Reference");
    ret &= configSignals.CreateRelative("Signals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.Write("Samples", 1);

    ret &= configSignals.MoveToRoot();

    ret &= gam.SetConfiguredDatabase(configSignals);
    if (ret) {
        ret &= !gam.Setup();
    }
    return ret;
}

bool PIDGAMTest::TestSetupWrongOutputDimension() {
    bool ret;
    PIDGAMTestHelper gam;
    ret = gam.HelperInitialise();
    ret &= gam.Initialise(gam.config);

    ConfigurationDatabase configSignals;
    ret &= configSignals.CreateAbsolute("Signals.InputSignals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.Write("NumberOfElements", 1);
    ret &= configSignals.Write("DataSource", "Reference");
    ret &= configSignals.Write("NumberOfDimensions", 1);
    ret &= configSignals.Write("ByteSize", 8);
    ret &= configSignals.MoveAbsolute("Signals.InputSignals");
    ret &= configSignals.CreateRelative("1");
    ret &= configSignals.Write("NumberOfElements", 1);
    ret &= configSignals.Write("DataSource", "Measurement");
    ret &= configSignals.Write("NumberOfDimensions", 1);
    ret &= configSignals.Write("ByteSize", 8);
    ret &= configSignals.MoveToAncestor(1u);
    ret &= configSignals.Write("ByteSize", 16);

    ret &= configSignals.MoveToRoot();
    ret &= configSignals.CreateAbsolute("Memory.InputSignals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.Write("DataSource", "Reference");
    ret &= configSignals.CreateRelative("Signals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.Write("Samples", 1);
    ret &= configSignals.MoveToAncestor(3u);
    ret &= configSignals.CreateRelative("1");
    ret &= configSignals.Write("DataSource", "Measurement");
    ret &= configSignals.CreateRelative("Signals");
    ret &= configSignals.CreateRelative("1");
    ret &= configSignals.Write("Samples", 1);

    ret &= configSignals.MoveToRoot();
    ret &= configSignals.CreateAbsolute("Signals.OutputSignals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.Write("NumberOfElements", 1);
    ret &= configSignals.Write("DataSource", "Reference");
    ret &= configSignals.Write("NumberOfDimensions", 5); //Wrong value
    ret &= configSignals.Write("ByteSize", 8);
    ret &= configSignals.MoveAbsolute("Signals.OutputSignals");
    ret &= configSignals.Write("ByteSize", 8);

    ret &= configSignals.MoveToRoot();
    ret &= configSignals.CreateAbsolute("Memory.OutputSignals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.Write("DataSource", "Reference");
    ret &= configSignals.CreateRelative("Signals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.Write("Samples", 1);

    ret &= configSignals.MoveToRoot();

    ret &= gam.SetConfiguredDatabase(configSignals);
    if (ret) {
        ret &= !gam.Setup();
    }
    return ret;
}

bool PIDGAMTest::TestSetupNoReferenceInputType() {
    bool ret;
    PIDGAMTestHelper gam;
    ret = gam.HelperInitialise();
    ret &= gam.Initialise(gam.config);

    ConfigurationDatabase configSignals;
    ret &= configSignals.CreateAbsolute("Signals.InputSignals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.Write("NumberOfElements", 1);
    ret &= configSignals.Write("DataSource", "Reference");
    ret &= configSignals.Write("NumberOfDimensions", 1);
    ret &= configSignals.Write("ByteSize", 8);
    ret &= configSignals.MoveAbsolute("Signals.InputSignals");
    ret &= configSignals.CreateRelative("1");
    ret &= configSignals.Write("NumberOfElements", 1);
    ret &= configSignals.Write("DataSource", "Measurement");
    ret &= configSignals.Write("NumberOfDimensions", 1);
    ret &= configSignals.Write("ByteSize", 8);
    ret &= configSignals.MoveToAncestor(1u);
    ret &= configSignals.Write("ByteSize", 16);

    ret &= configSignals.MoveToRoot();
    ret &= configSignals.CreateAbsolute("Memory.InputSignals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.Write("DataSource", "Reference");
    ret &= configSignals.CreateRelative("Signals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.Write("Samples", 1);
    ret &= configSignals.MoveToAncestor(3u);
    ret &= configSignals.CreateRelative("1");
    ret &= configSignals.Write("DataSource", "Measurement");
    ret &= configSignals.CreateRelative("Signals");
    ret &= configSignals.CreateRelative("1");
    ret &= configSignals.Write("Samples", 1);

    ret &= configSignals.MoveToRoot();
    ret &= configSignals.CreateAbsolute("Signals.OutputSignals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.Write("NumberOfElements", 1);
    ret &= configSignals.Write("DataSource", "Reference");
    ret &= configSignals.Write("NumberOfDimensions", 1);
    ret &= configSignals.Write("ByteSize", 8);
    ret &= configSignals.MoveAbsolute("Signals.OutputSignals");
    ret &= configSignals.Write("ByteSize", 8);

    ret &= configSignals.MoveToRoot();
    ret &= configSignals.CreateAbsolute("Memory.OutputSignals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.Write("DataSource", "Reference");
    ret &= configSignals.CreateRelative("Signals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.Write("Samples", 1);

    ret &= configSignals.MoveToRoot();

    ret &= gam.SetConfiguredDatabase(configSignals);
    if (ret) {
        ret &= !gam.Setup();
    }
    return ret;
}

bool PIDGAMTest::TestSetupWrongReferenceInputType() {
    bool ret;
    PIDGAMTestHelper gam;
    ret = gam.HelperInitialise();
    ret &= gam.Initialise(gam.config);

    ConfigurationDatabase configSignals;
    ret &= configSignals.CreateAbsolute("Signals.InputSignals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.Write("NumberOfElements", 1);
    ret &= configSignals.Write("DataSource", "Reference");
    ret &= configSignals.Write("NumberOfDimensions", 1);
    ret &= configSignals.Write("Type", "float32");
    ret &= configSignals.Write("ByteSize", 8);
    ret &= configSignals.MoveAbsolute("Signals.InputSignals");
    ret &= configSignals.CreateRelative("1");
    ret &= configSignals.Write("NumberOfElements", 1);
    ret &= configSignals.Write("DataSource", "Measurement");
    ret &= configSignals.Write("NumberOfDimensions", 1);
    ret &= configSignals.Write("ByteSize", 8);
    ret &= configSignals.MoveToAncestor(1u);
    ret &= configSignals.Write("ByteSize", 16);

    ret &= configSignals.MoveToRoot();
    ret &= configSignals.CreateAbsolute("Memory.InputSignals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.Write("DataSource", "Reference");
    ret &= configSignals.CreateRelative("Signals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.Write("Samples", 1);
    ret &= configSignals.MoveToAncestor(3u);
    ret &= configSignals.CreateRelative("1");
    ret &= configSignals.Write("DataSource", "Measurement");
    ret &= configSignals.CreateRelative("Signals");
    ret &= configSignals.CreateRelative("1");
    ret &= configSignals.Write("Samples", 1);

    ret &= configSignals.MoveToRoot();
    ret &= configSignals.CreateAbsolute("Signals.OutputSignals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.Write("NumberOfElements", 1);
    ret &= configSignals.Write("DataSource", "Reference");
    ret &= configSignals.Write("NumberOfDimensions", 1);
    ret &= configSignals.Write("ByteSize", 8);
    ret &= configSignals.MoveAbsolute("Signals.OutputSignals");
    ret &= configSignals.Write("ByteSize", 8);

    ret &= configSignals.MoveToRoot();
    ret &= configSignals.CreateAbsolute("Memory.OutputSignals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.Write("DataSource", "Reference");
    ret &= configSignals.CreateRelative("Signals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.Write("Samples", 1);

    ret &= configSignals.MoveToRoot();

    ret &= gam.SetConfiguredDatabase(configSignals);
    if (ret) {
        ret &= !gam.Setup();
    }
    return ret;
}

bool PIDGAMTest::TestSetupNoMeasurementInputType() {
    bool ret;
    PIDGAMTestHelper gam;
    ret = gam.HelperInitialise();
    ret &= gam.Initialise(gam.config);

    ConfigurationDatabase configSignals;
    ret &= configSignals.CreateAbsolute("Signals.InputSignals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.Write("NumberOfElements", 1);
    ret &= configSignals.Write("DataSource", "Reference");
    ret &= configSignals.Write("NumberOfDimensions", 1);
    ret &= configSignals.Write("Type", "float64");
    ret &= configSignals.Write("ByteSize", 8);
    ret &= configSignals.MoveAbsolute("Signals.InputSignals");
    ret &= configSignals.CreateRelative("1");
    ret &= configSignals.Write("NumberOfElements", 1);
    ret &= configSignals.Write("DataSource", "Measurement");
    ret &= configSignals.Write("NumberOfDimensions", 1);
    ret &= configSignals.Write("ByteSize", 8);
    ret &= configSignals.MoveToAncestor(1u);
    ret &= configSignals.Write("ByteSize", 16);

    ret &= configSignals.MoveToRoot();
    ret &= configSignals.CreateAbsolute("Memory.InputSignals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.Write("DataSource", "Reference");
    ret &= configSignals.CreateRelative("Signals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.Write("Samples", 1);
    ret &= configSignals.MoveToAncestor(3u);
    ret &= configSignals.CreateRelative("1");
    ret &= configSignals.Write("DataSource", "Measurement");
    ret &= configSignals.CreateRelative("Signals");
    ret &= configSignals.CreateRelative("1");
    ret &= configSignals.Write("Samples", 1);

    ret &= configSignals.MoveToRoot();
    ret &= configSignals.CreateAbsolute("Signals.OutputSignals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.Write("NumberOfElements", 1);
    ret &= configSignals.Write("DataSource", "Reference");
    ret &= configSignals.Write("NumberOfDimensions", 1);
    ret &= configSignals.Write("ByteSize", 8);
    ret &= configSignals.MoveAbsolute("Signals.OutputSignals");
    ret &= configSignals.Write("ByteSize", 8);

    ret &= configSignals.MoveToRoot();
    ret &= configSignals.CreateAbsolute("Memory.OutputSignals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.Write("DataSource", "Reference");
    ret &= configSignals.CreateRelative("Signals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.Write("Samples", 1);

    ret &= configSignals.MoveToRoot();

    ret &= gam.SetConfiguredDatabase(configSignals);
    if (ret) {
        ret &= !gam.Setup();
    }
    return ret;
}

bool PIDGAMTest::TestSetupWrongMeasurementInputType() {
    bool ret;
    PIDGAMTestHelper gam;
    ret = gam.HelperInitialise();
    ret &= gam.Initialise(gam.config);

    ConfigurationDatabase configSignals;
    ret &= configSignals.CreateAbsolute("Signals.InputSignals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.Write("NumberOfElements", 1);
    ret &= configSignals.Write("DataSource", "Reference");
    ret &= configSignals.Write("NumberOfDimensions", 1);
    ret &= configSignals.Write("Type", "float64");
    ret &= configSignals.Write("ByteSize", 8);
    ret &= configSignals.MoveAbsolute("Signals.InputSignals");
    ret &= configSignals.CreateRelative("1");
    ret &= configSignals.Write("NumberOfElements", 1);
    ret &= configSignals.Write("DataSource", "Measurement");
    ret &= configSignals.Write("NumberOfDimensions", 1);
    ret &= configSignals.Write("Type", "float32");
    ret &= configSignals.Write("ByteSize", 8);
    ret &= configSignals.MoveToAncestor(1u);
    ret &= configSignals.Write("ByteSize", 16);

    ret &= configSignals.MoveToRoot();
    ret &= configSignals.CreateAbsolute("Memory.InputSignals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.Write("DataSource", "Reference");
    ret &= configSignals.CreateRelative("Signals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.Write("Samples", 1);
    ret &= configSignals.MoveToAncestor(3u);
    ret &= configSignals.CreateRelative("1");
    ret &= configSignals.Write("DataSource", "Measurement");
    ret &= configSignals.CreateRelative("Signals");
    ret &= configSignals.CreateRelative("1");
    ret &= configSignals.Write("Samples", 1);

    ret &= configSignals.MoveToRoot();
    ret &= configSignals.CreateAbsolute("Signals.OutputSignals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.Write("NumberOfElements", 1);
    ret &= configSignals.Write("DataSource", "Reference");
    ret &= configSignals.Write("NumberOfDimensions", 1);
    ret &= configSignals.Write("ByteSize", 8);
    ret &= configSignals.MoveAbsolute("Signals.OutputSignals");
    ret &= configSignals.Write("ByteSize", 8);

    ret &= configSignals.MoveToRoot();
    ret &= configSignals.CreateAbsolute("Memory.OutputSignals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.Write("DataSource", "Reference");
    ret &= configSignals.CreateRelative("Signals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.Write("Samples", 1);

    ret &= configSignals.MoveToRoot();

    ret &= gam.SetConfiguredDatabase(configSignals);
    if (ret) {
        ret &= !gam.Setup();
    }
    return ret;
}

bool PIDGAMTest::TestSetupNoOutputType() {
    bool ret;
    PIDGAMTestHelper gam;
    ret = gam.HelperInitialise();
    ret &= gam.Initialise(gam.config);

    ConfigurationDatabase configSignals;
    ret &= configSignals.CreateAbsolute("Signals.InputSignals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.Write("NumberOfElements", 1);
    ret &= configSignals.Write("DataSource", "Reference");
    ret &= configSignals.Write("NumberOfDimensions", 1);
    ret &= configSignals.Write("Type", "float64");
    ret &= configSignals.Write("ByteSize", 8);
    ret &= configSignals.MoveAbsolute("Signals.InputSignals");
    ret &= configSignals.CreateRelative("1");
    ret &= configSignals.Write("NumberOfElements", 1);
    ret &= configSignals.Write("DataSource", "Measurement");
    ret &= configSignals.Write("NumberOfDimensions", 1);
    ret &= configSignals.Write("Type", "float64");
    ret &= configSignals.Write("ByteSize", 8);
    ret &= configSignals.MoveToAncestor(1u);
    ret &= configSignals.Write("ByteSize", 16);

    ret &= configSignals.MoveToRoot();
    ret &= configSignals.CreateAbsolute("Memory.InputSignals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.Write("DataSource", "Reference");
    ret &= configSignals.CreateRelative("Signals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.Write("Samples", 1);
    ret &= configSignals.MoveToAncestor(3u);
    ret &= configSignals.CreateRelative("1");
    ret &= configSignals.Write("DataSource", "Measurement");
    ret &= configSignals.CreateRelative("Signals");
    ret &= configSignals.CreateRelative("1");
    ret &= configSignals.Write("Samples", 1);

    ret &= configSignals.MoveToRoot();
    ret &= configSignals.CreateAbsolute("Signals.OutputSignals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.Write("NumberOfElements", 1);
    ret &= configSignals.Write("DataSource", "Reference");
    ret &= configSignals.Write("NumberOfDimensions", 1);
    ret &= configSignals.Write("ByteSize", 8);
    ret &= configSignals.MoveAbsolute("Signals.OutputSignals");
    ret &= configSignals.Write("ByteSize", 8);

    ret &= configSignals.MoveToRoot();
    ret &= configSignals.CreateAbsolute("Memory.OutputSignals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.Write("DataSource", "Reference");
    ret &= configSignals.CreateRelative("Signals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.Write("Samples", 1);

    ret &= configSignals.MoveToRoot();

    ret &= gam.SetConfiguredDatabase(configSignals);
    if (ret) {
        ret &= !gam.Setup();
    }
    return ret;
}

bool PIDGAMTest::TestSetupWrongOutputType() {
    bool ret;
    PIDGAMTestHelper gam;
    ret = gam.HelperInitialise();
    ret &= gam.Initialise(gam.config);

    ConfigurationDatabase configSignals;
    ret &= configSignals.CreateAbsolute("Signals.InputSignals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.Write("NumberOfElements", 1);
    ret &= configSignals.Write("DataSource", "Reference");
    ret &= configSignals.Write("NumberOfDimensions", 1);
    ret &= configSignals.Write("Type", "float64");
    ret &= configSignals.Write("ByteSize", 8);
    ret &= configSignals.MoveAbsolute("Signals.InputSignals");
    ret &= configSignals.CreateRelative("1");
    ret &= configSignals.Write("NumberOfElements", 1);
    ret &= configSignals.Write("DataSource", "Measurement");
    ret &= configSignals.Write("NumberOfDimensions", 1);
    ret &= configSignals.Write("Type", "float64");
    ret &= configSignals.Write("ByteSize", 8);
    ret &= configSignals.MoveToAncestor(1u);
    ret &= configSignals.Write("ByteSize", 16);

    ret &= configSignals.MoveToRoot();
    ret &= configSignals.CreateAbsolute("Memory.InputSignals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.Write("DataSource", "Reference");
    ret &= configSignals.CreateRelative("Signals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.Write("Samples", 1);
    ret &= configSignals.MoveToAncestor(3u);
    ret &= configSignals.CreateRelative("1");
    ret &= configSignals.Write("DataSource", "Measurement");
    ret &= configSignals.CreateRelative("Signals");
    ret &= configSignals.CreateRelative("1");
    ret &= configSignals.Write("Samples", 1);

    ret &= configSignals.MoveToRoot();
    ret &= configSignals.CreateAbsolute("Signals.OutputSignals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.Write("NumberOfElements", 1);
    ret &= configSignals.Write("DataSource", "Reference");
    ret &= configSignals.Write("NumberOfDimensions", 1);
    ret &= configSignals.Write("Type", "int32");
    ret &= configSignals.Write("ByteSize", 8);
    ret &= configSignals.MoveAbsolute("Signals.OutputSignals");
    ret &= configSignals.Write("ByteSize", 8);

    ret &= configSignals.MoveToRoot();
    ret &= configSignals.CreateAbsolute("Memory.OutputSignals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.Write("DataSource", "Reference");
    ret &= configSignals.CreateRelative("Signals");
    ret &= configSignals.CreateRelative("0");
    ret &= configSignals.Write("Samples", 1);

    ret &= configSignals.MoveToRoot();

    ret &= gam.SetConfiguredDatabase(configSignals);
    if (ret) {
        ret &= !gam.Setup();
    }
    return ret;
}

bool PIDGAMTest::TestSetup1InputSignal() {
    bool ret;
    PIDGAMTestHelper gam;
    ret = gam.HelperInitialise();
    ret &= gam.Initialise(gam.config);
    ret &= gam.HelperSetup1();
    /*
     //example how to print a ConfigurationDatabase
     StreamString a;
     printf("size of a %llu\n", a.Size());
     printf("%d\n", a.Printf("%!", gam.configSignals));
     printf("size of a %llu\n", a.Size());
     printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
     printf("%s\n", a.Buffer());
     printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
     */
    ret &= gam.SetConfiguredDatabase(gam.configSignals);
    ret &= gam.AllocateInputSignalsMemory();
    ret &= gam.AllocateOutputSignalsMemory();
    if (ret) {
        ret &= gam.Setup();
    }
    return ret;
}

bool PIDGAMTest::TestSetup2InputSignals() {
    bool ret;
    PIDGAMTestHelper gam;
    ret = gam.HelperInitialise();
    ret &= gam.Initialise(gam.config);
    ret &= gam.HelperSetup2();

    ret &= gam.SetConfiguredDatabase(gam.configSignals);
    ret &= gam.AllocateInputSignalsMemory();
    ret &= gam.AllocateOutputSignalsMemory();
    if (ret) {
        ret &= gam.Setup();
    }
    return ret;
}

bool PIDGAMTest::TestExecutekpSubtract() {
    bool ret;
    PIDGAMTestHelper gam(1, 0, 0, 0.001);
    ret = gam.HelperInitialise();
    ret &= gam.Initialise(gam.config);

    ret &= gam.HelperSetup2();
    ret &= gam.SetConfiguredDatabase(gam.configSignals);
    ret &= gam.AllocateInputSignalsMemory();
    ret &= gam.AllocateOutputSignalsMemory();
    ret &= gam.Setup();

    float64 *gamMemoryInR = static_cast<float64 *>(gam.GetInputSignalsMemory());
    float64 *gamMemoryInM = static_cast<float64 *>(gam.GetInputSignalsMemory(1));
    float64 *gamMemoryOut = static_cast<float64 *>(gam.GetOutputSignalsMemory());
    uint32 maxRep = 400;
    float64 *expectedVale = new float64[maxRep];
    //generate output
    *expectedVale = 1;
    for (uint32 i = 1u; i < maxRep; i++) {
        if (expectedVale[i - 1] == 1) {
            expectedVale[i] = 0;
        }
        else if (expectedVale[i - 1] == 0) {
            expectedVale[i] = 1;
        }
        else {
            ret = false;
        }
    }
    *gamMemoryInM = 0;
    for (uint32 i = 0u; i < maxRep && ret; i++) {
        //set inputs
        *gamMemoryInR = 1;
        gam.Execute();
        //Check output
        ret &= (*gamMemoryOut == expectedVale[i]);
        if (!ret) {
            printf("output value = %.17lf. expectedValue = %.17lf. index = %u \n", *gamMemoryOut, expectedVale[i], i);
        }
        *gamMemoryInM = *gamMemoryOut;
    }
    return ret;
}
bool PIDGAMTest::TestExecutekp() {
    bool ret;
    PIDGAMTestHelper gam(1, 0, 0, 0.001);
    ret = gam.HelperInitialise();
    ret &= gam.Initialise(gam.config);

    ret &= gam.HelperSetup1();
    ret &= gam.SetConfiguredDatabase(gam.configSignals);
    ret &= gam.AllocateInputSignalsMemory();
    ret &= gam.AllocateOutputSignalsMemory();
    ret &= gam.Setup();

    float64 *gamMemoryInError = static_cast<float64 *>(gam.GetInputSignalsMemory());
    float64 *gamMemoryOut = static_cast<float64 *>(gam.GetOutputSignalsMemory());
    uint32 maxRep = 400;
    float64 *expectedVale = new float64[maxRep];
    //generate output
    *expectedVale = 1;
    for (uint32 i = 1u; i < maxRep; i++) {
        if (expectedVale[i - 1] == 1) {
            expectedVale[i] = 0;
        }
        else if (expectedVale[i - 1] == 0) {
            expectedVale[i] = 1;
        }
        else {
            ret = false;
        }
    }
    float64 reference = 1.0;
    for (uint32 i = 0u; i < maxRep && ret; i++) {
        //set inputs
        *gamMemoryInError = reference - *gamMemoryOut;
        gam.Execute();
        //Check output
        ret &= (*gamMemoryOut == expectedVale[i]);
        if (!ret) {
            printf("output value = %.17lf. expectedValue = %.17lf. index = %u \n", *gamMemoryOut, expectedVale[i], i);
        }
    }
    return ret;
}

bool PIDGAMTest::TestExecutekiSubtract() {
    bool ret;
    PIDGAMTestHelper gam(0, 1000, 0, 0.001);
    ret = gam.HelperInitialise();
    ret &= gam.Initialise(gam.config);

    ret &= gam.HelperSetup2();
    ret &= gam.SetConfiguredDatabase(gam.configSignals);
    ret &= gam.AllocateInputSignalsMemory();
    ret &= gam.AllocateOutputSignalsMemory();
    ret &= gam.Setup();

    float64 *gamMemoryInR = static_cast<float64 *>(gam.GetInputSignalsMemory());
    float64 *gamMemoryInM = static_cast<float64 *>(gam.GetInputSignalsMemory(1));
    float64 *gamMemoryOut = static_cast<float64 *>(gam.GetOutputSignalsMemory());
    uint32 maxRep = 400;
    float64 *expectedVale = new float64[maxRep];
    //generate output
    *expectedVale = 1;
    for (uint32 i = 1u; i < maxRep; i++) {
        expectedVale[i] = 0;
    }
    *gamMemoryInM = 0;
    *gamMemoryInR = 1;
    for (uint32 i = 0u; i < maxRep && ret; i++) {
        //set inputs
        gam.Execute();
        //Check output
        ret &= (*gamMemoryOut == expectedVale[i]);
        if (!ret) {
            printf("output value = %.17lf. expectedValue = %.17lf. index = %u \n", *gamMemoryOut, expectedVale[i], i);
        }
        *gamMemoryInM = *gamMemoryOut;
        *gamMemoryInR = 0;
    }
    return ret;
}

bool PIDGAMTest::TestExecuteki() {
    bool ret;
    PIDGAMTestHelper gam(0, 1000, 0, 0.001);
    ret = gam.HelperInitialise();
    ret &= gam.Initialise(gam.config);

    ret &= gam.HelperSetup1();
    ret &= gam.SetConfiguredDatabase(gam.configSignals);
    ret &= gam.AllocateInputSignalsMemory();
    ret &= gam.AllocateOutputSignalsMemory();
    ret &= gam.Setup();

    float64 *gamMemoryInError = static_cast<float64 *>(gam.GetInputSignalsMemory());
    float64 *gamMemoryOut = static_cast<float64 *>(gam.GetOutputSignalsMemory());
    uint32 maxRep = 400;
    float64 *expectedVale = new float64[maxRep];
    //generate output
    *expectedVale = 1;
    for (uint32 i = 1u; i < maxRep; i++) {
        expectedVale[i] = 0;
    }
    *gamMemoryOut = 0;
    float64 gamMemoryInR = 1;
    for (uint32 i = 0u; i < maxRep && ret; i++) {
        //set inputs
        *gamMemoryInError = gamMemoryInR - *gamMemoryOut ;
        gam.Execute();
        //Check output
        ret &= (*gamMemoryOut == expectedVale[i]);
        if (!ret) {
            printf("output value = %.17lf. expectedValue = %.17lf. index = %u \n", *gamMemoryOut, expectedVale[i], i);
        }
        gamMemoryInR = 0;
    }
    return ret;
}

bool PIDGAMTest::TestExecutekiSubtract2() {
    bool ret;
    PIDGAMTestHelper gam(0, 500, 0, 0.001);
    ret = gam.HelperInitialise();
    ret &= gam.Initialise(gam.config);

    ret &= gam.HelperSetup2();
    ret &= gam.SetConfiguredDatabase(gam.configSignals);
    ret &= gam.AllocateInputSignalsMemory();
    ret &= gam.AllocateOutputSignalsMemory();
    ret &= gam.Setup();

    float64 *gamMemoryInR = static_cast<float64 *>(gam.GetInputSignalsMemory());
    float64 *gamMemoryInM = static_cast<float64 *>(gam.GetInputSignalsMemory(1));
    float64 *gamMemoryOut = static_cast<float64 *>(gam.GetOutputSignalsMemory());
    uint32 maxRep = 4;
    float64 *expectedVale = new float64[maxRep];
    //generate output
    expectedVale[0] = 0.5;
    expectedVale[1] = 0.75;
    expectedVale[2] = 0.875;
    expectedVale[3] = 0.9375;
    *gamMemoryInM = 0;
    *gamMemoryInR = 1;
    for (uint32 i = 0u; i < maxRep && ret; i++) {
        //set inputs
        gam.Execute();
        //Check output
        ret &= (*gamMemoryOut == expectedVale[i]);
        if (!ret) {
            printf("output value = %.17lf. expectedValue = %.17lf. index = %u \n", *gamMemoryOut, expectedVale[i], i);
        }
        *gamMemoryInM = *gamMemoryOut;
    }
    return ret;
}

bool PIDGAMTest::TestExecuteki2() {
    bool ret;
    PIDGAMTestHelper gam(0, 500, 0, 0.001);
    ret = gam.HelperInitialise();
    ret &= gam.Initialise(gam.config);

    ret &= gam.HelperSetup1();
    ret &= gam.SetConfiguredDatabase(gam.configSignals);
    ret &= gam.AllocateInputSignalsMemory();
    ret &= gam.AllocateOutputSignalsMemory();
    ret &= gam.Setup();

    float64 *gamMemoryInError = static_cast<float64 *>(gam.GetInputSignalsMemory());
    float64 *gamMemoryOut = static_cast<float64 *>(gam.GetOutputSignalsMemory());
    uint32 maxRep = 4;
    float64 *expectedVale = new float64[maxRep];
    //generate output
    //generate output
    expectedVale[0] = 0.5;
    expectedVale[1] = 0.75;
    expectedVale[2] = 0.875;
    expectedVale[3] = 0.9375;
    *gamMemoryOut = 0;
    float64 gamMemoryInR = 1;
    for (uint32 i = 0u; i < maxRep && ret; i++) {
        //set inputs
        *gamMemoryInError = gamMemoryInR - *gamMemoryOut ;
        gam.Execute();
        //Check output
        ret &= (*gamMemoryOut == expectedVale[i]);
        if (!ret) {
            printf("output value = %.17lf. expectedValue = %.17lf. index = %u \n", *gamMemoryOut, expectedVale[i], i);
        }
    }
    return ret;
}

bool PIDGAMTest::TestExecutekdSubtract() {
    bool ret;
    PIDGAMTestHelper gam(0, 0, 0.001, 0.001);
    ret = gam.HelperInitialise();
    ret &= gam.Initialise(gam.config);

    ret &= gam.HelperSetup2();
    ret &= gam.SetConfiguredDatabase(gam.configSignals);
    ret &= gam.AllocateInputSignalsMemory();
    ret &= gam.AllocateOutputSignalsMemory();
    ret &= gam.Setup();

    float64 *gamMemoryInR = static_cast<float64 *>(gam.GetInputSignalsMemory());
    float64 *gamMemoryInM = static_cast<float64 *>(gam.GetInputSignalsMemory(1));
    float64 *gamMemoryOut = static_cast<float64 *>(gam.GetOutputSignalsMemory());
    uint32 maxRep = 4;
    float64 *expectedVale = new float64[maxRep];
    //generate output
    expectedVale[0] = 1;
    expectedVale[1] = -1;
    expectedVale[2] = 2;
    expectedVale[3] = -3;//yep it is unstable.

    *gamMemoryInM = 0;
    *gamMemoryInR = 1;
    for (uint32 i = 0u; i < maxRep && ret; i++) {
        //set inputs
        gam.Execute();
        //Check output
        ret &= (*gamMemoryOut == expectedVale[i]);
        if (!ret) {
            printf("output value = %.17lf. expectedValue = %.17lf. index = %u \n", *gamMemoryOut, expectedVale[i], i);
        }
        *gamMemoryInM = *gamMemoryOut;
    }
    return ret;
}

bool PIDGAMTest::TestExecutekd() {
    bool ret;
    PIDGAMTestHelper gam(0, 0, 0.001, 0.001);
    ret = gam.HelperInitialise();
    ret &= gam.Initialise(gam.config);

    ret &= gam.HelperSetup1();
    ret &= gam.SetConfiguredDatabase(gam.configSignals);
    ret &= gam.AllocateInputSignalsMemory();
    ret &= gam.AllocateOutputSignalsMemory();
    ret &= gam.Setup();

    float64 *gamMemoryInError = static_cast<float64 *>(gam.GetInputSignalsMemory());
    float64 *gamMemoryOut = static_cast<float64 *>(gam.GetOutputSignalsMemory());
    uint32 maxRep = 4;
    float64 *expectedVale = new float64[maxRep];
    //generate output
    expectedVale[0] = 1;
    expectedVale[1] = -1;
    expectedVale[2] = 2;
    expectedVale[3] = -3;//yep it is unstable.
    *gamMemoryOut = 0;
    float64 gamMemoryInReference = 1;
    for (uint32 i = 0u; i < maxRep && ret; i++) {
        //set inputs
        *gamMemoryInError = gamMemoryInReference - *gamMemoryOut;
        gam.Execute();
        //Check output
        ret &= (*gamMemoryOut == expectedVale[i]);
        if (!ret) {
            printf("output value = %.17lf. expectedValue = %.17lf. index = %u \n", *gamMemoryOut, expectedVale[i], i);
        }
    }
    return ret;
}


bool PIDGAMTest::TestExecutekpkiSubtract() {
    bool ret;
    PIDGAMTestHelper gam(0.5, 500, 0.0, 0.001);
    ret = gam.HelperInitialise();
    ret &= gam.Initialise(gam.config);

    ret &= gam.HelperSetup2();
    ret &= gam.SetConfiguredDatabase(gam.configSignals);
    ret &= gam.AllocateInputSignalsMemory();
    ret &= gam.AllocateOutputSignalsMemory();
    ret &= gam.Setup();

    float64 *gamMemoryInR = static_cast<float64 *>(gam.GetInputSignalsMemory());
    float64 *gamMemoryInM = static_cast<float64 *>(gam.GetInputSignalsMemory(1));
    float64 *gamMemoryOut = static_cast<float64 *>(gam.GetOutputSignalsMemory());
    uint32 maxRep = 11;
    float64 *expectedVale = new float64[maxRep];
    //generate output
    expectedVale[0] = 1.0;
    expectedVale[1] = 0.5;
    expectedVale[2] = 1.0;
    expectedVale[3] = 0.75;
    expectedVale[4] = 1.0;
    expectedVale[5] = 0.875;
    expectedVale[6] = 1.0;
    expectedVale[7] = 0.9375;

    expectedVale[8] = 1;
    expectedVale[9] = 0.96875;
    expectedVale[10] = 1.0;

    *gamMemoryInM = 0;
    *gamMemoryInR = 1;
    for (uint32 i = 0u; i < maxRep && ret; i++) {
        //set inputs
        gam.Execute();
        //Check output
        ret &= gam.IsEqualLargerMargins(*gamMemoryOut, expectedVale[i]);
        if (!ret) {
            printf("output value  = %.16lf.\nexpectedValue = %.16lf. index = %u \n", *gamMemoryOut, expectedVale[i], i);
        }
        *gamMemoryInM = *gamMemoryOut;
    }
    return ret;
}

bool PIDGAMTest::TestExecutekpkikdSubtract() {
    bool ret;
    PIDGAMTestHelper gam(0.5, 500, 0.00001, 0.001);
    ret = gam.HelperInitialise();
    ret &= gam.Initialise(gam.config);

    ret &= gam.HelperSetup2();
    ret &= gam.SetConfiguredDatabase(gam.configSignals);
    ret &= gam.AllocateInputSignalsMemory();
    ret &= gam.AllocateOutputSignalsMemory();
    ret &= gam.Setup();

    float64 *gamMemoryInR = static_cast<float64 *>(gam.GetInputSignalsMemory());
    float64 *gamMemoryInM = static_cast<float64 *>(gam.GetInputSignalsMemory(1));
    float64 *gamMemoryOut = static_cast<float64 *>(gam.GetOutputSignalsMemory());
    uint32 maxRep = 8;
    float64 *expectedVale = new float64[maxRep];
    //generate output
    expectedVale[0] = 1.01;
    expectedVale[1] = 0.4799;
    expectedVale[2] = 1.020401;
    expectedVale[3] = 0.72924399;
    expectedVale[4] = 1.0185170801;
    expectedVale[5] = 0.858817693999;
    expectedVale[6] = 1.01374826481201;
    expectedVale[7] = 0.92626254743036;

    *gamMemoryInM = 0;
    *gamMemoryInR = 1;
    for (uint32 i = 0u; i < maxRep && ret; i++) {
        //set inputs
        gam.Execute();
        //Check output
        ret &= gam.IsEqualLargerMargins(*gamMemoryOut, expectedVale[i]);
        if (!ret) {
            printf("output value  = %.16lf.\nexpectedValue = %.16lf. index = %u \n", *gamMemoryOut, expectedVale[i], i);
        }
        *gamMemoryInM = *gamMemoryOut;
    }
    return ret;
}

bool PIDGAMTest::TestExecutekpkikdCompareBoth() {
    bool ret;
    PIDGAMTestHelper gam2(0.5, 500, 0.00001, 0.001);
    ret = gam2.HelperInitialise();
    ret &= gam2.Initialise(gam2.config);

    ret &= gam2.HelperSetup2();
    ret &= gam2.SetConfiguredDatabase(gam2.configSignals);
    ret &= gam2.AllocateInputSignalsMemory();
    ret &= gam2.AllocateOutputSignalsMemory();
    ret &= gam2.Setup();

    float64 *gamMemoryInR2 = static_cast<float64 *>(gam2.GetInputSignalsMemory());
    float64 *gamMemoryInM2 = static_cast<float64 *>(gam2.GetInputSignalsMemory(1));
    float64 *gamMemoryOut2 = static_cast<float64 *>(gam2.GetOutputSignalsMemory());
    uint32 maxRep = 8000;


    PIDGAMTestHelper gam1(0.5, 500, 0.00001, 0.001);
    ret = gam1.HelperInitialise();
    ret &= gam1.Initialise(gam1.config);

    ret &= gam1.HelperSetup2();
    ret &= gam1.SetConfiguredDatabase(gam1.configSignals);
    ret &= gam1.AllocateInputSignalsMemory();
    ret &= gam1.AllocateOutputSignalsMemory();
    ret &= gam1.Setup();

    float64 *gamMemoryInError1 = static_cast<float64 *>(gam1.GetInputSignalsMemory());
    float64 *gamMemoryOut1 = static_cast<float64 *>(gam1.GetOutputSignalsMemory());


    *gamMemoryInM2 = 0;
    *gamMemoryInR2 = 1;
    for (uint32 i = 0u; i < maxRep && ret; i++) {
        //set inputs
        *gamMemoryInError1 = *gamMemoryInR2 - *gamMemoryOut1;
        gam2.Execute();
        gam1.Execute();
        //Check output
        ret &= gam2.IsEqualLargerMargins(*gamMemoryOut2, *gamMemoryOut1);
        if (!ret) {
            printf("gamMemoryOut2 = %.17lf. gamMemoryOut1 = %.17lf. index = %u \n", *gamMemoryOut2, *gamMemoryOut1, i);
        }
        *gamMemoryInM2 = *gamMemoryOut2;
    }
    return ret;
}

bool PIDGAMTest::TestExecuteSaturationkp() {
    bool ret;
    PIDGAMTestHelper gam(1, 0, 0, 0.001, 0.1, -0.1);
    ret = gam.HelperInitialise();
    ret &= gam.Initialise(gam.config);

    ret &= gam.HelperSetup2();
    ret &= gam.SetConfiguredDatabase(gam.configSignals);
    ret &= gam.AllocateInputSignalsMemory();
    ret &= gam.AllocateOutputSignalsMemory();
    ret &= gam.Setup();

    float64 *gamMemoryInR = static_cast<float64 *>(gam.GetInputSignalsMemory());
    float64 *gamMemoryInM = static_cast<float64 *>(gam.GetInputSignalsMemory(1));
    float64 *gamMemoryOut = static_cast<float64 *>(gam.GetOutputSignalsMemory());
    uint32 maxRep = 4;
    float64 *expectedVale = new float64[maxRep];
    //generate output
    expectedVale[0] = 0.1;
    expectedVale[1] = 0.1;
    expectedVale[2] = 0.1;
    expectedVale[3] = 0.1;

    *gamMemoryInM = 0;
    *gamMemoryInR = 1;
    for (uint32 i = 0u; i < maxRep && ret; i++) {
        //set inputs
        gam.Execute();
        //Check output
        ret &= gam.IsEqualLargerMargins(*gamMemoryOut, expectedVale[i]);
        if (!ret) {
            printf("output value = %.17lf. expectedValue = %.17lf. index = %u \n", *gamMemoryOut, expectedVale[i], i);
        }
        *gamMemoryInM = *gamMemoryOut;
    }
    return ret;
}

bool PIDGAMTest::TestExecuteSaturationki() {
    bool ret;
    PIDGAMTestHelper gam(0, 500, 0, 0.001, 0.2, -0.2);
    ret = gam.HelperInitialise();
    ret &= gam.Initialise(gam.config);

    ret &= gam.HelperSetup2();
    ret &= gam.SetConfiguredDatabase(gam.configSignals);
    ret &= gam.AllocateInputSignalsMemory();
    ret &= gam.AllocateOutputSignalsMemory();
    ret &= gam.Setup();

    float64 *gamMemoryInR = static_cast<float64 *>(gam.GetInputSignalsMemory());
    float64 *gamMemoryInM = static_cast<float64 *>(gam.GetInputSignalsMemory(1));
    float64 *gamMemoryOut = static_cast<float64 *>(gam.GetOutputSignalsMemory());
    uint32 maxRep = 6;
    float64 *expectedVale = new float64[maxRep];
    //generate output
    expectedVale[0] = 0.2;
    expectedVale[1] = 0.2;
    expectedVale[2] = -0.1;
    expectedVale[3] = -0.05;
    expectedVale[4] = -0.025;
    expectedVale[5] = -0.0125;

    *gamMemoryInM = 0;
    *gamMemoryInR = 1;
    for (uint32 i = 0u; i < 2 && ret; i++) {
        //set inputs
        gam.Execute();
        //Check output
        ret &= gam.IsEqualLargerMargins(*gamMemoryOut, expectedVale[i]);
        if (!ret) {
            printf("output value = %.17lf. expectedValue = %.17lf. index = %u \n", *gamMemoryOut, expectedVale[i], i);
        }
        *gamMemoryInM = *gamMemoryOut;
    }
    *gamMemoryInR = 0;
    for (uint32 i = 2u; i < maxRep && ret; i++) {
        //set inputs
        gam.Execute();
        //Check output
        ret &= gam.IsEqualLargerMargins(*gamMemoryOut, expectedVale[i]);
        if (!ret) {
            printf("output value = %.17lf. expectedValue = %.17lf. index = %u \n", *gamMemoryOut, expectedVale[i], i);
        }
        *gamMemoryInM = *gamMemoryOut;
    }
    return ret;
}

bool PIDGAMTest::TestExecuteSaturationki2Subtract() {
    bool ret;
    PIDGAMTestHelper gam(0, 500, 0, 0.001, 0.2, -0.2);
    ret = gam.HelperInitialise();
    ret &= gam.Initialise(gam.config);

    ret &= gam.HelperSetup2();
    ret &= gam.SetConfiguredDatabase(gam.configSignals);
    ret &= gam.AllocateInputSignalsMemory();
    ret &= gam.AllocateOutputSignalsMemory();
    ret &= gam.Setup();

    float64 *gamMemoryInR = static_cast<float64 *>(gam.GetInputSignalsMemory());
    float64 *gamMemoryInM = static_cast<float64 *>(gam.GetInputSignalsMemory(1));
    float64 *gamMemoryOut = static_cast<float64 *>(gam.GetOutputSignalsMemory());
    uint32 maxRep = 7;
    float64 *expectedVale = new float64[maxRep];
    //generate output
    expectedVale[0] = 0.2;
    expectedVale[1] = 0.2;
    expectedVale[2] = 0.2;
    expectedVale[3] = -0.1;
    expectedVale[4] = -0.05;
    expectedVale[5] = -0.025;
    expectedVale[6] = -0.0125;

    *gamMemoryInM = 0;
    *gamMemoryInR = 1;
    for (uint32 i = 0u; i < 3 && ret; i++) {
        //set inputs
        gam.Execute();
        //Check output
        ret &= gam.IsEqualLargerMargins(*gamMemoryOut, expectedVale[i]);
        if (!ret) {
            printf("output value = %.17lf. expectedValue = %.17lf. index = %u \n", *gamMemoryOut, expectedVale[i], i);
        }
        *gamMemoryInM = *gamMemoryOut;
    }
    *gamMemoryInR = 0;
    for (uint32 i = 3u; i < maxRep && ret; i++) {
        //set inputs
        gam.Execute();
        //Check output
        ret &= gam.IsEqualLargerMargins(*gamMemoryOut, expectedVale[i]);
        if (!ret) {
            printf("output value = %.17lf. expectedValue = %.17lf. index = %u \n", *gamMemoryOut, expectedVale[i], i);
        }
        *gamMemoryInM = *gamMemoryOut;
    }
    return ret;
}

bool PIDGAMTest::TestExecuteSaturationki3Subtract() {
    bool ret;
    PIDGAMTestHelper gam(0, 500, 0, 0.001, 0.2, -0.2);
    ret = gam.HelperInitialise();
    ret &= gam.Initialise(gam.config);

    ret &= gam.HelperSetup2();
    ret &= gam.SetConfiguredDatabase(gam.configSignals);
    ret &= gam.AllocateInputSignalsMemory();
    ret &= gam.AllocateOutputSignalsMemory();
    ret &= gam.Setup();

    float64 *gamMemoryInR = static_cast<float64 *>(gam.GetInputSignalsMemory());
    float64 *gamMemoryInM = static_cast<float64 *>(gam.GetInputSignalsMemory(1));
    float64 *gamMemoryOut = static_cast<float64 *>(gam.GetOutputSignalsMemory());
    uint32 maxRep = 7;
    float64 *expectedVale = new float64[maxRep];
    //generate output
    expectedVale[0] = -0.2;
    expectedVale[1] = -0.2;
    expectedVale[2] = -0.2;
    expectedVale[3] = 0.1;
    expectedVale[4] = 0.05;
    expectedVale[5] = 0.025;
    expectedVale[6] = 0.0125;

    *gamMemoryInM = 0;
    *gamMemoryInR = -1;
    for (uint32 i = 0u; i < 3 && ret; i++) {
        //set inputs
        gam.Execute();
        //Check output
        ret &= gam.IsEqualLargerMargins(*gamMemoryOut, expectedVale[i]);
        if (!ret) {
            printf("output value = %.17lf. expectedValue = %.17lf. index = %u \n", *gamMemoryOut, expectedVale[i], i);
        }
        *gamMemoryInM = *gamMemoryOut;
    }
    *gamMemoryInR = 0;
    for (uint32 i = 3u; i < maxRep && ret; i++) {
        //set inputs
        gam.Execute();
        //Check output
        ret &= gam.IsEqualLargerMargins(*gamMemoryOut, expectedVale[i]);
        if (!ret) {
            printf("output value = %.17lf. expectedValue = %.17lf. index = %u \n", *gamMemoryOut, expectedVale[i], i);
        }
        *gamMemoryInM = *gamMemoryOut;
    }
    return ret;
}

bool PIDGAMTest::TestExecuteSaturationki2() {
    bool ret;
    PIDGAMTestHelper gam(0, 500, 0, 0.001, 0.2, -0.2);
    ret = gam.HelperInitialise();
    ret &= gam.Initialise(gam.config);

    ret &= gam.HelperSetup1();
    ret &= gam.SetConfiguredDatabase(gam.configSignals);
    ret &= gam.AllocateInputSignalsMemory();
    ret &= gam.AllocateOutputSignalsMemory();
    ret &= gam.Setup();

    float64 *gamMemoryInR = static_cast<float64 *>(gam.GetInputSignalsMemory());
    float64 *gamMemoryOut = static_cast<float64 *>(gam.GetOutputSignalsMemory());
    uint32 maxRep = 7;
    float64 *expectedVale = new float64[maxRep];
    //generate output
    expectedVale[0] = 0.2;
    expectedVale[1] = 0.2;
    expectedVale[2] = 0.2;
    expectedVale[3] = -0.1;
    expectedVale[4] = -0.05;
    expectedVale[5] = -0.025;
    expectedVale[6] = -0.0125;

    *gamMemoryOut = 0.0;
    for (uint32 i = 0u; i < 3 && ret; i++) {
        //set inputs
        *gamMemoryInR = 1.0 - *gamMemoryOut;
        gam.Execute();
        //Check output
        ret &= gam.IsEqualLargerMargins(*gamMemoryOut, expectedVale[i]);
        if (!ret) {
            printf("output value = %.17lf. expectedValue = %.17lf. index = %u \n", *gamMemoryOut, expectedVale[i], i);
        }
    }

    for (uint32 i = 3u; i < maxRep && ret; i++) {
        //set inputs
        *gamMemoryInR = 0 - *gamMemoryOut;
        gam.Execute();
        //Check output
        ret &= gam.IsEqualLargerMargins(*gamMemoryOut, expectedVale[i]);
        if (!ret) {
            printf("output value = %.17lf. expectedValue = %.17lf. index = %u \n", *gamMemoryOut, expectedVale[i], i);
        }
    }
    return ret;
}

bool PIDGAMTest::TestExecuteSaturationki3() {
    bool ret;
    PIDGAMTestHelper gam(0, 500, 0, 0.001, 0.2, -0.2);
    ret = gam.HelperInitialise();
    ret &= gam.Initialise(gam.config);

    ret &= gam.HelperSetup1();
    ret &= gam.SetConfiguredDatabase(gam.configSignals);
    ret &= gam.AllocateInputSignalsMemory();
    ret &= gam.AllocateOutputSignalsMemory();
    ret &= gam.Setup();

    float64 *gamMemoryInR = static_cast<float64 *>(gam.GetInputSignalsMemory());
    float64 *gamMemoryOut = static_cast<float64 *>(gam.GetOutputSignalsMemory());
    uint32 maxRep = 7;
    float64 *expectedVale = new float64[maxRep];
    //generate output
    expectedVale[0] = -0.2;
    expectedVale[1] = -0.2;
    expectedVale[2] = -0.2;
    expectedVale[3] = 0.1;
    expectedVale[4] = 0.05;
    expectedVale[5] = 0.025;
    expectedVale[6] = 0.0125;

    *gamMemoryOut = 0.0;
    for (uint32 i = 0u; i < 3 && ret; i++) {
        //set inputs
        *gamMemoryInR = -1.0 - *gamMemoryOut;
        gam.Execute();
        //Check output
        ret &= gam.IsEqualLargerMargins(*gamMemoryOut, expectedVale[i]);
        if (!ret) {
            printf("output value = %.17lf. expectedValue = %.17lf. index = %u \n", *gamMemoryOut, expectedVale[i], i);
        }
    }

    for (uint32 i = 3u; i < maxRep && ret; i++) {
        //set inputs
        *gamMemoryInR = 0 - *gamMemoryOut;
        gam.Execute();
        //Check output
        ret &= gam.IsEqualLargerMargins(*gamMemoryOut, expectedVale[i]);
        if (!ret) {
            printf("output value = %.17lf. expectedValue = %.17lf. index = %u \n", *gamMemoryOut, expectedVale[i], i);
        }
    }
    return ret;
}

}

