/**
 * @file MDSWriter.h
 * @brief Header file for class MDSWriter
 * @date 10/02/2017
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

 * @details This header file contains the declaration of the class MDSWriter
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef DATASOURCES_MDSWRITER_MDSWRITER_H_
#define DATASOURCES_MDSWRITER_MDSWRITER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "DataSourceI.h"
#include "ProcessorType.h"
#include "MDSWriterNode.h"
#include "MessageI.h"
#include "RegisteredMethodsMessageFilter.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {

/**
 * @brief TODO
 */
class MDSWriter: public DataSourceI, public MessageI {
public:
    CLASS_REGISTER_DECLARATION()
    /**
     * TODO
     * @brief Default constructor.
     * @details Initialises all the optional parameters as described in the class description.
     */
MDSWriter    ();

    /**
     * TODO
     * @brief Destructor.
     * @details Closes all the file descriptors associated to the board, including any opened channels.
     * Stops the embedded thread which is reading from this board.
     */
    virtual ~MDSWriter();

    /**
     * TODO
     * @brief See DataSourceI::AllocateMemory. NOOP.
     * @return true.
     */
    virtual bool AllocateMemory();

    /**
     * TODO
     * @brief See DataSourceI::GetNumberOfMemoryBuffers.
     * @return 1.
     */
    virtual uint32 GetNumberOfMemoryBuffers();

    /**
     * TODO
     * @brief See DataSourceI::GetSignalMemoryBuffer.
     */
    virtual bool GetSignalMemoryBuffer(const uint32 signalIdx,
            const uint32 bufferIdx,
            void *&signalAddress);

    /**
     * TODO
     * @brief See DataSourceI::GetNumberOfMemoryBuffers.
     * @details Only InputSignals are supported.
     * @return MemoryMapSynchronisedInputBroker if frequency > 0, MemoryMapInputBroker otherwise.
     */
    virtual const char8 *GetBrokerName(StructuredDataI &data,
            const SignalDirection direction);

    /**
     * TODO
     * @brief See DataSourceI::GetInputBrokers.
     * @details If the functionName is the one synchronising it adds a MemoryMapSynchronisedInputBroker instance to
     *  the inputBrokers, otherwise it adds a MemoryMapInputBroker instance to the inputBrokers.
     */
    virtual bool GetInputBrokers(ReferenceContainer &inputBrokers,
            const char8* const functionName,
            void * const gamMemPtr);

    /**
     * TODO
     * @brief See DataSourceI::GetOutputBrokers.
     * @return false.
     */
    virtual bool GetOutputBrokers(ReferenceContainer &outputBrokers,
            const char8* const functionName,
            void * const gamMemPtr);

    /**
     * TODO
     * @brief Waits on an EventSem for the requested number of samples to be acquired for all the channels.
     * @return true if the semaphore is successfully posted.
     */
    virtual bool Synchronise();

    /**
     * TODO
     * @brief Starts the EmbeddedThread and sets the counter and the time to zero.
     * @details See StatefulI::PrepareNextState. Starts the EmbeddedThread (if it was not already started) that reads from the ADC.
     * Sets the counter and the time to zero.
     * @return true if the EmbeddedThread can be successfully started.
     */
    virtual bool PrepareNextState(const char8 * const currentStateName,
            const char8 * const nextStateName);

    /**
     * TODO
     * @brief Loads and verifies the configuration parameters detailed in the class description.
     * @return true if all the mandatory parameters are correctly specified and if the specified optional parameters have valid values.
     */
    virtual bool Initialise(StructuredDataI & data);

    /**
     * TODO
     * @brief Final verification of all the parameters and setup of the board configuration.
     * @details This method verifies that all the parameters (e.g. number of samples) requested by the GAMs interacting with this DataSource
     *  are valid and consistent with the board parameters set during the initialisation phase.
     * In particular the following conditions shall be met:
     * - The type of the counter and of the time shall be 32 bit (un)signed integers.
     * - All the ADC channels have type float32.
     * - The number of samples of all the ADC channels is the same.
     * - For synchronising boards (i.e. where a Frequency was defined for a given channel):
     *  - The single ADC frequency (SamplingFrequency/NumberOfChannels) > Frequency * Samples
     * @return true if all the parameters are valid and consistent with the board parameters and if the board can be successfully configured with
     *  these parameters.
     */
    virtual bool SetConfiguredDatabase(StructuredDataI & data);

    /**
     * TODO
     */
    ErrorManagement::ErrorType FlushSegments();

    /**
     * TODO
     */
    ErrorManagement::ErrorType OpenTree(int32 pulseNumber);

private:

    //TODO
    int32 pulseNumber;

    //TODO
    bool storeOnTrigger;

    //TODO
    uint32 numberOfPreTriggers;

    //TODO
    uint32 numberOfPostTriggers;

    //TODO
    uint32 numberOfBuffers;

    //TODO
    uint32 numberOfMDSSignals;

    //TODO
    MDSWriterNode **nodes;

    //TODO
    int32 timeSignalIdx;

    //TODO
    uint32 *offsets;

    //TODO
    uint32 totalSignalMemory;

    //TODO
    char8 *dataSourceMemory;

    //TODO
    ProcessorType cpuMask;

    //TODO
    uint32 stackSize;

    //TODO
    StreamString treeName;

    //TODO
    StreamString eventName;

    //TODO
    uint32 timeRefresh;

    //TODO
    MDSplus::Tree *tree;

    //TODO
    ConfigurationDatabase originalSignalInformation;

    /**
     * Filter to receive the RPC
     */
    ReferenceT<RegisteredMethodsMessageFilter> filter;
};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* DATASOURCES_MDSWRITER_MDSWRITER_H_ */
