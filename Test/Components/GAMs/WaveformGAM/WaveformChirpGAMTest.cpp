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

namespace {

}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

WaveformChirpGAMTest::WaveformChirpGAMTest() {
//Auto-generated constructor stub for WaveformChirpGAMTest

//TODO Verify if manual additions are needed here
}

WaveformChirpGAMTest::~WaveformChirpGAMTest() {
//Auto-generated destructor stub for WaveformChirpGAMTest

//TODO Verify if manual additions are needed here
}

bool WaveformChirpGAMTest::TestMissingAmplitude(){
    bool ok = true;
    uint32 sizeOutput = 4u;
    WaveformChirpGAMTestHelper gam(1, 1, sizeOutput, 1);
    gam.SetName("Test");
    ok &= gam.Initialise(gam.config);
    return !ok;
}

bool WaveformChirpGAMTest::Test0Amplitude(){
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

bool WaveformChirpGAMTest::TestMissingPhase(){
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

bool WaveformChirpGAMTest::TestMissingOffset(){
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

bool WaveformChirpGAMTest::TestMissingFreq1(){
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

bool WaveformChirpGAMTest::TestMissingFreq2(){
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

bool WaveformChirpGAMTest::TestMissingChirpDuration(){
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


bool WaveformChirpGAMTest::Test0ChirpDuration(){
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


}

