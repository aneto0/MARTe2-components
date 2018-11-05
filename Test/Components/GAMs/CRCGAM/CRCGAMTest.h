/**
 * @file CRCGAMTest.h
 * @brief Header file for class CRCGAMTest
 * @date Oct 30, 2018 TODO Verify the value and format of the date
 * @author root TODO Verify the name and format of the author
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

class CRCGAMTest {
public:

    bool TestConstructor();

    bool TestInitialise();

    bool TestInitialiseMissingParameters();

    bool TestInitialiseMissingPolynomial();

    bool TestInitialiseMissingInitialCRCValue();

    bool TestInitialiseMissingInverted();

    bool TestSetup();

    bool TestSetupWrongNumberOfInput();

    bool TestSetupWrongNumberOfOutput();

    bool TestSetupWrongOutputType();

    bool TestExecuteUint8();

    bool TestExecuteUint16();

    bool TestExecuteUint32();
};


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

static const MARTe::char8 * const configFromBasicTypeTemplate = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        +Constant1 = {"
        "            Class = ConstantGAM"
        "            OutputSignals = {"
        "                Signal1 = {"
        "                    DataSource = DDB1"
        "                    Type = uint32"
        "                    Default = 10"
        "                }"
        "            }"
        "        }"
        "        Class = ReferenceContainer"
        "        +GAM1 = {"
        "            Class = CRCTestHelper"
        "            Polynomial = 0"
        "            InitialValue = 0"
        "            Inverted = 0"
        "            InputSignals = {"
        "               Signal1 = {"
        "                   DataSource = DDB1"
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
        "                    Functions = {Constant1 GAM1}"
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
	
