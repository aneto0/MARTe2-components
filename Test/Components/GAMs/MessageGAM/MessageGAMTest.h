/**
 * @file MessageGAMTest.h
 * @brief Header file for class MessageGAMTest
 * @date 02 ago 2019
 * @author pc
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

    bool TestConstructor();

    bool TestSetup();

    bool TestSetupNumberOfElements();

    bool TestSetup_FalseNoCommands();

    bool TestSetup_FalseInOutMismatch();

    bool TestSetup_FalseStateBadType();

    bool TestExecute();

    bool TestExecute_MoreCommands();

    bool TestExecute_CommandsAndStates();

    bool TestExecute_Commands();

    bool TestExecute_WithoutTriggerOnChange();

    bool TestGetNumberOfCommands();

    bool TestGetNumberOfEvents();

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* TEST_COMPONENTS_GAMS_MESSAGEGAM_MESSAGEGAMTEST_H_ */

