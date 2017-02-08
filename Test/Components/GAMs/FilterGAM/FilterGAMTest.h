/**
 * @file FilterGAMTest.h
 * @brief Header file for class FilterGAMTest
 * @date Jan 30, 2017 TODO Verify the value and format of the date
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

class FilterGAMTest {
//TODO Add the macro DLL_API to the class declaration (i.e. class DLL_API FilterGAMTest)
public:
    FilterGAMTest();
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
     * @brief Tests an initialization with no numerator coefficients.
     * @return true if the initialization fails.
     */
    bool TestInitialiseNum0();

    /**
     * @brief Tests an initialization with no denominator coefficients.
     * @return true if the initialization fails.
     */
    bool TestInitialiseDen0();

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
     * @brief test the Execute() with a simple average filter.
     * @details first Initialise() and Setup() the filter. Using a constant input, tests Execute() comparing the
     * output against the expected values.
     * @return true when the output is as expected.
     */
    bool TestExecuteFIRConstantInput();

    /**
     * @brief test the Execute() with a simple average filter.
     * @details first Initialise() and Setup() the filter. Using a ramp input, tests Execute() comparing the
     * output against the expected values.
     * @return true when the output is as expected.
     */
    bool TestExecuteFIRRampInput();


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
     * @brief test the Execute() with a simple integrater (sum(inputs) .
     * @details first Initialise() and Setup() the filter. Using a constant input, tests Execute() comparing the
     * output against the expected values. Notice he output is a ramp.
     * @return true when the output is as expected.
     */
    bool TestExecuteIIRConstant();

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
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* TEST_COMPONENTS_GAMS_FILTERGAM_FILTERGAMTEST_H_ */

