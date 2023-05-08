/**
 * @file UEIWriter.h
 * @brief Header file for class UEIWriter
 * @date 20/03/2023
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

 * @details This header file contains the declaration of the class UEIWriter
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef UEIWriter_H_
#define UEIWriter_H_

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
#include "UEIDataSourceI.h"



/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
* @brief DataSource implementation to be used with the UEIDAQ interface and UEIDAQ devices within MARTe to send data into the IOM for output capable devices.
*
* @details This DataSource implements the needed methods and mechanics to send data from MARTe into a/various UEIDAQ hardware layer/s.
* The DataSource needs to be linked to an existing UEIDAQ interface within the same application describing the desired configuration for the device
* and its different layers.
*
* The DataSource configuration must provide the Device paramter, which contains a string referencing the name of the UEIMasterObject object instantiated
* within the application which will serve as the interface to the UEIDAQ device. The DataSource must push the data through a Map object (see UEIRtDMap and
* UEIRtVMap) which defines the acquisition method and parameters. The configuration must also contain a Map parameter with a string referencing the name of the
* Map object associated to the acquisition scheme.
*
* The DataSource can only be used as an output DataSource where data from MARTe sinks into the UEIDAQ hardware layers. The number of output signals must be equal
* to the number of configured output channels on the hardware layers. The output signal types must match the accepted types for their respective hardware layer
* , othwerwise the DataSource setup procedure will fail. The number of samples requested for each of the output signals must be equal. No additional signals
* must be supplied (such as Timestamp or Status in UEIReader).
*
* See the example below for a configuration of the DataSource:
* <pre>
*   +UEIWriter1 = {
*        Class = UEIDataSourceI::UEIWriter
*        Device = "UEIDevice"
*        Map = "Map1"
*        Signals = {
*            Input0 = {
*                Type = float32
*            }
*            Input1 = {
*                Type = float64
*            }
*        }
*    }
* </pre>
*
*/

class UEIWriter : public UEIDataSourceI {
public:

    CLASS_REGISTER_DECLARATION()
    /**
     * @brief Constructor.
     */
    UEIWriter();

    /**
     * @brief Destructor.
     */
    virtual ~UEIWriter();

    /**
     * @brief Verifies that the configuration described above is correctly set.
     * @return true if the configuration respects all the rules described in the class description.
     */
    virtual bool Initialise(StructuredDataI &data);

    /**
     * @see Checks that the signal properties are as described in the class description.
     * @return true if the signal properties respect all the rules described in the class description.
     */
    bool SetConfiguredDatabase(StructuredDataI &data);

    /**
     * @see DataSourceI::TerminateInputCopy
     */
    virtual bool TerminateInputCopy(const uint32 signalIdx,  const uint32 offset, const uint32 numberOfSamples);

    /**
     * @brief Prepares the Datasource for the next state.
     * @details This method performs a reset of the DataSource and stops the current map for later restart.
     * @return true by default.
     */
    virtual bool PrepareNextState(const char8 * const currentStateName, const char8 * const nextStateName);
    
    /**
     * @brief Assigns the correct broker for the connected signals.
     * @details This method configures a MemoryMapSynchronisedOutputBroker for each of the configured output signals.
     * @param[in] data StructuredData object contatining the configuration of the component.
     * @param[in] direction direction of the signal to be used with the broker.
     * @return "MemoryMapSynchronisedOutputBroker" for the output signals, "" for the input signals.
     */
    const char8* GetBrokerName(StructuredDataI &data, const SignalDirection direction);

    /**
     * @brief Sycnhronise method for the DataSource.
     * @details This method calls the SetOutputs method of the configured map objects to push the latest data into the IOM.
     * @return true by default.
     */
    bool Synchronise();

    
    /**
     * @brief Allocates the memory needed for this DataSource implementation.
     * @details This method calls the AllocateMemory implementation in UEIDataSourceI with the appropriate signal direction.
     * @return true if memory allocation succeeded, false otherwise.
     */
    bool AllocateMemory();
};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* UEIWriter_H_ */
