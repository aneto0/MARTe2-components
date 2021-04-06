/**
 * @file MessageGAMTest.h
 * @brief Header file for class MessageGAMTest
 * @date 02/07/2019
 * @author Giuseppe Ferrò
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

 * @details This header file contains the declaration of the class MessageGAMTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef TEST_COMPONENTS_GAMS_MESSAGEGAM_MESSAGEGAMTEST_H_
#define TEST_COMPONENTS_GAMS_MESSAGEGAM_MESSAGEGAMTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "MessageGAM.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

class MessageGAMTest {

public:
    MessageGAMTest();
    virtual ~MessageGAMTest();

    /**
    * @brief Tests the constructor
    */
    bool TestConstructor();
    
    /**
    * @brief Fully constructs the GAM, configuring it
    */
    bool TestSetup();

    /**
    * @brief Same as TestSetup, which represents a better scenario over the simple Initialise call
    */    
    bool TestInitialise();

    /**
    * @brief Tests if the GAM correctly tells apart commands and events by counting them
    */
    bool TestSetupNumberOfElements();

    /**
    * @brief Tests the failure in case of missing commands
    */
    bool TestSetup_FalseNoCommands();

    /**
    * @brief Tests the failure in mismatching input and outputs
    */
    bool TestSetup_FalseInOutMismatch();

    /**
    * @brief Tests the failure if the state signal has a bad type
    */
    bool TestSetup_FalseStateBadType();

    /**
    * @brief Tests the integrated execution
    */
    bool TestExecute();

    /**
    * @brief Tests the integrated execution by causing the issue of multiple commands
    */
    bool TestExecute_MoreCommands();

    /**
    * @brief Tests the integrated execution by changing commands across states
    */
    bool TestExecute_CommandsAndStates();

    /**
    * @brief Same as execute, which is already calling PrepareNextState
    */
    bool TestPrepareNextState();

    /**
    * @brief Same as execute, which among others is already calling Purge method
    */
    bool TestPurge();

    /**
    * @brief Tests the integrated execution validating command ack
    */
    bool TestExecute_Commands();

    /**
    * @brief Tests the integrated execution with the TriggerOnChange parameter disabled
    */
    bool TestExecute_WithoutTriggerOnChange();

    /**
    * @brief Configures and gets the number of commands to tell if the command recognition is correctly working
    */
    bool TestGetNumberOfCommands();

    /**
    * @brief Configures and gets the number of commands to tell if the events recognition is correctly working
    */
    bool TestGetNumberOfEvents();

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* TEST_COMPONENTS_GAMS_MESSAGEGAM_MESSAGEGAMTEST_H_ */

