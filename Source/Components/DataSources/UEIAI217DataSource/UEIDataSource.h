/**
 * @file UEIDataSource.h
 * @brief Header file for class UEIDataSource
 * @date 07/02/2023
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

 * @details This header file contains the declaration of the class UEIDataSource
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef UEIDataSource_H_
#define UEIDataSource_H_

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

class UEIDataSource: public MemoryDataSourceI {
public:

    CLASS_REGISTER_DECLARATION()
    /**
     * @brief Constructor. NOOP.
     */
    UEIDataSource();

    /**
     * @brief Destructor. Stops the Embedded thread which reads from the CRIOUARTSerial.
     */
    virtual ~UEIDataSource();

    /**
     * @brief Verifies that the configuration described above is correctly set.
     * @return true if the configuration respects all the rules described in the class description.
     */
    virtual bool Initialise(StructuredDataI &data);

    /**
     * @see Checks that the signal properties are as described in the class description.
     * @return true if the signal properties respect all the rules described in the class description.
     */
    virtual bool SetConfiguredDatabase(StructuredDataI &data);

    /**
     * @see DataSourceI::TerminateInputCopy
     */
    virtual bool TerminateInputCopy(const uint32 signalIdx,
                                    const uint32 offset,
                                    const uint32 numberOfSamples);

    /**
     * @brief Assigns the correct broker for the connected signals. Checks that only input signals are provided, this
     * datasource does only support input signals.
     * @return "MemoryMapSychronisedInputBroker" if the signal is an input signal.
     */
    virtual const char8* GetBrokerName(StructuredDataI &data,
                                       const SignalDirection direction);

    /**
     * @brief Sycnhronise method for the DataSource. Performs the new data petition through the assigned MapContainer object.
     * @return true.
     */
    virtual bool Synchronise();

    virtual bool PrepareNextState(const char8 * const currentStateName,
                                  const char8 * const nextStateName);

    bool AllocateMemory();

private:

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
    uint32 poll_sleep_period;
    
    /**
     * Variable to store the signal type for all the signals on this DataSource 
     */
    TypeDescriptor signalType;
    
    /**
     * Variable to store the signal type for all the signals on this DataSource 
     */
    uint32 nOutputChannels;
    
    /**
     * Variable to store the signal type for all the signals on this DataSource 
     */
    uint32 nSamples;
    bool firstSync;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* UEIDataSource_H_ */
