/**
 * @file ConversionHelper.h
 * @brief Header file for class ConversionHelper
 * @date 20/01/2017
 * @author Andre Neto
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

 * @details This header file contains the declaration of the class ConversionHelper
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef CONVERSIONHELPER_H_
#define CONVERSIONHELPER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "StructuredDataI.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {
class ConversionHelper {
public:
    ConversionHelper(const void * inputMemoryIn, void * outputMemoryIn);

    virtual ~ConversionHelper();

    virtual void Convert() = 0;

    virtual bool LoadGain(StructuredDataI &data) = 0;

    const void* GetInputMemory() const;

    uint32 GetNumberOfElements() const ;

    void SetNumberOfElements(uint32 numberOfElements);

    uint32 GetNumberOfSamples() const;

    void SetNumberOfSamples(uint32 numberOfSamples);

    void* GetOutputMemory() const;


protected:
    uint32 numberOfSamples;
    uint32 numberOfElements;
    const void * inputMemory;
    void * outputMemory;
};
}



/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* CONVERSIONHELPER_H_ */
	
