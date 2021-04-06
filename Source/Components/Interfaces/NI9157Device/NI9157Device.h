/**
 * @file NI9157Device.h
 * @brief Header file for class NI9157Device.
 * @date 11/02/2021
 * @author Giuseppe Ferro
 * @author Pedro Lourenco
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
 *
 * @details This header file contains the declaration of the class NI9157Device
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef NI9157DEVICE_H_
#define NI9157DEVICE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
#include <NiFpga.h>

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "MessageI.h"
#include "ReferenceContainer.h"
#include "StreamString.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {

/**
 * @brief Wraps the NiFpga.h API to interface with the NI-9157 Compact-RIO chassis.
 * @details This class holds the handle of a NI-9157 device allowing other objects such as MARTe DataSources to communicate (and thus share)
 * with the same device.
 *
 * @details This class allows the user to perform the initial configuration of the NI-9157 FPGA directly within the MARTe configuration file.
 * The parameters to be configured match the names of the LabVIEW project variables, but they have to be preceded by its type (Bool_, U8_, I8, U16_,
 * I16_, U32_, I32_, U64_, I64_).
 * <pre>
 * +NiDevice = {
 *     Class = NI9157Device
 *     NiRioDeviceName = RIO0
 *     NiRioGenFile = "/path/to/the/NiRio/Gen/file/NiFpga_firmware.lvbitx"
 *     NiRioGenSignature = "CCF43684FE70CCDB4E23B1D2DF50940C"
 *     Open = 1 // specifies if the device must be opened or not. If 0, the Configuration block has no meaning.
 *              // Open = 0 is assumed if this parameter is not set.
 *     Reset = 0// specifies if the device should be reseted after being opened.
 *              // Run = 0 is assumed if this parameter is not set.
 *     Run = 0  // specifies if the device should be set into run mode after being opened.
 *              // Run = 0 is assumed if this parameter is not set.
 *     Configuration = {
 *         ConfigureBool_stop = 0
 *         ConfigureBool_use_dsfifo_data = 0
 *         ConfigureBool_use_counter = 1
 *         ConfigureU32_cycle_ticks = 200
 *         ConfigureU64_packet_size = 1
 *     }
 * }
 * </pre>
 * Where:
 * - NiRioDeviceName: the name of the device that appears in "/dev" after connecting the MXI cable.
 * - NiRioGenFile: the bitstream file exported by LabVIEW.
 * - NiRioGenSignature: a code that can be found in the LabVIEW exported header file (or by opening the bitstream file as text).
 * - In the Configuration block, it is possible to automatically initialise the LabVIEW exported variables. Their names can be found in the
 * LabVIEW exported header file. Make sure that the variable names contain the variable types (Bool, U8, I8, U16, I16, U32, I32, U64, I64).
 */
class NI9157Device: public ReferenceContainer, public MessageI {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor. NOOP.
     */
    NI9157Device ();

    /**
     * @brief Destructor
     * @details Resets and closes the NI-9157 device.
     */
    virtual ~NI9157Device();

    /**
     * @brief Initialises the NI-9157 device using the parameters list in the class description.
     * @return true if the device is successfully initialised.
     */
    virtual bool Initialise(StructuredDataI & data);

    /**
     * @brief Opens the NI-9157 device.
     * @details. It can be done in the Initialise function specifying the flag Open as true (=1) in the configuration file.
     * If the flag (isOpened == 1) the function does nothing because the device has been already opened.
     * @return status=0 if succeeds, status=[error_code] if it fails.
     * @post
     *   IsOpened = 1 (if the function succeeds)
     */
    virtual NiFpga_Status Open();

    /**
     * @brief Runs the NI-9157 FPGA program on the device.
     * @details This function can be performed by one of the objects (e.g MARTe DataSources) linked to this device.
     * If the flag (isRunning == 1) the function does nothing because the program has been already started.
     * @return status=0 if succeeds, status=[error_code] if it fails.
     * @post
     *   isRunning = 1 (if the function succeeds)
     */
    virtual NiFpga_Status Run();

    /**
     * @brief Returns 1 if the NI-9157 device has been opened, 0 otherwise.
     * @return 1 if the NI-9157 device has been opened, 0 otherwise.
     */
    uint8 IsOpened() const;

    /**
     * @brief Returns 1 if the NI-9157 device has been started, 0 otherwise.
     * @return 1 if the NI-9157 device has been started, 0 otherwise.
     */
    uint8 IsRunning() const;

    /**
     * @brief Resets the FPGA program on the NI-9157 device.
     * @return status=0 if succeeds, status=[error_code] if fails.
     */
    NiFpga_Status Reset();

    /**
     * @brief Closes the FPGA program on the NI-9157 device.
     * @return status=0 if succeeds, status=[error_code] if fails.
     */
    NiFpga_Status Close();

    /**
     * @brief Finds a bool variable and returns its descriptor
     * @param[in] varName the LabVIEW variable name
     * @param[in] type the value is not important, it is used to select the uint8 type
     * @param[out] varDescriptor the returned variable descriptor.
     * @return 0 if the method succeeds, error code if it fails.
     */
    NiFpga_Status FindResource(const char8 * const varName,
                            const bool type,
                            uint32 &varDescriptor);

    /**
     * @brief Finds a unsigned 8-bit variable and returns its descriptor
     * @param[in] varName the LabVIEW variable name
     * @param[in] type the value is not important, it is used to select the uint8 type
     * @param[out] varDescriptor the returned variable descriptor.
     * @return 0 if the method succeeds, error code if it fails.
     */
    NiFpga_Status FindResource(const char8 * const varName,
                            const uint8 type,
                            uint32 &varDescriptor);

    /**
     * @brief Finds a signed 8-bit variable and returns its descriptor
     * @param[in] varName the LabVIEW variable name
     * @param[in] type the value is not important, it is used to select the int8 type
     * @param[out] varDescriptor the returned variable descriptor.
     * @return 0 if the method succeeds, error code if it fails.
     */
    NiFpga_Status FindResource(const char8 * const varName,
                            const int8 type,
                            uint32 &varDescriptor);

    /**
     * @brief Finds a unsigned 16-bit variable and returns its descriptor
     * @param[in] varName the LabVIEW variable name
     * @param[in] type the value is not important, it is used to select the uint16 type
     * @param[out] varDescriptor the returned variable descriptor.
     * @return 0 if the method succeeds, error code if it fails.
     */
    NiFpga_Status FindResource(const char8 * const varName,
                            const uint16 type,
                            uint32 &varDescriptor);

    /**
     * @brief Finds a signed 16-bit variable and returns its descriptor
     * @param[in] varName the LabVIEW variable name
     * @param[in] type the value is not important, it is used to select the int16 type
     * @param[out] varDescriptor the returned variable descriptor.
     * @return 0 if the method succeeds, error code if it fails.
     */
    NiFpga_Status FindResource(const char8 * const varName,
                            const int16 type,
                            uint32 &varDescriptor);

    /**
     * @brief Finds a unsigned 32-bit variable and returns its descriptor
     * @param[in] varName the LabVIEW variable name
     * @param[in] type the value is not important, it is used to select the uint32 type
     * @param[out] varDescriptor the returned variable descriptor.
     * @return 0 if the method succeeds, error code if it fails.
     */
    NiFpga_Status FindResource(const char8 * const varName,
                            const uint32 type,
                            uint32 &varDescriptor);

    /**
     * @brief Finds a signed 32-bit variable and returns its descriptor
     * @param[in] varName the LabVIEW variable name
     * @param[in] type the value is not important, it is used to select the int32 type
     * @param[out] varDescriptor the returned variable descriptor.
     * @return 0 if the method succeeds, error code if it fails.
     */
    NiFpga_Status FindResource(const char8 * const varName,
                            const int32 type,
                            uint32 &varDescriptor);

    /**
     * @brief Finds a unsigned 64-bit variable and returns its descriptor
     * @param[in] varName the LabVIEW variable name
     * @param[in] type the value is not important, it is used to select the uint64 type
     * @param[out] varDescriptor the returned variable descriptor.
     * @return 0 if the method succeeds, error code if it fails.
     */
    NiFpga_Status FindResource(const char8 * const varName,
                            const uint64 type,
                            uint32 &varDescriptor);

    /**
     * @brief Finds a signed 64-bit variable and returns its descriptor
     * @param[in] varName the LabVIEW variable name
     * @param[in] type the value is not important, it is used to select the int64 type
     * @param[out] varDescriptor the returned variable descriptor.
     * @return 0 if the method succeeds, error code if it fails.
     */
    NiFpga_Status FindResource(const char8 * const varName,
                            const int64 type,
                            uint32 &varDescriptor);

    /**
     * @brief Reads a boolean value from a given indicator or control.
     * @param[in] indicator indicator or control from which to read
     * @param[out] value outputs the value that was read
     * @return status=0 if succeeds, status=[error_code] if fails.
     */
    NiFpga_Status NiRead(const uint32 indicator,
                            bool &value) const;

    /**
     * @brief Reads a signed 8-bit integer value from a given indicator or control.
     * @param[in] indicator indicator or control from which to read
     * @param[out] value outputs the value that was read
     * @return result of the call
     */
    NiFpga_Status NiRead(const uint32 indicator,
                            int8 &value) const;

    /**
     * @brief Reads an unsigned 8-bit integer value from a given indicator or control.
     * @param[in] indicator indicator or control from which to read
     * @param[out] value outputs the value that was read
     * @return status=0 if succeeds, status=[error_code] if fails.
     */
    NiFpga_Status NiRead(const uint32 indicator,
                            uint8 &value) const;

    /**
     * @brief Reads a signed 16-bit integer value from a given indicator or control.
     * @param[in] indicator indicator or control from which to read
     * @param[out] value outputs the value that was read
     * @return status=0 if succeeds, status=[error_code] if fails.
     */
    NiFpga_Status NiRead(const uint32 indicator,
                            int16 &value) const;

    /**
     * @brief Reads an unsigned 16-bit integer value from a given indicator or control.
     * @param[in] indicator indicator or control from which to read
     * @param[out] value outputs the value that was read
     * @return status=0 if succeeds, status=[error_code] if fails.
     */
    NiFpga_Status NiRead(const uint32 indicator,
                            uint16 &value) const;

    /**
     * @brief Reads a signed 32-bit integer value from a given indicator or control.
     * @param[in] indicator indicator or control from which to read
     * @param[out] value outputs the value that was read
     * @return status=0 if succeeds, status=[error_code] if fails.
     */
    NiFpga_Status NiRead(const uint32 indicator,
                            int32 &value) const;

    /**
     * @brief Reads an unsigned 32-bit integer value from a given indicator or control.
     * @param[in] indicator indicator or control from which to read
     * @param[out] value outputs the value that was read
     * @return status=0 if succeeds, status=[error_code] if fails.
     */
    NiFpga_Status NiRead(const uint32 indicator,
                            uint32 &value) const;

    /**
     * @brief Reads a signed 64-bit integer value from a given indicator or control.
     * @param[in] indicator indicator or control from which to read
     * @param[out] value outputs the value that was read
     * @return status=0 if succeeds, status=[error_code] if fails.
     */
    NiFpga_Status NiRead(const uint32 indicator,
                            int64 &value) const;

    /**
     * @brief Reads an unsigned 64-bit integer value from a given indicator or control.
     * @param[in] indicator indicator or control from which to read
     * @param[out] value outputs the value that was read
     * @return status=0 if succeeds, status=[error_code] if fails.
     */
    NiFpga_Status NiRead(const uint32 indicator,
                            uint64 &value) const;

    /**
     * @brief Writes a boolean value to a given control or indicator.
     * @param[in] control control or indicator to which to write
     * @param[in] value value to write
     * @return status=0 if succeeds, status=[error_code] if fails.
     */
    NiFpga_Status NiWrite(const uint32 control,
                            const bool value) const;

    /**
     * @brief Writes a signed 8-bit integer value to a given control or indicator.
     * @param[in] control control or indicator to which to write
     * @param[in] value value to write
     * @return status=0 if succeeds, status=[error_code] if fails.
     */
    NiFpga_Status NiWrite(const uint32 control,
                            const int8 value) const;

    /**
     * @brief Writes an unsigned 8-bit integer value to a given control or indicator.
     * @param[in] control control or indicator to which to write
     * @param[in] value value to write
     * @return status=0 if succeeds, status=[error_code] if fails.
     */
    NiFpga_Status NiWrite(const uint32 control,
                            const uint8 value) const;

    /**
     * @brief Writes a signed 16-bit integer value to a given control or indicator.
     * @param[in] control control or indicator to which to write
     * @param[in] value value to write
     * @return status=0 if succeeds, status=[error_code] if fails.
     */
    NiFpga_Status NiWrite(const uint32 control,
                            const int16 value) const;

    /**
     * @brief Writes an unsigned 16-bit integer value to a given control or indicator.
     * @param[in] control control or indicator to which to write
     * @param[in] value value to write
     * @return status=0 if succeeds, status=[error_code] if fails.
     */
    NiFpga_Status NiWrite(const uint32 control,
                            const uint16 value) const;

    /**
     * @brief Writes a signed 32-bit integer value to a given control or indicator.
     * @param[in] control control or indicator to which to write
     * @param[in] value value to write
     * @return status=0 if succeeds, status=[error_code] if fails.
     */
    NiFpga_Status NiWrite(const uint32 control,
                            const int32 value) const;

    /**
     * @brief Writes an unsigned 32-bit integer value to a given control or indicator.
     * @param[in] control control or indicator to which to write
     * @param[in] value value to write
     * @return status=0 if succeeds, status=[error_code] if fails.
     */
    NiFpga_Status NiWrite(uint32 control,
                            uint32 value) const;

    /**
     * @brief Writes a signed 64-bit integer value to a given control or indicator.
     * @param[in] control control or indicator to which to write
     * @param[in] value value to write
     * @return status=0 if succeeds, status=[error_code] if fails.
     */
    NiFpga_Status NiWrite(const uint32 control,
                            const int64 value) const;

    /**
     * @brief Writes an unsigned 64-bit integer value to a given control or indicator.
     * @param[in] control control or indicator to which to write
     * @param[in] value value to write
     * @return status=0 if succeeds, status=[error_code] if fails.
     */
    NiFpga_Status NiWrite(const uint32 control,
                            const uint64 value) const;

    /**
     * @brief Specifies the depth of the host memory part of the DMA FIFO. This method is
     * optional.
     * @param[in] fifo FIFO to configure
     * @param[in] requestedDepth requested number of elements in the host memory part
     *                       of the DMA FIFO
     * @param[out] actualDepth if non-NULL, outputs the actual number of elements in the
     *                    host memory part of the DMA FIFO, which may be more than
     *                    the requested number
     * @return status=0 if succeeds, status=[error_code] if fails.
     */
    NiFpga_Status NiConfigureFifo(const uint32 fifo,
                            const uint32 requestedDepth,
                            uint32 &actualDepth) const;

    /**
     * @brief Starts a FIFO. This method is optional.
     * @param[in] fifo FIFO to start
     * @return status=0 if succeeds, status=[error_code] if fails.
     */
    NiFpga_Status NiStartFifo(const uint32 fifo) const;

    /**
     * @brief Stops a FIFO. This method is optional.
     * @param[in] fifo FIFO to stop
     * @return status=0 if succeeds, status=[error_code] if fails.
     */
    NiFpga_Status NiStopFifo(const uint32 fifo) const;

    /**
     * @brief Reads from a target-to-host FIFO of booleans.
     * @param[in] fifo target-to-host FIFO from which to read
     * @param[out] data outputs the data that was read
     * @param[in] numberOfElements number of elements to read
     * @param[in] timeout timeout in milliseconds, or NiFpga_InfiniteTimeout
     * @param[out] elementsRemaining if non-NULL, outputs the number of elements
     *                          remaining in the host memory part of the DMA FIFO
     * @return status=0 if succeeds, status=[error_code] if fails.
     */
    NiFpga_Status NiReadFifo(const uint32 fifo,
                            bool * const data,
                            const uint32 numberOfElements,
                            const uint32 timeout,
                            uint32 &elementsRemaining) const;

    /**
     * @brief Reads from a target-to-host FIFO of signed 8-bit integers.
     * @param[in] fifo target-to-host FIFO from which to read
     * @param[out] data outputs the data that was read
     * @param[in] numberOfElements number of elements to read
     * @param[in] timeout timeout in milliseconds, or NiFpga_InfiniteTimeout
     * @param[out] elementsRemaining if non-NULL, outputs the number of elements
     *                          remaining in the host memory part of the DMA FIFO
     * @return status=0 if succeeds, status=[error_code] if fails.
     */
    NiFpga_Status NiReadFifo(const uint32 fifo,
                            int8 * const data,
                            const uint32 numberOfElements,
                            const uint32 timeout,
                            uint32 &elementsRemaining) const;

    /**
     * @brief Reads from a target-to-host FIFO of unsigned 8-bit integers.
     * @param[in] fifo target-to-host FIFO from which to read
     * @param[out] data outputs the data that was read
     * @param[in] numberOfElements number of elements to read
     * @param[in] timeout timeout in milliseconds, or NiFpga_InfiniteTimeout
     * @param[out] elementsRemaining if non-NULL, outputs the number of elements
     *                          remaining in the host memory part of the DMA FIFO
     * @return status=0 if succeeds, status=[error_code] if fails.
     */
    NiFpga_Status NiReadFifo(const uint32 fifo,
                            uint8 * const data,
                            const uint32 numberOfElements,
                            const uint32 timeout,
                            uint32 &elementsRemaining) const;

    /**
     * @brief Reads from a target-to-host FIFO of signed 16-bit integers.
     * @param[in] fifo target-to-host FIFO from which to read
     * @param[out] data outputs the data that was read
     * @param[in] numberOfElements number of elements to read
     * @param[in] timeout timeout in milliseconds, or NiFpga_InfiniteTimeout
     * @param[out] elementsRemaining if non-NULL, outputs the number of elements
     *                          remaining in the host memory part of the DMA FIFO
     * @return status=0 if succeeds, status=[error_code] if fails.
     */
    NiFpga_Status NiReadFifo(const uint32 fifo,
                            int16 * const data,
                            const uint32 numberOfElements,
                            const uint32 timeout,
                            uint32 &elementsRemaining) const;

    /**
     * @brief Reads from a target-to-host FIFO of unsigned 16-bit integers.
     * @param[in] fifo target-to-host FIFO from which to read
     * @param[out] data outputs the data that was read
     * @param[in] numberOfElements number of elements to read
     * @param[in] timeout timeout in milliseconds, or NiFpga_InfiniteTimeout
     * @param[out] elementsRemaining if non-NULL, outputs the number of elements
     *                          remaining in the host memory part of the DMA FIFO
     * @return status=0 if succeeds, status=[error_code] if fails.
     */
    NiFpga_Status NiReadFifo(const uint32 fifo,
                            uint16 * const data,
                            const uint32 numberOfElements,
                            const uint32 timeout,
                            uint32 &elementsRemaining) const;

    /**
     * @brief Reads from a target-to-host FIFO of signed 32-bit integers.
     * @param[in] fifo target-to-host FIFO from which to read
     * @param[out] data outputs the data that was read
     * @param[in] numberOfElements number of elements to read
     * @param[in] timeout timeout in milliseconds, or NiFpga_InfiniteTimeout
     * @param[out] elementsRemaining if non-NULL, outputs the number of elements
     *                          remaining in the host memory part of the DMA FIFO
     * @return status=0 if succeeds, status=[error_code] if fails.
     */
    NiFpga_Status NiReadFifo(const uint32 fifo,
                            int32 * const data,
                            const uint32 numberOfElements,
                            const uint32 timeout,
                            uint32 &elementsRemaining) const;

    /**
     * @brief Reads from a target-to-host FIFO of unsigned 32-bit integers.
     * @param[in] fifo target-to-host FIFO from which to read
     * @param[out] data outputs the data that was read
     * @param[in] numberOfElements number of elements to read
     * @param[in] timeout timeout in milliseconds, or NiFpga_InfiniteTimeout
     * @param[out] elementsRemaining if non-NULL, outputs the number of elements
     *                          remaining in the host memory part of the DMA FIFO
     * @return status=0 if succeeds, status=[error_code] if fails.
     */
    NiFpga_Status NiReadFifo(const uint32 fifo,
                            uint32 * const data,
                            const uint32 numberOfElements,
                            const uint32 timeout,
                            uint32 &elementsRemaining) const;

    /**
     * @brief Reads from a target-to-host FIFO of signed 64-bit integers.
     * @param[in] fifo target-to-host FIFO from which to read
     * @param[out] data outputs the data that was read
     * @param[in] numberOfElements number of elements to read
     * @param[in] timeout timeout in milliseconds, or NiFpga_InfiniteTimeout
     * @param[out] elementsRemaining if non-NULL, outputs the number of elements
     *                          remaining in the host memory part of the DMA FIFO
     * @return status=0 if succeeds, status=[error_code] if fails.
     */
    NiFpga_Status NiReadFifo(const uint32 fifo,
                            int64 * const data,
                            const uint32 numberOfElements,
                            const uint32 timeout,
                            uint32 &elementsRemaining) const;

    /**
     * @brief Reads from a target-to-host FIFO of unsigned 64-bit integers.
     * @param[in] fifo target-to-host FIFO from which to read
     * @param[out] data outputs the data that was read
     * @param[in] numberOfElements number of elements to read
     * @param[in] timeout timeout in milliseconds, or NiFpga_InfiniteTimeout
     * @param[out] elementsRemaining if non-NULL, outputs the number of elements
     *                          remaining in the host memory part of the DMA FIFO
     * @return status=0 if succeeds, status=[error_code] if fails.
     */
    NiFpga_Status NiReadFifo(const uint32 fifo,
                            uint64 * const data,
                            const uint32 numberOfElements,
                            const uint32 timeout,
                            uint32 &elementsRemaining) const;

    /**
     * @brief Writes to a host-to-target FIFO of Booleans.
     * @param[in] fifo host-to-target FIFO to which to write
     * @param[in] data data to write
     * @param[in] numberOfElements number of elements to write
     * @param[in] timeout timeout in milliseconds, or NiFpga_InfiniteTimeout
     * @param[out] emptyElementsRemaining if non-NULL, outputs the number of empty
     *                               elements remaining in the host memory part of
     *                               the DMA FIFO
     * @return status=0 if succeeds, status=[error_code] if fails.
     */
    NiFpga_Status NiWriteFifo(const uint32 fifo,
                            const bool * const data,
                            const uint32 numberOfElements,
                            const uint32 timeout,
                            uint32 &emptyElementsRemaining) const;

    /**
     * @brief Writes to a host-to-target FIFO of signed 8-bit integers.
     * @param[in] fifo host-to-target FIFO to which to write
     * @param[in] data data to write
     * @param[in] numberOfElements number of elements to write
     * @param[in] timeout timeout in milliseconds, or NiFpga_InfiniteTimeout
     * @param[out] emptyElementsRemaining if non-NULL, outputs the number of empty
     *                               elements remaining in the host memory part of
     *                               the DMA FIFO
     * @return status=0 if succeeds, status=[error_code] if fails.
     */
    NiFpga_Status NiWriteFifo(const uint32 fifo,
                            const int8 * const data,
                            const uint32 numberOfElements,
                            const uint32 timeout,
                            uint32 &emptyElementsRemaining) const;

    /**
     * @brief Writes to a host-to-target FIFO of unsigned 8-bit integers.
     * @param[in] fifo host-to-target FIFO to which to write
     * @param[in] data data to write
     * @param[in] numberOfElements number of elements to write
     * @param[in] timeout timeout in milliseconds, or NiFpga_InfiniteTimeout
     * @param[out] emptyElementsRemaining if non-NULL, outputs the number of empty
     *                               elements remaining in the host memory part of
     *                               the DMA FIFO
     * @return status=0 if succeeds, status=[error_code] if fails.
     */
    NiFpga_Status NiWriteFifo(const uint32 fifo,
                            const uint8 * const data,
                            const uint32 numberOfElements,
                            const uint32 timeout,
                            uint32 &emptyElementsRemaining) const;

    /**
     * @brief Writes to a host-to-target FIFO of signed 16-bit integers.
     * @param[in] fifo host-to-target FIFO to which to write
     * @param[in] data data to write
     * @param[in] numberOfElements number of elements to write
     * @param[in] timeout timeout in milliseconds, or NiFpga_InfiniteTimeout
     * @param[out] emptyElementsRemaining if non-NULL, outputs the number of empty
     *                               elements remaining in the host memory part of
     *                               the DMA FIFO
     * @return status=0 if succeeds, status=[error_code] if fails.
     */
    NiFpga_Status NiWriteFifo(const uint32 fifo,
                            const int16 * const data,
                            const uint32 numberOfElements,
                            const uint32 timeout,
                            uint32 &emptyElementsRemaining) const;

    /**
     * @brief Writes to a host-to-target FIFO of unsigned 16-bit integers.
     * @param[in] fifo host-to-target FIFO to which to write
     * @param[in] data data to write
     * @param[in] numberOfElements number of elements to write
     * @param[in] timeout timeout in milliseconds, or NiFpga_InfiniteTimeout
     * @param[out] emptyElementsRemaining if non-NULL, outputs the number of empty
     *                               elements remaining in the host memory part of
     *                               the DMA FIFO
     * @return status=0 if succeeds, status=[error_code] if fails.
     */
    NiFpga_Status NiWriteFifo(const uint32 fifo,
                            const uint16 * const data,
                            const uint32 numberOfElements,
                            const uint32 timeout,
                            uint32 &emptyElementsRemaining) const;

    /**
     * @brief Writes to a host-to-target FIFO of signed 32-bit integers.
     * @param[in] fifo host-to-target FIFO to which to write
     * @param[in] data data to write
     * @param[in] numberOfElements number of elements to write
     * @param[in] timeout timeout in milliseconds, or NiFpga_InfiniteTimeout
     * @param[out] emptyElementsRemaining if non-NULL, outputs the number of empty
     *                               elements remaining in the host memory part of
     *                               the DMA FIFO
     * @return status=0 if succeeds, status=[error_code] if fails.
     */
    NiFpga_Status NiWriteFifo(const uint32 fifo,
                            const int32 * const data,
                            const uint32 numberOfElements,
                            const uint32 timeout,
                            uint32 &emptyElementsRemaining) const;

    /**
     * @brief Writes to a host-to-target FIFO of unsigned 32-bit integers.
     * @param[in] fifo host-to-target FIFO to which to write
     * @param[in] data data to write
     * @param[in] numberOfElements number of elements to write
     * @param[in] timeout timeout in milliseconds, or NiFpga_InfiniteTimeout
     * @param[out] emptyElementsRemaining if non-NULL, outputs the number of empty
     *                               elements remaining in the host memory part of
     *                               the DMA FIFO
     * @return status=0 if succeeds, status=[error_code] if fails.
     */
    NiFpga_Status NiWriteFifo(const uint32 fifo,
                            const uint32 * const data,
                            const uint32 numberOfElements,
                            const uint32 timeout,
                            uint32 &emptyElementsRemaining) const;

    /**
     * @brief Writes to a host-to-target FIFO of signed 64-bit integers.
     * @param[in] fifo host-to-target FIFO to which to write
     * @param[in] data data to write
     * @param[in] numberOfElements number of elements to write
     * @param[in] timeout timeout in milliseconds, or NiFpga_InfiniteTimeout
     * @param[out] emptyElementsRemaining if non-NULL, outputs the number of empty
     *                               elements remaining in the host memory part of
     *                               the DMA FIFO
     * @return status=0 if succeeds, status=[error_code] if fails.
     */
    NiFpga_Status NiWriteFifo(const uint32 fifo,
                            const int64 * const data,
                            const uint32 numberOfElements,
                            const uint32 timeout,
                            uint32 &emptyElementsRemaining) const;

    /**
     * @brief Writes to a host-to-target FIFO of unsigned 64-bit integers.
     * @param[in] fifo host-to-target FIFO to which to write
     * @param[in] data data to write
     * @param[in] numberOfElements number of elements to write
     * @param[in] timeout timeout in milliseconds, or NiFpga_InfiniteTimeout
     * @param[out] emptyElementsRemaining if non-NULL, outputs the number of empty
     *                               elements remaining in the host memory part of
     *                               the DMA FIFO
     * @return status=0 if succeeds, status=[error_code] if fails.
     */
    NiFpga_Status NiWriteFifo(const uint32 fifo,
                            const uint64 * const data,
                            const uint32 numberOfElements,
                            const uint32 timeout,
                            uint32 &emptyElementsRemaining) const;

    /**
     * @brief Retrieves the Ni-9157 session.
     * @warning Using the device session outside this class could cause unexpected faults.
     * @return the NI-9157 device session.
     */
    NiFpga_Session GetSession() const;

    /**
     * @brief Starts the Compact-RIO.
	 * @details Calls the NI9157Device::Open method if the device is not opened and then the
	 * NI9157Device::Run method if the former call was successful.
     * @return No errors if the two calls were successful. Use the ErrorsCleared method to
	 * check the returned value.
     */
    ErrorManagement::ErrorType CrioStart();

    /**
     * @brief Stops the Compact-RIO.
	 * @details Calls the NI9157Device::Reset method if the device is running.
     * @return No errors if the call was successful. Use the ErrorsCleared method to check
	 * the returned value.
     */
    ErrorManagement::ErrorType CrioStop();

    /**
     * @brief Writes a parameter to a Compact-RIO FPGA resource.
	 * @details If the parameter type is recognized, the method calls the NI9157Device::FindResource
	 * method and, if successful, calls the NI9157Device::NiFpga_Write<type>.
     * @param[in] varName the name of the FPGA resource to write at.
     * @param[in] value the value to write to the FPGA resource.
     * @param[in] type the type to be used ("bool", "uint8", "uint16", "uint32", "uint64", "int8",
	 * "int16", "int32" or "int64").
     * @return No errors if the call was successful. Use the ErrorsCleared method to check
	 * the returned value.
     */
    ErrorManagement::ErrorType WriteParam(StreamString varName,
                            const uint64 value,
                            StreamString type);

    /**
     * @brief Reads a parameter from a cRIO FPGA resource.
	 * @details If the parameter type is recognized, the method calls the NI9157Device::FindResource
	 * method and, if successful, calls the NI9157Device::NiFpga_Read<type>.
     * @param[in] varName the name of the FPGA resource to read from.
     * @param[in] value the value read from the FPGA resource.
     * @param[in] type the type to be used ("bool", "uint8", "uint16", "uint32", "uint64", "int8",
	 * "int16", "int32" or "int64").
     * @return No errors if the call was successful. Use the ErrorsCleared method to check
	 * the returned value.
     */
    ErrorManagement::ErrorType ReadParam(StreamString varName,
                            uint64 &value,
                            StreamString type);

protected:

    /**
     * The NI-9157 device session.
     */
    NiFpga_Session session;

    /**
     * Holds the last NI-9157 device status code.
     */
    NiFpga_Status status;

    /**
     * Specifies if the NI-9157 device has been opened.
     */
    uint8 isOpened;

    /**
     * Specifies if the NI-9157 device has been started.
     */
    uint8 run;

    /**
     * Specifies if the NI-9157 device has been started.
     */
    uint8 isRunning;

    /**
     * Specifies if the NI-9157 device should be reseted after opening.
     */
    uint8 reset;

    /**
     * Holds the path to the LabVIEW generated header file.
     */
    StreamString niRioGenFile;

    /**
     * Holds the LabVIEW program signature.
     */
    StreamString niRioGenSignature;

    /**
     * Holds the Ni-9157 device name.
     */
    StreamString niRioDeviceName;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* NI9157DEVICE_H_ */
