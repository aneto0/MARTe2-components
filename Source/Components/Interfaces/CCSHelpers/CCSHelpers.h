/**
 * @file CCSHelpers.h
 * @brief Header file for class CCSHelpers
 * @date 25/05/2022
 * @author Giuseppe Ferro
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

 * @details This header file contains the declaration of the class CCSHelpers
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef CCS_HELPERS_H
#define CCS_HELPERS_H

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
#include "common/AnyValue.h"

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "AnyObject.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {
/**
 * @brief Wraps CCS AnyValue as a StructuredDataI.
 */
namespace CCSHelpers {

/**
 * TODO
 */
bool MARTeToCCSAnyValue(ccs::types::AnyValue &valueOut, MARTe::AnyType &valueIn);

/**
 * TODO
 */
bool MARTeToCCSAnyValue(ccs::types::AnyValue &valueOut, MARTe::StructuredDataI &valueIn);

/**
 * TODO
 */
MARTe::TypeDescriptor GetMARTeBasicType(ccs::base::SharedReference<const ccs::types::ScalarType> typeScalar);

/**
 * TODO
 */
bool CCSToMARTeAnyObject(MARTe::AnyObject &valueOut, const ccs::types::AnyValue &valueIn);

/**
 * TODO
 */
bool CCSToMARTeStructuredDataI(MARTe::StructuredDataI &valueOut, const ccs::types::AnyValue &valueIn);

/**
 * TODO
 */
ccs::types::AnyType *GetCCSBasicType(MARTe::TypeDescriptor td, ccs::types::uint32 numberOfDimensions, ccs::types::uint32 numberOfElements);
}

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* CCS_HELPERS_H */
