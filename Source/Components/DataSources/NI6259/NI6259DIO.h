/**
 * @file NI6259DIO.h
 * @brief Header file for class NI6259DIO
 * @date 03/01/2017
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

 * @details This header file contains the declaration of the class NI6259DIO
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef NI6259_NI6259DIO_H_
#define NI6259_NI6259DIO_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
#include "pxi-6259-lib.h"

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "DataSourceI.h"
#include "EmbeddedServiceMethodBinderI.h"
#include "EventSem.h"
#include "SingleThreadService.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {
/**
 * @brief A DataSource which provides a digital input/output interface to the NI6259 boards.
 * @details The DataSource shall always be ...
 *
 * +NI6259_0_DIO = {
 *     Class = NI6259::NI6259DIO
 *     DeviceName = "/dev/pxi6259" //Mandatory
 *     BoardId = 0 //Mandatory
 *     Signals = {
 *         DIO0_0 = {
 *             Type = uint32 //Mandatory. Only type that is supported.
 *             PortId = 0 //Mandatory. The port number in the range [0, 2]
 *             Mask = 0x1 //Mandatory. A mask where each bit defines if the pin is an input (bit=0) or an output (bit=1).
 *         }
 *     }
 * }
 *
 * Note that at least one of the GAMs writing to this DataSource must have set one of the signals with Trigger=1 (which forces the writing of all the signals to the DIO ports).
 */

class NI6259DIO: public DataSourceI {
public:
    CLASS_REGISTER_DECLARATION()
    /**
     * @brief Default constructor.
     * @details Initialises all the parameters described in the class description.
     */
NI6259DIO    ();

    /**
     * @brief Destructor.
     * @details Closes all the file descriptors associated to the board, including any opened ports.
     */
    virtual ~NI6259DIO();

    /**
     * @brief See DataSourceI::AllocateMemory.
     * @return true.
     */
    virtual bool AllocateMemory();

    /**
     * @brief See DataSourceI::GetNumberOfMemoryBuffers.
     * @return 1.
     */
    virtual uint32 GetNumberOfMemoryBuffers();

    /**
     * @brief See DataSourceI::GetNumberOfMemoryBuffers.
     */
    virtual bool GetSignalMemoryBuffer(const uint32 signalIdx,
            const uint32 bufferIdx,
            void *&signalAddress);

    /**
     * @brief See DataSourceI::GetBrokerName.
     * @return For OutputSignals: MemoryMapSynchronisedOutputBroker if Trigger == 1 for any of the signals, MemoryMapOutputBroker otherwise.
     * For InputSignals: MemoryMapSynchronisedInputBroker.
     */
    virtual const char8 *GetBrokerName(StructuredDataI &data,
            const SignalDirection direction);

    /**
     * @brief See DataSourceI::GetInputBrokers.
     * @details Adds a MemoryMapSynchronisedInputBroker instance to the inputBrokers.
     * @param[out] inputBrokers where the BrokerI instances have to be added to.
     * @param[in] functionName name of the function being queried.
     * @param[in] gamMemPtr the GAM memory where the signals will be read from.
     * @return true if the inputBrokers can be successfully configured.
     */
    virtual bool GetInputBrokers(ReferenceContainer &inputBrokers,
            const char8* const functionName,
            void * const gamMemPtr);

    /**
     * @brief See DataSourceI::GetOutputBrokers.
     * @details If the functionName is one of the functions which requested a Trigger,
     * it adds a MemoryMapSynchronisedOutputBroker instance to the outputBrokers,
     * otherwise it adds a MemoryMapOutputBroker instance to the outputBrokers.
     * @param[out] outputBrokers where the BrokerI instances have to be added to.
     * @param[in] functionName name of the function being queried.
     * @param[in] gamMemPtr the GAM memory where the signals will be read from.
     * @return true if the outputBrokers can be successfully configured.
     */
    virtual bool GetOutputBrokers(ReferenceContainer &outputBrokers,
            const char8* const functionName,
            void * const gamMemPtr);

    /**
     * @brief See StatefulI::PrepareNextState.
     * @details NOOP.
     * @return true.
     */
    virtual bool PrepareNextState(const char8 * const currentStateName,
            const char8 * const nextStateName);

    /**
     * @brief Loads and verifies the configuration parameters detailed in the class description.
     * @return true if all the mandatory parameters are correctly specified.
     */
    virtual bool Initialise(StructuredDataI & data);

    /**
     * @brief Final verification of all the parameters and setup of the board configuration.
     * @details This method verifies that all the parameters (e.g. number of samples) requested by the GAMs interacting with this DataSource
     *  are valid and consistent with the board parameters set during the initialisation phase.
     * In particular the following conditions shall be met:
     * - At least one triggering signal was requested by a GAM which wants to write to this board (with the property Trigger = 1)
     * - All the DIO channels have type uint32.
     * - The number of samples of all the DIO channels is exactly one.
     * @return true if all the parameters are valid and consistent with the board parameters and if the board can be successfully configured with
     *  these parameters.
     */
    virtual bool SetConfiguredDatabase(StructuredDataI & data);

    /**
     * @details Writes the value of all the DIO channels to the board, followed by a read of all the DIO channels from the board.
     * @return true if the reading and writing of all the channels is successful.
     */
    virtual bool Synchronise();

    /**
     * @brief Helper function to test the class.
     * @details This method is meant to be used by the unit-test class in order to verify the correct setting of the board parameters.
     * No real added value on making getters for all the structure elements, just for the sake of testing.
     * @return true if the board was opened (i.e. if SetConfiguredDatabase was successful).
     */
    bool ReadDIOConfiguration(pxi6259_dio_conf_t *conf) const;
private:

    /**
     * The board identifier
     */
    uint32 boardId;

    /**
     * The board device name
     */
    StreamString deviceName;

    /**
     * The board file descriptor
     */
    int32 boardFileDescriptor;

    /**
     * The DIOs that are enabled
     */
    bool portEnabled[PXI6259_NUMBER_OF_PORTS];

    /**
     * The i/o mask of each port
     */
    uint32 portMask[PXI6259_NUMBER_OF_PORTS];

    /**
     * The current port values.
     */
    uint32 portValues[PXI6259_NUMBER_OF_PORTS];

    /**
     * Number of ports enabled for this board.
     */
    uint32 numberOfPortsEnabled;

    /**
     * The ports file descriptors
     */
    int32 portFileDescriptors[PXI6259_NUMBER_OF_PORTS];

    /**
     * True if at least one trigger was set.
     */
    bool triggerSet;

    /**
     * True if this board is used to write digital values.
     */
    bool outputsEnabled;
};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* NI6259_NI6259DIO_H_ */

