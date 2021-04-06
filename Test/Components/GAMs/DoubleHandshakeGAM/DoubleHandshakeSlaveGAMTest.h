/**
 * @file DoubleHandshakeSlaveGAMTest.h
 * @brief Header file for class DoubleHandshakeSlaveGAMTest
 * @date 13/08/2019
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

 * @details This header file contains the declaration of the class DoubleHandshakeSlaveGAMTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef TEST_COMPONENTS_GAMS_DOUBLEHANDSHAKEGAM_DOUBLEHANDSHAKESLAVEGAMTEST_H_
#define TEST_COMPONENTS_GAMS_DOUBLEHANDSHAKEGAM_DOUBLEHANDSHAKESLAVEGAMTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "DoubleHandshakeSlaveGAM.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

class DoubleHandshakeSlaveGAMTest {
public:
    /**
    * @brief Constructor for the test 
    */    
    DoubleHandshakeSlaveGAMTest();

    /**
    * @brief Destructor for the test
    */    
    virtual ~DoubleHandshakeSlaveGAMTest();

    /**
    * @brief Tests the GAM constructor
    */        
    bool TestConstructor();

    /**
    * @brief Tests the setup method
    */    
    bool TestSetup();

    /**
    * @brief Tests with an useless signal
    */    
    bool TestSetup_UselessSignal();

    /**
    * @brief Tests the number of elements
    */    
    bool TestSetup_NumberOfElements();

    /**
    * @brief Tests the failure with number of commands differing from acknowledges
    */    
    bool TestSetup_FalseNCommansDiffThanNAcks();

    /**
    * @brief Tests the failure with number of commands differing from clears
    */    
    bool TestSetup_FalseNCommansDiffThanNClears();

    /**
    * @brief Tests the failure with number of commands differing from statuses
    */    
    bool TestSetup_FalseNCommansDiffThanNStatuses();

    /**
    * @brief Tests the failure with an invalid type in the triggers signals
    */    
    bool TestSetup_FalseClearTrigsWrongType();

    /**
    * @brief Tests the failure with an invalid type in internal states signals
    */    
    bool TestSetup_FalseInternalStateWrongType();

    /**
    * @brief Tests the failure when the acknowledge is mismatching
    */    
    bool TestSetup_FalseCommandAndAckMismatch();

    /**
    * @brief Tests the GAM execution
    */    
    bool TestExecute();

    /**
    * @brief Tests the state change to Error from Sending
    */    
    bool TestExecute_ErrorFromSending();

    /**
    * @brief Tests the state change to Error from Done
    */    
    bool TestExecute_ErrorFromDone();

    /**
    * @brief Tests the state change to Error from Sending Timeout
    */    
    bool TestExecute_ErrorFromSendingTimeout();

    /**
    * @brief Tests the state change to Error from Done Timeout
    */    
    bool TestExecute_ErrorFromDoneTimeout();

    /**
    * @brief Tests the state change and recover from Error
    */    
    bool TestExecute_RecoverFromError();

    /**
    * @brief Tests the GAM execution in normal running
    */    
    bool TestExecute_Interactive();

    /**
    * @brief Same as TestExecute() where the Initialise method is used
    */
    bool TestInitialise();
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* TEST_COMPONENTS_GAMS_DOUBLEHANDSHAKEGAM_DOUBLEHANDSHAKESLAVEGAMTEST_H_ */

