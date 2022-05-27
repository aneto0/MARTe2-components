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
 * @brief Helpers functions to convert from CODAC Core System (structured) types to MARTe (structured) types.
 */
namespace CCSHelpers {

/**
 * @brief Converts a MARTe::AnyType to a ccs::types::AnyValue.
 * @param[out] valueOut the destination (serialised) ccs::types::AnyValue. It shall exist and have a compatible type with the input MARTe::AnyType.
 * @param[in] valueIn the source MARTe::AnyType.
 * @return true if the valueIn type can be converted into a ccs::types::AnyValue.
 */
bool MARTeToCCSAnyValue(ccs::types::AnyValue &valueOut, MARTe::AnyType &valueIn);

/**
 * @brief As MARTeToCCSAnyValue, but using as an input a MARTe::StructuredDataI. 
 * @details If the valueOut is not valid (i.e. !valueOut.GetType().IsValid()), it will be created using the structure and the types from the valueIn.
  * @param[out] valueOut the destination (serialised) ccs::types::AnyValue.
 * @param[in] valueIn the source MARTe::StructuredDataI.
 * @return true if the valueIn type can be converted into a ccs::types::AnyValue.
 */
bool MARTeToCCSAnyValue(ccs::types::AnyValue &valueOut, MARTe::StructuredDataI &valueIn);

/**
 * @brief Converts a ccs::types::ScalarType to a MARTe::TypeDescriptor.
 * @param[in] typeScalar the ccs::types::ScalarType to be converted.
 * @return the converted MARTe::TypeDescriptor or InvalidType if the input type is not convertable.
 */
MARTe::TypeDescriptor GetMARTeBasicType(ccs::base::SharedReference<const ccs::types::ScalarType> typeScalar);

/**
 * @brief Converts a ccs::types::AnyValue into a MARTe::AnyObject.
 * @param[out] valueOut the converted MARTe::AnyObject.
 * @paran[in] valueIn the input ccs::types::AnyValue (to be converted).
 * @return true if the input ccs::types::AnyValue is successfully converted.
 */
bool CCSToMARTeAnyObject(MARTe::AnyObject &valueOut, const ccs::types::AnyValue &valueIn);

/**
 * @brief Converts a ccs::types::AnyValue into a MARTe::StructuredDataI.
 * @param[out] valueOut the converted MARTe::StructuredDataI.
 * @paran[in] valueIn the input ccs::types::AnyValue (to be converted).
 * @return true if the input ccs::types::AnyValue is successfully converted.
 */
bool CCSToMARTeStructuredDataI(MARTe::StructuredDataI &valueOut, const ccs::types::AnyValue &valueIn);

/**
 * @brief Converts a MARTe::TypeDescriptor to a ccs::types::ScalarType.
 * @details A new ccs::types::ScalarType is allocated. If the numberOfDimensions is greater than 0 a ccs::types::ArrayType is zero.
 * @param[in] td the MARTe::TypeDescriptor to be converted.
 * @param[in] numberOfDimensions the number of dimensions of the input type.
 * @param[in] numberOfElements the number of elements of the intput type.
 * @return the converted ccs::types::AnyType or NULL if the input type is not convertable.
 */
ccs::types::AnyType *GetCCSBasicType(MARTe::TypeDescriptor td, ccs::types::uint32 numberOfDimensions, ccs::types::uint32 numberOfElements);
}

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* CCS_HELPERS_H */
