/**
 * @file EpicsInputDataSource.h
 * @brief Header file for class EpicsInputDataSource
 * @date 01/12/2016
 * @author Ivan Herrero Molina
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

 * @details This header file contains the declaration of the class EpicsInputDataSource
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef EPICSINPUTDATASOURCE_H_
#define EPICSINPUTDATASOURCE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "CompilerTypes.h"
#include "DataSourceI.h"
#include "SharedDataArea.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief A data source which gets its signals' values from an EPICS IOC.
 * @details This class is an extension of the DataSourceI class, allowing
 * to read its signals' values from an EPICS IOC service by means of an input
 * broker, which will call the datasource's Synchronise method every time it
 * is executed.
 *
 * Each instance of this class will create and register a public shared memory
 * area in the OS, named "<application_name>_<datasource_name>", where the ex-
 * change of signals will occur. It is expected that the EPICS IOC connects to
 * the shared memory area and puts in the fresher signals' values.
 *
 * The configuration syntax is (names are only given as an example):
 * +ImportSignalsFromIOC = {
 *     Class = EpicsInputDataSource
 * }
 *
 * A signal will be added for each GAM signal that reads to this instance of
 * the DataSourceI.
 */
class EpicsInputDataSource: public DataSourceI {

public:

    CLASS_REGISTER_DECLARATION();

	/**
	 * @brief Default constructor
	 */
	EpicsInputDataSource();

    /**
     * @brief Destructor.
     */
    virtual ~EpicsInputDataSource();

    /**
     * @see DataSourceI::SetConfiguredDatabase()
     */
    virtual bool SetConfiguredDatabase(StructuredDataI & data);

    /**
     * @see DataSourceI::Synchronise()
     * @note It will set the signals' values of the datasource reading them
     * from the shared memory area.
     */
    bool Synchronise();

    /**
     * @see DataSourceI::AllocateMemory()
     * @note This method will allocate and configure the shared memory
     * area based on the signals owned by the datasource itself.
     */
    bool AllocateMemory();

    /**
     * @see DataSourceI::GetNumberOfMemoryBuffers()
     */
    uint32 GetNumberOfMemoryBuffers();

    /**
     * @see DataSourceI::GetSignalMemoryBuffer
     */
    bool GetSignalMemoryBuffer(const uint32 signalIdx, const uint32 bufferIdx, void *&signalAddress);

    /**
     * @see DataSourceI::GetBrokerName()
     * @note This method will return an empty string if the direction
     * is OutputSignals.
     */
    virtual const char8 *GetBrokerName(StructuredDataI &data, const SignalDirection direction);

    /**
     * @see DataSourceI::GetInputBrokers()
     */
    virtual bool GetInputBrokers(ReferenceContainer &inputBrokers, const char8* const functionName, void * const gamMemPtr);

    /**
     * @see DataSourceI::GetOutputBrokers()
     * @note This method has a null implementation, because this datasource
     * does not supply output brokers.
     */
    virtual bool GetOutputBrokers(ReferenceContainer &outputBrokers, const char8* const functionName, void * const gamMemPtr);

    /**
     * @see StatefulI::PrepareNextState()
     */
    virtual bool PrepareNextState(const char8 * const currentStateName,
            const char8 * const nextStateName);

private:

    /**
     * The interface to the shared data area, as a SigblockConsumer, which
     * will be used to read the signals from the shared data area.
     */
    SharedDataArea::SigblockConsumer* consumer;

    /**
     * The pointer to the local allocated sigblock.
     */
    Sigblock* signals;
};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* EPICSINPUTDATASOURCE_H_ */
