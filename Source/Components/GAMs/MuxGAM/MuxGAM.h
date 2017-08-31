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
 * @brief Implement a multiplexer using as many control signals (called selectors) as outputs.Each
 * output is controlled by a single selector.
 *
 * @details This GAM has two conceptual inputs: \b input \b signals which are the signals routed to
 * the outputs and \b selectors which are the control signals. Notice the difference between
 * inputs and input signals (inputs = input signals + selectors.\n
 *
 * The GAM copies the input signals (indicated by the selectors) to the outputs. Several types of data and single/array elements
 * are supported, however all the input and output signals must have the same type and the same number of elements.
 * The selectors are always uint32 and the number of elements could be or 1 or the same than the input and output signals.
 *
 * When the number of selector elements is 1,for a giver input signal array, all the elements will be routed to the
 * same output, in the other hand if the number of selector elements is equal than the number of input signal elements
 * the routed path (input-output) can be changed for each element of the array.
 *
 * Any number of inputs/outputs are allowed with the following restrictions:
 * <ul>
 * <li>The number of selectors signals must be equal than the number of outputs.</li>
 * <li>The number of output signals <= number of input signals.</li>
 * <li>The minimum number of outputs is 1.</li>
 * <li>The minimum number of input signals is 2. Hence the minimum number of inputs is 3 (1 selector and 2 input signals).</li>
 * <li>The maximum number of inputs is limited by an uin32 (2^32-1 inputs).</li>
 * </ul>
 *\n
 * The type supported by the GAM are:
 * <ul>
 * <li>SignedInteger8Bit</li>
 * <li>SignedInteger16Bit</li>
 * <li>UnsignedInteger16Bit</li>
 * <li>SignedInteger32Bit</li>
 * <li>UnsignedInteger32Bit</li>
 * <li>SignedInteger64Bit</li>
 * <li>Float32Bit</li>
 * <li>Float64Bit</li>
 * </ul>
 *
 * As an example imagine a MuxGAM with 4 inputs (2 input signals, 2 selectors) and 2 output signals. Inputs and outputs
 * are named I<sub>0</sub>, I<sub>1</sub>, S<sub>0</sub>, S<sub>1</sub>,
 * O<sub>0</sub> and O<sub>1</sub>. The S<sub>0</sub> control the O<sub>0</sub> and the S<sub>1</sub> controls the
 * O<sub>1</sub>. With the previous conditions the following functional table could be done:\n
 * <table>
 * <tr>
 * <th>S<sub>0</sub></th><th>S<sub>1</sub></th><th>O<sub>0</sub></th><th>O<sub>1</sub></th>
 * </tr>
 * <tr>
 *  <td>0</td> <td> 0 </td><td>I<sub>0</sub></td><td>I<sub>0</sub></td>
 *  </tr>
 *  <tr>
 *  <td>1</td><td>0</td><td>I<sub>1</sub></td><td>I<sub>0</sub></td>
 *  </tr>
 *  <tr>
 *  <td>0 </td><td> 1</td><td>I<sub>0</sub></td><td>I<sub>1</sub></td>
 *  </tr>
 *  <tr>
 *  <td>1</td> <td>1 </td><td>I<sub>1</sub></td><td>I<sub>1</sub></td>
 *  </tr>
 *  </table>
 *  \n
 * As can be observed the same input can be connected to several outputs.
 *
 * If an invalid selector is sent the GAM::Execute() return an error
 * *The configuration syntax is (names and signal quantity are only given as an example):
 *<pre>
 * +MuxGAM1 = {
 *     Class = MuxGAM
 *     InputSignals = {
 *         Selector0 = {
 *             DataSource = "DDB1"
 *             Type = uint32 //Supported type uint32 (int32 also valid since time cannot be negative)
 *         }
 *     }
 *     OutputSignals = {
 *         OutputSignal1 = {
 *             DataSource = "LCD"
 *             Type = float32
 *         }
 *         OutputSignal2 = {
 *             DataSource = "LCD"
 *             Type = float64
 *         }
 *     }
 * }
 * </pre>
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

