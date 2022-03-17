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

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
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
     * @brief Tests default constructor.
     * @details checks all the post conditions
     */
    bool TestConstructor();
    /**
     * @brief Tests an arbitrary initialisation configuration.
     * @details Verifies the post-conditions
     * @return true on succeed.
     */
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
    bool TestStaticGainFIR();

    /**
     * @brief Tests that the static gain is set properly with IIR filter
     */
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
    bool TestSetup();

    /**
     * @brief Tests that the Setup returns false if the wrong input signal type is asked for.
     */
    bool TestSetupWrongInputSignalType();

    /**
     * @brief Tests that the Setup returns false if the wrong output signal type is asked for.
     */
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
    bool TestExecuteNoFilter();
    /**
     * @brief test the Execute() with a simple two samples average filter.
     * @details first Initialise() and Setup() the filter. Using a constant input, tests Execute() comparing the
     * output against the expected values.
     * @return true when the output is as expected.
     */
    bool TestExecuteFIRConstantInput();

    /**
     * @brief test the Execute() with a simple two samples average filter.
     * @details first Initialise() and Setup() the filter. Using a constant input, tests Execute() comparing the
     * output against the expected values.
     * @return true when the output is as expected.
     */
    bool TestExecuteFIRConstantInputInputElements1();

    /**
     * @brief test the Execute() with a simple average filter.
     * @details first Initialise() and Setup() the filter. Using a ramp input, tests Execute() comparing the
     * output against the expected values.
     * @return true when the output is as expected.
     */
    bool TestExecuteFIRRampInput();

    /**
     * @brief test the Execute() with a simple average filter.
     * @details first Initialise() and Setup() the filter. Using a ramp input, tests Execute() comparing the
     * output against the expected values.
     * @return true when the output is as expected.
     */
    bool TestExecuteFIRRampInputInputElements1();

    /**
     * @brief test the Execute() with a simple 10 samples average filter.
     * @details first Initialise() and Setup() the filter. Using a ramp input, tests Execute() comparing the
     * output against the expected values.
     * @return true when the output is as expected.
     */
    bool TestExecuteAVG10RampInputInputElements1();

    /**
     * @brief test the Execute() with a simple average filter.
     * @details first Initialise() and Setup() the filter. Using a constant input, tests Execute() comparing the
     * output against the expected values. In this case Execute() is called twice in order to check that the last
     * state is correctly remembered.
     * @return true when the output is as expected.
     */
    bool TestExecuteFIRConstantInput2();

    /**
     * @brief test the Execute() with a simple average filter.
     * @details first Initialise() and Setup() the filter. Using a ramp input, tests Execute() comparing the
     * output against the expected values. In this case Execute() is called twice in order to check that the last
     * state is correctly remembered.
     * @return true when the output is as expected.
     */
    bool TestExecuteFIRRampInput2();

    /**
     * @brief test the Execute() with a simple FIR low pass filter with different coefficients (num[0] != num[1].
     * @details first Initialise() and Setup() the filter. Using a ramp input, tests Execute() comparing the
     * output against the expected values. In this case Execute() is called twice in order to check that the last
     * state is correctly remembered.
     * @return true when the output is as expected.
     */
    bool TestExecuteFIRRampInput2DiffCoef();

    /**
     * @brief test the Execute() with a simple integrater (sum(inputs) .
     * @details first Initialise() and Setup() the filter. Using a constant input, tests Execute() comparing the
     * output against the expected values. Notice he output is a ramp.
     * @return true when the output is as expected.
     */
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
    bool TestExecuteIIRInputRamp1InputElement();

    /**
     * @brief test the Execute() with a simple integrator (sum(inputs)) .
     * @details first Initialise() and Setup() the filter. Using a constant input, tests Execute() comparing the
     * output against the expected values. n this case Execute() is called twice in order to check that the last
     * state is correctly remembered
     * @return true when the output is as expected.
     */
    bool TestExecuteIIRConstant2();

    /**
     * @brief Test the Execute with dimension input array = 1
     * @return true when the output is as expected
     */
    bool TestExecuteIIRConstantDimArray1();

    /**
     * @brief Tests Execute with input elements 1 and input samples 10
     * @details the filterGAM output configuration  supported is samples = 1
     * @return true if the data output is as expected.
     */
    bool TestExecuteElements1Samples10();

    /**
     * @brief Tests arbitrary GAM configuration with two input (and output) signals.
     * @return true on succeed.
     */
    bool TestSetupSeveralSignals();

    /**
     * @brief Tests errors with two input signals.
     * @details Configure the GAM with different number of input elements.
     * @return True if Setup() fails.
     */
    bool TestSetupSeveralSignalsDiffNumberOfInputElements();

    /**
     * @brief Tests errors with two input signals.
     * @details Configure the GAM with different number of output elements.
     * @return True if Setup()fails.
     */
    bool TestSetupSeveralSignalsDiffNumberOfOutputElements();

    /**
     * @brief Tests errors with two input signals.
     * @details Configure the GAM with different number of input samples.
     * @return True if Setup()fails.
     */
    bool TestSetupSeveralSignalsDiffInputSamples();

    /**
     * @brief Tests a FIR filter with 2 input signals.
     * @details Apply two times the Execute() with different input signals and then compare the
     * output against the expected values.
     * @return True if the output is as expected.
     */
    bool TestExecuteSeveralSignalsFIR();

    /**
     * @brief Tests the reset FIR filter with resetInEachState = true;
     * @details The sequence tested is filter, reset filter and then filter again. The output of
     * the filter is check against the expected values
     * @return true if the output values are as expected.
     */
    bool TestAlwaysResetFIR();

    /**
     * @brief Tests the reset IIR filter with resetInEachState = true;
     * @detail The sequence tested is filter, reset filter and then filter again. The output of
     * the filter is check against the expected values
     * @return true if the output values are as expected.
     */
    bool TestAlwaysResetIIR();

    /**
     * @brief Tests errors resetInEachState = true.
     * @details Call function PrepareNextState() without allocating memory for the last state.
     * @return true if PrepareNextState() fails.
     */
    bool TestAlwaysResetMemoryNotInt();

    /**
     * @brief Tests the reset FIR filter with resetInEachState = false;
     * @details The sequence tested is filter, reset filter and then filter again. The output of
     * the filter is check against the expected values. The simulated state machin is ""->"A"->"B". Consequently the filter should no be reset
     * @return true if the output values are as expected.
     */
    bool TestResetOnlyWhenRequired();

    /**
     * @brief Tests the reset FIR filter with resetInEachState = false;
     * @details The sequence tested is filter, reset filter and then filter again. The output of
     * the filter is check against the expected values. The simulated state machin is ""->"A"->"C". Consequently the filter should be reset
     * @return true if the output values are as expected.
     */
    bool TestResetOnlyWhenRequired2();

    /**
     * @brief Tests the reset IIR filter with resetInEachState = false
     * @details The sequence tested is filter, reset filter and then filter again. The output of
     * the filter is check against the expected values. The simulated state machin is ""->"A"->"C". Consequently the filter should be reset
     * @return true if the output values are as expected.
     */
    bool TestResetOnlyWhenRequired3();

    /**
     * @brief Tests errors with resetInEachState = false
     * @details Call function PrepareNextState() without allocating memory for the last state.
     * @return true if PrepareNextState() fails.
     */
    bool TestResetOnlyWhenRequiredMemoryNotInit();
private:
    bool MyIsEqual(MARTe::float32 f1,
                   MARTe::float32 f2);
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* TEST_COMPONENTS_GAMS_FILTERGAM_FILTERGAMTEST_H_ */

