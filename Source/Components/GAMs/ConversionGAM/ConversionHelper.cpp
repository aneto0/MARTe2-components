/**
 * @file ConversionHelper.cpp
 * @brief Source file for class ConversionHelper
 * @date Jan 20, 2017
 * @author aneto
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
 * the class ConversionHelper (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "ConversionHelper.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {
ConversionHelper::ConversionHelper(const void *  const inputMemoryIn, void * const outputMemoryIn) {
    inputMemory = inputMemoryIn;
    outputMemory = outputMemoryIn;
    numberOfElements = 0u;
    numberOfSamples = 0u;
}

ConversionHelper::~ConversionHelper() {

}

const void* ConversionHelper::GetInputMemory() const {
    return inputMemory;
}

uint32 ConversionHelper::GetNumberOfElements() const {
    return numberOfElements;
}

void ConversionHelper::SetNumberOfElements(const uint32 numberOfElementsIn) {
    numberOfElements = numberOfElementsIn;
}

uint32 ConversionHelper::GetNumberOfSamples() const {
    return numberOfSamples;
}

void ConversionHelper::SetNumberOfSamples(const uint32 numberOfSamplesIn) {
    numberOfSamples = numberOfSamplesIn;
}

void* ConversionHelper::GetOutputMemory() const {
    return outputMemory;
}
}
