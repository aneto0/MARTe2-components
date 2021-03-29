/**
 * @file NI9157DeviceOperatorT.h
 * @brief Header file for class NI9157DeviceOperatorT.
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
 * NI9157DeviceOperatorT with all of its public, protected and private members.
 * It may also include definitions for inline methods which need to be visible
 * to the compiler.
 */

#ifndef NI9157DEVICEOPERATORT_H_
#define NI9157DEVICEOPERATORT_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "NI9157DeviceOperatorTI.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {

/*lint -esym(9107, MARTe::NI9157DeviceOperatorT*) [MISRA C++ Rule 3-1-1]. Justification: Required for template implementation.
 * No code is actually being generated and the header files can be included in multiple unit files.*/
template<typename T>

/**
 * @brief A class to allow typed read and write operations from/to a NI-9157 device.
 * @details Depending on the template type, the methods NiRead, NiWrite, NiReadFifo, NiWriteFifo, overload the relative
 * methods of the member NI9157Device.
 */
class NI9157DeviceOperatorT: public NI9157DeviceOperatorTI {
public:

    /**
     * @brief Constructor.
     * @post
     *   niDevice.IsValid() = false
     */
    NI9157DeviceOperatorT();

    /**
     * @brief Constructor by reference to NI9157Device.
     * @param[in] niDev a reference to the NI9157Device to be wrapped.
     */
    NI9157DeviceOperatorT(ReferenceT<NI9157Device> niDev);

    /**
     * @brief Destructor.
     */
    virtual ~NI9157DeviceOperatorT();

    /**
     * @see NI9157DeviceOperatorTI::FindResource.
     */
    virtual NiFpga_Status FindResource(const char8 * const varName, uint32 &varDescriptor) const;

    /**
     * @see NI9157DeviceOperatorTI::NiRead.
     * @details Typed read from the NI-9157 device depending on the template type.
     */
    virtual NiFpga_Status NiRead(const uint32 indicator, void * const value) const;

    /**
     * @see NI9157DeviceOperatorTI::NiWrite.
     * @details Typed write to the NI-9157 device depending on the template type.
     */
    virtual NiFpga_Status NiWrite(const uint32 control, void * const value) const;

    /**
     * @see NI9157DeviceOperatorTI::NiReadFifo.
     * @details Typed read from the NI-9157 FIFO depending on the template type.
     */
    virtual NiFpga_Status NiReadFifo(const uint32 fifo, void * const data, const uint32 numberOfElements, const uint32 timeout, uint32 &elementsRemaining) const;

    /**
     * @see NI9157DeviceOperatorTI::NiWriteFifo.
     * @details Typed write to the NI-9157 FIFO depending on the template type.
     */
    virtual NiFpga_Status NiWriteFifo(const uint32 fifo, const void * const data, const uint32 numberOfElements, const uint32 timeout, uint32 &emptyElementsRemaining) const;

    /**
     * @see NI9157DeviceOperatorTI::GetNI9157Device.
     */
    virtual ReferenceT<NI9157Device> GetNI9157Device() const;

    /**
     * @see NI9157DeviceOperatorTI::GetByteSize.
     */
    virtual uint8 GetByteSize() const;

    /**
     * @see NI9157DeviceOperatorTI::Compare.
     */
    virtual int32 Compare(const uint8* const a, const uint8* const b) const;

    /**
     * @see NI9157DeviceOperatorTI::Copy.
     */
    virtual void Copy(uint8 * const dest, const uint8 * const source) const;

    /**
     * @see NI9157DeviceOperatorTI::IsValid.
     */
    virtual bool IsValid() const;

private:

    /**
     * The reference to the wrapped NI9157Device.
     */
    ReferenceT<NI9157Device> niDevice;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/
namespace MARTe {

template<typename T>
NI9157DeviceOperatorT<T>::NI9157DeviceOperatorT() :
        NI9157DeviceOperatorTI() {
}

template<typename T>
NI9157DeviceOperatorT<T>::NI9157DeviceOperatorT(ReferenceT<NI9157Device> niDev) :
        NI9157DeviceOperatorTI() {
    niDevice = niDev;
}

template<typename T>
NI9157DeviceOperatorT<T>::~NI9157DeviceOperatorT() {
}

template<typename T>
bool NI9157DeviceOperatorT<T>::IsValid() const {
    return niDevice.IsValid();
}

template<typename T>
NiFpga_Status NI9157DeviceOperatorT<T>::FindResource(const char8 * const varName, uint32 &varDescriptor) const {
    T temp = static_cast<T>(0);
    return niDevice->FindResource(varName, temp, varDescriptor);
}

template<typename T>
NiFpga_Status NI9157DeviceOperatorT<T>::NiRead(const uint32 indicator, void * const value) const {
    return niDevice->NiRead(indicator, *reinterpret_cast<T*>(value));
}

template<typename T>
NiFpga_Status NI9157DeviceOperatorT<T>::NiWrite(const uint32 control, void * const value) const {
    return niDevice->NiWrite(control, *reinterpret_cast<T*>(value));
}

template<typename T>
NiFpga_Status NI9157DeviceOperatorT<T>::NiReadFifo(const uint32 fifo, void * const data, const uint32 numberOfElements, const uint32 timeout, uint32 &elementsRemaining) const {
    return niDevice->NiReadFifo(fifo, reinterpret_cast<T*>(data), numberOfElements, timeout, elementsRemaining);
}

template<typename T>
NiFpga_Status NI9157DeviceOperatorT<T>::NiWriteFifo(const uint32 fifo, const void * const data, const uint32 numberOfElements, const uint32 timeout, uint32 &emptyElementsRemaining) const {
    return niDevice->NiWriteFifo(fifo, reinterpret_cast<const T *>(data), numberOfElements, timeout, emptyElementsRemaining);
}

template<typename T>
ReferenceT<NI9157Device> NI9157DeviceOperatorT<T>::GetNI9157Device() const {
    return niDevice;
}

template<typename T>
uint8 NI9157DeviceOperatorT<T>::GetByteSize() const {
    return static_cast<uint8>(sizeof(T));
}

template<typename T>
int32 NI9157DeviceOperatorT<T>::Compare(const uint8 * const a, const uint8 * const b) const {
    int32 ret = 0;
    /*lint -e{927} -e{826} enable pointer to pointer cast for the templated types*/
    T x = *reinterpret_cast<const T*>(a);
    /*lint -e{927} -e{826} enable pointer to pointer cast for the templated types*/
    T y = *reinterpret_cast<const T*>(b);
    /*lint -e{9111} -e{503} boolean used as integer here*/
    if (x > y) {
        ret = 1;
    }
    /*lint -e{9111} -e{503} boolean used as integer here*/
    if (x < y) {
        ret = -1;
    }
    return ret;

}

template<typename T>
void NI9157DeviceOperatorT<T>::Copy(uint8 * const dest, const uint8 * const source) const {
    /*lint -e{927} -e{826} enable pointer to pointer cast for the templated types*/
    *reinterpret_cast<T*>(dest) = *reinterpret_cast<const T*>(source);
}

}

#endif /* NI9157DEVICEOPERATORT_H_ */
