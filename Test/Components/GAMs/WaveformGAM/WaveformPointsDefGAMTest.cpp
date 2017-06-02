/**
 * @file WaveformPointsDefGAMTest.cpp
 * @brief Source file for class WaveformPointsDefGAMTest
 * @date 30/05/2017
 * @author Llorenc
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

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "../WaveformGAM/WaveformPointsDefGAMTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

WaveformPointsDefGAMTest::WaveformPointsDefGAMTest() {
//Auto-generated constructor stub for WaveformPointsDefGAMTest

//TODO Verify if manual additions are needed here
}

WaveformPointsDefGAMTest::~WaveformPointsDefGAMTest() {

}

bool WaveformPointsDefGAMTest::TestMissingPoints() {
    bool ok = true;
    WaveformPointsDefGAMTestHelper gam;
    Vector<float64> yVec(gam.y1, gam.numberOfElementsY);
    //ok &= gam.config.Write("Points", yVec);
    Vector<float64> xVec(gam.x1, gam.numberOfElementsX);
    ok &= gam.config.Write("Times", xVec);
    ok &= gam.Initialise(gam.config);
    return !ok;
}

bool WaveformPointsDefGAMTest::TestFailingReadingPointsValues() {
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

bool WaveformPointsDefGAMTest::TestMissingTimes() {
    bool ok = true;
    WaveformPointsDefGAMTestHelper gam;
    Vector<float64> yVec(gam.y1, gam.numberOfElementsY);
    ok &= gam.config.Write("Points", yVec);
    Vector<float64> xVec(gam.x1, gam.numberOfElementsX);
    //ok &= gam.config.Write("Times", xVec);
    ok &= gam.Initialise(gam.config);
    return !ok;
}

bool WaveformPointsDefGAMTest::TestDifferentSizePointsTimes() {
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

bool WaveformPointsDefGAMTest::TestFailingReadingTimesValues() {
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

bool WaveformPointsDefGAMTest::TestInvalidTimes() {
    bool ok = true;
    WaveformPointsDefGAMTestHelper gam;
    Vector<float64> yVec(gam.y1, gam.numberOfElementsY);
    ok &= gam.config.Write("Points", yVec);
    Vector<float64> xVec(gam.x1, gam.numberOfElementsX);
    ok &= gam.config.Write("Times", xVec);
    ok &= gam.Initialise(gam.config);
    return !ok;
}

bool WaveformPointsDefGAMTest::TestInvalidTimes2() {
    bool ok = true;
    WaveformPointsDefGAMTestHelper gam;
    Vector<float64> yVec(gam.y1, gam.numberOfElementsY);
    ok &= gam.config.Write("Points", yVec);
    for(uint32 i = 0; i< gam.numberOfElementsX; i++){
        gam.y1[i] = i;
    }
    Vector<float64> xVec(gam.x1, gam.numberOfElementsX);
    ok &= gam.config.Write("Times", xVec);
    ok &= gam.Initialise(gam.config);
    return !ok;
}

bool WaveformPointsDefGAMTest::TestFailWaveformSetup() {
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

