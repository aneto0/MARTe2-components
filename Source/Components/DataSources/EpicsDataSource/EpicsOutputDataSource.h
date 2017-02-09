/**
 * @file EpicsOutputDataSource.h
 * @brief Header file for class EpicsOutputDataSource
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

 * @details This header file contains the declaration of the class EpicsOutputDataSource
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef EPICSOUTPUTDATASOURCE_H_
#define EPICSOUTPUTDATASOURCE_H_

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
 * @brief A data source which puts its signals' values to an EPICS IOC.
 * @details This class is an extension of the DataSourceI class, allowing to
 * write its signals' values to an EPICS IOC service by means of an output
 * broker, which will call the datasource's Synchronise method every time it
 * is executed.
 *
 * Each instance of this class will create and register a public shared memory
 * area in the OS, named "<application_name>_<datasource_name>", where the ex-
 * change of signals will occur. It is expected that the EPICS IOC connects to
 * the shared memory area and gets the last signals' values at their own pace.
 *
 * The configuration syntax is (names are only given as an example):
 * +ExportSignalsFromIOC = {
 *     Class = EpicsOutputDataSource
 * }
 *
 * A signal will be added for each GAM signal that writes to this instance of
 * the DataSourceI.
 *
 * @invariant (GetNumberOfMemoryBuffers() == 1u) and
 * (std::strcmp(GetBrokerName(INV_CDB, InputSignals), "") == 0) and
 * (std::strcmp(GetBrokerName(INV_CDB, OutputSignals), "MemoryMapSynchronisedOutputBroker") == 0) \
 */
class EpicsOutputDataSource: public DataSourceI {

public:

    CLASS_REGISTER_DECLARATION()

	/**
	 * @brief Default constructor
	 * @post
	 * 	(target.GetSharedDataAreaName().Size() == 0)
	 */
	EpicsOutputDataSource();

    /**
     * @brief Destructor.
     */
    virtual ~EpicsOutputDataSource();

    /**
     * @see DataSourceI::Synchronise()
     * @note This method will set the signals' values of the shared
     * memory area reading them from the datasource.
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
     * @post (result == 1)
     */
    uint32 GetNumberOfMemoryBuffers();

    /**
     * @see DataSourceI::GetSignalMemoryBuffer
     */
    bool GetSignalMemoryBuffer(const uint32 signalIdx, const uint32 bufferIdx, void *&signalAddress);

    /**
     * @see DataSourceI::GetBrokerName()
     * @note This method will return an empty string if the direction
     * is InputSignals.
     */
    virtual const char8 *GetBrokerName(StructuredDataI &data, const SignalDirection direction);

    /**
     * @see DataSourceI::GetInputBrokers()
     * @note This method has a null implementation, because this datasource
     * does not supply input brokers.
     */
    virtual bool GetInputBrokers(ReferenceContainer &inputBrokers, const char8* const functionName, void * const gamMemPtr);

    /**
     * @see DataSourceI::GetOutputBrokers()
     */
    virtual bool GetOutputBrokers(ReferenceContainer &outputBrokers, const char8* const functionName, void * const gamMemPtr);

    /**
     * @see StatefulI::PrepareNextState()
     */
    virtual bool PrepareNextState(const char8 * const currentStateName,
            const char8 * const nextStateName);

    /**
     * @brief Returns the name of the shared data area connected
     * to this data source.
     */
    virtual MARTe::StreamString GetSharedDataAreaName() const;

private:

    /**
     * The interface to the shared data area, as a SigblockProducer, which
     * will be used to write the signals to the shared data area.
     */
    SharedDataArea::SigblockProducer* producer;

    /**
     * The pointer to the local allocated sigblock.
     */
    Sigblock* signals;

    /**
     * The name of the shared data area.
     */
    MARTe::StreamString sharedDataAreaName;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* EPICSOUTPUTDATASOURCE_H_ */
