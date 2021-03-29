/**
 * @file CreateNI9157DeviceOperator.h
 * @brief Header file for class CreateNI9157DeviceOperator.
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
 * CreateNI9157DeviceOperator with all of its public, protected and private
 * members. It may also include definitions for inline methods which need to be
 * visible to the compiler.
 */

#ifndef CREATENI9157DEVICEOPERATOR_H_
#define CREATENI9157DEVICEOPERATOR_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "CreateNI9157DeviceOperatorI.h"
#include "NI9157DeviceOperatorT.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {

/**
 * @brief This object allows to create a specific NI9157DeviceOperatorT
 * depending on the template type.
 * @see NI9157DeviceOperatorTI and NI9157DeviceOperatorT.
 */
/*lint -esym(9107, MARTe::CreateNI9157DeviceOperator*) [MISRA C++ Rule 3-1-1]. Justification: Required for template implementation.
 * No code is actually being generated and the header files can be included in multiple unit files.*/
template<typename T>
class CreateNI9157DeviceOperator: public CreateNI9157DeviceOperatorI {
public:

    /**
     * @brief Default constructor.
     * @see CreateNI9157DeviceOperatorI.
     * @post
     *   identifier=NULL
     *   typeDes=InvalidType
     */
    CreateNI9157DeviceOperator();

    /**
     * @brief Constructor with parameters.
     * @param[in] id the object creator name.
     * @post
     *   identifier=id
     *   typeDes=[the type descriptor of the templated type T]. Supported types are all the signed and unsigned integers. If unsupported typeDes=InvalidType.
     */
    CreateNI9157DeviceOperator(const char8 * const id);

    /**
     * @brief Destructor.
     */
    virtual ~CreateNI9157DeviceOperator();

    /**
     * @see CreateNI9157DeviceOperatorI::Create.
     */
    virtual NI9157DeviceOperatorTI *Create(ReferenceT<NI9157Device> niDev) const;

    /**
     * @see CreateNI9157DeviceOperatorI::GetIdentifier.
     */
    virtual const char8 *GetIdentifier() const;

    /**
     * @see CreateNI9157DeviceOperatorI::GetTypeDescriptor.
     */
    virtual TypeDescriptor GetTypeDescriptor() const;

private:

    /**
     * The object name.
     */
    const char8 *identifier;

    /**
     * The associated type descriptor that depends on T.
     */
    TypeDescriptor typeDes;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/
namespace MARTe {

template<typename T>
CreateNI9157DeviceOperator<T>::CreateNI9157DeviceOperator() :
        CreateNI9157DeviceOperatorI(), typeDes() {
    identifier = NULL_PTR(const char8 *);
    typeDes = InvalidType;
}

/*lint -e{740} unusual pointer cast to CreateNI9157DeviceOperator is valid.*/
template<typename T>
CreateNI9157DeviceOperator<T>::CreateNI9157DeviceOperator(const char8 * const id) :
        CreateNI9157DeviceOperatorI() {
    identifier = id;
    //discover the type descriptor
    if (dynamic_cast<CreateNI9157DeviceOperator<bool>*>(this) != NULL) {
        typeDes = UnsignedInteger32Bit;
    }
    if (dynamic_cast<CreateNI9157DeviceOperator<uint8>*>(this) != NULL) {
        typeDes = UnsignedInteger8Bit;
    }
    else if (dynamic_cast<CreateNI9157DeviceOperator<int8>*>(this) != NULL) {
        typeDes = SignedInteger8Bit;
    }
    else if (dynamic_cast<CreateNI9157DeviceOperator<uint16>*>(this) != NULL) {
        typeDes = UnsignedInteger16Bit;
    }
    else if (dynamic_cast<CreateNI9157DeviceOperator<int16>*>(this) != NULL) {
        typeDes = SignedInteger16Bit;
    }
    else if (dynamic_cast<CreateNI9157DeviceOperator<uint32>*>(this) != NULL) {
        typeDes = UnsignedInteger32Bit;
    }
    else if (dynamic_cast<CreateNI9157DeviceOperator<int32>*>(this) != NULL) {
        typeDes = SignedInteger32Bit;
    }
    else if (dynamic_cast<CreateNI9157DeviceOperator<uint64>*>(this) != NULL) {
        typeDes = UnsignedInteger64Bit;
    }
    else if (dynamic_cast<CreateNI9157DeviceOperator<int64>*>(this) != NULL) {
        typeDes = SignedInteger64Bit;
    }
    else {
        typeDes = InvalidType;
    }
}

template<typename T>
CreateNI9157DeviceOperator<T>::~CreateNI9157DeviceOperator() {
    identifier = NULL_PTR(const char8 *);
}

template<typename T>
NI9157DeviceOperatorTI* CreateNI9157DeviceOperator<T>::Create(ReferenceT<NI9157Device> niDev) const {
    NI9157DeviceOperatorTI *newDev = new NI9157DeviceOperatorT<T>(niDev);
    return newDev;
}

template<typename T>
const char8 * CreateNI9157DeviceOperator<T>::GetIdentifier() const {
    return identifier;
}

template<typename T>
TypeDescriptor CreateNI9157DeviceOperator<T>::GetTypeDescriptor() const {
    return typeDes;
}

}

#endif /* CREATENI9157DEVICEOPERATOR_H_ */
