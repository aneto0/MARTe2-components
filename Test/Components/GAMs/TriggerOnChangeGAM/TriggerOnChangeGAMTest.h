/**
 * @file TriggerOnChangeGAMTest.h
 * @brief Header file for class TriggerOnChangeGAMTest
 * @date 17/07/2018
 * @author Giuseppe Ferro
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

 * @details This header file contains the declaration of the class TriggerOnChangeGAMTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef TRIGGERONCHANGEGAMTEST_H_
#define TRIGGERONCHANGEGAMTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "TriggerOnChangeGAM.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

/**
 * @brief Tests all the TriggerOnChangeGAM methods
 */
class TriggerOnChangeGAMTest {
public:

    /**
     * @brief Constructor
     */
    TriggerOnChangeGAMTest();

    /**
     * @brief Destructor
     */
    virtual ~TriggerOnChangeGAMTest();

    /**
     * @brief Tests the constructor
     */
    bool TestConstructor();

    /**
     * @brief Tests the TriggerOnChangeGAM::Setup method
     */
    bool TestSetup();

    /**
     * @brief Tests the TriggerOnChangeGAM::Setup method with more
     * than one command
     */
    bool TestSetup_MoreCommands();

    /**
     * @brief Tests the TriggerOnChangeGAM::Setup method with also variables
     * that are not commands
     */
    bool TestSetup_NotAllCommands();

    /**
     * @brief Tests the TriggerOnChangeGAM::Setup method with array of commands
     */
    bool TestSetup_InputCommandArray();

    /**
     * @brief Tests the TriggerOnChangeGAM::Setup method that fails if no command
     * is present
     */
    bool TestSetup_False_NoCommands();

    /**
     * @brief Tests the TriggerOnChangeGAM::Setup method that fails if the last output
     * signal has a number of elements different than the number of commands.
     */
    bool TestSetup_False_WrongStateNElements();

    /**
     * @brief Tests the TriggerOnChangeGAM::Setup method that fails if the last output
     * signal is not uint32
     */
    bool TestSetup_False_WrongStateType();

    /**
     * @brief Tests the TriggerOnChangeGAM::Setup method that fails if the input size is different
     * than the output size except for the last signal size.
     */
    bool TestSetup_False_WrongIOSizeMatch();

    /**
     * @brief Tests the TriggerOnChangeGAM::Execute method
     */
    bool TestExecute();

    /**
     * @brief Tests the TriggerOnChangeGAM::Execute method with more than one command
     */
    bool TestExecute_MoreCommands();

    /**
     * @brief Tests the TriggerOnChangeGAM::Execute method with also variables that are not
     * commands
     */
    bool TestExecute_NotAllCommands();

    /**
     * @brief Tests the TriggerOnChangeGAM::Execute method with array of commands
     */
    bool TestExecute_InputCommandArray();

    /**
     * @brief Tests the TriggerOnChangeGAM::Execute method with array of commands and
     * also other variables
     */
    bool TestExecute_NotAllCommandsArray();

    /**
     * @brief Tests the TriggerOnChangeGAM::Execute method where messages with
     * indirect replies are sent
     */
    bool TestExecute_WaitIndirectReply();

    /**
     * @brief Tests the TriggerOnChangeGAM::Execute method where messages with
     * indirect replies are sent and more than one command
     */
    bool TestExecute_MoreCommandsWaitIndirectReply();

    /**
     * @brief Tests the TriggerOnChangeGAM::Execute method where messages with
     * indirect replies are sent and also variables that are not commands
     */
    bool TestExecute_NotAllCommandsWaitIndirectReply();

    /**
     * @brief Tests the TriggerOnChangeGAM::Execute method triggering the ERROR state
     */
    bool TestExecute_ErrorState();

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* TRIGGERONCHANGEGAMTEST_H_ */

