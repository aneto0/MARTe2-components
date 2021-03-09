/**
 * @file NI9157CircularFifoReaderTest.h
 * @brief Header file for class NI9157CircularFifoReaderTest
 * @date 11/02/2021
 * @author Giuseppe Ferro
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
 * @details This header file contains the declaration of the class NI9157CircularFifoReaderTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef NI9157CIRCULARFIFOREADERTEST_H_
#define NI9157CIRCULARFIFOREADERTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "NI9157CircularFifoReader.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
using namespace MARTe;

/**
 * @brief Tests the NI9157CircularFifoReader methods
 * @details Compile into the folder TestLabviewFiles and be sure that in the
 * configuration streams of the tests the variable NiDevice.NiRioGenFile points
 * to this folder. Export also the environment variable NI9157_PROJ_DIR to the
 * folder when the dynamic library is created after the compilation.
 */
class NI9157CircularFifoReaderTest {
public:

    /**
     * @brief Constructor.
     */
    NI9157CircularFifoReaderTest();

    /**
     * @brief Destructor.
     */
    virtual ~NI9157CircularFifoReaderTest();

    /**
     * @brief Tests the constructor.
     */
    bool TestConstructor();

    /**
     * @brief Tests the Initialise method.
     */
    bool TestInitialise(uint32 model);

    /**
     * @brief Tests the NI9157CircularFifoReaderTest::Initialise method
     * with the default RunNi parameter.
     */
    bool TestInitialise_DefaultRunNi(uint32 model);

    /**
     * @brief Tests the NI9157CircularFifoReaderTest::Initialise method
     * with the default NumberOfPacketsInFIFO parameter.
     */
    bool TestInitialise_DefaultNumberOfPacketsInFIFO(uint32 model);

    /**
     * @brief Tests the NI9157CircularFifoReaderTest::Initialise method
     * with the default AcquiredFromPacket parameter.
     */
    bool TestInitialise_DefaultAcquiredFromPacket(uint32 model);

    /**
     * @brief Tests the NI9157CircularFifoReaderTest::Initialise method
     * with the default CheckPacketCounter parameter.
     */
    bool TestInitialise_DefaultCheckPacketCounter(uint32 model);

    /**
     * @brief Tests the NI9157CircularFifoReaderTest::Initialise method
     * with the default CheckCounterAfterNPackets parameter.
     */
    bool TestInitialise_DefaultCheckCounterAfterNPackets(uint32 model);

    /**
     * @brief Tests the NI9157CircularFifoReaderTest::Initialise method
     * with the default CounterStep parameter.
     */
    bool TestInitialise_DefaultCounterStep(uint32 model);

    /**
     * @brief Tests the NI9157CircularFifoReaderTest::Initialise method
     * with the default NFrameForSync parameter.
     */
    bool TestInitialise_DefaultNFrameForSync(uint32 model);

    /**
     * @brief Tests the NI9157CircularFifoReaderTest::Initialise method
     * that return false if CounterStep equal to 0.
     */
    bool TestInitialise_False_CounterStepZero(uint32 model);

    /**
     * @brief Tests the NI9157CircularFifoReaderTest::Initialise method
     * that return false with the default NFrameForSync parameter equal to 1.
     */
    bool TestInitialise_False_NFrameForSyncEqualTo1(uint32 model);

    /**
     * @brief Tests the NI9157CircularFifoReaderTest::Initialise method
     * that return false with the CheckCounterAfterNSteps does not divide
     * CounterStep.
     */
    bool TestInitialise_False_CheckAfterNotDivideCounterStep(uint32 model);

    /**
     * @brief Tests the NI9157CircularFifoReaderTest::Initialise method
     * that return false if CounterStep does not divide 
     * AcquireFromCounter-FirstCounter.
     */
    bool TestInitialise_False_CounterStepNotDivideAcquireFromCounterDifference(uint32 model);

    /**
     * @brief Tests the NI9157CircularFifoReaderTest::Initialise method
     * that returns false if the FIFO variable name is not specified.
     */
    bool TestInitialise_False_NoFifoName(uint32 model);

    /**
     * @brief Tests the NI9157CircularFifoReaderTest::Initialise method
     * that returns false if the NI9157Device path is not specified.
     */
    bool TestInitialise_False_NoNI9157Device(uint32 model);

    /**
     * @brief Tests the NI9157CircularFifoReaderTest::Initialise method
     * that returns false if the NI9157Devicepath is invalid.
     */
    bool TestInitialise_False_Invalidi9157Device(uint32 model);

    /**
     * @brief Tests the NI9157CircularFifoReaderTest::SetConfiguredDatabase
     * method.
     */
    bool TestSetConfiguredDatabase(uint32 model);

    /**
     * @brief Tests the NI9157CircularFifoReaderTest::SetConfiguredDatabase
     * method that returns false if more than one channel is specified.
     */
    bool TestSetConfiguredDatabase_False_MoreThanOneChannel(uint32 model);

    /**
     * @brief Tests the NI9157CircularFifoReaderTest::SetConfiguredDatabase
     * method that returns false if the NI9157 operator cannot be found
     * (unsupported type).
     */
    bool TestSetConfiguredDatabase_False_NiDevOperatorNotFound(uint32 model);

    /**
     * @brief Tests the NI9157CircularFifoReaderTest::SetConfiguredDatabase
     * method that returns false if the FIFO variable name is not valid.
     */
    bool TestSetConfiguredDatabase_False_InvalidFifoName(uint32 model);

    /**
     * @brief Tests the NI9157CircularFifoReaderTest::PrepareNextState
     * method.
     */
    bool TestPrepareNextState(uint32 model);

    /**
     * @brief Test the NI9157CircularFifoReaderTest::StopAcquisition method.
     */
    bool TestStopAcquisition(uint32 model);

    /**
     * @brief Test the NI9157CircularFifoReaderTest::StartAcquisition method.
     */
    bool TestStartAcquisition(uint32 model);

    /**
     * @brief Test the NI9157CircularFifoReaderTest::Synchronise method.
     */
    bool TestSynchronise(uint32 model);

    /**
     * @brief Tests the NI9157CircularFifoReaderTest::DriverRead method.
     */
    bool TestDriverRead(uint32 model);

    /**
     * @brief Tests the NI9157CircularFifoReaderTest::DriverRead method
     * with all the allowed signals.
     */
    bool TestDriverRead_AllSignals(uint32 model);

    /**
     * @brief Tests the NI9157CircularFifoReaderTest::DriverRead method
     * defining AcquiredFromCounter > 1.
     */
    bool TestDriverRead_AcquiredFromCounter(uint32 model);

    /**
     * @brief Tests the NI9157CircularFifoReaderTest::DriverRead method
     * performing the complete application cycle.
     */
    bool TestDriverReadCompleteCycle(uint32 model);

    /**
     * @brief Tests the NI9157CircularFifoReaderTest::DriverRead method
     * when the synchronisation fails and the re-sync procedure must be
     * executed.
     */
    bool TestDriverRead_Resync(uint32 model);

    /**
     * @brief Tests the NI9157CircularFifoReaderTest::DriverRead method
     * without doing packet counter control.
     */
    bool TestDriverRead_NoCheckCounter(uint32 model);

    /**
     * @brief Tests the NI9157CircularFifoReaderTest::DriverRead method
     * with CheckAfterNPackets greater than CounterStep.
     */
    bool TestDriverRead_CheckAfterNPackets(uint32 model);

    /**
     * @brief Tests the NI9157CircularFifoReaderTest::DriverRead method
     * doing the de-interleaving operation in the internal thread.
     */
    bool TestDriverRead_InternalInterleaved(uint32 model);

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* NI9157CIRCULARFIFOREADERTEST_H_ */
