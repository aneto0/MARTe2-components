/**
 * @file NI9157DeviceOperatorTI.h
 * @brief Header file for class NI9157DeviceOperatorTI.
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
 * @details This header file contains the declaration of the class
 * NI9157DeviceOperatorTI with all of its public, protected and private
 * members. It may also include definitions for inline methods which need to
 * be visible to the compiler.
 */

#ifndef NI9157DEVICEOPERATORTI_H_
#define NI9157DEVICEOPERATORTI_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
#include <NiFpga.h>

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "NI9157Device.h"
#include "ReferenceT.h"
#include "TypeDescriptor.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {

/**
 * @brief Interface of NI9157DeviceOperatorT.
 * @brief This interface allows to the NI9157DeviceOperatorDatabase functions
 * to return a pointer to the specific NI9157DeviceOperatorT.
 */
class NI9157DeviceOperatorTI {
public:

    /**
     * @brief Constructor.
     */
    NI9157DeviceOperatorTI();

    /**
     * @brief Destructor.
     */
    virtual ~NI9157DeviceOperatorTI();

    /**
     * @brief Find the resource identified by the \a varName.
     * @param[in] varName the name of the resource to find.
     * @param[out] varDescriptor the returned variable descriptor.
     * @return The NiFpga_Status.
     */
    virtual NiFpga_Status FindResource(const char8 * const varName, uint32 &varDescriptor) const = 0;

    /**
     * @brief Reads from NI-9157 device.
     * @param[in] indicator indicator or control from which to read.
     * @param[out] value outputs the value that was read.
     * @return The NiFpga_Status.
     * @pre
     *   IsValid() == true
     */
    virtual NiFpga_Status NiRead(const uint32 indicator, void * const value) const =0;

    /**
     * brief Writes to NI-9157 device.
     * @param[in] control control or indicator to which to write.
     * @param[in] value value to write.
     * @return status=0 if succeeds, status=[error_code] if fails.
     * @pre
     *   IsValid() == true
     */
    virtual NiFpga_Status NiWrite(const uint32 control, void * const value) const =0;

    /**
     * @brief Reads from NI-9157 FIFO.
     * @param[in] fifo target-to-host FIFO from which to read.
     * @param[out] data outputs the data that was read.
     * @param[in] numberOfElements number of elements to read.
     * @param[in] timeout timeout in milliseconds, or \a NiFpga_InfiniteTimeout.
     * @param[out] elementsRemaining if non-NULL, outputs the number of elements
     *                          remaining in the host memory part of the DMA FIFO.
     * @return status=0 if succeeds, status=[error_code] if fails.
     * @pre
     *   IsValid() == true
     */
    virtual NiFpga_Status NiReadFifo(const uint32 fifo, void * const data, const uint32 numberOfElements, const uint32 timeout, uint32 &elementsRemaining) const =0;

    /**
     * @brief Writes on NI-9157 FIFO.
     * @param[in] fifo host-to-target FIFO to which to write.
     * @param[in] data data to write.
     * @param[in] numberOfElements number of elements to write.
     * @param[in] timeout timeout in milliseconds, or \a NiFpga_InfiniteTimeout.
     * @param[out] emptyElementsRemaining if non-NULL, outputs the number of empty
     *                               elements remaining in the host memory part of
     *                               the DMA FIFO.
     * @return status=0 if succeeds, status=[error_code] if fails.
     * @pre
     *   IsValid() == true
     */
    virtual NiFpga_Status NiWriteFifo(const uint32 fifo, const void* const data, const uint32 numberOfElements, const uint32 timeout, uint32 &emptyElementsRemaining) const =0;

    /**
     * @brief Retrieves the reference to the NI9157Device wrapped by this object.
     * @return The reference to the NI9157Device wrapped by this object.
     */
    virtual ReferenceT<NI9157Device> GetNI9157Device() const =0;

    /**
     * @brief The byte size of the type supported by this operator.
     * @return The byte size of the type supported by this operator
     */
    virtual uint8 GetByteSize() const=0;

    /**
     * @brief Compares the values of two numbers depending by the type.
     * @param[in] a is a pointer to the first number.
     * @param[in] b is a pointer to the second number.
     * @return 0 if the values are equal, 1 if a>b, -1 if a<b.
     * @pre
     *   The pointers \a and \b must be valid and point to memories equal to \a sizeof(type).
     */
    virtual int32 Compare(const uint8* const a, const uint8* const b) const =0;

    /**
     * @brief Copies the value of \a source inside \a dest.
     * @param[in, out] dest is the pointer to the output memory.
     * @param[in] source is the pointer to the input memory.
     * @pre
     *   The pointers \a dest and \a source must be valid and point to memories equal to \a sizeof(type).
     */
    virtual void Copy(uint8* const dest, const uint8* const source) const =0;

    /**
     * @brief Returns true if the reference to the wrapped NI9157Device is valid.
     * @return true if the reference to the wrapped NI9157Device is valid, false otherwise.
     */
    virtual bool IsValid() const =0;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* NI9157DEVICEOPERATORTI_H_ */
