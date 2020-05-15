/**
 * @file MathExpressionGAM.cpp
 * @brief Header file for class MathExpressionGAM
 * @date 12/06/2020
 * @author RFX
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

 * @details This header file contains the declaration of the class MathExpressionGAM
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef MATHEXPRESSIONGAM_H_
#define MATHEXPRESSIONGAM_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

#include <cstdio>   // Debug

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "GAM.h"
#include "MathExpressionParser.h"
#include "RuntimeEvaluator.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {
/**
 * @brief GAM to evaluate mathematical expressions at runtime using
 *        MARTe2 internal evaluator engine RuntimeEvaluator 
 */
class MathExpressionGAM: public GAM {
    
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor. NOOP.
     */
    MathExpressionGAM();

    /**
     * @brief Destructor. NOOP.
     */
    virtual ~MathExpressionGAM();
    
    virtual bool Initialise(StructuredDataI &data);
    
    /**
     * @brief Verifies correctness of the GAM configuration.
     * @details Checks that the number of input signals is equal to the number of output signals and that,
     * for each signal, the same type is used.
     * @return is the pre-conditions are met.
     * @pre
     *   SetConfiguredDatabase() &&
     *   GetNumberOfInputSignals() == GetNumberOfOutputSignals() &&
     *   for each signal i: GetSignalByteSize(InputSignals, i) * GetSignalNumberOfSamples(InputSignals, i) == GetSignalByteSize(OutputSignals, i) * GetSignalNumberOfSamples(OutputSignals, i)
     */
    virtual bool Setup();

    /**
     * @brief Copies the input signals memory to the output signal memory.
     * @return true if all the signals memory can be successfully copied.
     */
    virtual bool Execute();

private:
    
    void RefreshSignals(const SignalDirection direction);
    
    template <typename T>
    void SignalMemCopy(const SignalDirection direction, const uint32 signalIdx);
    
    struct SignalStruct {
        
        StreamString    name;
        TypeDescriptor  type;
        uint32          numberOfElements;
        
    };
    
    SignalStruct*  inputSignals;
    SignalStruct* outputSignals;
    SignalStruct*    parameters;
    
    uint32 numberOfParameters;
    float64* parameterArray;
    
    StreamString expr;
    
    StreamString errStr;
    
    MathExpressionParser* mathParser;
    RuntimeEvaluator*     evaluator;
    
};

} /* MARTe */

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* MATHEXPRESSIONGAM_H_ */

