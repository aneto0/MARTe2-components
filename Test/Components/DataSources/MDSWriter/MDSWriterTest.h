/**
 * @file MDSWriterTest.h
 * @brief Header file for class MDSWriterTest
 * @date 14/02/2017
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

 * @details This header file contains the declaration of the class MDSWriterTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef MDSWRITER_MDSWRITERTEST_H_
#define MDSWRITER_MDSWRITERTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
#include "mdsobjects.h"
/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "MDSWriter.h"

#include "MDSWriterTreeTestHelper.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
/**
 * @brief Tests the MDSWriter public methods.
 */
class MDSWriterTest {
public:
    MDSWriterTest();

    ~MDSWriterTest();

    /**
     * @brief Tests the constructor.
     */
    bool TestConstructor();

    /**
     * @brief Tests the AllocateMemory method.
     */
    bool TestAllocateMemory();

    /**
     * @brief Tests the GetNumberOfMemoryBuffers method.
     */
    bool TestGetNumberOfMemoryBuffers();

    /**
     * @brief Tests the GetSignalMemoryBuffer method.
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
     * @brief Tests the GetInputBrokers method.
     */
    bool TestGetInputBrokers();

    /**
     * @brief Tests the GetOutputBrokers method.
     */
    bool TestGetOutputBrokers();

    /**
     * @brief Tests the Synchronise method.
     */
    bool TestSynchronise();

    /**
     * @brief Tests the PrepareNextState method.
     */
    bool TestPrepareNextState();

    /**
     * @brief Tests the Initialise method.
     */
    bool TestInitialise();

    /**
     * @brief Tests the Initialise method without specifying the number of buffers.
     */
    bool TestInitialise_False_NumberOfBuffers();

    /**
     * @brief Tests the Initialise method without specifying the CPU mask.
     */
    bool TestInitialise_False_CPUMask();

    /**
     * @brief Tests the Initialise method without specifying the stack size.
     */
    bool TestInitialise_False_StackSize();

    /**
     * @brief Tests the Initialise method without specifying the tree name.
     */
    bool TestInitialise_False_TreeName();

    /**
     * @brief Tests the Initialise method without specifying the event name.
     */
    bool TestInitialise_False_EventName();

    /**
     * @brief Tests the Initialise method without specifying the time refresh parameter.
     */
    bool TestInitialise_False_TimeRefresh();

    /**
     * @brief Tests the Initialise method without specifying the StoreOnTrigger.
     */
    bool TestInitialise_False_StoreOnTrigger();

    /**
     * @brief Tests the Initialise method without specifying the NumberOfPreTriggers.
     */
    bool TestInitialise_False_NumberOfPreTriggers();

    /**
     * @brief Tests the Initialise method without specifying the NumberOfPostTriggers.
     */
    bool TestInitialise_False_NumberOfPostTriggers();

    /**
     * @brief Tests the Initialise method without specifying the Signals.
     */
    bool TestInitialise_False_Signals();

    /**
     * @brief Tests the SetConfiguredDatabase.
     */
    bool TestSetConfiguredDatabase();

    /**
     * @brief Tests the SetConfiguredDatabase with dimensions > 1.
     */
    bool TestSetConfiguredDatabase_False_NumberOfDimensions();

    /**
     * @brief Tests the SetConfiguredDatabase with more than one samples.
     */
    bool TestSetConfiguredDatabase_False_NumberOfSamples();

    /**
     * @brief Tests the SetConfiguredDatabase with more than one time signal.
     */
    bool TestSetConfiguredDatabase_False_MoreThanOneTimeSignal();

    /**
     * @brief Tests the SetConfiguredDatabase with no MDSPlus signals defined.
     */
    bool TestSetConfiguredDatabase_False_NoMDSSignals();

    /**
     * @brief Tests the SetConfiguredDatabase with no TimeSignal defined.
     */
    bool TestSetConfiguredDatabase_False_NoTimeSignal();

    /**
     * @brief Tests the SetConfiguredDatabase with TimeSignal defined as not integer.
     */
    bool TestSetConfiguredDatabase_False_TimeSignal_NotInteger();

    /**
     * @brief Tests the SetConfiguredDatabase with more than one function interacting with the MDSWriter.
     */
    bool TestSetConfiguredDatabase_False_TimeSignal_MoreThanOneFunction();

    /**
     * @brief Tests the OpenTree method.
     */
    bool TestOpenTree();

    /**
     * @brief Tests the MDSWriter integrated in an application which continuously stores data.
     */
    bool TestIntegratedInApplication_NoTrigger();

    /**
     * @brief Tests the MDSWriter integrated in an application which continuously stores data
     * and that requires a flush at the end in order to demonstrate that all the data is stored in the MDSplus database.
     */
    bool TestIntegratedInApplication_NoTrigger_Flush();

    /**
     * @brief Tests the MDSWriter integrated in an application which asynchronously stores data based on a trigger event.
     */
    bool TestIntegratedInApplication_Trigger();

    /**
     * @brief Tests the MDSWriter integrated in an application which asynchronously stores data based on a trigger event and with a MakeSegmentAfterNWrites which is not multiple of the length of the array to write .
     */
    bool TestIntegratedInApplication_Trigger_NotMultiple();

    /**
     * @brief Tests the MDSWriter integrated in an application which asynchronously stores data based on a trigger event and with a MakeSegmentAfterNWrites which is not multiple of the length of the array to write  with Samples.
     */
    bool TestIntegratedInApplication_Trigger_NotMultiple_Samples();

    /**
     * @brief Tests the MDSWriter integrated in an application which asynchronously stores data based on a trigger event using a 8 bit time signal.
     */
    bool TestIntegratedInApplication_Trigger_Time8();

    /**
     * @brief Tests the MDSWriter integrated in an application which asynchronously stores data based on a trigger event using a 16 bit time signal.
     */
    bool TestIntegratedInApplication_Trigger_Time16();

    /**
     * @brief Tests the MDSWriter integrated in an application which asynchronously stores data based on a trigger event using a 64 bit time signal.
     */
    bool TestIntegratedInApplication_Trigger_Time64();

    /**
     * @brief Tests the MDSWriter integrated in an application which asynchronously stores data based on a trigger event using a 8 bit signed time signal.
     */
    bool TestIntegratedInApplication_Trigger_TimeS8();

    /**
     * @brief Tests the MDSWriter integrated in an application which asynchronously stores data based on a trigger event using a 16 bit signed time signal.
     */
    bool TestIntegratedInApplication_Trigger_TimeS16();

    /**
     * @brief Tests the MDSWriter integrated in an application which asynchronously stores data based on a trigger event using a 64 bit signed time signal.
     */
    bool TestIntegratedInApplication_Trigger_TimeS64();

    /**
     * @brief Tests the MDSWriter integrated in an application which asynchronously stores data based on a trigger event forcing discontinuities.
     */
    bool TestIntegratedInApplication_Trigger_Discontinuity();

    /**
     * @brief Tests the MDSWriter integrated in an application which asynchronously stores data based on a trigger event forcing discontinuities with FlushIfDiscontinuity set to 1
     */
    bool TestIntegratedInApplication_Trigger_Discontinuity_FlushIfDiscontinuity();

    /**
     * @brief Tests the MDSWriter integrated in an application which continuously stores data with NumberOfElements > 1.
     */
    bool TestIntegratedInApplication_NoTrigger_Elements();

    /**
     * @brief Tests the MDSWriter integrated in an application which asynchronously stores data, with NumberOfElements > 1, based on a trigger event.
     */
    bool TestIntegratedInApplication_Trigger_Elements();

    /**
     * @brief Tests the MDSWriter integrated in an application which asynchronously stores data, with NumberOfElements > 1, based on a trigger event.
     */
    bool TestIntegratedInApplication_NoTrigger_AutomaticSegmentation();

    /**
     * @brief Tests the MDSWriter integrated in an application which asynchronously stores data, with NumberOfElements > 1, based on a trigger event.
     */
    bool TestIntegratedInApplication_NoTrigger_AutomaticSegmentation_Vector();

    /**
     * @brief Tests the GetCPUMask method.
     */
    bool TestGetCPUMask();

    /**
     * @brief Tests the GetEventName method.
     */
    bool TestGetEventName();

    /**
     * @brief Tests the GetNumberOfBuffers method.
     */
    bool TestGetNumberOfBuffers();

    /**
     * @brief Tests the GetNumberOfMdsSignals method.
     */
    bool TestGetNumberOfMdsSignals();

    /**
     * @brief Tests the GetNumberOfPostTriggers method.
     */
    bool TestGetNumberOfPostTriggers();

    /**
     * @brief Tests the GetNumberOfPreTriggers method.
     */
    bool TestGetNumberOfPreTriggers();

    /**
     * @brief Tests the GetPulseNumber method.
     */
    bool TestGetPulseNumber();

    /**
     * @brief Tests the GetTreeName method.
     */
    bool TestGetTreeName();

    /**
     * @brief Tests the GetRefreshEveryCounts method.
     */
    bool TestGetRefreshEveryCounts();

    /**
     * @brief Tests the GetStackSize method.
     */
    bool TestGetStackSize();

    /**
     * @brief Tests the IsStoreOnTrigger method.
     */
    bool TestIsStoreOnTrigger();

    /**
     * @brief Tests the GetTimeSignalIdx method.
     */
    bool TestGetTimeSignalIdx();

    /**
     * @brief Tests that the open tree message successfully sends the value of the new pulse number
     */
    bool TestOpenTreeOKMessage();

    /**
     * @brief Tests that the open tree message successfully sends a message when it fails to open the tree
     */
    bool TestOpenTreeFailMessage();

    /**
     * @brief Tests that the flush segments successfully sends a message with the flushed segments information
     */
    bool TestFlushSegmentsMessage();

    /**
     * @brief Tests that the open tree message successfully warns that the message could not be sent
     */
    bool TestOpenTreeOKMessage_Fail();

    /**
     * @brief Tests that the open tree message successfully warns that the fail message could not be sent
     */
    bool TestOpenTreeFailMessage_Fail();

    /**
     * @brief Tests that the flush segments successfully warns that the flush segments message could not be sent
     */
    bool TestFlushSegmentsMessage_Fail();

    /**
     * @brief Tests that an Invalid message name is correctly captured.
     */
    bool TestInvalidMessageName();

    /**
     * @brief Tests that an Invalid message type is correctly captured.
     */
    bool TestInvalidMessageType();
private:
    MDSWriterTreeTestHelper treeTestHelper;
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* MDSWRITER_MDSWRITERTEST_H_ */

