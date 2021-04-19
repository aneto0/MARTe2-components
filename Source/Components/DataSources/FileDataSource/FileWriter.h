/**
 * @file FileWriter.h
 * @brief Header file for class FileWriter
 * @date 11/08/2017
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

 * @details This header file contains the declaration of the class FileWriter
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef FILEDATASOURCE_FILEWRITER_H_
#define FILEDATASOURCE_FILEWRITER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "DataSourceI.h"
#include "File.h"
#include "MemoryMapAsyncOutputBroker.h"
#include "MemoryMapAsyncTriggerOutputBroker.h"
#include "MessageI.h"
#include "ProcessorType.h"
#include "RegisteredMethodsMessageFilter.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {
/**
 * @brief A DataSourceI interface which allows to store signals in a file.
 *
 * @details Data written into this data source is temporarily stored in a circular buffer and
 * asynchronously flushed to a file either as text (csv) or binary. This circular buffer can either be
 * continuously stored or stored only when a given event occurs (see StoreOnTrigger below).
 *
 * All the signals are stored in a single file.
 * If the format is csv the first line will be a comment with each signal name, type and number of elements.
 * e.g."#Trigger (uint8)[1];Time (uint32)[1];SignalUInt8 (uint8)[1];SignalUInt16 (uint16)[4]", where ; is the CSVSeparator.
 * A new line will be added every time all the signal samples are written.
 *
 * If the format is binary an header with the following information is created: the first 4 bytes
 * contain the number of signals. Then, for each signal, the signal type will be encoded in two bytes, followed
 *  by 32 bytes to encode the signal name, followed by 4 bytes which store the number of elements of a given signal.
 *  Following the header the signal samples are consecutively stored in binary format.
 *
 * This DataSourceI has the functions FlushFile, OpenFile and CloseFile registered as RPCs.
 *
 * Only one and one GAM is allowed to write into this DataSourceI.
 *
 * The configuration syntax is (names are only given as an example):
 *
 * <pre>
 * +FileWriter_0 = {
 *     Class = FileWriter
 *     NumberOfBuffers = 10 //Compulsory. Number of buffers in the circular buffer defined above. Each buffer is capable of holding a copy of all the DataSourceI signals.
 *     CPUMask = 0xFEu //Compulsory. Affinity assigned to the threads responsible for asynchronously flush data into the file.
 *     StackSize = 10000000 //Compulsory. Stack size of the thread above.
 *     Filename = "test.bin" //Optional. If not set the filename shall be set using the OpenFile RPC.
 *     Overwrite = "yes" //Compulsory. If "yes" the file will be overwritten, otherwise new data will be added to the end of the existent file.
 *     FileFormat = "binary" //Compulsory. Possible values are: binary and csv.
 *     CSVSeparator = "," //Compulsory if Format=csv. Sets the file separator type.
 *     StoreOnTrigger = 1 //Compulsory. If 0 all the data in the circular buffer is continuously stored. If 1 data is stored when the Trigger signal is 1 (see below).
 *     RefreshContent = 0 //Optional. If set, new data will always overwrite old data, keeping always the last snapshot. Also enables header pretty-printing, which is referred as "Full Notation".
 *     NumberOfPreTriggers = 2 //Compulsory iff StoreOnTrigger = 1.  Number of cycles to store before the trigger.
 *     NumberOfPostTriggers = 1 //Compulsory iff StoreOnTrigger = 1.  Number of cycles to store after the trigger.
 *
 *     Signals = {
 *         Trigger = { //Compulsory when StoreOnTrigger = 1. Must be set in index 0 of the Signals node. When the value of this signal is 1 data will be stored.
 *             Type = 'uint8" //Type must be uint8
 *         }
 *         SignalUInt16F = { //As many as required.
 *             Type = "uint16"
 *         }
 *         ...
 *     }
 *     +Messages = { //Optional. If set a message will be fired every time one of the events below occur
 *         Class = ReferenceContainer
 *         +FileOpenedOK = { //Optional, but if set, the name of the Object shall be FileOpenedOK. If set a message will be sent to the Destination, every time the File is successfully opened
 *             Class = Message
 *             Destination = SomeObject
 *             Function = SomeFunction
 *             Mode = ExpectsReply
 *         }
 *         +FileOpenedFail = { //Optional, but if set, the name of the Object shall be FileOpenedFail. If set a message will be sent to the Destination, every time the File cannot be successfully opened
 *             Class = Message
 *             Destination = SomeObject
 *             Function = SomeFunction
 *             Mode = ExpectsReply
 *         }*
 *         +FileClosed = { //Optional, but if set, the name of the Object shall be FileClosed. If set a message will be sent to the Destination, every time the File is flushed.
 *             Class = Message
 *             Destination = SomeObject
 *             Function = SomeFunction
 *             Mode = ExpectsReply
 *         }
 *         +FileRuntimeError = { //Optional, but if set, the name of the Object shall be FileRuntimeError. If set a message will be sent to the Destination, the first time there is a runtime error writing to the file.
 *             Class = Message
 *             Destination = SomeObject
 *             Function = SomeFunction
 *             Mode = ExpectsReply
 *         }
 *     }
 * }
 *
 * </pre>
 */
class FileWriter: public DataSourceI, public MessageI {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Default constructor.
     * @details Initialises all the optional parameters as described in the class description.
     * Registers the RPC FlushFile, CloseFile and OpenFile callback functions.
     */
    FileWriter();

    /**
     * @brief Destructor.
     * @details Flushes the file and frees the circular buffer.
     */
    virtual ~FileWriter();

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
     * @details Only OutputSignals are supported.
     * @return MemoryMapAsyncOutputBroker if storeOnTrigger == 0, MemoryMapAsyncTriggerOutputBroker otherwise.
     */
    virtual const char8 *GetBrokerName(StructuredDataI &data,
            const SignalDirection direction);

    /**
     * @brief See DataSourceI::GetInputBrokers.
     * @return false.
     */
    virtual bool GetInputBrokers(ReferenceContainer &inputBrokers,
            const char8* const functionName,
            void * const gamMemPtr);

    /**
     * @brief See DataSourceI::GetOutputBrokers.
     * @details If storeOnTrigger == 0 it adds a MemoryMapAsyncOutputBroker instance to
     *  the inputBrokers, otherwise it adds a MemoryMapAsyncTriggerOutputBroker instance to the outputBrokers.
     * @pre
     *   GetNumberOfFunctions() == 1u
     */
    virtual bool GetOutputBrokers(ReferenceContainer &outputBrokers,
            const char8* const functionName,
            void * const gamMemPtr);

    /**
     * @brief Writes the buffer data into the specified file in the specified format.
     * @return true if the data can be successfully written into the file.
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
     * - If relevant, the Trigger signal shall have type uint8
     * - The number of samples of all the signals is one.
     * - At least one signal (apart from the eventual Trigger signal) is set.
     * @return true if all the parameters are valid and if the file can be successfully opened.
     */
    virtual bool SetConfiguredDatabase(StructuredDataI & data);

    /**
     * @brief Flushes the file.
     * @return true if the file can be successfully flushed.
     */
    ErrorManagement::ErrorType FlushFile();

    /**
     * @brief Opens a new File.
     * @param[in] filenameIn the name of the file to be opened.
     * @return ErrorManagement::NoError if the file can be successfully opened.
     */
    ErrorManagement::ErrorType OpenFile(StreamString filenameIn);

    /**
     * @brief Close the file. Function is registered as an RPC.
     * @return ErrorManagement::NoError if the file can be successfully closed.
     */
    ErrorManagement::ErrorType CloseFile();

    /**
     * @brief Gets the affinity of the thread which is going to be used to asynchronously store the data in the file.
     * @return the affinity of the thread which is going to be used to asynchronously store the data in the file.
     */
    const ProcessorType& GetCPUMask() const;

    /**
     * @brief Gets the number of buffers in the circular buffer.
     * @return the number of buffers in the circular buffer.
     */
    uint32 GetNumberOfBuffers() const;

    /**
     * @brief Gets the number of post configured buffers in the circular buffer.
     * @return the number of post configured buffers in the circular buffer.
     */
    uint32 GetNumberOfPostTriggers() const;

    /**
     * @brief Gets the number of pre configured buffers in the circular buffer.
     * @return the number of pre configured buffers in the circular buffer.
     */
    uint32 GetNumberOfPreTriggers() const;

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
     * @brief Gets the stack size of the thread which is going to be used to asynchronously store the data in the output file.
     * @return the stack size of the thread which is going to be used to asynchronously store the data in the output file.
     */
    uint32 GetStackSize() const;

    /**
     * @brief Returns true if the data is going to be stored in the output file based on the occurrence of an external trigger.
     * @return true if the data is going to be stored in the output file based on a trigger event.
     */
    bool IsStoreOnTrigger() const;

    /**
     * @brief Returns true if the file is to be overwritten.
     * @return true if the file is to be overwritten.
     */
    bool IsOverwrite() const;

    /**
     * @see DataSourceI::Purge()
     */
    virtual void Purge(ReferenceContainer &purgeList);

private:

    /**
     * True if the data is only to be stored in the output file following a trigger.
     */
    bool storeOnTrigger;

    /**
     * Number of pre buffers when StoreOnTrigger == 1.
     */
    uint32 numberOfPreTriggers;

    /**
     * Number of post buffers when StoreOnTrigger == 1.
     */
    uint32 numberOfPostTriggers;

    /**
     * Number of buffers in the circular buffer.
     */
    uint32 numberOfBuffers;

    /**
     * Offset of each signal in the dataSourceMemory
     */
    uint32 *offsets;

    /**
     * Memory holding all the signals that are to be stored, for each cycle, in the output file.
     */
    char8 *dataSourceMemory;

    /**
     * The affinity of the thread that asynchronously flushes data into the output file.
     */
    ProcessorType cpuMask;

    /**
     * The size of the stack of the thread that asynchronously flushes data into the output file.
     */
    uint32 stackSize;

    /**
     * The name of the file where data will be stored.
     */
    StreamString filename;

    /**
     * True if the file is to be overwritten.
     */
    bool overwrite;

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
     * The printf format in case of writing as csv.
     */
    StreamString csvPrintfFormat;

    /**
     * The CSV separator.
     */
    StreamString csvSeparator;

    /**
     * The signal memory as an AnyType array optimised for the PrintFormatted
     */
    AnyType *signalsAnyType;

    /**
     * If a fatal file error occurred do not try to flush segments nor do further writes.
     */
    bool fatalFileError;


    /**
     * Keeps the position of the next-to-header byte, in order to allow content refreshing without 
     * touching the header. 
     */
    uint64 headerPositionMarker;

    /**
     * The output file.
     */
    File outputFile;

    /**
     * Refresh the content at each cycle
     */
    uint8 refreshContent;

    /**
     * Write in full notation
     */
    uint8 fullNotation;

    /**
     * Filter to receive the RPC which allows to the handle the file with messages.
     */
    ReferenceT<RegisteredMethodsMessageFilter> filter;

    /**
     * The asynchronous triggered broker that provides the interface between the GAMs and the output file.
     */
    ReferenceT<MemoryMapBroker> brokerAsync;

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

#endif /* FILEDATASOURCE_FILEWRITER_H_ */
	
