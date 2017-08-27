/**
 * @file FileReader.h
 * @brief Header file for class FileReader
 * @date 11/08/2017
 * @author Andre' Neto
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

 * @details This header file contains the declaration of the class FileReader
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef FILEDATASOURCE_FILEREADER_H_
#define FILEDATASOURCE_FILEREADER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "DataSourceI.h"
#include "ProcessorType.h"
#include "File.h"
#include "MemoryMapInterpolatedInputBroker.h"
#include "MessageI.h"
#include "RegisteredMethodsMessageFilter.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {
/**
 * @brief A DataSourceI interface which allows to read signals from a file.
 *
 * @details The file format can be either text (csv) or binary. The data can be interpolated against a defined time
 *  vector or retrieved as is.
 *
 * If the format is csv the first line shall be be a comment starting with a # symbol followed by
 * each signal name, type and number of elements. A new line is expected for each signal sample. Arrays are encoded
 *  inside brackets as per BufferedStreamI::PrintFormatted
 *
 * If the format is binary an header with the following information is expected: the first 4 bytes
 * contain the number of signals. Then, for each signal, the signal type will be encoded in two bytes, followed
 *  by 32 bytes to encode the signal name, followed by 4 bytes which store the number of elements of a given signal.
 *  Following the header the signal samples are consecutively stored in binary format.
 * *
 * This DataSourceI has the function CloseFile registered as RPCs.
 *
 * Only one and one GAM is allowed to read from this DataSourceI.
 *
 * The configuration syntax is (names are only given as an example):
 * +FileReader_0 = {
 *     Class = FileReader
 *     Filename = "test.bin" //Compulsory.
 *     Interpolate = "yes" //Compulsory. If "yes" the data will be interpolated and a Time signal shall be provided. If "no" the data will be provided as is.
 *     FileFormat = "binary" //Compulsory. Possible values are: binary and csv.
 *     CSVSeparator = "," //Compulsory if Format=csv. Sets the file separator type.
 *     TimeSignalName = "Time" //Compulsory if Interpolate = "yes". Name of the signal containing the time vector to be interpolated.
 *
 *     Signals = {
 *         InterpolatedTimeSignal = { //Compulsory when Interpolate = "yes". Must be set in index 0 of the Signals node.
 *             Type = 'uint64" //Type must be uint64
 *         }
 *         //All the other signals are automatically added against the information stored in the csv file.
 *     }
 *     +Messages = { //Optional. If set a message will be fired every time one of the events below occur
 *         Class = ReferenceContainer
 *         +FileOpenedOK = { //Optional, but if set, the name of the Object shall be FileOpenedOK. If set a message will be sent to the Destination, every time the File is successfully opened
 *             Class = Message
 *             Destination = SomeObject
 *             Function = SomeFunction
 *             Mode = ExpectsReply
 *         }
 *         +FileClosed = { //Optional, but if set, the name of the Object shall be FileClosed. If set a message will be sent to the Destination, every time the File is flushed.
 *             Class = Message
 *             Destination = SomeObject
 *             Function = SomeFunction
 *             Mode = ExpectsReply
 *         }
 *         +FileRuntimeError = { //Optional, but if set, the name of the Object shall be FileRuntimeError. If set a message will be sent to the Destination, the first time there is a runtime error reading from the file.
 *             Class = Message
 *             Destination = SomeObject
 *             Function = SomeFunction
 *             Mode = ExpectsReply
 *         }
 *     }
 * }
 */
class FileReader: public DataSourceI, public MessageI {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Default constructor.
     * @details Initialises all the optional parameters as described in the class description.
     * Registers the RPC FlushFile and OpenFile callback functions.
     */
FileReader    ();

    /**
     * @brief Destructor.
     * @details TODO.
     */
    virtual ~FileReader();

    /**
     * @brief See DataSourceI::AllocateMemory. NOOP.
     * @return true.
     */
    virtual bool AllocateMemory();

    /**
     * @brief See DataSourceI::GetNumberOfMemoryBuffers.
     * @return 1.
     */
    virtual uint32 GetNumberOfMemoryBuffers();

    /**
     * @brief See DataSourceI::GetSignalMemoryBuffer.
     * @pre
     *   SetConfiguredDatabase
     */
    virtual bool GetSignalMemoryBuffer(const uint32 signalIdx,
            const uint32 bufferIdx,
            void *&signalAddress);

    /**
     * @brief See DataSourceI::GetBrokerName.
     * @details Only Input are supported.
     * @return MemoryMapSynchronisedInputBroker if interpolate = false, MemoryMapInterpolatedInputBroker otherwise.
     */
    virtual const char8 *GetBrokerName(StructuredDataI &data,
            const SignalDirection direction);

    /**
     * @brief See DataSourceI::GetInputBrokers.
     * @details If interpolate == yes it adds a MemoryMapInterpolatedInputBroker instance to
     *  the inputBrokers, otherwise it adds a MemoryMapSynchronisedInputBroker instance to the intputBrokers.
     * @pre
     *   GetNumberOfFunctions() == 1u
     */
    virtual bool GetInputBrokers(ReferenceContainer &inputBrokers,
            const char8* const functionName,
            void * const gamMemPtr);

    /**
     * @brief See DataSourceI::GetOutputBrokers.
     * @return false.
     */
    virtual bool GetOutputBrokers(ReferenceContainer &outputBrokers,
            const char8* const functionName,
            void * const gamMemPtr);

    /**
     * @brief Reads into the buffer data the data from the specified file in the specified format.
     * @return true if the data can be successfully read from the file.
     */
    virtual bool Synchronise();

    /**
     * @brief See DataSourceI::PrepareNextState. NOOP.
     * @return true.
     */
    virtual bool PrepareNextState(const char8 * const currentStateName,
            const char8 * const nextStateName);

    /**
     * @brief Loads and verifies the configuration parameters detailed in the class description.
     * @return true if all the mandatory parameters are correctly specified and if the specified optional parameters have valid values.
     */
    virtual bool Initialise(StructuredDataI & data);

    /**
     * @brief Final verification of all the parameters and opening of the file.
     * @details This method verifies that all the parameters (e.g. number of samples) requested by the GAMs interacting with this DataSource
     *  are valid and consistent with the parameters set during the initialisation phase.
     * In particular the following conditions shall be met:
     * - If relevant, the InterpolatedTimeSignal shall have type uint64
     * - The number of samples of all the signals is one.
     * - At least one signal (apart from the eventual InterpolatedTimeSignal signal) is set.
     * @return true if all the parameters are valid and if the file can be successfully opened.
     */
    virtual bool SetConfiguredDatabase(StructuredDataI & data);


    /**
     * @brief Close the file. Function is registered as an RPC.
     * @return ErrorManagement::NoError if the file can be successfully closed.
     */
    ErrorManagement::ErrorType CloseFile();

    /**
     * @brief Gets the configured filename.
     * @return the configured filename.
     */
    const StreamString& GetFilename() const;

    /**
     * @brief Gets the configured FileFormat.
     * @return the configured FileFormat.
     */
    const StreamString& GetFileFormat() const;

    /**
     * @brief Gets the configured CSVSeparator.
     * @return the configured CSVSeparator.
     */
    const StreamString& GetCSVSeparator() const;

    /**
     * @brief Returns true if the file is to be interpolated.
     * @return true if the file is to be interpolated.
     */
    bool IsInterpolate() const;

private:

    /**
     * @brief Opens a new File, parses the header and registers the signals in the cdb.
     * @param[out] cdb the ConfigurationDatabase where signals will be written into.
     * @return ErrorManagement::NoError if the file can be successfully opened.
     */
    ErrorManagement::ErrorType OpenFile(StructuredDataI &cdb);

    /**
     * Offset of each signal in the dataSourceMemory
     */
    uint32 *offsets;

    /**
     * Memory holding all the signals that are to be stored, for each cycle, in the output file.
     */
    char8 *dataSourceMemory;

    /**
     * The name of the file where data will be stored.
     */
    StreamString filename;

    /**
     * True if the data from this file is to be interpolated.
     */
    bool interpolate;

    /**
     * The file format as text.
     */
    StreamString fileFormatStr;

    /**
     * The file format.
     */
    int32 fileFormat;

    /**
     * Number of bytes to write in case of binary format.
     */
    uint32 numberOfBinaryBytes;

    /**
     * The CSV separator.
     */
    StreamString csvSeparator;

    /**
     * If a fatal file error occurred do not try to flush segments nor do further writes.
     */
    bool fatalFileError;

    /**
     * The output file.
     */
    File inputFile;

    /**
     * The signal memory as an AnyType array optimised for reading the data.
     */
    AnyType *signalsAnyType;

    /**
     * Filter to receive the RPC which allows to change the pulse number.
     */
    ReferenceT<RegisteredMethodsMessageFilter> filter;

    /**
     * The asynchronous triggered broker that provides the interface between the GAMs and the output file.
     */
    MemoryMapInterpolatedInputBroker *interpolatedInputBroker;

    /**
     * The message to send if the file is successfully opened.
     */
    ReferenceT<Message> fileOpenedOKMsg;

    /**
     * The message to send if the file cannot be successfully opened.
     */
    ReferenceT<Message> fileOpenedFailMsg;

    /**
     * The message to send if the File is be successfully closed.
     */
    ReferenceT<Message> fileClosedMsg;

    /**
     * The message to send if there is a runtime error.
     */
    ReferenceT<Message> fileRuntimeErrorMsg;
};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* FILEDATASOURCE_FILEREADER_H_ */

