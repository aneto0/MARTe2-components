/**
 * @file WaveformPointsDefGAMTest.cpp
 * @brief Source file for class WaveformPointsDefGAMTest
 * @date 30/05/2017
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
 * the class WaveformPointsDefGAMTest (public, protected, and private). Be aware that some 
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

#include "WaveformPointsDefGAMTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

WaveformPointsDefGAMTest::WaveformPointsDefGAMTest() {

}

WaveformPointsDefGAMTest::~WaveformPointsDefGAMTest() {

}

bool WaveformPointsDefGAMTest::TestInitialise_MissingPoints() {
    bool ok = true;
    WaveformPointsDefGAMTestHelper gam;
    Vector<float64> yVec(gam.y1, gam.numberOfElementsY);
    //ok &= gam.config.Write("Points", yVec);
    Vector<float64> xVec(gam.x1, gam.numberOfElementsX);
    ok &= gam.config.Write("Times", xVec);
    ok &= gam.Initialise(gam.config);
    return !ok;
}

bool WaveformPointsDefGAMTest::TestInitialise_FailingReadingPointsValues() {
    bool ok = true;
    WaveformPointsDefGAMTestHelper gam;
    //Vector<float64> yVec(gam.y1, gam.numberOfElementsY);
    float64 y = 1.15;
    ok &= gam.config.Write("Points", y);
    Vector<float64> xVec(gam.x1, gam.numberOfElementsX);
    ok &= gam.config.Write("Times", xVec);
    /*
     StreamString a;
     printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
     printf("%d\n", a.Printf("%!", gam.config));
     printf("size of a %llu\n", a.Size());
     printf("%s\n", a.Buffer());
     printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
     */
    ok &= gam.Initialise(gam.config);
    return !ok;
}

bool WaveformPointsDefGAMTest::TestInitialise_1Point() {
    bool ok = true;
    WaveformPointsDefGAMTestHelper gam;
    float64 *x = new float64[1];
    *x = 0.0;
    float64 *y = new float64[1];
    *y = 32.9;
    Vector<float64> yVec(y, 1);
    ok &= gam.config.Write("Points", yVec);
    Vector<float64> xVec(x, 1);
    ok &= gam.config.Write("Times", xVec);
    ok &= gam.Initialise(gam.config);
    return !ok;
}

bool WaveformPointsDefGAMTest::TestInitialise_MissingTimes() {
    bool ok = true;
    WaveformPointsDefGAMTestHelper gam;
    Vector<float64> yVec(gam.y1, gam.numberOfElementsY);
    ok &= gam.config.Write("Points", yVec);
    Vector<float64> xVec(gam.x1, gam.numberOfElementsX);
    //ok &= gam.config.Write("Times", xVec);
    ok &= gam.Initialise(gam.config);
    return !ok;
}

bool WaveformPointsDefGAMTest::TestInitialise_DifferentSizePointsTimes() {
    bool ok = true;
    WaveformPointsDefGAMTestHelper gam;
    Vector<float64> yVec(gam.y1, gam.numberOfElementsY);
    ok &= gam.config.Write("Points", yVec);
    Vector<float64> xVec(gam.x1, gam.numberOfElementsX);
    float64 x = 1.15;
    ok &= gam.config.Write("Times", x);
    ok &= gam.Initialise(gam.config);
    return !ok;
}

bool WaveformPointsDefGAMTest::TestInitialise_FailingReadingTimesValues() {
    bool ok = true;
    WaveformPointsDefGAMTestHelper gam;
    float64 aux = 0.123;
    float64 *y = &aux;
    Vector<float64> yVec(y, 1);
    ok &= gam.config.Write("Points", yVec);
    Vector<float64> xVec(gam.x1, gam.numberOfElementsX);
    float64 x = 1.15;
    ok &= gam.config.Write("Times", x);
    ok &= gam.Initialise(gam.config);
    return !ok;
}

bool WaveformPointsDefGAMTest::TestInitialise_InvalidTimes() {
    bool ok = true;
    WaveformPointsDefGAMTestHelper gam;
    Vector<float64> yVec(gam.y1, gam.numberOfElementsY);
    ok &= gam.config.Write("Points", yVec);
    Vector<float64> xVec(gam.x1, gam.numberOfElementsX);
    ok &= gam.config.Write("Times", xVec);
    ok &= gam.Initialise(gam.config);
    return !ok;
}

bool WaveformPointsDefGAMTest::TestInitialise_InvalidTimes2() {
    bool ok = true;
    WaveformPointsDefGAMTestHelper gam;
    Vector<float64> yVec(gam.y1, gam.numberOfElementsY);
    ok &= gam.config.Write("Points", yVec);
    for (uint32 i = 0; i < gam.numberOfElementsX; i++) {
        gam.y1[i] = i;
    }
    Vector<float64> xVec(gam.x1, gam.numberOfElementsX);
    ok &= gam.config.Write("Times", xVec);
    ok &= gam.Initialise(gam.config);
    return !ok;
}

bool WaveformPointsDefGAMTest::TestInitialise_FailWaveformSetup() {
    bool ok = true;
    WaveformPointsDefGAMTestHelper gam;
    gam.SetName("Test");
    ok &= gam.InitialisePointsdef1();
    gam.config.MoveToRoot();
    ok &= gam.Initialise(gam.config);

    ok &= gam.SetConfiguredDatabase(gam.configSignals);
    ok &= gam.Setup();

    return !ok;
}

bool WaveformPointsDefGAMTest::TestExecuteSmallIncrementTimes() {
    bool ok = true;
    WaveformPointsDefGAMTestHelper gam(1, 1, 4, 1, "float64", 0, 0);
    uint32 sizeOutput = 4u;
    uint32 timeIterationIncrement = 4000000u;
    gam.SetName("Test");
    ok &= gam.InitialisePointsdefTimesExtreme();
    /*
    //example
    StreamString a;
    printf("%d\n", a.Printf("%!", gam.config));
    printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
    printf("%s\n", a.Buffer());
    printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
    */
    gam.config.MoveToRoot();
    ok &= gam.Initialise(gam.config);

    ok &= gam.InitialiseConfigDataBaseSignal1();
    ok &= gam.SetConfiguredDatabase(gam.configSignals);
    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();

    ok &= gam.Setup();

    uint32 *timeIteration = NULL;
    timeIteration = static_cast<uint32 *>(gam.GetInputSignalsMemory());
    *timeIteration = 0;

    float64 *output = NULL;
    output = static_cast<float64 *>(gam.GetOutputSignalsMemory());
    for (uint32 i = 0u; i < sizeOutput; i++) {
        output[i] = static_cast<float64>(0.0);
    }
    if (ok) {
        ok = gam.Execute();
    }
    if (ok) {
        ok &= gam.ComparePointsdef1(output, true);
    }
    if (ok) {
        *timeIteration += timeIterationIncrement;
        ok &= gam.Execute();
    }
    if (ok) {
        ok &= gam.ComparePointsdef1(output, false);
    }
    if (ok) {
        *timeIteration += timeIterationIncrement;
        ok &= gam.Execute();
    }
    if (ok) {
        ok &= gam.ComparePointsdef1(output, false);
    }
    if (ok) {
        *timeIteration += timeIterationIncrement;
        ok &= gam.Execute();
    }
    if (ok) {
        ok &= gam.ComparePointsdef1(output, false);
    }
    return ok;
}

