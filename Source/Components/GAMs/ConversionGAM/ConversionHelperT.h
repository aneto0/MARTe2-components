/**
 * @file ConversionHelperT.h
 * @brief Header file for class ConversionHelperT
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

 * @details This header file contains the declaration of the class ConversionHelperT
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef CONVERSIONHELPERT_H_
#define CONVERSIONHELPERT_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "StructuredDataI.h"
#include "ConversionHelper.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {
/**
 * @brief Support class for the ConversionGAM. One instance for each input signal is to be allocated.
 */
/*lint -esym(9107, MARTe::ConversionHelperT*) [MISRA C++ Rule 3-1-1]. Justification: Required for template implementation.
 * No code is actually being generated and the header files can be included in multiple unit files.*/
template<typename inputType, typename outputType>
class ConversionHelperT: public ConversionHelper {
public:
    /**
     * @see ConversionHelper::ConversionHelper
     */
    ConversionHelperT(const void * inputMemoryIn, void * outputMemoryIn);

    /**
     * @brief Default destructor. NOOP.
     */
    virtual ~ConversionHelperT();

    /**
     * @see ConversionHelper::Convert.
     * @details Converts for the declared type names. A cast to the declared inputType and outputType is performed and the signals are copied.
     */
    virtual void Convert();

    /**
     * @see ConversionHelper::LoadGain.
     */
    virtual bool LoadGain(StructuredDataI &data);

private:
    /**
     * True if the Gain parameter was defined.
     */
    bool gainDefined;

    /**
     * The gain that is used to scale the input signal.
     */
    outputType gain;
    /*lint -e{1712} This class does not have a default constructor because
     * the inputMemory and the outputMemory must be defined on construction and both remain constant
     * during the object's lifetime*/
};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/
namespace MARTe {
template<typename inputType, typename outputType>
ConversionHelperT<inputType, outputType>::ConversionHelperT(
        const void * const inputMemoryIn, void * const outputMemoryIn) :
        ConversionHelper(inputMemoryIn, outputMemoryIn) {
    gainDefined = false;
    /*lint -e{9117} [MISRA C++ Rule 5-0-4]. Justification: the type of the gain will depend on the outputType.*/
    gain = static_cast<outputType>(0);
}

template<typename inputType, typename outputType>
ConversionHelperT<inputType, outputType>::~ConversionHelperT() {

}

template<typename inputType, typename outputType>
bool ConversionHelperT<inputType, outputType>::LoadGain(StructuredDataI &data) {
    gainDefined = data.Read("Gain", gain);
    return gainDefined;
}

template<typename inputType, typename outputType>
void ConversionHelperT<inputType, outputType>::Convert() {
    uint32 s;
    uint32 n;
    uint32 idx;
    outputType *dest = reinterpret_cast<outputType *>(outputMemory);
    const inputType *src = reinterpret_cast<const inputType *>(inputMemory);
    if ((dest != NULL) && (src != NULL)) {
        for (s = 0u; s < numberOfSamples; s++) {
            for (n = 0u; n < numberOfElements; n++) {
                idx = s * numberOfElements;
                idx += n;
                if (gainDefined) {
                    /*lint -e{734} -e{571} Loss of precision is responsibility of the conversion requested by the user.*/
                    dest[idx] = gain * static_cast<outputType>(src[idx]);
                }
                else {
                    /*lint -e{734} -e{571} Loss of precision is responsibility of the conversion requested by the user.*/
                    dest[idx] = static_cast<outputType>(src[idx]);
                }
            }
        }
    }
}

}
#endif /* CONVERSIONHELPERT_H_ */

