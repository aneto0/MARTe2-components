/**
 * @file ConstantGAM.h
 * @brief Header file for class ConstantGAM
 * @date 08/03/2017
 * @author Bertrand Bauvir
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

 * @details This header file contains the declaration of the class ConstantGAM
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef ConstantGAM_H_
#define ConstantGAM_H_

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

/**
 * @brief GAM which provides constant output signals. Allows to plug different DataSources (e.g. driver with a DDB).
 * @details This GAM provides constant output signals.
 *
 * The configuration syntax is (names and signal quantity are only given as an example):
 * +Constants = {
 *     Class = ConstantGAM
 *     OutputSignals = {
 *         Signal1 = {
 *             DataSource = "DDB"
 *             Type = uint32
 *             Default = 0
 *         }
 *         Signal2 = {
 *             DataSource = "DDB"
 *             Type = int32
 *             Default = 100
 *         Signal2 = {
 *             DataSource = "DDB"
 *             Type = int8
 *             NumberOfElements = 8
 *             Default = {1 2 3 4 5 6 7 8}
 *         }
 *     }
 * }
 *
 * @todo Add messaging to allow for runtime change of signal values.
 */

namespace MARTe {

class ConstantGAM: public GAM {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor. NOOP.
     */
    ConstantGAM();

    /**
     * @brief Destructor. NOOP.
     */
    virtual ~ConstantGAM();

    /**
     * @brief Initialises the output signal memory with defaultvalues  provided through configuration.
     * @return if the pre-conditions are met.
     * @pre
     *   SetConfiguredDatabase() && GetNumberOfInputSignals() == 0 &&
     *   for each signal i: The default value provided corresponds to the expected type and dimensionality.
     */
    virtual bool Setup();

    /**
     * @brief Execute method. NOOP.
     * @return true.
     */
    virtual bool Execute();
};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* ConstantGAM_H_ */

