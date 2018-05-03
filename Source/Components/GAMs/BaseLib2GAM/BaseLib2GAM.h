/**
 * @file BaseLib2GAM.h
 * @brief Header file for class BaseLib2GAM
 * @date 2/12/2016
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

 * @details This header file contains the declaration of the class BaseLib2GAM
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef BASELIB2GAM_H_
#define BASELIB2GAM_H_

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
 * @brief Encapsulate and execute GAMs from BaseLib2 in MARTe2.
 * @details This GAM instantiates a BaseLib2 GAM. It plugs into its inputs the input signals from MARTe2
 * and plugs into its outputs the output signals of MARTe2. The names and sizes of the signals shall match.
 * The configuration syntax is (names are only given as an example):
 * <pre>
 * +BL2SynchGAM = {
 *     Class = BaseLib2GAM
 *     GAMName = TypeConvert1 //This name must be the same of the one specified in the BaseLib2Config
 *     //The BaseLib2Config must include the GAM configuration as specified in a BaseLib2 application, including the +
 *     //The name of the GAM shall match the GAMName above
 *     BaseLib2Config = "
 *         +TypeConvert1 = {
 *             Class = \"TypeConvertGAM\"
 *             InputSignals = {
 *                 Signal1 = {
 *                     SignalName = \"Signal1[3]\"
 *                     SignalType = float
 *                 }
 *             }
 *             OutputSignals = {
 *                 Signal2 = {
 *                     SignalName = \"Signal2[3]\"
 *                     SignalType = int32
 *                 }
 *             }
 *         }
 *     }"
 *     GAMFunctionNumber = 0x00010000 //The BaseLib2 Execute(GAM_FunctionNumbers).
 *     InputSignals = {
 *         Signal1 = {//The name and dimension of the signals shall match the name and dimension of the signals expected by the BaseLib2 GAM
 *             DataSource = DDB1
 *             Type = float32
 *             NumberOfDimensions = 1
 *             NumberOfElements = 3
 *             Default = {3.5 1.5 2.5}
 *         }"
 *     }"
 *     OutputSignals = {
 *         Signal2 = {
 *             DataSource = DDB1
 *             Type = int32
 *             NumberOfDimensions = 1
 *             NumberOfElements = 3"
 *         }
 *     }
 * }
 *</pre>
 */
class BaseLib2GAM: public GAM {
public:
    CLASS_REGISTER_DECLARATION()
    /**
     * @brief Default constructor. NOOP.
     */
    BaseLib2GAM();

    /**
     * @brief Destructor. Unregisters the GAM from the BaseLib2::GAMAdapter.
     */
    virtual ~BaseLib2GAM();

    /**
     * @brief Retrieves the GAM main parameters and adds the GAM to the BaseLib2::GAMAdapter.
     * @param[in] data configuration in the form described in the class description.
     * The parameters GAMName, BaseLib2Config and GAMFunctionNumber are compulsory.
     * @return true iff all the parameters set and if the GAM can be successfully added to the BaseLib2::GAMAdapter,
     * which in turn means that the BaseLib2Config could be successfully parsed.
     */
    virtual bool Initialise(StructuredDataI & data);

    /**
     * @brief Registers and matches the MARTe2 input/output signals in the BaseLib2 input/output signals.
     * @return true iff all the signals can be successfully matched (size and dimension).
     * @pre
     *     Initialise
     */
    virtual bool Setup();

    /**
     * @brief Calls the BaseLib2 GAM Execute with the GAMFunctionNumber defined in the Execute method.
     * @details The MARTe2 input signals are copied into the BaseLib2 GAM input, the BaseLib2 GAM is executed
     * and finally the BaseLib2 GAM output signals are copied into the MARTe2 output signals.
     * @return the return value of the BaseLib2 GAM Execute method call.
     * @pre
     *     Setup
     */
    virtual bool Execute();

    /**
     * @brief Translates the MARTe2 GAM signal type names to the BaseLib2 GAM signal types.
     * @param[in] signalType the MARTe2 GAM signal type.
     * @param[out] signalTypeName the BaseLib2 GAM signal type name.
     * @return true if the name can be successfully translated.
     */
    bool TranslateSignalName(TypeDescriptor signalType,
                             StreamString &signalTypeName) const ;

private:

    /**
     * The GAM index returned by the GAMAdapter
     */
    uint32 gamIdx;

    /**
     * @brief Memory of the BaseLib2 GAM input signals.
     */
    void *inputToGAM;

    /**
     * @brief Memory size of the BaseLib2 GAM input signals.
     */
    uint32 inputToGAMByteSize;

    /**
     * @brief Memory of the BaseLib2 GAM output signals.
     */
    void *outputFromGAM;

    /**
     * @brief Memory size of the BaseLib2 GAM output signals.
     */
    uint32 outputFromGAMByteSize;

    /**
     * The BaseLib2 Execute(GAM_FunctionNumbers).
     */
    uint32 gamFunctionNumber;
};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* BASELIB2GAM_H_ */

