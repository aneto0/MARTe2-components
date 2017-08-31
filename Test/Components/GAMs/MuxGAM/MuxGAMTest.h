/**
 * @file MuxGAMTest.h
 * @brief Header file for class MuxGAMTest
 * @date 28/08/2017
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

 * @details This header file contains the declaration of the class MuxGAMTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef MUXGAMTEST_H_
#define MUXGAMTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "ConfigurationDatabase.h"
#include "GAM.h"
#include "StreamString.h"
#include "MuxGAM.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

class MuxGAMTest {
//TODO Add the macro DLL_API to the class declaration (i.e. class DLL_API MuxGAMTest)
public:
    MuxGAMTest();
    virtual ~MuxGAMTest();

    /**
     * @brief Test MuxGAM::Initialise().
     */
    bool TestInitialise();

    /**
     * @brief Test messages errors MuxGAM::Setup().
     * @details no outputs no inputs
     */
    bool TestSetupNoOutputs();

    /**
     * @brief Test messages errors MuxGAM::Setup().
     * @details no inputs, 1 output.
     */
    bool TestSetupNoInputs();

    /**
     * @brief Test messages errors MuxGAM::Setup().
     * @details 1 input, 1 output.
     */
    bool TestSetupWrongInputs();

    /**
     * @brief Test messages errors MuxGAM::Setup().
     * @details 2 inputs, 1 output.
     */
    bool TestSetupWrongInputs2();

    /**
     * @brief Test messages errors MuxGAM::Setup().
     * @details 2 inputs, 2 outputs.
     */
    bool TestSetupWrongInputs3();

    /**
     * @brief Test messages errors MuxGAM::Setup().
     * @details 3 inputs, 1 output.
     */
    bool TestSetupNoSelectorType();

    /**
     * @brief Test messages errors MuxGAM::Setup().
     * @details 3 inputs, 1 output.
     */
    bool TestSetupWrongSelectorType();

    /**
     * @brief Test messages errors MuxGAM::Setup().
     * @details 3 inputs, 1 output.
     */
    bool TestSetupWrongInputType();

    /**
     * @brief Test messages errors MuxGAM::Setup().
     * @details 4 inputs, 2 output.
     */
    bool TestSetupWrongSelectorType2();

    /**
     * @brief Test messages errors MuxGAM::Setup().
     * @details 4 inputs, 2 output.
     */
    bool TestSetupWrongInputType2();

    /**
     * @brief Test messages errors MuxGAM::Setup().
     * @details 4 inputs, 2 output.
     */
    bool TestSetupWrongOutputType();

    /**
     * @brief Test messages errors MuxGAM::Setup().
     * @details 4 inputs, 2 output.
     */
    bool TestSetupWrongOutputType2();

    /**
     * @brief Test messages errors MuxGAM::Setup().
     * @details 4 inputs, 2 output.
     */
    bool TestSetupNoOutputElements();

    /**
     * @brief Test messages errors MuxGAM::Setup().
     * @details 4 inputs, 2 output. Second output without parameter elements defined.
     */
    bool TestSetupNoOutputElements_2();

    /**
     * @brief Test messages errors MuxGAM::Setup().
     * @details 4 inputs, 2 output.
     */
    bool TestSetupWrongOutputElements();

    /**
     * @brief Test messages errors MuxGAM::Setup().
     * @details 4 inputs, 2 output. Second number of elements different
     * from the first number of elements.
     */
    bool TestSetupWrongOutputElements2();

    /**
     * @brief Test messages errors MuxGAM::Setup().
     * @details 4 inputs, 2 output.
     */
    bool TestSetupNoSelectorElements();

    /**
     * @brief Test messages errors MuxGAM::Setup().
     * @details 4 inputs, 2 output.
     */
    bool TestSetupWrongSelectorElements();

    /**
     * @brief Test messages errors MuxGAM::Setup().
     * @details 4 inputs, 2 output. Second number of elements different
     * from the first number of elements.
     */
    bool TestSetupWrongSelectorElements2();

    /**
     * @brief Test messages errors MuxGAM::Setup().
     * @details 4 inputs, 2 output.
     */
    bool TestSetupNoInputElements();

    /**
     * @brief Test messages errors MuxGAM::Setup().
     * @details 4 inputs, 2 output.
     */
    bool TestSetupWrongInputElements();

    /**
     * @brief Test messages errors MuxGAM::Setup().
     * @details 4 inputs, 2 output. Second number of elements different
     * from the first number of elements.
     */
    bool TestSetupWrongInputElements2();

    /**
     * @brief Test messages errors MuxGAM::Setup().
     * @details 4 inputs, 2 output.
     */
    bool TestSetupNoInputDiemnsion();

    /**
     * @brief Test messages errors MuxGAM::Setup().
     * @details 4 inputs, 2 output.
     */
    bool TestSetupWrongInputDiemnsion();

    /**
     * @brief Test messages errors MuxGAM::Setup().
     * @details 4 inputs, 2 output. Second number of dimensions different
     * from the first number of dimensions.
     */
    bool TestSetupWrongInputDiemnsion2();

    /**
     * @brief Test messages errors MuxGAM::Setup().
     * @details 4 inputs, 2 output.
     */
    bool TestSetupNoOutputDiemnsion();

    /**
     * @brief Test messages errors MuxGAM::Setup().
     * @details 4 inputs, 2 output.
     */
    bool TestSetupWrongOutputDiemnsion();

    /**
     * @brief Test messages errors MuxGAM::Setup().
     * @details 4 inputs, 2 output. Second number of dimensions different
     * from the first number of dimensions.
     */
    bool TestSetupWrongOutputDiemnsion2();

    /**
     * @brief Test messages errors MuxGAM::Setup().
     * @details 4 inputs, 2 output.
     */
    bool TestSetupNoInputSamples();

    /**
     * @brief Test messages errors MuxGAM::Setup().
     * @details 4 inputs, 2 output.
     */
    bool TestSetupWrongInputSamples();

    /**
     * @brief Test messages errors MuxGAM::Setup().
     * @details 4 inputs, 2 output. Second number of samples different
     * from the first number of samples.
     */
    bool TestSetupWrongInputSamples2();

    /**
     * @brief Test messages errors MuxGAM::Setup().
     * @details 4 inputs, 2 output. Third number of samples different
     * from the first number of samples.
     */
    bool TestSetupWrongInputSamples3();

    /**
     * @brief Test messages errors MuxGAM::Setup().
     * @details 4 inputs, 2 output. Fourth number of samples different
     * from the first number of samples.
     */
    bool TestSetupWrongInputSamples4();

    /**
     * @brief Test messages errors MuxGAM::Setup().
     * @details 4 inputs, 2 output.
     */
    bool TestSetupNoOutputSamples();

    /**
     * @brief Test messages errors MuxGAM::Setup().
     * @details 4 inputs, 2 output.
     */
    bool TestSetupWrongOutputSamples();

    /**
     * @brief Test messages errors MuxGAM::Setup().
     * @details 4 inputs, 2 output. Second number of samples different
     * from the first number of samples.
     */
    bool TestSetupWrongOutputSamples2();

    /**
     * @brief Test MuxGAM::Setup().
     * @details 4 inputs, 2 output.
     */
    bool TestSetup4I2O(StreamString strType,
                       uint32 nOfElements,
                       uint32 nOfSelectorElements);

    /**
     * @brief Test MuxGAM::Execute() configured with 4 inputs two outputs.
     * @details Test all valid types, all selector combinations (without changing  selectors in the middle) and with a selector array.
     */
    template<typename T>
    bool TestExecute4I2OSelectorArray(StreamString strType,
                                      uint32 nOfElements,
                                      uint32 nOfSelectorElements);

    /**
     * @brief Test MuxGAM::Execute() configured with 4 inputs two outputs.
     * @details Test all valid types, all selector combinations and with a selector array.
     */
    template<typename T>
    bool TestExecute4I2OSelectorArray_2(StreamString strType,
                                        uint32 nOfElements,
                                        uint32 nOfSelectorElements);

    /**
     * @brief Test MuxGAM::Execute() configured with 4 inputs two outputs.
     * @details Test all valid types, all selector combinations and with a single selector element.
     */
    template<typename T>
    bool TestExecute4I2OSelector(StreamString strType,
                                        uint32 nOfElements,
                                        uint32 nOfSelectorElements);

    /**
     * @brief Test MuxGAM::Execute() with an invalid selector.
     * @details Test single element selector and selector array.
     */
    template<typename T>
    bool TestExecute4I2OInvalidSelector(StreamString strType,
                                        uint32 nOfElements,
                                        uint32 nOfSelectorElements);

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/
namespace MARTe {
class MuxGAMTestHelper: public MuxGAM {
public:
    MuxGAMTestHelper() {

    }
    virtual ~MuxGAMTestHelper() {

    }
    void *GetInputSignalsMemory(uint32 idx) {
        return GAM::GetInputSignalMemory(idx);
    }

    void *GetOutputSignalsMemory(uint32 idx) {
        return GAM::GetOutputSignalMemory(idx);
    }
    uint32 Sizeof(StreamString str) {
        uint32 ret = 0;
        StreamString aux("uint8");
        StreamString aux1("int8");
        StreamString aux2("uint16");
        StreamString aux3("int16");
        StreamString aux4("uint32");
        StreamString aux5("int32");
        StreamString aux6("uint64");
        StreamString aux7("int64");
        StreamString aux8("float32");
        StreamString aux9("float64");

        if (str == aux) {
            ret = 1;
        }
        else if (str == aux1) {
            ret = 1;
        }
        else if (str == aux2) {
            ret = 2;
        }
        else if (str == aux3) {
            ret = 2;
        }
        else if (str == aux4) {
            ret = 4;
        }
        else if (str == aux5) {
            ret = 4;
        }
        else if (str == aux6) {
            ret = 8;
        }
        else if (str == aux7) {
            ret = 8;
        }
        else if (str == aux8) {
            ret = 4;
        }
        else if (str == aux9) {
            ret = 8;
        }
        else {
            printf("Sizeof::error know type");
        }
        return ret;
    }
    bool Setup4Inputs2Outputs(StreamString strType,
                              uint32 nOfElements,
                              uint32 nOfSelectorElements) {
        uint32 sizeOutputSignals = nOfElements * Sizeof(strType.Buffer());
        uint32 totalSizeOutputSignals = 2 * sizeOutputSignals;
        bool ok = configSignals.CreateAbsolute("Signals.OutputSignals");
        ok &= configSignals.CreateRelative("0");
        ok &= configSignals.Write("Type", strType.Buffer());
        ok &= configSignals.Write("NumberOfElements", nOfElements);
        ok &= configSignals.Write("NumberOfDimensions", 1);
        ok &= configSignals.Write("DataSource", "OutputSignals1");
        ok &= configSignals.Write("ByteSize", sizeOutputSignals);
        ok &= configSignals.MoveToAncestor(1u);
        ok &= configSignals.CreateRelative("1");
        ok &= configSignals.Write("Type", strType.Buffer());
        ok &= configSignals.Write("NumberOfElements", nOfElements);
        ok &= configSignals.Write("NumberOfDimensions", 1);
        ok &= configSignals.Write("DataSource", "OutputSignals1");
        ok &= configSignals.Write("ByteSize", sizeOutputSignals);
        ok &= configSignals.MoveToAncestor(1u);
        ok &= configSignals.Write("ByteSize", totalSizeOutputSignals);

        uint32 sizeInputSignals = nOfElements * Sizeof(strType.Buffer());
        uint32 sizeInputSelectors = nOfSelectorElements * Sizeof("uint32");
        uint32 totalSizeInputSignalsSignals = 2 * sizeInputSelectors + 2 * sizeInputSignals;
        ok &= configSignals.CreateAbsolute("Signals.InputSignals");
        ok &= configSignals.CreateRelative("0");
        ok &= configSignals.Write("Type", "uint32");
        ok &= configSignals.Write("NumberOfElements", nOfSelectorElements);
        ok &= configSignals.Write("NumberOfDimensions", 1);
        ok &= configSignals.Write("DataSource", "Selector");
        ok &= configSignals.Write("ByteSize", sizeInputSelectors);
        ok &= configSignals.MoveToAncestor(1u);
        ok &= configSignals.CreateRelative("1");
        ok &= configSignals.Write("Type", "uint32");
        ok &= configSignals.Write("NumberOfElements", nOfSelectorElements);
        ok &= configSignals.Write("NumberOfDimensions", 1);
        ok &= configSignals.Write("DataSource", "Selector");
        ok &= configSignals.Write("ByteSize", sizeInputSelectors);
        ok &= configSignals.MoveToAncestor(1u);
        ok &= configSignals.CreateRelative("2");
        ok &= configSignals.Write("Type", strType.Buffer());
        ok &= configSignals.Write("NumberOfElements", nOfElements);
        ok &= configSignals.Write("NumberOfDimensions", 1);
        ok &= configSignals.Write("DataSource", "InputSignals1");
        ok &= configSignals.Write("ByteSize", sizeInputSignals);
        ok &= configSignals.MoveToAncestor(1u);
        ok &= configSignals.CreateRelative("3");
        ok &= configSignals.Write("Type", strType.Buffer());
        ok &= configSignals.Write("NumberOfElements", nOfElements);
        ok &= configSignals.Write("NumberOfDimensions", 1);
        ok &= configSignals.Write("DataSource", "InputSignals1");
        ok &= configSignals.Write("ByteSize", sizeInputSignals);
        ok &= configSignals.MoveToAncestor(1u);
        ok &= configSignals.Write("ByteSize", totalSizeInputSignalsSignals);
        ok &= configSignals.MoveToRoot();

        ok &= configSignals.CreateAbsolute("Memory.InputSignals");
        ok &= configSignals.CreateRelative("0");
        ok &= configSignals.Write("DataSource", "Selector");
        ok &= configSignals.CreateRelative("Signals");
        ok &= configSignals.CreateRelative("0");
        ok &= configSignals.Write("Samples", 1);
        ok &= configSignals.MoveToAncestor(1u);
        ok &= configSignals.CreateRelative("1");
        ok &= configSignals.Write("Samples", 1);
        ok &= configSignals.MoveToAncestor(3u);
        ok &= configSignals.CreateRelative("1");
        ok &= configSignals.Write("DataSource", "InputSignals1");
        ok &= configSignals.CreateRelative("Signals");
        ok &= configSignals.CreateRelative("2");
        ok &= configSignals.Write("Samples", 1);
        ok &= configSignals.MoveToAncestor(1u);
        ok &= configSignals.CreateRelative("3");
        ok &= configSignals.Write("Samples", 1);

        ok &= configSignals.CreateAbsolute("Memory.OutputSignals");
        ok &= configSignals.CreateRelative("0");
        ok &= configSignals.Write("DataSource", "OutputSignals1");
        ok &= configSignals.CreateRelative("Signals");
        ok &= configSignals.CreateRelative("0");
        ok &= configSignals.Write("Samples", 1);
        ok &= configSignals.MoveToAncestor(1u);
        ok &= configSignals.CreateRelative("1");
        ok &= configSignals.Write("Samples", 1);

        ok &= configSignals.MoveToRoot();

        return ok;
    }
    ConfigurationDatabase config;
    ConfigurationDatabase configSignals;

};

template<typename T>
bool MuxGAMTest::TestExecute4I2OSelectorArray(StreamString strType,
                                              uint32 nOfElements,
                                              uint32 nOfSelectorElements) {
    ConfigurationDatabase config;
    MuxGAMTestHelper gam;
    bool ok = gam.Initialise(gam.config);
    ok &= gam.Setup4Inputs2Outputs(strType, nOfElements, nOfSelectorElements);
    ok &= gam.SetConfiguredDatabase(gam.configSignals);

    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();

    if (ok) {
        ok &= gam.Setup();
    }
    if (ok) {
        uint32 * selector0 = static_cast<uint32 *>(gam.GetInputSignalsMemory(0u));
        uint32 * selector1 = static_cast<uint32 *>(gam.GetInputSignalsMemory(1u));
        T * input0 = static_cast<T *>(gam.GetInputSignalsMemory(2u));
        T * input1 = static_cast<T *>(gam.GetInputSignalsMemory(3u));
        T * Output0 = static_cast<T *>(gam.GetOutputSignalsMemory(0u));
        T * Output1 = static_cast<T *>(gam.GetOutputSignalsMemory(1u));
        //initialize inputs
        for (uint32 i = 0u; i < nOfElements; i++) {
            input0[i] = static_cast<T>(i + 1.1);
            input1[i] = 2.0 * static_cast<T>(i + 0.7);
            Output0[i] = static_cast<T>(0.0);
            Output1[i] = static_cast<T>(0.0);
        }
        for (uint32 i = 0u; i < nOfSelectorElements; i++) {
            selector0[i] = 0u;
            selector1[i] = 0u;
        }
        if (ok) {
            ok &= gam.Execute();
        }
        //Check output
        for (uint32 i = 0u; (i < nOfElements) && ok; i++) {
            ok &= (Output0[i] == input0[i]);
            if (ok) {
                ok &= (Output1[i] == input0[i]);
            }
        }
        //repeat for other selectors
        for (uint32 i = 0u; i < nOfSelectorElements; i++) {
            selector0[i] = 1u;
            selector1[i] = 0u;
        }
        if (ok) {
            ok &= gam.Execute();
        }
        //Check output
        for (uint32 i = 0u; (i < nOfElements) && ok; i++) {
            ok &= (Output0[i] == input1[i]);
            if (ok) {
                ok &= (Output1[i] == input0[i]);
            }
        }
        //repeat for other selectors
        for (uint32 i = 0u; i < nOfSelectorElements; i++) {
            selector0[i] = 0u;
            selector1[i] = 1u;
        }
        if (ok) {
            ok &= gam.Execute();
        }
        //Check output
        for (uint32 i = 0u; (i < nOfElements) && ok; i++) {
            ok &= (Output0[i] == input0[i]);
            if (ok) {
                ok &= (Output1[i] == input1[i]);
            }
        }
        //repeat for other selectors
        for (uint32 i = 0u; i < nOfSelectorElements; i++) {
            selector0[i] = 1u;
            selector1[i] = 1u;
        }
        if (ok) {
            ok &= gam.Execute();
        }
        //Check output
        for (uint32 i = 0u; (i < nOfElements) && ok; i++) {
            ok &= (Output0[i] == input1[i]);
            if (ok) {
                ok &= (Output1[i] == input1[i]);
            }
        }
    }
    return ok;
}

template<typename T>
bool MuxGAMTest::TestExecute4I2OSelectorArray_2(StreamString strType,
                                                uint32 nOfElements,
                                                uint32 nOfSelectorElements) {
    ConfigurationDatabase config;
    MuxGAMTestHelper gam;
    bool ok = gam.Initialise(gam.config);
    ok &= gam.Setup4Inputs2Outputs(strType, nOfElements, nOfSelectorElements);
    ok &= gam.SetConfiguredDatabase(gam.configSignals);

    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();

    if (ok) {
        ok &= gam.Setup();
    }
    if (ok) {
        uint32 * selector0 = static_cast<uint32 *>(gam.GetInputSignalsMemory(0u));
        uint32 * selector1 = static_cast<uint32 *>(gam.GetInputSignalsMemory(1u));
        T * input0 = static_cast<T *>(gam.GetInputSignalsMemory(2u));
        T * input1 = static_cast<T *>(gam.GetInputSignalsMemory(3u));
        T * Output0 = static_cast<T *>(gam.GetOutputSignalsMemory(0u));
        T * Output1 = static_cast<T *>(gam.GetOutputSignalsMemory(1u));
        //initialize inputs
        for (uint32 i = 0u; i < nOfElements; i++) {
            input0[i] = static_cast<T>(i + 1.1);
            input1[i] = 2.0 * static_cast<T>(i + 0.7);
            Output0[i] = static_cast<T>(0.0);
            Output1[i] = static_cast<T>(0.0);
        }
        for (uint32 i = 0u; i < (nOfSelectorElements / 4); i++) {
            selector0[i] = 0u;
            selector1[i] = 0u;
        }
        for (uint32 i = (nOfSelectorElements / 4); i < (nOfSelectorElements / 2); i++) {
            selector0[i] = 1u;
            selector1[i] = 0u;
        }
        for (uint32 i = (nOfSelectorElements / 2); i < (3 * nOfSelectorElements / 4); i++) {
            selector0[i] = 0u;
            selector1[i] = 1u;
        }
        for (uint32 i = (3 * nOfSelectorElements / 4); i < (nOfSelectorElements); i++) {
            selector0[i] = 1u;
            selector1[i] = 1u;
        }
        if (ok) {
            ok &= gam.Execute();
        }
        //Verify outputs
        for (uint32 i = 0u; i < (nOfSelectorElements / 4) && ok; i++) {
            ok &= (Output0[i] == input0[i]);
            if (ok) {
                ok &= (Output1[i] == input0[i]);
            }
        }
        for (uint32 i = (nOfSelectorElements / 4); i < (nOfSelectorElements / 2) && ok; i++) {
            ok &= (Output0[i] == input1[i]);
             if (ok) {
                 ok &= (Output1[i] == input0[i]);
             }
        }
        for (uint32 i = (nOfSelectorElements / 2); i < (3 * nOfSelectorElements / 4) && ok; i++) {
            ok &= (Output0[i] == input0[i]);
             if (ok) {
                 ok &= (Output1[i] == input1[i]);
             }
        }
        for (uint32 i = (3 * nOfSelectorElements / 4); i < (nOfSelectorElements) && ok; i++) {
            ok &= (Output0[i] == input1[i]);
             if (ok) {
                 ok &= (Output1[i] == input1[i]);
             }
        }
    }
    return ok;
}


template<typename T>
bool MuxGAMTest::TestExecute4I2OSelector(StreamString strType,
                                              uint32 nOfElements,
                                              uint32 nOfSelectorElements) {
    ConfigurationDatabase config;
    MuxGAMTestHelper gam;
    bool ok = gam.Initialise(gam.config);
    ok &= gam.Setup4Inputs2Outputs(strType, nOfElements, nOfSelectorElements);
    ok &= gam.SetConfiguredDatabase(gam.configSignals);

    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();

    if (ok) {
        ok &= gam.Setup();
    }
    if (ok) {
        uint32 * selector0 = static_cast<uint32 *>(gam.GetInputSignalsMemory(0u));
        uint32 * selector1 = static_cast<uint32 *>(gam.GetInputSignalsMemory(1u));
        T * input0 = static_cast<T *>(gam.GetInputSignalsMemory(2u));
        T * input1 = static_cast<T *>(gam.GetInputSignalsMemory(3u));
        T * Output0 = static_cast<T *>(gam.GetOutputSignalsMemory(0u));
        T * Output1 = static_cast<T *>(gam.GetOutputSignalsMemory(1u));
        //initialize inputs
        for (uint32 i = 0u; i < nOfElements; i++) {
            input0[i] = static_cast<T>(i + 1.1);
            input1[i] = 2.0 * static_cast<T>(i + 0.7);
            Output0[i] = static_cast<T>(0.0);
            Output1[i] = static_cast<T>(0.0);
        }
        for (uint32 i = 0u; i < nOfSelectorElements; i++) {
            selector0[i] = 0u;
            selector1[i] = 0u;
        }
        if (ok) {
            ok &= gam.Execute();
        }
        //Check output
        for (uint32 i = 0u; (i < nOfElements) && ok; i++) {
            ok &= (Output0[i] == input0[i]);
            if (ok) {
                ok &= (Output1[i] == input0[i]);
            }
        }
        //repeat for other selectors
        for (uint32 i = 0u; i < nOfSelectorElements; i++) {
            selector0[i] = 1u;
            selector1[i] = 0u;
        }
        if (ok) {
            ok &= gam.Execute();
        }
        //Check output
        for (uint32 i = 0u; (i < nOfElements) && ok; i++) {
            ok &= (Output0[i] == input1[i]);
            if (ok) {
                ok &= (Output1[i] == input0[i]);
            }
        }
        //repeat for other selectors
        for (uint32 i = 0u; i < nOfSelectorElements; i++) {
            selector0[i] = 0u;
            selector1[i] = 1u;
        }
        if (ok) {
            ok &= gam.Execute();
        }
        //Check output
        for (uint32 i = 0u; (i < nOfElements) && ok; i++) {
            ok &= (Output0[i] == input0[i]);
            if (ok) {
                ok &= (Output1[i] == input1[i]);
            }
        }
        //repeat for other selectors
        for (uint32 i = 0u; i < nOfSelectorElements; i++) {
            selector0[i] = 1u;
            selector1[i] = 1u;
        }
        if (ok) {
            ok &= gam.Execute();
        }
        //Check output
        for (uint32 i = 0u; (i < nOfElements) && ok; i++) {
            ok &= (Output0[i] == input1[i]);
            if (ok) {
                ok &= (Output1[i] == input1[i]);
            }
        }
    }
    return ok;
}

template<typename T>
bool MuxGAMTest::TestExecute4I2OInvalidSelector(StreamString strType,
                                              uint32 nOfElements,
                                              uint32 nOfSelectorElements) {
    ConfigurationDatabase config;
    MuxGAMTestHelper gam;
    bool ok = gam.Initialise(gam.config);
    ok &= gam.Setup4Inputs2Outputs(strType, nOfElements, nOfSelectorElements);
    ok &= gam.SetConfiguredDatabase(gam.configSignals);

    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();

    if (ok) {
        ok &= gam.Setup();
    }
    if (ok) {
        uint32 * selector0 = static_cast<uint32 *>(gam.GetInputSignalsMemory(0u));
        uint32 * selector1 = static_cast<uint32 *>(gam.GetInputSignalsMemory(1u));
        T * input0 = static_cast<T *>(gam.GetInputSignalsMemory(2u));
        T * input1 = static_cast<T *>(gam.GetInputSignalsMemory(3u));
        T * Output0 = static_cast<T *>(gam.GetOutputSignalsMemory(0u));
        T * Output1 = static_cast<T *>(gam.GetOutputSignalsMemory(1u));
        //initialize inputs
        for (uint32 i = 0u; i < nOfElements; i++) {
            input0[i] = static_cast<T>(i + 1.1);
            input1[i] = 2.0 * static_cast<T>(i + 0.7);
            Output0[i] = static_cast<T>(0.0);
            Output1[i] = static_cast<T>(0.0);
        }
        for (uint32 i = 0u; i < nOfSelectorElements; i++) {
            selector0[i] = 2u;
            selector1[i] = 0u;
        }
        if (ok) {
            ok &= !gam.Execute();
        }
    }
    return ok;
}
}

#endif /* MUXGAMTEST_H_ */

