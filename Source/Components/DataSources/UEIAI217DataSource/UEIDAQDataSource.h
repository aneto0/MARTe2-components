/**
 * @file UEIDAQDataSource.h
 * @brief Header file for class UEIDAQDataSource
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

 * @details This header file contains the declaration of the class UEIDAQDataSource
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef UEIDAQDataSource_H_
#define UEIDAQDataSource_H_

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

#include "DAQMasterObject.h"
#include "DAQMapContainer.h"



/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief TODO
 *
 *
 *
 *
 *
 *
 * <pre>
 *  +UEIDAQDS = {
 *      Class   = UEIDAQDataSource
 *      Device  = UEIDevice1
 *      Map     = Map1
 *  }
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
*/

class UEIDAQDataSource: public MemoryDataSourceI {
public:

    CLASS_REGISTER_DECLARATION()
    /**
     * @brief Constructor. NOOP.
     */
    UEIDAQDataSource();

    /**
     * @brief Destructor. Stops the Embedded thread which reads from the CRIOUARTSerial.
     */
    virtual ~UEIDAQDataSource();

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
     * @return "MemoryMapMultiBufferInputBroker"
     */
    virtual const char8* GetBrokerName(StructuredDataI &data,
                                       const SignalDirection direction);

    /**
     * @return true.
     */
    virtual bool Synchronise();

    bool GetMapAddr();
    bool PollForNextPacket();

    virtual bool PrepareNextState(const char8 * const currentStateName,
                                  const char8 * const nextStateName);

private:

    /**
     * Variable to store the selected DAQ mode
     */
    StreamString name;
    StreamString deviceName;
    StreamString mapName;

    /**
     * Reference to the UEIMasterObject containing the configuration of the desired Device
     */
    ReferenceT<DAQMasterObject> device;
    
    /**
     * Reference to the UEIMapContainer containing the desired map. This map must exist within the specified device
     */
    ReferenceT<DAQMapContainer> map;

    uint32 poll_sleep_period;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* UEIDAQDataSource_H_ */
