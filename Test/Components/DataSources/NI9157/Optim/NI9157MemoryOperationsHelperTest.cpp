/**
 * @file NI9157MemoryOperationsHelperTest.cpp
 * @brief Source file for class NI9157MemoryOperationsHelperTest.
 * @date 26/03/2021
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
 * the class NI9157MemoryOperationsHelperTest (public, protected, and private).
 * Be aware that some methods, such as those inline could be defined on the
 * header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "NI9157MemoryOperationsHelperTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
NI9157MemoryOperationsHelperTest::NI9157MemoryOperationsHelperTest() {
}

NI9157MemoryOperationsHelperTest::~NI9157MemoryOperationsHelperTest() {
}

bool NI9157MemoryOperationsHelperTest::TestInterleavedToFlat() {

    const uint32 memSize = 150;
    const uint32 nSamples = 10;
    uint8 mem[memSize];
    uint8 dest[memSize];
    uint8 *ptr = &mem[0];
    uint32 cnt = 0u;
    for (uint32 i = 0u; i < nSamples; i++) {
        *ptr = cnt;
        ptr++;
        cnt++;
        *(uint16*) ptr = cnt;
        ptr += 2;
        cnt++;
        *(uint32*) ptr = cnt;
        ptr += 4;
        cnt++;
        *(uint64*) ptr = cnt;
        ptr += 8;
        cnt++;
    }
    uint32 packetInputChunkSize[] = { 1, 2, 4, 8 };
    uint32 inputByteSize = 15;
    uint32 numberOfInputPacketChunks = 4;
    uint32 numberOfInputSamples = nSamples;

    NI9157MemoryOperationsHelper::InterleavedToFlat(&mem[0], &dest[0], 0, &packetInputChunkSize[0], inputByteSize, numberOfInputPacketChunks, numberOfInputSamples);

    bool ret = true;
    ptr = &dest[0];
    for (uint32 i = 0u; (i < nSamples) && (ret); i++) {
        ret = (ptr[i] == 4 * i);
    }
    uint16 *ptr16 = (uint16*) &dest[nSamples];
    for (uint32 i = 0u; (i < nSamples) && (ret); i++) {
        ret = (ptr16[i] == 4 * i + 1);
    }
    uint32 *ptr32 = (uint32*) &dest[3 * nSamples];
    for (uint32 i = 0u; (i < nSamples) && (ret); i++) {
        ret = (ptr32[i] == 4 * i + 2);
    }
    uint64 *ptr64 = (uint64*) &dest[7 * nSamples];
    for (uint32 i = 0u; (i < nSamples) && (ret); i++) {
        ret = (ptr64[i] == 4 * i + 3);
    }

    return ret;
}

bool NI9157MemoryOperationsHelperTest::TestFlatToInterleaved() {

    const uint32 memSize = 150;
    const uint32 nSamples = 10;
    uint8 mem[memSize];
    uint8 dest[memSize];
    uint8 *ptr = &mem[0];
    uint32 cnt = 0u;
    for (uint32 i = 0u; (i < nSamples); i++) {
        ptr[i] = cnt;
        cnt++;
    }
    uint16 *ptr16 = (uint16*) &mem[nSamples];
    for (uint32 i = 0u; (i < nSamples); i++) {
        ptr16[i] = cnt;
        cnt++;
    }
    uint32 *ptr32 = (uint32*) &mem[3 * nSamples];
    for (uint32 i = 0u; (i < nSamples); i++) {
        ptr32[i] = cnt;
        cnt++;
    }
    uint64 *ptr64 = (uint64*) &mem[7 * nSamples];
    for (uint32 i = 0u; (i < nSamples); i++) {
        ptr64[i] = cnt;
        cnt++;
    }
    uint32 packetOutputChunkSize[] = { 1, 2, 4, 8 };
    uint32 outputByteSize = 15;
    uint32 numberOfOutputPacketChunks = 4;
    uint32 numberOfOutputSamples = nSamples;

    NI9157MemoryOperationsHelper::FlatToInterleaved(&mem[0], &dest[0], 0, &packetOutputChunkSize[0], outputByteSize, numberOfOutputPacketChunks, numberOfOutputSamples);

    bool ret = true;
    ptr = &dest[0];
    for (uint32 i = 0u; (i < nSamples) && (ret); i++) {
        ret&=(*ptr==(i));
        ptr++;
        ret&=(*ptr==(10+i));
        ptr += 2;
        ret&=(*ptr==(20+i));
        ptr += 4;
        ret&=(*ptr==(30+i));
        ptr += 8;
    }

    return ret;
}
