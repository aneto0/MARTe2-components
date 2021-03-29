/**
 * @file NI9157MemoryOperationsHelper.cpp
 * @brief Source file for class NI9157MemoryOperationsHelper.
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
 * @details This source file contains the definition of all the methods for
 * the class NI9157MemoryOperationsHelper (public, protected, and private). Be
 * aware that some methods, such as those inline could be defined on the header
 * file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "NI9157MemoryOperationsHelper.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
/*lint -e{9141} -e{714} known global declaration*/
void NI9157MemoryOperationsHelper::InterleavedToFlat(MARTe::uint8 * const originSource, MARTe::uint8 * const originDest, const MARTe::uint32 beginIndex, const MARTe::uint32 * const packetMemberSize, const MARTe::uint32 packetByteSize,
                                                     const MARTe::uint32 numberOfPacketMembers, const MARTe::uint32 numberOfSamples) {
    /*lint -e{714} symbol referenced*/
    MARTe::MemoryOperationsHelper::InterleavedToFlat(originSource, originDest, beginIndex, packetMemberSize, packetByteSize, numberOfPacketMembers, numberOfSamples);
}

/*lint -e{9141} -e{714} known global declaration*/
void NI9157MemoryOperationsHelper::FlatToInterleaved(MARTe::uint8 * const originSource, MARTe::uint8 * const originDest, const MARTe::uint32 beginIndex, const MARTe::uint32 * const packetMemberSize, const MARTe::uint32 packetByteSize,
                                                     const MARTe::uint32 numberOfPacketMembers, const MARTe::uint32 numberOfSamples) {
    /*lint -e{714} symbol referenced*/
    MARTe::MemoryOperationsHelper::FlatToInterleaved(originSource, originDest, beginIndex, packetMemberSize, packetByteSize, numberOfPacketMembers, numberOfSamples);
}
