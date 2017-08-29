/**
 * @file FileReaderTest.h
 * @brief Header file for class FileReaderTest
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

 * @details This header file contains the declaration of the class FileReaderTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef FILEREADER_FILEREADERTEST_H_
#define FILEREADER_FILEREADERTEST_H_

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
 * @brief Tests the FileReader public methods.
 */
class FileReaderTest {
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
     * @brief Tests that the GetBrokerName method correctly returns an empty string for OutputSignals.
     */
    bool TestGetBrokerName_OutputSignals();

    /**
     * @brief Tests that the GetBrokerName method correctly returns a MemoryMapInterpolatedInputBroker.
     */
    bool TestGetBrokerName_MemoryMapInterpolatedInputBroker();

    /**
     * @brief Tests that the GetBrokerName method correctly returns a MemoryMapInputBroker.
     */
    bool TestGetBrokerName_MemoryMapInputBroker();

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
     * @brief Tests the Initialise method with no Interpolation.
     */
    bool TestInitialise_1();

    /**
     * @brief Tests the Initialise method with a binary file format.
     */
    bool TestInitialise_Binary();

    /**
     * @brief Tests the Initialise method without specifying the Interpolate parameter.
     */
    bool TestInitialise_False_Interpolate();

    /**
     * @brief Tests the Initialise method specifying an invalid Interpolate parameter.
     */
    bool TestInitialise_False_Interpolate_Invalid();

    /**
     * @brief Tests the Initialise method without specifying the file name.
     */
    bool TestInitialise_False_Filename();

    /**
     * @brief Tests the Initialise method specifying an invalid file name.
     */
    bool TestInitialise_False_Filename_1();

    /**
     * @brief Tests the Initialise method specifying an invalid file.
     */
    bool TestInitialise_False_CorruptedFile();

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
     * @brief Tests the Initialise method without specifying the XAxisSignal.
     */
    bool TestInitialise_False_XAxisSignal();

    /**
     * @brief Tests the Initialise method without specifying the InterpolationPeriod.
     */
    bool TestInitialise_False_InterpolationPeriod();

    /**
     * @brief Tests the SetConfiguredDatabase.
     */
    bool TestSetConfiguredDatabase();

    /**
     * @brief Tests the SetConfiguredDatabase with more than one sample.
     */
    bool TestSetConfiguredDatabase_False_NumberOfSamples();

    /**
     * @brief Tests the SetConfiguredDatabase with more than one function interacting with the FileReader.
     */
    bool TestSetConfiguredDatabase_False_MoreThanOneFunction();

    /**
     * @brief Tests the OpenFile method.
     */
    bool TestOpenFile();

    /**
     * @brief Tests the CloseFile method.
     */
    bool TestCloseFile();

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
     * @brief Tests the IsStoreOnTrigger method.
     */
    bool TestIsInterpolate();

    /**
     * @brief Tests the GetXAxisSignal method.
     */
    bool TestGetXAxisSignal();

    /**
     * @brief Tests the GetInterpolationPeriod method.
     */
    bool TestGetInterpolationPeriod();

    /**
     * @brief Tests that the runtime error successfully sends a message when there is an error reading from the file
     */
    bool TestRuntimeErrorMessage();

    /**
     * @brief Tests that the runtime error successfully sends a message when there is an error reading from the file (but driven by an inconsistent number of elements in array)
     */
    bool TestRuntimeErrorMessage_2();

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

#endif /* FILEREADER_FILEREADERTEST_H_ */

