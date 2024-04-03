/**
 * @file FilterGAMTest.h
 * @brief Header file for class FilterGAMTest
 * @date 30/01/2017
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

 * @details This header file contains the declaration of the class FilterGAMTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef TEST_COMPONENTS_GAMS_FILTERGAM_FILTERGAMTEST_H_
#define TEST_COMPONENTS_GAMS_FILTERGAM_FILTERGAMTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "FilterGAMTestHelper.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {
/**
 * @brief Tests the FilterGAM public methods.
 */
class FilterGAMTest {
public:
    /**
     * @brief Constructor. NOOP.
     */
    FilterGAMTest();

    /**
     * @brief Destructor. NOOP.
     */
    virtual ~FilterGAMTest();

    /**
     * @brief Test initialise on error
     */
    bool TestInitialise_GAMInitialiseFail();

    /**
     * @brief Test initialise on error
     */
    bool TestInitialise_ZeroInputSignals();

    /**
     * @brief Test initialise on error
     */
    bool TestInitialise_UnsupportedInputType();

    /**
     * @brief Test initialise on error
     */
    bool TestInitialise_NoNum();

    /**
     * @brief Test initialise on error
     */
    template<typename T>
    bool TestInitialise_EmptyNum();

    /**
     * @brief Test initialise on error
     */
    template<typename T>
    bool TestInitialise_EmptyDen();

    /**
     * @brief Test initialise on error
     */
    bool TestInitialise_WrongFilterCoeff();

    /**
     * @brief Test initialise on error
     */
    bool TestInitialise_WrongRestInEachState();

    /**
     * @brief Test initialise on error
     */
    bool TestInitialise_CheckNormalisationBeforeInitialise();

    /**
     * @brief Tests default constructor.
     * @details checks all the post conditions
     */
    bool TestConstructor();
    /**
     * @brief Tests an arbitrary initialisation configuration.
     * @details Verifies the post-conditions
     * @return true on succeed.
     */
    template<typename T>
    bool TestInitialise();

    /**
     * @brief Tests an initialisation with no ResetInEachState.
     * @return true if the initialisation succeed & ResetInEachState = true (but a warning must be lunched).
     */
    bool TestInitialiseNoResetInEachState();
    /**
     * @brief Tests an initialisation with a wrong ResetInEachState.
     * @return true if the initialisation fails.
     */
    bool TestInitialiseWrongResetInEachState();

    /**
     * @brief Tests an initialisation with no numerator coefficients.
     * @return true if the initialisation fails.
     */
    bool TestInitialiseNoNum();

    /**
     * @brief Tests Initialise() errors
     * @details Configure the numerator with a boolean value.
     * @return true if Initialise() fails.
     */
    bool TestInitialiseWrongNumType();

    /**
     * @brief Tests an initialisation with no denominator coefficients.
     * @return true if the initialisation fails.
     */
    bool TestInitialiseNoDen();

    /**
     * @brief Tests Initialise() errors
     * @details Configure the denominator with a boolean value.
     * @return true if Initialise() fails.
     */
    bool TestInitialiseWrongDenType();

    /**
     * @brief Tests that the static gain is set properly with FIR filter
     */
    template<typename T>
    bool TestStaticGainFIR();

    /**
     * @brief Tests that the static gain is set properly with IIR filter
     */
    template<typename T>
    bool TestStaticGainIIR();

    /**
     * @brief Tests the error normalise
     * @return true when initailise fails due to CheckNormalisation fails
     */
    bool TestFailNormalise();

    /**
     * @brief Tests that a correct default setup works
     * @details Verify the post-conditions
     * @return true  if Setup returns true.
     */
    template<typename T>
    bool TestSetup();

    /**
     * @brief Tests that the Setup returns false if the wrong input signal type is asked for.
     */
    template<typename T>
    bool TestSetupWrongInputSignalType();

    /**
     * @brief Tests that the Setup returns false if the wrong output signal type is asked for.
     */
    template<typename T>
    bool TestSetupWrongOutputSignalType();

    /**
     * @brief Tests errors regarding to wrong GAM input setup
     * @details call the function Setup() without initialising the inputs in the database.
     * @return true if the Setup() fails.
     */
    bool TestSetupNoInputSignal();

    /**
     * @brief Tests errors regarding to wrong GAM output setup
     * @details call the function Setup() without initialising the GAM output in the database.
     * @return true if the Setup() fails.
     */
    bool TestSetupNoOutputSignal();

    /**
     * @brief Tests errors
     * @details Configure the GAM without input samples field
     * @return True if Setup() fails.
     */
    bool TestSetupNoInputSamples();

    /**
     * @brief Test errors.
     * @details Configure the GAM without output samples field.
     * @return True if Setup() fails.
     */
    bool TestSetupNoOutputSamples();

    /**
     * @brief Tests errors
     * @details Configure the GAM without input number of elements field
     * @return True if Setup() fails.
     */
    bool TestSetupNoNumberOfElementsInput();

    /**
     * @brief Tests errors
     * @details Configure the GAM without output number of elements field
     * @return True if Setup() fails.
     */
    bool TestSetupNoNumberOfElementsOutput();

    /**
     * @brief Tests errors
     * @details Configure the GAM with wrong number of output samples.
     * @return True if Setup() fails.
     */
    bool TestSetupNumberOfSamplesOutput2();

    /**
     * @brief Tests errors.
     * @details call the function Setup() initialising the input with 0 elements
     * @return true if the Setup() fails.
     */
    bool TestSetup0NumberOfElements();

    /**
     * @brief Tests errors
     * @details tests the fail with samples > 1 and Elements > 1.
     * @return true if Setup() fails.
     */
    bool TestSetupFailNumberOfSamples();

    /**
     * @brief Tests errors.
     * @details call the function Setup() after initialising the input and the outputs of the
     * GAM with different number of elements.
     * @return true if Setup() fails.
     */
    bool TestSetupDifferentInputOutputSamples();

    /**
     * @brief Tests errors.
     * @details initialise the GAM without input dimension.
     * @return true if Setup() fails.
     */
    bool TestSetupNoInputDimension();

    /**
     * @brief Tests errors.
     * @details initialise the GAM with a wrong input dimension. the dimension supported is 1 (array)
     * @return true if Setup() fails.
     */
    bool TestSetupWrongInputDimension();

    /**
     * @brief Tests errors.
     * @details initialise the GAM without output dimension.
     * @return true if Setup() fails.
     */
    bool TestSetupNoOutputDimension();

    /**
     * @brief Tests errors.
     * @details initialise the GAM with a wrong output dimension. the dimension supported is 1 (array)
     * @return true if Setup() fails.
     */
    bool TestSetupWrongOutputDimension();

    /**
     * @brief test the Execute() with um = 1 den = 1.
     * @details first Initialise() and Setup() the filter. Using a constant input, tests Execute() comparing the
     * output against the expected values.
     * @return true when the output is as expected.
     */
    template<typename T>
    bool TestExecuteNoFilter();

    /**
     * @brief test the Execute() with a simple two samples average filter.
     * @details first Initialise() and Setup() the filter. Using a constant input, tests Execute() comparing the
     * output against the expected values.
     * @return true when the output is as expected.
     */
    template<typename T>
    bool TestExecuteFIRConstantInput();

    /**
     * @brief test the Execute() with a simple two samples average filter.
     * @details first Initialise() and Setup() the filter. Using a constant input, tests Execute() comparing the
     * output against the expected values.
     * @return true when the output is as expected.
     */
    template<typename T>
    bool TestExecuteFIRConstantInputInputElements1();

    /**
     * @brief test the Execute() with a simple average filter.
     * @details first Initialise() and Setup() the filter. Using a ramp input, tests Execute() comparing the
     * output against the expected values.
     * @return true when the output is as expected.
     */
    template<typename T>
    bool TestExecuteFIRRampInput();

    /**
     * @brief test the Execute() with a simple average filter.
     * @details first Initialise() and Setup() the filter. Using a ramp input, tests Execute() comparing the
     * output against the expected values.
     * @return true when the output is as expected.
     */
    template<typename T>
    bool TestExecuteFIRRampInputInputElements1();

    /**
     * @brief test the Execute() with a simple 10 samples average filter.
     * @details first Initialise() and Setup() the filter. Using a ramp input, tests Execute() comparing the
     * output against the expected values.
     * @return true when the output is as expected.
     */
    template<typename T>
    bool TestExecuteAVG10RampInputInputElements1();

    /**
     * @brief test the Execute() with a simple average filter.
     * @details first Initialise() and Setup() the filter. Using a constant input, tests Execute() comparing the
     * output against the expected values. In this case Execute() is called twice in order to check that the last
     * state is correctly remembered.
     * @return true when the output is as expected.
     */
    template<typename T>
    bool TestExecuteFIRConstantInput2();

    /**
     * @brief test the Execute() with a simple average filter.
     * @details first Initialise() and Setup() the filter. Using a ramp input, tests Execute() comparing the
     * output against the expected values. In this case Execute() is called twice in order to check that the last
     * state is correctly remembered.
     * @return true when the output is as expected.
     */
    template<typename T>
    bool TestExecuteFIRRampInput2();

    /**
     * @brief test the Execute() with a simple FIR low pass filter with different coefficients (num[0] != num[1].
     * @details first Initialise() and Setup() the filter. Using a ramp input, tests Execute() comparing the
     * output against the expected values. In this case Execute() is called twice in order to check that the last
     * state is correctly remembered.
     * @return true when the output is as expected.
     */
    template<typename T>
    bool TestExecuteFIRRampInput2DiffCoef();

    /**
     * @brief test the Execute() with a simple integrater (sum(inputs) .
     * @details first Initialise() and Setup() the filter. Using a constant input, tests Execute() comparing the
     * output against the expected values. Notice he output is a ramp.
     * @return true when the output is as expected.
     */
    template<typename T>
    bool TestExecuteIIRConstant();

    /**
     * @brief test the Execute() with an arbitrary IIR filter.
     * @details Filter configuration :
     *     - num [0.1 0.2 0.3]
     *     - den [1 0.5 0.2]
     * first Initialise() and Setup() the filter. Using a ramp input of slope 1, tests Execute() comparing the
     * output against the expected values (computed from matlab).
     * @return true when the output is as expected.
     */
    template<typename T>
    bool TestExecuteIIRInputRamp1InputElement();

    /**
     * @brief test the Execute() with a simple integrator (sum(inputs)) .
     * @details first Initialise() and Setup() the filter. Using a constant input, tests Execute() comparing the
     * output against the expected values. n this case Execute() is called twice in order to check that the last
     * state is correctly remembered
     * @return true when the output is as expected.
     */
    template<typename T>
    bool TestExecuteIIRConstant2();

    /**
     * @brief Test the Execute with dimension input array = 1
     * @return true when the output is as expected
     */
    template<typename T>
    bool TestExecuteIIRConstantDimArray1();

    /**
     * @brief Tests Execute with input elements 1 and input samples 10
     * @details the filterGAM output configuration  supported is samples = 1
     * @return true if the data output is as expected.
     */
    template<typename T>
    bool TestExecuteElements1Samples10();

    /**
     * @brief Tests arbitrary GAM configuration with two input (and output) signals.
     * @return true on succeed.
     */
    template<typename T>
    bool TestSetupSeveralSignals();

    /**
     * @brief Tests errors with two input signals.
     * @details Configure the GAM with different number of input elements.
     * @return True if Setup() fails.
     */
    template<typename T>
    bool TestSetupSeveralSignalsDiffNumberOfInputElements();

    /**
     * @brief Tests errors with two input signals.
     * @details Configure the GAM with different number of output elements.
     * @return True if Setup()fails.
     */
    template<typename T>
    bool TestSetupSeveralSignalsDiffNumberOfOutputElements();

    /**
     * @brief Tests errors with two input signals.
     * @details Configure the GAM with different number of input samples.
     * @return True if Setup()fails.
     */
    template<typename T>
    bool TestSetupSeveralSignalsDiffInputSamples();

    /**
     * @brief Tests a FIR filter with 2 input signals.
     * @details Apply two times the Execute() with different input signals and then compare the
     * output against the expected values.
     * @return True if the output is as expected.
     */
    template<typename T>
    bool TestExecuteSeveralSignalsFIR();

    /**
     * @brief Tests the reset FIR filter with resetInEachState = true;
     * @details The sequence tested is filter, reset filter and then filter again. The output of
     * the filter is check against the expected values
     * @return true if the output values are as expected.
     */
    template<typename T>
    bool TestAlwaysResetFIR();

    /**
     * @brief Tests the reset IIR filter with resetInEachState = true;
     * @detail The sequence tested is filter, reset filter and then filter again. The output of
     * the filter is check against the expected values
     * @return true if the output values are as expected.
     */
    template<typename T>
    bool TestAlwaysResetIIR();

    /**
     * @brief Tests errors resetInEachState = true.
     * @details Call function PrepareNextState() without allocating memory for the last state.
     * @return true if PrepareNextState() fails.
     */
    template<typename T>
    bool TestAlwaysResetMemoryNotInt();

    /**
     * @brief Tests the reset FIR filter with resetInEachState = false;
     * @details The sequence tested is filter, reset filter and then filter again. The output of
     * the filter is check against the expected values. The simulated state machin is ""->"A"->"B". Consequently the filter should no be reset
     * @return true if the output values are as expected.
     */
    template<typename T>
    bool TestResetOnlyWhenRequired();

    /**
     * @brief Tests the reset FIR filter with resetInEachState = false;
     * @details The sequence tested is filter, reset filter and then filter again. The output of
     * the filter is check against the expected values. The simulated state machin is ""->"A"->"C". Consequently the filter should be reset
     * @return true if the output values are as expected.
     */
    template<typename T>
    bool TestResetOnlyWhenRequired2();

    /**
     * @brief Tests the reset IIR filter with resetInEachState = false
     * @details The sequence tested is filter, reset filter and then filter again. The output of
     * the filter is check against the expected values. The simulated state machin is ""->"A"->"C". Consequently the filter should be reset
     * @return true if the output values are as expected.
     */
    template<typename T>
    bool TestResetOnlyWhenRequired3();

    /**
     * @brief Tests errors with resetInEachState = false
     * @details Call function PrepareNextState() without allocating memory for the last state.
     * @return true if PrepareNextState() fails.
     */
    template<typename T>
    bool TestResetOnlyWhenRequiredMemoryNotInit();
private:
    bool MyIsEqual(MARTe::float32 f1,
                   MARTe::float32 f2);
    bool MyIsEqual(MARTe::float64 f1,
                   MARTe::float64 f2);
    bool MyIsEqual(MARTe::int32 f1,
                   MARTe::int32 f2);
    bool MyIsEqual(MARTe::int64 f1,
                   MARTe::int64 f2);
};

template<typename T>
bool FilterGAMTest::TestInitialise_EmptyNum() {
    using namespace MARTe;
    FilterGAMTestHelper<T> gam;
    ConfigurationDatabase cfg;
    cfg.CreateAbsolute("InputSignals.Input0");
    T auxVar = 0;
    StreamString auxStr = gam.GetTypeString(auxVar);
    cfg.Write("Type", auxStr.Buffer());
    cfg.MoveToRoot();
    cfg.Write("Num", "");
    cfg.Write("Den", "");
    bool ok = !gam.Initialise(cfg);
    return ok;
}

template<typename T>
bool FilterGAMTest::TestInitialise_EmptyDen() {
    using namespace MARTe;
    FilterGAMTestHelper<T> gam;
    ConfigurationDatabase cfg;
    cfg.CreateAbsolute("InputSignals.Input0");
    T auxVar = 0;
    StreamString auxStr = gam.GetTypeString(auxVar);
    cfg.Write("Type", auxStr.Buffer());
    cfg.MoveToRoot();
    Vector<T> auxVec(1);
    auxVec[0] = 1;
    cfg.Write("Num", auxVec);
    cfg.Write("Den", "");
    bool ok = !gam.Initialise(cfg);
    return ok;
}

template<typename T>
bool FilterGAMTest::TestInitialise() {
    using namespace MARTe;
    FilterGAMTestHelper<T> gam;
    gam.SetName("Test");
    bool ok = true;

    ok &= gam.InitialiseFilterFIR();
    ok &= gam.Initialise(gam.config);
    StreamString a;
    //example how to print a ConfigurationDatabase
    /*
     gam.config.MoveToRoot();
     printf("size of a %llu\n",a.Size());
     printf("%d\n",a.Printf("%!", gam.config));
     printf("size of a %llu\n",a.Size());
     printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
     printf("%s\n", a.Buffer());
     printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
     */
    ok &= gam.GetNumberOfNumCoeff() == 2;
    ok &= gam.GetNumberOfDenCoeff() == 1;
    T *retNum = new T[2];
    T *retDen = new T[1];
    ok &= gam.GetNumCoeff(retNum);
    for (uint32 i = 0; i < 2; i++) {
        ok &= (retNum[i] == gam.numH[i]);
    }
    ok &= gam.GetDenCoeff(retDen);
    for (uint32 i = 0u; i < 1; i++) {
        ok &= (retDen[i] == gam.denH[i]);
    }
    bool isInfinite;
    T gainReturn;
    gam.GetStaticGain(isInfinite, gainReturn);
    ok &= (2 == gainReturn);
    ok &= !isInfinite;
    ok &= (0 == gam.GetNumberOfSamples());
    ok &= (0 == gam.GetNumberOfSignals());
    ok &= !gam.GetResetInEachState();
    delete[] retNum;
    delete[] retDen;
    ok &= gam.CheckNormalisation();
    return ok;
}

template<typename T>
bool FilterGAMTest::TestStaticGainFIR() {
    using namespace MARTe;
    FilterGAMTestHelper<T> gam;
    T gainReturn;
    bool isInfinite = true;
    gam.SetName("Test");
    bool ok = true;
    gam.GetStaticGain(isInfinite, gainReturn);
    ok &= gam.InitialiseFilterFIR();
    ok &= gam.Initialise(gam.config);
    //Check static gain

    gam.GetStaticGain(isInfinite, gainReturn);
    ok &= (static_cast<T>(2) == gainReturn);
    ok &= !isInfinite;

    return ok;
}

template<typename T>
bool FilterGAMTest::TestStaticGainIIR() {
    using namespace MARTe;
    FilterGAMTestHelper<T> gam;
    gam.SetName("Test");
    bool ok = true;

    ok &= gam.InitialiseFilterIIR();
    ok &= gam.Initialise(gam.config);
    //Check static gain
    bool isInfinite = false;
    float32 gainReturn;
    gam.GetStaticGain(isInfinite, gainReturn);
    ok &= gainReturn == 0;
    ok &= isInfinite;
    return ok;
}

template<typename T>
bool FilterGAMTest::TestSetup() {
    using namespace MARTe;
    FilterGAMTestHelper<T> gam;
    gam.SetName("Test");
    bool ok = true;

    ok &= gam.InitialiseFilterFIR();
    ok &= gam.Initialise(gam.config);

    ok &= gam.InitialiseConfigDataBaseSignal1();
    ok &= gam.SetConfiguredDatabase(gam.configSignals);

    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();

    ok &= gam.Setup();

    //Verify post-conditions
    ok &= (gam.GetNumberOfSamples() == gam.numberOfElements);
    ok &= (gam.GetNumberOfSignals() == 1);
    return ok;
}

template<typename T>
bool FilterGAMTest::TestSetupWrongInputSignalType() {
    using namespace MARTe;
    FilterGAMTestHelper<T> gam;
    gam.SetName("Test");
    bool ok = true;

    ok &= gam.InitialiseFilterFIR();
    ok &= gam.Initialise(gam.config);

    ok &= gam.InitialiseConfigDataBaseSignal3();
    ok &= gam.SetConfiguredDatabase(gam.configSignals);

    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();

    ok &= gam.Setup();
    return !ok;
}

template<typename T>
bool FilterGAMTest::TestSetupWrongOutputSignalType() {
    using namespace MARTe;
    FilterGAMTestHelper<T> gam;
    gam.SetName("Test");
    bool ok = true;

    ok &= gam.InitialiseFilterFIR();
    ok &= gam.Initialise(gam.config);

    ok &= gam.InitialiseConfigDataBaseSignal4();
    ok &= gam.SetConfiguredDatabase(gam.configSignals);

    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();

    ok &= gam.Setup();

    return !ok;
}

template<typename T>
bool FilterGAMTest::TestExecuteNoFilter() {
    using namespace MARTe;
    FilterGAMTestHelper<T> gam;
    gam.SetName("Test");
    bool ok = true;

    ok &= gam.InitialiseNoFilter();
    ok &= gam.Initialise(gam.config);

    ok &= gam.InitialiseConfigDataBaseSignal1();
    ok &= gam.SetConfiguredDatabase(gam.configSignals);

    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();

    ok &= gam.Setup();

    T *gamMemoryIn = static_cast<T*>(gam.GetInputSignalsMemory());
    T *gamMemoryOut = static_cast<T*>(gam.GetOutputSignalsMemory());
    //assign inputs and outputs
    for (uint32 i = 0u; i < gam.numberOfElements; i++) {
        gamMemoryIn[i] = 1;
        gamMemoryOut[i] = 0;
    }
    if (ok) {
        gam.Execute();
    }
    for (uint32 i = 0u; i < gam.numberOfElements; i++) {
        ok &= (gamMemoryOut[i] == 1.0);
    }
    return ok;
}

template<typename T>
bool FilterGAMTest::TestExecuteFIRConstantInput() {
    using namespace MARTe;
    FilterGAMTestHelper<T> gam;
    gam.SetName("Test");
    bool ok = true;

    ok &= gam.InitialiseFilterFIR();
    ok &= gam.Initialise(gam.config);

    ok &= gam.InitialiseConfigDataBaseSignal1();
    ok &= gam.SetConfiguredDatabase(gam.configSignals);

    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();

    ok &= gam.Setup();

    T *gamMemoryIn = static_cast<T*>(gam.GetInputSignalsMemory());
    T *gamMemoryOut = static_cast<T*>(gam.GetOutputSignalsMemory());
    //assign inputs and outputs
    for (uint32 i = 0u; i < gam.numberOfElements; i++) {
        gamMemoryIn[i] = 1;
        gamMemoryOut[i] = 0;
    }
    if (ok) {
        gam.Execute();
    }
    ok &= (gamMemoryOut[0] == 1.0);
    for (uint32 i = 1u; i < gam.numberOfElements; i++) {
        ok &= (gamMemoryOut[i] == 2.0);
    }
    return ok;
}
template<typename T>
bool FilterGAMTest::TestExecuteFIRConstantInputInputElements1() {
    using namespace MARTe;
    FilterGAMTestHelper<T> gam(1, 1);
    gam.SetName("Test");
    bool ok = true;

    ok &= gam.InitialiseFilterFIR();
    ok &= gam.Initialise(gam.config);

    ok &= gam.InitialiseConfigDataBaseSignal1();
    ok &= gam.SetConfiguredDatabase(gam.configSignals);

    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();

    ok &= gam.Setup();

    float32 *gamMemoryIn = static_cast<float32*>(gam.GetInputSignalsMemory());
    float32 *gamMemoryOut = static_cast<float32*>(gam.GetOutputSignalsMemory());
    //assign inputs and outputs
    for (uint32 i = 0u; i < gam.numberOfElements; i++) {
        gamMemoryIn[i] = 1;
        gamMemoryOut[i] = 0;
    }
    if (ok) { //Samples 1
        gam.Execute();
    }
    ok &= (gamMemoryOut[0] == 1.0);
    if (ok) { //Samples 2
        gam.Execute();
    }
    ok &= (gamMemoryOut[0] == gamMemoryOut[0]);
    if (ok) { //Samples 3
        gam.Execute();
    }
    ok &= (gamMemoryOut[0] == gamMemoryOut[0]);
    return ok;
}

template<typename T>
bool FilterGAMTest::TestExecuteFIRRampInput() {
    using namespace MARTe;
    FilterGAMTestHelper<T> gam;
    gam.SetName("Test");
    bool ok = true;

    ok &= gam.InitialiseFilterFIR();
    ok &= gam.Initialise(gam.config);

    ok &= gam.InitialiseConfigDataBaseSignal1();
    ok &= gam.SetConfiguredDatabase(gam.configSignals);

    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();

    ok &= gam.Setup();

    float32 *gamMemoryIn = static_cast<float32*>(gam.GetInputSignalsMemory());
    float32 *gamMemoryOut = static_cast<float32*>(gam.GetOutputSignalsMemory());

    //assign inputs and outputs
    for (uint32 i = 0u; i < gam.numberOfSamples; i++) {
        gamMemoryIn[i] = i;
        gamMemoryOut[i] = 0;
    }
    if (ok) {
        gam.Execute();
    }
    ok &= (gamMemoryOut[0] == 0);
    for (uint32 i = 1u; i < gam.numberOfSamples; i++) {
        //ok &= (gamMemoryOut[i] == i - 0.5);
        ok &= (gamMemoryOut[i] == i - 0.5);
    }
    return ok;
}

template<typename T>
bool FilterGAMTest::TestExecuteFIRRampInputInputElements1() {
    using namespace MARTe;
    FilterGAMTestHelper<T> gam(1, 1);
    gam.SetName("Test");
    bool ok = true;

    ok &= gam.InitialiseFilterFIR();
    ok &= gam.Initialise(gam.config);

    ok &= gam.InitialiseConfigDataBaseSignal1();
    ok &= gam.SetConfiguredDatabase(gam.configSignals);

    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();

    ok &= gam.Setup();

    float32 *gamMemoryIn = static_cast<float32*>(gam.GetInputSignalsMemory());
    float32 *gamMemoryOut = static_cast<float32*>(gam.GetOutputSignalsMemory());
    float32 rampInput = 0.0;
    //assign inputs and outputs
    for (uint32 i = 0u; i < gam.numberOfSamples; i++) {
        gamMemoryIn[i] = rampInput;
        gamMemoryOut[i] = 0;
    }
    if (ok) { //sample 0
        gam.Execute();
    }
    ok &= (gamMemoryOut[0] == 0);

    rampInput += 1.0;
    //assign inputs and outputs
    for (uint32 i = 0u; i < gam.numberOfSamples; i++) {
        gamMemoryIn[i] = rampInput;
    }
    if (ok) { //sample 1
        gam.Execute();
    }
    for (uint32 i = 1u; i < gam.numberOfSamples; i++) {
        ok &= (gamMemoryOut[i] == rampInput);
    }
    rampInput += 1.0;
    //assign inputs and outputs
    for (uint32 i = 0u; i < gam.numberOfSamples; i++) {
        gamMemoryIn[i] = rampInput;
    }
    if (ok) { //sample 2
        gam.Execute();
    }
    for (uint32 i = 1u; i < gam.numberOfSamples; i++) {
        ok &= (gamMemoryOut[i] == rampInput);
    }
    rampInput += 1.0;
    //assign inputs and outputs
    for (uint32 i = 0u; i < gam.numberOfSamples; i++) {
        gamMemoryIn[i] = rampInput;
    }
    if (ok) { //sample 3
        gam.Execute();
    }
    for (uint32 i = 1u; i < gam.numberOfSamples; i++) {
        ok &= (gamMemoryOut[i] == rampInput);
    }
    return ok;
}

template<typename T>
bool FilterGAMTest::TestExecuteAVG10RampInputInputElements1() {
    using namespace MARTe;
    FilterGAMTestHelper<T> gam(1, 1);
    gam.SetName("Test");
    uint32 nOfTestSamples = 50;
    T *outputRef = new T[nOfTestSamples];
    T rampVal = 1.0;
    T accum = 1.0;
    for (uint32 i = 0; i < 10; i++) {
        outputRef[i] = accum;
        rampVal += 1.0;
        accum += rampVal;
    }
    for (uint32 i = 10; i < nOfTestSamples; i++) {
        outputRef[i] = outputRef[i - 1] + 10.0;
    }
    bool ok = true;

    ok &= gam.InitialiseAVG10();
    ok &= gam.Initialise(gam.config);

    ok &= gam.InitialiseConfigDataBaseSignal1();
    ok &= gam.SetConfiguredDatabase(gam.configSignals);

    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();

    ok &= gam.Setup();

    T *gamMemoryIn = static_cast<T*>(gam.GetInputSignalsMemory());
    T *gamMemoryOut = static_cast<T*>(gam.GetOutputSignalsMemory());
    gamMemoryIn[0] = 0.0;
    gamMemoryOut[0] = 0;
    uint32 idxFail = 0;
    for (uint32 i = 0u; (i < nOfTestSamples) && ok; i++) {
        gamMemoryIn[0] += 1.0;
        if (ok) { //sample 0
            gam.Execute();
        }
        ok = MyIsEqual(gamMemoryOut[0], outputRef[i]);
        if (!ok) {
            idxFail = i;
        }
    }
    if (idxFail > 0) {
        ok = false;
    }
    delete[] outputRef;
    return ok;
}

template<typename T>
bool FilterGAMTest::TestExecuteFIRConstantInput2() {
    using namespace MARTe;
    FilterGAMTestHelper<T> gam;
    gam.SetName("Test");
    bool ok = true;

    ok &= gam.InitialiseFilterFIR();
    ok &= gam.Initialise(gam.config);

    ok &= gam.InitialiseConfigDataBaseSignal1();
    ok &= gam.SetConfiguredDatabase(gam.configSignals);

    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();

    ok &= gam.Setup();

    T *gamMemoryIn = static_cast<T*>(gam.GetInputSignalsMemory());
    T *gamMemoryOut = static_cast<T*>(gam.GetOutputSignalsMemory());

    //assign inputs and outputs
    for (uint32 i = 0u; i < gam.numberOfElements; i++) {
        gamMemoryIn[i] = 1;
        gamMemoryOut[i] = 0;
    }
    if (ok) {
        gam.Execute();
    }
    ok &= (gamMemoryOut[0] == 1);
    for (uint32 i = 1u; i < gam.numberOfElements; i++) {
        ok &= (gamMemoryOut[i] == 2.0);
    }
    gam.Execute();
    for (uint32 i = 0u; i < gam.numberOfElements; i++) {
        ok &= (gamMemoryOut[i] == 2.0);
    }

    return ok;
}

template<typename T>
bool FilterGAMTest::TestExecuteFIRRampInput2() {
    using namespace MARTe;
    FilterGAMTestHelper<T> gam;
    gam.SetName("Test");
    bool ok = true;

    ok &= gam.InitialiseFilterFIR();
    ok &= gam.Initialise(gam.config);

    ok &= gam.InitialiseConfigDataBaseSignal1();
    ok &= gam.SetConfiguredDatabase(gam.configSignals);

    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();

    ok &= gam.Setup();

    T *gamMemoryIn = static_cast<T*>(gam.GetInputSignalsMemory());
    T *gamMemoryOut = static_cast<T*>(gam.GetOutputSignalsMemory());
    //assign inputs and outputs
    for (uint32 i = 0u; i < gam.numberOfElements; i++) {
        gamMemoryIn[i] = i;
        gamMemoryOut[i] = 0;
    }
    if (ok) {
        gam.Execute();
    }
    ok &= (gamMemoryOut[0] == 0);
    for (uint32 i = 1u; i < gam.numberOfElements; i++) {
        ok &= (gamMemoryOut[i] == static_cast<int32>(2 * i - 1));
    }
    for (uint32 i = 0u; i < gam.numberOfElements; i++) {
        gamMemoryIn[i] = i + 10;
        //gamMemoryOut[i] = 0;
    }
    gam.Execute();
    for (uint32 i = 0u; i < gam.numberOfElements; i++) {
        ok &= (gamMemoryOut[i] == static_cast<int32>(20 + 2 * i - 1));
    }
    return ok;
}

template<typename T>
bool FilterGAMTest::TestExecuteFIRRampInput2DiffCoef() {
    using namespace MARTe;
    FilterGAMTestHelper<T> gam;
    gam.SetName("Test");
    bool ok = true;

    ok &= gam.InitialiseFilterFIR2();
    ok &= gam.Initialise(gam.config);

    ok &= gam.InitialiseConfigDataBaseSignal1();
    ok &= gam.SetConfiguredDatabase(gam.configSignals);

    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();

    ok &= gam.Setup();

    T *gamMemoryIn = static_cast<T*>(gam.GetInputSignalsMemory());
    T *gamMemoryOut = static_cast<T*>(gam.GetOutputSignalsMemory());
    //assign inputs and outputs
    for (uint32 i = 0u; i < gam.numberOfElements; i++) {
        gamMemoryIn[i] = i;
        gamMemoryOut[i] = 0;
    }
    if (ok) {
        gam.Execute();
    }
    ok &= (gamMemoryOut[0] == 0);
    T refValue = 0;
    for (uint32 i = 1u; i < gam.numberOfElements; i++) {
        refValue = (i * 6) + (i - 1) * 4;
        ok &= MyIsEqual(gamMemoryOut[i], refValue);
    }
    for (uint32 i = 0u; i < gam.numberOfElements; i++) {
        gamMemoryIn[i] = i + 10;
        //gamMemoryOut[i] = 0;
    }
    if (ok) {
        gam.Execute();
    }
    for (uint32 i = 1u; i < gam.numberOfElements; i++) {
        refValue = ((i + 10) * 6) + (i + 9) * 4;
        ok &= MyIsEqual(gamMemoryOut[i], refValue);
    }
    return ok;
}

template<typename T>
bool FilterGAMTest::TestExecuteIIRConstant() {
    using namespace MARTe;
    FilterGAMTestHelper<T> gam;
    gam.SetName("Test");
    bool ok = true;

    ok &= gam.InitialiseFilterIIR();
    ok &= gam.Initialise(gam.config);

    ok &= gam.InitialiseConfigDataBaseSignal1();
    ok &= gam.SetConfiguredDatabase(gam.configSignals);

    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();

    ok &= gam.Setup();

    T *gamMemoryIn = static_cast<T*>(gam.GetInputSignalsMemory());
    T *gamMemoryOut = static_cast<T*>(gam.GetOutputSignalsMemory());
    //assign inputs and outputs
    for (uint32 i = 0u; i < gam.numberOfElements; i++) {
        gamMemoryIn[i] = 1;
        gamMemoryOut[i] = 0;
    }
    if (ok) {
        gam.Execute();
    }
    ok &= (gamMemoryOut[0] == 1);
    for (uint32 i = 1u; i < gam.numberOfElements; i++) {
        ok &= (gamMemoryOut[i] == static_cast<int32>(1 + i));
    }
    return ok;
}

template<typename T>
bool FilterGAMTest::TestExecuteIIRInputRamp1InputElement() {
    using namespace MARTe;
    FilterGAMTestHelper<T> gam(1, 1);
    gam.SetName("Test");
    uint32 nOfIterations = 10;
    T *expectedOutput = new T[nOfIterations];
    expectedOutput[0] = 0;
    expectedOutput[1] = 1;
//    expectedOutput[2] = 0.35;
    expectedOutput[2] = -1;
    //expectedOutput[3] = 0.805;
    expectedOutput[3] = 13;
//    expectedOutput[4] = 1.1275;
//    expectedOutput[5] = 1.47525;
//    expectedOutput[6] = 1.836875;
//    expectedOutput[7] = 2.1865125;
//    expectedOutput[8] = 2.53936875;
//    expectedOutput[9] = 2.893013125;

    expectedOutput[4] = -47;
    expectedOutput[5] = 231;
    expectedOutput[6] = -1033;
    expectedOutput[7] = 4737;
    expectedOutput[8] = -21579;
    expectedOutput[9] = 98467;
    bool ok = true;

    ok &= gam.InitialiseFilterIIR2();
    ok &= gam.Initialise(gam.config);

    ok &= gam.InitialiseConfigDataBaseSignal1();
    ok &= gam.SetConfiguredDatabase(gam.configSignals);

    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();

    ok &= gam.Setup();

    T *gamMemoryIn = static_cast<T*>(gam.GetInputSignalsMemory());
    T *gamMemoryOut = static_cast<T*>(gam.GetOutputSignalsMemory());
    //assign inputs and outputs
    gamMemoryIn[0] = 0;
    gamMemoryOut[0] = 0;
    for (uint32 i = 0u; i < nOfIterations && ok; i++) {
        gam.Execute();
        ok &= MyIsEqual(gamMemoryOut[0], expectedOutput[i]);
        gamMemoryIn[0] = gamMemoryIn[0] + 1.0;
    }

    delete[] expectedOutput;
    return ok;
}

template<typename T>
bool FilterGAMTest::TestExecuteIIRConstant2() {
    using namespace MARTe;
    FilterGAMTestHelper<T> gam;
    gam.SetName("Test");
    bool ok = true;

    ok &= gam.InitialiseFilterIIR();
    ok &= gam.Initialise(gam.config);

    ok &= gam.InitialiseConfigDataBaseSignal1();
    ok &= gam.SetConfiguredDatabase(gam.configSignals);

    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();

    ok &= gam.Setup();

    T *gamMemoryIn = static_cast<T*>(gam.GetInputSignalsMemory());
    T *gamMemoryOut = static_cast<T*>(gam.GetOutputSignalsMemory());
    //assign inputs and outputs
    for (uint32 i = 0u; i < gam.numberOfElements; i++) {
        gamMemoryIn[i] = 1;
        gamMemoryOut[i] = 0;
    }
    if (ok) {
        gam.Execute();
    }
    ok &= (gamMemoryOut[0] == 1);
    for (uint32 i = 1u; i < gam.numberOfElements; i++) {
        ok &= (gamMemoryOut[i] == static_cast<int32>(1 + i));
    }
    gam.Execute();
    for (uint32 i = 0u; i < gam.numberOfElements; i++) {
        ok &= (gamMemoryOut[i] == static_cast<int32>(10 + 1 + i));
    }
    return ok;
}

template<typename T>
bool FilterGAMTest::TestExecuteIIRConstantDimArray1() {
    using namespace MARTe;
    uint32 numberOfElements = 1;
    FilterGAMTestHelper<T> gam(numberOfElements, 1);
    gam.SetName("Test");
    bool ok = gam.InitialiseFilterIntegrator();
    ok &= gam.Initialise(gam.config);
    ok &= gam.InitialiseConfigDataBaseSignal1();
    ok &= gam.SetConfiguredDatabase(gam.configSignals);

    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();
    ok &= gam.Setup();

    T *gamMemoryIn = static_cast<T*>(gam.GetInputSignalsMemory());
    T *gamMemoryOut = static_cast<T*>(gam.GetOutputSignalsMemory());
    //assign inputs and outputs
    for (uint32 i = 0u; i < numberOfElements; i++) {
        gamMemoryIn[i] = 1;
        gamMemoryOut[i] = 0;
    }
    if (ok) {
        gam.Execute();
    }
    ok &= (gamMemoryOut[0] == 1);

    ok &= (gamMemoryOut[0] == 1);
    //Call execute 9 extra times just to filter the signal, which has dimension = 1,
    T refValue = 1u;
    for (uint32 n = 0; n < 9; n++) {
        gam.Execute();
        refValue++;
        ok &= (gamMemoryOut[0] == refValue);
    }
    return ok;
}

template<typename T>
bool FilterGAMTest::TestExecuteElements1Samples10() {
    using namespace MARTe;
    FilterGAMTestHelper<T> gam(1, 10);
    gam.SetName("Test");
    bool ok = true;

    ok &= gam.InitialiseFilterFIR();
    ok &= gam.Initialise(gam.config);

    ok &= gam.InitialiseConfigDataBaseSignal1();

    ok &= gam.SetConfiguredDatabase(gam.configSignals);

    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();

    ok &= gam.Setup();

    T *gamMemoryIn = static_cast<T*>(gam.GetInputSignalsMemory());
    T *gamMemoryOut = static_cast<T*>(gam.GetOutputSignalsMemory());

    //assign inputs and outputs
    for (uint32 i = 0u; i < gam.numberOfSamples; i++) {
        gamMemoryIn[i] = i;
        gamMemoryOut[i] = 0;
    }
    if (ok) {
        gam.Execute();
    }
    ok &= (gamMemoryOut[0] == 0);
    for (uint32 i = 1u; i < gam.numberOfSamples; i++) {
        ok &= (gamMemoryOut[i] == 2 * i - 1.0);
    }
    for (uint32 i = 0u; i < gam.numberOfSamples; i++) {
        gamMemoryIn[i] = i + gam.numberOfSamples;
        //gamMemoryOut[i] = 0;
    }
    if (ok) {
        gam.Execute();
    }
    for (uint32 i = 0u; i < gam.numberOfSamples; i++) {
        ok &= (gamMemoryOut[i] == 2 * (gam.numberOfSamples + i - 0.5));
    }
    //Check size elements input or output (they are the same)
    ok &= (gam.GetNumberOfSamples() == 10);
    return ok;
}

template<typename T>
bool FilterGAMTest::TestSetupSeveralSignals() {
    using namespace MARTe;
    FilterGAMTestHelper<T> gam;
    gam.SetName("Test");
    bool ok = true;

    ok &= gam.InitialiseFilterFIR();
    ok &= gam.Initialise(gam.config);

    ok &= gam.InitialiseConfigDataBaseSignal2();
    ok &= gam.SetConfiguredDatabase(gam.configSignals);

    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();

    ok &= gam.Setup();
    ok &= (gam.GetNumberOfInputSignals() == 2);
    ok &= (gam.GetNumberOfSamples() == gam.numberOfElements);
    return ok;
}

template<typename T>
bool FilterGAMTest::TestSetupSeveralSignalsDiffNumberOfInputElements() {
    using namespace MARTe;
    FilterGAMTestHelper<T> gam;
    gam.SetName("Test");
    bool ok = true;

    ok &= gam.InitialiseFilterFIR();
    ok &= gam.Initialise(gam.config);

    uint32 numberOfElements = 10;
    uint32 numberOfSamples = 1;
    uint32 byteSize = numberOfElements * sizeof(T);
    uint32 numberOfElements1 = 11;
    uint32 byteSize1 = numberOfElements1 * sizeof(T);
    uint32 totalByteSize = byteSize + byteSize1;
    ConfigurationDatabase configSignals;

    ok &= configSignals.CreateAbsolute("Signals.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("QualifiedName", "InputSignal1");
    ok &= configSignals.Write("DataSource", "TestDataSource");
    T auxVar = 0;
    StreamString auxStrType = gam.GetTypeString(auxVar);
    ok &= configSignals.Write("Type", auxStrType.Buffer());
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.Write("NumberOfElements", numberOfElements);
    ok &= configSignals.Write("ByteSize", byteSize);

    ok &= configSignals.MoveAbsolute("Signals.InputSignals");
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("QualifiedName", "InputSignal2");
    ok &= configSignals.Write("DataSource", "TestDataSource");
    ok &= configSignals.Write("Type", auxStrType.Buffer());
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.Write("NumberOfElements", numberOfElements1);
    ok &= configSignals.Write("ByteSize", byteSize);

    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", totalByteSize);

    ok &= configSignals.MoveToRoot();
    ok &= configSignals.CreateAbsolute("Signals.OutputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("QualifiedName", "OutputSignal1");
    ok &= configSignals.Write("DataSource", "TestDataSource");
    ok &= configSignals.Write("Type", auxStrType.Buffer());
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.Write("NumberOfElements", numberOfElements);
    ok &= configSignals.Write("ByteSize", byteSize);

    ok &= configSignals.MoveAbsolute("Signals.OutputSignals");
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("QualifiedName", "OutputSignal2");
    ok &= configSignals.Write("DataSource", "TestDataSource");
    ok &= configSignals.Write("Type", auxStrType.Buffer());
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.Write("NumberOfElements", numberOfElements1);
    ok &= configSignals.Write("ByteSize", byteSize);

    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", totalByteSize);

    ok &= configSignals.MoveToRoot();

    ok &= configSignals.CreateAbsolute("Memory.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("DataSource", "TestDataSource");
    ok &= configSignals.CreateRelative("Signals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Samples", numberOfSamples);

    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("Samples", numberOfSamples);

    ok &= configSignals.CreateAbsolute("Memory.OutputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("DataSource", "TestDataSource");
    ok &= configSignals.CreateRelative("Signals");
    ok &= configSignals.CreateRelative("0");
    //The Filter GAM only support numberOfSamples 1 and numberOfElements>=1
    ok &= configSignals.Write("Samples", 1);

    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("Samples", 1);

    ok &= configSignals.MoveToRoot();
    ok &= gam.SetConfiguredDatabase(configSignals);

    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();

    ok &= !gam.Setup();

    return ok;
}

template<typename T>
bool FilterGAMTest::TestSetupSeveralSignalsDiffNumberOfOutputElements() {
    using namespace MARTe;
    FilterGAMTestHelper<T> gam;
    gam.SetName("Test");
    bool ok = true;

    ok &= gam.InitialiseFilterFIR();
    ok &= gam.Initialise(gam.config);

    uint32 numberOfElements = 10;
    uint32 numberOfSamples = 1;
    uint32 byteSize = numberOfElements * sizeof(float32);
    uint32 numberOfElements1 = 11;
    uint32 byteSize1 = numberOfElements1 * sizeof(float32);
    uint32 totalByteSize = byteSize + byteSize1;
    ConfigurationDatabase configSignals;

    ok &= configSignals.CreateAbsolute("Signals.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("QualifiedName", "InputSignal1");
    ok &= configSignals.Write("DataSource", "TestDataSource");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.Write("NumberOfElements", numberOfElements);
    ok &= configSignals.Write("ByteSize", byteSize);

    ok &= configSignals.MoveAbsolute("Signals.InputSignals");
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("QualifiedName", "InputSignal2");
    ok &= configSignals.Write("DataSource", "TestDataSource");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.Write("NumberOfElements", numberOfElements);
    ok &= configSignals.Write("ByteSize", byteSize);

    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", totalByteSize);

    ok &= configSignals.MoveToRoot();
    ok &= configSignals.CreateAbsolute("Signals.OutputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("QualifiedName", "OutputSignal1");
    ok &= configSignals.Write("DataSource", "TestDataSource");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.Write("NumberOfElements", numberOfElements);
    ok &= configSignals.Write("ByteSize", byteSize);

    ok &= configSignals.MoveAbsolute("Signals.OutputSignals");
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("QualifiedName", "OutputSignal2");
    ok &= configSignals.Write("DataSource", "TestDataSource");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.Write("NumberOfElements", numberOfElements1);
    ok &= configSignals.Write("ByteSize", byteSize);

    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", totalByteSize);

    ok &= configSignals.MoveToRoot();

    ok &= configSignals.CreateAbsolute("Memory.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("DataSource", "TestDataSource");
    ok &= configSignals.CreateRelative("Signals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Samples", numberOfSamples);

    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("Samples", numberOfSamples);

    ok &= configSignals.CreateAbsolute("Memory.OutputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("DataSource", "TestDataSource");
    ok &= configSignals.CreateRelative("Signals");
    ok &= configSignals.CreateRelative("0");
    //The Filter GAM only support numberOfSamples 1 and numberOfElements>=1
    ok &= configSignals.Write("Samples", 1);

    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("Samples", 1);

    ok &= configSignals.MoveToRoot();
    ok &= gam.SetConfiguredDatabase(configSignals);

    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();

    ok &= !gam.Setup();

    return ok;
}
template<typename T>
bool FilterGAMTest::TestSetupSeveralSignalsDiffInputSamples() {
    using namespace MARTe;
    FilterGAMTestHelper<T> gam;
    gam.SetName("Test");
    bool ok = true;

    ok &= gam.InitialiseFilterFIR();
    ok &= gam.Initialise(gam.config);

    uint32 numberOfElements = 10;
    uint32 numberOfSamples = 1;
    uint32 byteSize = numberOfElements * sizeof(float32);
    uint32 numberOfElements1 = 11;
    uint32 numberOfSamples1 = 2;
    uint32 byteSize1 = numberOfElements1 * sizeof(float32);
    uint32 totalByteSize = byteSize + byteSize1;
    ConfigurationDatabase configSignals;

    ok &= configSignals.CreateAbsolute("Signals.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("QualifiedName", "InputSignal1");
    ok &= configSignals.Write("DataSource", "TestDataSource");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.Write("NumberOfElements", numberOfElements);
    ok &= configSignals.Write("ByteSize", byteSize);

    ok &= configSignals.MoveAbsolute("Signals.InputSignals");
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("QualifiedName", "InputSignal2");
    ok &= configSignals.Write("DataSource", "TestDataSource");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.Write("NumberOfElements", numberOfElements);
    ok &= configSignals.Write("ByteSize", byteSize);

    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", totalByteSize);

    ok &= configSignals.MoveToRoot();
    ok &= configSignals.CreateAbsolute("Signals.OutputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("QualifiedName", "OutputSignal1");
    ok &= configSignals.Write("DataSource", "TestDataSource");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.Write("NumberOfElements", numberOfElements);
    ok &= configSignals.Write("ByteSize", byteSize);

    ok &= configSignals.MoveAbsolute("Signals.OutputSignals");
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("QualifiedName", "OutputSignal2");
    ok &= configSignals.Write("DataSource", "TestDataSource");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.Write("NumberOfElements", numberOfElements);
    ok &= configSignals.Write("ByteSize", byteSize);

    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", totalByteSize);

    ok &= configSignals.MoveToRoot();

    ok &= configSignals.CreateAbsolute("Memory.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("DataSource", "TestDataSource");
    ok &= configSignals.CreateRelative("Signals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Samples", numberOfSamples);

    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("Samples", numberOfSamples1);

    ok &= configSignals.CreateAbsolute("Memory.OutputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("DataSource", "TestDataSource");
    ok &= configSignals.CreateRelative("Signals");
    ok &= configSignals.CreateRelative("0");
    //The Filter GAM only support numberOfSamples 1 and numberOfElements>=1
    ok &= configSignals.Write("Samples", 1);

    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("Samples", 1);

    ok &= configSignals.MoveToRoot();
    ok &= gam.SetConfiguredDatabase(configSignals);

    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();

    ok &= !gam.Setup();

    return ok;
}

template<typename T>
bool FilterGAMTest::TestExecuteSeveralSignalsFIR() {
    using namespace MARTe;
    FilterGAMTestHelper<T> gam;
    gam.SetName("Test");
    bool ok = true;

    ok &= gam.InitialiseFilterFIR();
    ok &= gam.Initialise(gam.config);

    ok &= gam.InitialiseConfigDataBaseSignal2();
    ok &= gam.SetConfiguredDatabase(gam.configSignals);

    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();

    ok &= gam.Setup();

    T *gamMemoryIn0 = static_cast<T*>(gam.GetInputSignalsMemory());
    T *gamMemoryOut0 = static_cast<T*>(gam.GetOutputSignalsMemory());
    T *gamMemoryIn1 = static_cast<T*>(gam.GetInputSignalsMemory(1));
    T *gamMemoryOut1 = static_cast<T*>(gam.GetOutputSignalsMemory(1));
    //assign inputs and outputs
    for (uint32 i = 0u; i < gam.numberOfElements; i++) {
        gamMemoryIn0[i] = i;
        gamMemoryOut0[i] = 0;
        gamMemoryIn1[i] = 1;
        gamMemoryOut1[i] = 0;
    }
    if (ok) {
        gam.Execute();
    }
    ok &= (gamMemoryOut0[0] == 0);
    ok &= (gamMemoryOut1[0] == 1);
    for (uint32 i = 1u; i < gam.numberOfElements; i++) {
        ok &= (gamMemoryOut0[i] == 2 * i - 1);
        ok &= (gamMemoryOut1[i] == 2);
    }
    for (uint32 i = 0u; i < gam.numberOfElements; i++) {
        gamMemoryIn0[i] = i + 10;
        gamMemoryIn1[i] = 1;
        //gamMemoryOut[i] = 0;
    }
    gam.Execute();
    for (uint32 i = 0u; i < gam.numberOfElements; i++) {
        ok &= (gamMemoryOut0[i] == 20 + 2 * i - 1);
        ok &= (gamMemoryOut1[i] == 2);
    }
    return ok;
}

template<typename T>
bool FilterGAMTest::TestAlwaysResetFIR() {
    using namespace MARTe;
    FilterGAMTestHelper<T> gam;
    gam.SetName("Test");
    bool ok = true;
    //gam.config.Write("ResetInEachState", true); set the value to true, then the gam.InitialiseFilterFIR() set the value to false,
    //however the read stops at the first ResetInEachState
    ok &= gam.InitialiseFilterFIR();
    ok &= gam.config.Write("ResetInEachState", 1);
    ok &= gam.Initialise(gam.config);
    ok &= gam.InitialiseConfigDataBaseSignal2();
    ok &= gam.SetConfiguredDatabase(gam.configSignals);

    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();

    ok &= gam.Setup();

    T *gamMemoryIn0 = static_cast<T*>(gam.GetInputSignalsMemory());
    T *gamMemoryOut0 = static_cast<T*>(gam.GetOutputSignalsMemory());
    T *gamMemoryIn1 = static_cast<T*>(gam.GetInputSignalsMemory(1));
    T *gamMemoryOut1 = static_cast<T*>(gam.GetOutputSignalsMemory(1));
    //assign inputs and outputs
    for (uint32 i = 0u; i < gam.numberOfElements; i++) {
        gamMemoryIn0[i] = i;
        gamMemoryOut0[i] = 0;
        gamMemoryIn1[i] = 1;
        gamMemoryOut1[i] = 0;
    }
    if (ok) {
        gam.PrepareNextState("", "A");
        gam.Execute();
    }
    ok &= (gamMemoryOut0[0] == 0);
    ok &= (gamMemoryOut1[0] == 1);
    for (uint32 i = 1u; i < gam.numberOfElements; i++) {
        ok &= (gamMemoryOut0[i] == 2 * i - 1);
        ok &= (gamMemoryOut1[i] == 2);
    }
    for (uint32 i = 0u; i < gam.numberOfElements; i++) {
        gamMemoryIn0[i] = i;
        gamMemoryIn1[i] = 1;
        //gamMemoryOut[i] = 0;
    }
    gam.PrepareNextState("A", "B");
    gam.Execute();
    ok &= (gamMemoryOut0[0] == 0);
    ok &= (gamMemoryOut1[0] == 1);
    for (uint32 i = 1u; i < gam.numberOfElements; i++) {
        T auxRef = 2 * (i - 0.5);
        ok &= MyIsEqual(gamMemoryOut0[i], auxRef);
        auxRef = 2;
        ok &= MyIsEqual(gamMemoryOut1[i], auxRef);
    }
    return ok;
}

template<typename T>
bool FilterGAMTest::TestAlwaysResetIIR() {
    using namespace MARTe;
    FilterGAMTestHelper<T> gam;
    gam.SetName("Test");
    bool ok = true;
    //gam.config.Write("ResetInEachState", true); set the value to true, then the gam.InitialiseFilterFIR() set the value to false,
    //however the read stops at the first ResetInEachState
    ok &= gam.InitialiseFilterIIR();
    ok &= gam.config.Write("ResetInEachState", true);
    ok &= gam.Initialise(gam.config);
    ok &= gam.InitialiseConfigDataBaseSignal2();
    ok &= gam.SetConfiguredDatabase(gam.configSignals);

    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();

    ok &= gam.Setup();

    T *gamMemoryIn0 = static_cast<T*>(gam.GetInputSignalsMemory());
    T *gamMemoryOut0 = static_cast<T*>(gam.GetOutputSignalsMemory());
    T *gamMemoryIn1 = static_cast<T*>(gam.GetInputSignalsMemory(1));
    T *gamMemoryOut1 = static_cast<T*>(gam.GetOutputSignalsMemory(1));
    //assign inputs and outputs
    for (uint32 i = 0u; i < gam.numberOfElements; i++) {
        gamMemoryIn0[i] = 1.0F;
        gamMemoryOut0[i] = 0;
        gamMemoryIn1[i] = 1.0F;
        gamMemoryOut1[i] = 0;
    }
    if (ok) {
        gam.PrepareNextState("", "A");
        gam.Execute();
    }
    for (uint32 i = 1u; i < gam.numberOfElements; i++) {
        T auxRef = i + 1;
        ok &= MyIsEqual(gamMemoryOut0[i], auxRef);
        ok &= MyIsEqual(gamMemoryOut1[i], auxRef);
    }
    for (uint32 i = 0u; i < gam.numberOfElements; i++) {
        gamMemoryIn0[i] = 1;
        gamMemoryIn1[i] = 1;
        //gamMemoryOut[i] = 0;
    }
    gam.PrepareNextState("A", "B");
    gam.Execute();
    for (uint32 i = 1u; i < gam.numberOfElements; i++) {
        T auxRef = i + 1.0F;
        ok &= MyIsEqual(gamMemoryOut0[i], auxRef);
        ok &= MyIsEqual(gamMemoryOut1[i], auxRef);
    }
    return ok;
}

template<typename T>
bool FilterGAMTest::TestAlwaysResetMemoryNotInt() {
    using namespace MARTe;
    FilterGAMTestHelper<T> gam;
    gam.SetName("Test");
    bool ok = true;
    //gam.config.Write("ResetInEachState", true); set the value to true, then the gam.InitialiseFilterFIR() set the value to false,
    //however the read stops at the first ResetInEachState
    ok &= gam.InitialiseFilterFIR();
    ok &= gam.config.Write("ResetInEachState", true);
    ok &= gam.Initialise(gam.config);
    ok &= gam.InitialiseConfigDataBaseSignal2();
    ok &= gam.SetConfiguredDatabase(gam.configSignals);

    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();

    float32 *gamMemoryIn0 = static_cast<float32*>(gam.GetInputSignalsMemory());
    float32 *gamMemoryOut0 = static_cast<float32*>(gam.GetOutputSignalsMemory());
    float32 *gamMemoryIn1 = static_cast<float32*>(gam.GetInputSignalsMemory(1));
    float32 *gamMemoryOut1 = static_cast<float32*>(gam.GetOutputSignalsMemory(1));
    //assign inputs and outputs
    for (uint32 i = 0u; i < gam.numberOfElements; i++) {
        gamMemoryIn0[i] = i;
        gamMemoryOut0[i] = 0;
        gamMemoryIn1[i] = 1;
        gamMemoryOut1[i] = 0;
    }
    if (ok) {
        ok &= !gam.PrepareNextState("", "A");

    }
    return ok;
}

template<typename T>
bool FilterGAMTest::TestResetOnlyWhenRequired() {
    using namespace MARTe;
    FilterGAMTestHelper<T> gam;
    gam.SetName("Test");
    bool ok = true;

    ok &= gam.InitialiseFilterFIR();
    ok &= gam.Initialise(gam.config);
    ok &= gam.InitialiseConfigDataBaseSignal2();
    ok &= gam.SetConfiguredDatabase(gam.configSignals);

    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();

    ok &= gam.Setup();

    float32 *gamMemoryIn0 = static_cast<float32*>(gam.GetInputSignalsMemory());
    float32 *gamMemoryOut0 = static_cast<float32*>(gam.GetOutputSignalsMemory());
    float32 *gamMemoryIn1 = static_cast<float32*>(gam.GetInputSignalsMemory(1));
    float32 *gamMemoryOut1 = static_cast<float32*>(gam.GetOutputSignalsMemory(1));
    //assign inputs and outputs
    for (uint32 i = 0u; i < gam.numberOfElements; i++) {
        gamMemoryIn0[i] = i;
        gamMemoryOut0[i] = 0;
        gamMemoryIn1[i] = 1;
        gamMemoryOut1[i] = 0;
    }
    if (ok) {
        gam.PrepareNextState("", "A");
        gam.Execute();
    }
    ok &= (gamMemoryOut0[0] == 0);
    ok &= (gamMemoryOut1[0] == 1);
    for (uint32 i = 1u; i < gam.numberOfElements; i++) {
        ok &= (gamMemoryOut0[i] == 2 * i - 1);
        ok &= (gamMemoryOut1[i] == 2);
    }
    for (uint32 i = 0u; i < gam.numberOfElements; i++) {
        gamMemoryIn0[i] = i;
        gamMemoryIn1[i] = 1;
        //gamMemoryOut[i] = 0;
    }
    gam.PrepareNextState("A", "B");
    gam.Execute();
    for (uint32 i = 0u; i < gam.numberOfElements; i++) {
        gamMemoryIn0[i] = i + 10;
        gamMemoryIn1[i] = 1;
        //gamMemoryOut[i] = 0;
    }
    gam.Execute();
    for (uint32 i = 0u; i < gam.numberOfElements; i++) {
        ok &= (gamMemoryOut0[i] == 20 + 2 * i - 1);
        ok &= (gamMemoryOut1[i] == 2);
    }
    return ok;
}

template<typename T>
bool FilterGAMTest::TestResetOnlyWhenRequired2() {
    using namespace MARTe;
    FilterGAMTestHelper<T> gam;
    gam.SetName("Test");
    bool ok = true;

    ok &= gam.InitialiseFilterFIR();
    ok &= gam.Initialise(gam.config);
    ok &= gam.InitialiseConfigDataBaseSignal2();
    ok &= gam.SetConfiguredDatabase(gam.configSignals);

    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();

    ok &= gam.Setup();

    T *gamMemoryIn0 = static_cast<T*>(gam.GetInputSignalsMemory());
    T *gamMemoryOut0 = static_cast<T*>(gam.GetOutputSignalsMemory());
    T *gamMemoryIn1 = static_cast<T*>(gam.GetInputSignalsMemory(1));
    T *gamMemoryOut1 = static_cast<T*>(gam.GetOutputSignalsMemory(1));
    //assign inputs and outputs
    for (uint32 i = 0u; i < gam.numberOfElements; i++) {
        gamMemoryIn0[i] = i;
        gamMemoryOut0[i] = 0;
        gamMemoryIn1[i] = 1;
        gamMemoryOut1[i] = 0;
    }
    if (ok) {
        gam.PrepareNextState("", "A");
        gam.Execute();
    }
    ok &= (gamMemoryOut0[0] == 0);
    ok &= (gamMemoryOut1[0] == 1);
    for (uint32 i = 1u; i < gam.numberOfElements; i++) {
        ok &= (gamMemoryOut0[i] == 2 * i - 1);
        ok &= (gamMemoryOut1[i] == 2);
    }

    for (uint32 i = 0u; i < gam.numberOfElements; i++) {
        gamMemoryIn0[i] = i;
        gamMemoryIn1[i] = 1;
        //gamMemoryOut[i] = 0;
    }
    gam.PrepareNextState("B", "C");
    gam.Execute();
    ok &= (gamMemoryOut0[0] == 0);
    ok &= (gamMemoryOut1[0] == 1);
    for (uint32 i = 1u; i < gam.numberOfElements; i++) {
        ok &= (gamMemoryOut0[i] == 2 * i - 1);
        ok &= (gamMemoryOut1[i] == 2);
    }
    return ok;
}

template<typename T>
bool FilterGAMTest::TestResetOnlyWhenRequired3() {
    using namespace MARTe;
    FilterGAMTestHelper<T> gam;
    gam.SetName("Test");
    bool ok = true;

    ok &= gam.InitialiseFilterIIR();
    ok &= gam.Initialise(gam.config);
    ok &= gam.InitialiseConfigDataBaseSignal2();
    ok &= gam.SetConfiguredDatabase(gam.configSignals);

    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();

    ok &= gam.Setup();

    float32 *gamMemoryIn0 = static_cast<float32*>(gam.GetInputSignalsMemory());
    float32 *gamMemoryOut0 = static_cast<float32*>(gam.GetOutputSignalsMemory());
    float32 *gamMemoryIn1 = static_cast<float32*>(gam.GetInputSignalsMemory(1));
    float32 *gamMemoryOut1 = static_cast<float32*>(gam.GetOutputSignalsMemory(1));
    //assign inputs and outputs
    for (uint32 i = 0u; i < gam.numberOfElements; i++) {
        gamMemoryIn0[i] = 1;
        gamMemoryOut0[i] = 0;
        gamMemoryIn1[i] = 1;
        gamMemoryOut1[i] = 0;
    }
    if (ok) {
        gam.PrepareNextState("", "A");
        gam.Execute();
    }
    //Check output
    for (uint32 i = 0u; i < gam.numberOfElements; i++) {
        ok &= (gamMemoryOut0[i] == i + 1.0F);
        ok &= (gamMemoryOut1[i] == i + 1.0F);
    }

    for (uint32 i = 0u; i < gam.numberOfElements; i++) {
        gamMemoryIn0[i] = 1;
        gamMemoryIn1[i] = 1;
        //gamMemoryOut[i] = 0;
    }
    gam.PrepareNextState("B", "C");
    gam.Execute();
    for (uint32 i = 0u; i < gam.numberOfElements; i++) {
        ok &= (gamMemoryOut0[i] == i + 1.0F);
        ok &= (gamMemoryOut1[i] == i + 1.0F);
    }
    return ok;
}

template<typename T>
bool FilterGAMTest::TestResetOnlyWhenRequiredMemoryNotInit() {
    using namespace MARTe;
    FilterGAMTestHelper<T> gam;
    gam.SetName("Test");
    bool ok = true;

    ok &= gam.InitialiseFilterFIR();
    ok &= gam.Initialise(gam.config);
    ok &= gam.InitialiseConfigDataBaseSignal2();
    ok &= gam.SetConfiguredDatabase(gam.configSignals);

    ok &= gam.AllocateInputSignalsMemory();
    ok &= gam.AllocateOutputSignalsMemory();

    float32 *gamMemoryIn0 = static_cast<float32*>(gam.GetInputSignalsMemory());
    float32 *gamMemoryOut0 = static_cast<float32*>(gam.GetOutputSignalsMemory());
    float32 *gamMemoryIn1 = static_cast<float32*>(gam.GetInputSignalsMemory(1));
    float32 *gamMemoryOut1 = static_cast<float32*>(gam.GetOutputSignalsMemory(1));
    //assign inputs and outputs
    for (uint32 i = 0u; i < gam.numberOfElements; i++) {
        gamMemoryIn0[i] = i;
        gamMemoryOut0[i] = 0;
        gamMemoryIn1[i] = 1;
        gamMemoryOut1[i] = 0;
    }
    if (ok) {
        ok &= !gam.PrepareNextState("", "A");
        ok &= !gam.PrepareNextState("B", "C");
    }
    return ok;
}

}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* TEST_COMPONENTS_GAMS_FILTERGAM_FILTERGAMTEST_H_ */

