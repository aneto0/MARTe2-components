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
/**
 * @brief Support class for the ConversionGAM. One instance for each input signal is to be allocated.
 * @details This class is responsible for converting all the input signal elements/samples to the corresponding
 * output signal elements/samples.
 */
class ConversionHelper {
public:
    /**
     * @brief Sets the memory address where the signal is to be read from and written to.
     * @param[in] inputMemoryIn memory address where the signal is to be read from.
     * @param[in] outputMemoryIn  memory address where the signal is to be written to.
     */
    ConversionHelper(const void * inputMemoryIn, void * outputMemoryIn);

    /**
     * @brief Destructor. NOOP.
     */
    virtual ~ConversionHelper();

    /**
     * @brief To be specialised by ConversionHelperT, for all the supported data types.
     * @details Converts all the signal elements and samples from the input type to the output type (set in the ConversionHelperT template).
     */
    virtual void Convert() = 0;

    /**
     * @brief Reads the Gain parameter which is used to scale the input.
     * @param data where to read the Gain parameter.
     * @return true if the Gain was specified.
     */
    virtual bool LoadGain(StructuredDataI &data) = 0;

    /**
     * @brief Gets a pointer to input signal memory.
     * @return a pointer to input signal memory.
     */
    const void* GetInputMemory() const;

    /**
     * @brief Gets the number of elements to be converted.
     * @return the number of elements to be converted.
     */
    uint32 GetNumberOfElements() const;

    /**
     * @brief Sets the number of samples to be converted.
     * @param[in] numberOfElementsIn the number of samples to be converted.
     */
    void SetNumberOfElements(const uint32 numberOfElementsIn);

    /**
     * @brief Gets the number of samples to be converted.
     * @return the number of samples to be converted.
     */
    uint32 GetNumberOfSamples() const;

    /**
     * @brief Sets the number of samples to be converted.
     * @param[in] numberOfSamplesIn the number of samples to be converted.
     */
    void SetNumberOfSamples(const uint32 numberOfSamplesIn);

    /**
     * @brief Gets a pointer to output signal memory.
     * @return a pointer to output signal memory.
     */
    void* GetOutputMemory();

protected:
    /**
     * The number of samples to be converted.
     */
    uint32 numberOfSamples;

    /**
     * The number of elements to be converted.
     */
    uint32 numberOfElements;

    /**
     * The signal input memory.
     */
    const void * inputMemory;

    /**
     * The signal output memory.
     */
    void * outputMemory;

    /*lint -e{1712} This class does not have a default constructor because
     * the inputMemory and the outputMemory must be defined on construction and both remain constant
     * during the object's lifetime*/
};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* CONVERSIONHELPER_H_ */

