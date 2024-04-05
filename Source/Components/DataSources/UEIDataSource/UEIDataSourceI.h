/**
 * @file UEIDataSourceI.h
 * @brief Header file for class UEIDataSourceI
 * @date 05/05/2023
 * @author Xavier Ruche
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

 * @details This header file contains the declaration of the class UEIDataSourceI
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef UEIDataSourceI_H_
#define UEIDataSourceI_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "CLASSMETHODREGISTER.h"
#include "MemoryDataSourceI.h"
#include "StructuredDataI.h"
#include "StructuredDataIHelper.h"
#include "StreamString.h"
#include "ObjectRegistryDatabase.h"

#include "UEIMasterObject.h"
#include "UEIMapContainer.h"



/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
* @brief Base class implementing the common methods for the UEIDataSource components UEIWriter and UEIReader
* @details  This class provides the base methods which are common to the derived classes UEIWriter and UEIReader, which are to be used as 
* the MARTe components to interface to UEIDAQ interface components and the UEIDAQ API.
*
* Objects of this class should not be instantiated in a MARTe application since this class lacks any implementation of the methods providing
* data retrieval functionalities. Instead, the derived classes UEIWriter and UEIReader classes should be instantiated.
*/
class UEIDataSourceI: public MemoryDataSourceI {
public:

    CLASS_REGISTER_DECLARATION()
    /**
     * @brief Constructor.
     */
    UEIDataSourceI();

    /**
     * @brief Destructor.
     */
    virtual ~UEIDataSourceI();

    /**
     * @brief Verifies that the object configuration is correctly set.
     * @return true if the configuration provided is valid, false otherwise.
     */
    virtual bool Initialise(StructuredDataI &data);

    /**
     * @see Checks that the signal properties are as described in the class description.
     * @return true if the signal properties respect all the rules described in the class description.
     */
    virtual bool SetConfiguredDatabase();

    /**
     * @see DataSourceI::TerminateInputCopy
     */
    virtual bool TerminateInputCopy(const uint32 signalIdx,  const uint32 offset, const uint32 numberOfSamples);

    /**
     * @see DataSourceI::PrepareNextState
     */
    virtual bool PrepareNextState(const char8 * const currentStateName, const char8 * const nextStateName);

    /**
     * @brief Method which redefines the behavior of AllocateMemory method in MemoryDataSource.
     * @details This method reimplements the AllocateMemory method in MemoryDataSource to allow multiple samples to be recognized and 
     * taken into account when allocating and assigning memory to input and output signals for this DataSource
     * @param[in] direction direction in which the object initialises its signals. A UEI DataSource can only interface either input or output signals by design.
     * @return true if memory allocation succeeded, false otherwise.
     */
    bool AllocateMemory(SignalDirection direction);

    /**
     * @brief Assigns the correct broker for the connected signals.
     * @details This method is virtual and must be reimplemented by the derived classes to allow different brokers on different signal directions.
     * @param[in] data StructuredData object contatining the configuration of the component.
     * @param[in] direction direction of the signal to be used with the broker.
     * @return The name of the broker to be used by this signal when interfacing the component.
     */
    virtual const char8* GetBrokerName(StructuredDataI &data, const SignalDirection direction);

    /**
     * @brief Sycnhronise method for the DataSource.
     * @details The behaviour of this method must be specified in the derived classes, UEIReader and UEIWriter DataSources. This implementation returns false
     * by default.
     * The call to this method performs a packet-exchange with the UEIDAQ IOM to retrieve/send the data specified for the map associated to this DataSource.
     * @return false by default.
     */
    virtual bool Synchronise();

protected:

    /**
     * Variable to store the name of this DataSource
     */
    StreamString name;
    
    /**
     * Variable to store the identifier (name) of the UEIDAQ device to which connect
     */
    StreamString deviceName;
    
    /**
     * Variable to store the identifier (name) of the MapContainer object which this DataSource is connected to
     */
    StreamString mapName;

    /**
     * Reference to the UEIMasterObject containing the configuration of the desired Device
     */
    ReferenceT<UEIMasterObject> device;
    
    /**
     * Reference to the UEIMapContainer containing the desired map. This map must exist within the specified device
     */
    ReferenceT<UEIMapContainer> map;

    /**
     * Variable to store the selected polling sleep period configured for this datasource. This is the period of MARTe sleep
     * between an unsuccessful poll request to the UEIDAQ device and the next poll request. 
     */
    uint32 PollSleepPeriod;
          
    /**
     * Variable to store the signal type for all the signals on this DataSource 
     */
    uint32 nSamples;
    
    /**
     * Flag signaling if it is the first time MARTe calls the Synchronise method on this DataSource.
     * (MARTe may call the datasource Synchronise method during application initialisation but starve the thread while
     * performing initialisation tasks, in this case the FIFO servicing on a VMap can be afected and the map fail).
     */
    bool firstSync;
};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* UEIDataSourceI_H_ */
