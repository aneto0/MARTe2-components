/**
 * @file IOGAM.h
 * @brief Header file for class IOGAM
 * @date 06/08/2016
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

 * @details This header file contains the declaration of the class IOGAM
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef IOGAM_H_
#define IOGAM_H_

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
 * @brief GAM which copies its inputs to its outputs. Allows to plug different DataSources (e.g. driver with a DDB).
 * @details This GAM copies its inputs to its outputs. The total input and output memory sizes shall be the same (but the types and number of signals may be different).
 *  Given that the DataSources cannot interchange data directly between them the main scope of the IOGAM is to
 *  serve as a (direct) connector between DataSources.
 *
 * The configuration syntax is (names and signal quantity are only given as an example):
 * <pre>
 * +Buffer = {
 *     Class = IOGAM
 *     InputSignals = {
 *         Signal1 = {
 *             DataSource = "Drv1"
 *             Type = uint64
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
 * </pre>
 */
class IOGAM: public GAM {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor. NOOP.
     */
    IOGAM();

    /**
     * @brief Destructor. NOOP.
     */
    virtual ~IOGAM();

    /**
     * @brief Checks that the total input signal memory size is equal to the total output signal memory size.
     * @return true is the pre-conditions are met.
     */
    virtual bool Setup();

    /**
     * @brief Copies the input signals memory to the output signal memory.
     * @return true if all the signals memory can be successfully copied.
     */
    virtual bool Execute();

private:
    /**
     * Total number of bytes to copy.
     */
    uint32 totalSignalsByteSize;
};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* IOGAM_H_ */

