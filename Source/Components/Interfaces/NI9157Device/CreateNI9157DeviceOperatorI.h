/**
 * @file CreateNI9157DeviceOperatorI.h
 * @brief Header file for class CreateNI9157DeviceOperatorI.
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
 * CreateNI9157DeviceOperatorI with all of its public, protected and private
 * members. It may also include definitions for inline methods which need to be
 * visible to the compiler.
 */

#ifndef CREATENI9157DEVICEOPERATORI_H_
#define CREATENI9157DEVICEOPERATORI_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "NI9157Device.h"
#include "NI9157DeviceOperatorTI.h"
#include "ReferenceT.h"
#include "TypeDescriptor.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe{

/**
 * @brief Interface of the CreateNI9157DeviceOperator templated class. This
 * allows for the NI9157DeviceOperatorDatabase functions to return a pointer to
 * the specific CreateNI9157DeviceOperator.
 * @see NI9157DeviceOperatorDatabase.
 * @see CreateNI9157DeviceOperator.
 */
class CreateNI9157DeviceOperatorI {
public:

    /**
     * @brief Constructor.
     */
    CreateNI9157DeviceOperatorI();

    /**
     * @brief Destructor.
     */
    virtual ~CreateNI9157DeviceOperatorI();

    /**
     * @brief Creates the specific NI9157DeviceOperatorT and returns a pointer
     * to its interface.
     * @param[in] niDev is the reference to the NI9157Device to be wrapped by
     * the returned NI9157DeviceOperatorT.
     * @return a pointer to the interface of the created NI9157DeviceOperatorT.
     * @warning the returned NI9157DeviceOperatorT memory must be freed when
     * finishing to use it.
     */
    virtual NI9157DeviceOperatorTI *Create(ReferenceT<NI9157Device> niDev) const=0;

    /**
     * @brief Returns the name of this creator object.
     * @return the name of this creator object.
     */
    virtual const char8 *GetIdentifier() const=0;

    /**
     * @brief Returns the type associated to this creator object.
     * @return the TypeDescriptor of the type associated to this creator
     * object.
     */
    virtual TypeDescriptor GetTypeDescriptor() const=0;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* CREATENI9157DEVICEOPERATORI_H_ */
