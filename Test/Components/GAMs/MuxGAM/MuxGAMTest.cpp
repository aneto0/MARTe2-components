/**
 * @file MuxGAMTest.cpp
 * @brief Source file for class MuxGAMTest
 * @date 28/08/2017
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
 * the class MuxGAMTest (public, protected, and private). Be aware that some 
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

#include "MuxGAMTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace {

}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

MuxGAMTest::MuxGAMTest() {
//Auto-generated constructor stub for MuxGAMTest

//TODO Verify if manual additions are needed here
}

MuxGAMTest::~MuxGAMTest() {
//Auto-generated destructor stub for MuxGAMTest

//TODO Verify if manual additions are needed here
}

bool MuxGAMTest::TestInitialise() {
    ConfigurationDatabase config;
    MuxGAM gam;
    bool ok = gam.Initialise(config);
    return ok;
}

bool MuxGAMTest::TestSetupNoOutputs() {
    ConfigurationDatabase config;
    MuxGAM gam;
    bool ok = gam.Initialise(config);
    ConfigurationDatabase configSignals;
    ok &= gam.SetConfiguredDatabase(configSignals);
    if (ok) {
        ok &= !gam.Setup();
    }
    return ok;
}

bool MuxGAMTest::TestSetupNoInputs() {
    ConfigurationDatabase config;
    MuxGAM gam;
    bool ok = gam.Initialise(config);
    ConfigurationDatabase configSignals;
    ok &= configSignals.CreateAbsolute("Signals.OutputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 4);
    ok &= configSignals.MoveToRoot();
    /*
     //example how to print a ConfigurationDatabase
     StreamString a;
     printf("%d\n", a.Printf("%!", configSignals));
     printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
     printf("%s\n", a.Buffer());
     printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
     */
    ok &= gam.SetConfiguredDatabase(configSignals);
    if (ok) {
        ok &= !gam.Setup();
    }
    return ok;
}

bool MuxGAMTest::TestSetupWrongInputs() {
    ConfigurationDatabase config;
    MuxGAM gam;
    bool ok = gam.Initialise(config);
    ConfigurationDatabase configSignals;
    ok &= configSignals.CreateAbsolute("Signals.OutputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 4);

    ok &= configSignals.CreateAbsolute("Signals.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 4);
    ok &= configSignals.MoveToRoot();

    ok &= gam.SetConfiguredDatabase(configSignals);
    if (ok) {
        ok &= !gam.Setup();
    }
    return ok;
}

bool MuxGAMTest::TestSetupWrongInputs2() {
    ConfigurationDatabase config;
    MuxGAM gam;
    bool ok = gam.Initialise(config);
    ConfigurationDatabase configSignals;
    ok &= configSignals.CreateAbsolute("Signals.OutputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 4);

    ok &= configSignals.CreateAbsolute("Signals.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 4);
    ok &= configSignals.MoveToRoot();

    ok &= gam.SetConfiguredDatabase(configSignals);
    if (ok) {
        ok &= !gam.Setup();
    }
    return ok;
}

bool MuxGAMTest::TestSetupWrongInputs3() {
    ConfigurationDatabase config;
    MuxGAM gam;
    bool ok = gam.Initialise(config);
    ConfigurationDatabase configSignals;
    ok &= configSignals.CreateAbsolute("Signals.OutputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 4);

    ok &= configSignals.CreateAbsolute("Signals.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 4);
    ok &= configSignals.MoveToRoot();

    ok &= gam.SetConfiguredDatabase(configSignals);
    if (ok) {
        ok &= !gam.Setup();
    }
    return ok;
}

bool MuxGAMTest::TestSetupNoSelectorType() {
    ConfigurationDatabase config;
    MuxGAM gam;
    bool ok = gam.Initialise(config);
    ConfigurationDatabase configSignals;
    ok &= configSignals.CreateAbsolute("Signals.OutputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 4);

    ok &= configSignals.CreateAbsolute("Signals.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("2");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 4);
    ok &= configSignals.MoveToRoot();

    ok &= gam.SetConfiguredDatabase(configSignals);
    if (ok) {
        ok &= !gam.Setup();
    }
    return ok;
}

bool MuxGAMTest::TestSetupWrongSelectorType() {
    ConfigurationDatabase config;
    MuxGAM gam;
    bool ok = gam.Initialise(config);
    ConfigurationDatabase configSignals;
    ok &= configSignals.CreateAbsolute("Signals.OutputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 4);

    ok &= configSignals.CreateAbsolute("Signals.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Type", "uint16");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("2");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 4);
    ok &= configSignals.MoveToRoot();

    ok &= gam.SetConfiguredDatabase(configSignals);
    if (ok) {
        ok &= !gam.Setup();
    }
    return ok;
}

bool MuxGAMTest::TestSetupWrongInputType() {
    ConfigurationDatabase config;
    MuxGAM gam;
    bool ok = gam.Initialise(config);
    ConfigurationDatabase configSignals;
    ok &= configSignals.CreateAbsolute("Signals.OutputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 4);
    ok &= configSignals.CreateAbsolute("Signals.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Type", "uint32");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("2");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 4);
    ok &= configSignals.MoveToRoot();
    ok &= gam.SetConfiguredDatabase(configSignals);
    if (ok) {
        ok &= !gam.Setup();
    }
    return ok;
}

bool MuxGAMTest::TestSetupWrongSelectorType2() {
    ConfigurationDatabase config;
    MuxGAM gam;
    bool ok = gam.Initialise(config);
    ConfigurationDatabase configSignals;
    ok &= configSignals.CreateAbsolute("Signals.OutputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 4);
    ok &= configSignals.CreateAbsolute("Signals.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Type", "uint32");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("2");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("3");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 4);
    ok &= configSignals.MoveToRoot();
    ok &= gam.SetConfiguredDatabase(configSignals);
    if (ok) {
        ok &= !gam.Setup();
    }
    return ok;
}

bool MuxGAMTest::TestSetupWrongInputType2() {
    ConfigurationDatabase config;
    MuxGAM gam;
    bool ok = gam.Initialise(config);
    ConfigurationDatabase configSignals;
    ok &= configSignals.CreateAbsolute("Signals.OutputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 4);
    ok &= configSignals.CreateAbsolute("Signals.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Type", "uint32");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("Type", "uint32");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("2");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("3");
    ok &= configSignals.Write("Type", "float64");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 4);
    ok &= configSignals.MoveToRoot();
    ok &= gam.SetConfiguredDatabase(configSignals);
    if (ok) {
        ok &= !gam.Setup();
    }
    return ok;
}

bool MuxGAMTest::TestSetupWrongOutputType() {
    ConfigurationDatabase config;
    MuxGAM gam;
    bool ok = gam.Initialise(config);
    ConfigurationDatabase configSignals;
    ok &= configSignals.CreateAbsolute("Signals.OutputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Type", "int32");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 4);
    ok &= configSignals.CreateAbsolute("Signals.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Type", "uint32");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("Type", "uint32");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("2");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("3");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 4);
    ok &= configSignals.MoveToRoot();
    ok &= gam.SetConfiguredDatabase(configSignals);
    if (ok) {
        ok &= !gam.Setup();
    }
    return ok;
}

bool MuxGAMTest::TestSetupWrongOutputType2() {
    ConfigurationDatabase config;
    MuxGAM gam;
    bool ok = gam.Initialise(config);
    ConfigurationDatabase configSignals;
    ok &= configSignals.CreateAbsolute("Signals.OutputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("Type", "int32");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 4);
    ok &= configSignals.CreateAbsolute("Signals.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Type", "uint32");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("Type", "uint32");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("2");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("3");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 4);
    ok &= configSignals.MoveToRoot();
    ok &= gam.SetConfiguredDatabase(configSignals);
    if (ok) {
        ok &= !gam.Setup();
    }
    return ok;
}

bool MuxGAMTest::TestSetupNoOutputElements() {
    ConfigurationDatabase config;
    MuxGAM gam;
    bool ok = gam.Initialise(config);
    ConfigurationDatabase configSignals;
    ok &= configSignals.CreateAbsolute("Signals.OutputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 4);
    ok &= configSignals.CreateAbsolute("Signals.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Type", "uint32");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("Type", "uint32");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("2");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("3");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 4);
    ok &= configSignals.MoveToRoot();
    ok &= gam.SetConfiguredDatabase(configSignals);
    if (ok) {
        ok &= !gam.Setup();
    }
    return ok;
}


bool MuxGAMTest::TestSetupNoOutputElements_2() {
    ConfigurationDatabase config;
    MuxGAM gam;
    bool ok = gam.Initialise(config);
    ConfigurationDatabase configSignals;
    ok &= configSignals.CreateAbsolute("Signals.OutputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.Write("NumberOfElements", 1);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 4);
    ok &= configSignals.CreateAbsolute("Signals.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Type", "uint32");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("Type", "uint32");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("2");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("3");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 4);
    ok &= configSignals.MoveToRoot();
    ok &= gam.SetConfiguredDatabase(configSignals);
    if (ok) {
        ok &= !gam.Setup();
    }
    return ok;
}



bool MuxGAMTest::TestSetupWrongOutputElements() {
    ConfigurationDatabase config;
    MuxGAM gam;
    bool ok = gam.Initialise(config);
    ConfigurationDatabase configSignals;
    ok &= configSignals.CreateAbsolute("Signals.OutputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.Write("NumberOfElements", 0);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 4);
    ok &= configSignals.CreateAbsolute("Signals.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Type", "uint32");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("Type", "uint32");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("2");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("3");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 4);
    ok &= configSignals.MoveToRoot();
    ok &= gam.SetConfiguredDatabase(configSignals);
    if (ok) {
        ok &= !gam.Setup();
    }
    return ok;
}

bool MuxGAMTest::TestSetupWrongOutputElements2() {
    ConfigurationDatabase config;
    MuxGAM gam;
    bool ok = gam.Initialise(config);
    ConfigurationDatabase configSignals;
    ok &= configSignals.CreateAbsolute("Signals.OutputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.Write("NumberOfElements", 3);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.Write("NumberOfElements", 6);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 4);
    ok &= configSignals.CreateAbsolute("Signals.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Type", "uint32");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("Type", "uint32");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("2");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("3");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 4);
    ok &= configSignals.MoveToRoot();
    ok &= gam.SetConfiguredDatabase(configSignals);
    if (ok) {
        ok &= !gam.Setup();
    }
    return ok;
}

bool MuxGAMTest::TestSetupNoSelectorElements() {
    ConfigurationDatabase config;
    MuxGAM gam;
    bool ok = gam.Initialise(config);
    ConfigurationDatabase configSignals;
    ok &= configSignals.CreateAbsolute("Signals.OutputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.Write("NumberOfElements", 3);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.Write("NumberOfElements", 3);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 4);

    ok &= configSignals.CreateAbsolute("Signals.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Type", "uint32");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("Type", "uint32");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("2");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("3");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 4);
    ok &= configSignals.MoveToRoot();
    ok &= gam.SetConfiguredDatabase(configSignals);
    if (ok) {
        ok &= !gam.Setup();
    }
    return ok;
}

bool MuxGAMTest::TestSetupWrongSelectorElements() {
    ConfigurationDatabase config;
    MuxGAM gam;
    bool ok = gam.Initialise(config);
    ConfigurationDatabase configSignals;
    ok &= configSignals.CreateAbsolute("Signals.OutputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.Write("NumberOfElements", 3);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.Write("NumberOfElements", 3);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 4);

    ok &= configSignals.CreateAbsolute("Signals.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Type", "uint32");
    ok &= configSignals.Write("NumberOfElements", 2);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("Type", "uint32");
    ok &= configSignals.Write("NumberOfElements", 3);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("2");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("3");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 4);
    ok &= configSignals.MoveToRoot();
    ok &= gam.SetConfiguredDatabase(configSignals);
    if (ok) {
        ok &= !gam.Setup();
    }
    return ok;
}

bool MuxGAMTest::TestSetupWrongSelectorElements2() {
    ConfigurationDatabase config;
    MuxGAM gam;
    bool ok = gam.Initialise(config);
    ConfigurationDatabase configSignals;
    ok &= configSignals.CreateAbsolute("Signals.OutputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.Write("NumberOfElements", 3);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.Write("NumberOfElements", 3);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 4);

    ok &= configSignals.CreateAbsolute("Signals.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Type", "uint32");
    ok &= configSignals.Write("NumberOfElements", 1);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("Type", "uint32");
    ok &= configSignals.Write("NumberOfElements", 3);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("2");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("3");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 4);
    ok &= configSignals.MoveToRoot();
    ok &= gam.SetConfiguredDatabase(configSignals);
    if (ok) {
        ok &= !gam.Setup();
    }
    return ok;
}

bool MuxGAMTest::TestSetupNoInputElements() {
    ConfigurationDatabase config;
    MuxGAM gam;
    bool ok = gam.Initialise(config);
    ConfigurationDatabase configSignals;
    ok &= configSignals.CreateAbsolute("Signals.OutputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.Write("NumberOfElements", 3);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.Write("NumberOfElements", 3);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 4);

    ok &= configSignals.CreateAbsolute("Signals.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Type", "uint32");
    ok &= configSignals.Write("NumberOfElements", 1);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("Type", "uint32");
    ok &= configSignals.Write("NumberOfElements", 1);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("2");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("3");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 4);
    ok &= configSignals.MoveToRoot();
    ok &= gam.SetConfiguredDatabase(configSignals);
    if (ok) {
        ok &= !gam.Setup();
    }
    return ok;
}

bool MuxGAMTest::TestSetupWrongInputElements() {
    ConfigurationDatabase config;
    MuxGAM gam;
    bool ok = gam.Initialise(config);
    ConfigurationDatabase configSignals;
    ok &= configSignals.CreateAbsolute("Signals.OutputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.Write("NumberOfElements", 3);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.Write("NumberOfElements", 3);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 4);

    ok &= configSignals.CreateAbsolute("Signals.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Type", "uint32");
    ok &= configSignals.Write("NumberOfElements", 1);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("Type", "uint32");
    ok &= configSignals.Write("NumberOfElements", 1);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("2");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.Write("NumberOfElements", 5);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("3");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 4);
    ok &= configSignals.MoveToRoot();
    ok &= gam.SetConfiguredDatabase(configSignals);
    if (ok) {
        ok &= !gam.Setup();
    }
    return ok;
}

bool MuxGAMTest::TestSetupWrongInputElements2() {
    ConfigurationDatabase config;
    MuxGAM gam;
    bool ok = gam.Initialise(config);
    ConfigurationDatabase configSignals;
    ok &= configSignals.CreateAbsolute("Signals.OutputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.Write("NumberOfElements", 3);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.Write("NumberOfElements", 3);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 4);

    ok &= configSignals.CreateAbsolute("Signals.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Type", "uint32");
    ok &= configSignals.Write("NumberOfElements", 1);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("Type", "uint32");
    ok &= configSignals.Write("NumberOfElements", 1);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("2");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.Write("NumberOfElements", 3);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("3");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.Write("NumberOfElements", 23);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 4);
    ok &= configSignals.MoveToRoot();
    ok &= gam.SetConfiguredDatabase(configSignals);
    if (ok) {
        ok &= !gam.Setup();
    }
    return ok;
}

bool MuxGAMTest::TestSetupNoInputDiemnsion() {
    ConfigurationDatabase config;
    MuxGAM gam;
    bool ok = gam.Initialise(config);
    ConfigurationDatabase configSignals;
    ok &= configSignals.CreateAbsolute("Signals.OutputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.Write("NumberOfElements", 3);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.Write("NumberOfElements", 3);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 4);

    ok &= configSignals.CreateAbsolute("Signals.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Type", "uint32");
    ok &= configSignals.Write("NumberOfElements", 1);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("Type", "uint32");
    ok &= configSignals.Write("NumberOfElements", 1);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("2");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.Write("NumberOfElements", 3);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("3");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.Write("NumberOfElements", 3);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 4);
    ok &= configSignals.MoveToRoot();
    ok &= gam.SetConfiguredDatabase(configSignals);
    if (ok) {
        ok &= !gam.Setup();
    }
    return ok;
}

bool MuxGAMTest::TestSetupWrongInputDiemnsion() {
    ConfigurationDatabase config;
    MuxGAM gam;
    bool ok = gam.Initialise(config);
    ConfigurationDatabase configSignals;
    ok &= configSignals.CreateAbsolute("Signals.OutputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.Write("NumberOfElements", 3);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.Write("NumberOfElements", 3);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 4);

    ok &= configSignals.CreateAbsolute("Signals.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Type", "uint32");
    ok &= configSignals.Write("NumberOfElements", 1);
    ok &= configSignals.Write("NumberOfDimensions", 11);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("Type", "uint32");
    ok &= configSignals.Write("NumberOfElements", 1);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("2");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.Write("NumberOfElements", 3);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("3");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.Write("NumberOfElements", 3);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 4);
    ok &= configSignals.MoveToRoot();
    ok &= gam.SetConfiguredDatabase(configSignals);
    if (ok) {
        ok &= !gam.Setup();
    }
    return ok;
}

bool MuxGAMTest::TestSetupWrongInputDiemnsion2() {
    ConfigurationDatabase config;
    MuxGAM gam;
    bool ok = gam.Initialise(config);
    ConfigurationDatabase configSignals;
    ok &= configSignals.CreateAbsolute("Signals.OutputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.Write("NumberOfElements", 3);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.Write("NumberOfElements", 3);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 4);

    ok &= configSignals.CreateAbsolute("Signals.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Type", "uint32");
    ok &= configSignals.Write("NumberOfElements", 1);
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("Type", "uint32");
    ok &= configSignals.Write("NumberOfElements", 1);
    ok &= configSignals.Write("NumberOfDimensions", 11);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("2");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.Write("NumberOfElements", 3);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("3");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.Write("NumberOfElements", 3);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 4);
    ok &= configSignals.MoveToRoot();
    ok &= gam.SetConfiguredDatabase(configSignals);
    if (ok) {
        ok &= !gam.Setup();
    }
    return ok;
}

bool MuxGAMTest::TestSetupNoOutputDiemnsion() {
    ConfigurationDatabase config;
    MuxGAM gam;
    bool ok = gam.Initialise(config);
    ConfigurationDatabase configSignals;
    ok &= configSignals.CreateAbsolute("Signals.OutputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.Write("NumberOfElements", 3);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.Write("NumberOfElements", 3);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 4);

    ok &= configSignals.CreateAbsolute("Signals.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Type", "uint32");
    ok &= configSignals.Write("NumberOfElements", 1);
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("Type", "uint32");
    ok &= configSignals.Write("NumberOfElements", 1);
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("2");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.Write("NumberOfElements", 3);
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("3");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.Write("NumberOfElements", 3);
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 4);
    ok &= configSignals.MoveToRoot();
    ok &= gam.SetConfiguredDatabase(configSignals);
    if (ok) {
        ok &= !gam.Setup();
    }
    return ok;
}

bool MuxGAMTest::TestSetupWrongOutputDiemnsion() {
    ConfigurationDatabase config;
    MuxGAM gam;
    bool ok = gam.Initialise(config);
    ConfigurationDatabase configSignals;
    ok &= configSignals.CreateAbsolute("Signals.OutputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.Write("NumberOfElements", 3);
    ok &= configSignals.Write("NumberOfDimensions", 2);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.Write("NumberOfElements", 3);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 4);

    ok &= configSignals.CreateAbsolute("Signals.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Type", "uint32");
    ok &= configSignals.Write("NumberOfElements", 1);
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("Type", "uint32");
    ok &= configSignals.Write("NumberOfElements", 1);
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("2");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.Write("NumberOfElements", 3);
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("3");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.Write("NumberOfElements", 3);
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 4);
    ok &= configSignals.MoveToRoot();
    ok &= gam.SetConfiguredDatabase(configSignals);
    if (ok) {
        ok &= !gam.Setup();
    }
    return ok;
}

bool MuxGAMTest::TestSetupWrongOutputDiemnsion2() {
    ConfigurationDatabase config;
    MuxGAM gam;
    bool ok = gam.Initialise(config);
    ConfigurationDatabase configSignals;
    ok &= configSignals.CreateAbsolute("Signals.OutputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.Write("NumberOfElements", 3);
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.Write("NumberOfElements", 3);
    ok &= configSignals.Write("NumberOfDimensions", 2);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 4);

    ok &= configSignals.CreateAbsolute("Signals.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Type", "uint32");
    ok &= configSignals.Write("NumberOfElements", 1);
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("Type", "uint32");
    ok &= configSignals.Write("NumberOfElements", 1);
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("2");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.Write("NumberOfElements", 3);
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("3");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.Write("NumberOfElements", 3);
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 4);
    ok &= configSignals.MoveToRoot();
    ok &= gam.SetConfiguredDatabase(configSignals);
    if (ok) {
        ok &= !gam.Setup();
    }
    return ok;
}

bool MuxGAMTest::TestSetupNoInputSamples() {
    ConfigurationDatabase config;
    MuxGAM gam;
    bool ok = gam.Initialise(config);
    ConfigurationDatabase configSignals;
    ok &= configSignals.CreateAbsolute("Signals.OutputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.Write("NumberOfElements", 3);
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.Write("NumberOfElements", 3);
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 4);

    ok &= configSignals.CreateAbsolute("Signals.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Type", "uint32");
    ok &= configSignals.Write("NumberOfElements", 1);
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("Type", "uint32");
    ok &= configSignals.Write("NumberOfElements", 1);
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("2");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.Write("NumberOfElements", 3);
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("3");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.Write("NumberOfElements", 3);
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 4);
    ok &= configSignals.MoveToRoot();
    ok &= gam.SetConfiguredDatabase(configSignals);
    if (ok) {
        ok &= !gam.Setup();
    }
    return ok;
}

bool MuxGAMTest::TestSetupWrongInputSamples() {
    ConfigurationDatabase config;
    MuxGAM gam;
    bool ok = gam.Initialise(config);
    ConfigurationDatabase configSignals;
    ok &= configSignals.CreateAbsolute("Signals.OutputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.Write("NumberOfElements", 3);
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.Write("NumberOfElements", 3);
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 4);

    ok &= configSignals.CreateAbsolute("Signals.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Type", "uint32");
    ok &= configSignals.Write("NumberOfElements", 1);
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.Write("DataSource", "Selector");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("Type", "uint32");
    ok &= configSignals.Write("NumberOfElements", 1);
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("2");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.Write("NumberOfElements", 3);
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("3");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.Write("NumberOfElements", 3);
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 4);
    ok &= configSignals.MoveToRoot();

    ok &= configSignals.CreateAbsolute("Memory.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("DataSource", "Selector");
    ok &= configSignals.CreateRelative("Signals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Samples", 2);

    ok &= configSignals.MoveToRoot();
    /*
     //example how to print a ConfigurationDatabase
     StreamString a;
     printf("%d\n", a.Printf("%!", configSignals));
     printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
     printf("%s\n", a.Buffer());
     printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
     */
    ok &= gam.SetConfiguredDatabase(configSignals);
    if (ok) {
        ok &= !gam.Setup();
    }
    return ok;
}

bool MuxGAMTest::TestSetupWrongInputSamples2() {
    ConfigurationDatabase config;
    MuxGAM gam;
    bool ok = gam.Initialise(config);
    ConfigurationDatabase configSignals;
    ok &= configSignals.CreateAbsolute("Signals.OutputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.Write("NumberOfElements", 3);
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.Write("NumberOfElements", 3);
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 4);

    ok &= configSignals.CreateAbsolute("Signals.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Type", "uint32");
    ok &= configSignals.Write("NumberOfElements", 1);
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.Write("DataSource", "Selector");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("Type", "uint32");
    ok &= configSignals.Write("NumberOfElements", 1);
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.Write("DataSource", "Selector");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("2");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.Write("NumberOfElements", 3);
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("3");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.Write("NumberOfElements", 3);
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 4);
    ok &= configSignals.MoveToRoot();

    ok &= configSignals.CreateAbsolute("Memory.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("DataSource", "Selector");
    ok &= configSignals.CreateRelative("Signals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Samples", 1);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("Samples", 3);

    ok &= configSignals.MoveToRoot();
    ok &= gam.SetConfiguredDatabase(configSignals);
    if (ok) {
        ok &= !gam.Setup();
    }
    return ok;
}

bool MuxGAMTest::TestSetupWrongInputSamples3() {
    ConfigurationDatabase config;
    MuxGAM gam;
    bool ok = gam.Initialise(config);
    ConfigurationDatabase configSignals;
    ok &= configSignals.CreateAbsolute("Signals.OutputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.Write("NumberOfElements", 3);
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.Write("ByteSize", 12);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.Write("NumberOfElements", 3);
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.Write("ByteSize", 12);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 24);

    ok &= configSignals.CreateAbsolute("Signals.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Type", "uint32");
    ok &= configSignals.Write("NumberOfElements", 1);
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.Write("DataSource", "Selector");
    ok &= configSignals.Write("ByteSize", 4);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("Type", "uint32");
    ok &= configSignals.Write("NumberOfElements", 1);
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.Write("DataSource", "Selector");
    ok &= configSignals.Write("ByteSize", 4);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("2");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.Write("NumberOfElements", 3);
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.Write("DataSource", "InputSignals1");
    ok &= configSignals.Write("ByteSize", 12);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("3");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.Write("NumberOfElements", 3);
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.Write("DataSource", "InputSignals1");
    ok &= configSignals.Write("ByteSize", 12);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 32);
    ok &= configSignals.MoveToRoot();

    ok &= configSignals.CreateAbsolute("Memory.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("DataSource", "Selector");
    ok &= configSignals.CreateRelative("Signals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Samples", 1);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("Samples", 1);

    ok &= configSignals.MoveToAncestor(3u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("DataSource", "InputSignals1");
    ok &= configSignals.CreateRelative("Signals");
    ok &= configSignals.CreateRelative("2");
    ok &= configSignals.Write("Samples", 5);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("3");
    ok &= configSignals.Write("Samples", 1);

    ok &= configSignals.MoveToRoot();

    ok &= gam.SetConfiguredDatabase(configSignals);
    if (ok) {
        ok &= !gam.Setup();
    }
    return ok;
}

bool MuxGAMTest::TestSetupWrongInputSamples4() {
    ConfigurationDatabase config;
    MuxGAM gam;
    bool ok = gam.Initialise(config);
    ConfigurationDatabase configSignals;
    ok &= configSignals.CreateAbsolute("Signals.OutputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.Write("NumberOfElements", 3);
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.Write("ByteSize", 12);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.Write("NumberOfElements", 3);
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.Write("ByteSize", 12);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 24);

    ok &= configSignals.CreateAbsolute("Signals.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Type", "uint32");
    ok &= configSignals.Write("NumberOfElements", 1);
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.Write("DataSource", "Selector");
    ok &= configSignals.Write("ByteSize", 4);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("Type", "uint32");
    ok &= configSignals.Write("NumberOfElements", 1);
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.Write("DataSource", "Selector");
    ok &= configSignals.Write("ByteSize", 4);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("2");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.Write("NumberOfElements", 3);
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.Write("DataSource", "InputSignals1");
    ok &= configSignals.Write("ByteSize", 12);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("3");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.Write("NumberOfElements", 3);
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.Write("DataSource", "InputSignals1");
    ok &= configSignals.Write("ByteSize", 12);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 32);
    ok &= configSignals.MoveToRoot();

    ok &= configSignals.CreateAbsolute("Memory.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("DataSource", "Selector");
    ok &= configSignals.CreateRelative("Signals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Samples", 1);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("Samples", 1);

    ok &= configSignals.MoveToAncestor(3u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("DataSource", "InputSignals1");
    ok &= configSignals.CreateRelative("Signals");
    ok &= configSignals.CreateRelative("2");
    ok &= configSignals.Write("Samples", 1);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("3");
    ok &= configSignals.Write("Samples", 134);

    ok &= configSignals.MoveToRoot();

    ok &= gam.SetConfiguredDatabase(configSignals);
    if (ok) {
        ok &= !gam.Setup();
    }
    return ok;
}

bool MuxGAMTest::TestSetupNoOutputSamples() {
    ConfigurationDatabase config;
    MuxGAM gam;
    bool ok = gam.Initialise(config);
    ConfigurationDatabase configSignals;
    ok &= configSignals.CreateAbsolute("Signals.OutputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.Write("NumberOfElements", 3);
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.Write("ByteSize", 12);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.Write("NumberOfElements", 3);
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.Write("ByteSize", 12);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 24);

    ok &= configSignals.CreateAbsolute("Signals.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Type", "uint32");
    ok &= configSignals.Write("NumberOfElements", 1);
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.Write("DataSource", "Selector");
    ok &= configSignals.Write("ByteSize", 4);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("Type", "uint32");
    ok &= configSignals.Write("NumberOfElements", 1);
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.Write("DataSource", "Selector");
    ok &= configSignals.Write("ByteSize", 4);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("2");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.Write("NumberOfElements", 3);
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.Write("DataSource", "InputSignals1");
    ok &= configSignals.Write("ByteSize", 12);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("3");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.Write("NumberOfElements", 3);
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.Write("DataSource", "InputSignals1");
    ok &= configSignals.Write("ByteSize", 12);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 32);
    ok &= configSignals.MoveToRoot();

    ok &= configSignals.CreateAbsolute("Memory.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("DataSource", "Selector");
    ok &= configSignals.CreateRelative("Signals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Samples", 1);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("Samples", 1);

    ok &= configSignals.MoveToAncestor(3u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("DataSource", "InputSignals1");
    ok &= configSignals.CreateRelative("Signals");
    ok &= configSignals.CreateRelative("2");
    ok &= configSignals.Write("Samples", 1);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("3");
    ok &= configSignals.Write("Samples", 1);

    ok &= configSignals.MoveToRoot();

    ok &= gam.SetConfiguredDatabase(configSignals);
    if (ok) {
        ok &= !gam.Setup();
    }
    return ok;
}

bool MuxGAMTest::TestSetupWrongOutputSamples() {
    ConfigurationDatabase config;
    MuxGAM gam;
    bool ok = gam.Initialise(config);
    ConfigurationDatabase configSignals;
    ok &= configSignals.CreateAbsolute("Signals.OutputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.Write("NumberOfElements", 3);
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.Write("DataSource", "OutputSignals1");
    ok &= configSignals.Write("ByteSize", 12);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.Write("NumberOfElements", 3);
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.Write("ByteSize", 12);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 24);

    ok &= configSignals.CreateAbsolute("Signals.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Type", "uint32");
    ok &= configSignals.Write("NumberOfElements", 1);
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.Write("DataSource", "Selector");
    ok &= configSignals.Write("ByteSize", 4);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("Type", "uint32");
    ok &= configSignals.Write("NumberOfElements", 1);
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.Write("DataSource", "Selector");
    ok &= configSignals.Write("ByteSize", 4);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("2");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.Write("NumberOfElements", 3);
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.Write("DataSource", "InputSignals1");
    ok &= configSignals.Write("ByteSize", 12);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("3");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.Write("NumberOfElements", 3);
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.Write("DataSource", "InputSignals1");
    ok &= configSignals.Write("ByteSize", 12);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 32);
    ok &= configSignals.MoveToRoot();

    ok &= configSignals.CreateAbsolute("Memory.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("DataSource", "Selector");
    ok &= configSignals.CreateRelative("Signals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Samples", 1);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("Samples", 1);
    ok &= configSignals.MoveToAncestor(3u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("DataSource", "InputSignals1");
    ok &= configSignals.CreateRelative("Signals");
    ok &= configSignals.CreateRelative("2");
    ok &= configSignals.Write("Samples", 1);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("3");
    ok &= configSignals.Write("Samples", 1);

    ok &= configSignals.CreateAbsolute("Memory.OutputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("DataSource", "OutputSignals1");
    ok &= configSignals.CreateRelative("Signals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Samples", 2);

    ok &= configSignals.MoveToRoot();

    ok &= gam.SetConfiguredDatabase(configSignals);
    if (ok) {
        ok &= !gam.Setup();
    }
    return ok;
}

bool MuxGAMTest::TestSetupWrongOutputSamples2() {
    ConfigurationDatabase config;
    MuxGAM gam;
    bool ok = gam.Initialise(config);
    ConfigurationDatabase configSignals;
    ok &= configSignals.CreateAbsolute("Signals.OutputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.Write("NumberOfElements", 3);
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.Write("DataSource", "OutputSignals1");
    ok &= configSignals.Write("ByteSize", 12);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.Write("NumberOfElements", 3);
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.Write("DataSource", "OutputSignals1");
    ok &= configSignals.Write("ByteSize", 12);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 24);

    ok &= configSignals.CreateAbsolute("Signals.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Type", "uint32");
    ok &= configSignals.Write("NumberOfElements", 1);
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.Write("DataSource", "Selector");
    ok &= configSignals.Write("ByteSize", 4);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("Type", "uint32");
    ok &= configSignals.Write("NumberOfElements", 1);
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.Write("DataSource", "Selector");
    ok &= configSignals.Write("ByteSize", 4);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("2");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.Write("NumberOfElements", 3);
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.Write("DataSource", "InputSignals1");
    ok &= configSignals.Write("ByteSize", 12);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("3");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.Write("NumberOfElements", 3);
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.Write("DataSource", "InputSignals1");
    ok &= configSignals.Write("ByteSize", 12);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 32);
    ok &= configSignals.MoveToRoot();

    ok &= configSignals.CreateAbsolute("Memory.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("DataSource", "Selector");
    ok &= configSignals.CreateRelative("Signals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Samples", 1);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("Samples", 1);
    ok &= configSignals.MoveToAncestor(3u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("DataSource", "InputSignals1");
    ok &= configSignals.CreateRelative("Signals");
    ok &= configSignals.CreateRelative("2");
    ok &= configSignals.Write("Samples", 1);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("3");
    ok &= configSignals.Write("Samples", 1);

    ok &= configSignals.CreateAbsolute("Memory.OutputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("DataSource", "OutputSignals1");
    ok &= configSignals.CreateRelative("Signals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Samples", 1);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("Samples", 14);

    ok &= configSignals.MoveToRoot();

    ok &= gam.SetConfiguredDatabase(configSignals);
    if (ok) {
        ok &= !gam.Setup();
    }
    return ok;
}

bool MuxGAMTest::TestSetup4I2O(StreamString strType,
                               uint32 nOfElements,
                               uint32 nOfSelectorElements) {
    ConfigurationDatabase config;
    MuxGAMTestHelper gam;
    bool ok = gam.Initialise(gam.config);
    ok &= gam.Setup4Inputs2Outputs(strType, nOfElements, nOfSelectorElements);
    ok &= gam.SetConfiguredDatabase(gam.configSignals);

    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();

    if (ok) {
        ok &= gam.Setup();
    }
    return ok;
}


}

