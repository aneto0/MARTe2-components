/**
 * @file UEIBufferPointer.cpp
 * @brief Source file for class UEIBufferPointer
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

 * @details This source file contains the definition of all the methods for
 * the class UEIBufferPointer (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "UEIBufferPointer.h"


/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

UEIBufferPointer::UEIBufferPointer() : Object(){
    blockHeadPointer = NULL_PTR(uint8*);
    offset = 0;
    pointerGain = 0;
    maxLength = 0;
}

UEIBufferPointer::~UEIBufferPointer(){
    if (blockHeadPointer != NULL_PTR(uint8*)){
        blockHeadPointer = NULL_PTR(uint8*);
    }
}

bool UEIBufferPointer::SetPointerCharacteristics(uint32 offset_, uint32 pointerGain_, uint32 maxLength_){
    offset = offset_;
    pointerGain = pointerGain_;
    maxLength = maxLength_;
    return true;
}

bool UEIBufferPointer::SetHead (uint8* newHead){
    bool ok = (newHead != NULL_PTR(uint8*));
    if (ok){
        blockHeadPointer = newHead;
    }
    return ok;
}

void* UEIBufferPointer::GetSample(uint32 Idx){
    if (Idx < maxLength){
        return reinterpret_cast<void*>(blockHeadPointer+(offset+Idx*pointerGain));
    }else{
        return reinterpret_cast<void*>(blockHeadPointer+offset);
    }
}

bool UEIBufferPointer::CheckPointer(){
    bool ok = true;
    ok &= (blockHeadPointer != NULL_PTR(uint8*));
    ok &= (maxLength > 0);
    ok &= (pointerGain > 0);
    return ok;
}
}