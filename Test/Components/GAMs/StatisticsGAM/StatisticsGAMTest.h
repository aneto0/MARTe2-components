/**
 * @file StatisticsGAMTest.h
 * @brief Header file for class StatisticsGAMTest
 * @date 13/03/2017
 * @author Bertrand Bauvir
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

 * @details This header file contains the declaration of the class StatisticsGAMTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef STATISTICSGAMTEST_H_
#define STATISTICSGAMTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
/**
 * @brief Tests the StatisticsGAM public methods.
 */
class StatisticsGAMTest {
public:
    /**
     * @brief Constructor. NOOP.
     */
    StatisticsGAMTest();

    /**
     * @brief Destructor. NOOP.
     */
    virtual ~StatisticsGAMTest();

    /**
     * @brief Tests SquareRoot<> methods.
     */
    bool TestSquareRoot();

    /**
     * @brief Tests default constructor.
     * @details checks all the post conditions
     */
    bool TestConstructor();

    /**
     * @brief Tests an arbitrary initialisation configuration.
     * @details Verifies the post-conditions
     * @return true on succeed.
     */
    bool TestInitialise();

    /**
     * @brief Tests an initialisation with parameter.
     * @return true if the initialisation succeed & ResetInEachState = true (but a warning must be lunched).
     */
    bool TestInitialise_WindowSize();

    /**
     * @brief Tests that a correct default setup works
     * @details Verify the post-conditions
     * @return true if Setup returns true.
     */
    bool TestSetup_float32();

    /**
     * @brief Tests the Setup() method
     * @details Verify the post-conditions
     * @return true if Setup() fails.
     */
    bool TestSetup_NoInputSignal();

    /**
     * @brief Tests the Setup() method
     * @details Verify the post-conditions
     * @return true if Setup() fails.
     */
    bool TestSetup_NoOutputSignal();

    /**
     * @brief Tests the Setup() method
     * @details Verify the post-conditions
     * @return true if Setup() fails.
     */
    bool TestSetup_NotAScalarInput();

    /**
     * @brief Tests the Setup() method
     * @details Verify the post-conditions
     * @return true if Setup() fails.
     */
    bool TestSetup_NotAScalarOutput();

    /**
     * @brief Tests the Setup() method
     * @details Verify the post-conditions
     * @return true if Setup() fails.
     */
    bool TestSetup_DistinctTypes_1();

    /**
     * @brief Tests the Setup() method
     * @details Verify the post-conditions
     * @return true if Setup() fails.
     */
    bool TestSetup_DistinctTypes_2();

    /**
     * @brief Tests the Execute method.
     * @return true if Execute() fails.
     */
    bool TestExecute_Error();

    /**
     * @Tests the execute method against anytype
     */
    template<typename Type> bool TestExecute_AnyType(
            Type value,
            MARTe::uint32 cycleCounter = 0u,
            bool infiniteMaxMin = false
            );

    /**
     * @brief Tests the Execute method.
     * @return true if computed statistics are as expected.
     */
    bool TestExecute_uint8();

    /**
     * @brief Tests the Execute method.
     * @return true if computed statistics are as expected.
     */
    bool TestExecute_int8();

    /**
     * @brief Tests the Execute method.
     * @return true if computed statistics are as expected.
     */
    bool TestExecute_uint16();

    /**
     * @brief Tests the Execute method.
     * @return true if computed statistics are as expected.
     */
    bool TestExecute_int16();

    /**
     * @brief Tests the Execute method.
     * @return true if computed statistics are as expected.
     */
    bool TestExecute_int32();

    /**
     * @brief Tests the Execute method.
     * @return true if computed statistics are as expected.
     */
    bool TestExecute_uint32();

    /**
     * @brief Tests the Execute method.
     * @return true if computed statistics are as expected.
     */
    bool TestExecute_int64();

    /**
     * @brief Tests the Execute method.
     * @return true if computed statistics are as expected.
     */
    bool TestExecute_uint64();

    /**
     * @brief Tests the Execute method.
     * @return true if computed statistics are as expected.
     */
    bool TestExecute_float32();

    /**
     * @brief Tests the Execute method.
     * @return true if computed statistics are as expected.
     */
    bool TestExecute_float64();

    /**
     * @brief Tests the reset of the sample buffer.
     * @return true if PrepareForNextState() fails.
     */
    bool TestPrepareForNextState_Error();

    /**
     * @brief Tests the reset of the sample buffer.
     * @return true if the sample buffer is reset.
     */
    bool TestPrepareForNextState_Success();

    /**
     * @brief Tests the effect of the Cycle Counter starting offset
     */
    bool TestExecute_uint32_withCycleCounter();

    /**
     * @brief Tests the absolute Max/Min handling mode
     */
    bool TestExecute_uint32_withAbsoluteMaxMin();
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* STATISTICSGAMTEST_H_ */

