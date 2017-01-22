/**
 * @file ConversionGAMTest.h
 * @brief Header file for class ConversionGAMTest
 * @date 21/01/2017
 * @author Andre Neto
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

 * @details This header file contains the declaration of the class ConversionGAMTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef CONVERSIONGAMTEST_H_
#define CONVERSIONGAMTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "ConversionGAM.h"
#include "DataSourceI.h"
#include "GAMScheduler.h"
#include "MemoryOperationsHelper.h"
#include "MemoryMapInputBroker.h"
#include "MemoryMapOutputBroker.h"
#include "ObjectRegistryDatabase.h"
#include "RealTimeApplication.h"
#include "StandardParser.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
/**
 * @brief Tests the ConversionGAM public methods.
 */
class ConversionGAMTest {
public:
    /**
     * @brief Tests the constructor.
     */
    bool TestConstructor();

    /**
     * @brief Tests the Setup method. This is already performed in the Execute method.
     */
    bool TestSetup();

    /**
     * @brief Tests the Setup method with an invalid output signal type.
     */
    template<typename baseType>
    bool TestSetup_False_InvalidOutputSignalType();

    /**
     * @brief Tests the Setup method with an invalid input signal type.
     */
    bool TestSetup_False_InvalidInputSignalType();

    /**
     * @brief Tests the Setup method with a number of inputs different from the number of outputs.
     */
    bool TestSetup_False_InvalidNumberOfInputsOutputs();

    /**
     * @brief Tests the Setup method with a number a mismatch of the input signals number of elements.
     */
    bool TestSetup_False_InvalidInputElementsMismatch();

    /**
     * @brief Tests the Setup method with a number a mismatch of the output signals number of elements.
     */
    bool TestSetup_False_InvalidOutputElementsMismatch();

    /**
     * @brief Tests the Setup method with a number a mismatch of the input signals number of samples.
     */
    bool TestSetup_False_InvalidInputSamplesMismatch();

    /**
     * @brief Tests the Setup method with a number a mismatch of the output signals number of samples.
     */
    bool TestSetup_False_InvalidOutputSamplesMismatch();

    /**
     * @brief Tests the Execute method for all the basic types.
     */
    template<typename baseType>
    bool TestExecute(baseType typeValue, bool setGain = false, MARTe::float32 gainValue = 1.0);

private:
    /**
     * Starts a MARTe application that uses this GAM instance.
     */
    bool TestIntegratedInApplication(const MARTe::char8 * const config, bool destroy = true);
};

/**
 * Gives access to the ConversionGAM memory.
 */
class ConversionGAMTestHelper: public MARTe::ConversionGAM {
public:
    CLASS_REGISTER_DECLARATION()

ConversionGAMTestHelper    () : MARTe::ConversionGAM() {
    }

    virtual ~ConversionGAMTestHelper() {

    }

    void *GetInputSignalsMemory() {
        return ConversionGAM::GetInputSignalsMemory();
    }

    void *GetOutputSignalsMemory() {
        return ConversionGAM::GetOutputSignalsMemory();
    }

    void *GetInputSignalMemory(MARTe::uint32 signalIdx) {
        return ConversionGAM::GetInputSignalMemory(signalIdx);
    }

    void *GetOutputSignalMemory(MARTe::uint32 signalIdx) const {
        return ConversionGAM::GetOutputSignalMemory(signalIdx);
    }
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/
//From basic type to all types (with elements and samples)
static const MARTe::char8 * const configFromBasicTypeTemplate = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAM1 = {"
        "            Class = ConversionGAMTestHelper"
        "            InputSignals = {"
        "               Signal1 = {"
        "                   DataSource = Drv1"
        "                   Type = uint8"
        "               }"
        "               Signal2 = {"
        "                   DataSource = Drv1"
        "                   Type = uint8"
        "               }"
        "               Signal3 = {"
        "                   DataSource = Drv1"
        "                   Type = uint8"
        "               }"
        "               Signal4 = {"
        "                   DataSource = Drv1"
        "                   Type = uint8"
        "               }"
        "               Signal5 = {"
        "                   DataSource = Drv1"
        "                   Type = uint8"
        "               }"
        "               Signal6 = {"
        "                   DataSource = Drv1"
        "                   Type = uint8"
        "               }"
        "               Signal7 = {"
        "                   DataSource = Drv1"
        "                   Type = uint8"
        "               }"
        "               Signal8 = {"
        "                   DataSource = Drv1"
        "                   Type = uint8"
        "               }"
        "               Signal9 = {"
        "                   DataSource = Drv1"
        "                   Type = uint8"
        "               }"
        "               Signal10 = {"
        "                   DataSource = Drv1"
        "                   Type = uint8"
        "               }"
        "               Signal11 = {"
        "                   DataSource = Drv1"
        "                   Type = uint8"
        "                   NumberOfElements = 10"
        "               }"
        "               Signal12 = {"
        "                   DataSource = Drv1"
        "                   Type = uint8"
        "                   NumberOfElements = 10"
        "               }"
        "               Signal13 = {"
        "                   DataSource = Drv1"
        "                   Type = uint8"
        "                   NumberOfElements = 10"
        "               }"
        "               Signal14 = {"
        "                   DataSource = Drv1"
        "                   Type = uint8"
        "                   NumberOfElements = 10"
        "               }"
        "               Signal15 = {"
        "                   DataSource = Drv1"
        "                   Type = uint8"
        "                   NumberOfElements = 10"
        "               }"
        "               Signal16 = {"
        "                   DataSource = Drv1"
        "                   Type = uint8"
        "                   NumberOfElements = 10"
        "               }"
        "               Signal17 = {"
        "                   DataSource = Drv1"
        "                   Type = uint8"
        "                   NumberOfElements = 10"
        "               }"
        "               Signal18 = {"
        "                   DataSource = Drv1"
        "                   Type = uint8"
        "                   NumberOfElements = 10"
        "               }"
        "               Signal19 = {"
        "                   DataSource = Drv1"
        "                   Type = uint8"
        "                   NumberOfElements = 10"
        "               }"
        "               Signal20 = {"
        "                   DataSource = Drv1"
        "                   Type = uint8"
        "                   NumberOfElements = 10"
        "               }"
        "               Signal21 = {"
        "                   DataSource = Drv1"
        "                   Type = uint8"
        "                   NumberOfElements = 10"
        "                   Samples = 2"
        "               }"
        "               Signal22 = {"
        "                   DataSource = Drv1"
        "                   Type = uint8"
        "                   NumberOfElements = 10"
        "                   Samples = 2"
        "               }"
        "               Signal23 = {"
        "                   DataSource = Drv1"
        "                   Type = uint8"
        "                   NumberOfElements = 10"
        "                   Samples = 2"
        "               }"
        "               Signal24 = {"
        "                   DataSource = Drv1"
        "                   Type = uint8"
        "                   NumberOfElements = 10"
        "                   Samples = 2"
        "               }"
        "               Signal25 = {"
        "                   DataSource = Drv1"
        "                   Type = uint8"
        "                   NumberOfElements = 10"
        "                   Samples = 2"
        "               }"
        "               Signal26 = {"
        "                   DataSource = Drv1"
        "                   Type = uint8"
        "                   NumberOfElements = 10"
        "                   Samples = 2"
        "               }"
        "               Signal27 = {"
        "                   DataSource = Drv1"
        "                   Type = uint8"
        "                   NumberOfElements = 10"
        "                   Samples = 2"
        "               }"
        "               Signal28 = {"
        "                   DataSource = Drv1"
        "                   Type = uint8"
        "                   NumberOfElements = 10"
        "                   Samples = 2"
        "               }"
        "               Signal29 = {"
        "                   DataSource = Drv1"
        "                   Type = uint8"
        "                   NumberOfElements = 10"
        "                   Samples = 2"
        "               }"
        "               Signal30 = {"
        "                   DataSource = Drv1"
        "                   Type = uint8"
        "                   NumberOfElements = 10"
        "                   Samples = 2"
        "               }"
        "            }"
        "            OutputSignals = {"
        "               Signal1 = {"
        "                   DataSource = DDB1"
        "                   Type = uint8"
        "               }"
        "               Signal2 = {"
        "                   DataSource = DDB1"
        "                   Type = int8"
        "               }"
        "               Signal3 = {"
        "                   DataSource = DDB1"
        "                   Type = uint16"
        "               }"
        "               Signal4 = {"
        "                   DataSource = DDB1"
        "                   Type = int16"
        "               }"
        "               Signal5 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "               }"
        "               Signal6 = {"
        "                   DataSource = DDB1"
        "                   Type = int32"
        "               }"
        "               Signal7 = {"
        "                   DataSource = DDB1"
        "                   Type = uint64"
        "               }"
        "               Signal8 = {"
        "                   DataSource = DDB1"
        "                   Type = int64"
        "               }"
        "               Signal9 = {"
        "                   DataSource = DDB1"
        "                   Type = float32"
        "               }"
        "               Signal10 = {"
        "                   DataSource = DDB1"
        "                   Type = float64"
        "               }"
        "               Signal11 = {"
        "                   DataSource = DDB1"
        "                   Type = uint8"
        "                   NumberOfElements = 10"
        "               }"
        "               Signal12 = {"
        "                   DataSource = DDB1"
        "                   Type = int8"
        "                   NumberOfElements = 10"
        "               }"
        "               Signal13 = {"
        "                   DataSource = DDB1"
        "                   Type = uint16"
        "                   NumberOfElements = 10"
        "               }"
        "               Signal14 = {"
        "                   DataSource = DDB1"
        "                   Type = int16"
        "                   NumberOfElements = 10"
        "               }"
        "               Signal15 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "                   NumberOfElements = 10"
        "               }"
        "               Signal16 = {"
        "                   DataSource = DDB1"
        "                   Type = int32"
        "                   NumberOfElements = 10"
        "               }"
        "               Signal17 = {"
        "                   DataSource = DDB1"
        "                   Type = uint64"
        "                   NumberOfElements = 10"
        "               }"
        "               Signal18 = {"
        "                   DataSource = DDB1"
        "                   Type = int64"
        "                   NumberOfElements = 10"
        "               }"
        "               Signal19 = {"
        "                   DataSource = DDB1"
        "                   Type = float32"
        "                   NumberOfElements = 10"
        "               }"
        "               Signal20 = {"
        "                   DataSource = DDB1"
        "                   Type = float64"
        "                   NumberOfElements = 10"
        "               }"
        "               Signal21 = {"
        "                   DataSource = DDB1"
        "                   Type = uint8"
        "                   NumberOfElements = 20"
        "               }"
        "               Signal22 = {"
        "                   DataSource = DDB1"
        "                   Type = int8"
        "                   NumberOfElements = 20"
        "               }"
        "               Signal23 = {"
        "                   DataSource = DDB1"
        "                   Type = uint16"
        "                   NumberOfElements = 20"
        "               }"
        "               Signal24 = {"
        "                   DataSource = DDB1"
        "                   Type = int16"
        "                   NumberOfElements = 20"
        "               }"
        "               Signal25 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "                   NumberOfElements = 20"
        "               }"
        "               Signal26 = {"
        "                   DataSource = DDB1"
        "                   Type = int32"
        "                   NumberOfElements = 20"
        "               }"
        "               Signal27 = {"
        "                   DataSource = DDB1"
        "                   Type = uint64"
        "                   NumberOfElements = 20"
        "               }"
        "               Signal28 = {"
        "                   DataSource = DDB1"
        "                   Type = int64"
        "                   NumberOfElements = 20"
        "               }"
        "               Signal29 = {"
        "                   DataSource = DDB1"
        "                   Type = float32"
        "                   NumberOfElements = 20"
        "               }"
        "               Signal30 = {"
        "                   DataSource = DDB1"
        "                   Type = float64"
        "                   NumberOfElements = 20"
        "               }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +DDB1 = {"
        "            Class = GAMDataSource"
        "        }"
        "        +Timings = {"
        "            Class = TimingDataSource"
        "        }"
        "        +Drv1 = {"
        "            Class = ConversionGAMDataSourceHelper"
        "        }"
        "    }"
        "    +States = {"
        "        Class = ReferenceContainer"
        "        +State1 = {"
        "            Class = RealTimeState"
        "            +Threads = {"
        "                Class = ReferenceContainer"
        "                +Thread1 = {"
        "                    Class = RealTimeThread"
        "                    Functions = {GAM1}"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Scheduler = {"
        "        Class = GAMScheduler"
        "        TimingDataSource = Timings"
        "    }"
        "}";

template<typename baseType>
bool ConversionGAMTest::TestSetup_False_InvalidOutputSignalType() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = configFromBasicTypeTemplate;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    //Parse the template configuration file
    bool ok = parser.Parse();

    //Patch it with the baseType to be tested
    baseType typeToDetect;
    AnyType typeDetector(typeToDetect);
    StreamString typeName = TypeDescriptor::GetTypeNameFromTypeDescriptor(typeDetector.GetTypeDescriptor());
    if (ok) {
        ok = cdb.MoveAbsolute("$Test.+Functions.+GAM1.InputSignals");
    }
    if (ok) {
        uint32 i;
        StreamString signalName;
        const uint32 numberOfSignals = 30u;
        for (i = 0; (i < numberOfSignals) && (ok); i++) {
            signalName = "";
            signalName.Printf("Signal%d", (i + 1));
            ok = cdb.MoveRelative(signalName.Buffer());
            if (ok) {
                cdb.Delete("Type");
            }
            if (ok) {
                cdb.Write("Type", typeName.Buffer());
            }
            if (ok) {
                cdb.MoveToAncestor(1u);
            }
        }
    }

    if (ok) {
        ok = cdb.MoveAbsolute("$Test.+Functions.+GAM1.OutputSignals");
    }
    if (ok) {
        StreamString signalName = "Signal1";
        ok = cdb.MoveRelative(signalName.Buffer());
        if (ok) {
            cdb.Delete("Type");
        }
        if (ok) {
            cdb.Write("Type", "string");
        }
        if (ok) {
            cdb.MoveToAncestor(1u);
        }
    }

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();

    //Initialise the application
    if (ok) {
        cdb.MoveToRoot();
        god->Purge();
        ok = god->Initialise(cdb);
    }
    ReferenceT<RealTimeApplication> application;
    if (ok) {
        application = god->Find("Test");
        ok = application.IsValid();
    }
    if (ok) {
        ok = !application->ConfigureApplication();
    }

    god->Purge();
    return ok;
}

template<typename baseType>
bool ConversionGAMTest::TestExecute(baseType typeValue, bool setGain, MARTe::float32 gainValue) {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = configFromBasicTypeTemplate;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    const uint32 numberOfSignals = 30u;
    //Parse the template configuration file
    bool ok = parser.Parse();

    //Patch it with the type to be tested
    baseType typeToDetect;
    AnyType typeDetector(typeToDetect);
    StreamString typeName = TypeDescriptor::GetTypeNameFromTypeDescriptor(typeDetector.GetTypeDescriptor());
    if (ok) {
        ok = cdb.MoveAbsolute("$Test.+Functions.+GAM1.InputSignals");
    }
    if (ok) {
        uint32 i;
        StreamString signalName;
        const uint32 numberOfSignals = 30u;
        for (i = 0; (i < numberOfSignals) && (ok); i++) {
            signalName = "";
            signalName.Printf("Signal%d", (i + 1));
            ok = cdb.MoveRelative(signalName.Buffer());
            if (ok) {
                cdb.Delete("Type");
            }
            if (ok) {
                cdb.Write("Type", typeName.Buffer());
            }
            if (ok) {
                cdb.MoveToAncestor(1u);
            }
        }
    }
    if (setGain) {
        if (ok) {
            ok = cdb.MoveAbsolute("$Test.+Functions.+GAM1.OutputSignals");
        }
        if (ok) {
            uint32 i;
            StreamString signalName;
            for (i = 0; (i < numberOfSignals) && (ok); i++) {
                signalName = "";
                signalName.Printf("Signal%d", (i + 1));
                ok = cdb.MoveRelative(signalName.Buffer());
                cdb.Write("Gain", gainValue);
                if (ok) {
                    cdb.MoveToAncestor(1u);
                }
            }
        }
    }
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();

    //Initialise the application
    if (ok) {
        cdb.MoveToRoot();
        god->Purge();
        ok = god->Initialise(cdb);
    }
    ReferenceT<RealTimeApplication> application;
    if (ok) {
        application = god->Find("Test");
        ok = application.IsValid();
    }
    if (ok) {
        ok = application->ConfigureApplication();
    }

    ReferenceT<ConversionGAMTestHelper> gam = god->Find("Test.Functions.GAM1");
    if (ok) {
        ok = gam.IsValid();
    }

    //Execute the GAM and check that the values match
    baseType *inputSignals;
    uint8 *uint8Signal;
    int8 *int8Signal;
    uint16 *uint16Signal;
    int16 *int16Signal;
    uint32 *uint32Signal;
    int32 *int32Signal;
    uint64 *uint64Signal;
    int64 *int64Signal;
    float32 *float32Signal;
    float64 *float64Signal;
    uint32 i;
    if (ok) {
        inputSignals = static_cast<baseType *>(gam->GetInputSignalsMemory());
        uint32 c = 0u;
        for (i = 0; i < gam->GetNumberOfInputSignals(); i++) {
            uint32 numberOfSamples;
            gam->GetSignalNumberOfSamples(InputSignals, i, numberOfSamples);
            uint32 s;
            for (s = 0; s < numberOfSamples; s++) {
                uint32 numberOfElements;
                gam->GetSignalNumberOfElements(InputSignals, i, numberOfElements);
                uint32 e;
                for (e = 0; e < numberOfElements; e++) {
                    inputSignals[c++] = typeValue;
                }
            }
        }
    }
    if (ok) {
        ok = gam->Execute();
    }
    if (ok) {
        uint32 numberOfSamples;
        uint32 s;
        uint32 z;

        i = 0u;
        //z is to test the version with one element, many elements one sample and many elements many samples
        for (z = 0; z < numberOfSignals; z += 10) {
            uint32 signalIdx = i + z;
            uint8Signal = reinterpret_cast<uint8 *>(gam->GetOutputSignalMemory(signalIdx));
            gam->GetSignalNumberOfSamples(OutputSignals, signalIdx, numberOfSamples);
            uint32 c = 0u;
            for (s = 0; s < numberOfSamples; s++) {
                uint32 numberOfElements;
                gam->GetSignalNumberOfElements(OutputSignals, signalIdx, numberOfElements);
                uint32 e;
                for (e = 0; e < numberOfElements; e++) {
                    ok &= (uint8Signal[c++]
                            == static_cast<uint8>((static_cast<uint8>(gainValue) * static_cast<uint8>(typeValue))));
                }
            }
        }

        i++;
        for (z = 0; z < numberOfSignals; z += 10) {
            uint32 signalIdx = i + z;
            int8Signal = reinterpret_cast<int8 *>(gam->GetOutputSignalMemory(signalIdx));
            gam->GetSignalNumberOfSamples(OutputSignals, signalIdx, numberOfSamples);
            uint32 c = 0u;
            for (s = 0; s < numberOfSamples; s++) {
                uint32 numberOfElements;
                gam->GetSignalNumberOfElements(OutputSignals, signalIdx, numberOfElements);
                uint32 e;
                for (e = 0; e < numberOfElements; e++) {
                    ok &= (int8Signal[c++]
                            == static_cast<int8>((static_cast<int8>(gainValue) * static_cast<int8>(typeValue))));
                }
            }
        }
        i++;
        for (z = 0; z < numberOfSignals; z += 10) {
            uint32 signalIdx = i + z;
            uint16Signal = reinterpret_cast<uint16 *>(gam->GetOutputSignalMemory(signalIdx));
            gam->GetSignalNumberOfSamples(OutputSignals, signalIdx, numberOfSamples);
            uint32 c = 0u;
            for (s = 0; s < numberOfSamples; s++) {
                uint32 numberOfElements;
                gam->GetSignalNumberOfElements(OutputSignals, signalIdx, numberOfElements);
                uint32 e;
                for (e = 0; e < numberOfElements; e++) {
                    ok &= (uint16Signal[c++]
                            == static_cast<uint16>((static_cast<uint16>(gainValue) * static_cast<uint16>(typeValue))));
                }
            }
        }
        i++;
        for (z = 0; z < numberOfSignals; z += 10) {
            uint32 signalIdx = i + z;
            int16Signal = reinterpret_cast<int16 *>(gam->GetOutputSignalMemory(signalIdx));
            gam->GetSignalNumberOfSamples(OutputSignals, signalIdx, numberOfSamples);
            uint32 c = 0u;
            for (s = 0; s < numberOfSamples; s++) {
                uint32 numberOfElements;
                gam->GetSignalNumberOfElements(OutputSignals, signalIdx, numberOfElements);
                uint32 e;
                for (e = 0; e < numberOfElements; e++) {
                    ok &= (int16Signal[c++]
                            == static_cast<int16>((static_cast<int16>(gainValue) * static_cast<int16>(typeValue))));
                }
            }
        }
        i++;
        for (z = 0; z < numberOfSignals; z += 10) {
            uint32 signalIdx = i + z;
            uint32Signal = reinterpret_cast<uint32 *>(gam->GetOutputSignalMemory(signalIdx));
            gam->GetSignalNumberOfSamples(OutputSignals, signalIdx, numberOfSamples);
            uint32 c = 0u;
            for (s = 0; s < numberOfSamples; s++) {
                uint32 numberOfElements;
                gam->GetSignalNumberOfElements(OutputSignals, signalIdx, numberOfElements);
                uint32 e;
                for (e = 0; e < numberOfElements; e++) {
                    ok &= (uint32Signal[c++]
                            == static_cast<uint32>((static_cast<uint32>(gainValue) * static_cast<uint32>(typeValue))));
                }
            }
        }
        i++;
        for (z = 0; z < numberOfSignals; z += 10) {
            uint32 signalIdx = i + z;
            int32Signal = reinterpret_cast<int32 *>(gam->GetOutputSignalMemory(signalIdx));
            gam->GetSignalNumberOfSamples(OutputSignals, signalIdx, numberOfSamples);
            uint32 c = 0u;
            for (s = 0; s < numberOfSamples; s++) {
                uint32 numberOfElements;
                gam->GetSignalNumberOfElements(OutputSignals, signalIdx, numberOfElements);
                uint32 e;
                for (e = 0; e < numberOfElements; e++) {
                    ok &= (int32Signal[c++]
                            == static_cast<int32>((static_cast<int32>(gainValue) * static_cast<int32>(typeValue))));
                }
            }
        }
        i++;
        for (z = 0; z < numberOfSignals; z += 10) {
            uint32 signalIdx = i + z;
            uint64Signal = reinterpret_cast<uint64 *>(gam->GetOutputSignalMemory(signalIdx));
            gam->GetSignalNumberOfSamples(OutputSignals, signalIdx, numberOfSamples);
            uint32 c = 0u;
            for (s = 0; s < numberOfSamples; s++) {
                uint32 numberOfElements;
                gam->GetSignalNumberOfElements(OutputSignals, signalIdx, numberOfElements);
                uint32 e;
                for (e = 0; e < numberOfElements; e++) {
                    ok &= (uint64Signal[c++]
                            == static_cast<uint64>((static_cast<uint64>(gainValue) * static_cast<uint64>(typeValue))));
                }
            }
        }
        i++;
        for (z = 0; z < numberOfSignals; z += 10) {
            uint32 signalIdx = i + z;
            int64Signal = reinterpret_cast<int64 *>(gam->GetOutputSignalMemory(signalIdx));
            gam->GetSignalNumberOfSamples(OutputSignals, signalIdx, numberOfSamples);
            uint32 c = 0u;
            for (s = 0; s < numberOfSamples; s++) {
                uint32 numberOfElements;
                gam->GetSignalNumberOfElements(OutputSignals, signalIdx, numberOfElements);
                uint32 e;
                for (e = 0; e < numberOfElements; e++) {
                    ok &= (int64Signal[c++]
                            == static_cast<int64>((static_cast<int64>(gainValue) * static_cast<int64>(typeValue))));
                }
            }
        }
        i++;
        for (z = 0; z < numberOfSignals; z += 10) {
            uint32 signalIdx = i + z;
            float32Signal = reinterpret_cast<float32 *>(gam->GetOutputSignalMemory(signalIdx));
            gam->GetSignalNumberOfSamples(OutputSignals, signalIdx, numberOfSamples);
            uint32 c = 0u;
            for (s = 0; s < numberOfSamples; s++) {
                uint32 numberOfElements;
                gam->GetSignalNumberOfElements(OutputSignals, signalIdx, numberOfElements);
                uint32 e;
                for (e = 0; e < numberOfElements; e++) {
                    ok &=
                            (float32Signal[c++]
                                    == static_cast<float32>((static_cast<float32>(gainValue)
                                            * static_cast<float32>(typeValue))));
                }
            }
        }
        i++;
        for (z = 0; z < numberOfSignals; z += 10) {
            uint32 signalIdx = i + z;
            float64Signal = reinterpret_cast<float64 *>(gam->GetOutputSignalMemory(signalIdx));
            gam->GetSignalNumberOfSamples(OutputSignals, signalIdx, numberOfSamples);
            uint32 c = 0u;
            for (s = 0; s < numberOfSamples; s++) {
                uint32 numberOfElements;
                gam->GetSignalNumberOfElements(OutputSignals, signalIdx, numberOfElements);
                uint32 e;
                for (e = 0; e < numberOfElements; e++) {
                    ok &=
                            (float64Signal[c++]
                                    == static_cast<float64>((static_cast<float64>(gainValue)
                                            * static_cast<float64>(typeValue))));
                }
            }
        }

    }
    god->Purge();
    return ok;
}
#endif /* CONVERSIONGAMTEST_H_ */

