/**
 * @file WaveformPointsDefGAMTest.h
 * @brief Header file for class WaveformPointsDefGAMTest
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

 * @details This header file contains the declaration of the class WaveformPointsDefGAMTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef WAVEFORMPOINTSDEFGAMTEST_H_
#define WAVEFORMPOINTSDEFGAMTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "ConfigurationDatabase.h"
#include "GAM.h"
#include "StreamString.h"
#include "WaveformPointsDef.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

class WaveformPointsDefGAMTest {
public:

    /**
     * @brief default constructor
     */
    WaveformPointsDefGAMTest();

    /**
     * @brief default destructor
     */
    virtual ~WaveformPointsDefGAMTest();

    /**
     * @brief Test error message of WaveformPointsDef::Initialise()
     */
    bool TestInitialise_MissingPoints();

    /**
     * @brief Test error message of WaveformPointsDef::Initialise()
     */
    bool TestInitialise_FailingReadingPointsValues();

    /**
     * @brief Test error message of WaveformPointsDef::Initialise()
     */
    bool TestInitialise_1Point();

    /**
     * @brief Test error message of WaveformPointsDef::Initialise()
     */
    bool TestInitialise_MissingTimes();

    /**
     * @brief Test error message of WaveformPointsDef::Initialise()
     */
    bool TestInitialise_DifferentSizePointsTimes();

    /**
     * @brief Test error message of WaveformPointsDef::Initialise()
     */
    bool TestInitialise_FailingReadingTimesValues();

    /**
     * @brief Test error message of WaveformPointsDef::Initialise()
     */
    bool TestInitialise_InvalidTimes();

    /**
     * @brief Test error message of WaveformPointsDef::Initialise()
     */
    bool TestInitialise_InvalidTimes2();

    /**
     * @brief Test error message of WaveformPointsDef::Initialise()
     */
    bool TestInitialise_FailWaveformSetup();

    /**
     * @brief Template test. Verifies the correctness of the data.
     */
    template<typename T2>
    bool TestExecute(StreamString str);

    /**
     * @brief Template test. Verifies the correctness of the data with trigger mechanism Enable.
     */
    template<typename T3>
    bool TestExecuteTrigger(StreamString str);

    /**
     * @brief Template test. Verifies the correctness of the data with two output signals.
     */
    template<typename T>
    bool TestExecute2Signals(StreamString str);

    /**
     * @brief Template test. Verifies the correctness of the data with the minimum possible points array.
     */
    template<typename T>
    bool TestExecuteTrigger2Point(StreamString str);

    /**
     * @brief Template test. Verifies the correctness of the data with Only on element per cycle.
     */
    template<typename T>
    bool TestExecuteTrigger_1Element(StreamString str);

    /**
     * @brief Template test. Test execute messages errors.
     */
    template<typename T>
    bool TestExecuteWrongInput(StreamString str);

    /**
     * @brief Template test. Test execute messages errors.
     */
    template<typename T>
    bool TestExecuteWrongInput_2(StreamString str);

    /**
     * @brief Test WaveformPointsDef::Execute() in time extreme condition
     * @details Some of the increment times values are smaller than the sample time.
     * @return true if the output is as expected.
     */
    bool TestExecuteSmallIncrementTimes();

    /**
     * @brief Test WaveformPointsDef::Execute() with a sawtooth
     */
    bool TestExecuteSawtooth();

    /**
     * @brief Test WaveformPointsDef::Execute() with a sawtooth
     */
    bool TestExecuteSawtooth_4elements();
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/
namespace MARTe {

class WaveformPointsDefGAMTestHelper: public WaveformPointsDef {
public:

    WaveformPointsDefGAMTestHelper(uint32 elementsIn = 1,
                                   uint32 samplesIn = 1,
                                   uint32 elementsOut = 4,
                                   uint32 samplesOut = 1,
                                   StreamString str = "int8",
                                   uint32 refStartTrigger = 5u,
                                   uint32 reftStopTrigger = 4u,
                                   uint32 refNumberOfElementsY = 4u,
                                   uint32 refNumberOfElementsX = 4u) {
        numberOfElementsIn = elementsIn;
        numberOfSamplesIn = samplesIn;
        byteSizeIn = numberOfElementsIn * sizeof(uint32);
        // The input time could be int32 or uint32 both supported
        numberOfElementsOut = elementsOut;
        numberOfSamplesOut = samplesOut;

        byteSizeOut = 0;
        startTrigger = NULL;
        stopTrigger = NULL;
        elementsStartTrigger = refStartTrigger;
        elementsStopTrigger = reftStopTrigger;

        typeStr = str;
        type = TypeDescriptor::GetTypeDescriptorFromTypeName(typeStr.Buffer());
        isInitialised = false;
        numberOfElementsY = refNumberOfElementsY;
        numberOfElementsX = refNumberOfElementsX;
        x1 = new float64[numberOfElementsX];
        x11 = new float64[2];
        y1 = new float64[numberOfElementsY];
        y11 = new float64[2];
        for (uint32 i = 0u; i < numberOfElementsX; i++) {
            x1[i] = 0.0;
            y1[i] = 0.0;
        }
        dimArrayCompare1 = 15;
        refValues1 = new float64[dimArrayCompare1];
        refValues = NULL;
        ref1Values1 = new float64[2];
        indexCompare1 = 0;
        indexCompare2 = 0;
    }

    WaveformPointsDefGAMTestHelper(StreamString str,
                                   uint32 refStartTrigger = 5u,
                                   uint32 reftStopTrigger = 4u,
                                   uint32 refNumberOfElementsY = 4u,
                                   uint32 refNumberOfElementsX = 4u) {
        numberOfElementsIn = 1;
        numberOfSamplesIn = 1;
        byteSizeIn = numberOfElementsIn * sizeof(uint32);
        // The input time could be int32 or uint32 both supported
        numberOfElementsOut = 4;
        numberOfSamplesOut = 1;

        byteSizeOut = 0;
        startTrigger = NULL;
        stopTrigger = NULL;
        elementsStartTrigger = refStartTrigger;
        elementsStopTrigger = reftStopTrigger;
        typeStr = str;
        type = TypeDescriptor::GetTypeDescriptorFromTypeName(typeStr.Buffer());
        isInitialised = false;
        numberOfElementsY = refNumberOfElementsY;
        numberOfElementsX = refNumberOfElementsX;
        x1 = new float64[numberOfElementsX];
        x11 = new float64[2];
        y1 = new float64[numberOfElementsY];
        y11 = new float64[2];
        for (uint32 i = 0u; i < numberOfElementsX; i++) {
            x1[i] = 0.0;
            y1[i] = 0.0;
        }
        dimArrayCompare1 = 15;
        refValues1 = new float64[dimArrayCompare1];
        ref1Values1 = new float64[2];
        refValues = NULL;
        indexCompare1 = 0;
        indexCompare2 = 0;
        elementsStopTrigger = 4;
    }

    ~WaveformPointsDefGAMTestHelper() {
        if (y1 != NULL) {
            delete[] y1;
            y1 = NULL;
        }
        if (x1 != NULL) {
            delete[] x1;
            x1 = NULL;
        }
        if (startTrigger != NULL) {
            delete[] startTrigger;
        }
        if (stopTrigger != NULL) {
            delete[] stopTrigger;
        }
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
    bool InitialisePointsdefSawtooth() {
        bool ret = true;
        if (isInitialised == false) {
            x1[0] = 0;
            x1[1] = 0.9;
            y1[0] = 0;
            y1[1] = 9;
            refValues = new float64[10];
            refValues[0] = 0;
            refValues[1] = 1;
            refValues[2] = 2;
            refValues[3] = 3;
            refValues[4] = 4;
            refValues[5] = 5;
            refValues[6] = 6;
            refValues[7] = 7;
            refValues[8] = 8;
            refValues[9] = 9;
            dimArrayCompare1 = 10;
            Vector<float64> yVec(y1, numberOfElementsY);
            ret &= config.Write("Points", yVec);
            Vector<float64> xVec(x1, numberOfElementsX);
            ret &= config.Write("Times", xVec);
            isInitialised = ret;
        }
        else {
            ret = false;
        }
        return ret;
    }
    bool InitialisePointsdefTimesExtreme() {
        bool ret = true;
        if (isInitialised == false) {
            x1[0] = 0;
            x1[1] = 2;
            x1[2] = 2.1;
            x1[3] = 4;
            y1[0] = 0;
            y1[1] = 2;
            y1[2] = 1.6;
            y1[3] = 5.4;
            refValues1[0] = 0;
            refValues1[1] = 1;
            refValues1[2] = 2;
            refValues1[3] = 1.6 + 0.9 * 2.0;
            refValues1[4] = 5.4;
            refValues1[5] = 0;
            refValues1[6] = 1;
            refValues1[7] = 2;
            refValues1[8] = 1.6 + 0.9 * 2.0;
            refValues1[9] = 5.4;
            refValues1[10] = 0;
            refValues1[11] = 1;
            refValues1[12] = 2;
            refValues1[13] = 1.6 + 0.9 * 2.0;
            refValues1[14] = 5.4;

            Vector<float64> yVec(y1, numberOfElementsY);
            ret &= config.Write("Points", yVec);
            Vector<float64> xVec(x1, numberOfElementsX);
            ret &= config.Write("Times", xVec);
            isInitialised = ret;
        }
        else {
            ret = false;
        }
        return ret;
    }
    bool InitialisePointsdef1() {
        bool ret = true;
        if (isInitialised == false) {
            x1[0] = 0;
            x1[1] = 1.25;
            x1[2] = 2;
            x1[3] = 3.5;
            y1[0] = 0;
            y1[1] = 6;
            y1[2] = 5.25;
            y1[3] = 2.25;
            refValues1[0] = 0;
            refValues1[1] = 1.2;
            refValues1[2] = 2.4;
            refValues1[3] = 3.6;
            refValues1[4] = 4.8;
            refValues1[5] = 6;
            refValues1[6] = 5.75;
            refValues1[7] = 5.5;
            refValues1[8] = 5.25;
            refValues1[9] = 4.75;
            refValues1[10] = 4.25;
            refValues1[11] = 3.75;
            refValues1[12] = 3.25;
            refValues1[13] = 2.75;
            refValues1[14] = 2.25;

            Vector<float64> yVec(y1, numberOfElementsY);
            ret &= config.Write("Points", yVec);
            Vector<float64> xVec(x1, numberOfElementsX);
            ret &= config.Write("Times", xVec);
            isInitialised = ret;
        }
        else {
            ret = false;
        }
        return ret;
    }
    bool IsEqualLargerMargins(float64 f1,
                              float64 f2) {
        float64 *min = reinterpret_cast<float64*>(const_cast<uint64*>(&EPSILON_FLOAT64));
        float64 minLarger = *min * 200;
        return ((f1 - f2) < (minLarger)) && ((f1 - f2) > -(minLarger));
    }
    bool InitialisePointsdef1Trigger() {
        bool ret = true;

        if (isInitialised == false) {
            x1[0] = 0;
            x1[1] = 1.25;
            x1[2] = 2;
            x1[3] = 3.5;
            y1[0] = 0;
            y1[1] = 6;
            y1[2] = 5.25;
            y1[3] = 2.25;
            refValues1[0] = 0;
            refValues1[1] = 1.2;
            refValues1[2] = 2.4;
            refValues1[3] = 3.6;
            refValues1[4] = 4.8;
            refValues1[5] = 6;
            refValues1[6] = 5.75;
            refValues1[7] = 5.5;
            refValues1[8] = 5.25;
            refValues1[9] = 4.75;
            refValues1[10] = 4.25;
            refValues1[11] = 3.75;
            refValues1[12] = 3.25;
            refValues1[13] = 2.75;
            refValues1[14] = 2.25;
            Vector<float64> yVec(y1, numberOfElementsY);
            ret &= config.Write("Points", yVec);
            Vector<float64> xVec(x1, numberOfElementsX);
            ret &= config.Write("Times", xVec);

            startTrigger = new float64[elementsStartTrigger];
            stopTrigger = new float64[elementsStopTrigger];
            startTrigger[0] = 1.25;
            stopTrigger[0] = 1.75;
            startTrigger[1] = 2.;
            stopTrigger[1] = 3.25;
            startTrigger[2] = 3.5;
            stopTrigger[2] = 3.75;
            startTrigger[3] = 4.;
            stopTrigger[3] = 4.25;
            startTrigger[4] = 4.55;
            Vector<float64> startTVect(startTrigger, elementsStartTrigger);
            Vector<float64> stopTVect(stopTrigger, elementsStopTrigger);
            ret &= config.Write("StartTriggerTime", startTVect);
            ret &= config.Write("StopTriggerTime", stopTVect);
            isInitialised = ret;
        }
        else {
            ret = false;
        }

        return ret;

    }

    bool Initialise2Pointsdef1Trigger() {
        bool ret = true;

        if (isInitialised == false) {
            x11[0] = 0.0;
            x11[1] = 1.5;
            y11[0] = -5.25;
            y11[1] = -5.25;
            ref1Values1[0] = -5.25;
            ref1Values1[1] = -5.25;
            Vector<float64> yVec(y11, 2);
            ret &= config.Write("Points", yVec);
            Vector<float64> xVec(x11, 2);
            ret &= config.Write("Times", xVec);

            startTrigger = new float64[elementsStartTrigger];
            stopTrigger = new float64[elementsStopTrigger];
            startTrigger[0] = 1.25;
            stopTrigger[0] = 1.75;
            startTrigger[1] = 2.;
            stopTrigger[1] = 3.25;
            startTrigger[2] = 3.5;
            stopTrigger[2] = 3.75;
            startTrigger[3] = 4.;
            stopTrigger[3] = 4.25;
            startTrigger[4] = 4.55;
            Vector<float64> startTVect(startTrigger, elementsStartTrigger);
            Vector<float64> stopTVect(stopTrigger, elementsStopTrigger);
            ret &= config.Write("StartTriggerTime", startTVect);
            ret &= config.Write("StopTriggerTime", stopTVect);
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
        uint32 totalByteSizeIn = byteSizeIn;
        ok &= configSignals.CreateAbsolute("Signals.InputSignals");
        ok &= configSignals.CreateRelative("0");
        ok &= configSignals.Write("QualifiedName", "InputSignal1");
        ok &= configSignals.Write("DataSource", "TestDataSource");
        ok &= configSignals.Write("Type", "uint32");
        ok &= configSignals.Write("NumberOfDimensions", 1);
        ok &= configSignals.Write("NumberOfElements", 1);
        ok &= configSignals.Write("ByteSize", totalByteSizeIn);

        ok &= configSignals.MoveToAncestor(1u);
        ok &= configSignals.Write("ByteSize", totalByteSizeIn);

        uint32 totalByteSizeOut = numberOfElementsOut * type.numberOfBits / 8;
        ok &= configSignals.MoveToRoot();
        ok &= configSignals.CreateAbsolute("Signals.OutputSignals");
        ok &= configSignals.CreateRelative("0");
        ok &= configSignals.Write("QualifiedName", "OutputSignal1");
        ok &= configSignals.Write("DataSource", "TestDataSource");
        ok &= configSignals.Write("Type", TypeDescriptor::GetTypeNameFromTypeDescriptor(type));
        ok &= configSignals.Write("NumberOfDimensions", 1);

        ok &= configSignals.Write("NumberOfElements", numberOfElementsOut);
        ok &= configSignals.Write("ByteSize", totalByteSizeOut);

        ok &= configSignals.MoveToAncestor(1u);
        ok &= configSignals.Write("ByteSize", totalByteSizeOut);

        ok &= configSignals.MoveToRoot();

        ok &= configSignals.CreateAbsolute("Memory.InputSignals");
        ok &= configSignals.CreateRelative("0");
        ok &= configSignals.Write("DataSource", "TestDataSource");
        ok &= configSignals.CreateRelative("Signals");
        ok &= configSignals.CreateRelative("0");
        ok &= configSignals.Write("Samples", numberOfSamplesIn);

        ok &= configSignals.CreateAbsolute("Memory.OutputSignals");
        ok &= configSignals.CreateRelative("0");
        ok &= configSignals.Write("DataSource", "TestDataSource");
        ok &= configSignals.CreateRelative("Signals");
        ok &= configSignals.CreateRelative("0");
        ok &= configSignals.Write("Samples", numberOfSamplesOut);

        ok &= configSignals.MoveToRoot();
        return ok;
    }

    bool InitialiseConfigDataBaseSignal2() {
        bool ok = true;
        uint32 totalByteSizeIn = byteSizeIn;
        ok &= configSignals.CreateAbsolute("Signals.InputSignals");
        ok &= configSignals.CreateRelative("0");
        ok &= configSignals.Write("QualifiedName", "InputSignal1");
        ok &= configSignals.Write("DataSource", "TestDataSource");
        ok &= configSignals.Write("Type", "uint32");
        ok &= configSignals.Write("NumberOfDimensions", 1);
        ok &= configSignals.Write("NumberOfElements", 1);
        ok &= configSignals.Write("ByteSize", totalByteSizeIn);

        ok &= configSignals.MoveToAncestor(1u);
        ok &= configSignals.Write("ByteSize", totalByteSizeIn);

        uint32 totalByteSizeOut1 = numberOfElementsOut * type.numberOfBits / 8;
        ok &= configSignals.MoveToRoot();
        ok &= configSignals.CreateAbsolute("Signals.OutputSignals");
        ok &= configSignals.CreateRelative("0");
        ok &= configSignals.Write("QualifiedName", "OutputSignal1");
        ok &= configSignals.Write("DataSource", "TestDataSource");
        ok &= configSignals.Write("Type", TypeDescriptor::GetTypeNameFromTypeDescriptor(type));
        ok &= configSignals.Write("NumberOfDimensions", 1);
        ok &= configSignals.Write("NumberOfElements", numberOfElementsOut);
        ok &= configSignals.Write("ByteSize", totalByteSizeOut1);

        uint32 totalByteSizeOut2 = numberOfElementsOut * sizeof(uint32);
        ok &= configSignals.MoveToAncestor(1u);
        ok &= configSignals.CreateRelative("1");
        ok &= configSignals.Write("QualifiedName", "OutputSignal2");
        ok &= configSignals.Write("DataSource", "TestDataSource");
        ok &= configSignals.Write("Type", "uint32");
        ok &= configSignals.Write("NumberOfDimensions", 1);
        ok &= configSignals.Write("NumberOfElements", numberOfElementsOut);
        ok &= configSignals.Write("ByteSize", totalByteSizeOut2);

        ok &= configSignals.MoveToAncestor(1u);

        uint32 totalByteSizeOut = totalByteSizeOut1 + totalByteSizeOut2;
        ok &= configSignals.Write("ByteSize", totalByteSizeOut);

        ok &= configSignals.MoveToRoot();

        ok &= configSignals.CreateAbsolute("Memory.InputSignals");
        ok &= configSignals.CreateRelative("0");
        ok &= configSignals.Write("DataSource", "TestDataSource");
        ok &= configSignals.CreateRelative("Signals");
        ok &= configSignals.CreateRelative("0");
        ok &= configSignals.Write("Samples", numberOfSamplesIn);

        ok &= configSignals.CreateAbsolute("Memory.OutputSignals");
        ok &= configSignals.CreateRelative("0");
        ok &= configSignals.Write("DataSource", "TestDataSource");
        ok &= configSignals.CreateRelative("Signals");
        ok &= configSignals.CreateRelative("0");
        ok &= configSignals.Write("Samples", numberOfSamplesOut);
        ok &= configSignals.MoveToAncestor(1u);
        ok &= configSignals.CreateRelative("1");
        ok &= configSignals.Write("Samples", numberOfSamplesOut);

        ok &= configSignals.MoveToRoot();
        return ok;
    }

    bool ComparePointsdef(float64 *output,
                          bool firstIteration,
                          float64 &error) {
        bool ret = true;
        for (uint32 i = 0; (i < numberOfElementsOut) && ret; i++) {
            if (firstIteration) {
                ret = (static_cast<float64>(0.0) == output[i]);
            }
            else {
                ret = IsEqualLargerMargins(refValues[indexCompare1], output[i]);
            }

            error = refValues[indexCompare1] - output[i];
            indexCompare1++;
            if (indexCompare1 == dimArrayCompare1) {
                indexCompare1 = 0;
            }
        }
        return ret;
    }

    template<typename T>
    bool ComparePointsdef1(T *output,
                           bool firstIteration) {
        bool ret = true;
        for (uint32 i = 0; (i < numberOfElementsOut) && ret; i++) {
            if (firstIteration) {
                ret = (static_cast<T>(0.0) == output[i]);
            }
            else {
                StreamString auxStr = TypeDescriptor::GetTypeNameFromTypeDescriptor(type);
                if (auxStr == "float32") {
                    ret = IsEqual(static_cast<float32>(refValues1[indexCompare1]), static_cast<float32>(output[i]));
                }
                else if (auxStr == "float64") {
                    ret = IsEqualLargerMargins(static_cast<float64>(refValues1[indexCompare1]), static_cast<float64>(output[i]));
                }
                else {
                    ret = (static_cast<T>(refValues1[indexCompare1]) == output[i]);
                }
            }
            indexCompare1++;
            if (indexCompare1 == dimArrayCompare1) {
                indexCompare1 = 0;
            }
        }
        if (firstIteration) {
            firstIteration = false;
        }
        return ret;
    }

    template<typename T>
    bool ComparePointsdef1Trigger(T *output,
                                  float64 t,
                                  float64 it) {
        bool ret = true;
        static bool firstIteration = true;
        for (uint32 i = 0; (i < numberOfElementsOut) && ret; i++) {
            if (firstIteration) {
                ret = (static_cast<T>(0.0) == output[i]);
            }
            else {
                if (ShouldBeSignalOutOn(t)) {
                    StreamString auxStr = TypeDescriptor::GetTypeNameFromTypeDescriptor(type);
                    if (auxStr == "float32") {
                        ret = IsEqual(static_cast<float32>(refValues1[indexCompare1]), static_cast<float32>(output[i]));
                    }
                    else if (auxStr == "float64") {
                        ret = IsEqualLargerMargins(static_cast<float64>(refValues1[indexCompare1]), static_cast<float64>(output[i]));
                    }
                    else {
                        ret = (static_cast<T>(refValues1[indexCompare1]) == output[i]);
                    }
                }
                else {
                    ret = (static_cast<T>(0.0) == output[i]);
                }
            }
            indexCompare1++;
            if (indexCompare1 == dimArrayCompare1) {
                indexCompare1 = 0;
            }
            t += it;
        }
        if (firstIteration) {
            firstIteration = false;
        }
        return ret;
    }

    template<typename T>
    bool Compare2Pointsdef1Trigger(T *output,
                                   float64 t,
                                   float64 it) {
        bool ret = true;
        static bool firstIteration = true;
        for (uint32 i = 0; (i < numberOfElementsOut) && ret; i++) {
            if (firstIteration) {
                ret = (static_cast<T>(0.0) == output[i]);
            }
            else {
                if (ShouldBeSignalOutOn(t)) {
                    StreamString auxStr = TypeDescriptor::GetTypeNameFromTypeDescriptor(type);
                    if (auxStr == "float32") {
                        ret = IsEqual(static_cast<float32>(ref1Values1[indexCompare1]), static_cast<float32>(output[i]));
                    }
                    else if (auxStr == "float64") {
                        ret = IsEqualLargerMargins(static_cast<float64>(ref1Values1[indexCompare1]), static_cast<float64>(output[i]));
                    }
                    else {
                        ret = (static_cast<T>(ref1Values1[indexCompare1]) == output[i]);
                    }
                }
                else {
                    ret = (static_cast<T>(0.0) == output[i]);
                }
            }
            indexCompare1++;
            if (indexCompare1 == 2) {
                indexCompare1 = 0;
            }
            t += it;
        }
        if (firstIteration) {
            firstIteration = false;
        }
        return ret;
    }

    template<typename T>
    bool ComparePointsdef1Trigger2(T *output,
                                   float64 t,
                                   float64 it) {
        bool ret = true;
        static bool firstIteration = true;
        for (uint32 i = 0; (i < numberOfElementsOut) && ret; i++) {
            if (firstIteration) {
                ret = (static_cast<T>(0.0) == output[i]);
            }
            else {
                if (ShouldBeSignalOutOn(t)) {
                    StreamString auxStr = "uint32";
                    if (auxStr == "float32") {
                        ret = IsEqual(static_cast<float32>(refValues1[indexCompare2]), static_cast<float32>(output[i]));
                    }
                    else if (auxStr == "float64") {
                        ret = IsEqualLargerMargins(static_cast<float64>(refValues1[indexCompare2]), static_cast<float64>(output[i]));
                    }
                    else {
                        ret = (static_cast<T>(refValues1[indexCompare2]) == output[i]);
                    }
                }
                else {
                    ret = (static_cast<T>(0.0) == output[i]);
                }
            }
            indexCompare2++;
            if (indexCompare2 == dimArrayCompare1) {
                indexCompare2 = 0;
            }
            t += it;
        }
        if (firstIteration) {
            firstIteration = false;
        }
        return ret;
    }

    uint32 numberOfElementsIn;
    uint32 numberOfSamplesIn;
    uint32 numberOfElementsOut;
    uint32 numberOfSamplesOut;
    uint32 byteSizeIn;
    uint32 byteSizeOut;
    ConfigurationDatabase configSignals;
    ConfigurationDatabase config;
    TypeDescriptor type;
    StreamString typeStr;
    float64 *y11;
    float64 *y1;
    float64 *x1;
    float64 *x11;
    uint32 numberOfElementsY;
    uint32 numberOfElementsX;
    uint32 indexCompare1;
    uint32 indexCompare2;
    float64 *refValues1;
    float64 *refValues;
    float64 *ref1Values1;
private:
    bool isInitialised;
    float64 *startTrigger;
    float64 *stopTrigger;
    uint32 elementsStartTrigger;
    uint32 elementsStopTrigger;
    uint32 dimArrayCompare1;

    bool ShouldBeSignalOutOn(float64 t) { //assuming a lot of thinks which are not checked i.e elementsStopTrigger cannot be greater than elementsStartTrigger
        bool ret;
        bool stop = false;
        for (uint32 i = 0; (i < elementsStopTrigger) && !stop; i++) {
            if (t < stopTrigger[i]) {
                if (t >= startTrigger[i]) {
                    ret = true;
                }
                else {
                    ret = false;
                }
                stop = true;
            }
        }
        if (!stop) {
            if (elementsStartTrigger > elementsStopTrigger) {
                if (t >= startTrigger[elementsStartTrigger - 1]) {
                    ret = true;
                }
                else {
                    ret = false;
                }
            }
            else {
                ret = false;
            }
        }
        return ret;
    }
}
;
//CLASS_REGISTER(WaveformPointsDefGAMTestHelper, "1.0")
}

template<typename T1>
bool WaveformPointsDefGAMTest::TestExecute(StreamString str) {
    using namespace MARTe;
    bool ok = true;
    uint32 timeIterationIncrement = 1000000u;
    uint32 *timeIteration = NULL;
    uint32 numberOfIteration = 4000u; //if the number of iteration is too large the time will overflow
    uint32 sizeOutput = 4u;
    WaveformPointsDefGAMTestHelper gam(1, 1, sizeOutput, 1, str);

    T1 *output = NULL;

    gam.SetName("Test");
    ok &= gam.InitialisePointsdef1();
    gam.config.MoveToRoot();
    ok &= gam.Initialise(gam.config);

    ok &= gam.InitialiseConfigDataBaseSignal1();
    ok &= gam.SetConfiguredDatabase(gam.configSignals);
    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();

    ok &= gam.Setup();

    timeIteration = static_cast<uint32 *>(gam.GetInputSignalsMemory());
    *timeIteration = 0;
    output = static_cast<T1 *>(gam.GetOutputSignalsMemory());
    for (uint32 i = 0u; i < sizeOutput; i++) {
        output[i] = static_cast<T1>(0.0);
    }
    for (uint32 i = 0u; (i < numberOfIteration) && ok; i++) {
        ok = gam.Execute();
        if (!ok) {
            REPORT_ERROR_STATIC_PARAMETERS(ErrorManagement::FatalError, "Execute fails in iteration %u\n", i);
        }
        if (ok) {
            if (i == 0u) {
                ok = gam.ComparePointsdef1(output, true);
            }
            else {
                ok = gam.ComparePointsdef1(output, false);
            }
            if (!ok) {
                REPORT_ERROR_STATIC_PARAMETERS(ErrorManagement::FatalError, "iteration which fails %u\n", i);
            }
        }
        *timeIteration += timeIterationIncrement;
    }
    return ok;
}

template<typename T4>
bool WaveformPointsDefGAMTest::TestExecuteTrigger(StreamString str) {
    using namespace MARTe;
    bool ok = true;
    uint32 timeIterationIncrement = 1000000u;
    uint32 *timeIteration = NULL;
    uint32 numberOfIteration = 4000u; //if the number of iteration is too large the time will overflow
    uint32 sizeOutput = 4u;
    WaveformPointsDefGAMTestHelper gam(1, 1, sizeOutput, 1, str);

    T4 *output = NULL;

    gam.SetName("Test");
    ok &= gam.InitialisePointsdef1Trigger();
    gam.config.MoveToRoot();
    ok &= gam.Initialise(gam.config);

    ok &= gam.InitialiseConfigDataBaseSignal1();
    ok &= gam.SetConfiguredDatabase(gam.configSignals);
    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();

    ok &= gam.Setup();

    timeIteration = static_cast<uint32 *>(gam.GetInputSignalsMemory());
    *timeIteration = 0;
    output = static_cast<T4 *>(gam.GetOutputSignalsMemory());
    for (uint32 i = 0u; i < sizeOutput; i++) {
        output[i] = static_cast<T4>(0.0);
    }
    for (uint32 i = 0u; (i < numberOfIteration) && ok; i++) {
        gam.Execute();
        ok &= gam.ComparePointsdef1Trigger(output, ((float64) (*timeIteration) / 1e6), (float64) (timeIterationIncrement) / gam.numberOfElementsOut / 1e6);
        if (!ok) {
            REPORT_ERROR_STATIC_PARAMETERS(ErrorManagement::FatalError, "iteration which fails %u\n", i);
        }
        *timeIteration += timeIterationIncrement;
    }
    return ok;
}

template<typename T4>
bool WaveformPointsDefGAMTest::TestExecute2Signals(StreamString str) {
    using namespace MARTe;
    bool ok = true;
    uint32 timeIterationIncrement = 1000000u;
    uint32 *timeIteration = NULL;
    uint32 numberOfIteration = 4000u; //if the number of iteration is too large the time will overflow
    uint32 sizeOutput = 4u;
    WaveformPointsDefGAMTestHelper gam(1, 1, sizeOutput, 1, str);

    T4 *output = NULL;
    uint32 *output2 = NULL;

    gam.SetName("Test");
    ok &= gam.InitialisePointsdef1Trigger();
    gam.config.MoveToRoot();
    ok &= gam.Initialise(gam.config);

    ok &= gam.InitialiseConfigDataBaseSignal2();
    ok &= gam.SetConfiguredDatabase(gam.configSignals);
    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();

    ok &= gam.Setup();

    timeIteration = static_cast<uint32 *>(gam.GetInputSignalsMemory());
    *timeIteration = 0;
    output = static_cast<T4 *>(gam.GetOutputSignalsMemory());
    output2 = static_cast<uint32 *>(gam.GetOutputSignalsMemory(1));

    for (uint32 i = 0u; i < sizeOutput; i++) {
        output[i] = static_cast<T4>(0.0);
        output[i] = static_cast<uint32>(0.0);
    }
    for (uint32 i = 0u; (i < numberOfIteration) && ok; i++) {
        gam.Execute();
        ok &= gam.ComparePointsdef1Trigger(output, ((float64) (*timeIteration) / 1e6), (float64) (timeIterationIncrement) / gam.numberOfElementsOut / 1e6);
        ok &= gam.ComparePointsdef1Trigger2(output2, ((float64) (*timeIteration) / 1e6), (float64) (timeIterationIncrement) / gam.numberOfElementsOut / 1e6);
        if (!ok) {
            REPORT_ERROR_STATIC_PARAMETERS(ErrorManagement::FatalError, "iteration which fails %u\n", i);
        }
        *timeIteration += timeIterationIncrement;
    }
    return ok;
}

template<typename T>
bool WaveformPointsDefGAMTest::TestExecuteTrigger2Point(StreamString str) {
    using namespace MARTe;
    bool ok = true;
    uint32 timeIterationIncrement = 1000000u;
    uint32 *timeIteration = NULL;
    uint32 numberOfIteration = 4000u; //if the number of iteration is too large the time will overflow
    uint32 sizeOutput = 4u;
    WaveformPointsDefGAMTestHelper gam(1, 1, sizeOutput, 1, str);

    T *output = NULL;

    gam.SetName("Test");
    ok &= gam.Initialise2Pointsdef1Trigger();
    gam.config.MoveToRoot();
    ok &= gam.Initialise(gam.config);

    ok &= gam.InitialiseConfigDataBaseSignal1();
    ok &= gam.SetConfiguredDatabase(gam.configSignals);
    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();

    ok &= gam.Setup();

    timeIteration = static_cast<uint32 *>(gam.GetInputSignalsMemory());
    *timeIteration = 0;
    output = static_cast<T *>(gam.GetOutputSignalsMemory());
    for (uint32 i = 0u; i < sizeOutput; i++) {
        output[i] = static_cast<T>(0.0);
    }
    for (uint32 i = 0u; (i < numberOfIteration) && ok; i++) {
        gam.Execute();
        ok &= gam.Compare2Pointsdef1Trigger(output, ((float64) (*timeIteration) / 1e6), (float64) (timeIterationIncrement) / gam.numberOfElementsOut / 1e6);
        if (!ok) {
            REPORT_ERROR_STATIC_PARAMETERS(ErrorManagement::FatalError, "iteration which fails %u\n", i);
        }
        *timeIteration += timeIterationIncrement;
    }
    return ok;
}

template<typename T>
bool WaveformPointsDefGAMTest::TestExecuteTrigger_1Element(StreamString str) {
    using namespace MARTe;
    bool ok = true;
    uint32 timeIterationIncrement = 250000u;
    uint32 *timeIteration = NULL;
    uint32 numberOfIteration = 4000u; //if the number of iteration is too large the time will overflow
    uint32 sizeOutput = 1u;
    WaveformPointsDefGAMTestHelper gam(1, 1, sizeOutput, 1, str);

    T *output = NULL;

    gam.SetName("Test");
    ok &= gam.InitialisePointsdef1Trigger();
    gam.config.MoveToRoot();
    ok &= gam.Initialise(gam.config);

    ok &= gam.InitialiseConfigDataBaseSignal1();
    ok &= gam.SetConfiguredDatabase(gam.configSignals);
    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();

    ok &= gam.Setup();

    timeIteration = static_cast<uint32 *>(gam.GetInputSignalsMemory());
    *timeIteration = 0;
    output = static_cast<T *>(gam.GetOutputSignalsMemory());
    for (uint32 i = 0u; i < sizeOutput; i++) {
        output[i] = static_cast<T>(0.0);
    }
    for (uint32 i = 0u; (i < numberOfIteration) && ok; i++) {
        gam.Execute();
        ok &= gam.ComparePointsdef1Trigger(output, ((float64) (*timeIteration) / 1e6), (float64) (timeIterationIncrement) / gam.numberOfElementsOut / 1e6);
        if (!ok) {
            REPORT_ERROR_STATIC_PARAMETERS(ErrorManagement::FatalError, "iteration which fails %u\n", i);
        }
        *timeIteration += timeIterationIncrement;
    }
    return ok;
}

template<typename T>
bool WaveformPointsDefGAMTest::TestExecuteWrongInput(StreamString str) {
    using namespace MARTe;
    bool ok = true;
    uint32 timeIterationIncrement = 250000u;
    uint32 *timeIteration = NULL;
    uint32 sizeOutput = 1u;
    WaveformPointsDefGAMTestHelper gam(1, 1, sizeOutput, 1, str);

    T *output = NULL;

    gam.SetName("Test");
    ok &= gam.InitialisePointsdef1Trigger();
    gam.config.MoveToRoot();
    ok &= gam.Initialise(gam.config);

    ok &= gam.InitialiseConfigDataBaseSignal1();
    ok &= gam.SetConfiguredDatabase(gam.configSignals);
    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();

    ok &= gam.Setup();

    timeIteration = static_cast<uint32 *>(gam.GetInputSignalsMemory());
    *timeIteration = 0;
    output = static_cast<T *>(gam.GetOutputSignalsMemory());
    for (uint32 i = 0u; i < sizeOutput; i++) {
        output[i] = static_cast<T>(0.0);
    }
    if (ok) {
        ok = gam.Execute();
    }
    if (ok) {
        ok &= gam.ComparePointsdef1Trigger(output, ((float64) (*timeIteration) / 1e6), (float64) (timeIterationIncrement) / gam.numberOfElementsOut / 1e6);
    }
    ok &= !gam.Execute();

    return ok;
}

template<typename T>
bool WaveformPointsDefGAMTest::TestExecuteWrongInput_2(StreamString str) {
    using namespace MARTe;
    bool ok = true;
    uint32 timeIterationIncrement = 250000u;
    uint32 *timeIteration = NULL;
    uint32 sizeOutput = 1u;
    WaveformPointsDefGAMTestHelper gam(1, 1, sizeOutput, 1, str);

    T *output = NULL;

    gam.SetName("Test");
    ok &= gam.InitialisePointsdef1Trigger();
    gam.config.MoveToRoot();
    ok &= gam.Initialise(gam.config);

    ok &= gam.InitialiseConfigDataBaseSignal1();
    ok &= gam.SetConfiguredDatabase(gam.configSignals);
    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();

    ok &= gam.Setup();

    timeIteration = static_cast<uint32 *>(gam.GetInputSignalsMemory());
    *timeIteration = 0;
    output = static_cast<T *>(gam.GetOutputSignalsMemory());
    for (uint32 i = 0u; i < sizeOutput; i++) {
        output[i] = static_cast<T>(0.0);
    }
    if (ok) {
        ok = gam.Execute();
    }
    if (ok) {
        ok &= gam.ComparePointsdef1Trigger(output, ((float64) (*timeIteration) / 1e6), (float64) (timeIterationIncrement) / gam.numberOfElementsOut / 1e6);
    }
    if (ok) {
        *timeIteration += timeIterationIncrement;
        ok &= gam.Execute();
    }
    if (ok) {
        ok &= gam.ComparePointsdef1Trigger(output, ((float64) (*timeIteration) / 1e6), (float64) (timeIterationIncrement) / gam.numberOfElementsOut / 1e6);
    }
    if (ok) { //no input time increment--> Execute should fail.
        ok &= !gam.Execute();
    }

    return ok;
}

#endif /*WAVEFORMPOINTSDEFGAMTEST_H_ */

