/**
 * @file DANSourceTest.h
 * @brief Header file for class DANSourceTest
 * @date 18/04/2017
 * @author Andre Neto
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

 * @details This header file contains the declaration of the class DANSourceTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef DAN_DANSOURCETEST_H_
#define DAN_DANSOURCETEST_H_

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
 * @brief Tests the DANSource public methods.
 */
class DANSourceTest {
public:
    /**
     * @brief Tests the constructor.
     */
    bool TestConstructor();

    /**
     * @brief Tests the AllocateMemory method
     */
    bool TestAllocateMemory();

    /**
     * @brief Tests the GetNumberOfMemoryBuffers method
     */
    bool TestGetNumberOfMemoryBuffers();

    /**
     * @brief Tests the GetSignalMemoryBuffer method
     */
    bool TestGetSignalMemoryBuffer();

    /**
     * @brief Tests that the GetBrokerName method correctly returns an empty string for InputSignals.
     */
    bool TestGetBrokerName_InputSignals();

    /**
     * @brief Tests that the GetBrokerName method correctly returns a MemoryMapAsyncOutputBroker.
     */
    bool TestGetBrokerName_MemoryMapAsyncOutputBroker();

    /**
     * @brief Tests that the GetBrokerName method correctly returns a MemoryMapAsyncTriggerOutputBroker.
     */
    bool TestGetBrokerName_MemoryMapAsyncTriggerOutputBroker();

    /**
     * @brief Tests the GetInputBrokers method
     */
    bool TestGetInputBrokers();

    /**
     * @brief Tests the GetOutputBrokers method
     */
    bool TestGetOutputBrokers();

    /**
     * @brief Tests the DANSource integrated in an application which continuously stores data.
     */
    bool TestIntegratedInApplication_NoTrigger();

    /**
     * @brief Tests the DANSource integrated in an application which asynchronously stores data based on a trigger event.
     */
    bool TestIntegratedInApplication_Trigger();

    /**
     * @brief Tests the DANSource integrated in an application which asynchronously stores data based on a trigger event and using absolute time.
     */
    bool TestIntegratedInApplication_Trigger_AbsoluteTime();

    /**
     * @brief Tests the Synchronise method. This is tested by the TestIntegratedInApplication_NoTrigger.
     */
    bool TestSynchronise();

    /**
     * @brief Tests the PrepareNextState method. This is tested by the TestIntegratedInApplication_Trigger_AbsoluteTime.
     */
    bool TestPrepareNextState();

    /**
     * @brief Tests the Initialise method
     */
    bool TestInitialise();

    /**
     * @brief Tests the SetConfiguredDatabase method
     */
    bool TestSetConfiguredDatabase();

    /**
     * @brief Tests the OpenStream method
     */
    bool TestOpenStream();

    /**
     * @brief Tests the CloseStream method
     */
    bool TestCloseStream();

    /**
     * @brief Tests the GetCPUMask method
     */
    bool TestGetCPUMask();

    /**
     * @brief Tests the GetNumberOfBuffers method
     */
    bool TestGetNumberOfBuffers();

    /**
     * @brief Tests the GetNumberOfPostTriggers method
     */
    bool TestGetNumberOfPostTriggers();

    /**
     * @brief Tests the GetNumberOfPreTriggers method
     */
    bool TestGetNumberOfPreTriggers();

    /**
     * @brief Tests the GetStackSize method
     */
    bool TestGetStackSize();

    /**
     * @brief Tests the IsStoreOnTrigger method
     */
    bool TestIsStoreOnTrigger();

    /**
     * @brief Tests the IsAbsoluteTime method
     */
    bool TestIsAbsoluteTime();

    /**
     * @brief Tests the GetTimeSignalIdx method
     */
    bool TestGetTimeSignalIdx();

    /**
     * @brief Tests the GetDANDataCore method
     */
    bool TestGetDANDataCore();
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* DAN_DANSOURCETEST_H_ */

