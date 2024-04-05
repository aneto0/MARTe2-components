/**
 * @file UEIBufferPointer.h
 * @brief Header file for class UEIBufferPointer
 * @date 22/03/2023
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

 * @details This header file contains the declaration of the class UEIBufferPointer
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef UEIBufferPointer_H_
#define UEIBufferPointer_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "Object.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Class designed to hold a pointer to a UEICircularBuffer buffer and retrieve stored channel data as flat data.
 *
 * @details This class serves the simple and only purpose to provide a flat-memory access method to the data stored in a 
 * UEICircularBUffer object and stored as raw data. The UEIBufferPointer is designed to be configured by the UEICircularBuffer
 * and then shiped to the consumer, who will retrieve the data directly from the buffer without the need to know about the 
 * internal buffer memory structure, number of channels or alignment.
 *
 */
    class UEIBufferPointer : public Object{
        public:
        /**
        * @brief Default constructor.
        * @details NOOP.
        */
        UEIBufferPointer();

        /**
        * @brief Default destructor.
        * @details NOOP.
        */
        ~UEIBufferPointer();

        /**
        * @brief Getter for a sample in the virtual array simulated by this class.
        * @details This method allows the user to retrieve a sample in the UEICircularBuffer at the location
        * pointed by the selected sample in the pointer channel. The method returns a void pointer
        * to the location where the sample is stored in a UEICircularBuffer object.
        * @param[in] Idx index of the sample to be retrieved by the call. If the sample index is greater than the maximum length (configured)
        * then the call always returns the pointer to the first sample in the virtual array.
        * @return a void pointer to the location where the selected sample is stored. Developer is responsible to cast such pointer and obtain the 
        * data correctly.
        */
        void* GetSample(uint32 Idx);
        
        /**
        * @brief Configuration method to set the required behavior for this pointer object.
        * @details Setter for the configuration parameters defining the behavior of the pointer and therefore the virtual array.
        * @param[in] offset_ number of bytes from the head pointer location to the first sample for this pointer channel.
        * @param[in] pointerGain_ number of bytes in the buffer memory area from a sample to the next consecutive sample for this pointer channel.
        * @param[in] maxLength_ maximum number of samples to be retrieved in this channels array.
        * @return true if the supplied parameters are correct and yield a correct setup of the pointer, false otherwise.
        */
        bool SetPointerCharacteristics(uint32 offset_, uint32 pointerGain_, uint32 maxLength_);
        
        /**
        * @brief Setter for the head location of the pointer.
        * @details Setter for the configuration parameters defining the behavior of the pointer and therefore the virtual array.
        * @param[in] newHead pointer to the start position of the read memory area on the circular buffer object memory.
        * @return true if the supplied pointer is valid and is correctly configured into the pointer object.
        */
        bool SetHead(uint8* newHead);
        
        /**
        * @brief Checker method for the pointer.
        * @details Checks if the pointer has been already configured and is safe to use.
        * @return true if the object is configured and ready, false otherwise.
        */
        bool CheckPointer();

        protected:
        
        /** 
        * Pointer to the first memory location of the circular buffer memory area where the raw data is stored
        */
        uint8* blockHeadPointer;
        
        /** 
        * Configured offset for the pointer object. Number of bytes from the blockHeadPointer location to the first
        * sample location for this channel.
        */
        uint32 offset;
        
        /** 
        * Configured gain for the pointer object. Number of bytes from one sample of the pointer channel to the next consecutive
        * sample.
        */
        uint32 pointerGain;
        
        /** 
        * Maximum number of samples to be retrieved for this pointer channel.
        */
        uint32 maxLength;
    };
}

#endif /* UEIBufferPointer_H_ */