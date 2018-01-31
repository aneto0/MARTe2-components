/**
 * @file WaveformChirpGAMTest.cpp
 * @brief Source file for class WaveformChirpGAMTest
 * @date 02/06/2017
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
 * the class WaveformChirpGAMTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "WaveformChirpGAMTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

WaveformChirpGAMTest::WaveformChirpGAMTest() {
}

WaveformChirpGAMTest::~WaveformChirpGAMTest() {
}

bool WaveformChirpGAMTest::TestInitialiseMissingAmplitude() {
    bool ok = true;
    uint32 sizeOutput = 4u;
    WaveformChirpGAMTestHelper gam(1, 1, sizeOutput, 1);
    gam.SetName("Test");
    ok &= gam.Initialise(gam.config);
    return !ok;
}

bool WaveformChirpGAMTest::TestInitialise0Amplitude() {
    bool ok = true;
    uint32 sizeOutput = 4u;
    WaveformChirpGAMTestHelper gam(1, 1, sizeOutput, 1);
    gam.SetName("Test");
    float64 f1 = 0;
    float64 f2 = 10;
    //amplitude = 10.0;
    float64 amplitude = 0.0;
    float64 phase = 0.0;
    float64 offset = 0.0;
    float64 chirpDuration = 5.0;
    //chirp configuration
    ok &= gam.config.Write("Frequency1", f1);
    ok &= gam.config.Write("Frequency2", f2);
    ok &= gam.config.Write("Amplitude", amplitude);
    ok &= gam.config.Write("Phase", phase);
    ok &= gam.config.Write("Offset", offset);
    ok &= gam.config.Write("ChirpDuration", chirpDuration);
    ok &= gam.Initialise(gam.config);

    return !ok;
}

bool WaveformChirpGAMTest::TestInitialiseMissingPhase() {
    bool ok = true;
    uint32 sizeOutput = 4u;
    WaveformChirpGAMTestHelper gam(1, 1, sizeOutput, 1);
    gam.SetName("Test");
    float64 f1 = 0;
    float64 f2 = 10;
    float64 amplitude = 10.0;
    //float64 phase = 0.0;
    float64 offset = 0.0;
    float64 chirpDuration = 5.0;
    //chirp configuration
    ok &= gam.config.Write("Frequency1", f1);
    ok &= gam.config.Write("Frequency2", f2);
    ok &= gam.config.Write("Amplitude", amplitude);
    //ok &= gam.config.Write("Phase", phase);
    ok &= gam.config.Write("Offset", offset);
    ok &= gam.config.Write("ChirpDuration", chirpDuration);
    ok &= gam.Initialise(gam.config);

    return !ok;
}

bool WaveformChirpGAMTest::TestInitialiseMissingOffset() {
    bool ok = true;
    uint32 sizeOutput = 4u;
    WaveformChirpGAMTestHelper gam(1, 1, sizeOutput, 1);
    gam.SetName("Test");
    float64 f1 = 0;
    float64 f2 = 10;
    float64 amplitude = 10.0;
    float64 phase = 0.0;
    //float64 offset = 0.0;
    float64 chirpDuration = 5.0;
    //chirp configuration
    ok &= gam.config.Write("Frequency1", f1);
    ok &= gam.config.Write("Frequency2", f2);
    ok &= gam.config.Write("Amplitude", amplitude);
    ok &= gam.config.Write("Phase", phase);
    //ok &= gam.config.Write("Offset", offset);
    ok &= gam.config.Write("ChirpDuration", chirpDuration);
    ok &= gam.Initialise(gam.config);

    return !ok;
}

bool WaveformChirpGAMTest::TestInitialiseMissingFreq1() {
    bool ok = true;
    uint32 sizeOutput = 4u;
    WaveformChirpGAMTestHelper gam(1, 1, sizeOutput, 1);
    gam.SetName("Test");
    //float64 f1 = 0;
    float64 f2 = 10;
    float64 amplitude = 10.0;
    float64 phase = 0.0;
    float64 offset = 0.0;
    float64 chirpDuration = 5.0;
    //chirp configuration
    //ok &= gam.config.Write("Frequency1", f1);
    ok &= gam.config.Write("Frequency2", f2);
    ok &= gam.config.Write("Amplitude", amplitude);
    ok &= gam.config.Write("Phase", phase);
    ok &= gam.config.Write("Offset", offset);
    ok &= gam.config.Write("ChirpDuration", chirpDuration);
    ok &= gam.Initialise(gam.config);

    return !ok;
}

bool WaveformChirpGAMTest::TestInitialiseMissingFreq2() {
    bool ok = true;
    uint32 sizeOutput = 4u;
    WaveformChirpGAMTestHelper gam(1, 1, sizeOutput, 1);
    gam.SetName("Test");
    float64 f1 = 0;
    //float64 f2 = 10;
    float64 amplitude = 10.0;
    float64 phase = 0.0;
    float64 offset = 0.0;
    float64 chirpDuration = 5.0;
    //chirp configuration
    ok &= gam.config.Write("Frequency1", f1);
    //ok &= gam.config.Write("Frequency2", f2);
    ok &= gam.config.Write("Amplitude", amplitude);
    ok &= gam.config.Write("Phase", phase);
    ok &= gam.config.Write("Offset", offset);
    ok &= gam.config.Write("ChirpDuration", chirpDuration);
    ok &= gam.Initialise(gam.config);

    return !ok;
}

bool WaveformChirpGAMTest::TestInitialiseMissingChirpDuration() {
    bool ok = true;
    uint32 sizeOutput = 4u;
    WaveformChirpGAMTestHelper gam(1, 1, sizeOutput, 1);
    gam.SetName("Test");
    float64 f1 = 0;
    float64 f2 = 10;
    float64 amplitude = 10.0;
    float64 phase = 0.0;
    float64 offset = 0.0;
    //float64 chirpDuration = 5.0;
    //chirp configuration
    ok &= gam.config.Write("Frequency1", f1);
    ok &= gam.config.Write("Frequency2", f2);
    ok &= gam.config.Write("Amplitude", amplitude);
    ok &= gam.config.Write("Phase", phase);
    ok &= gam.config.Write("Offset", offset);
    //ok &= gam.config.Write("ChirpDuration", chirpDuration);
    ok &= gam.Initialise(gam.config);

    return !ok;
}

bool WaveformChirpGAMTest::TestInitialise0ChirpDuration() {
    bool ok = true;
    uint32 sizeOutput = 4u;
    WaveformChirpGAMTestHelper gam(1, 1, sizeOutput, 1);
    gam.SetName("Test");
    float64 f1 = 0;
    float64 f2 = 10;
    float64 amplitude = 10.0;
    float64 phase = 0.0;
    float64 offset = 0.0;
    float64 chirpDuration = 0.0;
    //chirp configuration
    ok &= gam.config.Write("Frequency1", f1);
    ok &= gam.config.Write("Frequency2", f2);
    ok &= gam.config.Write("Amplitude", amplitude);
    ok &= gam.config.Write("Phase", phase);
    ok &= gam.config.Write("Offset", offset);
    ok &= gam.config.Write("ChirpDuration", chirpDuration);
    ok &= gam.Initialise(gam.config);

    return !ok;
}

bool WaveformChirpGAMTest::TestExecuteNyquistViolation() {
    using namespace MARTe;
    bool ok = true;
    uint32 timeIterationIncrement = 100000u;
    uint32 *timeIteration = NULL;
    uint32 sizeOutput = 4u;
    WaveformChirpGAMTestHelper gam(1, 1, sizeOutput, 1, "int8");

    int8 *output = NULL;

    gam.SetName("Test");
    float64 f1 = 100000;
    float64 f2 = 10;
    float64 amplitude = 10.0;
    float64 phase = 0.0;
    float64 offset = 0.0;
    float64 chirpDuration = 10.0;
    //chirp configuration
    ok &= gam.config.Write("Frequency1", f1);
    ok &= gam.config.Write("Frequency2", f2);
    ok &= gam.config.Write("Amplitude", amplitude);
    ok &= gam.config.Write("Phase", phase);
    ok &= gam.config.Write("Offset", offset);
    ok &= gam.config.Write("ChirpDuration", chirpDuration);
    ok &= gam.Initialise(gam.config);

    if (ok) {
        ok &= gam.InitialiseConfigDataBaseSignal1();
    }
    if (ok) {
        ok &= gam.SetConfiguredDatabase(gam.configSignals);
    }
    if (ok) {
        ok &= gam.AllocateInputSignalsMemory();
    }
    if (ok) {
        ok &= gam.AllocateOutputSignalsMemory();
    }
    if (ok) {
        ok &= gam.Setup();
    }

    if (ok) {
        timeIteration = static_cast<uint32 *>(gam.GetInputSignalsMemory());

        *timeIteration = 0;
    }
    if (ok) {
        output = static_cast<int8 *>(gam.GetOutputSignalsMemory());
    }
    for (uint32 i = 0u; (i < sizeOutput) && ok; i++) {
        output[i] = static_cast<int8>(0.0);
    }
    if (ok) {
        ok = gam.Execute();
    }
    timeIteration[0] += timeIterationIncrement;
    if (ok) {
        ok = !gam.Execute();
    }
    return ok;
}

}

