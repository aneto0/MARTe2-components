/**
 * @file TypeCastGAM.h
 * @brief Header file for class TypeCastGAM
 * @date 16/01/2017
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

 * @details This header file contains the declaration of the class TypeCastGAM
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef TYPE_CAST_GAM_H_
#define TYPE_CAST_GAM_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "GAM.h"
#include "ConversionHelper.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
/**
 * @brief TODO TODO
 *
 * GAM which copies its inputs to its outputs. Allows to plug different DataSources (e.g. driver with a DDB).
 * @details This GAM copies its inputs to its outputs. The number of inputs shall
 *  be exactly the number of the outputs and, for each signal, the types shall also be the same.
 *  Given that the DataSources cannot interchange data directly between them the main scope of the TypeCastGAM is to
 *  serve as a (direct) connector between DataSources.
 *
 * The configuration syntax is (names and signal quantity are only given as an example):
 * +Buffer = {
 *     Class = TypeCastGAM
 *     InputSignals = {
 *         Signal1 = {
 *             DataSource = "Drv1"
 *             Type = uint32
 *         }
 *         Signal2 = {
 *             DataSource = "Drv2"
 *             Type = int32
 *         }
 *     }
 *     OutputSignals = {
 *         Signal1O = {
 *             DataSource = "LCD"
 *             Type = uint32
 *         }
 *         Signal2O = {
 *             DataSource = "LCD"
 *             Type = int32
 *         }
 *     }
 * }
 */
namespace MARTe {
class TypeCastGAM: public GAM {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor. NOOP.
     */
    TypeCastGAM();

    /**
     * @brief Destructor. NOOP.
     */
    virtual ~TypeCastGAM();

    /**
     * @brief TODO TODO
     *
     *
     * Verifies correctness of the GAM configuration.
     * @details Checks that the number of input signals is equal to the number of output signals and that,
     * for each signal, the same type is used.
     * @return is the pre-conditions are met.
     * @pre
     *   SetConfiguredDatabase() &&
     *   GetNumberOfInputSignals() == GetNumberOfOutputSignals() &&
     *   for each signal i: GetSignalType(InputSignals, i) == GetSignalType(OutputSignals, i) &&
     *   for each signal i: GetSignalByteSize(InputSignals, i) * GetSignalNumberOfSamples(InputSignals, i) == GetSignalByteSize(OutputSignals, i) * GetSignalNumberOfSamples(OutputSignals, i)
     */
    virtual bool Setup();


    /**
     * @brief TODO
     */
    virtual bool Initialise(StructuredDataI & data);

    /**
     * @brief TODO TODO
     * @brief Copies the input signals memory to the output signal memory.
     * @return true if all the signals memory can be successfully copied.
     */
    virtual bool Execute();

private:
    //TODO
    ConversionHelper **conversionHelpers;

    //TODO
    uint32 numberOfSignals;

    //TODO
    ConfigurationDatabase cdb;
};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* TYPE_CAST_GAM_H_ */

