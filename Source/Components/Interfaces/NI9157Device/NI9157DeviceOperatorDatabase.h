/**
 * @file NI9157DeviceOperatorDatabase.h
 * @brief Header file for class NI9157DeviceOperatorDatabase.
 * @date 11/02/2021
 * @author Giuseppe Ferro
 * @author Pedro Lourenco
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
 *
 * @details This header file contains the declaration of the class
 * NI9157DeviceOperatorDatabase  with all of its public, protected and private
 * members. It may also include definitions for inline methods which need to be
 * visible to the compiler.
 */

#ifndef NI9157DEVICEOPERATORDATABASE_H_
#define NI9157DEVICEOPERATORDATABASE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "CreateNI9157DeviceOperatorI.h"
#include "NI9157DeviceOperatorTI.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe{

/**
 * @brief These functions allow to get the CreateNI9157DeviceOperator object or
 * directly the NI9157DeviceOperatorT by name (or by type descriptor), in order
 * to create the object to interface with the NI-9157 device.
 * @see CreateNI9157DeviceOperatorI and CreateNI9157DeviceOperator
 * @see NI9157DeviceOperatorTI and NI9157DeviceOperatorT
 */
namespace NI9157DeviceOperatorDatabase {

/**
 * @brief Retrieves the CreateNI9157DeviceOperator by name.
 * @details Possible values of \a niDevId are:
 *   NI9157DeviceBool\n
 *   NI9157DeviceU8\n
 *   NI9157DeviceU16\n
 *   NI9157DeviceU32\n
 *   NI9157DeviceU64\n
 *   NI9157DeviceI8\n
 *   NI9157DeviceI16\n
 *   NI9157DeviceI32\n
 *   NI9157DeviceI64\n
 * @param[in] niDevId is the name associated to the CreateNI9157DeviceOperator in return.
 * @return a pointer to the CreateNI9157DeviceOperator associated to \a niDevId or NULL if it is not found.
 */
CreateNI9157DeviceOperatorI *GetCreateNI9157DeviceOperator(const char8 * const niDevId);

/**
 * @brief Retrieves the CreateNI9157DeviceOperator by type.
 * @details All the SignedInteger and UnsignedInteger TypeDescriptors are supported.
 * @param[in] td is the type associated to the CreateNI9157DeviceOperator in return.
 * @return a pointer to the CreateNI9157DeviceOperator associated to \a td or NULL if it is not found.
 */
CreateNI9157DeviceOperatorI *GetCreateNI9157DeviceOperator(const TypeDescriptor &td);

/**
 * @brief Creates a NI9157DeviceOperatorT by name.
 * @details Possible values of \a niDevId are:
 *   NI9157DeviceBool\n
 *   NI9157DeviceU8\n
 *   NI9157DeviceU16\n
 *   NI9157DeviceU32\n
 *   NI9157DeviceU64\n
 *   NI9157DeviceI8\n
 *   NI9157DeviceI16\n
 *   NI9157DeviceI32\n
 *   NI9157DeviceI64\n
 * @param[in] niDevId is the name associated to the CreateNI9157DeviceOperator in return.
 * @param[in] niDev is the reference to the NI9157Device object that the NI9157DeviceOperatorT has to be linked to.
 * @return a pointer to the NI9157DeviceOperatorT associated to \a niDevId or NULL if it is not found.
 * @warning The NI9157DeviceOperatorT pointer returned has to be freed when it is not used anymore.
 */
NI9157DeviceOperatorTI *GetNI9157DeviceOperator(const char8 * const niDevId, ReferenceT<NI9157Device> niDev);

/**
 * @brief Creates a NI9157DeviceOperatorT by type.
 * @details All the SignedInteger and UnsignedInteger TypeDescriptors are supported.
 * @param[in] td is the type associated to the CreateNI9157DeviceOperator in return.
 * @param[in] niDev is the reference to the NI9157Device object that the NI9157DeviceOperatorT has to be linked to.
 * @return a pointer to the NI9157DeviceOperatorT associated to \a td or NULL if it is not found.
 * @warning The NI9157DeviceOperatorT pointer returned has to be freed when it is not used anymore.
 */
NI9157DeviceOperatorTI *GetNI9157DeviceOperator(const TypeDescriptor &td, ReferenceT<NI9157Device> niDev);

}

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* NI9157DEVICEOPERATORDATABASE_H_ */
