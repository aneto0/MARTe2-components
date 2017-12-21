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

bool WaveformPointsDefGAMTest::TestExecuteSawtooth() {
    bool ok = true;
    WaveformPointsDefGAMTestHelper gam(1, 1, 1, 1, "float64", 0, 0, 2, 2);
    uint32 sizeOutput = 1u;
    uint32 timeIterationIncrement = 100000u;
    uint32 MAX_REP = 100u;
    gam.SetName("Test");
    ok &= gam.InitialisePointsdefSawtooth();

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
    float64 ref123 = 0.0;
    float64 *output = NULL;
    output = static_cast<float64 *>(gam.GetOutputSignalsMemory());
    for (uint32 i = 0u; i < sizeOutput; i++) {
        output[i] = static_cast<float64>(0.0);
    }
    if (ok) {
        ok = gam.Execute();
    }
    if (ok) {
        ok &= gam.ComparePointsdef(output, true, ref123);
    }
    uint32 error = 0;
    for (uint32 i = 1u; i < MAX_REP && (error < 5); i++) {
        if (ok) {
            *timeIteration += timeIterationIncrement;
            ok = gam.Execute();
        }
        if (!ok) {
            printf("Error. gam.Execute() returns an error in iteration %u\n", i);
            error++;
        }
        if (ok) {
            ok = gam.ComparePointsdef(output, false, ref123);
        }
        if (!ok) {
            printf("Error. *output - refVal = %.16lf, iteration %u\n", ref123, i);
            error++;
        }
    }
    return ok;
}

bool WaveformPointsDefGAMTest::TestExecuteSawtooth_4elements() {
    bool ok = true;
    WaveformPointsDefGAMTestHelper gam(1, 1, 4, 1, "float64", 0, 0, 2, 2);
    uint32 sizeOutput = 4u;
    uint32 timeIterationIncrement = 400000u;
    uint32 MAX_REP = 10u;
    gam.SetName("Test");
    ok &= gam.InitialisePointsdefSawtooth();

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
    float64 ref123 = 0.0;
    float64 *output = NULL;
    output = static_cast<float64 *>(gam.GetOutputSignalsMemory());
    for (uint32 i = 0u; i < sizeOutput; i++) {
        output[i] = static_cast<float64>(0.0);
    }
    if (ok) {
        ok = gam.Execute();
    }
    if (ok) {
        ok &= gam.ComparePointsdef(output, true, ref123);
    }
    uint32 error = 0;
    for (uint32 i = 1u; i < MAX_REP && (error < 5); i++) {
        if (ok) {
            *timeIteration += timeIterationIncrement;
            ok = gam.Execute();
        }
        if (!ok) {
            printf("Error. gam.Execute() returns an error in iteration %u\n", i);
            error++;
        }
        if (ok) {
            ok = gam.ComparePointsdef(output, false, ref123);

        }
        if (!ok) {
            printf("Error. refVal - output = %.16lf, iteration %u\n", ref123, i);
            error++;
        }
    }
    return ok;
}

bool WaveformPointsDefGAMTest::TestExecuteLargeElements() {
    bool ok = true;
    WaveformPointsDefGAMTestHelper gam(1, 1, 2000000, 1, "int32", 0, 0, 2, 2);
    uint32 sizeOutput = 2000000u;
    uint32 timeIterationIncrement = 1000000u;
    uint32 MAX_REP = 5u;
    gam.SetName("Test");
    ok &= gam.InitialisePointsdefConstValue();

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
    int32 *output = NULL;
    output = static_cast<int32 *>(gam.GetOutputSignalsMemory());
    for (uint32 i = 0u; i < sizeOutput; i++) {
        output[i] = 0;
    }
    if (ok) {
        ok = gam.Execute();
    }
    if (ok) {
        ok &= gam.ComparePointsdef1(output, true);
    }
    uint32 error = 0;
    for (uint32 i = 1u; (i < MAX_REP) && ok; i++) {

        *timeIteration += timeIterationIncrement;
        ok = gam.Execute();

        if (!ok) {
            printf("Error. gam.Execute() returns an error in iteration %u\n", i);
            error++;
        }
        if (ok) {
            ok = gam.ComparePointsdef1(output, false);

        }
        if (!ok) {
            printf("Error. output = %d, iteration %u\n", output[i], i);
            error++;
        }
    }
    return ok;
}

