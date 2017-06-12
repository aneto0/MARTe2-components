/**
 * @file WaveformChirpGAMTest.h
 * @brief Header file for class WaveformChirpGAMTest
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

 * @details This header file contains the declaration of the class WaveformChirpGAMTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef WAVEFORMCHIRPGAMTEST_H_
#define WAVEFORMCHIRPGAMTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
#include "math.h"
#include "FastMath.h"
#include "stdio.h"
/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "WaveformChirp.h"
#include "StreamString.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

class WaveformChirpGAMTest {
//TODO Add the macro DLL_API to the class declaration (i.e. class DLL_API WaveformChirpGAMTest)
public:
    /**
     * @brief Default constructor
     */
    WaveformChirpGAMTest();

    /**
     * Default destructor
     */
    virtual ~WaveformChirpGAMTest();

    /**
     * @brief Test error message of WaveformChirp::Initialise()
     */
    bool TestMissingAmplitude();

    /**
     * @brief Test error message of WaveformChirp::Initialise()
     */
    bool Test0Amplitude();

    /**
     * @brief Test error message of WaveformChirp::Initialise()
     */
    bool TestMissingPhase();

    /**
     * @brief Test error message of WaveformChirp::Initialise()
     */
    bool TestMissingOffset();

    /**
     * @brief Test error message of WaveformChirp::Initialise()
     */
    bool TestMissingFreq1();

    /**
     * @brief Test error message of WaveformChirp::Initialise()
     */
    bool TestMissingFreq2();

    /**
     * @brief Test error message of WaveformChirp::Initialise()
     */
    bool TestMissingChirpDuration();

    /**
     * @brief Test error message of WaveformChirp::Initialise()
     */
    bool Test0ChirpDuration();

    /**
     * @brief Template test. Verifies the correctness of the data.
     */
    template<typename T>
    bool TestExecute(StreamString str);

    /**
     * @brief Template test. Verifies the correctness of the data with trigger mechanism Enable.
     */
    template<typename T>
    bool TestExecuteTrigger(StreamString str);

    /**
     * @brief Template test. Verifies the correctness of the data with trigger mechanism Enable
     * and two output signals.
     */
    template<typename T>
    bool TestExecute2Signals(StreamString str);

    /**
     * @brief Template test. Verifies the correctness of the data with trigger mechanism Enable
     * and one element per cycle.
     */
    template<typename T>
    bool TestExecute1ElementPerCycle(StreamString str);


};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/
namespace MARTe {
class WaveformChirpGAMTestHelper: public WaveformChirp {
public:

    WaveformChirpGAMTestHelper(uint32 elementsIn = 1,
                               uint32 samplesIn = 1,
                               uint32 elementsOut = 4,
                               uint32 samplesOut = 1,
                               StreamString str = "int8") {
        numberOfElementsIn = elementsIn;
        numberOfSamplesIn = samplesIn;
        byteSizeIn = numberOfElementsIn * sizeof(uint32);
        // The input time could be int32 or uint32 both supported
        numberOfElementsOut = elementsOut;
        numberOfSamplesOut = samplesOut;

        byteSizeOut = 0;
        startTrigger = NULL;
        stopTrigger = NULL;
        elementsStartTrigger = 5;
        elementsStopTrigger = 4;

        typeStr = str;
        type = TypeDescriptor::GetTypeDescriptorFromTypeName(typeStr.Buffer());
        isInitialised = false;
        numberOfElementsY = 4;
        numberOfElementsX = 4;
        f1 = 0.0;
        f2 = 10.0;
        amplitude = 10.0;
        phase = 0.0;
        offset = 0.0;
        chirpDuration = 0.0;
    }

    WaveformChirpGAMTestHelper(StreamString str) {
        numberOfElementsIn = 1;
        numberOfSamplesIn = 1;
        byteSizeIn = numberOfElementsIn * sizeof(uint32);
        // The input time could be int32 or uint32 both supported
        numberOfElementsOut = 4;
        numberOfSamplesOut = 1;

        byteSizeOut = 0;
        startTrigger = NULL;
        stopTrigger = NULL;
        elementsStartTrigger = 5;
        elementsStopTrigger = 4;
        typeStr = str;
        type = TypeDescriptor::GetTypeDescriptorFromTypeName(typeStr.Buffer());
        isInitialised = false;
        numberOfElementsY = 4;
        numberOfElementsX = 4;
        f1 = 0.0;
        f2 = 10.0;
        amplitude = 10.0;
        phase = 0.0;
        offset = 0.0;
        chirpDuration = 0.0;
    }

    ~WaveformChirpGAMTestHelper() {
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
    bool InitialiseChirp1() {
        bool ret = true;
        if (isInitialised == false) {
            f1 = 0;
            f2 = 10;
            amplitude = 10.0;
            phase = 0.0;
            offset = 0.0;
            chirpDuration = 5.0;
            ret &= config.Write("Frequency1", f1);
            ret &= config.Write("Frequency2", f2);
            ret &= config.Write("Amplitude", amplitude);
            ret &= config.Write("Phase", phase);
            ret &= config.Write("Offset", offset);
            ret &= config.Write("ChirpDuration", chirpDuration);
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
        float64 minLarger = *min * 3;
        return ((f1 - f2) < (minLarger)) && ((f1 - f2) > -(minLarger));
    }

    bool IsEqualLargerMargins(float32 f1,
                              float32 f2) {
        float32 *min = reinterpret_cast<float32*>(const_cast<uint32*>(&EPSILON_FLOAT32));
        float32 minLarger = *min * 3;
        return ((f1 - f2) < (minLarger)) && ((f1 - f2) > -(minLarger));
    }

    bool InitialiseChirp1Trigger() {
        bool ret = true;
        if (isInitialised == false) {
            f1 = 0;
            f2 = 10;
            amplitude = 10.0;
            phase = 0.0;
            offset = 0.0;
            chirpDuration = 5.0;
            ret &= config.Write("Frequency1", f1);
            ret &= config.Write("Frequency2", f2);
            ret &= config.Write("Amplitude", amplitude);
            ret &= config.Write("Phase", phase);
            ret &= config.Write("Offset", offset);
            ret &= config.Write("ChirpDuration", chirpDuration);
            isInitialised = ret;

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

    template<typename T>
    bool CompareChirp1(T *output,
                       float64 t,
                       float64 it) {
        bool ret = true;
        static bool firstIteration = true;
        for (uint32 i = 0; (i < numberOfElementsOut) && ret; i++) {
            if (firstIteration) {
                ret = (static_cast<T>(0.0) == output[i]);
            }
            else {
                float64 refValue = amplitude * sin(2.0 * FastMath::PI * f1 * t + 2 * FastMath::PI * (f2 - f1) * t * t / 2.0 / chirpDuration + phase) + offset;
                StreamString auxStr = TypeDescriptor::GetTypeNameFromTypeDescriptor(type);
                if (auxStr == "float32") {
                    ret = IsEqualLargerMargins(static_cast<float32>(refValue), static_cast<float32>(output[i]));
                }
                else if (auxStr == "float64") {
                    ret = IsEqualLargerMargins(static_cast<float64>(refValue), static_cast<float64>(output[i]));
                }
                else {
                    ret = (static_cast<T>(refValue) == output[i]);
                }
            }
            t += it;
        }
        if (firstIteration) {
            firstIteration = false;
        }
        return ret;
    }

    template<typename T>
    bool CompareChirp1Trigger(T *output,
                              float64 t,
                              float64 it) {
        bool ret = true;

        static bool firstIteration = true;
        for (uint32 i = 0; (i < numberOfElementsOut) && ret; i++) {
            if (firstIteration) {
                ret = (static_cast<T>(0.0) == output[i]);
            }
            else {
                float64 refValue = amplitude * sin(2.0 * FastMath::PI * f1 * t + 2 * FastMath::PI * (f2 - f1) * t * t / 2.0 / chirpDuration + phase) + offset;
                if (ShouldBeSignalOutOn(t)) {
                    StreamString auxStr = TypeDescriptor::GetTypeNameFromTypeDescriptor(type);
                    if (auxStr == "float32") {
                        ret = IsEqualLargerMargins(static_cast<float32>(refValue), static_cast<float32>(output[i]));
                    }
                    else if (auxStr == "float64") {
                        ret = IsEqualLargerMargins(static_cast<float64>(refValue), static_cast<float64>(output[i]));
                    }
                    else {
                        ret = (static_cast<T>(refValue) == output[i]);
                    }
                }
                else {
                    ret = (static_cast<T>(0.0) == output[i]);
                }
            }

            if (firstIteration) {
                firstIteration = false;
            }
            t += it;
        }
        if (firstIteration) {
            firstIteration = false;
        }
        return ret;
    }

    template<typename T>
    bool CompareChirpTrigger2(T *output,
                              float64 t,
                              float64 it) {
        bool ret = true;

        static bool firstIteration = true;
        for (uint32 i = 0; (i < numberOfElementsOut) && ret; i++) {
            if (firstIteration) {
                ret = (static_cast<T>(0.0) == output[i]);
            }
            else {
                float64 refValue = amplitude * sin(2.0 * FastMath::PI * f1 * t + 2 * FastMath::PI * (f2 - f1) * t * t / 2.0 / chirpDuration + phase) + offset;
                if (ShouldBeSignalOutOn(t)) {
                    StreamString auxStr = "uint32";
                    if (auxStr == "float32") {
                        ret = IsEqualLargerMargins(static_cast<float32>(refValue), static_cast<float32>(output[i]));
                    }
                    else if (auxStr == "float64") {
                        ret = IsEqualLargerMargins(static_cast<float64>(refValue), static_cast<float64>(output[i]));
                    }
                    else {
                        ret = (static_cast<T>(refValue) == output[i]);
                    }
                }
                else {
                    ret = (static_cast<T>(0.0) == output[i]);
                }
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
    uint32 numberOfElementsY;
    uint32 numberOfElementsX;
    float64 f1;
    float64 f2;
    float64 amplitude;
    float64 offset;
    float64 phase;
    float64 chirpDuration;
private:
    bool isInitialised;
    float64 *startTrigger;
    float64 *stopTrigger;
    uint32 elementsStartTrigger;
    uint32 elementsStopTrigger;

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

#endif /* WAVEFORMCHIRPGAMTEST_H_ */

template<typename T>
bool WaveformChirpGAMTest::TestExecute(StreamString str) {
    using namespace MARTe;
    bool ok = true;
    uint32 timeIterationIncrement = 1000000u;
    uint32 *timeIteration = NULL;
    uint32 numberOfIteration = 4000u; //if the number of iteration is too large the time will overflow
    uint32 sizeOutput = 4u;
    WaveformChirpGAMTestHelper gam(1, 1, sizeOutput, 1, str);

    T *output = NULL;

    gam.SetName("Test");
    ok &= gam.InitialiseChirp1();
    gam.config.MoveToRoot();
    if (ok) {
        ok &= gam.Initialise(gam.config);
    }
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
        output = static_cast<T *>(gam.GetOutputSignalsMemory());
    }
    for (uint32 i = 0u; (i < sizeOutput) && ok; i++) {
        output[i] = static_cast<T>(0.0);
    }
    for (uint32 i = 0u; (i < numberOfIteration) && ok; i++) {
        gam.Execute();
        ok &= gam.CompareChirp1(output, static_cast<float64>(*timeIteration) / 1e6,
                                static_cast<float64>(timeIterationIncrement) / gam.numberOfElementsOut / 1e6);
        if (!ok) {
            printf("iteration which fails %u\n", i);
        }
        *timeIteration += timeIterationIncrement;
    }
    return ok;
}

template<typename T>
bool WaveformChirpGAMTest::TestExecuteTrigger(StreamString str) {
    using namespace MARTe;
    bool ok = true;
    uint32 timeIterationIncrement = 1000000u;
    uint32 *timeIteration = NULL;
    uint32 numberOfIteration = 4000u; //if the number of iteration is too large the time will overflow
    uint32 sizeOutput = 4u;
    WaveformChirpGAMTestHelper gam(1, 1, sizeOutput, 1, str);

    T *output = NULL;

    gam.SetName("Test");
    ok &= gam.InitialiseChirp1Trigger();
    gam.config.MoveToRoot();
    if (ok) {
        ok &= gam.Initialise(gam.config);
    }
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
        output = static_cast<T *>(gam.GetOutputSignalsMemory());
    }
    for (uint32 i = 0u; (i < sizeOutput) && ok; i++) {
        output[i] = static_cast<T>(0.0);
    }
    for (uint32 i = 0u; (i < numberOfIteration) && ok; i++) {
        gam.Execute();
        ok &= gam.CompareChirp1Trigger(output, static_cast<float64>(*timeIteration) / 1e6,
                                       static_cast<float64>(timeIterationIncrement) / gam.numberOfElementsOut / 1e6);
        if (!ok) {
            printf("iteration which fails %u\n", i);
        }
        *timeIteration += timeIterationIncrement;
    }
    return ok;
}

template<typename T>
bool WaveformChirpGAMTest::TestExecute2Signals(StreamString str) {
    using namespace MARTe;
    bool ok = true;
    uint32 timeIterationIncrement = 1000000u;
    uint32 *timeIteration = NULL;
    uint32 numberOfIteration = 4000u; //if the number of iteration is too large the time will overflow
    uint32 sizeOutput = 4u;
    WaveformChirpGAMTestHelper gam(1, 1, sizeOutput, 1, str);

    T *output = NULL;
    uint32 *output2 = NULL;

    gam.SetName("Test");
    ok &= gam.InitialiseChirp1Trigger();
    gam.config.MoveToRoot();
    if (ok) {
        ok &= gam.Initialise(gam.config);
    }
    if (ok) {
        ok &= gam.InitialiseConfigDataBaseSignal2();
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
        output = static_cast<T *>(gam.GetOutputSignalsMemory());
        output2 = static_cast<uint32 *>(gam.GetOutputSignalsMemory(1));
    }
    for (uint32 i = 0u; (i < sizeOutput) && ok; i++) {
        output[i] = static_cast<T>(0.0);
        output2[i] = static_cast<uint32>(0.0);
    }
    for (uint32 i = 0u; (i < numberOfIteration) && ok; i++) {
        gam.Execute();
        ok &= gam.CompareChirp1Trigger(output, static_cast<float64>(*timeIteration) / 1e6,
                                static_cast<float64>(timeIterationIncrement) / gam.numberOfElementsOut / 1e6);
        ok &= gam.CompareChirpTrigger2(output2, static_cast<float64>(*timeIteration) / 1e6,
                                static_cast<float64>(timeIterationIncrement) / gam.numberOfElementsOut / 1e6);
        if (!ok) {
            printf("iteration which fails %u\n", i);
        }
        *timeIteration += timeIterationIncrement;
    }
    return ok;
}


template<typename T>
bool WaveformChirpGAMTest::TestExecute1ElementPerCycle(StreamString str) {
    using namespace MARTe;
    bool ok = true;
    uint32 timeIterationIncrement = 2000000u;
    uint32 *timeIteration = NULL;
    uint32 numberOfIteration = 2000u; //if the number of iteration is too large the time will overflow
    uint32 sizeOutput = 1u;
    WaveformChirpGAMTestHelper gam(1, 1, sizeOutput, 1, str);

    T *output = NULL;

    gam.SetName("Test");
    ok &= gam.InitialiseChirp1Trigger();
    gam.config.MoveToRoot();
    if (ok) {
        ok &= gam.Initialise(gam.config);
    }
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
        output = static_cast<T *>(gam.GetOutputSignalsMemory());
    }
    for (uint32 i = 0u; (i < sizeOutput) && ok; i++) {
        output[i] = static_cast<T>(0.0);
    }
    for (uint32 i = 0u; (i < numberOfIteration) && ok; i++) {
        gam.Execute();
        ok &= gam.CompareChirp1Trigger(output, static_cast<float64>(*timeIteration) / 1e6,
                                static_cast<float64>(timeIterationIncrement) / gam.numberOfElementsOut / 1e6);
        if (!ok) {
            printf("iteration which fails %u\n", i);
        }
        *timeIteration += timeIterationIncrement;
    }
    return ok;
}
}
