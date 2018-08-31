/**
 * @file LinkDataSource.h
 * @brief Header file for class LinkDataSource
 * @date 06/07/2018
 * @author Giuseppe Ferro
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

 * @details This header file contains the declaration of the class LinkDataSource
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef LINKDATASOURCE_H_
#define LINKDATASOURCE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "MemoryDataSourceI.h"
#include "MemoryGate.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Implementation of a DataSource capable of reading and writing data from/to
 * an external MemoryGate object whose path is defined in the configuration.
 *
 * @details The user has to specify in the configuration the field "Link" containing the absolute path of
 * the MemoryGate object to be linked with. Moreover the user has to specify in the configuration the flag
 * "IsWriter" that defines if this data source has to write or reads its signals memory form/to the linked
 * MemoryGate within the Synchronise() function.
 *
 * @details Follows an example of configuration.
 * <pre>
 *  +InputGAM1 = {
 *      Class = LinkDataSource
 *      Link = ExternalComponent1
 *      IsWriter = 0
 *  }
 *  </pre>
 */
class LinkDataSource: public MemoryDataSourceI {
public:

    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor
     */
LinkDataSource    ();

    /**
     * @brief Destructor
     */
    virtual ~LinkDataSource();

    /**
     * @see MemoryDataSourceI::AllocateMemory
     * @details calls MemoryGate::SetMemorySize to check if all the components
     * linked to the MemoryGate declare the same memory size.
     * @return true if if all the components linked to the MemoryGate declare the same memory size.
     */
    virtual bool AllocateMemory();

    /**
     * @see ReferenceContainer::Initialise
     * @details The following configuration variables shall be defined:
     *   Link = "the link of the MemoryGate component"
     *   IsWriter = [0-1] specifies if the signals memory must be written or read in the Synchronise() function.
     * @return true if all the configuration parameters are correctly defined.
     */
    virtual bool Initialise(StructuredDataI &data);

    /**
     * @see DataSourceI::Synchronise.
     * @details If IsWriter==1 calls MemoryGate::MwmoryWrite() otherwise it calls MemoryGate::MemoryRead.
     * @return true.
     */
    virtual bool Synchronise();

    /**
     * @brief Returns true.
     */
    virtual bool PrepareNextState(const char8 * const currentStateName,
            const char8 * const nextStateName);

    /**
     * @brief Returns the broker name.
     * @return If direction == InputSignals, returns MemoryMapSynchronisedMultiBufferInputBroker, otherwise returns MemoryMapSynchronisedMultiBufferOutputBroker.
     */
    virtual const char8 *GetBrokerName(StructuredDataI &data,
            const SignalDirection direction);

protected:

    /**
     * The link to the linked MemoryGate component.
     */
    StreamString linkPath;

    /**
     * A reference to the linked MemoryGate Component
     */
    ReferenceT<MemoryGate> link;

    /**
     * Denotes if data must be read or written from/to the MemoryGate
     */
    uint8 isWriter;

};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* LINKDATASOURCE_H_ */

