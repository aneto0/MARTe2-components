/**
 * @file CounterCheckerTest.h
 * @brief Header file for class CounterCheckerTest.
 * @date 26/03/2021
 * @author Pedro Lourenco
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
 *
 * @details This header file contains the declaration of the class
 * CounterCheckerTest with all of its public, protected and private members.
 * It may also include definitions for inline methods which need to be visible
 * to the compiler.
 */

#ifndef NI9157COUNTERCHECKERTEST_H_
#define NI9157COUNTERCHECKERTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "ConfigurationDatabase.h"
#include "CounterChecker.h"
#include "ObjectRegistryDatabase.h"
#include "StandardParser.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
using namespace MARTe;

/**
 * @brief Tests the CounterCheckerTest methods.
 */
class CounterCheckerTest {
public:

    /**
     * @brief CounterCheckerTest Constructor.
     */
    CounterCheckerTest();

    /**
     * @brief CounterCheckerTest Destructor.
     */
    virtual ~CounterCheckerTest();

    /**
     * @brief Tests the CounterChecker Constructor.
     */
    bool TestConstructor();

    /**
     * @brief Tests the CounterChecker::Initialise method.
     */
    bool TestInitialise();

    /**
     * @brief Tests the CounterChecker::Initialise method which fails when
     * the parameters used by SampleCheck::Initialise fail.
     */
    bool TestInitialise_FailSampleChecker();

    /**
     * @brief Tests the CounterChecker::Initialise method which fails when
     * the parameter NumOfFrameForSync is less than 2.
     */
    bool TestInitialise_FailNumOfFrameForSync();

    /**
     * @brief Tests the CounterChecker::Initialise method when the parameter
     * FirstPacketCounter is not set.
     */
    bool TestInitialise_NoFirstPacketCounter();

    /**
     * @brief Tests the CounterChecker::Initialise method when the parameter
     * AcquireFromCounter is not set.
     */
    bool TestInitialise_NoAcquireFromCounter();

    /**
     * @brief Tests the CounterChecker::Initialise method when the parameter
     * CounterStep is not set.
     */
    bool TestInitialise_NoCounterStep();

    /**
     * @brief Tests the CounterChecker::Initialise method fails when the 
     * parameter CounterSetp is 0.
     */
    bool TestInitialise_CounterStepZero();

    /**
     * @brief Tests the CounterChecker::Initialise method when the parameter
     * CheckCounterAfterNSteps is not set.
     */
    bool TestInitialise_NoCheckCounterAfterNSteps();

    /**
     * @brief Tests the CounterChecker::Initialise method when the parameters
     * CheckCounterAfterNSteps and CounterStep are not divisible.
     */
    bool TestInitialise_NoCheckCounterAfterNStepsDivCounterStep();

    /**
     * @brief Tests the CounterChecker::Initialise method when the parameters
     * (AcquireFromCounter - PacketCounter) % CounterStep != 0.
     */
    bool TestInitialise_NoAcquireFromCounterMinusPacketCounterDivCounterStep();

    /**
     * @brief Tests the CounterChecker::Check method.
     */
    bool TestCheck();

    /**
     * @brief Tests the CounterChecker::Check method whose argument write
     * returns false when the argument sample and the AcquireFromCounter value
     * do not match.
     */
    bool TestCheck_FalseWrite();

    /**
     * @brief Tests the CounterChecker::Check method which retruns false
     * when the packetCounter and the sample value do not match.
     */
    bool TestCheck_ReturnFalse();

    /**
     * @brief Tests the CounterChecker::Check method which retruns true and
     * with argument write true upon consecutive calls (increments).
     */
    bool TestCheck_ConsecutiveCalls();

    /**
     * @brief Tests the CounterChecker::Synchronise method.
     */
    bool TestSynchronise();

    /**
     * @brief Tests the CounterChecker::Synchronise method whose parameter
     * write retruns false.
     */
    bool TestSynchronise_WriteFalse();

    /**
     * @brief Tests the CounterChecker::Synchronise method which retruns false
     * when the argument sizeToRead is zero.
     */
    bool TestSynchronise_FalseSizeToReadZero();

    /**
     * @brief Tests the CounterChecker::Synchronise method which retruns false
     * when the argument frame data is wrong.
     */
    bool TestSynchronise_FalseFrameIsWrong();

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* NI9157COUNTERCHECKERTEST_H_ */
