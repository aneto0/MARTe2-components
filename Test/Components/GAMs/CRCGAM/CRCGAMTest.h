/**
 * @file CRCGAMTest.h
 * @brief Header file for class CRCGAMTest
 * @date 30/10/2018
 * @author Luca Porzio
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

 * @details This header file contains the declaration of the class CRCGAMTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef TEST_COMPONENTS_GAMS_CRCGAM_CRCGAMTEST_H_
#define TEST_COMPONENTS_GAMS_CRCGAM_CRCGAMTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "CRCGAM.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief Test class for CRCGAM
 */
class CRCGAMTest {
public:

    /**
     * @brief Test default constructor
     */
    bool TestConstructor();

    /**
     * @brief Test initialise function
     */
    bool TestInitialise();

    /**
     * @brief Test the initialise function when all the parameters from
     * configuration are missing
     */
    bool TestInitialiseMissingParameters();

    /**
     * @brief Test the initialise function when the Polynomial
     * parameter is missing
     */
    bool TestInitialiseMissingPolynomial();

    /**
     * @brief Test the initialise function when the InitialValue
     * parameter is missing
     */
    bool TestInitialiseMissingInitialCRCValue();

    /**
     * @brief Test the initialise function when the Inverted
     * parameter is missing
     */
    bool TestInitialiseMissingInverted();

    /**
     * @brief Test the initialise function when the Inverted
     * parameter is greater then 1
     */
    bool TestInitialiseWrongInverted();

    /**
     * @brief Test the Setup function for output type = uint8
     */
    bool TestSetupUint8();

    /**
     * @brief Test the Setup function for output type = uint16
     */
    bool TestSetupUint16();

    /**
     * @brief Test the Setup function for output type = uint32
     */
    bool TestSetupUint32();

    /**
     * @brief Test the setup function when the number of input is zero
     */
    bool TestSetupWrongNumberOfInput();

    /**
     * @brief Test the setup function when the number of output is zero
     */
    bool TestSetupWrongNumberOfOutput();

    /**
     * @brief Test the setup function when the output type is not
     * a supported type
     */
    bool TestSetupWrongOutputType();

    /**
     * @brief Execute test function to be called by TestExecuteUint8,
     * TestExecuteUint16, TestExecuteUint32
     */
    template <typename T>
    bool TestExecute(T value);
    /**
     * @brief Test the execute function for output type = uint8
     */
    bool TestExecuteUint8();

    /**
     * @brief Test the execute function for output type = uint16
     */
    bool TestExecuteUint16();

    /**
     * @brief Test the execute function for output type = uint32
     */
    bool TestExecuteUint32();
};


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

/**
 * @brief Configuration example to be used by Test functions
 */
static const MARTe::char8 * const configFromBasicTypeTemplate = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAM1 = {"
        "            Class = CRCTestHelper"
        "            Polynomial = 0"
        "            InitialValue = 0"
        "            Inverted = 1"
        "            InputSignals = {"
        "               Signal1 = {"
        "                   DataSource = DSH"
        "               }"
        "            }"
        "            OutputSignals = {"
        "               Signal2 = {"
        "                   DataSource = DDB1"
        "                   Type = uint8"
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
        "        +DSH = {"
        "            Class = DataSourceHelper"
        "            Constant = 10"
        "            Signals = {"
        "                Signal1 = {"
        "                    Type = uint8"
        "                }"
        "            }"
        "        }"
        "        +Timings = {"
        "            Class = TimingDataSource"
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


#endif /* TEST_COMPONENTS_GAMS_CRCGAM_CRCGAMTEST_H_ */
	
