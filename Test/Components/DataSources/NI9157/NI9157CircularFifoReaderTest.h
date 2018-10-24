/**
 * @file NI9157CircularFifoReaderTest.h
 * @brief Header file for class NI9157CircularFifoReaderTest
 * @date 17/05/2018
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
 * @details Compile into the folder TestLabviewFiles and be sure that in the configuration streams of the tests the variable NiDevice.NiRioGenFile points to this
 * folder. Export also the environment variable NI9157_PROJ_DIR to the folder when the dynamic library is created after the compilation.
 */
class NI9157CircularFifoReaderTest {
public:

    /**
     * @brief Constructor
     */
    NI9157CircularFifoReaderTest();

    /**
     * @brief Destructor
     */
    virtual ~NI9157CircularFifoReaderTest();

    /**
     * @brief Tests thw constructor
     */
    bool TestConstructor();

    /**
     * @brief Tests the Initialise method
     */
    bool TestInitialise();

    /**
     * @brief Tests the Initialise method with the default RunNi parameter
     */
    bool TestInitialise_DefaultRunNi();

    /**
     * @brief Tests the Initialise method with the default NumberOfPacketsInFIFO parameter
     */
    bool TestInitialise_DefaultNumberOfPacketsInFIFO();

    /**
     * @brief Tests the Initialise method with the default AcquiredFromPacket parameter
     */
    bool TestInitialise_DefaultAcquiredFromPacket();

    /**
     * @brief Tests the Initialise method with the default CheckPacketCounter parameter
     */
    bool TestInitialise_DefaultCheckPacketCounter();

    /**
     * @brief Tests the Initialise method with the default CheckCounterAfterNPackets parameter
     */
    bool TestInitialise_DefaultCheckCounterAfterNPackets();

    /**
     * @brief Tests the Initialise method with the default CounterStep parameter
     */
    bool TestInitialise_DefaultCounterStep();

    /**
     * @brief Tests the Initialise method with the default NFrameForSync parameter
     */
    bool TestInitialise_DefaultNFrameForSync();

    /**
     * @brief Tests the Initialise method that return false if CounterStep equal to 0
     */
    bool TestInitialise_False_CounterStepZero();

    /**
     * @brief Tests the Initialise method that return false with the default NFrameForSync parameter equal to 1
     */
    bool TestInitialise_False_NFrameForSyncEqualTo1();

    /**
     * @brief Tests the Initialise method that return false with the CheckCounterAfterNSteps
     * does not divide CounterStep
     */
    bool TestInitialise_False_CheckAfterNotDivideCounterStep();

    /**
     * @brief Tests the Initialise method that return false if CounterStep
     * does not divide AcquireFromCounter-FirstCounter
     */
    bool TestInitialise_False_CounterStepNotDivideAcquireFromCounterDifference();

    /**
     * @brief Tests the Initialise method that returns false if the FIFO variable name is not specified
     */
    bool TestInitialise_False_NoFifoName();

    /**
     * @brief Tests the Initialise method that returns false if the NI9157Device path is not specified
     */
    bool TestInitialise_False_NoNI9157Device();

    /**
     * @brief Tests the Initialise method that returns false if the NI9157Device path is invalid
     */
    bool TestInitialise_False_Invalidi9157Device();

    /**
     * @brief Tests the SetConfiguredDatabase method
     */
    bool TestSetConfiguredDatabase();

    /**
     * @brief Tests the SetConfiguredDatabase method that returns fale if more than one channel is specified
     */
    bool TestSetConfiguredDatabase_False_MoreThanOneChannel();

    /**
     * @brief Tests the SetConfiguredDatabase method that returns false if the NI9157 operator cannot be found (unsupported type)
     */
    bool TestSetConfiguredDatabase_False_NiDevOperatorNotFound();

    /**
     * @brief Tests the SetConfiguredDatabase method that returns false if the FIFO variable name is not valid
     */
    bool TestSetConfiguredDatabase_False_InvalidFifoName();


    /**
     * @brief Tests the PrepareNextState method
     */
    bool TestPrepareNextState();

    /**
     * @brief Tests the DriverRead method
     */
    bool TestDriverRead();

    /**
     * @brief Tests the DriverRead method with all the allowed signals
     */
    bool TestDriverRead_AllSignals();

    /**
     * @brief Tests the DriverRead method defining AcquiredFromCounter>1
     */
    bool TestDriverRead_AcquiredFromCounter();

    /**
     * @brief Tests the DriverRead method performing the complete application cycle
     */
    bool TestDriverReadCompleteCycle();

    /**
     * @brief Tests the DriverRead method when the synchronisation fails and the re-sync
     * procedure must be executed
     */
    bool TestDriverRead_Resync();

    /**
     * @brief Tests the DriverRead method without doing packet counter control
     */
    bool TestDriverRead_NoCheckCounter();

    /**
     * @brief Tests the DriverRead method with CheckAfterNPackets greater than CounterStep
     */
    bool TestDriverRead_CheckAfterNPackets();

    /**
     * @brief Tests the DriverRead method doing the de-interleaving operation in the
     * internal thread. The acquiring frequency has been set to 2MHz (instead of 200KHz of the other tests)
     */
    bool TestDriverRead_InternalInterleaved();

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* NI9157CIRCULARFIFOREADERTEST_H_ */

