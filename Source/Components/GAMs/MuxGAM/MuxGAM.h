/**
 * @file MuxGAM.h
 * @brief Header file for class MuxGAM
 * @date 28/08/2017
 * @author Llorenc Capella
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

 * @details This header file contains the declaration of the class MuxGAM
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef MUXGAM_H_
#define MUXGAM_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "GAM.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @details
 * if (numberOfOutputs > 1) => numberOfInputs >= 2*numberOfOutputs if (numberOfOutputs = 1) =>  numberOfInputs > 2 * numberOfOutputs
 */
class MuxGAM: public GAM {
//TODO Add the macro DLL_API to the class declaration (i.e. class DLL_API MuxGAM)
public:
    CLASS_REGISTER_DECLARATION()
    MuxGAM();
    virtual ~MuxGAM();
    virtual bool Initialise(StructuredDataI &data);
    virtual bool Setup();
    virtual bool Execute();
private:

    /**
     * Selectors pointer. There one selector for each output
     */
    uint32 **selectors;

    TypeDescriptor typeSelector;

    /**
     * Used to go through the selector array.
     */
    uint32 selectorIndex;

    /**
     * number of selector elements. It cold be one or numberOfElements
     */

    uint32 numberOfSelectorElements;

    /**
     * Input signal pointer. The first inputs are selectors whose type is uint32
     */
    void **inputSignals;

    /**
     * Number of inputs including selectors. numberOfInputs = numberOfInputSignalsG + numberOfInputSelectors
     */
    uint32 numberOfInputs;

    /**
     * Number of input signals of the GAM excluding selectors. numberOfInputSignalsG = numberOfInputs - numberOfOutputs.
     */
    uint32 numberOfInputSignalsG;

    /**
     * Type of signals. All inputs/outputs must have the same type
     */
    TypeDescriptor typeSignals;

    /**
     * Size array. All inputs/outputs must have the same number of elements.
     */
    uint32 numberOfElements;

    /**
     * index to go through the elements array
     */
    uint32 elementIndex;

    /**
     * All inputs/outputs and selectors (which are inputs) must have the same dimension. numberOfDimensions= 1.
     */
    uint32 numberOfDimensions;

    /**
     * All inputs/outputs and selectors must have the same number of samples. numberOfSamples = 1
     */
    uint32 numberOfSamples;

    /**
     * Output signal pointer.
     */
    void **outputSignals;

    /**
     * Number of outputs
     */
    uint32 numberOfOutputs;

    /**
     * The selection value must be smaller than maxSelectorValue (not equal).
     */
    uint32 maxSelectorValue;

    /**
     * Size memory to copy in bytes. It is used when numberOFSelectorElements = 1 to copy a region of memory.
     */
    uint32 sizeToCopy;

    bool IsValidType(TypeDescriptor const &typeRef) const;

    inline bool Copy()const;

    inline bool IsValidSelector(const uint32 value) const;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* MUXGAM_H_ */

