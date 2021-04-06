/**
 * @file DoubleHandshakeMasterGAMTest.h
 * @brief Header file for class DoubleHandshakeMasterGAMTest
 * @date 07/08/2019
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

 * @details This header file contains the declaration of the class DoubleHandshakeMasterGAMTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef TEST_COMPONENTS_GAMS_DOUBLEHANDSHAKEGAM_DOUBLEHANDSHAKEMASTERGAMTEST_H_
#define TEST_COMPONENTS_GAMS_DOUBLEHANDSHAKEGAM_DOUBLEHANDSHAKEMASTERGAMTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "DoubleHandshakeMasterGAM.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

class DoubleHandshakeMasterGAMTest {
public:
    /**
    * @brief Constructor for the test case
    */
    DoubleHandshakeMasterGAMTest();

    /**
    * @brief Destructor for the test case
    */
    virtual ~DoubleHandshakeMasterGAMTest();

    /**
    * @brief Tests the constructor
    */
    bool TestConstructor();

    /**
    * @brief Tests the call of the setup method
    */
    bool TestSetup();

    /**
    * @brief Tests the execution in case of an useless signal
    */
    bool TestSetup_UselessSignal();

    /**
    * @brief Tests if the events and commands signal elements are correctly counted
    */
    bool TestSetup_NumberOfElements();

    /**
    * @brief Tests the failure when commands mismatch acks
    */
    bool TestSetup_FalseNInCommandsDiffThanNInAcks();

    /**
    * @brief Tests failure when commands mismatch clear triggers
    */
    bool TestSetup_FalseNInCommandsDiffThanClearTrigs();

    /**
    * @brief Tests failure when triggers are of the wrong type
    */
    bool TestSetup_FalseClearTrigsWrongType();

    /**
    * @brief Tests failure when the internal state signals are of the wrong type
    */
    bool TestSetup_FalseInternalStateWrongType();

    /**
    * @brief Tests failure in mismatching in/out commands
    */
    bool TestSetup_FalseNInCommandsDiffThanNOutCommands();

    /**
    * @brief Tests failure in mismatching commands on out statuses
    */
    bool TestSetup_FalseNInCommandsDiffThanNOutStatus();

    /**
    * @brief Tests failure in mismatching command size on ack
    */
    bool TestSetup_FalseSizeInCommandDiffThanSizeInAck();

    /**
    * @brief Tests failure in mismatching command size on out
    */
    bool TestSetup_FalseSizeInCommandDiffThanSizeOutCommand();

    /**
    * @brief Tests the integrated execution
    */
    bool TestExecute();

    /**
    * @brief Tests the change from ready to error
    */
    bool TestExecute_ErrorFromReady();

    /**
    * @brief Tests the change from sending to error
    */
    bool TestExecute_ErrorFromSending();

    /**
    * @brief Tests the change from clear to error
    */
    bool TestExecute_ErrorFromClear();

    /**
    * @brief Tests the change from done to error
    */
    bool TestExecute_ErrorFromDone();

    /**
    * @brief Tests the change from sending timeout to error
    */
    bool TestExecute_ErrorFromSendingTimeout();

    /**
    * @brief Tests the change from clear timeout to error
    */
    bool TestExecute_ErrorFromClearTimeout();

    /**
    * @brief Tests the change from done timeout to error
    */
    bool TestExecute_ErrorFromDoneTimeout();

    /**
    * @brief Tests the change from error to recover
    */
    bool TestExecute_RecoverFromError();

    /**
    * @brief Tests the execution for multiple cycles
    */
    bool TestExecute_DoubleCycle();

    /**
    * @brief Test the execution interactively
    */
    bool TestExecute_Interactive();

    /**
    * @brief Test the execution interactively
    */
    bool TestExecute_InteractiveManual();

    /**
    * @brief Same as TestExecute() where the Initialise function is called
    */
    bool TestInitialise();
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* TEST_COMPONENTS_GAMS_DOUBLEHANDSHAKEGAM_DOUBLEHANDSHAKEMASTERGAMTEST_H_ */

