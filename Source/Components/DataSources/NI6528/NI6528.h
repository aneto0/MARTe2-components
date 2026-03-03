/**
 * @file NI6528.h
 * @brief Header file for class NI6528
 * @date 07/06/2018
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

 * @details This header file contains the declaration of the class NI6528
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef NI6528_NI6528OUTPUT_H_
#define NI6528_NI6528OUTPUT_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
#include <pxi6528.h>

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "DataSourceI.h"

#define PXI6528_NUMBER_OF_OUTPUT_PORTS_ 3
#define PXI6528_NUMBER_OF_INPUT_PORTS_ 3
#define PXI6528_NUMBER_OF_PORTS_ 6
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {

/**
 * @brief NI6528 data source implementation.
 *
 * @details the NI-6528 board provides 6 DIO ports, each with 8 digital pins. Ports 0, 1 and 2 are digital inputs, ports 3, 4 and 5 are digital outputs.
 *
 * @details the NI-6528 digital outputs are actually optical relays which are normally closed. This means that, if connected to a voltage source, they are normally
 * connecting voltage in output.
 *
 * The configuration syntax is (names and signal quantities are only given as an example):
 * +NI6528 = {
 *     Class = NI6528
 *     DeviceName = "/dev/pxi6528" //The device name
 *     BoardId = 0 //The board id
 *     Signals = {
 *         Port0 = {
 *             PortId = 0
 *             Type = uint8 //The only type supported
 *             InversionMask = 0xF //Optional, a mask with bits to invert before writing or after reading. Default is 0 (no invert)
 *         }
 *         Port1 = {
 *             PortId = 1
 *             Type = uint8 //The only type supported
 *         }
 *         Port2 = {
 *             PortId = 2
 *             Type = uint8 //The only type supported
 *         }
 *         Port3 = {
 *             PortId = 3
 *             Type = uint8 //The only type supported
 *         }
 *         Port4 = {
 *             PortId = 4
 *             Type = uint8 //The only type supported
 *         }
 *         Port5 = {
 *             PortId = 5
 *             Type = uint8 //The only type supported
 *             InversionMask = 0xF //Optional, a mask with bits to invert before writing or after reading. Default is 0 (no invert)
 *
 *         }
 *     }
 * }
 */

class NI6528: public DataSourceI {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor. NOOP.
     */
NI6528    ();

    /**
     * @brief Destructor. NOOP.
     */
    virtual ~NI6528();

    /**
     * @brief The configuration data detailed in the class description
     * @return true if all the compulsory parameters are set.
     */
    virtual bool Initialise(StructuredDataI & data);

    /**
     * @brief Verifies that at most one signal has been set with the correct type (i.e. any integer).
     * @return true if the above conditions are met.
     */
    virtual bool SetConfiguredDatabase(StructuredDataI & data);

    /**
     * @brief @see DataSourceI::Synchronise
     */
    virtual bool Synchronise();

    /**
     * @brief @see DataSourceI::AllocateMemory
     */
    virtual bool AllocateMemory();

    /**
     * @brief @see DataSourceI::GetSignalMemoryBuffer
     */
    virtual bool GetSignalMemoryBuffer(const uint32 signalIdx, const uint32 bufferIdx, void *&signalAddress);

    /**
     * @return "MemoryMapSynchronisedInputBroker"
     */
    virtual const char8 *GetBrokerName(StructuredDataI &data, const SignalDirection direction);

    /**
     * @brief NOOP
     */
    virtual bool PrepareNextState(const char8 * const currentStateName, const char8 * const nextStateName);

private:
    /**
     * The value to write.
     */
    uint8 value[PXI6528_NUMBER_OF_PORTS_];

    /**
     * If the port is enabled
     */
    uint32 portEnabled[PXI6528_NUMBER_OF_PORTS_];

    /**
     * The inversion mask
     */
    uint8 inversionMask[PXI6528_NUMBER_OF_PORTS_];

    /**
     * The board file descriptor
     */
    pxi6528_device_t boardFileDescriptor;

    /**
     * The device name
     */
    StreamString deviceName;

    /**
     * The board identifier
     */
    uint32 boardId;

};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* NI6528_NI6528OUTPUT_H_ */

