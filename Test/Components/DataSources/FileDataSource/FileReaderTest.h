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
     * @brief Tests the Synchronise method with csv files.
     */
    bool TestSynchronise_CSV();

    /**
     * @brief Tests the Synchronise method with csv files and using the comma as the separator.
     */
    bool TestSynchronise_CSV_Comma();

    /**
     * @brief Tests the Synchronise method with csv files and strings.
     */
    bool TestSynchronise_CSV_Strings();

    /**
     * @brief Tests the Synchronise method with csv files and interpolation.
     */
    bool TestSynchronise_CSV_Interpolation();

    /**
     * @brief Tests the Synchronise method with csv files, interpolation and preload.
     */
    bool TestSynchronise_CSV_Interpolation_Preload();

    /**
     * @brief Tests the Synchronise method with binary files.
     */
    bool TestSynchronise_Binary();

    /**
     * @brief Tests the Synchronise method with binary files and interpolation.
     */
    bool TestSynchronise_Binary_Interpolation();

    /**
     * @brief Tests the Synchronise method with binary files, interpolation and preload.
     */
    bool TestSynchronise_Binary_Interpolation_Preload();

    /**
     * @brief Tests the Synchronise method using the Frequency parameter.
     */
    bool TestSynchronise_Frequency();

    /**
     * @brief Tests the Synchronise method using different xAxisSignal types.
     */
    bool TestSynchronise_XAxisSignals();

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
     * @brief Tests the Initialise method specifying a bad EOF option.
     */
    bool TestInitialise_False_EOF();

    /**
     * @brief Tests the Initialise method without specifying the XAxisSignal.
     */
    bool TestInitialise_Warning_XAxisSignal();

    /**
     * @brief Tests the Initialise method without specifying the InterpolationPeriod.
     */
    bool TestInitialise_Warning_InterpolationPeriod();

    /**
     * @brief Tests the Initialise method with Preaload = "no".
     */
    bool TestInitialise_Preload_no();

    /**
     * @brief Tests the Initialise method with Preaload = "yes" MaxFileByteSize not specified.
     */
    bool TestInitialise_Preload_yes_NoMaxSize();

    /**
     * @brief Tests the Initialise method with Preaload = "yes" MaxFileByteSize not specified.
     */
    bool TestInitialise_Preload_yes_MaxSizeToLarge();

    /**
     * @brief Tests the SetConfiguredDatabase.
     */
    bool TestSetConfiguredDatabase();

    /**
     * @brief Test SetConfiguredDatabase on error.
     */
    bool TestSetConfiguradDatabase_InputSignalSize0();

    /**
     * @brief Tests the SetConfiguredDatabase with more than one sample.
     */
    bool TestSetConfiguredDatabase_False_NumberOfSamples();

    /**
     * @brief Tests the SetConfiguredDatabase wrong file size.
     */
    bool TestSetConfiguredDatabase_WrongFileSize_CSV();

    /**
     * @brief Tests the SetConfiguredDatabase wrong file size.
     */
    bool TestSetConfiguredDatabase_WrongFileSize_Bin();

    /**
     * @brief Tests the SetConfiguredDatabase wrong file size.
     */
    bool TestSetConfiguredDatabase_MaxSizeSmallerThanFileSize();

    /**
     * @brief Tests the SetConfiguredDatabase with more than one function interacting with the FileReader.
     */
    bool TestSetConfiguredDatabase_False_MoreThanOneFunction();

    /**
     * @brief Tests the SetConfiguredDatabase with an invalid XAxisSignal name.
     */
    bool TestSetConfiguredDatabase_False_XAxisSignal();

    /**
     * @brief Tests the SetConfiguredDatabase with an invalid XAxisSignal number of dimensions.
     */
    bool TestSetConfiguredDatabase_False_XAxisSignal_Dimensions();

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
     * @brief Tests that the runtime error successfully warns that the runtime error message could not be sent
     */
    bool TestRuntimeErrorMessage_Fail_Preload();

    /**
     * @brief Tests that an Invalid message name is correctly captured.
     */
    bool TestInvalidMessageName();

    /**
     * @brief Tests that an Invalid message type is correctly captured.
     */
    bool TestInvalidMessageType();

    /**
     * @brief Tests that an Invalid message type is correctly captured.
     */
    bool TestInvalidMessageType_Preload();

    /**
     * @brief Tests the EOF for the CVS files with the default behaviour (Rewind).
     */
    bool TestEOF_CSV();

    /**
     * @brief Tests the EOF for the CVS files with the default behaviour (Rewind) using preload.
     */
    bool TestEOF_CSV_Preload();

    /**
     * @brief Tests the EOF for binary files with the default behaviour (Rewind).
     */
    bool TestEOF_Binary();

    /**
     * @brief Tests the EOF for binary files with the default behaviour (Rewind). The file size is 2.1 GB
     */
    bool TestEOF_Binary_JumboFile();

    /**
     * @brief Tests the EOF for binary files with the default behaviour (Rewind) using preload.
     */
    bool TestEOF_Binary_Preload();

    /**
     * @brief Tests the EOF for the CVS files with the Rewind behaviour.
     */
    bool TestEOF_Rewind_CSV();

    /**
     * @brief Tests the EOF for the CVS files with the Rewind behaviour using prelaod.
     */
    bool TestEOF_Rewind_CSV_Preload();

    /**
     * @brief Tests the EOF for binary files with the Rewind behaviour.
     */
    bool TestEOF_Rewind_Binary();

    /**
     * @brief Tests the EOF for binary files with the Rewind behaviour using prelaod.
     */
    bool TestEOF_Rewind_Binary_Preload();

    /**
     * @brief Tests the EOF for the CVS files with the Last behaviour.
     */
    bool TestEOF_Last_CSV();

    /**
     * @brief Tests the EOF for the CVS files with the Last behaviour using preload.
     */
    bool TestEOF_Last_CSV_Preload();

    /**
     * @brief Tests the EOF for binary files with the Last behaviour.
     */
    bool TestEOF_Last_Binary();

    /**
     * @brief Tests the EOF for binary files with the Last behaviour using preload.
     */
    bool TestEOF_Last_Binary_Preload();

    /**
     * @brief Tests the EOF for the CVS files with the Error behaviour.
     */
    bool TestEOF_Error_CSV();

    /**
     * @brief Tests the EOF for the CVS files with the Error behaviour using preload.
     */
    bool TestEOF_Error_CSV_Preload();

    /**
     * @brief Tests the EOF for binary files with the Error behaviour.
     */
    bool TestEOF_Error_Binary();

    /**
     * @brief Tests the EOF for binary files with the Error behaviour using preload..
     */
    bool TestEOF_Error_Binary_Preload();
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* FILEREADER_FILEREADERTEST_H_ */

