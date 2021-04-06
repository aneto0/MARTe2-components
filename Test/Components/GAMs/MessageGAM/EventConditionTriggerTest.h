/**
 * @file EventConditionTriggerTest.h
 * @brief Header file for class EventConditionTriggerTest
 * @date 19/07/2018
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

 * @details This header file contains the declaration of the class EventConditionTriggerTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef EVENTCONDITIONTRIGGERTEST_H_
#define EVENTCONDITIONTRIGGERTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "EventConditionTrigger.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

/**
 * @brief Tests all the EventConditionTrigger methods
 */
class EventConditionTriggerTest {
public:

    /**
     * @brief Constructor
     */
    EventConditionTriggerTest();

    /**
     * @brief Destructor
     */
    virtual ~EventConditionTriggerTest();

    /**
     * @brief Tests the constructor
     */
    bool TestConstructor();

    /**
     * @brief Tests the EventConditionTrigger::Initialise method
     */
    bool TestInitialise();

    /**
     * @brief Tests the EventConditionTrigger::Initialise method specifying the CPUMask.
     */
    bool TestInitialise_CPUMask();

    /**
     * @brief Tests the EventConditionTrigger::Initialise method that fails if
     * no EventTrigger block is found
     */
    bool TestInitialise_FalseNoEventTrigger();

    /**
     * @brief Tests the EventConditionTrigger::Initialise method that fails if
     * not only Messages are defined.
     */
    bool TestInitialise_FalseNotOnlyMessages();

    /**
     * @brief Tests the EventConditionTrigger::SetMetadataConfig method
     */
    bool TestSetMetadataConfig();

    /**
     * @brief Tests the EventConditionTrigger::SetMetadataConfig method that fails if
     * the field name does not match with anyone in the metadata information
     */
    bool TestSetMetadataConfig_FalseNoMetadataFieldMatch();

    /**
     * @brief Tests the EventConditionTrigger::SetMetadataConfig method that fails if
     * the read from the configuration fails for the specified field type
     */
    bool TestSetMetadataConfig_FalseReadFailedTypeMismatch();

    /**
     * @brief Tests the EventConditionTrigger::Check method
     */
    bool TestCheck();

    /**
     * @brief Tests the EventConditionTrigger::Execute method
     * with messages with immediate replies
     */
    bool TestExecute_ImmediateReply();

    /**
     * @brief Tests the EventConditionTrigger::Execute method
     * with messages with indirect replies
     */
    bool TestExecute_IndirectReply();

    /**
     * @brief Tests the EventConditionTrigger::Execute method
     * with both type of message replies
     */
    bool TestExecute_Mixed();

    /**
     * @brief Tests the EventConditionTrigger::Replied method
     * with messages with immediate replies
     */
    bool TestReplied_ImmediateReply();

    /**
     * @brief Tests the EventConditionTrigger::Replied method
     * with messages with indirect replies
     */
    bool TestReplied_IndirectReply();

    /**
     * @brief Tests the EventConditionTrigger::Replied method
     * with both message replies
     */
    bool TestReplied_Mixed();

    /**
     * @brief Tests the EventConditionTrigger::GetCPUMask method
     */
    bool TestGetCPUMask();

    /**
    * @brief Same as TestExecute_ImmediateReply which among others already calls Purge
    */
    bool TestPurge();

    /**
    * @brief Same as TestExecute_ImmediateReply, which already uses the EventCondition field
    */
    bool TestEventConditionField();
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* EVENTCONDITIONTRIGGERTEST_H_ */

