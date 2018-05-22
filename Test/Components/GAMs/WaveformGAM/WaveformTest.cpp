/**
 * @file WaveformTest.cpp
 * @brief Source file for class WaveformTest
 * @date 22/02/2018
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
 * the class WaveformTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "WaveformTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace {

}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

WaveformTest::WaveformTest() {
//Auto-generated constructor stub for WaveformTest

//TODO Verify if manual additions are needed here
}

WaveformTest::~WaveformTest() {
//Auto-generated destructor stub for WaveformTest

//TODO Verify if manual additions are needed here
}

bool WaveformTest::SetupNoName() {
    WaveformSin gam;
    return !gam.Setup();
}

bool WaveformTest::SetupNoInputSignals() {
    WaveformSin gam;
    ConfigurationDatabase configSignals;
    bool ok = configSignals.Write("QualifiedName", "WaveformSinGAMTest");
    if (ok) {
        gam.SetConfiguredDatabase(configSignals);
    }
    if (ok) {
        ok = !gam.Setup();
    }
    return ok;
}

bool WaveformTest::SetupNoOutputSignals() {
    WaveformSin gam;
    ConfigurationDatabase configSignals;
    bool ok = configSignals.Write("QualifiedName", "WaveformSinGAMTest");
    ok &= configSignals.CreateAbsolute("Signals.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 123);

    ok &= configSignals.MoveToRoot();
    if (ok) {
        gam.SetConfiguredDatabase(configSignals);
    }
    if (ok) {
        ok = !gam.Setup();
    }
    return ok;
}

bool WaveformTest::SetupNoInputElements() {
    WaveformSin gam;
    ConfigurationDatabase configSignals;
    bool ok = configSignals.Write("QualifiedName", "WaveformSinGAMTest");
    ok &= configSignals.CreateAbsolute("Signals.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 123);
    ok &= configSignals.CreateAbsolute("Signals.OutputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 123);
    ok &= configSignals.MoveToRoot();
    if (ok) {
        gam.SetConfiguredDatabase(configSignals);
    }
    if (ok) {
        ok = !gam.Setup();
    }
    return ok;
}

bool WaveformTest::SetupWrongInputElements() {
    WaveformSin gam;
    ConfigurationDatabase configSignals;
    bool ok = configSignals.Write("QualifiedName", "WaveformSinGAMTest");
    ok &= configSignals.CreateAbsolute("Signals.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("NumberOfElements", 123);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 123);
    ok &= configSignals.CreateAbsolute("Signals.OutputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 123);
    ok &= configSignals.MoveToRoot();
    if (ok) {
        gam.SetConfiguredDatabase(configSignals);
    }
    if (ok) {
        ok = !gam.Setup();
    }
    return ok;
}

bool WaveformTest::SetupNoOutputElements() {
    WaveformSin gam;
    ConfigurationDatabase configSignals;
    bool ok = configSignals.Write("QualifiedName", "WaveformSinGAMTest");
    ok &= configSignals.CreateAbsolute("Signals.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("NumberOfElements", 1);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 123);
    ok &= configSignals.CreateAbsolute("Signals.OutputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 123);
    ok &= configSignals.MoveToRoot();
    if (ok) {
        gam.SetConfiguredDatabase(configSignals);
    }
    if (ok) {
        ok = !gam.Setup();
    }
    return ok;
}

bool WaveformTest::SetupWrongOutputElements() {
    WaveformSin gam;
    ConfigurationDatabase configSignals;
    bool ok = configSignals.Write("QualifiedName", "WaveformSinGAMTest");
    ok &= configSignals.CreateAbsolute("Signals.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("NumberOfElements", 1);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 123);
    ok &= configSignals.CreateAbsolute("Signals.OutputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("NumberOfElements", 0);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 123);
    ok &= configSignals.MoveToRoot();
    if (ok) {
        gam.SetConfiguredDatabase(configSignals);
    }
    if (ok) {
        ok = !gam.Setup();
    }
    return ok;
}

bool WaveformTest::SetupWrongOutputElements_2() {
    WaveformSin gam;
    ConfigurationDatabase configSignals;
    bool ok = configSignals.Write("QualifiedName", "WaveformSinGAMTest");
    ok &= configSignals.CreateAbsolute("Signals.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("NumberOfElements", 1);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 123);
    ok &= configSignals.CreateAbsolute("Signals.OutputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("NumberOfElements", 135);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 123);
    ok &= configSignals.MoveToRoot();
    if (ok) {
        gam.SetConfiguredDatabase(configSignals);
    }
    if (ok) {
        ok = !gam.Setup();
    }
    return ok;
}

bool WaveformTest::SetupWrongOutputElements_3() {
    WaveformSin gam;
    ConfigurationDatabase configSignals;
    bool ok = configSignals.Write("QualifiedName", "WaveformSinGAMTest");
    ok &= configSignals.CreateAbsolute("Signals.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("NumberOfElements", 1);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 123);
    ok &= configSignals.CreateAbsolute("Signals.OutputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("NumberOfElements", 135);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("NumberOfElements", 134);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 123);
    ok &= configSignals.MoveToRoot();
    if (ok) {
        gam.SetConfiguredDatabase(configSignals);
    }
    if (ok) {
        ok = !gam.Setup();
    }
    return ok;
}

bool WaveformTest::SetupNoInputSamples() {
    WaveformSin gam;
    ConfigurationDatabase configSignals;
    bool ok = configSignals.Write("QualifiedName", "WaveformSinGAMTest");
    ok &= configSignals.CreateAbsolute("Signals.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("NumberOfElements", 1);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 123);

    ok &= configSignals.CreateAbsolute("Signals.OutputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("NumberOfElements", 135);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("NumberOfElements", 135);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 123);

    ok &= configSignals.MoveToRoot();
    if (ok) {
        gam.SetConfiguredDatabase(configSignals);
    }
    if (ok) {
        ok = !gam.Setup();
    }
    return ok;
}

bool WaveformTest::SetupWrongInputSamples() {
    WaveformSin gam;
    ConfigurationDatabase configSignals;
    bool ok = configSignals.Write("QualifiedName", "WaveformSinGAMTest");
    ok &= configSignals.CreateAbsolute("Signals.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("NumberOfElements", 1);
    ok &= configSignals.Write("DataSource", "InputDataSource");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 123);

    ok &= configSignals.CreateAbsolute("Signals.OutputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("NumberOfElements", 135);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("NumberOfElements", 135);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 123);

    ok &= configSignals.CreateAbsolute("Memory.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("DataSource", "InputDataSource");
    ok &= configSignals.CreateRelative("Signals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Samples", 2);
    ok &= configSignals.MoveToRoot();
    if (ok) {
        gam.SetConfiguredDatabase(configSignals);
    }
    if (ok) {
        ok = !gam.Setup();
    }
    return ok;
}

bool WaveformTest::SetupNoOutputSamples() {
    WaveformSin gam;
    ConfigurationDatabase configSignals;
    bool ok = configSignals.Write("QualifiedName", "WaveformSinGAMTest");
    ok &= configSignals.CreateAbsolute("Signals.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("NumberOfElements", 1);
    ok &= configSignals.Write("DataSource", "InputDataSource");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 123);

    ok &= configSignals.CreateAbsolute("Signals.OutputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("NumberOfElements", 135);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("NumberOfElements", 135);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 123);

    ok &= configSignals.CreateAbsolute("Memory.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("DataSource", "InputDataSource");
    ok &= configSignals.CreateRelative("Signals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Samples", 1);

    ok &= configSignals.MoveToRoot();
    if (ok) {
        gam.SetConfiguredDatabase(configSignals);
    }
    if (ok) {
        ok = !gam.Setup();
    }
    return ok;
}

bool WaveformTest::SetupWrongOutputSamples() {
    WaveformSin gam;
    ConfigurationDatabase configSignals;
    bool ok = configSignals.Write("QualifiedName", "WaveformSinGAMTest");
    ok &= configSignals.CreateAbsolute("Signals.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("NumberOfElements", 1);
    ok &= configSignals.Write("DataSource", "InputDataSource");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 123);

    ok &= configSignals.CreateAbsolute("Signals.OutputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("NumberOfElements", 135);
    ok &= configSignals.Write("DataSource", "OutputDataSource");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("NumberOfElements", 135);
    ok &= configSignals.Write("DataSource", "OutputDataSource");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 123);

    ok &= configSignals.CreateAbsolute("Memory.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("DataSource", "InputDataSource");
    ok &= configSignals.CreateRelative("Signals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Samples", 1);

    ok &= configSignals.CreateAbsolute("Memory.OutputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("DataSource", "OutputDataSource");
    ok &= configSignals.CreateRelative("Signals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Samples", 1);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("Samples", 2);

    ok &= configSignals.MoveToRoot();
    if (ok) {
        gam.SetConfiguredDatabase(configSignals);
    }
    if (ok) {
        ok = !gam.Setup();
    }
    return ok;
}

bool WaveformTest::SetupInvalitInputType() {
    WaveformSin gam;
    ConfigurationDatabase configSignals;
    bool ok = configSignals.Write("QualifiedName", "WaveformSinGAMTest");
    ok &= configSignals.CreateAbsolute("Signals.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("NumberOfElements", 1);
    ok &= configSignals.Write("DataSource", "InputDataSource");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 123);

    ok &= configSignals.CreateAbsolute("Signals.OutputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("NumberOfElements", 135);
    ok &= configSignals.Write("DataSource", "OutputDataSource");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("NumberOfElements", 135);
    ok &= configSignals.Write("DataSource", "OutputDataSource");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 123);

    ok &= configSignals.CreateAbsolute("Memory.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("DataSource", "InputDataSource");
    ok &= configSignals.CreateRelative("Signals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Samples", 1);

    ok &= configSignals.CreateAbsolute("Memory.OutputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("DataSource", "OutputDataSource");
    ok &= configSignals.CreateRelative("Signals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Samples", 1);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("Samples", 1);

    ok &= configSignals.MoveToRoot();
    if (ok) {
        gam.SetConfiguredDatabase(configSignals);
    }
    if (ok) {
        ok = !gam.Setup();
    }
    return ok;
}

bool WaveformTest::SetupInvalitOutputType() {
    WaveformSin gam;
    ConfigurationDatabase configSignals;
    bool ok = configSignals.Write("QualifiedName", "WaveformSinGAMTest");
    ok &= configSignals.CreateAbsolute("Signals.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("NumberOfElements", 1);
    ok &= configSignals.Write("DataSource", "InputDataSource");
    ok &= configSignals.Write("Type", "uint32");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 123);

    ok &= configSignals.CreateAbsolute("Signals.OutputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("NumberOfElements", 135);
    ok &= configSignals.Write("DataSource", "OutputDataSource");
    ok &= configSignals.Write("Type", "uint32");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("NumberOfElements", 135);
    ok &= configSignals.Write("DataSource", "OutputDataSource");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 123);

    ok &= configSignals.CreateAbsolute("Memory.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("DataSource", "InputDataSource");
    ok &= configSignals.CreateRelative("Signals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Samples", 1);

    ok &= configSignals.CreateAbsolute("Memory.OutputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("DataSource", "OutputDataSource");
    ok &= configSignals.CreateRelative("Signals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Samples", 1);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("Samples", 1);

    ok &= configSignals.MoveToRoot();
    if (ok) {
        gam.SetConfiguredDatabase(configSignals);
    }
    if (ok) {
        ok = !gam.Setup();
    }
    return ok;
}

bool WaveformTest::SetupNoInputDimension() {
    WaveformSin gam;
    ConfigurationDatabase configSignals;
    bool ok = configSignals.Write("QualifiedName", "WaveformSinGAMTest");
    ok &= configSignals.CreateAbsolute("Signals.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("NumberOfElements", 1);
    ok &= configSignals.Write("DataSource", "InputDataSource");
    ok &= configSignals.Write("Type", "uint32");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 123);

    ok &= configSignals.CreateAbsolute("Signals.OutputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("NumberOfElements", 135);
    ok &= configSignals.Write("DataSource", "OutputDataSource");
    ok &= configSignals.Write("Type", "uint32");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("NumberOfElements", 135);
    ok &= configSignals.Write("DataSource", "OutputDataSource");
    ok &= configSignals.Write("Type", "uint64");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 123);

    ok &= configSignals.CreateAbsolute("Memory.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("DataSource", "InputDataSource");
    ok &= configSignals.CreateRelative("Signals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Samples", 1);

    ok &= configSignals.CreateAbsolute("Memory.OutputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("DataSource", "OutputDataSource");
    ok &= configSignals.CreateRelative("Signals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Samples", 1);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("Samples", 1);

    ok &= configSignals.MoveToRoot();
    if (ok) {
        gam.SetConfiguredDatabase(configSignals);
    }
    if (ok) {
        ok = !gam.Setup();
    }
    return ok;
}

bool WaveformTest::SetupWrongInputDimension() {
    WaveformSin gam;
    ConfigurationDatabase configSignals;
    bool ok = configSignals.Write("QualifiedName", "WaveformSinGAMTest");
    ok &= configSignals.CreateAbsolute("Signals.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("NumberOfElements", 1);
    ok &= configSignals.Write("DataSource", "InputDataSource");
    ok &= configSignals.Write("Type", "uint32");
    ok &= configSignals.Write("NumberOfDimensions", 2);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 123);

    ok &= configSignals.CreateAbsolute("Signals.OutputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("NumberOfElements", 135);
    ok &= configSignals.Write("DataSource", "OutputDataSource");
    ok &= configSignals.Write("Type", "uint32");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("NumberOfElements", 135);
    ok &= configSignals.Write("DataSource", "OutputDataSource");
    ok &= configSignals.Write("Type", "uint64");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 123);

    ok &= configSignals.CreateAbsolute("Memory.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("DataSource", "InputDataSource");
    ok &= configSignals.CreateRelative("Signals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Samples", 1);

    ok &= configSignals.CreateAbsolute("Memory.OutputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("DataSource", "OutputDataSource");
    ok &= configSignals.CreateRelative("Signals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Samples", 1);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("Samples", 1);

    ok &= configSignals.MoveToRoot();
    if (ok) {
        gam.SetConfiguredDatabase(configSignals);
    }
    if (ok) {
        ok = !gam.Setup();
    }
    return ok;
}

bool WaveformTest::SetupNoOutputDimension() {
    WaveformSin gam;
    ConfigurationDatabase configSignals;
    bool ok = configSignals.Write("QualifiedName", "WaveformSinGAMTest");
    ok &= configSignals.CreateAbsolute("Signals.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("NumberOfElements", 1);
    ok &= configSignals.Write("DataSource", "InputDataSource");
    ok &= configSignals.Write("Type", "uint32");
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 123);

    ok &= configSignals.CreateAbsolute("Signals.OutputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("NumberOfElements", 135);
    ok &= configSignals.Write("DataSource", "OutputDataSource");
    ok &= configSignals.Write("Type", "uint32");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("NumberOfElements", 135);
    ok &= configSignals.Write("DataSource", "OutputDataSource");
    ok &= configSignals.Write("Type", "uint64");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 123);

    ok &= configSignals.CreateAbsolute("Memory.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("DataSource", "InputDataSource");
    ok &= configSignals.CreateRelative("Signals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Samples", 1);

    ok &= configSignals.CreateAbsolute("Memory.OutputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("DataSource", "OutputDataSource");
    ok &= configSignals.CreateRelative("Signals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Samples", 1);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("Samples", 1);

    ok &= configSignals.MoveToRoot();
    if (ok) {
        gam.SetConfiguredDatabase(configSignals);
    }
    if (ok) {
        ok = !gam.Setup();
    }
    return ok;
}


bool WaveformTest::SetupWrongOutputDimension() {
    WaveformSin gam;
    ConfigurationDatabase configSignals;
    bool ok = configSignals.Write("QualifiedName", "WaveformSinGAMTest");
    ok &= configSignals.CreateAbsolute("Signals.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("NumberOfElements", 1);
    ok &= configSignals.Write("DataSource", "InputDataSource");
    ok &= configSignals.Write("Type", "uint32");
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 123);

    ok &= configSignals.CreateAbsolute("Signals.OutputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("NumberOfElements", 135);
    ok &= configSignals.Write("DataSource", "OutputDataSource");
    ok &= configSignals.Write("Type", "uint32");
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("NumberOfElements", 135);
    ok &= configSignals.Write("DataSource", "OutputDataSource");
    ok &= configSignals.Write("Type", "uint64");
    ok &= configSignals.Write("NumberOfDimensions", 2);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 123);

    ok &= configSignals.CreateAbsolute("Memory.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("DataSource", "InputDataSource");
    ok &= configSignals.CreateRelative("Signals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Samples", 1);

    ok &= configSignals.CreateAbsolute("Memory.OutputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("DataSource", "OutputDataSource");
    ok &= configSignals.CreateRelative("Signals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Samples", 1);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("Samples", 1);

    ok &= configSignals.MoveToRoot();
    if (ok) {
        gam.SetConfiguredDatabase(configSignals);
    }
    if (ok) {
        ok = !gam.Setup();
    }
    return ok;
}

bool WaveformTest::SetupWrongOutputDimension_2() {
    WaveformSin gam;
    ConfigurationDatabase configSignals;
    bool ok = configSignals.Write("QualifiedName", "WaveformSinGAMTest");
    ok &= configSignals.CreateAbsolute("Signals.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("NumberOfElements", 1);
    ok &= configSignals.Write("DataSource", "InputDataSource");
    ok &= configSignals.Write("Type", "uint32");
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 123);

    ok &= configSignals.CreateAbsolute("Signals.OutputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("NumberOfElements", 2);
    ok &= configSignals.Write("DataSource", "OutputDataSource");
    ok &= configSignals.Write("Type", "uint32");
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("NumberOfElements", 2);
    ok &= configSignals.Write("DataSource", "OutputDataSource");
    ok &= configSignals.Write("Type", "uint64");
    ok &= configSignals.Write("NumberOfDimensions", 0);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 123);

    ok &= configSignals.CreateAbsolute("Memory.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("DataSource", "InputDataSource");
    ok &= configSignals.CreateRelative("Signals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Samples", 1);

    ok &= configSignals.CreateAbsolute("Memory.OutputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("DataSource", "OutputDataSource");
    ok &= configSignals.CreateRelative("Signals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Samples", 1);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("Samples", 1);

    ok &= configSignals.MoveToRoot();
    if (ok) {
        gam.SetConfiguredDatabase(configSignals);
    }
    if (ok) {
        ok = !gam.Setup();
    }
    return ok;
}

bool WaveformTest::SetupNULLInputPointer() {
    WaveformSin gam;
    ConfigurationDatabase configSignals;
    bool ok = configSignals.Write("QualifiedName", "WaveformSinGAMTest");
    ok &= configSignals.CreateAbsolute("Signals.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("NumberOfElements", 1);
    ok &= configSignals.Write("DataSource", "InputDataSource");
    ok &= configSignals.Write("Type", "uint32");
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 123);

    ok &= configSignals.CreateAbsolute("Signals.OutputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("NumberOfElements", 135);
    ok &= configSignals.Write("DataSource", "OutputDataSource");
    ok &= configSignals.Write("Type", "uint32");
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("NumberOfElements", 135);
    ok &= configSignals.Write("DataSource", "OutputDataSource");
    ok &= configSignals.Write("Type", "int64");
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 123);

    ok &= configSignals.CreateAbsolute("Memory.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("DataSource", "InputDataSource");
    ok &= configSignals.CreateRelative("Signals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Samples", 1);

    ok &= configSignals.CreateAbsolute("Memory.OutputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("DataSource", "OutputDataSource");
    ok &= configSignals.CreateRelative("Signals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Samples", 1);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("Samples", 1);

    ok &= configSignals.MoveToRoot();
    if (ok) {
        gam.SetConfiguredDatabase(configSignals);
    }
    if (ok) {
        ok = !gam.Setup();
    }
    return ok;
}

bool WaveformTest::SetupNULLOutputPointer() {
    WaveformSin gam;
    ConfigurationDatabase configSignals;
    bool ok = configSignals.Write("QualifiedName", "WaveformSinGAMTest");
    ok &= configSignals.CreateAbsolute("Signals.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("NumberOfElements", 1);
    ok &= configSignals.Write("DataSource", "InputDataSource");
    ok &= configSignals.Write("Type", "uint32");
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.Write("ByteSize", 4);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 4);

    ok &= configSignals.CreateAbsolute("Signals.OutputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("NumberOfElements", 135);
    ok &= configSignals.Write("DataSource", "OutputDataSource");
    ok &= configSignals.Write("Type", "uint32");
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.Write("ByteSize", 540);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("NumberOfElements", 135);
    ok &= configSignals.Write("DataSource", "OutputDataSource");
    ok &= configSignals.Write("Type", "int64");
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.Write("ByteSize", 1080);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 1620);

    ok &= configSignals.CreateAbsolute("Memory.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("DataSource", "InputDataSource");
    ok &= configSignals.CreateRelative("Signals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Samples", 1);

    ok &= configSignals.CreateAbsolute("Memory.OutputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("DataSource", "OutputDataSource");
    ok &= configSignals.CreateRelative("Signals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Samples", 1);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("Samples", 1);

    ok &= configSignals.MoveToRoot();

    if (ok) {
        ok = gam.SetConfiguredDatabase(configSignals);
    }
    if(ok){
        ok = gam.AllocateInputSignalsMemory();
    }
    if (ok) {
        ok = !gam.Setup();
    }
    return ok;
}

bool WaveformTest::Setup() {
    WaveformSin gam;
    ConfigurationDatabase configSignals;
    bool ok = configSignals.Write("QualifiedName", "WaveformSinGAMTest");
    ok &= configSignals.CreateAbsolute("Signals.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("NumberOfElements", 1);
    ok &= configSignals.Write("DataSource", "InputDataSource");
    ok &= configSignals.Write("Type", "uint32");
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.Write("ByteSize", 4);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 4);

    ok &= configSignals.CreateAbsolute("Signals.OutputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("NumberOfElements", 135);
    ok &= configSignals.Write("DataSource", "OutputDataSource");
    ok &= configSignals.Write("Type", "uint32");
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.Write("ByteSize", 540);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("NumberOfElements", 135);
    ok &= configSignals.Write("DataSource", "OutputDataSource");
    ok &= configSignals.Write("Type", "int64");
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.Write("ByteSize", 1080);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 1620);

    ok &= configSignals.CreateAbsolute("Memory.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("DataSource", "InputDataSource");
    ok &= configSignals.CreateRelative("Signals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Samples", 1);

    ok &= configSignals.CreateAbsolute("Memory.OutputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("DataSource", "OutputDataSource");
    ok &= configSignals.CreateRelative("Signals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Samples", 1);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("Samples", 1);

    ok &= configSignals.MoveToRoot();

    if (ok) {
        ok = gam.SetConfiguredDatabase(configSignals);
    }
    if(ok){
        ok = gam.AllocateInputSignalsMemory();
    }
    if(ok){
        ok = gam.AllocateOutputSignalsMemory();
    }
    if (ok) {
        ok = gam.Setup();
    }
    return ok;
}

}

