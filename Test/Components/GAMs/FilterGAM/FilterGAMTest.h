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
     * @brief Tests an arbitrary initialisation works
     */
    bool TestInitialise();

    /**
     * @brief Tests the number which indicates the quantity of coefficients of the numerator is correctly set.
     */
    bool TestNumberOfNumCoeff();

    /**
     * @brief Tests the values of the numerator coefficients are properly set.
     */
    bool TestValuesOfNumCoeff();

    /**
     * @brief Tests an initialization with no numerator coefficients.
     * @return true if the initialization fails.
     */
    bool TestInitialiseNum0();

    /**
     * @brief Tests the number which indicates the quantity of coefficients of the denominator is correctly set.
     */
    bool TestNumberOfDenCoeff();

    /**
     * @brief Tests the values of the denominator coefficients are properly set.
     */
    bool TestValuesOfDenCoeff();

    /**
     * @brief Tests an initialization with no denominator coefficients.
     * @return true if the initialization fails.
     */
    bool TestInitialiseDen0();

    /**
     * @brief Tests that the static gain is set properly
     */
    bool TestStaticGain();

    /**
     * @brief Tests the error normalise
     * @return true when initailise fails due to CheckNormalisation fails
     */
    bool TestFailNormalise();

    /**
     * @brief Tests a correct default setup works
     * @return true  if Setup returns true.
     */
    bool TestSetup();

    /**
     * @brief Tests errors regarding to wrong GAM input setup
     * @details call the function Setup() without initialising the inputs in the database.
     * @return true if the Setup() fails.
     */
    bool TestSetupFailInputs();

    /**
     * @brief Tests errors regarding to wrong GAM output setup
     * @details call the function Setup() without initialising the GAM output in the database.
     * @return true if the Setup() fails.
     */
    bool TestSetupFailOutputs();

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
     * @brief test the Execute() with a simple integrater (sum(inputs) .
     * @details first Initialise() and Setup() the filter. Using a constant input, tests Execute() comparing the
     * output against the expected values. n this case Execute() is called twice in order to check that the last
     * state is correctly remembered
     * @return true when the output is as expected.
     */
    bool TestExecuteIIRConstant2();
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* TEST_COMPONENTS_GAMS_FILTERGAM_FILTERGAMTEST_H_ */

