/**
 * @file NI9157MxiDataSource.h
 * @brief Header file for class NI9157MxiDataSource
 * @date 23/05/2018
 * @author Giuseppe Ferrò
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

 * @details This header file contains the declaration of the class NI9157MxiDataSource
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef NI9157MXIDATASOURCE_H_
#define NI9157MXIDATASOURCE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "MemoryDataSourceI.h"
#include "MessageI.h"
#include "NI9157DeviceOperatorT.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe{

/**
 * @brief A class to communicate with a NI-9157 device
 * @details This object allows to read from Labview exported indicator variables, to write on Labview exported control variables and to read and write from/to FIFOs.
 * @details The names of the signals in the data source should be equal to the names of the Labview exported variables which the user want to read from or write to. If the
 * NumberOfDumensions parameter of the signal is greater than one, the signal is assumed to be a FIFO.
 * @details The two functions AsyncRead and AsyncWrite can be called using the Message interface to read and write simple variables (indicators and controls, no FIFOs).
 * @details Follows a configuration example:
 * <pre>
 * +Drv1 = {
 *     Class = NI9157MxiDataSource
 *     RunNi = 1 //if the device must be started from this data source
 *     NI9157DevicePath = NiDevice //the absolute path of the device in the configuration database
 *     NumberOfPacketsInFIFO = 10 //the number of packets in the FIFO host side. This parameter is valid for all the defined FIFOs. By default is 5.
 *     Signals = {
 *         // read from indicator
 *         NiFpga_TestGTD0001_IndicatorU32_Tick_Count_Ticks = {
 *             Type = uint32"
 *         }
 *         // read from FIFO
 *         NiFpga_TestGTD0001_TargetToHostFifoU64_FIFO = {
 *             Type = uint64
 *             NumberOfDimensions = 1
 *             NumberOfElements = 10000
 *         }
 *         // write on control
 *         NiFpga_TestGTD0001_ControlU8_options = {
 *             Type = uint8
 *         }
 *         // write on FIFO
 *         NiFpga_TestGTD0001_HostToTargetFifoU16_FIFO2 = {
 *             Type = uint16
 *             NumberOfDimensions = 1
 *             NumberOfElements = 1
 *         }
 *     }"
 * }
 * </pre>
 */
class NI9157MxiDataSource: public MemoryDataSourceI, public MessageI {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor
     * @post
     *   numberOfPacketsInFifo = 0u;
     *   niDevice = NULL_PTR(NI9157DeviceOperatorTI **);
     *   runNi = 0u;
     *   numberOfSignals = 0u;
     *   signalFlag = NULL_PTR(uint8 *);
     *   varId = NULL_PTR(uint32 *);
     *   numberOfElements = NULL_PTR(uint32 *);
     */
    NI9157MxiDataSource();

    /**
     * @brief Destructor
     * @details Frees the memory
     */
    virtual ~NI9157MxiDataSource();

    /**
     * @see MemoryDataSourceI::Initialise
     * @details Follows the list of parameters to be configured by the user for initialisation:\n
     *     RunNi: if the device must be started from this data source.\n
     *     NI9157DevicePath: the absolute path of the device in the configuration database.\n
     *     NumberOfPacketsInFIFO: the number of packets in the FIFO host side. This parameter is valid for all the defined FIFOs. By default is 5.
     */
    virtual bool Initialise(StructuredDataI &data);

    /**
     * @see MemoryDataSourceI::GetBrokerName
     * @details
     *   - If (Trigger == 1) || (Frequency > 0.) return MemoryMapSynchronisedInputBroker if (direction == InputSignals) and MemoryMapSynchronisedOutputBroker if
     *   (direction == OutputSignals).\n
     *   - If (Trigger == 0) && (Frequency == 0.) return MemoryMapInputBroker if (direction == InputSignals) and MemoryMapOutputBroker if
     *   (direction == OutputSignals).\n
     */
    virtual const char8 *GetBrokerName(StructuredDataI &data,
                                       const SignalDirection direction);

    /**
     * @see MemoryDataSourceI::PrepareNextState
     * @details Sets the member values of signalFlags for each signal storing if the signal has to be read, write and if it is a FIFO.
     * Then, if RunNi == 1, starts the NI-9157 device.
     */
    virtual bool PrepareNextState(const char8 * const currentStateName,
                                  const char8 * const nextStateName);

    /**
     * @see MemoryDataSourceI::SetConfiguredDatabase
     * @details Creates the NI-9157 device operators for each signal looking at the configured signal type. It returns false if
     * the signal name does not correspond to a valid variable name exported in the Labview header file.
     */
    virtual bool SetConfiguredDatabase(StructuredDataI & data);

    /**
     * @brief Reads and writes all the signals from/to the internal memory buffers.
     */
    virtual bool Synchronise();

    /**
     * @brief This method allows to read from a Labview exported indicator. It can be called within a MARTe message.
     */
    ErrorManagement::ErrorType AsyncRead(StreamString varName,
                                         uint64 &varValue);

    /**
     * @brief This method allows to write to a Labview exported control. It can be called within a MARTe message.
     */
    ErrorManagement::ErrorType AsyncWrite(StreamString varName,
                                          uint64 varValue);

protected:

    /**
     * The reference to the NI9157Device
     */
    ReferenceT<NI9157Device> niDeviceBoard;

    /**
     * The NI9157Device operator
     */
    NI9157DeviceOperatorTI **niDevice;

    /**
     * The number of packets in the host-side FIFOs
     */
    uint32 numberOfPacketsInFifo;

    /**
     * Holds the descriptors of the Labview exported variabled for each signal
     */
    uint32 *varId;

    /**
     * If the device has to be started by this data source
     */
    uint8 runNi;

    /**
     * Defines for each signal if it has to be read or write, and if it is a FIFO.
     */
    uint8 *signalFlag;

    /**
     * Holds the number of elements for each signal.
     */
    uint32 *numberOfElements;
};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* NI9157MXIDATASOURCE_H_ */

