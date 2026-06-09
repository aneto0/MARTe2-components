/**
 * @file NI6368DIO.h
 * @brief Header file for class NI6368DIO
 * @date 30/01/2017
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

 * @details This header file contains the declaration of the class NI6368DIO
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef NI6368_NI6368DIO_H_
#define NI6368_NI6368DIO_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
/*lint -u__cplusplus This is required as otherwise lint will get confused after including this header file.*/
#include "xseries-lib.h"
/*lint -D__cplusplus */

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "DataSourceI.h"
#include "EmbeddedServiceMethodBinderI.h"
#include "EventSem.h"
#include "SingleThreadService.h"

/*lint -e1923 allow macros*/
#define PXI6363_NUMBER_OF_PORTS 3U
/*lint +e1923 */

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {
/**
 * @brief A DataSource which provides a digital input/output interface to the NI6368 boards.
 * @details The configuration syntax is (names are only given as an example):
 *
 * @details It is possible to define one optional signal which must be declared before the channels.
 *   - Signal 0 (Status): the board status.
 * The board status is a uint32 bitmask. Follows the error description for each bit:
 *   0: ADC Overrun detected in STC3 (Intimer)
 *   1: AI FIFO overflow detected in STC3 (Intimer)
 *   2: AI FIFO overflow detected in Simultaneous Control
 *   4: AI DMA channel error happened, the channel shall be restarted
 *   5: DI Overrun error happened
 *   6: DI Overflow error happened
 *   8: DI DMA channel error happend, the channel shall be restarted
 *   10: AO Overrun error detected
 *   11: AO Underflow error detected
 *   12: AO write too fast error detected
 *   13: AO DMA channel error happend, the channel shall be restarted
 *   14: DO Overrun error detected
 *   15: DO Underflow error detected
 *   16: DO Write too fast detected
 *   17: DO DMA channel error happend, the channel shall be restarted
 *   20: GPC0 DMA channel error happend, the channel shall be restarted
 *   21: GPC1 DMA channel error happend, the channel shall be restarted
 *   22: GPC2 DMA channel error happend, the channel shall be restarted
 *   23: GPC3 DMA channel error happend, the channel shall be restarted
 *
 * <pre>
 * +NI6368_0_DIO = {
 *
 *     Class = NI6368::NI6368DIO
 *     DeviceName = "/dev/pxie-6368"
 *     BoardId = 0
 *     Signals = {
 *         Status = { //Optional. The board status
 *             Type = uint32
 *         }
 *         Port0 = {
 *             Type = uint32 //Mandatory. Only type that is supported.
 *             PortId = 0 //Mandatory. The port id (0, 1 or 2)
 *             Mask = 0xFFFF //Mandatory. A mask where each bit defines if the pin is an input (bit=0) or an output (bit=1).
 *             InversionMask = 0xF //Optional, a mask with bits to invert before writing or after reading. Default is 0 (no invert)
 *         }
 *         Port1 = {
 *             Type = uint32 //Mandatory. Only type that is supported.
 *             PortId = 1 //Mandatory. The port id (0, 1 or 2)
 *             Mask = 0xFF //Mandatory. A mask where each bit defines if the pin is an input (bit=0) or an output (bit=1).
 *         }
 *         Port2 = {
 *             Type = uint32 //Mandatory. Only type that is supported.
 *             PortId = 2 //Mandatory. The port id (0, 1 or 2)
 *             Mask = 0xFF //Mandatory. A mask where each bit defines if the pin is an input (bit=0) or an output (bit=1).
 *         }
 *     }
 * }
 * </pre>
 */

class NI6368DIO: public DataSourceI {
public:
    CLASS_REGISTER_DECLARATION()
    /**
     * @brief Default constructor.
     * @details Initialises all the parameters described in the class description.
     */
NI6368DIO    ();

    /**
     * @brief Destructor.
     * @details Closes all the file descriptors associated to the board, including any opened ports.
     */
    virtual ~NI6368DIO();

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
     * @return For OutputSignals: MemoryMapSynchronisedOutputBroker.
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
     * - The DIO channel has type uint32.
     * - The number of samples of the DIO channel is exactly one.
     * - The number of elements of the DIO channel is exactly one.
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
    bool ReadDIOConfiguration(xseries_di_conf_t *confDI, xseries_do_conf_t *confDO) const;
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
     * The PFI file descriptor
     */
    int32 pfiFileDescriptor;

    /**
     * The board status file descriptor
     */
    int32 rootFileDescriptor;

    /**
     * The input mask of the DIO port
     */
    uint32 portMask[PXI6363_NUMBER_OF_PORTS];

    /**
     * The current port value.
     */
    uint32 portValue[PXI6363_NUMBER_OF_PORTS];

    /**
     * If the DIO port is enabled
     */
    bool portEnabled[PXI6363_NUMBER_OF_PORTS];

    /**
     * The inversion mask
     */
    uint32 inversionMask[PXI6363_NUMBER_OF_PORTS];

    /**
     * If this data source is sync
     */
    bool triggerSet;

    /**
     * The number of enabled DIO ports
     */
    uint32 numberOfPortsEnabled;

    /**
     * The number of signals in header
     */
    uint32 headerSize;

    /**
     * The board status
     */
    uint32 boardStatus;

};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* NI6368_NI6368DIO_H_ */

