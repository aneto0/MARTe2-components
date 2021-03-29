/**
 * @file NI9157MemoryOperationsHelper.h
 * @brief Header file for class NI9157MemoryOperationsHelper.
 * @date 11/02/2021
 * @author Andre Neto
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
 * NI9157MemoryOperationsHelper with all of its public, protected and private
 * members. It may also include definitions for inline methods which need to be
 * visible to the compiler.
 */

#ifndef NI9157MEMORYOPERATIONSHELPER_H_
#define NI9157MEMORYOPERATIONSHELPER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "MemoryOperationsHelper.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
/**
 * @brief Proxy class to MemoryOperationsHelper that can be compiled and optimised separately.
 */
/*lint -estring(19, "*NI9157MemoryOperationsHelper*") -estring(757,"*NI9157MemoryOperationsHelper*") -estring(526, "*NI9157MemoryOperationsHelper*") -estring(714, "*NI9157MemoryOperationsHelper*") functions defined, used and required for optimisation.*/
namespace NI9157MemoryOperationsHelper {

    /**
     * @see MemoryOperationsHelper::InterleavedToFlat
     */
    void InterleavedToFlat(MARTe::uint8 * const originSource, MARTe::uint8 * const originDest, const MARTe::uint32 beginIndex, const MARTe::uint32 * const packetMemberSize, const MARTe::uint32 packetByteSize,
                           const MARTe::uint32 numberOfPacketMembers, const MARTe::uint32 numberOfSamples);

    /**
     * @see MemoryOperationsHelper::FlatToInterleaved
     */
    void FlatToInterleaved(MARTe::uint8 * const originSource, MARTe::uint8 * const originDest, const MARTe::uint32 beginIndex, const MARTe::uint32 * const packetMemberSize, const MARTe::uint32 packetByteSize,
                           const MARTe::uint32 numberOfPacketMembers, const MARTe::uint32 numberOfSamples);

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* NI9157MEMORYOPERATIONSHELPER_H_ */
