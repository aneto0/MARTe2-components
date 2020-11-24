/**
 * @file SigblockSupport.h
 * @brief Header file for class SigblockSupport
 * @date 02/02/2017
 * @author Ivan Herrero
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

 * @details This header file contains the declaration of the class SigblockSupport
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SIGBLOCKSUPPORT_H_
#define SIGBLOCKSUPPORT_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

#include <cstddef>
#include <cstdio>
#include <cstring>
#include <sstream>

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "CompilerTypes.h"
#include "Sigblock.h"
#include "StreamString.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

SDA::Sigblock* MallocSigblock(std::size_t size);

SDA::Sigblock::Metadata* MallocSigblockMetadata(std::size_t size);

void FreeSigblock(SDA::Sigblock*& sb);

void FreeSigblockMetadata(SDA::Sigblock::Metadata*& sbmd);

template<typename SignalType>
void InitSigblock(SDA::Sigblock* sigblock,
                  const unsigned int numberOfSignals,
                  const SignalType seedValue);

template<typename SignalType>
void GenerateMetadataForSigblock(SDA::Signal::Metadata sbmd[],
                                 const unsigned int numberOfSignals);

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

inline SDA::Sigblock* MallocSigblock(std::size_t size) {
    char* mem = new char[size];
    std::memset(mem, '\0', size);
    return reinterpret_cast<SDA::Sigblock*>(mem);
}

inline SDA::Sigblock::Metadata* MallocSigblockMetadata(unsigned int count) {
    size_t size = SDA::Sigblock::Metadata::SizeOf(count);
    char* mem = new char[size];
    std::memset(mem, '\0', size);
    return reinterpret_cast<SDA::Sigblock::Metadata*>(mem);
}

inline void FreeSigblock(SDA::Sigblock*& sb) {
    char* mem = reinterpret_cast<char*>(sb);
    delete[] mem;
    sb = NULL;
}

inline void FreeSigblockMetadata(SDA::Sigblock::Metadata*& sbmd) {
    char* mem = reinterpret_cast<char*>(sbmd);
    delete[] mem;
    sbmd = NULL;
}

template<typename SignalType>
void InitSigblock(SDA::Sigblock* sigblock,
                  const unsigned int numberOfSignals,
                  const SignalType seedValue) {
    char* rawPointer = reinterpret_cast<char*>(sigblock);
    SignalType generatedValue = seedValue;
    unsigned int index = 0;
    while (index < numberOfSignals) {
        SignalType* typedPointer = reinterpret_cast<SignalType*>(rawPointer);
        *typedPointer = generatedValue;
        rawPointer += sizeof(SignalType);
        generatedValue++;
        index++;
    }
}

template<typename SignalType>
void GenerateMetadataForSigblock(SDA::Signal::Metadata sbmd[],
                                 const unsigned int numberOfSignals) {
    for (unsigned int i = 0u; (i < numberOfSignals); i++) {
        MARTe::StreamString name;
        name.Printf("Signal %u", i);
        std::strncpy(sbmd[i].name, name.Buffer(), SDA::Signal::Metadata::NAME_MAX_LEN - 1);
        sbmd[i].size = sizeof(SignalType);
    }
}

#endif /* SIGBLOCKSUPPORT_H_ */
