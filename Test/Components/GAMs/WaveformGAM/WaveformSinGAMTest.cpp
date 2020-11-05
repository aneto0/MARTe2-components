/**
 * @file WaveformSinGAMTest.cpp
 * @brief Source file for class WaveformSinGAMTest
 * @date 22/05/2017
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
 * the class WaveformSinGAMTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ConfigurationDatabase.h"
#include "WaveformSin.h"
#include "../WaveformGAM/WaveformSinGAMTest.h"
#include "Vector.h"
#include "FastMath.h"
#include "math.h"
#include "stdio.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {
class WaveformSinGAMTestHelper: public WaveformSin {
public:

    WaveformSinGAMTestHelper(uint32 elementsIn = 1,
                             uint32 samplesIn = 1,
                             uint32 elementsOut = 4,
                             uint32 samplesOut = 1) :
            WaveformSin() {
        numberOfElementsIn = elementsIn;
        numberOfSamplesIn = samplesIn;
        if (numberOfElementsIn > numberOfSamplesIn) {
            byteSizeIn = numberOfElementsIn * sizeof(uint32);
        }
        else {
            byteSizeIn = numberOfSamplesIn * sizeof(uint32);
        }
        // The input time could be int32 or uint32 both supported

        numberOfElementsOut = elementsOut;
        numberOfSamplesOut = samplesOut;

        byteSizeOut = 0;
        frequency = 0.0;
        phase = .00;
        amplitude = 0.0;
        offset = 0.0;
        isInitialised = false;
    }

    virtual ~WaveformSinGAMTestHelper() {

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
    bool InitialiseWaveSin(float64 A = 10.0,
                           float64 freq = 1.0,
                           float64 angle = 0.0,
                           float64 DC = 10.0) {
        bool ret = true;
        if (isInitialised == false) {
            amplitude = A;
            frequency = freq;
            phase = angle;
            offset = DC;
            ret &= config.Write("Amplitude", amplitude);
            ret &= config.Write("Frequency", frequency);
            ret &= config.Write("Phase", phase);
            ret &= config.Write("Offset", offset);
            isInitialised = ret;
        }
        else {
            ret = false;
        }
        return ret;
    }

    bool InitialiseWaveSinTrigger(float64 A = 10.0,
                                  float64 freq = 1.0,
                                  float64 angle = 0.0,
                                  float64 DC = 10.0) {
        bool ret = true;
        if (isInitialised == false) {
            amplitude = A;
            frequency = freq;
            phase = angle;
            offset = DC;
            ret &= config.Write("Amplitude", amplitude);
            ret &= config.Write("Frequency", frequency);
            ret &= config.Write("Phase", phase);
            ret &= config.Write("Offset", offset);
            uint32 dimTrigger = 5;
            float64 *startTrigger = new float64[dimTrigger];
            float64 *stopTrigger = new float64[dimTrigger];
            startTrigger[0] = 1.25;
            stopTrigger[0] = 1.750000;
            startTrigger[1] = 2.0;
            stopTrigger[1] = 3.25;
            startTrigger[2] = 3.5;
            stopTrigger[2] = 3.750;
            startTrigger[3] = 4.0;
            stopTrigger[3] = 4.25;
            startTrigger[4] = 4.55;
            stopTrigger[4] = 4.8;
            Vector<float64> startTVect(startTrigger, dimTrigger);
            Vector<float64> stopTVect(stopTrigger, dimTrigger);
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
    bool InitialiseConfigDataBaseSignal1(TypeDescriptor type = UnsignedInteger8Bit) {
        bool ok = true;
        ok &= configSignals.Write("QualifiedName", "WaveformSinGAMTest");
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

    uint32 numberOfElementsIn;
    uint32 numberOfSamplesIn;
    uint32 numberOfElementsOut;
    uint32 numberOfSamplesOut;
    uint32 byteSizeIn;
    uint32 byteSizeOut;
    ConfigurationDatabase configSignals;
    ConfigurationDatabase config;
    float64 amplitude;
    float64 phase;
    float64 frequency;
    float64 offset;
private:
    bool isInitialised;
};

}
/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

WaveformSinGAMTest::WaveformSinGAMTest() {

}

WaveformSinGAMTest::~WaveformSinGAMTest() {

}

bool WaveformSinGAMTest::TestInitialise_MissingAmplitude() {
    using namespace MARTe;
    using namespace FastMath;
    bool ok = true;
    WaveformSinGAMTestHelper gam;
    ok &= gam.Initialise(gam.config);

    return !ok;
}

bool WaveformSinGAMTest::TestInitialise_MissingFrequency() {
    using namespace MARTe;
    using namespace FastMath;
    bool ok = true;
    WaveformSinGAMTestHelper gam;
    ok &= gam.config.Write("Amplitude", 10.0);
    ok &= gam.Initialise(gam.config);
    return !ok;
}

bool WaveformSinGAMTest::TestInitialise_MissingPhase() {
    using namespace MARTe;
    using namespace FastMath;
    bool ok = true;
    WaveformSinGAMTestHelper gam;
    ok &= gam.config.Write("Amplitude", 10.0);
    ok &= gam.config.Write("Frequency", 1.0);
    ok &= gam.Initialise(gam.config);
    return !ok;
}

bool WaveformSinGAMTest::TestInitialise_MissingOffset() {
    using namespace MARTe;
    using namespace FastMath;
    bool ok = true;
    WaveformSinGAMTestHelper gam;
    ok &= gam.config.Write("Amplitude", 10.0);
    ok &= gam.config.Write("Frequency", 1.0);
    ok &= gam.config.Write("Phase", 0.0);
    ok &= gam.Initialise(gam.config);
    return !ok;
}

bool WaveformSinGAMTest::TestInitialise_Frequency0() {
    bool ok = true;
    using namespace MARTe;
    using namespace FastMath;
    WaveformSinGAMTestHelper gam;
    gam.SetName("Test");

    ok &= gam.InitialiseWaveSin(10.0, 0.0, 0.0, 10);

    gam.config.MoveToRoot();

    ok &= gam.Initialise(gam.config);
    return ok;
}

bool WaveformSinGAMTest::TestInitialise_Amplitude0() {
    bool ok = true;
    using namespace MARTe;
    using namespace FastMath;
    WaveformSinGAMTestHelper gam;
    gam.SetName("Test");

    ok &= gam.InitialiseWaveSin(0, 1, 0.0, 10);

    gam.config.MoveToRoot();

    ok &= gam.Initialise(gam.config);
    return !ok;
}

bool WaveformSinGAMTest::TestInitialise_MissingInputSignal() {
    bool ok = true;
    using namespace MARTe;
    using namespace FastMath;
    WaveformSinGAMTestHelper gam;
    gam.SetName("Test");

    ok &= gam.InitialiseWaveSin(10.0, 1.0, 0.0, 10);
    gam.config.MoveToRoot();
    ok &= gam.Initialise(gam.config);
    ok &= gam.Setup();
    return !ok;
}

bool WaveformSinGAMTest::TestInitialise_MissingOutputSignal() {
    bool ok = true;
    using namespace MARTe;
    using namespace FastMath;
    WaveformSinGAMTestHelper gam;
    gam.SetName("Test");
    ok &= gam.InitialiseWaveSin(10.0, 1.0, 0.0, 10);
    gam.config.MoveToRoot();
    ok &= gam.Initialise(gam.config);

    ok &= gam.configSignals.CreateAbsolute("Signals.InputSignals");
    ok &= gam.configSignals.CreateRelative("0");
    ok &= gam.configSignals.Write("QualifiedName", "InputSignal1");
    ok &= gam.configSignals.Write("DataSource", "TestDataSource");
    ok &= gam.configSignals.Write("Type", "uint32");
    ok &= gam.configSignals.Write("NumberOfDimensions", 1);
    ok &= gam.configSignals.Write("NumberOfElements", 1);
    ok &= gam.configSignals.Write("ByteSize", 1);
    ok &= gam.configSignals.MoveToAncestor(1u);
    ok &= gam.configSignals.Write("ByteSize", 1);
    ok &= gam.configSignals.MoveToRoot();

    ok &= gam.SetConfiguredDatabase(gam.configSignals);

    ok &= gam.Setup();
    return !ok;
}

bool WaveformSinGAMTest::TestInitialise_MissingInputNumberOfElements() {
    bool ok = true;
    using namespace MARTe;
    using namespace FastMath;
    WaveformSinGAMTestHelper gam;
    gam.SetName("Test");
    ok &= gam.InitialiseWaveSin(10.0, 1.0, 0.0, 10);
    gam.config.MoveToRoot();
    ok &= gam.Initialise(gam.config);

    ok &= gam.configSignals.CreateAbsolute("Signals.InputSignals");
    ok &= gam.configSignals.CreateRelative("0");
    ok &= gam.configSignals.Write("QualifiedName", "InputSignal1");
    ok &= gam.configSignals.Write("DataSource", "TestDataSource");
    ok &= gam.configSignals.Write("Type", "uint32");
    ok &= gam.configSignals.Write("NumberOfDimensions", 1);
    ok &= gam.configSignals.Write("ByteSize", 1);
    ok &= gam.configSignals.MoveToAncestor(1u);
    ok &= gam.configSignals.Write("ByteSize", 1);

    ok &= gam.configSignals.MoveToRoot();
    ok &= gam.configSignals.CreateAbsolute("Signals.OutputSignals");
    ok &= gam.configSignals.CreateRelative("0");
    ok &= gam.configSignals.Write("QualifiedName", "OutputSignal1");
    ok &= gam.configSignals.Write("DataSource", "TestDataSource");
    ok &= gam.configSignals.Write("Type", "uint8");
    ok &= gam.configSignals.Write("NumberOfDimensions", 1);

    ok &= gam.configSignals.Write("NumberOfElements", 4);
    ok &= gam.configSignals.Write("ByteSize", 4);

    ok &= gam.configSignals.MoveToAncestor(1u);
    ok &= gam.configSignals.Write("ByteSize", 4);

    ok &= gam.configSignals.MoveToRoot();

    ok &= gam.SetConfiguredDatabase(gam.configSignals);

    ok &= gam.Setup();
    return !ok;
}

bool WaveformSinGAMTest::TestInitialise_WrongInputNumberOfElements() {
    bool ok = true;
    using namespace MARTe;
    using namespace FastMath;
    WaveformSinGAMTestHelper gam;
    gam.SetName("Test");
    ok &= gam.InitialiseWaveSin(10.0, 1.0, 0.0, 10);
    gam.config.MoveToRoot();
    ok &= gam.Initialise(gam.config);

    ok &= gam.configSignals.CreateAbsolute("Signals.InputSignals");
    ok &= gam.configSignals.CreateRelative("0");
    ok &= gam.configSignals.Write("QualifiedName", "InputSignal1");
    ok &= gam.configSignals.Write("DataSource", "TestDataSource");
    ok &= gam.configSignals.Write("Type", "uint32");
    ok &= gam.configSignals.Write("NumberOfDimensions", 1);
    ok &= gam.configSignals.Write("NumberOfElements", 2);
    ok &= gam.configSignals.Write("ByteSize", 1);
    ok &= gam.configSignals.MoveToAncestor(1u);
    ok &= gam.configSignals.Write("ByteSize", 1);

    ok &= gam.configSignals.MoveToRoot();
    ok &= gam.configSignals.CreateAbsolute("Signals.OutputSignals");
    ok &= gam.configSignals.CreateRelative("0");
    ok &= gam.configSignals.Write("QualifiedName", "OutputSignal1");
    ok &= gam.configSignals.Write("DataSource", "TestDataSource");
    ok &= gam.configSignals.Write("Type", "uint8");
    ok &= gam.configSignals.Write("NumberOfDimensions", 1);

    ok &= gam.configSignals.Write("NumberOfElements", 4);
    ok &= gam.configSignals.Write("ByteSize", 4);

    ok &= gam.configSignals.MoveToAncestor(1u);
    ok &= gam.configSignals.Write("ByteSize", 4);
    ok &= gam.configSignals.MoveToRoot();

    ok &= gam.SetConfiguredDatabase(gam.configSignals);

    ok &= gam.Setup();
    return !ok;
}

bool WaveformSinGAMTest::TestInitialise_MissingOutputNumberOfElements() {
    bool ok = true;
    using namespace MARTe;
    using namespace FastMath;
    WaveformSinGAMTestHelper gam;
    gam.SetName("Test");
    ok &= gam.InitialiseWaveSin(10.0, 1.0, 0.0, 10);
    gam.config.MoveToRoot();
    ok &= gam.Initialise(gam.config);

    ok &= gam.configSignals.CreateAbsolute("Signals.InputSignals");
    ok &= gam.configSignals.CreateRelative("0");
    ok &= gam.configSignals.Write("QualifiedName", "InputSignal1");
    ok &= gam.configSignals.Write("DataSource", "TestDataSource");
    ok &= gam.configSignals.Write("Type", "uint32");
    ok &= gam.configSignals.Write("NumberOfDimensions", 1);
    ok &= gam.configSignals.Write("NumberOfElements", 1);
    ok &= gam.configSignals.Write("ByteSize", 1);
    ok &= gam.configSignals.MoveToAncestor(1u);
    ok &= gam.configSignals.Write("ByteSize", 1);

    ok &= gam.configSignals.MoveToRoot();
    ok &= gam.configSignals.CreateAbsolute("Signals.OutputSignals");
    ok &= gam.configSignals.CreateRelative("0");
    ok &= gam.configSignals.Write("QualifiedName", "OutputSignal1");
    ok &= gam.configSignals.Write("DataSource", "TestDataSource");
    ok &= gam.configSignals.Write("Type", "uint8");
    ok &= gam.configSignals.Write("NumberOfDimensions", 1);

    //ok &= gam.configSignals.Write("NumberOfElements", 4);
    ok &= gam.configSignals.Write("ByteSize", 4);

    ok &= gam.configSignals.MoveToAncestor(1u);
    ok &= gam.configSignals.Write("ByteSize", 4);
    ok &= gam.configSignals.MoveToRoot();

    ok &= gam.SetConfiguredDatabase(gam.configSignals);

    ok &= gam.Setup();
    return !ok;
}

bool WaveformSinGAMTest::TestInitialise_WrongOutputNumberOfElements() {
    bool ok = true;
    using namespace MARTe;
    using namespace FastMath;
    WaveformSinGAMTestHelper gam;
    gam.SetName("Test");
    ok &= gam.InitialiseWaveSin(10.0, 1.0, 0.0, 10);
    gam.config.MoveToRoot();
    ok &= gam.Initialise(gam.config);

    ok &= gam.configSignals.CreateAbsolute("Signals.InputSignals");
    ok &= gam.configSignals.CreateRelative("0");
    ok &= gam.configSignals.Write("QualifiedName", "InputSignal1");
    ok &= gam.configSignals.Write("DataSource", "TestDataSource");
    ok &= gam.configSignals.Write("Type", "uint32");
    ok &= gam.configSignals.Write("NumberOfDimensions", 1);
    ok &= gam.configSignals.Write("NumberOfElements", 1);
    ok &= gam.configSignals.Write("ByteSize", 1);
    ok &= gam.configSignals.MoveToAncestor(1u);
    ok &= gam.configSignals.Write("ByteSize", 1);

    ok &= gam.configSignals.MoveToRoot();
    ok &= gam.configSignals.CreateAbsolute("Signals.OutputSignals");
    ok &= gam.configSignals.CreateRelative("0");
    ok &= gam.configSignals.Write("QualifiedName", "OutputSignal1");
    ok &= gam.configSignals.Write("DataSource", "TestDataSource");
    ok &= gam.configSignals.Write("Type", "uint8");
    ok &= gam.configSignals.Write("NumberOfDimensions", 1);

    ok &= gam.configSignals.Write("NumberOfElements", 0);
    ok &= gam.configSignals.Write("ByteSize", 4);

    ok &= gam.configSignals.MoveToAncestor(1u);
    ok &= gam.configSignals.Write("ByteSize", 4);
    ok &= gam.configSignals.MoveToRoot();

    ok &= gam.SetConfiguredDatabase(gam.configSignals);

    ok &= gam.Setup();
    return !ok;
}

bool WaveformSinGAMTest::TestInitialise_MissingSecondOutputNumberOfElements() {
    bool ok = true;
    using namespace MARTe;
    using namespace FastMath;
    WaveformSinGAMTestHelper gam;
    gam.SetName("Test");
    ok &= gam.InitialiseWaveSin(10.0, 1.0, 0.0, 10);
    gam.config.MoveToRoot();
    ok &= gam.Initialise(gam.config);

    ok &= gam.configSignals.CreateAbsolute("Signals.InputSignals");
    ok &= gam.configSignals.CreateRelative("0");
    ok &= gam.configSignals.Write("QualifiedName", "InputSignal1");
    ok &= gam.configSignals.Write("DataSource", "TestDataSource");
    ok &= gam.configSignals.Write("Type", "uint32");
    ok &= gam.configSignals.Write("NumberOfDimensions", 1);
    ok &= gam.configSignals.Write("NumberOfElements", 1);
    ok &= gam.configSignals.Write("ByteSize", 1);
    ok &= gam.configSignals.MoveToAncestor(1u);
    ok &= gam.configSignals.Write("ByteSize", 1);

    ok &= gam.configSignals.MoveToRoot();
    ok &= gam.configSignals.CreateAbsolute("Signals.OutputSignals");
    ok &= gam.configSignals.CreateRelative("0");
    ok &= gam.configSignals.Write("QualifiedName", "OutputSignal1");
    ok &= gam.configSignals.Write("DataSource", "TestDataSource");
    ok &= gam.configSignals.Write("Type", "uint8");
    ok &= gam.configSignals.Write("NumberOfDimensions", 1);
    ok &= gam.configSignals.Write("NumberOfElements", 4);
    ok &= gam.configSignals.Write("ByteSize", 4);

    ok &= gam.configSignals.MoveAbsolute("Signals.OutputSignals");
    ok &= gam.configSignals.CreateRelative("1");
    ok &= gam.configSignals.Write("QualifiedName", "OutputSignal2");
    ok &= gam.configSignals.Write("DataSource", "TestDataSource");
    ok &= gam.configSignals.Write("Type", "uint8");
    ok &= gam.configSignals.Write("NumberOfDimensions", 1);
//    ok &= gam.configSignals.Write("NumberOfElements", 4);
    ok &= gam.configSignals.Write("ByteSize", 4);
    ok &= gam.configSignals.MoveToAncestor(1u);
    ok &= gam.configSignals.Write("ByteSize", 4 * 2);

    ok &= gam.configSignals.MoveToRoot();

    ok &= gam.SetConfiguredDatabase(gam.configSignals);

    ok &= gam.Setup();
    return !ok;
}

bool WaveformSinGAMTest::TestInitialise_WrongSecondOutputNumberOfElements() {
    bool ok = true;
    using namespace MARTe;
    using namespace FastMath;
    WaveformSinGAMTestHelper gam;
    gam.SetName("Test");
    ok &= gam.InitialiseWaveSin(10.0, 1.0, 0.0, 10);
    gam.config.MoveToRoot();
    ok &= gam.Initialise(gam.config);

    ok &= gam.configSignals.CreateAbsolute("Signals.InputSignals");
    ok &= gam.configSignals.CreateRelative("0");
    ok &= gam.configSignals.Write("QualifiedName", "InputSignal1");
    ok &= gam.configSignals.Write("DataSource", "TestDataSource");
    ok &= gam.configSignals.Write("Type", "uint32");
    ok &= gam.configSignals.Write("NumberOfDimensions", 1);
    ok &= gam.configSignals.Write("NumberOfElements", 1);
    ok &= gam.configSignals.Write("ByteSize", 1);
    ok &= gam.configSignals.MoveToAncestor(1u);
    ok &= gam.configSignals.Write("ByteSize", 1);

    ok &= gam.configSignals.MoveToRoot();
    ok &= gam.configSignals.CreateAbsolute("Signals.OutputSignals");
    ok &= gam.configSignals.CreateRelative("0");
    ok &= gam.configSignals.Write("QualifiedName", "OutputSignal1");
    ok &= gam.configSignals.Write("DataSource", "TestDataSource");
    ok &= gam.configSignals.Write("Type", "uint8");
    ok &= gam.configSignals.Write("NumberOfDimensions", 1);
    ok &= gam.configSignals.Write("NumberOfElements", 4);
    ok &= gam.configSignals.Write("ByteSize", 4);

    ok &= gam.configSignals.MoveAbsolute("Signals.OutputSignals");
    ok &= gam.configSignals.CreateRelative("1");
    ok &= gam.configSignals.Write("QualifiedName", "OutputSignal2");
    ok &= gam.configSignals.Write("DataSource", "TestDataSource");
    ok &= gam.configSignals.Write("Type", "uint8");
    ok &= gam.configSignals.Write("NumberOfDimensions", 1);
    ok &= gam.configSignals.Write("NumberOfElements", 3);
    ok &= gam.configSignals.Write("ByteSize", 4);
    ok &= gam.configSignals.MoveToAncestor(1u);
    ok &= gam.configSignals.Write("ByteSize", 4 * 2);

    ok &= gam.configSignals.MoveToRoot();

    ok &= gam.SetConfiguredDatabase(gam.configSignals);

    ok &= gam.Setup();
    return !ok;
}

bool WaveformSinGAMTest::TestInitialise_MissingInputNumberOfSamples() {
    bool ok = true;
    using namespace MARTe;
    using namespace FastMath;
    WaveformSinGAMTestHelper gam;
    gam.SetName("Test");
    ok &= gam.InitialiseWaveSin(10.0, 1.0, 0.0, 10);
    gam.config.MoveToRoot();
    ok &= gam.Initialise(gam.config);

    ok &= gam.configSignals.CreateAbsolute("Signals.InputSignals");
    ok &= gam.configSignals.CreateRelative("0");
    ok &= gam.configSignals.Write("QualifiedName", "InputSignal1");
    ok &= gam.configSignals.Write("DataSource", "TestDataSource");
    ok &= gam.configSignals.Write("Type", "uint32");
    ok &= gam.configSignals.Write("NumberOfDimensions", 1);
    ok &= gam.configSignals.Write("NumberOfElements", 1);
    ok &= gam.configSignals.Write("ByteSize", 1);
    ok &= gam.configSignals.MoveToAncestor(1u);
    ok &= gam.configSignals.Write("ByteSize", 1);

    ok &= gam.configSignals.MoveToRoot();
    ok &= gam.configSignals.CreateAbsolute("Signals.OutputSignals");
    ok &= gam.configSignals.CreateRelative("0");
    ok &= gam.configSignals.Write("QualifiedName", "OutputSignal1");
    ok &= gam.configSignals.Write("DataSource", "TestDataSource");
    ok &= gam.configSignals.Write("Type", "uint8");
    ok &= gam.configSignals.Write("NumberOfDimensions", 1);

    ok &= gam.configSignals.Write("NumberOfElements", 4);
    ok &= gam.configSignals.Write("ByteSize", 4);

    ok &= gam.configSignals.MoveToAncestor(1u);
    ok &= gam.configSignals.Write("ByteSize", 4);
    ok &= gam.configSignals.MoveToRoot();

    ok &= gam.configSignals.CreateAbsolute("Memory.InputSignals");
    ok &= gam.configSignals.CreateRelative("0");
    ok &= gam.configSignals.Write("DataSource", "TestDataSource");
    ok &= gam.configSignals.CreateRelative("Signals");
    ok &= gam.configSignals.CreateRelative("0");
    //ok &= gam.configSignals.Write("Samples", 1);

    ok &= gam.configSignals.CreateAbsolute("Memory.OutputSignals");
    ok &= gam.configSignals.CreateRelative("0");
    ok &= gam.configSignals.Write("DataSource", "TestDataSource");
    ok &= gam.configSignals.CreateRelative("Signals");
    ok &= gam.configSignals.CreateRelative("0");
    ok &= gam.configSignals.Write("Samples", 1);
    ok &= gam.configSignals.MoveToRoot();

    ok &= gam.SetConfiguredDatabase(gam.configSignals);

    ok &= gam.Setup();
    return !ok;
}

bool WaveformSinGAMTest::TestInitialise_WrongInputNumberOfSamples() {
    bool ok = true;
    using namespace MARTe;
    using namespace FastMath;
    WaveformSinGAMTestHelper gam;
    gam.SetName("Test");
    ok &= gam.InitialiseWaveSin(10.0, 1.0, 0.0, 10);
    gam.config.MoveToRoot();
    ok &= gam.Initialise(gam.config);

    ok &= gam.configSignals.CreateAbsolute("Signals.InputSignals");
    ok &= gam.configSignals.CreateRelative("0");
    ok &= gam.configSignals.Write("QualifiedName", "InputSignal1");
    ok &= gam.configSignals.Write("DataSource", "TestDataSource");
    ok &= gam.configSignals.Write("Type", "uint32");
    ok &= gam.configSignals.Write("NumberOfDimensions", 1);
    ok &= gam.configSignals.Write("NumberOfElements", 1);
    ok &= gam.configSignals.Write("ByteSize", 1);
    ok &= gam.configSignals.MoveToAncestor(1u);
    ok &= gam.configSignals.Write("ByteSize", 1);

    ok &= gam.configSignals.MoveToRoot();
    ok &= gam.configSignals.CreateAbsolute("Signals.OutputSignals");
    ok &= gam.configSignals.CreateRelative("0");
    ok &= gam.configSignals.Write("QualifiedName", "OutputSignal1");
    ok &= gam.configSignals.Write("DataSource", "TestDataSource");
    ok &= gam.configSignals.Write("Type", "uint8");
    ok &= gam.configSignals.Write("NumberOfDimensions", 1);

    ok &= gam.configSignals.Write("NumberOfElements", 4);
    ok &= gam.configSignals.Write("ByteSize", 4);

    ok &= gam.configSignals.MoveToAncestor(1u);
    ok &= gam.configSignals.Write("ByteSize", 4);
    ok &= gam.configSignals.MoveToRoot();

    ok &= gam.configSignals.CreateAbsolute("Memory.InputSignals");
    ok &= gam.configSignals.CreateRelative("0");
    ok &= gam.configSignals.Write("DataSource", "TestDataSource");
    ok &= gam.configSignals.CreateRelative("Signals");
    ok &= gam.configSignals.CreateRelative("0");
    ok &= gam.configSignals.Write("Samples", 2);

    ok &= gam.configSignals.CreateAbsolute("Memory.OutputSignals");
    ok &= gam.configSignals.CreateRelative("0");
    ok &= gam.configSignals.Write("DataSource", "TestDataSource");
    ok &= gam.configSignals.CreateRelative("Signals");
    ok &= gam.configSignals.CreateRelative("0");
    ok &= gam.configSignals.Write("Samples", 1);
    ok &= gam.configSignals.MoveToRoot();

    ok &= gam.SetConfiguredDatabase(gam.configSignals);

    ok &= gam.Setup();
    return !ok;
}

bool WaveformSinGAMTest::TestInitialise_MissingOutputNumberOfSamples() {
    bool ok = true;
    using namespace MARTe;
    using namespace FastMath;
    WaveformSinGAMTestHelper gam;
    gam.SetName("Test");
    ok &= gam.InitialiseWaveSin(10.0, 1.0, 0.0, 10);
    gam.config.MoveToRoot();
    ok &= gam.Initialise(gam.config);

    ok &= gam.configSignals.CreateAbsolute("Signals.InputSignals");
    ok &= gam.configSignals.CreateRelative("0");
    ok &= gam.configSignals.Write("QualifiedName", "InputSignal1");
    ok &= gam.configSignals.Write("DataSource", "TestDataSource");
    ok &= gam.configSignals.Write("Type", "uint32");
    ok &= gam.configSignals.Write("NumberOfDimensions", 1);
    ok &= gam.configSignals.Write("NumberOfElements", 1);
    ok &= gam.configSignals.Write("ByteSize", 1);
    ok &= gam.configSignals.MoveToAncestor(1u);
    ok &= gam.configSignals.Write("ByteSize", 1);

    ok &= gam.configSignals.MoveToRoot();
    ok &= gam.configSignals.CreateAbsolute("Signals.OutputSignals");
    ok &= gam.configSignals.CreateRelative("0");
    ok &= gam.configSignals.Write("QualifiedName", "OutputSignal1");
    ok &= gam.configSignals.Write("DataSource", "TestDataSource");
    ok &= gam.configSignals.Write("Type", "uint8");
    ok &= gam.configSignals.Write("NumberOfDimensions", 1);

    ok &= gam.configSignals.Write("NumberOfElements", 4);
    ok &= gam.configSignals.Write("ByteSize", 4);

    ok &= gam.configSignals.MoveToAncestor(1u);
    ok &= gam.configSignals.Write("ByteSize", 4);
    ok &= gam.configSignals.MoveToRoot();

    ok &= gam.configSignals.CreateAbsolute("Memory.InputSignals");
    ok &= gam.configSignals.CreateRelative("0");
    ok &= gam.configSignals.Write("DataSource", "TestDataSource");
    ok &= gam.configSignals.CreateRelative("Signals");
    ok &= gam.configSignals.CreateRelative("0");
    ok &= gam.configSignals.Write("Samples", 1);

    ok &= gam.configSignals.CreateAbsolute("Memory.OutputSignals");
    ok &= gam.configSignals.CreateRelative("0");
    ok &= gam.configSignals.Write("DataSource", "TestDataSource");
    ok &= gam.configSignals.CreateRelative("Signals");
    ok &= gam.configSignals.CreateRelative("0");
    //ok &= gam.configSignals.Write("Samples", 1);
    ok &= gam.configSignals.MoveToRoot();

    ok &= gam.SetConfiguredDatabase(gam.configSignals);

    ok &= gam.Setup();
    return !ok;
}

bool WaveformSinGAMTest::TestInitialise_WrongOutputNumberOfSamples() {
    bool ok = true;
    using namespace MARTe;
    using namespace FastMath;
    WaveformSinGAMTestHelper gam;
    gam.SetName("Test");
    ok &= gam.InitialiseWaveSin(10.0, 1.0, 0.0, 10);
    gam.config.MoveToRoot();
    ok &= gam.Initialise(gam.config);

    ok &= gam.configSignals.CreateAbsolute("Signals.InputSignals");
    ok &= gam.configSignals.CreateRelative("0");
    ok &= gam.configSignals.Write("QualifiedName", "InputSignal1");
    ok &= gam.configSignals.Write("DataSource", "TestDataSource");
    ok &= gam.configSignals.Write("Type", "uint32");
    ok &= gam.configSignals.Write("NumberOfDimensions", 1);
    ok &= gam.configSignals.Write("NumberOfElements", 1);
    ok &= gam.configSignals.Write("ByteSize", 1);
    ok &= gam.configSignals.MoveToAncestor(1u);
    ok &= gam.configSignals.Write("ByteSize", 1);

    ok &= gam.configSignals.MoveToRoot();
    ok &= gam.configSignals.CreateAbsolute("Signals.OutputSignals");
    ok &= gam.configSignals.CreateRelative("0");
    ok &= gam.configSignals.Write("QualifiedName", "OutputSignal1");
    ok &= gam.configSignals.Write("DataSource", "TestDataSource");
    ok &= gam.configSignals.Write("Type", "uint8");
    ok &= gam.configSignals.Write("NumberOfDimensions", 1);

    ok &= gam.configSignals.Write("NumberOfElements", 4);
    ok &= gam.configSignals.Write("ByteSize", 4);

    ok &= gam.configSignals.MoveToAncestor(1u);
    ok &= gam.configSignals.Write("ByteSize", 4);
    ok &= gam.configSignals.MoveToRoot();

    ok &= gam.configSignals.CreateAbsolute("Memory.InputSignals");
    ok &= gam.configSignals.CreateRelative("0");
    ok &= gam.configSignals.Write("DataSource", "TestDataSource");
    ok &= gam.configSignals.CreateRelative("Signals");
    ok &= gam.configSignals.CreateRelative("0");
    ok &= gam.configSignals.Write("Samples", 1);

    ok &= gam.configSignals.CreateAbsolute("Memory.OutputSignals");
    ok &= gam.configSignals.CreateRelative("0");
    ok &= gam.configSignals.Write("DataSource", "TestDataSource");
    ok &= gam.configSignals.CreateRelative("Signals");
    ok &= gam.configSignals.CreateRelative("0");
    ok &= gam.configSignals.Write("Samples", 2);
    ok &= gam.configSignals.MoveToRoot();

    ok &= gam.SetConfiguredDatabase(gam.configSignals);

    ok &= gam.Setup();
    return !ok;
}

bool WaveformSinGAMTest::TestInitialise_MissingSeondOutputNumberOfSamples() {
    bool ok = true;
    using namespace MARTe;
    using namespace FastMath;
    WaveformSinGAMTestHelper gam;
    gam.SetName("Test");
    ok &= gam.InitialiseWaveSin(10.0, 1.0, 0.0, 10);
    gam.config.MoveToRoot();
    ok &= gam.Initialise(gam.config);

    ok &= gam.configSignals.CreateAbsolute("Signals.InputSignals");
    ok &= gam.configSignals.CreateRelative("0");
    ok &= gam.configSignals.Write("QualifiedName", "InputSignal1");
    ok &= gam.configSignals.Write("DataSource", "TestDataSource");
    ok &= gam.configSignals.Write("Type", "uint32");
    ok &= gam.configSignals.Write("NumberOfDimensions", 1);
    ok &= gam.configSignals.Write("NumberOfElements", 1);
    ok &= gam.configSignals.Write("ByteSize", 1);
    ok &= gam.configSignals.MoveToAncestor(1u);
    ok &= gam.configSignals.Write("ByteSize", 1);

    ok &= gam.configSignals.MoveToRoot();
    ok &= gam.configSignals.CreateAbsolute("Signals.OutputSignals");
    ok &= gam.configSignals.CreateRelative("0");
    ok &= gam.configSignals.Write("QualifiedName", "OutputSignal1");
    ok &= gam.configSignals.Write("DataSource", "TestDataSource");
    ok &= gam.configSignals.Write("Type", "uint8");
    ok &= gam.configSignals.Write("NumberOfDimensions", 1);

    ok &= gam.configSignals.Write("NumberOfElements", 4);
    ok &= gam.configSignals.Write("ByteSize", 4);

    ok &= gam.configSignals.MoveAbsolute("Signals.OutputSignals");
    ok &= gam.configSignals.CreateRelative("1");
    ok &= gam.configSignals.Write("QualifiedName", "OutputSignal2");
    ok &= gam.configSignals.Write("DataSource", "TestDataSource");
    ok &= gam.configSignals.Write("Type", "uint8");
    ok &= gam.configSignals.Write("NumberOfDimensions", 1);
    ok &= gam.configSignals.Write("NumberOfElements", 4);
    ok &= gam.configSignals.Write("ByteSize", 4);
    ok &= gam.configSignals.MoveToAncestor(1u);
    ok &= gam.configSignals.Write("ByteSize", 4 * 2);

    ok &= gam.configSignals.MoveToRoot();

    ok &= gam.configSignals.CreateAbsolute("Memory.InputSignals");
    ok &= gam.configSignals.CreateRelative("0");
    ok &= gam.configSignals.Write("DataSource", "TestDataSource");
    ok &= gam.configSignals.CreateRelative("Signals");
    ok &= gam.configSignals.CreateRelative("0");
    ok &= gam.configSignals.Write("Samples", 1);

    ok &= gam.configSignals.CreateAbsolute("Memory.OutputSignals");
    ok &= gam.configSignals.CreateRelative("0");
    ok &= gam.configSignals.Write("DataSource", "TestDataSource");
    ok &= gam.configSignals.CreateRelative("Signals");
    ok &= gam.configSignals.CreateRelative("0");
    ok &= gam.configSignals.Write("Samples", 1);
    ok &= gam.configSignals.MoveToAncestor(1u);
    ok &= gam.configSignals.CreateRelative("1");
    //ok &= gam.configSignals.Write("Samples", 1);
    ok &= gam.configSignals.MoveToRoot();

    ok &= gam.SetConfiguredDatabase(gam.configSignals);

    ok &= gam.Setup();
    return !ok;
}

bool WaveformSinGAMTest::TestInitialise_WrongSecondOutputNumberOfSamples() {
    bool ok = true;
    using namespace MARTe;
    using namespace FastMath;
    WaveformSinGAMTestHelper gam;
    gam.SetName("Test");
    ok &= gam.InitialiseWaveSin(10.0, 1.0, 0.0, 10);
    gam.config.MoveToRoot();
    ok &= gam.Initialise(gam.config);

    ok &= gam.configSignals.CreateAbsolute("Signals.InputSignals");
    ok &= gam.configSignals.CreateRelative("0");
    ok &= gam.configSignals.Write("QualifiedName", "InputSignal1");
    ok &= gam.configSignals.Write("DataSource", "TestDataSource");
    ok &= gam.configSignals.Write("Type", "uint32");
    ok &= gam.configSignals.Write("NumberOfDimensions", 1);
    ok &= gam.configSignals.Write("NumberOfElements", 1);
    ok &= gam.configSignals.Write("ByteSize", 1);
    ok &= gam.configSignals.MoveToAncestor(1u);
    ok &= gam.configSignals.Write("ByteSize", 1);

    ok &= gam.configSignals.MoveToRoot();
    ok &= gam.configSignals.CreateAbsolute("Signals.OutputSignals");
    ok &= gam.configSignals.CreateRelative("0");
    ok &= gam.configSignals.Write("QualifiedName", "OutputSignal1");
    ok &= gam.configSignals.Write("DataSource", "TestDataSource");
    ok &= gam.configSignals.Write("Type", "uint8");
    ok &= gam.configSignals.Write("NumberOfDimensions", 1);

    ok &= gam.configSignals.Write("NumberOfElements", 4);
    ok &= gam.configSignals.Write("ByteSize", 4);

    ok &= gam.configSignals.MoveAbsolute("Signals.OutputSignals");
    ok &= gam.configSignals.CreateRelative("1");
    ok &= gam.configSignals.Write("QualifiedName", "OutputSignal2");
    ok &= gam.configSignals.Write("DataSource", "TestDataSource");
    ok &= gam.configSignals.Write("Type", "uint8");
    ok &= gam.configSignals.Write("NumberOfDimensions", 1);
    ok &= gam.configSignals.Write("NumberOfElements", 4);
    ok &= gam.configSignals.Write("ByteSize", 4);
    ok &= gam.configSignals.MoveToAncestor(1u);
    ok &= gam.configSignals.Write("ByteSize", 4 * 2);

    ok &= gam.configSignals.MoveToRoot();

    ok &= gam.configSignals.CreateAbsolute("Memory.InputSignals");
    ok &= gam.configSignals.CreateRelative("0");
    ok &= gam.configSignals.Write("DataSource", "TestDataSource");
    ok &= gam.configSignals.CreateRelative("Signals");
    ok &= gam.configSignals.CreateRelative("0");
    ok &= gam.configSignals.Write("Samples", 1);

    ok &= gam.configSignals.CreateAbsolute("Memory.OutputSignals");
    ok &= gam.configSignals.CreateRelative("0");
    ok &= gam.configSignals.Write("DataSource", "TestDataSource");
    ok &= gam.configSignals.CreateRelative("Signals");
    ok &= gam.configSignals.CreateRelative("0");
    ok &= gam.configSignals.Write("Samples", 1);
    ok &= gam.configSignals.MoveToAncestor(1u);
    ok &= gam.configSignals.CreateRelative("1");
    ok &= gam.configSignals.Write("Samples", 2);
    ok &= gam.configSignals.MoveToRoot();

    ok &= gam.SetConfiguredDatabase(gam.configSignals);

    ok &= gam.Setup();
    return !ok;
}

bool WaveformSinGAMTest::TestInitialise_MissingInputType() {
    bool ok = true;
    using namespace MARTe;
    using namespace FastMath;
    WaveformSinGAMTestHelper gam;
    gam.SetName("Test");
    ok &= gam.InitialiseWaveSin(10.0, 1.0, 0.0, 10);
    gam.config.MoveToRoot();
    ok &= gam.Initialise(gam.config);

    ok &= gam.configSignals.CreateAbsolute("Signals.InputSignals");
    ok &= gam.configSignals.CreateRelative("0");
    ok &= gam.configSignals.Write("QualifiedName", "InputSignal1");
    ok &= gam.configSignals.Write("DataSource", "TestDataSource");
    //ok &= gam.configSignals.Write("Type", "uint32");
    ok &= gam.configSignals.Write("NumberOfDimensions", 1);
    ok &= gam.configSignals.Write("NumberOfElements", 1);
    ok &= gam.configSignals.Write("ByteSize", 1);
    ok &= gam.configSignals.MoveToAncestor(1u);
    ok &= gam.configSignals.Write("ByteSize", 1);

    ok &= gam.configSignals.MoveToRoot();
    ok &= gam.configSignals.CreateAbsolute("Signals.OutputSignals");
    ok &= gam.configSignals.CreateRelative("0");
    ok &= gam.configSignals.Write("QualifiedName", "OutputSignal1");
    ok &= gam.configSignals.Write("DataSource", "TestDataSource");
    ok &= gam.configSignals.Write("Type", "uint8");
    ok &= gam.configSignals.Write("NumberOfDimensions", 1);

    ok &= gam.configSignals.Write("NumberOfElements", 4);
    ok &= gam.configSignals.Write("ByteSize", 4);

    ok &= gam.configSignals.MoveAbsolute("Signals.OutputSignals");
    ok &= gam.configSignals.CreateRelative("1");
    ok &= gam.configSignals.Write("QualifiedName", "OutputSignal2");
    ok &= gam.configSignals.Write("DataSource", "TestDataSource");
    ok &= gam.configSignals.Write("Type", "uint8");
    ok &= gam.configSignals.Write("NumberOfDimensions", 1);
    ok &= gam.configSignals.Write("NumberOfElements", 4);
    ok &= gam.configSignals.Write("ByteSize", 4);
    ok &= gam.configSignals.MoveToAncestor(1u);
    ok &= gam.configSignals.Write("ByteSize", 4 * 2);

    ok &= gam.configSignals.MoveToRoot();

    ok &= gam.configSignals.CreateAbsolute("Memory.InputSignals");
    ok &= gam.configSignals.CreateRelative("0");
    ok &= gam.configSignals.Write("DataSource", "TestDataSource");
    ok &= gam.configSignals.CreateRelative("Signals");
    ok &= gam.configSignals.CreateRelative("0");
    ok &= gam.configSignals.Write("Samples", 1);

    ok &= gam.configSignals.CreateAbsolute("Memory.OutputSignals");
    ok &= gam.configSignals.CreateRelative("0");
    ok &= gam.configSignals.Write("DataSource", "TestDataSource");
    ok &= gam.configSignals.CreateRelative("Signals");
    ok &= gam.configSignals.CreateRelative("0");
    ok &= gam.configSignals.Write("Samples", 1);
    ok &= gam.configSignals.MoveToAncestor(1u);
    ok &= gam.configSignals.CreateRelative("1");
    ok &= gam.configSignals.Write("Samples", 1);
    ok &= gam.configSignals.MoveToRoot();

    ok &= gam.SetConfiguredDatabase(gam.configSignals);

    ok &= gam.Setup();
    return !ok;
}

bool WaveformSinGAMTest::TestInitialise_MissingOutputType() {
    bool ok = true;
    using namespace MARTe;
    using namespace FastMath;
    WaveformSinGAMTestHelper gam;
    gam.SetName("Test");
    ok &= gam.InitialiseWaveSin(10.0, 1.0, 0.0, 10);
    gam.config.MoveToRoot();
    ok &= gam.Initialise(gam.config);

    ok &= gam.configSignals.CreateAbsolute("Signals.InputSignals");
    ok &= gam.configSignals.CreateRelative("0");
    ok &= gam.configSignals.Write("QualifiedName", "InputSignal1");
    ok &= gam.configSignals.Write("DataSource", "TestDataSource");
    ok &= gam.configSignals.Write("Type", "uint32");
    ok &= gam.configSignals.Write("NumberOfDimensions", 1);
    ok &= gam.configSignals.Write("NumberOfElements", 1);
    ok &= gam.configSignals.Write("ByteSize", 1);
    ok &= gam.configSignals.MoveToAncestor(1u);
    ok &= gam.configSignals.Write("ByteSize", 1);

    ok &= gam.configSignals.MoveToRoot();
    ok &= gam.configSignals.CreateAbsolute("Signals.OutputSignals");
    ok &= gam.configSignals.CreateRelative("0");
    ok &= gam.configSignals.Write("QualifiedName", "OutputSignal1");
    ok &= gam.configSignals.Write("DataSource", "TestDataSource");
    //ok &= gam.configSignals.Write("Type", "uint8");
    ok &= gam.configSignals.Write("NumberOfDimensions", 1);

    ok &= gam.configSignals.Write("NumberOfElements", 4);
    ok &= gam.configSignals.Write("ByteSize", 4);

    ok &= gam.configSignals.MoveAbsolute("Signals.OutputSignals");
    ok &= gam.configSignals.CreateRelative("1");
    ok &= gam.configSignals.Write("QualifiedName", "OutputSignal2");
    ok &= gam.configSignals.Write("DataSource", "TestDataSource");
    ok &= gam.configSignals.Write("Type", "uint8");
    ok &= gam.configSignals.Write("NumberOfDimensions", 1);
    ok &= gam.configSignals.Write("NumberOfElements", 4);
    ok &= gam.configSignals.Write("ByteSize", 4);
    ok &= gam.configSignals.MoveToAncestor(1u);
    ok &= gam.configSignals.Write("ByteSize", 4 * 2);

    ok &= gam.configSignals.MoveToRoot();

    ok &= gam.configSignals.CreateAbsolute("Memory.InputSignals");
    ok &= gam.configSignals.CreateRelative("0");
    ok &= gam.configSignals.Write("DataSource", "TestDataSource");
    ok &= gam.configSignals.CreateRelative("Signals");
    ok &= gam.configSignals.CreateRelative("0");
    ok &= gam.configSignals.Write("Samples", 1);

    ok &= gam.configSignals.CreateAbsolute("Memory.OutputSignals");
    ok &= gam.configSignals.CreateRelative("0");
    ok &= gam.configSignals.Write("DataSource", "TestDataSource");
    ok &= gam.configSignals.CreateRelative("Signals");
    ok &= gam.configSignals.CreateRelative("0");
    ok &= gam.configSignals.Write("Samples", 1);
    ok &= gam.configSignals.MoveToAncestor(1u);
    ok &= gam.configSignals.CreateRelative("1");
    ok &= gam.configSignals.Write("Samples", 1);
    ok &= gam.configSignals.MoveToRoot();

    ok &= gam.SetConfiguredDatabase(gam.configSignals);

    ok &= gam.Setup();
    return !ok;
}

bool WaveformSinGAMTest::TestInitialise_MissingSecondOutputType() {
    bool ok = true;
    using namespace MARTe;
    using namespace FastMath;
    WaveformSinGAMTestHelper gam;
    gam.SetName("Test");
    ok &= gam.InitialiseWaveSin(10.0, 1.0, 0.0, 10);
    gam.config.MoveToRoot();
    ok &= gam.Initialise(gam.config);

    ok &= gam.configSignals.CreateAbsolute("Signals.InputSignals");
    ok &= gam.configSignals.CreateRelative("0");
    ok &= gam.configSignals.Write("QualifiedName", "InputSignal1");
    ok &= gam.configSignals.Write("DataSource", "TestDataSource");
    ok &= gam.configSignals.Write("Type", "uint32");
    ok &= gam.configSignals.Write("NumberOfDimensions", 1);
    ok &= gam.configSignals.Write("NumberOfElements", 1);
    ok &= gam.configSignals.Write("ByteSize", 1);
    ok &= gam.configSignals.MoveToAncestor(1u);
    ok &= gam.configSignals.Write("ByteSize", 1);

    ok &= gam.configSignals.MoveToRoot();
    ok &= gam.configSignals.CreateAbsolute("Signals.OutputSignals");
    ok &= gam.configSignals.CreateRelative("0");
    ok &= gam.configSignals.Write("QualifiedName", "OutputSignal1");
    ok &= gam.configSignals.Write("DataSource", "TestDataSource");
    ok &= gam.configSignals.Write("Type", "uint8");
    ok &= gam.configSignals.Write("NumberOfDimensions", 1);

    ok &= gam.configSignals.Write("NumberOfElements", 4);
    ok &= gam.configSignals.Write("ByteSize", 4);

    ok &= gam.configSignals.MoveAbsolute("Signals.OutputSignals");
    ok &= gam.configSignals.CreateRelative("1");
    ok &= gam.configSignals.Write("QualifiedName", "OutputSignal2");
    ok &= gam.configSignals.Write("DataSource", "TestDataSource");
    //ok &= gam.configSignals.Write("Type", "uint8");
    ok &= gam.configSignals.Write("NumberOfDimensions", 1);
    ok &= gam.configSignals.Write("NumberOfElements", 4);
    ok &= gam.configSignals.Write("ByteSize", 4);
    ok &= gam.configSignals.MoveToAncestor(1u);
    ok &= gam.configSignals.Write("ByteSize", 4 * 2);

    ok &= gam.configSignals.MoveToRoot();

    ok &= gam.configSignals.CreateAbsolute("Memory.InputSignals");
    ok &= gam.configSignals.CreateRelative("0");
    ok &= gam.configSignals.Write("DataSource", "TestDataSource");
    ok &= gam.configSignals.CreateRelative("Signals");
    ok &= gam.configSignals.CreateRelative("0");
    ok &= gam.configSignals.Write("Samples", 1);

    ok &= gam.configSignals.CreateAbsolute("Memory.OutputSignals");
    ok &= gam.configSignals.CreateRelative("0");
    ok &= gam.configSignals.Write("DataSource", "TestDataSource");
    ok &= gam.configSignals.CreateRelative("Signals");
    ok &= gam.configSignals.CreateRelative("0");
    ok &= gam.configSignals.Write("Samples", 1);
    ok &= gam.configSignals.MoveToAncestor(1u);
    ok &= gam.configSignals.CreateRelative("1");
    ok &= gam.configSignals.Write("Samples", 1);
    ok &= gam.configSignals.MoveToRoot();

    ok &= gam.SetConfiguredDatabase(gam.configSignals);

    ok &= gam.Setup();
    return !ok;
}

bool WaveformSinGAMTest::TestUInt8Execute() {
    bool ok = true;
    using namespace MARTe;
    using namespace FastMath;
    WaveformSinGAMTestHelper gam;
    gam.SetName("Test");
    ok &= gam.InitialiseWaveSin();

    gam.config.MoveToRoot();
    /*
     //example how to print a ConfigurationDatabase
     StreamString a;
     printf("size of a %llu\n", a.Size());
     printf("%d\n", a.Printf("%!", gam.config));
     printf("size of a %llu\n", a.Size());
     printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
     printf("%s\n", a.Buffer());
     printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");*/

    ok &= gam.Initialise(gam.config);

    ok &= gam.InitialiseConfigDataBaseSignal1();
    ok &= gam.SetConfiguredDatabase(gam.configSignals);
    /*StreamString b;
     printf("size of a %llu\n", b.Size());
     printf("%d\n", b.Printf("%!", gam.configSignals));
     printf("size of a %llu\n", b.Size());
     printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
     printf("%s\n", b.Buffer());
     printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");*/

    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();
    ok &= gam.Setup();

    uint32 *gamMemoryIn = static_cast<uint32 *>(gam.GetInputSignalsMemory());
    uint8 *gamMemoryOut = static_cast<uint8 *>(gam.GetOutputSignalsMemory());
    *gamMemoryIn = 0;
    //assign inputs and outputs
    for (uint32 i = 0u; i < gam.numberOfElementsOut; i++) {
        gamMemoryOut[i] = 0;
    }
    if (ok) {
        gam.Execute();
    }
    //Compare result against expected vale
    for (uint32 i = 0; i < gam.numberOfElementsOut; i++) {
        ok &= (gamMemoryOut[i] == 0.0); //uint8(gam.amplitude[0] * sin(2.02.0 * FastMath::PI * gam.frequency[0] *() + gam.phase[0])));
    }
    *gamMemoryIn = 1000000;
    if (ok) {
        gam.Execute();
    }
    //Compare result against expected vale
    uint8 aux = 0;
    for (uint32 i = 0; i < gam.numberOfElementsOut; i++) {
        aux = uint8(
                gam.amplitude * sin(2.0 * FastMath::PI * gam.frequency * (*gamMemoryIn / 1e6 + i * (1.0 / gam.numberOfElementsOut)) + gam.phase) + gam.offset);
        ok &= (gamMemoryOut[i] == aux);
    }
    *gamMemoryIn = 2000000;
    if (ok) {
        gam.Execute();
    }
    //Compare result against expected vale
    for (uint32 i = 0; i < gam.numberOfElementsOut; i++) {
        aux = int8(
                gam.amplitude * sin(2.0 * FastMath::PI * gam.frequency * (*gamMemoryIn / 1e6 + i * (1.0 / gam.numberOfElementsOut)) + gam.phase) + gam.offset);
        ok &= (gamMemoryOut[i] == aux);
    }
    return ok;
}

bool WaveformSinGAMTest::TestExecuteNyquistViolation() {
    bool ok = true;
    using namespace MARTe;
    using namespace FastMath;
    WaveformSinGAMTestHelper gam;
    gam.SetName("Test");
    ok &= gam.InitialiseWaveSin();

    gam.config.MoveToRoot();
    /*
     //example how to print a ConfigurationDatabase
     StreamString a;
     printf("size of a %llu\n", a.Size());
     printf("%d\n", a.Printf("%!", gam.config));
     printf("size of a %llu\n", a.Size());
     printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
     printf("%s\n", a.Buffer());
     printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");*/

    ok &= gam.Initialise(gam.config);

    ok &= gam.InitialiseConfigDataBaseSignal1();
    ok &= gam.SetConfiguredDatabase(gam.configSignals);
    /*StreamString b;
     printf("size of a %llu\n", b.Size());
     printf("%d\n", b.Printf("%!", gam.configSignals));
     printf("size of a %llu\n", b.Size());
     printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
     printf("%s\n", b.Buffer());
     printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");*/

    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();
    ok &= gam.Setup();

    uint32 *gamMemoryIn = static_cast<uint32 *>(gam.GetInputSignalsMemory());
    uint8 *gamMemoryOut = static_cast<uint8 *>(gam.GetOutputSignalsMemory());
    *gamMemoryIn = 0;
    //assign inputs and outputs
    for (uint32 i = 0u; i < gam.numberOfElementsOut; i++) {
        gamMemoryOut[i] = 0;
    }
    if (ok) {
        ok = gam.Execute();
    }
    //Compare result against expected vale
    for (uint32 i = 0; i < gam.numberOfElementsOut; i++) {
        ok &= (gamMemoryOut[i] == 0.0); //uint8(gam.amplitude[0] * sin(2.02.0 * FastMath::PI * gam.frequency[0] *() + gam.phase[0])));
    }
    *gamMemoryIn = 10000000;
    if (ok) {
        ok = !gam.Execute();
    }
    return ok;
}

bool WaveformSinGAMTest::TestInt8Execute() {
    bool ok = true;
    using namespace MARTe;
    using namespace FastMath;
    WaveformSinGAMTestHelper gam;
    gam.SetName("Test");
    ok &= gam.InitialiseWaveSin(10, 1, 0, 0);
    StreamString a;
    //example how to print a ConfigurationDatabase
    gam.config.MoveToRoot();
    ok &= gam.Initialise(gam.config);

    ok &= gam.InitialiseConfigDataBaseSignal1(SignedInteger8Bit);
    ok &= gam.SetConfiguredDatabase(gam.configSignals);
    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();
    ok &= gam.Setup();

    uint32 *gamMemoryIn = static_cast<uint32 *>(gam.GetInputSignalsMemory());
    int8 *gamMemoryOut = static_cast<int8 *>(gam.GetOutputSignalsMemory());
    *gamMemoryIn = 0;
    //assign inputs and outputs
    for (uint32 i = 0u; i < gam.numberOfElementsOut; i++) {
        gamMemoryOut[i] = 0;
    }
    if (ok) {
        gam.Execute();
    }
    //Compare result against expected vale
    for (uint32 i = 0; i < gam.numberOfElementsOut; i++) {
        ok &= (gamMemoryOut[i] == 0.0);
    }
    *gamMemoryIn = 1000000;
    if (ok) {
        gam.Execute();
    }
    //Compare result against expected vale
    int8 aux = 0;
    for (uint32 i = 0; i < gam.numberOfElementsOut; i++) {
        aux = int8(
                gam.amplitude * sin(2.0 * FastMath::PI * gam.frequency * (*gamMemoryIn / 1e6 + i * (1.0 / gam.numberOfElementsOut)) + gam.phase) + gam.offset);
        ok &= (gamMemoryOut[i] == aux);
    }
    *gamMemoryIn = 2000000;
    if (ok) {
        gam.Execute();
    }
    //Compare result against expected vale
    for (uint32 i = 0; i < gam.numberOfElementsOut; i++) {
        aux = int8(
                gam.amplitude * sin(2.0 * FastMath::PI * gam.frequency * (*gamMemoryIn / 1e6 + i * (1.0 / gam.numberOfElementsOut)) + gam.phase) + gam.offset);
        ok &= (gamMemoryOut[i] == aux);
    }
    return ok;
}


bool WaveformSinGAMTest::TestInt8Execute_2() {
    bool ok = true;
    using namespace MARTe;
    using namespace FastMath;
    WaveformSinGAMTestHelper gam;
    gam.SetName("Test");
    ok &= gam.InitialiseWaveSin(10, 1, 0, 0);
    StreamString a;
    //example how to print a ConfigurationDatabase
    gam.config.MoveToRoot();
    ok &= gam.Initialise(gam.config);

    TypeDescriptor type = UnsignedInteger8Bit;
    ok &= gam.configSignals.Write("QualifiedName", "WaveformSinGAMTest");
    uint32 totalByteSizeIn = gam.byteSizeIn;
    ok &= gam.configSignals.CreateAbsolute("Signals.InputSignals");
    ok &= gam.configSignals.CreateRelative("0");
    ok &= gam.configSignals.Write("QualifiedName", "InputSignal1");
    ok &= gam.configSignals.Write("DataSource", "TestDataSource");
    ok &= gam.configSignals.Write("Type", "uint64");
    ok &= gam.configSignals.Write("NumberOfDimensions", 1);
    ok &= gam.configSignals.Write("NumberOfElements", 1);
    ok &= gam.configSignals.Write("ByteSize", totalByteSizeIn);

    ok &= gam.configSignals.MoveToAncestor(1u);
    ok &= gam.configSignals.Write("ByteSize", totalByteSizeIn);

    uint32 totalByteSizeOut = gam.numberOfElementsOut * type.numberOfBits / 8;
    ok &= gam.configSignals.MoveToRoot();
    ok &= gam.configSignals.CreateAbsolute("Signals.OutputSignals");
    ok &= gam.configSignals.CreateRelative("0");
    ok &= gam.configSignals.Write("QualifiedName", "OutputSignal1");
    ok &= gam.configSignals.Write("DataSource", "TestDataSource");
    ok &= gam.configSignals.Write("Type", TypeDescriptor::GetTypeNameFromTypeDescriptor(type));
    ok &= gam.configSignals.Write("NumberOfDimensions", 1);

    ok &= gam.configSignals.Write("NumberOfElements", gam.numberOfElementsOut);
    ok &= gam.configSignals.Write("ByteSize", totalByteSizeOut);

    ok &= gam.configSignals.MoveToAncestor(1u);
    ok &= gam.configSignals.Write("ByteSize", totalByteSizeOut);

    ok &= gam.configSignals.MoveToRoot();

    ok &= gam.configSignals.CreateAbsolute("Memory.InputSignals");
    ok &= gam.configSignals.CreateRelative("0");
    ok &= gam.configSignals.Write("DataSource", "TestDataSource");
    ok &= gam.configSignals.CreateRelative("Signals");
    ok &= gam.configSignals.CreateRelative("0");
    ok &= gam.configSignals.Write("Samples", gam.numberOfSamplesIn);

    ok &= gam.configSignals.CreateAbsolute("Memory.OutputSignals");
    ok &= gam.configSignals.CreateRelative("0");
    ok &= gam.configSignals.Write("DataSource", "TestDataSource");
    ok &= gam.configSignals.CreateRelative("Signals");
    ok &= gam.configSignals.CreateRelative("0");
    ok &= gam.configSignals.Write("Samples", gam.numberOfSamplesOut);

    ok &= gam.configSignals.MoveToRoot();

    ok &= gam.SetConfiguredDatabase(gam.configSignals);
    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();
    ok &= gam.Setup();

    uint64 *gamMemoryIn = static_cast<uint64 *>(gam.GetInputSignalsMemory());
    int8 *gamMemoryOut = static_cast<int8 *>(gam.GetOutputSignalsMemory());
    *gamMemoryIn = 0;
    //assign inputs and outputs
    for (uint32 i = 0u; i < gam.numberOfElementsOut; i++) {
        gamMemoryOut[i] = 0;
    }
    if (ok) {
        gam.Execute();
    }
    //Compare result against expected vale
    for (uint32 i = 0; i < gam.numberOfElementsOut; i++) {
        ok &= (gamMemoryOut[i] == 0.0);
    }
    *gamMemoryIn = 1000000;
    if (ok) {
        gam.Execute();
    }
    //Compare result against expected vale
    int8 aux = 0;
    for (uint32 i = 0; i < gam.numberOfElementsOut; i++) {
        aux = int8(
                gam.amplitude * sin(2.0 * FastMath::PI * gam.frequency * (*gamMemoryIn / 1e6 + i * (1.0 / gam.numberOfElementsOut)) + gam.phase) + gam.offset);
        ok &= (gamMemoryOut[i] == aux);
    }
    *gamMemoryIn = 2000000;
    if (ok) {
        gam.Execute();
    }
    //Compare result against expected vale
    for (uint32 i = 0; i < gam.numberOfElementsOut; i++) {
        aux = int8(
                gam.amplitude * sin(2.0 * FastMath::PI * gam.frequency * (*gamMemoryIn / 1e6 + i * (1.0 / gam.numberOfElementsOut)) + gam.phase) + gam.offset);
        ok &= (gamMemoryOut[i] == aux);
    }
    return ok;
}

bool WaveformSinGAMTest::TestInt16Execute() {
    bool ok = true;
    using namespace MARTe;
    using namespace FastMath;
    WaveformSinGAMTestHelper gam;
    gam.SetName("Test");
    ok &= gam.InitialiseWaveSin(100, 1, 0, 0);
    StreamString a;
    //example how to print a ConfigurationDatabase
    gam.config.MoveToRoot();
    ok &= gam.Initialise(gam.config);

    ok &= gam.InitialiseConfigDataBaseSignal1(SignedInteger16Bit);
    ok &= gam.SetConfiguredDatabase(gam.configSignals);
    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();
    ok &= gam.Setup();

    uint32 *gamMemoryIn = static_cast<uint32 *>(gam.GetInputSignalsMemory());
    int16 *gamMemoryOut = static_cast<int16 *>(gam.GetOutputSignalsMemory());
    *gamMemoryIn = 0;
    //assign inputs and outputs
    for (uint32 i = 0u; i < gam.numberOfElementsOut; i++) {
        gamMemoryOut[i] = 0;
    }
    if (ok) {
        gam.Execute();
    }
    //Compare result against expected vale
    for (uint32 i = 0; i < gam.numberOfElementsOut; i++) {
        ok &= (gamMemoryOut[i] == 0.0);
    }
    *gamMemoryIn = 1000000;
    if (ok) {
        gam.Execute();
    }
    //Compare result against expected vale
    int16 aux = 0;
    for (uint32 i = 0; i < gam.numberOfElementsOut; i++) {
        aux = int16(
                gam.amplitude * sin(2.0 * FastMath::PI * gam.frequency * (*gamMemoryIn / 1e6 + i * (1.0 / gam.numberOfElementsOut)) + gam.phase) + gam.offset);
        ok &= (gamMemoryOut[i] == aux);
    }
    *gamMemoryIn = 2000000;
    if (ok) {
        gam.Execute();
    }
    //Compare result against expected vale
    for (uint32 i = 0; i < gam.numberOfElementsOut; i++) {
        aux = int16(
                gam.amplitude * sin(2.0 * FastMath::PI * gam.frequency * (*gamMemoryIn / 1e6 + i * (1.0 / gam.numberOfElementsOut)) + gam.phase) + gam.offset);
        ok &= (gamMemoryOut[i] == aux);
    }
    return ok;
}

bool WaveformSinGAMTest::TestUInt16Execute() {
    bool ok = true;
    using namespace MARTe;
    using namespace FastMath;
    using namespace FastMath;
    WaveformSinGAMTestHelper gam;
    gam.SetName("Test");
    ok &= gam.InitialiseWaveSin(100, 1, 0, 100);
    StreamString a;
    //example how to print a ConfigurationDatabase
    gam.config.MoveToRoot();
    ok &= gam.Initialise(gam.config);

    ok &= gam.InitialiseConfigDataBaseSignal1(UnsignedInteger16Bit);
    ok &= gam.SetConfiguredDatabase(gam.configSignals);
    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();
    ok &= gam.Setup();

    uint32 *gamMemoryIn = static_cast<uint32 *>(gam.GetInputSignalsMemory());
    uint16 *gamMemoryOut = static_cast<uint16 *>(gam.GetOutputSignalsMemory());
    *gamMemoryIn = 0;
    //assign inputs and outputs
    for (uint32 i = 0u; i < gam.numberOfElementsOut; i++) {
        gamMemoryOut[i] = 0;
    }
    if (ok) {
        gam.Execute();
    }
    //Compare result against expected vale
    for (uint32 i = 0; i < gam.numberOfElementsOut; i++) {
        ok &= (gamMemoryOut[i] == 0.0);
    }
    *gamMemoryIn = 1000000;
    if (ok) {
        gam.Execute();
    }
    //Compare result against expected vale
    uint16 aux = 0;
    for (uint32 i = 0; i < gam.numberOfElementsOut; i++) {
        aux = uint16(
                gam.amplitude * sin(2.0 * FastMath::PI * gam.frequency * (*gamMemoryIn / 1e6 + i * (1.0 / gam.numberOfElementsOut)) + gam.phase) + gam.offset);
        ok &= (gamMemoryOut[i] == aux);
    }
    *gamMemoryIn = 2000000;
    if (ok) {
        gam.Execute();
    }
    //Compare result against expected vale
    for (uint32 i = 0; i < gam.numberOfElementsOut; i++) {
        aux = uint16(
                gam.amplitude * sin(2.0 * FastMath::PI * gam.frequency * (*gamMemoryIn / 1e6 + i * (1.0 / gam.numberOfElementsOut)) + gam.phase) + gam.offset);
        ok &= (gamMemoryOut[i] == aux);
    }
    return ok;
}

bool WaveformSinGAMTest::TestInt32Execute() {
    bool ok = true;
    using namespace MARTe;
    using namespace FastMath;
    WaveformSinGAMTestHelper gam;
    gam.SetName("Test");
    ok &= gam.InitialiseWaveSin(10000, 1, 0, 0);
    StreamString a;
    //example how to print a ConfigurationDatabase
    gam.config.MoveToRoot();
    ok &= gam.Initialise(gam.config);

    ok &= gam.InitialiseConfigDataBaseSignal1(SignedInteger32Bit);
    ok &= gam.SetConfiguredDatabase(gam.configSignals);
    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();
    ok &= gam.Setup();

    uint32 *gamMemoryIn = static_cast<uint32 *>(gam.GetInputSignalsMemory());
    int32 *gamMemoryOut = static_cast<int32 *>(gam.GetOutputSignalsMemory());
    *gamMemoryIn = 0;
    //assign inputs and outputs
    for (uint32 i = 0u; i < gam.numberOfElementsOut; i++) {
        gamMemoryOut[i] = 0;
    }
    if (ok) {
        gam.Execute();
    }
    //Compare result against expected vale
    for (uint32 i = 0; i < gam.numberOfElementsOut; i++) {
        ok &= (gamMemoryOut[i] == 0.0);
    }
    *gamMemoryIn = 1000000;
    if (ok) {
        gam.Execute();
    }
    //Compare result against expected vale
    int32 aux = 0;
    for (uint32 i = 0; i < gam.numberOfElementsOut; i++) {
        aux = int32(
                gam.amplitude * sin(2.0 * FastMath::PI * gam.frequency * (*gamMemoryIn / 1e6 + i * (1.0 / gam.numberOfElementsOut)) + gam.phase) + gam.offset);
        ok &= (gamMemoryOut[i] == aux);
    }
    *gamMemoryIn = 2000000;
    if (ok) {
        gam.Execute();
    }
    //Compare result against expected vale
    for (uint32 i = 0; i < gam.numberOfElementsOut; i++) {
        aux = int32(
                gam.amplitude * sin(2.0 * FastMath::PI * gam.frequency * (*gamMemoryIn / 1e6 + i * (1.0 / gam.numberOfElementsOut)) + gam.phase) + gam.offset);
        ok &= (gamMemoryOut[i] == aux);
    }
    return ok;
}

bool WaveformSinGAMTest::TestUInt32Execute() {
    bool ok = true;
    using namespace MARTe;
    using namespace FastMath;
    WaveformSinGAMTestHelper gam;
    gam.SetName("Test");
    ok &= gam.InitialiseWaveSin(10000, 1, 0, 10000);
    StreamString a;
    //example how to print a ConfigurationDatabase
    gam.config.MoveToRoot();
    ok &= gam.Initialise(gam.config);

    ok &= gam.InitialiseConfigDataBaseSignal1(UnsignedInteger32Bit);
    ok &= gam.SetConfiguredDatabase(gam.configSignals);
    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();
    ok &= gam.Setup();

    uint32 *gamMemoryIn = static_cast<uint32 *>(gam.GetInputSignalsMemory());
    uint32 *gamMemoryOut = static_cast<uint32 *>(gam.GetOutputSignalsMemory());
    *gamMemoryIn = 0;
    //assign inputs and outputs
    for (uint32 i = 0u; i < gam.numberOfElementsOut; i++) {
        gamMemoryOut[i] = 0;
    }
    if (ok) {
        gam.Execute();
    }
    //Compare result against expected vale
    for (uint32 i = 0; i < gam.numberOfElementsOut; i++) {
        ok &= (gamMemoryOut[i] == 0.0);
    }
    *gamMemoryIn = 1000000;
    if (ok) {
        gam.Execute();
    }
    //Compare result against expected vale
    uint32 aux = 0;
    for (uint32 i = 0; i < gam.numberOfElementsOut; i++) {
        aux = uint32(
                gam.amplitude * sin(2.0 * FastMath::PI * gam.frequency * (*gamMemoryIn / 1e6 + i * (1.0 / gam.numberOfElementsOut)) + gam.phase) + gam.offset);
        ok &= (gamMemoryOut[i] == aux);
    }
    *gamMemoryIn = 2000000;
    if (ok) {
        gam.Execute();
    }
    //Compare result against expected vale
    for (uint32 i = 0; i < gam.numberOfElementsOut; i++) {
        aux = uint32(
                gam.amplitude * sin(2.0 * FastMath::PI * gam.frequency * (*gamMemoryIn / 1e6 + i * (1.0 / gam.numberOfElementsOut)) + gam.phase) + gam.offset);
        ok &= (gamMemoryOut[i] == aux);
    }
    return ok;
}

bool WaveformSinGAMTest::TestInt64Execute() {
    bool ok = true;
    using namespace MARTe;
    using namespace FastMath;
    WaveformSinGAMTestHelper gam;

    gam.SetName("Test");
    ok &= gam.InitialiseWaveSin(10000, 1, 0, 0);
    //example how to print a ConfigurationDatabase
    gam.config.MoveToRoot();
    ok &= gam.Initialise(gam.config);

    ok &= gam.InitialiseConfigDataBaseSignal1(SignedInteger64Bit);
    ok &= gam.SetConfiguredDatabase(gam.configSignals);
    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();
    ok &= gam.Setup();

    uint32 *gamMemoryIn = static_cast<uint32 *>(gam.GetInputSignalsMemory());
    int64 *gamMemoryOut = static_cast<int64 *>(gam.GetOutputSignalsMemory());
    *gamMemoryIn = 0;
    //assign inputs and outputs
    for (uint32 i = 0u; i < gam.numberOfElementsOut; i++) {
        gamMemoryOut[i] = 0;
    }
    if (ok) {
        gam.Execute();
    }

    //Compare result against expected vale
    for (uint32 i = 0; i < gam.numberOfElementsOut; i++) {
        ok &= (gamMemoryOut[i] == 0.0);
    }
    *gamMemoryIn = 1000000;
    if (ok) {
        gam.Execute();
    }
    //Compare result against expected vale
    int64 aux = 0;
    for (uint32 i = 0; i < gam.numberOfElementsOut; i++) {
        aux = int64(
                gam.amplitude * sin(2.0 * FastMath::PI * gam.frequency * (*gamMemoryIn / 1e6 + i * (1.0 / gam.numberOfElementsOut)) + gam.phase) + gam.offset);
        ok &= (gamMemoryOut[i] == aux);
    }
    *gamMemoryIn = 2000000;
    if (ok) {
        gam.Execute();
    }
    //Compare result against expected vale
    for (uint32 i = 0; i < gam.numberOfElementsOut; i++) {
        aux = int64(
                gam.amplitude * sin(2.0 * FastMath::PI * gam.frequency * (*gamMemoryIn / 1e6 + i * (1.0 / gam.numberOfElementsOut)) + gam.phase) + gam.offset);
        ok &= (gamMemoryOut[i] == aux);
    }
    return ok;
}

bool WaveformSinGAMTest::TestUInt64Execute() {
    bool ok = true;
    using namespace MARTe;
    using namespace FastMath;
    WaveformSinGAMTestHelper gam;
    gam.SetName("Test");
    ok &= gam.InitialiseWaveSin(100000, 1, 0, 10000);
    StreamString a;
    //example how to print a ConfigurationDatabase
    gam.config.MoveToRoot();
    ok &= gam.Initialise(gam.config);

    ok &= gam.InitialiseConfigDataBaseSignal1(UnsignedInteger64Bit);
    ok &= gam.SetConfiguredDatabase(gam.configSignals);
    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();
    ok &= gam.Setup();

    uint32 *gamMemoryIn = static_cast<uint32 *>(gam.GetInputSignalsMemory());
    uint64 *gamMemoryOut = static_cast<uint64 *>(gam.GetOutputSignalsMemory());
    *gamMemoryIn = 0;
    //assign inputs and outputs
    for (uint32 i = 0u; i < gam.numberOfElementsOut; i++) {
        gamMemoryOut[i] = 0;
    }
    if (ok) {
        gam.Execute();
    }
    //Compare result against expected vale
    for (uint32 i = 0; i < gam.numberOfElementsOut; i++) {
        ok &= (gamMemoryOut[i] == 0.0);
    }
    *gamMemoryIn = 1000000;
    if (ok) {
        gam.Execute();
    }
    //Compare result against expected vale
    uint64 aux = 0;
    for (uint32 i = 0; i < gam.numberOfElementsOut; i++) {
        aux = uint64(
                gam.amplitude * sin(2.0 * FastMath::PI * gam.frequency * (*gamMemoryIn / 1e6 + i * (1.0 / gam.numberOfElementsOut)) + gam.phase) + gam.offset);
        ok &= (gamMemoryOut[i] == aux);
    }
    *gamMemoryIn = 2000000;
    if (ok) {
        gam.Execute();
    }
    //Compare result against expected vale
    for (uint32 i = 0; i < gam.numberOfElementsOut; i++) {
        aux = uint64(
                gam.amplitude * sin(2.0 * FastMath::PI * gam.frequency * (*gamMemoryIn / 1e6 + i * (1.0 / gam.numberOfElementsOut)) + gam.phase) + gam.offset);
        ok &= (gamMemoryOut[i] == aux);
    }
    return ok;
}

bool WaveformSinGAMTest::TestFloat32Execute() {
    bool ok = true;
    using namespace MARTe;
    using namespace FastMath;
    WaveformSinGAMTestHelper gam;
    gam.SetName("Test");
    ok &= gam.InitialiseWaveSin(100000, 1, 0, 10000);
    StreamString a;
    //example how to print a ConfigurationDatabase
    gam.config.MoveToRoot();
    ok &= gam.Initialise(gam.config);

    ok &= gam.InitialiseConfigDataBaseSignal1(Float32Bit);
    ok &= gam.SetConfiguredDatabase(gam.configSignals);
    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();
    ok &= gam.Setup();

    uint32 *gamMemoryIn = static_cast<uint32 *>(gam.GetInputSignalsMemory());
    float32 *gamMemoryOut = static_cast<float32 *>(gam.GetOutputSignalsMemory());
    *gamMemoryIn = 0;
    //assign inputs and outputs
    for (uint32 i = 0u; i < gam.numberOfElementsOut; i++) {
        gamMemoryOut[i] = 0;
    }
    if (ok) {
        gam.Execute();
    }
    //Compare result against expected vale
    for (uint32 i = 0; i < gam.numberOfElementsOut; i++) {
        ok &= (gamMemoryOut[i] == 0.0);
    }
    *gamMemoryIn = 1000000;
    if (ok) {
        gam.Execute();
    }
    //Compare result against expected vale
    float32 aux = 0;
    for (uint32 i = 0; i < gam.numberOfElementsOut; i++) {
        aux = float32(
                gam.amplitude * sin(2.0 * FastMath::PI * gam.frequency * (*gamMemoryIn / 1e6 + i * (1.0 / gam.numberOfElementsOut)) + gam.phase) + gam.offset);
        ok &= (gamMemoryOut[i] == aux);
    }
    *gamMemoryIn = 2000000;
    if (ok) {
        gam.Execute();
    }
    //Compare result against expected vale
    for (uint32 i = 0; i < gam.numberOfElementsOut; i++) {
        aux = float32(
                gam.amplitude * sin(2.0 * FastMath::PI * gam.frequency * (*gamMemoryIn / 1e6 + i * (1.0 / gam.numberOfElementsOut)) + gam.phase) + gam.offset);
        ok &= (gamMemoryOut[i] == aux);
    }
    return ok;
}

bool WaveformSinGAMTest::TestFloat64Execute() {
    bool ok = true;
    using namespace MARTe;
    using namespace FastMath;
    WaveformSinGAMTestHelper gam;
    gam.SetName("Test");
    ok &= gam.InitialiseWaveSin(100000, 1, 0, 10000);
    StreamString a;
    //example how to print a ConfigurationDatabase
    gam.config.MoveToRoot();
    ok &= gam.Initialise(gam.config);

    ok &= gam.InitialiseConfigDataBaseSignal1(Float64Bit);
    ok &= gam.SetConfiguredDatabase(gam.configSignals);
    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();
    ok &= gam.Setup();

    uint32 *gamMemoryIn = static_cast<uint32 *>(gam.GetInputSignalsMemory());
    float64 *gamMemoryOut = static_cast<float64 *>(gam.GetOutputSignalsMemory());
    *gamMemoryIn = 0;
    //assign inputs and outputs
    for (uint32 i = 0u; i < gam.numberOfElementsOut; i++) {
        gamMemoryOut[i] = 0;
    }
    if (ok) {
        gam.Execute();
    }
    //Compare result against expected vale
    for (uint32 i = 0; i < gam.numberOfElementsOut; i++) {
        ok &= (gamMemoryOut[i] == 0.0);
    }
    *gamMemoryIn = 1000000;
    if (ok) {
        gam.Execute();
    }
    //Compare result against expected vale
    float64 aux = 0;
    for (uint32 i = 0; i < gam.numberOfElementsOut; i++) {
        aux = float64(
                gam.amplitude * sin(2.0 * FastMath::PI * gam.frequency * (*gamMemoryIn / 1e6 + i * (1.0 / gam.numberOfElementsOut)) + gam.phase) + gam.offset);
        ok &= (gamMemoryOut[i] == aux);
    }
    *gamMemoryIn = 2000000;
    if (ok) {
        gam.Execute();
    }
    //Compare result against expected vale
    for (uint32 i = 0; i < gam.numberOfElementsOut; i++) {
        aux = float64(
                gam.amplitude * sin(2.0 * FastMath::PI * gam.frequency * (*gamMemoryIn / 1e6 + i * (1.0 / gam.numberOfElementsOut)) + gam.phase) + gam.offset);
        ok &= (gamMemoryOut[i] == aux);
    }
    return ok;
}

bool WaveformSinGAMTest::TestSetup_StopTriggerGreaterThanStartTrigger() {
    bool ok = true;
    using namespace MARTe;
    using namespace FastMath;
    WaveformSinGAMTestHelper gam;
    gam.SetName("Test");

    ok &= gam.config.Write("Amplitude", 10);
    ok &= gam.config.Write("Frequency", 1);
    ok &= gam.config.Write("Phase", 0);
    ok &= gam.config.Write("Offset", 10);
    uint32 dimTrigger = 5;
    float64 *startTrigger = new float64[dimTrigger];
    float64 *stopTrigger = new float64[dimTrigger];
    startTrigger[0] = 1.250000;
    stopTrigger[0] = 1.750000;
    startTrigger[1] = 2.000000;
    stopTrigger[1] = 3.250000;
    startTrigger[2] = 3.500000;
    stopTrigger[2] = 3.750000;
    startTrigger[3] = 4.000000;
    stopTrigger[3] = 4.250000;
    startTrigger[4] = 4.550000;
    stopTrigger[4] = 4.800000;
    Vector<float64> startTVect(startTrigger, dimTrigger);
    Vector<float64> stopTVect(stopTrigger, dimTrigger);
    //ok &= gam.config.Write("StartTriggerTime", startTVect);
    ok &= gam.config.Write("StartTriggerTime", "");
    ok &= gam.config.Write("StopTriggerTime", stopTVect);

    gam.config.MoveToRoot();
    ok &= gam.Initialise(gam.config);
    ok &= gam.InitialiseConfigDataBaseSignal1(UnsignedInteger8Bit);
    ok &= gam.SetConfiguredDatabase(gam.configSignals);
    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();
    ok &= gam.Setup();
    return ok;
}

bool WaveformSinGAMTest::TestSetup_StartTriggerGreaterThanStopTriggerPlus2() {
    bool ok = true;
    using namespace MARTe;
    using namespace FastMath;
    WaveformSinGAMTestHelper gam;
    gam.SetName("Test");

    ok &= gam.config.Write("Amplitude", 10);
    ok &= gam.config.Write("Frequency", 1);
    ok &= gam.config.Write("Phase", 0);
    ok &= gam.config.Write("Offset", 10);
    uint32 dimTrigger = 5;
    float64 *startTrigger = new float64[dimTrigger];
    float64 *stopTrigger = new float64[dimTrigger - 2];
    startTrigger[0] = 1.250000;
    stopTrigger[0] = 1.750000;
    startTrigger[1] = 2.000000;
    stopTrigger[1] = 3.250000;
    startTrigger[2] = 3.500000;
    stopTrigger[2] = 3.750000;
    startTrigger[3] = 4.000000;
    //stopTrigger[3] = 4250000;
    startTrigger[4] = 4.550000;
    // stopTrigger[4] = 4800000;
    Vector<float64> startTVect(startTrigger, dimTrigger);
    Vector<float64> stopTVect(stopTrigger, dimTrigger - 2);
    //ok &= gam.config.Write("StartTriggerTime", startTVect);
    ok &= gam.config.Write("StartTriggerTime", startTVect);
    ok &= gam.config.Write("StopTriggerTime", stopTVect);

    gam.config.MoveToRoot();
    ok &= gam.Initialise(gam.config);
    ok &= gam.InitialiseConfigDataBaseSignal1(UnsignedInteger8Bit);
    ok &= gam.SetConfiguredDatabase(gam.configSignals);
    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();
    ok &= gam.Setup();
    return ok;
}

bool WaveformSinGAMTest::TestSetup_StartTriggerTimeArrayInconsistent() {
    bool ok = true;
    using namespace MARTe;
    using namespace FastMath;
    WaveformSinGAMTestHelper gam;
    gam.SetName("Test");

    ok &= gam.config.Write("Amplitude", 10);
    ok &= gam.config.Write("Frequency", 1);
    ok &= gam.config.Write("Phase", 0);
    ok &= gam.config.Write("Offset", 10);
    uint32 dimTrigger = 5;
    float64 *startTrigger = new float64[dimTrigger];
    float64 *stopTrigger = new float64[dimTrigger];
    startTrigger[0] = 1.25;
    stopTrigger[0] = 1.5;
    startTrigger[1] = 2.0;
    stopTrigger[1] = 3.25;
    startTrigger[2] = 3.5;
    stopTrigger[2] = 3.75;
    startTrigger[3] = 4.0;
    stopTrigger[3] = 4.25;
    startTrigger[4] = 4.55;
    stopTrigger[4] = 4.0;
    Vector<float64> startTVect(startTrigger, dimTrigger);
    Vector<float64> stopTVect(stopTrigger, dimTrigger);
    //ok &= gam.config.Write("StartTriggerTime", startTVect);
    ok &= gam.config.Write("StartTriggerTime", startTVect);
    ok &= gam.config.Write("StopTriggerTime", stopTVect);

    gam.config.MoveToRoot();
    ok &= gam.Initialise(gam.config);
    ok &= gam.InitialiseConfigDataBaseSignal1(UnsignedInteger8Bit);
    ok &= gam.SetConfiguredDatabase(gam.configSignals);
    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();
    ok &= gam.Setup();
    return ok;
}

bool WaveformSinGAMTest::TestUInt8ExecuteTrigger() {
    bool ok = true;
    using namespace MARTe;
    using namespace FastMath;
    WaveformSinGAMTestHelper gam;
    gam.SetName("Test");
    ok &= gam.InitialiseWaveSinTrigger();
    gam.config.MoveToRoot();
    ok &= gam.Initialise(gam.config);
    ok &= gam.InitialiseConfigDataBaseSignal1(UnsignedInteger8Bit);
    ok &= gam.SetConfiguredDatabase(gam.configSignals);
    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();
    ok &= gam.Setup();

    uint32 *gamMemoryIn = static_cast<uint32 *>(gam.GetInputSignalsMemory());
    uint8 *gamMemoryOut = static_cast<uint8 *>(gam.GetOutputSignalsMemory());
    *gamMemoryIn = 0;
    //assign inputs and outputs
    for (uint32 i = 0u; i < gam.numberOfElementsOut; i++) {
        gamMemoryOut[i] = 0;
    }
    if (ok) {
        gam.Execute();
    }
    //Compare result against expected vale
    for (uint32 i = 0; i < gam.numberOfElementsOut; i++) {
        ok &= (gamMemoryOut[i] == 0.0); //uint8(gam.amplitude[0] * sin(2.0 * FastMath::PI * gam.frequency[0] *() + gam.phase[0])));
    }
    *gamMemoryIn = 1000000;
    if (ok) {
        gam.Execute();
    }
    //Compare result against expected vale
    uint8 aux = 0;
    ok &= (gamMemoryOut[0] == 0.0);
    for (uint32 i = 1; i < 3; i++) {
        aux = uint8(
                gam.amplitude * sin(2.0 * FastMath::PI * gam.frequency * (*gamMemoryIn / 1e6 + i * (1.0 / gam.numberOfElementsOut)) + gam.phase) + gam.offset);
        ok &= (gamMemoryOut[i] == aux);
    }
    ok &= (gamMemoryOut[3] == 0.0);
    *gamMemoryIn = 2000000;
    if (ok) {
        gam.Execute();
    }
    //Compare result against expected vale
    for (uint32 i = 0; i < gam.numberOfElementsOut; i++) {
        aux = uint8(
                gam.amplitude * sin(2.0 * FastMath::PI * gam.frequency * (*gamMemoryIn / 1e6 + i * (1.0 / gam.numberOfElementsOut)) + gam.phase) + gam.offset);
        ok &= (gamMemoryOut[i] == aux);
    }

    *gamMemoryIn = 3000000;
    if (ok) {
        gam.Execute();
    }
    //Compare result against expected vale

    aux = uint8(gam.amplitude * sin(2.0 * FastMath::PI * gam.frequency * (*gamMemoryIn / 1e6 + 0 * (1.0 / gam.numberOfElementsOut)) + gam.phase) + gam.offset);
    ok &= (gamMemoryOut[0] == aux);

    ok &= (gamMemoryOut[1] == 0);

    aux = uint8(gam.amplitude * sin(2.0 * FastMath::PI * gam.frequency * (*gamMemoryIn / 1e6 + 2 * (1.0 / gam.numberOfElementsOut)) + gam.phase) + gam.offset);
    ok &= (gamMemoryOut[2] == aux);

    ok &= (gamMemoryOut[3] == 0u);

    *gamMemoryIn = 4000000;
    if (ok) {
        gam.Execute();
    }
    aux = uint8(gam.amplitude * sin(2.0 * FastMath::PI * gam.frequency * (*gamMemoryIn / 1e6 + 0 * (1.0 / gam.numberOfElementsOut)) + gam.phase) + gam.offset);
    ok &= (gamMemoryOut[0] == aux);

    ok &= (gamMemoryOut[1] == 0u);
    ok &= (gamMemoryOut[2] == 0u);

    aux = uint8(gam.amplitude * sin(2.0 * FastMath::PI * gam.frequency * (*gamMemoryIn / 1e6 + 3 * (1.0 / gam.numberOfElementsOut)) + gam.phase) + gam.offset);
    ok &= (gamMemoryOut[3] == aux);
    *gamMemoryIn = 5000000;
    if (ok) {
        gam.Execute();
    }
    for (uint32 i = 0; i < gam.numberOfElementsOut; i++) {
        ok &= (gamMemoryOut[i] == 0u);
    }
    return ok;
}

bool WaveformSinGAMTest::TestInt8ExecuteTrigger() {
    bool ok = true;
    using namespace MARTe;
    using namespace FastMath;
    WaveformSinGAMTestHelper gam;
    gam.SetName("Test");
    ok &= gam.InitialiseWaveSinTrigger();
    gam.config.MoveToRoot();
    ok &= gam.Initialise(gam.config);
    ok &= gam.InitialiseConfigDataBaseSignal1(SignedInteger8Bit);
    ok &= gam.SetConfiguredDatabase(gam.configSignals);
    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();
    ok &= gam.Setup();

    uint32 *gamMemoryIn = static_cast<uint32 *>(gam.GetInputSignalsMemory());
    int8 *gamMemoryOut = static_cast<int8 *>(gam.GetOutputSignalsMemory());
    *gamMemoryIn = 0;
    //assign inputs and outputs
    for (uint32 i = 0u; i < gam.numberOfElementsOut; i++) {
        gamMemoryOut[i] = 0;
    }
    if (ok) {
        gam.Execute();
    }
    //Compare result against expected vale
    for (uint32 i = 0; i < gam.numberOfElementsOut; i++) {
        ok &= (gamMemoryOut[i] == 0.0);
    }
    *gamMemoryIn = 1000000;
    if (ok) {
        gam.Execute();
    }
    //Compare result against expected vale
    int8 aux = 0;
    ok &= (gamMemoryOut[0] == 0.0);
    for (uint32 i = 1; i < 3; i++) {
        aux = int8(
                gam.amplitude * sin(2.0 * FastMath::PI * gam.frequency * (*gamMemoryIn / 1e6 + i * (1.0 / gam.numberOfElementsOut)) + gam.phase) + gam.offset);
        ok &= (gamMemoryOut[i] == aux);
    }
    ok &= (gamMemoryOut[3] == 0.0);
    *gamMemoryIn = 2000000;
    if (ok) {
        gam.Execute();
    }
    //Compare result against expected vale
    for (uint32 i = 0; i < gam.numberOfElementsOut; i++) {
        aux = int8(
                gam.amplitude * sin(2.0 * FastMath::PI * gam.frequency * (*gamMemoryIn / 1e6 + i * (1.0 / gam.numberOfElementsOut)) + gam.phase) + gam.offset);
        ok &= (gamMemoryOut[i] == aux);
    }

    *gamMemoryIn = 3000000;
    if (ok) {
        gam.Execute();
    }
    //Compare result against expected vale

    aux = int8(gam.amplitude * sin(2.0 * FastMath::PI * gam.frequency * (*gamMemoryIn / 1e6 + 0 * (1.0 / gam.numberOfElementsOut)) + gam.phase) + gam.offset);
    ok &= (gamMemoryOut[0] == aux);

    ok &= (gamMemoryOut[1] == 0);

    aux = int8(gam.amplitude * sin(2.0 * FastMath::PI * gam.frequency * (*gamMemoryIn / 1e6 + 2 * (1.0 / gam.numberOfElementsOut)) + gam.phase) + gam.offset);
    ok &= (gamMemoryOut[2] == aux);

    ok &= (gamMemoryOut[3] == 0u);

    *gamMemoryIn = 4000000;
    if (ok) {
        gam.Execute();
    }
    aux = int8(gam.amplitude * sin(2.0 * FastMath::PI * gam.frequency * (*gamMemoryIn / 1e6 + 0 * (1.0 / gam.numberOfElementsOut)) + gam.phase) + gam.offset);
    ok &= (gamMemoryOut[0] == aux);

    ok &= (gamMemoryOut[1] == 0u);
    ok &= (gamMemoryOut[2] == 0u);

    aux = int8(gam.amplitude * sin(2.0 * FastMath::PI * gam.frequency * (*gamMemoryIn / 1e6 + 3 * (1.0 / gam.numberOfElementsOut)) + gam.phase) + gam.offset);
    ok &= (gamMemoryOut[3] == aux);
    *gamMemoryIn = 5000000;
    if (ok) {
        gam.Execute();
    }
    for (uint32 i = 0; i < gam.numberOfElementsOut; i++) {
        ok &= (gamMemoryOut[i] == 0u);
    }
    return ok;
}

bool WaveformSinGAMTest::TestUInt16ExecuteTrigger() {
    bool ok = true;
    using namespace MARTe;
    using namespace FastMath;
    WaveformSinGAMTestHelper gam;
    gam.SetName("Test");
    ok &= gam.InitialiseWaveSinTrigger();
    gam.config.MoveToRoot();
    ok &= gam.Initialise(gam.config);
    ok &= gam.InitialiseConfigDataBaseSignal1(UnsignedInteger16Bit);
    ok &= gam.SetConfiguredDatabase(gam.configSignals);
    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();
    ok &= gam.Setup();

    uint32 *gamMemoryIn = static_cast<uint32 *>(gam.GetInputSignalsMemory());
    uint16 *gamMemoryOut = static_cast<uint16 *>(gam.GetOutputSignalsMemory());
    *gamMemoryIn = 0;
    //assign inputs and outputs
    for (uint32 i = 0u; i < gam.numberOfElementsOut; i++) {
        gamMemoryOut[i] = 0;
    }
    if (ok) {
        gam.Execute();
    }
    //Compare result against expected vale
    for (uint32 i = 0; i < gam.numberOfElementsOut; i++) {
        ok &= (gamMemoryOut[i] == 0.0);
    }
    *gamMemoryIn = 1000000;
    if (ok) {
        gam.Execute();
    }
    //Compare result against expected vale
    uint16 aux = 0;
    ok &= (gamMemoryOut[0] == 0.0);
    for (uint32 i = 1; i < 3; i++) {
        aux = uint16(
                gam.amplitude * sin(2.0 * FastMath::PI * gam.frequency * (*gamMemoryIn / 1e6 + i * (1.0 / gam.numberOfElementsOut)) + gam.phase) + gam.offset);
        ok &= (gamMemoryOut[i] == aux);
    }
    ok &= (gamMemoryOut[3] == 0.0);
    *gamMemoryIn = 2000000;
    if (ok) {
        gam.Execute();
    }
    //Compare result against expected vale
    for (uint32 i = 0; i < gam.numberOfElementsOut; i++) {
        aux = uint16(
                gam.amplitude * sin(2.0 * FastMath::PI * gam.frequency * (*gamMemoryIn / 1e6 + i * (1.0 / gam.numberOfElementsOut)) + gam.phase) + gam.offset);
        ok &= (gamMemoryOut[i] == aux);
    }

    *gamMemoryIn = 3000000;
    if (ok) {
        gam.Execute();
    }
    //Compare result against expected vale

    aux = uint16(gam.amplitude * sin(2.0 * FastMath::PI * gam.frequency * (*gamMemoryIn / 1e6 + 0 * (1.0 / gam.numberOfElementsOut)) + gam.phase) + gam.offset);
    ok &= (gamMemoryOut[0] == aux);

    ok &= (gamMemoryOut[1] == 0);

    aux = uint16(gam.amplitude * sin(2.0 * FastMath::PI * gam.frequency * (*gamMemoryIn / 1e6 + 2 * (1.0 / gam.numberOfElementsOut)) + gam.phase) + gam.offset);
    ok &= (gamMemoryOut[2] == aux);

    ok &= (gamMemoryOut[3] == 0u);

    *gamMemoryIn = 4000000;
    if (ok) {
        gam.Execute();
    }
    aux = uint16(gam.amplitude * sin(2.0 * FastMath::PI * gam.frequency * (*gamMemoryIn / 1e6 + 0 * (1.0 / gam.numberOfElementsOut)) + gam.phase) + gam.offset);
    ok &= (gamMemoryOut[0] == aux);

    ok &= (gamMemoryOut[1] == 0u);
    ok &= (gamMemoryOut[2] == 0u);

    aux = uint16(gam.amplitude * sin(2.0 * FastMath::PI * gam.frequency * (*gamMemoryIn / 1e6 + 3 * (1.0 / gam.numberOfElementsOut)) + gam.phase) + gam.offset);
    ok &= (gamMemoryOut[3] == aux);
    *gamMemoryIn = 5000000;
    if (ok) {
        gam.Execute();
    }
    for (uint32 i = 0; i < gam.numberOfElementsOut; i++) {
        ok &= (gamMemoryOut[i] == 0u);
    }
    return ok;
}

bool WaveformSinGAMTest::TestInt16ExecuteTrigger() {
    bool ok = true;
    using namespace MARTe;
    using namespace FastMath;
    WaveformSinGAMTestHelper gam;
    gam.SetName("Test");
    ok &= gam.InitialiseWaveSinTrigger();
    gam.config.MoveToRoot();
    ok &= gam.Initialise(gam.config);
    ok &= gam.InitialiseConfigDataBaseSignal1(SignedInteger16Bit);
    ok &= gam.SetConfiguredDatabase(gam.configSignals);
    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();
    ok &= gam.Setup();

    uint32 *gamMemoryIn = static_cast<uint32 *>(gam.GetInputSignalsMemory());
    int16 *gamMemoryOut = static_cast<int16 *>(gam.GetOutputSignalsMemory());
    *gamMemoryIn = 0;
    //assign inputs and outputs
    for (uint32 i = 0u; i < gam.numberOfElementsOut; i++) {
        gamMemoryOut[i] = 0;
    }
    if (ok) {
        gam.Execute();
    }
    //Compare result against expected vale
    for (uint32 i = 0; i < gam.numberOfElementsOut; i++) {
        ok &= (gamMemoryOut[i] == 0.0);
    }
    *gamMemoryIn = 1000000;
    if (ok) {
        gam.Execute();
    }
    //Compare result against expected vale
    int16 aux = 0;
    ok &= (gamMemoryOut[0] == 0.0);
    for (uint32 i = 1; i < 3; i++) {
        aux = int16(
                gam.amplitude * sin(2.0 * FastMath::PI * gam.frequency * (*gamMemoryIn / 1e6 + i * (1.0 / gam.numberOfElementsOut)) + gam.phase) + gam.offset);
        ok &= (gamMemoryOut[i] == aux);
    }
    ok &= (gamMemoryOut[3] == 0.0);
    *gamMemoryIn = 2000000;
    if (ok) {
        gam.Execute();
    }
    //Compare result against expected vale
    for (uint32 i = 0; i < gam.numberOfElementsOut; i++) {
        aux = int16(
                gam.amplitude * sin(2.0 * FastMath::PI * gam.frequency * (*gamMemoryIn / 1e6 + i * (1.0 / gam.numberOfElementsOut)) + gam.phase) + gam.offset);
        ok &= (gamMemoryOut[i] == aux);
    }

    *gamMemoryIn = 3000000;
    if (ok) {
        gam.Execute();
    }
    //Compare result against expected vale

    aux = int16(gam.amplitude * sin(2.0 * FastMath::PI * gam.frequency * (*gamMemoryIn / 1e6 + 0 * (1.0 / gam.numberOfElementsOut)) + gam.phase) + gam.offset);
    ok &= (gamMemoryOut[0] == aux);

    ok &= (gamMemoryOut[1] == 0);

    aux = int16(gam.amplitude * sin(2.0 * FastMath::PI * gam.frequency * (*gamMemoryIn / 1e6 + 2 * (1.0 / gam.numberOfElementsOut)) + gam.phase) + gam.offset);
    ok &= (gamMemoryOut[2] == aux);

    ok &= (gamMemoryOut[3] == 0u);

    *gamMemoryIn = 4000000;
    if (ok) {
        gam.Execute();
    }
    aux = int16(gam.amplitude * sin(2.0 * FastMath::PI * gam.frequency * (*gamMemoryIn / 1e6 + 0 * (1.0 / gam.numberOfElementsOut)) + gam.phase) + gam.offset);
    ok &= (gamMemoryOut[0] == aux);

    ok &= (gamMemoryOut[1] == 0u);
    ok &= (gamMemoryOut[2] == 0u);

    aux = int16(gam.amplitude * sin(2.0 * FastMath::PI * gam.frequency * (*gamMemoryIn / 1e6 + 3 * (1.0 / gam.numberOfElementsOut)) + gam.phase) + gam.offset);
    ok &= (gamMemoryOut[3] == aux);
    *gamMemoryIn = 5000000;
    if (ok) {
        gam.Execute();
    }
    for (uint32 i = 0; i < gam.numberOfElementsOut; i++) {
        ok &= (gamMemoryOut[i] == 0u);
    }
    return ok;
}

bool WaveformSinGAMTest::TestUInt32ExecuteTrigger() {
    bool ok = true;
    using namespace MARTe;
    using namespace FastMath;
    WaveformSinGAMTestHelper gam;
    gam.SetName("Test");
    ok &= gam.InitialiseWaveSinTrigger();
    gam.config.MoveToRoot();
    ok &= gam.Initialise(gam.config);
    ok &= gam.InitialiseConfigDataBaseSignal1(UnsignedInteger32Bit);
    ok &= gam.SetConfiguredDatabase(gam.configSignals);
    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();
    ok &= gam.Setup();

    uint32 *gamMemoryIn = static_cast<uint32 *>(gam.GetInputSignalsMemory());
    uint32 *gamMemoryOut = static_cast<uint32 *>(gam.GetOutputSignalsMemory());
    *gamMemoryIn = 0;
    //assign inputs and outputs
    for (uint32 i = 0u; i < gam.numberOfElementsOut; i++) {
        gamMemoryOut[i] = 0;
    }
    if (ok) {
        gam.Execute();
    }
    //Compare result against expected vale
    for (uint32 i = 0; i < gam.numberOfElementsOut; i++) {
        ok &= (gamMemoryOut[i] == 0.0);
    }
    *gamMemoryIn = 1000000;
    if (ok) {
        gam.Execute();
    }
    //Compare result against expected vale
    uint32 aux = 0;
    ok &= (gamMemoryOut[0] == 0.0);
    for (uint32 i = 1; i < 3; i++) {
        aux = uint32(
                gam.amplitude * sin(2.0 * FastMath::PI * gam.frequency * (*gamMemoryIn / 1e6 + i * (1.0 / gam.numberOfElementsOut)) + gam.phase) + gam.offset);
        ok &= (gamMemoryOut[i] == aux);
    }
    ok &= (gamMemoryOut[3] == 0.0);
    *gamMemoryIn = 2000000;
    if (ok) {
        gam.Execute();
    }
    //Compare result against expected vale
    for (uint32 i = 0; i < gam.numberOfElementsOut; i++) {
        aux = uint32(
                gam.amplitude * sin(2.0 * FastMath::PI * gam.frequency * (*gamMemoryIn / 1e6 + i * (1.0 / gam.numberOfElementsOut)) + gam.phase) + gam.offset);
        ok &= (gamMemoryOut[i] == aux);
    }

    *gamMemoryIn = 3000000;
    if (ok) {
        gam.Execute();
    }
    //Compare result against expected vale

    aux = uint32(gam.amplitude * sin(2.0 * FastMath::PI * gam.frequency * (*gamMemoryIn / 1e6 + 0 * (1.0 / gam.numberOfElementsOut)) + gam.phase) + gam.offset);
    ok &= (gamMemoryOut[0] == aux);

    ok &= (gamMemoryOut[1] == 0);

    aux = uint32(gam.amplitude * sin(2.0 * FastMath::PI * gam.frequency * (*gamMemoryIn / 1e6 + 2 * (1.0 / gam.numberOfElementsOut)) + gam.phase) + gam.offset);
    ok &= (gamMemoryOut[2] == aux);

    ok &= (gamMemoryOut[3] == 0u);

    *gamMemoryIn = 4000000;
    if (ok) {
        gam.Execute();
    }
    aux = uint32(gam.amplitude * sin(2.0 * FastMath::PI * gam.frequency * (*gamMemoryIn / 1e6 + 0 * (1.0 / gam.numberOfElementsOut)) + gam.phase) + gam.offset);
    ok &= (gamMemoryOut[0] == aux);

    ok &= (gamMemoryOut[1] == 0u);
    ok &= (gamMemoryOut[2] == 0u);

    aux = uint32(gam.amplitude * sin(2.0 * FastMath::PI * gam.frequency * (*gamMemoryIn / 1e6 + 3 * (1.0 / gam.numberOfElementsOut)) + gam.phase) + gam.offset);
    ok &= (gamMemoryOut[3] == aux);
    *gamMemoryIn = 5000000;
    if (ok) {
        gam.Execute();
    }
    for (uint32 i = 0; i < gam.numberOfElementsOut; i++) {
        ok &= (gamMemoryOut[i] == 0u);
    }
    return ok;
}

bool WaveformSinGAMTest::TestInt32ExecuteTrigger() {
    bool ok = true;
    using namespace MARTe;
    using namespace FastMath;
    WaveformSinGAMTestHelper gam;
    gam.SetName("Test");
    ok &= gam.InitialiseWaveSinTrigger();
    gam.config.MoveToRoot();
    ok &= gam.Initialise(gam.config);
    ok &= gam.InitialiseConfigDataBaseSignal1(SignedInteger32Bit);
    ok &= gam.SetConfiguredDatabase(gam.configSignals);
    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();
    ok &= gam.Setup();

    uint32 *gamMemoryIn = static_cast<uint32 *>(gam.GetInputSignalsMemory());
    int32 *gamMemoryOut = static_cast<int32 *>(gam.GetOutputSignalsMemory());
    *gamMemoryIn = 0;
    //assign inputs and outputs
    for (uint32 i = 0u; i < gam.numberOfElementsOut; i++) {
        gamMemoryOut[i] = 0;
    }
    if (ok) {
        gam.Execute();
    }
    //Compare result against expected vale
    for (uint32 i = 0; i < gam.numberOfElementsOut; i++) {
        ok &= (gamMemoryOut[i] == 0.0);
    }
    *gamMemoryIn = 1000000;
    if (ok) {
        gam.Execute();
    }
    //Compare result against expected vale
    int32 aux = 0;
    ok &= (gamMemoryOut[0] == 0.0);
    for (uint32 i = 1; i < 3; i++) {
        aux = int32(
                gam.amplitude * sin(2.0 * FastMath::PI * gam.frequency * (*gamMemoryIn / 1e6 + i * (1.0 / gam.numberOfElementsOut)) + gam.phase) + gam.offset);
        ok &= (gamMemoryOut[i] == aux);
    }
    ok &= (gamMemoryOut[3] == 0.0);
    *gamMemoryIn = 2000000;
    if (ok) {
        gam.Execute();
    }
    //Compare result against expected vale
    for (uint32 i = 0; i < gam.numberOfElementsOut; i++) {
        aux = int32(
                gam.amplitude * sin(2.0 * FastMath::PI * gam.frequency * (*gamMemoryIn / 1e6 + i * (1.0 / gam.numberOfElementsOut)) + gam.phase) + gam.offset);
        ok &= (gamMemoryOut[i] == aux);
    }

    *gamMemoryIn = 3000000;
    if (ok) {
        gam.Execute();
    }
    //Compare result against expected vale

    aux = int32(gam.amplitude * sin(2.0 * FastMath::PI * gam.frequency * (*gamMemoryIn / 1e6 + 0 * (1.0 / gam.numberOfElementsOut)) + gam.phase) + gam.offset);
    ok &= (gamMemoryOut[0] == aux);

    ok &= (gamMemoryOut[1] == 0);

    aux = int32(gam.amplitude * sin(2.0 * FastMath::PI * gam.frequency * (*gamMemoryIn / 1e6 + 2 * (1.0 / gam.numberOfElementsOut)) + gam.phase) + gam.offset);
    ok &= (gamMemoryOut[2] == aux);

    ok &= (gamMemoryOut[3] == 0u);

    *gamMemoryIn = 4000000;
    if (ok) {
        gam.Execute();
    }
    aux = int32(gam.amplitude * sin(2.0 * FastMath::PI * gam.frequency * (*gamMemoryIn / 1e6 + 0 * (1.0 / gam.numberOfElementsOut)) + gam.phase) + gam.offset);
    ok &= (gamMemoryOut[0] == aux);

    ok &= (gamMemoryOut[1] == 0u);
    ok &= (gamMemoryOut[2] == 0u);

    aux = int32(gam.amplitude * sin(2.0 * FastMath::PI * gam.frequency * (*gamMemoryIn / 1e6 + 3 * (1.0 / gam.numberOfElementsOut)) + gam.phase) + gam.offset);
    ok &= (gamMemoryOut[3] == aux);
    *gamMemoryIn = 5000000;
    if (ok) {
        gam.Execute();
    }
    for (uint32 i = 0; i < gam.numberOfElementsOut; i++) {
        ok &= (gamMemoryOut[i] == 0u);
    }
    return ok;
}

bool WaveformSinGAMTest::TestUInt64ExecuteTrigger() {
    bool ok = true;
    using namespace MARTe;
    using namespace FastMath;
    WaveformSinGAMTestHelper gam;
    gam.SetName("Test");
    ok &= gam.InitialiseWaveSinTrigger(1000, 1, 0, 1000);
    gam.config.MoveToRoot();
    ok &= gam.Initialise(gam.config);
    ok &= gam.InitialiseConfigDataBaseSignal1(UnsignedInteger64Bit);
    ok &= gam.SetConfiguredDatabase(gam.configSignals);
    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();
    ok &= gam.Setup();

    uint32 *gamMemoryIn = static_cast<uint32 *>(gam.GetInputSignalsMemory());
    uint64 *gamMemoryOut = static_cast<uint64 *>(gam.GetOutputSignalsMemory());
    *gamMemoryIn = 0;
    //assign inputs and outputs
    for (uint32 i = 0u; i < gam.numberOfElementsOut; i++) {
        gamMemoryOut[i] = 0;
    }
    if (ok) {
        gam.Execute();
    }
    //Compare result against expected vale
    for (uint32 i = 0; i < gam.numberOfElementsOut; i++) {
        ok &= (gamMemoryOut[i] == 0.0);
    }
    *gamMemoryIn = 1000000;
    if (ok) {
        gam.Execute();
    }
    //Compare result against expected vale
    uint64 aux = 0;
    ok &= (gamMemoryOut[0] == 0.0);
    for (uint32 i = 1; i < 3; i++) {
        aux = uint64(
                gam.amplitude * sin(2.0 * FastMath::PI * gam.frequency * (*gamMemoryIn / 1e6 + i * (1.0 / gam.numberOfElementsOut)) + gam.phase) + gam.offset);
        ok &= (gamMemoryOut[i] == aux);
    }
    ok &= (gamMemoryOut[3] == 0.0);
    *gamMemoryIn = 2000000;
    if (ok) {
        gam.Execute();
    }
    //Compare result against expected vale
    for (uint32 i = 0; i < gam.numberOfElementsOut; i++) {
        aux = uint64(
                gam.amplitude * sin(2.0 * FastMath::PI * gam.frequency * (*gamMemoryIn / 1e6 + i * (1.0 / gam.numberOfElementsOut)) + gam.phase) + gam.offset);
        ok &= (gamMemoryOut[i] == aux);
    }

    *gamMemoryIn = 3000000;
    if (ok) {
        gam.Execute();
    }
    //Compare result against expected vale

    aux = uint64(gam.amplitude * sin(2.0 * FastMath::PI * gam.frequency * (*gamMemoryIn / 1e6 + 0 * (1.0 / gam.numberOfElementsOut)) + gam.phase) + gam.offset);
    ok &= (gamMemoryOut[0] == aux);

    ok &= (gamMemoryOut[1] == 0);

    aux = uint64(gam.amplitude * sin(2.0 * FastMath::PI * gam.frequency * (*gamMemoryIn / 1e6 + 2 * (1.0 / gam.numberOfElementsOut)) + gam.phase) + gam.offset);
    ok &= (gamMemoryOut[2] == aux);

    ok &= (gamMemoryOut[3] == 0u);

    *gamMemoryIn = 4000000;
    if (ok) {
        gam.Execute();
    }
    aux = uint64(gam.amplitude * sin(2.0 * FastMath::PI * gam.frequency * (*gamMemoryIn / 1e6 + 0 * (1.0 / gam.numberOfElementsOut)) + gam.phase) + gam.offset);
    ok &= (gamMemoryOut[0] == aux);

    ok &= (gamMemoryOut[1] == 0u);
    ok &= (gamMemoryOut[2] == 0u);

    aux = uint64(gam.amplitude * sin(2.0 * FastMath::PI * gam.frequency * (*gamMemoryIn / 1e6 + 3 * (1.0 / gam.numberOfElementsOut)) + gam.phase) + gam.offset);
    ok &= (gamMemoryOut[3] == aux);
    *gamMemoryIn = 5000000;
    if (ok) {
        gam.Execute();
    }
    for (uint32 i = 0; i < gam.numberOfElementsOut; i++) {
        ok &= (gamMemoryOut[i] == 0u);
    }
    return ok;
}

bool WaveformSinGAMTest::TestInt64ExecuteTrigger() {
    bool ok = true;
    using namespace MARTe;
    using namespace FastMath;
    WaveformSinGAMTestHelper gam;
    gam.SetName("Test");
    ok &= gam.InitialiseWaveSinTrigger(1000);
    gam.config.MoveToRoot();
    ok &= gam.Initialise(gam.config);
    ok &= gam.InitialiseConfigDataBaseSignal1(SignedInteger64Bit);
    ok &= gam.SetConfiguredDatabase(gam.configSignals);
    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();
    ok &= gam.Setup();

    uint32 *gamMemoryIn = static_cast<uint32 *>(gam.GetInputSignalsMemory());
    int64 *gamMemoryOut = static_cast<int64 *>(gam.GetOutputSignalsMemory());
    *gamMemoryIn = 0;
    //assign inputs and outputs
    for (uint32 i = 0u; i < gam.numberOfElementsOut; i++) {
        gamMemoryOut[i] = 0;
    }
    if (ok) {
        gam.Execute();
    }
    //Compare result against expected vale
    for (uint32 i = 0; i < gam.numberOfElementsOut; i++) {
        ok &= (gamMemoryOut[i] == 0.0);
    }
    *gamMemoryIn = 1000000;
    if (ok) {
        gam.Execute();
    }
    //Compare result against expected vale
    int64 aux = 0;
    ok &= (gamMemoryOut[0] == 0.0);
    for (uint32 i = 1; i < 3; i++) {
        aux = int64(
                gam.amplitude * sin(2.0 * FastMath::PI * gam.frequency * (*gamMemoryIn / 1e6 + i * (1.0 / gam.numberOfElementsOut)) + gam.phase) + gam.offset);
        ok &= (gamMemoryOut[i] == aux);
    }
    ok &= (gamMemoryOut[3] == 0.0);
    *gamMemoryIn = 2000000;
    if (ok) {
        gam.Execute();
    }
    //Compare result against expected vale
    for (uint32 i = 0; i < gam.numberOfElementsOut; i++) {
        aux = int64(
                gam.amplitude * sin(2.0 * FastMath::PI * gam.frequency * (*gamMemoryIn / 1e6 + i * (1.0 / gam.numberOfElementsOut)) + gam.phase) + gam.offset);
        ok &= (gamMemoryOut[i] == aux);
    }

    *gamMemoryIn = 3000000;
    if (ok) {
        gam.Execute();
    }
    //Compare result against expected vale

    aux = int64(gam.amplitude * sin(2.0 * FastMath::PI * gam.frequency * (*gamMemoryIn / 1e6 + 0 * (1.0 / gam.numberOfElementsOut)) + gam.phase) + gam.offset);
    ok &= (gamMemoryOut[0] == aux);

    ok &= (gamMemoryOut[1] == 0);

    aux = int64(gam.amplitude * sin(2.0 * FastMath::PI * gam.frequency * (*gamMemoryIn / 1e6 + 2 * (1.0 / gam.numberOfElementsOut)) + gam.phase) + gam.offset);
    ok &= (gamMemoryOut[2] == aux);

    ok &= (gamMemoryOut[3] == 0u);

    *gamMemoryIn = 4000000;
    if (ok) {
        gam.Execute();
    }
    aux = int64(gam.amplitude * sin(2.0 * FastMath::PI * gam.frequency * (*gamMemoryIn / 1e6 + 0 * (1.0 / gam.numberOfElementsOut)) + gam.phase) + gam.offset);
    ok &= (gamMemoryOut[0] == aux);

    ok &= (gamMemoryOut[1] == 0u);
    ok &= (gamMemoryOut[2] == 0u);

    aux = int64(gam.amplitude * sin(2.0 * FastMath::PI * gam.frequency * (*gamMemoryIn / 1e6 + 3 * (1.0 / gam.numberOfElementsOut)) + gam.phase) + gam.offset);
    ok &= (gamMemoryOut[3] == aux);
    *gamMemoryIn = 5000000;
    if (ok) {
        gam.Execute();
    }
    for (uint32 i = 0; i < gam.numberOfElementsOut; i++) {
        ok &= (gamMemoryOut[i] == 0u);
    }
    return ok;
}

bool WaveformSinGAMTest::TestFloat32ExecuteTrigger() {
    bool ok = true;
    using namespace MARTe;
    using namespace FastMath;
    WaveformSinGAMTestHelper gam;
    gam.SetName("Test");
    ok &= gam.InitialiseWaveSinTrigger(1000);
    gam.config.MoveToRoot();
    ok &= gam.Initialise(gam.config);
    ok &= gam.InitialiseConfigDataBaseSignal1(Float32Bit);
    ok &= gam.SetConfiguredDatabase(gam.configSignals);
    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();
    ok &= gam.Setup();

    uint32 *gamMemoryIn = static_cast<uint32 *>(gam.GetInputSignalsMemory());
    float32 *gamMemoryOut = static_cast<float32 *>(gam.GetOutputSignalsMemory());
    *gamMemoryIn = 0;
    //assign inputs and outputs
    for (uint32 i = 0u; i < gam.numberOfElementsOut; i++) {
        gamMemoryOut[i] = 0;
    }
    if (ok) {
        gam.Execute();
    }
    //Compare result against expected vale
    for (uint32 i = 0; i < gam.numberOfElementsOut; i++) {
        ok &= (gamMemoryOut[i] == 0.0);
    }
    *gamMemoryIn = 1000000;
    if (ok) {
        gam.Execute();
    }
    //Compare result against expected vale
    float32 aux = 0;
    ok &= (gamMemoryOut[0] == 0.0);
    for (uint32 i = 1; i < 3; i++) {
        aux = float32(
                gam.amplitude * sin(2.0 * FastMath::PI * gam.frequency * (*gamMemoryIn / 1e6 + i * (1.0 / gam.numberOfElementsOut)) + gam.phase) + gam.offset);
        ok &= (gamMemoryOut[i] == aux);
    }
    ok &= (gamMemoryOut[3] == 0.0);
    *gamMemoryIn = 2000000;
    if (ok) {
        gam.Execute();
    }
    //Compare result against expected vale
    for (uint32 i = 0; i < gam.numberOfElementsOut; i++) {
        aux = float32(
                gam.amplitude * sin(2.0 * FastMath::PI * gam.frequency * (*gamMemoryIn / 1e6 + i * (1.0 / gam.numberOfElementsOut)) + gam.phase) + gam.offset);
        ok &= (gamMemoryOut[i] == aux);
    }

    *gamMemoryIn = 3000000;
    if (ok) {
        gam.Execute();
    }
    //Compare result against expected vale

    aux = float32(
            gam.amplitude * sin(2.0 * FastMath::PI * gam.frequency * (*gamMemoryIn / 1e6 + 0 * (1.0 / gam.numberOfElementsOut)) + gam.phase) + gam.offset);
    ok &= (gamMemoryOut[0] == aux);

    ok &= (gamMemoryOut[1] == 0);

    aux = float32(
            gam.amplitude * sin(2.0 * FastMath::PI * gam.frequency * (*gamMemoryIn / 1e6 + 2 * (1.0 / gam.numberOfElementsOut)) + gam.phase) + gam.offset);
    ok &= (gamMemoryOut[2] == aux);

    ok &= (gamMemoryOut[3] == 0u);

    *gamMemoryIn = 4000000;
    if (ok) {
        gam.Execute();
    }
    aux = float32(
            gam.amplitude * sin(2.0 * FastMath::PI * gam.frequency * (*gamMemoryIn / 1e6 + 0 * (1.0 / gam.numberOfElementsOut)) + gam.phase) + gam.offset);
    ok &= (gamMemoryOut[0] == aux);

    ok &= (gamMemoryOut[1] == 0u);
    ok &= (gamMemoryOut[2] == 0u);

    aux = float32(
            gam.amplitude * sin(2.0 * FastMath::PI * gam.frequency * (*gamMemoryIn / 1e6 + 3 * (1.0 / gam.numberOfElementsOut)) + gam.phase) + gam.offset);
    ok &= (gamMemoryOut[3] == aux);
    *gamMemoryIn = 5000000;
    if (ok) {
        gam.Execute();
    }
    for (uint32 i = 0; i < gam.numberOfElementsOut; i++) {
        ok &= (gamMemoryOut[i] == 0u);
    }
    return ok;
}

bool WaveformSinGAMTest::TestFloat64ExecuteTrigger() {
    bool ok = true;
    using namespace MARTe;
    using namespace FastMath;
    WaveformSinGAMTestHelper gam;
    gam.SetName("Test");
    ok &= gam.InitialiseWaveSinTrigger(1000);
    gam.config.MoveToRoot();
    ok &= gam.Initialise(gam.config);
    ok &= gam.InitialiseConfigDataBaseSignal1(Float64Bit);
    ok &= gam.SetConfiguredDatabase(gam.configSignals);
    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();
    ok &= gam.Setup();

    uint32 *gamMemoryIn = static_cast<uint32 *>(gam.GetInputSignalsMemory());
    float64 *gamMemoryOut = static_cast<float64 *>(gam.GetOutputSignalsMemory());
    *gamMemoryIn = 0;
    //assign inputs and outputs
    for (uint32 i = 0u; i < gam.numberOfElementsOut; i++) {
        gamMemoryOut[i] = 0;
    }
    if (ok) {
        gam.Execute();
    }
    //Compare result against expected vale
    for (uint32 i = 0; i < gam.numberOfElementsOut; i++) {
        ok &= (gamMemoryOut[i] == 0.0);
    }
    *gamMemoryIn = 1000000;
    if (ok) {
        gam.Execute();
    }
    //Compare result against expected vale
    float64 aux = 0;
    ok &= (gamMemoryOut[0] == 0.0);
    for (uint32 i = 1; i < 3; i++) {
        aux = float64(
                gam.amplitude * sin(2.0 * FastMath::PI * gam.frequency * (*gamMemoryIn / 1e6 + i * (1.0 / gam.numberOfElementsOut)) + gam.phase) + gam.offset);
        ok &= (gamMemoryOut[i] == aux);
    }
    ok &= (gamMemoryOut[3] == 0.0);
    *gamMemoryIn = 2000000;
    if (ok) {
        gam.Execute();
    }
    //Compare result against expected vale
    for (uint32 i = 0; i < gam.numberOfElementsOut; i++) {
        aux = float64(
                gam.amplitude * sin(2.0 * FastMath::PI * gam.frequency * (*gamMemoryIn / 1e6 + i * (1.0 / gam.numberOfElementsOut)) + gam.phase) + gam.offset);
        ok &= (gamMemoryOut[i] == aux);
    }

    *gamMemoryIn = 3000000;
    if (ok) {
        gam.Execute();
    }
    //Compare result against expected vale

    aux = float64(
            gam.amplitude * sin(2.0 * FastMath::PI * gam.frequency * (*gamMemoryIn / 1e6 + 0 * (1.0 / gam.numberOfElementsOut)) + gam.phase) + gam.offset);
    ok &= (gamMemoryOut[0] == aux);

    ok &= (gamMemoryOut[1] == 0);

    aux = float64(
            gam.amplitude * sin(2.0 * FastMath::PI * gam.frequency * (*gamMemoryIn / 1e6 + 2 * (1.0 / gam.numberOfElementsOut)) + gam.phase) + gam.offset);
    ok &= (gamMemoryOut[2] == aux);

    ok &= (gamMemoryOut[3] == 0u);

    *gamMemoryIn = 4000000;
    if (ok) {
        gam.Execute();
    }
    aux = float64(
            gam.amplitude * sin(2.0 * FastMath::PI * gam.frequency * (*gamMemoryIn / 1e6 + 0 * (1.0 / gam.numberOfElementsOut)) + gam.phase) + gam.offset);
    ok &= (gamMemoryOut[0] == aux);

    ok &= (gamMemoryOut[1] == 0u);
    ok &= (gamMemoryOut[2] == 0u);

    aux = float64(
            gam.amplitude * sin(2.0 * FastMath::PI * gam.frequency * (*gamMemoryIn / 1e6 + 3 * (1.0 / gam.numberOfElementsOut)) + gam.phase) + gam.offset);
    ok &= (gamMemoryOut[3] == aux);
    *gamMemoryIn = 5000000;
    if (ok) {
        gam.Execute();
    }
    for (uint32 i = 0; i < gam.numberOfElementsOut; i++) {
        ok &= (gamMemoryOut[i] == 0u);
    }
    return ok;
}

bool WaveformSinGAMTest::TestExecuteTriggerDiffArrayLength() {
    bool ok = true;
    using namespace MARTe;
    using namespace FastMath;
    WaveformSinGAMTestHelper gam;
    gam.SetName("Test");
    gam.amplitude = 1000;
    gam.frequency = 1;
    gam.phase = 0;
    gam.offset = 0;
    ok &= gam.config.Write("Amplitude", gam.amplitude);
    ok &= gam.config.Write("Frequency", gam.frequency);
    ok &= gam.config.Write("Phase", gam.phase);
    ok &= gam.config.Write("Offset", gam.offset);
    uint32 dimTrigger = 5;
    float64 *startTrigger = new float64[dimTrigger];
    float64 *stopTrigger = new float64[dimTrigger - 1];
    startTrigger[0] = 1.25;
    stopTrigger[0] = 1.75;
    startTrigger[1] = 2.;
    stopTrigger[1] = 3.25;
    startTrigger[2] = 3.5;
    stopTrigger[2] = 3.75;
    startTrigger[3] = 4.0;
    stopTrigger[3] = 4.25;
    startTrigger[4] = 4.55;
    Vector<float64> startTVect(startTrigger, dimTrigger);
    Vector<float64> stopTVect(stopTrigger, dimTrigger - 1);
    //ok &= gam.config.Write("StartTriggerTime", startTVect);
    ok &= gam.config.Write("StartTriggerTime", startTVect);
    ok &= gam.config.Write("StopTriggerTime", stopTVect);

    gam.config.MoveToRoot();
    ok &= gam.Initialise(gam.config);
    ok &= gam.InitialiseConfigDataBaseSignal1(SignedInteger64Bit);
    ok &= gam.SetConfiguredDatabase(gam.configSignals);
    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();
    ok &= gam.Setup();

    uint32 *gamMemoryIn = static_cast<uint32 *>(gam.GetInputSignalsMemory());
    int64 *gamMemoryOut = static_cast<int64 *>(gam.GetOutputSignalsMemory());
    *gamMemoryIn = 0;
    //assign inputs and outputs
    for (uint32 i = 0u; i < gam.numberOfElementsOut; i++) {
        gamMemoryOut[i] = 0;
    }
    if (ok) {
        gam.Execute();
    }
    //Compare result against expected vale
    for (uint32 i = 0; i < gam.numberOfElementsOut; i++) {
        ok &= (gamMemoryOut[i] == 0.0);
    }
    *gamMemoryIn = 1000000;
    if (ok) {
        gam.Execute();
    }
    //Compare result against expected vale
    int64 aux = 0;
    ok &= (gamMemoryOut[0] == 0.0);
    for (uint32 i = 1; i < 3; i++) {
        aux = int64(
                gam.amplitude * sin(2.0 * FastMath::PI * gam.frequency * (*gamMemoryIn / 1e6 + i * (1.0 / gam.numberOfElementsOut)) + gam.phase) + gam.offset);
        ok &= (gamMemoryOut[i] == aux);
    }
    ok &= (gamMemoryOut[3] == 0.0);
    *gamMemoryIn = 2000000;
    if (ok) {
        gam.Execute();
    }
    //Compare result against expected vale
    for (uint32 i = 0; i < gam.numberOfElementsOut; i++) {
        aux = int64(
                gam.amplitude * sin(2.0 * FastMath::PI * gam.frequency * (*gamMemoryIn / 1e6 + i * (1.0 / gam.numberOfElementsOut)) + gam.phase) + gam.offset);
        ok &= (gamMemoryOut[i] == aux);
    }

    *gamMemoryIn = 3000000;
    if (ok) {
        gam.Execute();
    }
    //Compare result against expected vale

    aux = int64(gam.amplitude * sin(2.0 * FastMath::PI * gam.frequency * (*gamMemoryIn / 1e6 + 0 * (1.0 / gam.numberOfElementsOut)) + gam.phase) + gam.offset);
    ok &= (gamMemoryOut[0] == aux);

    ok &= (gamMemoryOut[1] == 0);

    aux = int64(gam.amplitude * sin(2.0 * FastMath::PI * gam.frequency * (*gamMemoryIn / 1e6 + 2 * (1.0 / gam.numberOfElementsOut)) + gam.phase) + gam.offset);
    ok &= (gamMemoryOut[2] == aux);

    ok &= (gamMemoryOut[3] == 0u);

    *gamMemoryIn = 4000000;
    if (ok) {
        gam.Execute();
    }
    aux = int64(gam.amplitude * sin(2.0 * FastMath::PI * gam.frequency * (*gamMemoryIn / 1e6 + 0 * (1.0 / gam.numberOfElementsOut)) + gam.phase) + gam.offset);
    ok &= (gamMemoryOut[0] == aux);

    ok &= (gamMemoryOut[1] == 0u);
    ok &= (gamMemoryOut[2] == 0u);

    aux = int64(gam.amplitude * sin(2.0 * FastMath::PI * gam.frequency * (*gamMemoryIn / 1e6 + 3 * (1.0 / gam.numberOfElementsOut)) + gam.phase) + gam.offset);
    ok &= (gamMemoryOut[3] == aux);
    *gamMemoryIn = 5000000;
    if (ok) {
        gam.Execute();
    }
    for (uint32 i = 0; i < gam.numberOfElementsOut; i++) {
        aux = int64(
                gam.amplitude * sin(2.0 * FastMath::PI * gam.frequency * (*gamMemoryIn / 1e6 + i * (1.0 / gam.numberOfElementsOut)) + gam.phase) + gam.offset);
        ok &= (gamMemoryOut[i] == aux);
    }

    *gamMemoryIn = 6000000;
    if (ok) {
        gam.Execute();
    }
    for (uint32 i = 0; i < gam.numberOfElementsOut; i++) {
        aux = int64(
                gam.amplitude * sin(2.0 * FastMath::PI * gam.frequency * (*gamMemoryIn / 1e6 + i * (1.0 / gam.numberOfElementsOut)) + gam.phase) + gam.offset);
        ok &= (gamMemoryOut[i] == aux);
    }
    return ok;
}

bool WaveformSinGAMTest::TestExecuteTigger1Element() {
    bool ok = true;
    using namespace MARTe;
    using namespace FastMath;
    WaveformSinGAMTestHelper gam;
    gam.SetName("Test");
    gam.amplitude = 1000;
    gam.frequency = 0.1;
    gam.phase = 0;
    gam.offset = 0;
    ok &= gam.config.Write("Amplitude", gam.amplitude);
    ok &= gam.config.Write("Frequency", gam.frequency);
    ok &= gam.config.Write("Phase", gam.phase);
    ok &= gam.config.Write("Offset", gam.offset);
    uint32 dimTrigger = 5;
    float64 *startTrigger = new float64[dimTrigger];
    float64 *stopTrigger = new float64[dimTrigger - 1];
    startTrigger[0] = 1.25;
    stopTrigger[0] = 1.75;
    startTrigger[1] = 2.;
    stopTrigger[1] = 3.25;
    startTrigger[2] = 3.5;
    stopTrigger[2] = 3.75;
    startTrigger[3] = 4.0;
    stopTrigger[3] = 4.25;
    startTrigger[4] = 4.55;
    stopTrigger[4] = 4.8;
    Vector<float64> startTVect(startTrigger, dimTrigger);
    Vector<float64> stopTVect(stopTrigger, dimTrigger);
    //ok &= gam.config.Write("StartTriggerTime", startTVect);
    ok &= gam.config.Write("StartTriggerTime", startTVect);
    ok &= gam.config.Write("StopTriggerTime", stopTVect);

    gam.config.MoveToRoot();
    ok &= gam.Initialise(gam.config);

    uint32 totalByteSizeIn = 4u;
    ok &= gam.configSignals.Write("QualifiedName", "WaveformSinGAMTest");
    ok &= gam.configSignals.CreateAbsolute("Signals.InputSignals");
    ok &= gam.configSignals.CreateRelative("0");
    ok &= gam.configSignals.Write("QualifiedName", "InputSignal1");
    ok &= gam.configSignals.Write("DataSource", "TestDataSource");
    ok &= gam.configSignals.Write("Type", "uint32");
    ok &= gam.configSignals.Write("NumberOfDimensions", 1);
    ok &= gam.configSignals.Write("NumberOfElements", 1);
    ok &= gam.configSignals.Write("ByteSize", totalByteSizeIn);

    ok &= gam.configSignals.MoveToAncestor(1u);
    ok &= gam.configSignals.Write("ByteSize", totalByteSizeIn);

    uint32 totalByteSizeOut = 1 * 4;
    ok &= gam.configSignals.MoveToRoot();
    ok &= gam.configSignals.CreateAbsolute("Signals.OutputSignals");
    ok &= gam.configSignals.CreateRelative("0");
    ok &= gam.configSignals.Write("QualifiedName", "OutputSignal1");
    ok &= gam.configSignals.Write("DataSource", "TestDataSource");
    ok &= gam.configSignals.Write("Type", "float32");
    ok &= gam.configSignals.Write("NumberOfDimensions", 1);

    ok &= gam.configSignals.Write("NumberOfElements", 1);
    ok &= gam.configSignals.Write("ByteSize", totalByteSizeOut);

    ok &= gam.configSignals.MoveToAncestor(1u);
    ok &= gam.configSignals.Write("ByteSize", totalByteSizeOut);

    ok &= gam.configSignals.MoveToRoot();

    ok &= gam.configSignals.CreateAbsolute("Memory.InputSignals");
    ok &= gam.configSignals.CreateRelative("0");
    ok &= gam.configSignals.Write("DataSource", "TestDataSource");
    ok &= gam.configSignals.CreateRelative("Signals");
    ok &= gam.configSignals.CreateRelative("0");
    ok &= gam.configSignals.Write("Samples", 1);

    ok &= gam.configSignals.CreateAbsolute("Memory.OutputSignals");
    ok &= gam.configSignals.CreateRelative("0");
    ok &= gam.configSignals.Write("DataSource", "TestDataSource");
    ok &= gam.configSignals.CreateRelative("Signals");
    ok &= gam.configSignals.CreateRelative("0");
    ok &= gam.configSignals.Write("Samples", 1);

    ok &= gam.configSignals.MoveToRoot();

    ok &= gam.SetConfiguredDatabase(gam.configSignals);
    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();
    ok &= gam.Setup();

    uint32 *gamMemoryIn = static_cast<uint32 *>(gam.GetInputSignalsMemory());
    float32 *gamMemoryOut = static_cast<float32 *>(gam.GetOutputSignalsMemory());
    *gamMemoryIn = 0;
    //assign inputs and outputs
    for (uint32 i = 0u; i < 1; i++) {
        gamMemoryOut[i] = 0;
    }
    if (ok) {
        ok = gam.Execute();
    }
    //Compare result against expected vale
    for (uint32 i = 0; i < 1; i++) {
        ok &= (gamMemoryOut[i] == 0.0);
    }
    *gamMemoryIn = 1000000;
    if (ok) {
        ok = gam.Execute();
    }
    //Compare result against expected vale
    float32 aux = 0;
    ok &= (gamMemoryOut[0] == 0.0);
    *gamMemoryIn = 2000000;
    if (ok) {
        ok = gam.Execute();
    }
    //Compare result against expected vale
    for (uint32 i = 0; i < 1; i++) {
        aux = float32(gam.amplitude * sin(2.0 * FastMath::PI * gam.frequency * (*gamMemoryIn / 1e6 + i * (1.0 / 1)) + gam.phase) + gam.offset);
        ok &= IsEqual(gamMemoryOut[i], aux);
    }

    *gamMemoryIn = 3000000;
    if (ok) {
        ok = gam.Execute();
    }
    //Compare result against expected vale

    aux = float32(gam.amplitude * sin(2.0 * FastMath::PI * gam.frequency * (*gamMemoryIn / 1e6 + 0 * (1.0 / 1)) + gam.phase) + gam.offset);
    ok &= IsEqual(gamMemoryOut[0], aux);

    *gamMemoryIn = 4000000;
    if (ok) {
        ok = gam.Execute();
    }
    aux = float32(gam.amplitude * sin(2.0 * FastMath::PI * gam.frequency * (*gamMemoryIn / 1e6 + 0 * (1.0 / 1)) + gam.phase) + gam.offset);
    ok &= IsEqual(gamMemoryOut[0], aux);

    *gamMemoryIn = 5000000;
    if (ok) {
        ok = gam.Execute();
    }

    ok &= (gamMemoryOut[0] == 0.0);

    *gamMemoryIn = 6000000;
    if (ok) {
        ok = gam.Execute();
    }
    ok &= (gamMemoryOut[0] == 0.0);
    return ok;
}

bool WaveformSinGAMTest::TestExecuteWrongInput() {
    bool ok = true;
    using namespace MARTe;
    using namespace FastMath;
    WaveformSinGAMTestHelper gam;
    gam.SetName("Test");
    gam.amplitude = 1000;
    gam.frequency = 1;
    gam.phase = 0;
    gam.offset = 0;
    ok &= gam.config.Write("Amplitude", gam.amplitude);
    ok &= gam.config.Write("Frequency", gam.frequency);
    ok &= gam.config.Write("Phase", gam.phase);
    ok &= gam.config.Write("Offset", gam.offset);
    uint32 dimTrigger = 5;
    float64 *startTrigger = new float64[dimTrigger];
    float64 *stopTrigger = new float64[dimTrigger - 1];
    startTrigger[0] = 1.25;
    stopTrigger[0] = 1.75;
    startTrigger[1] = 2.;
    stopTrigger[1] = 3.25;
    startTrigger[2] = 3.5;
    stopTrigger[2] = 3.75;
    startTrigger[3] = 4.0;
    stopTrigger[3] = 4.25;
    startTrigger[4] = 4.55;
    stopTrigger[4] = 4.8;
    Vector<float64> startTVect(startTrigger, dimTrigger);
    Vector<float64> stopTVect(stopTrigger, dimTrigger);
    //ok &= gam.config.Write("StartTriggerTime", startTVect);
    ok &= gam.config.Write("StartTriggerTime", startTVect);
    ok &= gam.config.Write("StopTriggerTime", stopTVect);

    gam.config.MoveToRoot();
    ok &= gam.Initialise(gam.config);

    uint32 totalByteSizeIn = 4u;
    ok &= gam.configSignals.Write("QualifiedName", "WaveformSinGAMTest");
    ok &= gam.configSignals.CreateAbsolute("Signals.InputSignals");
    ok &= gam.configSignals.CreateRelative("0");
    ok &= gam.configSignals.Write("QualifiedName", "InputSignal1");
    ok &= gam.configSignals.Write("DataSource", "TestDataSource");
    ok &= gam.configSignals.Write("Type", "uint32");
    ok &= gam.configSignals.Write("NumberOfDimensions", 1);
    ok &= gam.configSignals.Write("NumberOfElements", 1);
    ok &= gam.configSignals.Write("ByteSize", totalByteSizeIn);

    ok &= gam.configSignals.MoveToAncestor(1u);
    ok &= gam.configSignals.Write("ByteSize", totalByteSizeIn);

    uint32 totalByteSizeOut = 1 * 4;
    ok &= gam.configSignals.MoveToRoot();
    ok &= gam.configSignals.CreateAbsolute("Signals.OutputSignals");
    ok &= gam.configSignals.CreateRelative("0");
    ok &= gam.configSignals.Write("QualifiedName", "OutputSignal1");
    ok &= gam.configSignals.Write("DataSource", "TestDataSource");
    ok &= gam.configSignals.Write("Type", "float32");
    ok &= gam.configSignals.Write("NumberOfDimensions", 1);

    ok &= gam.configSignals.Write("NumberOfElements", 1);
    ok &= gam.configSignals.Write("ByteSize", totalByteSizeOut);

    ok &= gam.configSignals.MoveToAncestor(1u);
    ok &= gam.configSignals.Write("ByteSize", totalByteSizeOut);

    ok &= gam.configSignals.MoveToRoot();

    ok &= gam.configSignals.CreateAbsolute("Memory.InputSignals");
    ok &= gam.configSignals.CreateRelative("0");
    ok &= gam.configSignals.Write("DataSource", "TestDataSource");
    ok &= gam.configSignals.CreateRelative("Signals");
    ok &= gam.configSignals.CreateRelative("0");
    ok &= gam.configSignals.Write("Samples", 1);

    ok &= gam.configSignals.CreateAbsolute("Memory.OutputSignals");
    ok &= gam.configSignals.CreateRelative("0");
    ok &= gam.configSignals.Write("DataSource", "TestDataSource");
    ok &= gam.configSignals.CreateRelative("Signals");
    ok &= gam.configSignals.CreateRelative("0");
    ok &= gam.configSignals.Write("Samples", 1);

    ok &= gam.configSignals.MoveToRoot();

    ok &= gam.SetConfiguredDatabase(gam.configSignals);
    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();
    ok &= gam.Setup();

    uint32 *gamMemoryIn = static_cast<uint32 *>(gam.GetInputSignalsMemory());
    float32 *gamMemoryOut = static_cast<float32 *>(gam.GetOutputSignalsMemory());
    *gamMemoryIn = 0;
    //assign inputs and outputs
    for (uint32 i = 0u; i < 1; i++) {
        gamMemoryOut[i] = 0;
    }
    if (ok) {
        gam.Execute();
    }
    //Compare result against expected vale
    for (uint32 i = 0; i < 1; i++) {
        ok &= (gamMemoryOut[i] == 0.0);
    }
    *gamMemoryIn = 0;
    if (ok) {
        ok = !gam.Execute();
    }
    return ok;
}

bool WaveformSinGAMTest::TestExecuteWrongInput_2() {
    bool ok = true;
    using namespace MARTe;
    using namespace FastMath;
    WaveformSinGAMTestHelper gam;
    gam.SetName("Test");
    gam.amplitude = 1000;
    gam.frequency = 1;
    gam.phase = 0;
    gam.offset = 0;
    ok &= gam.config.Write("Amplitude", gam.amplitude);
    ok &= gam.config.Write("Frequency", gam.frequency);
    ok &= gam.config.Write("Phase", gam.phase);
    ok &= gam.config.Write("Offset", gam.offset);
    uint32 dimTrigger = 5;
    float64 *startTrigger = new float64[dimTrigger];
    float64 *stopTrigger = new float64[dimTrigger - 1];
    startTrigger[0] = 1.25;
    stopTrigger[0] = 1.75;
    startTrigger[1] = 2.;
    stopTrigger[1] = 3.25;
    startTrigger[2] = 3.5;
    stopTrigger[2] = 3.75;
    startTrigger[3] = 4.0;
    stopTrigger[3] = 4.25;
    startTrigger[4] = 4.55;
    stopTrigger[4] = 4.8;
    Vector<float64> startTVect(startTrigger, dimTrigger);
    Vector<float64> stopTVect(stopTrigger, dimTrigger);
    //ok &= gam.config.Write("StartTriggerTime", startTVect);
    ok &= gam.config.Write("StartTriggerTime", startTVect);
    ok &= gam.config.Write("StopTriggerTime", stopTVect);

    gam.config.MoveToRoot();
    ok &= gam.Initialise(gam.config);

    uint32 totalByteSizeIn = 4u;
    ok &= gam.configSignals.Write("QualifiedName", "WaveformSinGAMTest");
    ok &= gam.configSignals.CreateAbsolute("Signals.InputSignals");
    ok &= gam.configSignals.CreateRelative("0");
    ok &= gam.configSignals.Write("QualifiedName", "InputSignal1");
    ok &= gam.configSignals.Write("DataSource", "TestDataSource");
    ok &= gam.configSignals.Write("Type", "uint32");
    ok &= gam.configSignals.Write("NumberOfDimensions", 1);
    ok &= gam.configSignals.Write("NumberOfElements", 1);
    ok &= gam.configSignals.Write("ByteSize", totalByteSizeIn);

    ok &= gam.configSignals.MoveToAncestor(1u);
    ok &= gam.configSignals.Write("ByteSize", totalByteSizeIn);

    uint32 totalByteSizeOut = 1 * 4;
    ok &= gam.configSignals.MoveToRoot();
    ok &= gam.configSignals.CreateAbsolute("Signals.OutputSignals");
    ok &= gam.configSignals.CreateRelative("0");
    ok &= gam.configSignals.Write("QualifiedName", "OutputSignal1");
    ok &= gam.configSignals.Write("DataSource", "TestDataSource");
    ok &= gam.configSignals.Write("Type", "float32");
    ok &= gam.configSignals.Write("NumberOfDimensions", 1);

    ok &= gam.configSignals.Write("NumberOfElements", 1);
    ok &= gam.configSignals.Write("ByteSize", totalByteSizeOut);

    ok &= gam.configSignals.MoveToAncestor(1u);
    ok &= gam.configSignals.Write("ByteSize", totalByteSizeOut);

    ok &= gam.configSignals.MoveToRoot();

    ok &= gam.configSignals.CreateAbsolute("Memory.InputSignals");
    ok &= gam.configSignals.CreateRelative("0");
    ok &= gam.configSignals.Write("DataSource", "TestDataSource");
    ok &= gam.configSignals.CreateRelative("Signals");
    ok &= gam.configSignals.CreateRelative("0");
    ok &= gam.configSignals.Write("Samples", 1);

    ok &= gam.configSignals.CreateAbsolute("Memory.OutputSignals");
    ok &= gam.configSignals.CreateRelative("0");
    ok &= gam.configSignals.Write("DataSource", "TestDataSource");
    ok &= gam.configSignals.CreateRelative("Signals");
    ok &= gam.configSignals.CreateRelative("0");
    ok &= gam.configSignals.Write("Samples", 1);

    ok &= gam.configSignals.MoveToRoot();

    ok &= gam.SetConfiguredDatabase(gam.configSignals);
    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();
    ok &= gam.Setup();

    uint32 *gamMemoryIn = static_cast<uint32 *>(gam.GetInputSignalsMemory());
    float32 *gamMemoryOut = static_cast<float32 *>(gam.GetOutputSignalsMemory());
    *gamMemoryIn = 0;
    //assign inputs and outputs
    for (uint32 i = 0u; i < 1; i++) {
        gamMemoryOut[i] = 0;
    }
    if (ok) {
        gam.Execute();
    }
    //Compare result against expected vale
    for (uint32 i = 0; i < 1; i++) {
        ok &= (gamMemoryOut[i] == 0.0);
    }
    *gamMemoryIn = 100000;
    if (ok) {
        ok = gam.Execute();
    }
    //Compare result against expected vale
    ok &= (gamMemoryOut[0] == 0.0);
    *gamMemoryIn = 100000;
    if (ok) {
        ok = !gam.Execute();
    }
    return ok;
}

bool WaveformSinGAMTest::TestExecuteNegativeInput() {
    bool ok = true;
    using namespace MARTe;
    using namespace FastMath;
    WaveformSinGAMTestHelper gam;
    gam.SetName("Test");
    gam.amplitude = 1000;
    gam.frequency = 1;
    gam.phase = 0;
    gam.offset = 0;
    ok &= gam.config.Write("Amplitude", gam.amplitude);
    ok &= gam.config.Write("Frequency", gam.frequency);
    ok &= gam.config.Write("Phase", gam.phase);
    ok &= gam.config.Write("Offset", gam.offset);
    uint32 dimTrigger = 5;
    float64 *startTrigger = new float64[dimTrigger];
    float64 *stopTrigger = new float64[dimTrigger - 1];
    startTrigger[0] = 1.25;
    stopTrigger[0] = 1.75;
    startTrigger[1] = 2.;
    stopTrigger[1] = 3.25;
    startTrigger[2] = 3.5;
    stopTrigger[2] = 3.75;
    startTrigger[3] = 4.0;
    stopTrigger[3] = 4.25;
    startTrigger[4] = 4.55;
    stopTrigger[4] = 4.8;
    Vector<float64> startTVect(startTrigger, dimTrigger);
    Vector<float64> stopTVect(stopTrigger, dimTrigger);
    //ok &= gam.config.Write("StartTriggerTime", startTVect);
    ok &= gam.config.Write("StartTriggerTime", startTVect);
    ok &= gam.config.Write("StopTriggerTime", stopTVect);

    gam.config.MoveToRoot();
    ok &= gam.Initialise(gam.config);

    uint32 totalByteSizeIn = 4u;
    ok &= gam.configSignals.Write("QualifiedName", "WaveformSinGAMTest");
    ok &= gam.configSignals.CreateAbsolute("Signals.InputSignals");
    ok &= gam.configSignals.CreateRelative("0");
    ok &= gam.configSignals.Write("QualifiedName", "InputSignal1");
    ok &= gam.configSignals.Write("DataSource", "TestDataSource");
    ok &= gam.configSignals.Write("Type", "int32");
    ok &= gam.configSignals.Write("NumberOfDimensions", 1);
    ok &= gam.configSignals.Write("NumberOfElements", 1);
    ok &= gam.configSignals.Write("ByteSize", totalByteSizeIn);

    ok &= gam.configSignals.MoveToAncestor(1u);
    ok &= gam.configSignals.Write("ByteSize", totalByteSizeIn);

    uint32 totalByteSizeOut = 1 * 4;
    ok &= gam.configSignals.MoveToRoot();
    ok &= gam.configSignals.CreateAbsolute("Signals.OutputSignals");
    ok &= gam.configSignals.CreateRelative("0");
    ok &= gam.configSignals.Write("QualifiedName", "OutputSignal1");
    ok &= gam.configSignals.Write("DataSource", "TestDataSource");
    ok &= gam.configSignals.Write("Type", "float32");
    ok &= gam.configSignals.Write("NumberOfDimensions", 1);

    ok &= gam.configSignals.Write("NumberOfElements", 1);
    ok &= gam.configSignals.Write("ByteSize", totalByteSizeOut);

    ok &= gam.configSignals.MoveToAncestor(1u);
    ok &= gam.configSignals.Write("ByteSize", totalByteSizeOut);

    ok &= gam.configSignals.MoveToRoot();

    ok &= gam.configSignals.CreateAbsolute("Memory.InputSignals");
    ok &= gam.configSignals.CreateRelative("0");
    ok &= gam.configSignals.Write("DataSource", "TestDataSource");
    ok &= gam.configSignals.CreateRelative("Signals");
    ok &= gam.configSignals.CreateRelative("0");
    ok &= gam.configSignals.Write("Samples", 1);

    ok &= gam.configSignals.CreateAbsolute("Memory.OutputSignals");
    ok &= gam.configSignals.CreateRelative("0");
    ok &= gam.configSignals.Write("DataSource", "TestDataSource");
    ok &= gam.configSignals.CreateRelative("Signals");
    ok &= gam.configSignals.CreateRelative("0");
    ok &= gam.configSignals.Write("Samples", 1);

    ok &= gam.configSignals.MoveToRoot();

    ok &= gam.SetConfiguredDatabase(gam.configSignals);
    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();
    ok &= gam.Setup();

    int32 *gamMemoryIn = static_cast<int32 *>(gam.GetInputSignalsMemory());
    float32 *gamMemoryOut = static_cast<float32 *>(gam.GetOutputSignalsMemory());
    *gamMemoryIn = 0;
    //assign inputs and outputs
    for (uint32 i = 0u; i < 1; i++) {
        gamMemoryOut[i] = 0;
    }
    if (ok) {
        gam.Execute();
    }
    //Compare result against expected vale
    for (uint32 i = 0; i < 1; i++) {
        ok &= (gamMemoryOut[i] == 0.0);
    }
    *gamMemoryIn = 100000;
    if (ok) {
        ok = gam.Execute();
    }
    //Compare result against expected vale
    ok &= (gamMemoryOut[0] == 0.0);
    *gamMemoryIn = -100000;
    if (ok) {
        ok = !gam.Execute();
    }
    return ok;
}

bool WaveformSinGAMTest::TestExecuteNegativeInput_2() {
    bool ok = true;
    using namespace MARTe;
    using namespace FastMath;
    WaveformSinGAMTestHelper gam;
    gam.SetName("Test");
    gam.amplitude = 1000;
    gam.frequency = 1;
    gam.phase = 0;
    gam.offset = 0;
    ok &= gam.config.Write("Amplitude", gam.amplitude);
    ok &= gam.config.Write("Frequency", gam.frequency);
    ok &= gam.config.Write("Phase", gam.phase);
    ok &= gam.config.Write("Offset", gam.offset);
    uint32 dimTrigger = 5;
    float64 *startTrigger = new float64[dimTrigger];
    float64 *stopTrigger = new float64[dimTrigger - 1];
    startTrigger[0] = 1.25;
    stopTrigger[0] = 1.75;
    startTrigger[1] = 2.;
    stopTrigger[1] = 3.25;
    startTrigger[2] = 3.5;
    stopTrigger[2] = 3.75;
    startTrigger[3] = 4.0;
    stopTrigger[3] = 4.25;
    startTrigger[4] = 4.55;
    stopTrigger[4] = 4.8;
    Vector<float64> startTVect(startTrigger, dimTrigger);
    Vector<float64> stopTVect(stopTrigger, dimTrigger);
    //ok &= gam.config.Write("StartTriggerTime", startTVect);
    ok &= gam.config.Write("StartTriggerTime", startTVect);
    ok &= gam.config.Write("StopTriggerTime", stopTVect);

    gam.config.MoveToRoot();
    ok &= gam.Initialise(gam.config);

    uint32 totalByteSizeIn = 4u;
    ok &= gam.configSignals.Write("QualifiedName", "WaveformSinGAMTest");
    ok &= gam.configSignals.CreateAbsolute("Signals.InputSignals");
    ok &= gam.configSignals.CreateRelative("0");
    ok &= gam.configSignals.Write("QualifiedName", "InputSignal1");
    ok &= gam.configSignals.Write("DataSource", "TestDataSource");
    ok &= gam.configSignals.Write("Type", "int64");
    ok &= gam.configSignals.Write("NumberOfDimensions", 1);
    ok &= gam.configSignals.Write("NumberOfElements", 1);
    ok &= gam.configSignals.Write("ByteSize", totalByteSizeIn);

    ok &= gam.configSignals.MoveToAncestor(1u);
    ok &= gam.configSignals.Write("ByteSize", totalByteSizeIn);

    uint32 totalByteSizeOut = 1 * 4;
    ok &= gam.configSignals.MoveToRoot();
    ok &= gam.configSignals.CreateAbsolute("Signals.OutputSignals");
    ok &= gam.configSignals.CreateRelative("0");
    ok &= gam.configSignals.Write("QualifiedName", "OutputSignal1");
    ok &= gam.configSignals.Write("DataSource", "TestDataSource");
    ok &= gam.configSignals.Write("Type", "float32");
    ok &= gam.configSignals.Write("NumberOfDimensions", 1);

    ok &= gam.configSignals.Write("NumberOfElements", 1);
    ok &= gam.configSignals.Write("ByteSize", totalByteSizeOut);

    ok &= gam.configSignals.MoveToAncestor(1u);
    ok &= gam.configSignals.Write("ByteSize", totalByteSizeOut);

    ok &= gam.configSignals.MoveToRoot();

    ok &= gam.configSignals.CreateAbsolute("Memory.InputSignals");
    ok &= gam.configSignals.CreateRelative("0");
    ok &= gam.configSignals.Write("DataSource", "TestDataSource");
    ok &= gam.configSignals.CreateRelative("Signals");
    ok &= gam.configSignals.CreateRelative("0");
    ok &= gam.configSignals.Write("Samples", 1);

    ok &= gam.configSignals.CreateAbsolute("Memory.OutputSignals");
    ok &= gam.configSignals.CreateRelative("0");
    ok &= gam.configSignals.Write("DataSource", "TestDataSource");
    ok &= gam.configSignals.CreateRelative("Signals");
    ok &= gam.configSignals.CreateRelative("0");
    ok &= gam.configSignals.Write("Samples", 1);

    ok &= gam.configSignals.MoveToRoot();

    ok &= gam.SetConfiguredDatabase(gam.configSignals);
    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();
    ok &= gam.Setup();

    int64 *gamMemoryIn = static_cast<int64 *>(gam.GetInputSignalsMemory());
    float32 *gamMemoryOut = static_cast<float32 *>(gam.GetOutputSignalsMemory());
    *gamMemoryIn = 0;
    //assign inputs and outputs
    for (uint32 i = 0u; i < 1; i++) {
        gamMemoryOut[i] = 0;
    }
    if (ok) {
        gam.Execute();
    }
    //Compare result against expected vale
    for (uint32 i = 0; i < 1; i++) {
        ok &= (gamMemoryOut[i] == 0.0);
    }
    *gamMemoryIn = 100000;
    if (ok) {
        ok = gam.Execute();
    }
    //Compare result against expected vale
    ok &= (gamMemoryOut[0] == 0.0);
    *gamMemoryIn = -100000;
    if (ok) {
        ok = !gam.Execute();
    }
    return ok;
}
