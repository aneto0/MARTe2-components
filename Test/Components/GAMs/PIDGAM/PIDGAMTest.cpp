/**
 * @file PIDGAMTest.cpp
 * @brief Source file for class PIDGAMTest
 * @date Aug 22, 2017 TODO Verify the value and format of the date
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
    CLASS_REGISTER_DECLARATION()PIDGAMTestHelper(double kpDef = 1.0,
            double kiDef = 1.2,
            double kdDef = 1.3,
            double sampleTimeDef = 0.001) {
        kp = kpDef;
        ki = kiDef;
        kd = kdDef;
        sampleTime = sampleTimeDef;
    }

    bool HelperInitialise() {
        bool ok;
        double kp = 1.1;
        ok = config.Write("Kp", kp);
        double sampleTime = 0.001;
        ok &= config.Write("SampleTime", sampleTime);
        double maxOutput = 1.5;
        ok &= config.Write("MaxOutput", maxOutput);
        double minOutput = -1.5;
        ok &= config.Write("MinOutput", minOutput);
        return ok;
    }

    ConfigurationDatabase config;
private :
    double kp;
    double ki;
    double kd;
    double sampleTime;

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
    double kp = 1.1;
    config.Write("Kp", kp);
    ret = !gam.Initialise(config);
    return ret;
}

bool PIDGAMTest::TestInitialiseWrongSampleTime() {
    PIDGAM gam;
    bool ret;
    ConfigurationDatabase config;
    double kp = 1.1;
    config.Write("Kp", kp);
    double sampleTime = 0.0;
    config.Write("SampleTime", sampleTime);
    ret = !gam.Initialise(config);
    return ret;
}

bool PIDGAMTest::TestInitialiseWrongSaturationLimits() {
    PIDGAM gam;
    bool ret;
    ConfigurationDatabase config;
    double kp = 1.1;
    config.Write("Kp", kp);
    double sampleTime = 0.001;
    config.Write("SampleTime", sampleTime);
    double maxOutput = 1.5;
    config.Write("MaxOutput", maxOutput);
    double minOutput = 1.7;
    config.Write("MinOutput", minOutput);
    ret = !gam.Initialise(config);
    return ret;
}

bool PIDGAMTest::TestInitialiseWrongSaturationLimits2() {
    PIDGAM gam;
    bool ret;
    ConfigurationDatabase config;
    double kp = 1.1;
    config.Write("Kp", kp);
    double sampleTime = 0.001;
    config.Write("SampleTime", sampleTime);
    double maxOutput = 1.5;
    config.Write("MaxOutput", maxOutput);
    double minOutput = 1.5;
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

bool PIDGAMTest::TestInitialiseWrongNumberOfInputs() {
    bool ret;
    PIDGAMTestHelper gam;
    ret = gam.HelperInitialise();
    ret &= gam.Initialise(gam.config);
    ret &= !gam.Setup();
    return ret;
}

bool PIDGAMTest::TestInitialiseWrongNumberOfInputs2() {
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
    printf("ret = %d\n", ret);
    if(ret){
        ret &= !gam.Setup();
    }
    return ret;
}

}

