/**
 * @file FileWriterTest.h
 * @brief Header file for class FileWriterTest
 * @date 21/08/2017
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

 * @details This header file contains the declaration of the class FileWriterTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef FILEWRITER_FILEWRITERTEST_H_
#define FILEWRITER_FILEWRITERTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "CompilerTypes.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
/**
 * @brief Tests the FileWriter public methods.
 */
class FileWriterTest {
public:
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
     * @brief Tests the Initialise method with a binary file format.
     */
    bool TestInitialise_Binary();

    /**
     * @brief Tests the Initialise method without specifying the overwrite parameter.
     */
    bool TestInitialise_False_Overwrite();

    /**
     * @brief Tests the Initialise method specifying an invalid overwrite parameter.
     */
    bool TestInitialise_False_Overwrite_Invalid();

    /**
     * @brief Tests the Initialise method without specifying the number of buffers.
     */
    bool TestInitialise_False_NumberOfBuffers();

    /**
     * @brief Tests the Initialise method with a number of buffers that is not > 0.
     */
    bool TestInitialise_False_NumberOfBuffers_GT_0();

    /**
     * @brief Tests the Initialise method without specifying the CPU mask.
     */
    bool TestInitialise_False_CPUMask();

    /**
     * @brief Tests the Initialise method without specifying the stack size.
     */
    bool TestInitialise_False_StackSize();

    /**
     * @brief Tests the Initialise method with a the stack size that is not > 0.
     */
    bool TestInitialise_False_StackSize_GT_0();

    /**
     * @brief Tests the Initialise method without specifying the file name.
     */
    bool TestInitialise_Warning_Filename();

    /**
     * @brief Tests the Initialise method without specifying the FileFormat.
     */
    bool TestInitialise_False_FileFormat();

    /**
     * @brief Tests the Initialise method with an invalid FileFormat.
     */
    bool TestInitialise_False_FileFormat_Invalid();

    /**
     * @brief Tests the Initialise method without specifying the CSVSeparator.
     */
    bool TestInitialise_False_CSVSeparator();

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
     * @brief Tests the SetConfiguredDatabase with more than one sample.
     */
    bool TestSetConfiguredDatabase_False_NumberOfSamples();

    /**
     * @brief Tests the SetConfiguredDatabase with no File signals defined.
     */
    bool TestSetConfiguredDatabase_False_NoFileSignals();

    /**
     * @brief Tests the SetConfiguredDatabase with more than one function interacting with the FileWriter.
     */
    bool TestSetConfiguredDatabase_False_TimeSignal_MoreThanOneFunction();

    /**
     * @brief Tests the OpenFile method.
     */
    bool TestOpenFile();

    /**
     * @brief Tests that the OpenFile does not overwrite.
     */
    bool TestOpenFile_Overwrite();

    /**
     * @brief Tests the CloseFile method.
     */
    bool TestCloseFile();

    /**
     * @brief Tests the FlushFile method.
     */
    bool TestFlushFile();

    /**
     * @brief Tests the FileWriter integrated in an application which continuously stores data.
     */
    bool TestIntegratedInApplication_NoTrigger(const MARTe::char8 *filename, bool csv = true, MARTe::uint8 refreshContent = 0u, MARTe::uint32* detectedFileSize = 0);

    /**
     * @brief Tests the FileWriter integrated in an application which continuously stores data.
     */
    bool TestIntegratedInApplication_RefreshContent(const MARTe::char8 *filename, bool csv = true, MARTe::uint32 * detectedFileSize = 0);

    /**
     * @brief Tests the FileWriter integrated in an application which continuously stores data.
     */
    bool TestIntegratedInApplication_NoTrigger_Array(const MARTe::char8 *filename, bool csv = true);

    /**
     * @brief Tests the FileWriter integrated in an application which asynchronously stores data based on a trigger event.
     */
    bool TestIntegratedInApplication_Trigger(const MARTe::char8 *filename, bool csv = true);

    /**
     * @brief Tests the GetCPUMask method.
     */
    bool TestGetCPUMask();

    /**
     * @brief Tests the GetFilename method.
     */
    bool TestGetFilename();

    /**
     * @brief Tests the GetFileFormat method.
     */
    bool TestGetFileFormat();

    /**
     * @brief Tests the GetCSVSeparator method.
     */
    bool TestGetCSVSeparator();

    /**
     * @brief Tests the GetNumberOfBuffers method.
     */
    bool TestGetNumberOfBuffers();

    /**
     * @brief Tests the GetNumberOfPostTriggers method.
     */
    bool TestGetNumberOfPostTriggers();

    /**
     * @brief Tests the GetNumberOfPreTriggers method.
     */
    bool TestGetNumberOfPreTriggers();

    /**
     * @brief Tests the GetStackSize method.
     */
    bool TestGetStackSize();

    /**
     * @brief Tests the IsOverwrite method.
     */
    bool TestIsOverwrite();

    /**
     * @brief Tests the IsStoreOnTrigger method.
     */
    bool TestIsStoreOnTrigger();

    /**
     * @brief Tests that the open file message successfully opens the file
     */
    bool TestOpenFileOKMessage();

    /**
     * @brief Tests that the open file message successfully sends a message when it fails to open the file
     */
    bool TestOpenFileFailMessage();

    /**
     * @brief Tests that the close file successfully sends a message when the file is flushed
     */
    bool TestCloseFileMessage();

    /**
     * @brief Tests that the runtime error successfully sends a message when there is an error writing to the file
     */
    bool TestRuntimeErrorMessage();

    /**
     * @brief Tests that the open file message successfully warns that the message could not be sent
     */
    bool TestOpenFileOKMessage_Fail();

    /**
     * @brief Tests that the open file message successfully warns that the fail message could not be sent
     */
    bool TestOpenFileFailMessage_Fail();

    /**
     * @brief Tests that the close file successfully warns that the close file message could not be sent
     */
    bool TestCloseFileMessage_Fail();

    /**
     * @brief Tests that the runtime error successfully warns that the runtime error message could not be sent
     */
    bool TestRuntimeErrorMessage_Fail();

    /**
     * @brief Tests that an Invalid message name is correctly captured.
     */
    bool TestInvalidMessageName();

    /**
     * @brief Tests that an Invalid message type is correctly captured.
     */
    bool TestInvalidMessageType();

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* FILEWRITER_FILEWRITERTEST_H_ */

