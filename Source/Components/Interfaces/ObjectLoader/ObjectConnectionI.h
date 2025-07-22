/**
 * @file ObjectConnectionI.h
 * @brief Header file for class ObjectConnectionI
 * @date 16/09/2024
 * @author Cristian Galperti
 * @author Nicolo Ferron
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
 * @details This header file contains the declaration of the class ObjectConnectionI
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef OBJECTCONNECTIONI_H_
#define OBJECTCONNECTIONI_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "MessageI.h"
#include "ReferenceContainer.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {

/**
 * @brief StaticList of AnyType. The AnyTypes are meant to be Serialised by
 *        the ObjectLoader.
 *
 * The object provides a TransposeAndCopy method to be used if the loading
 * of a parameter requires orientation change from row-major to column-major.
 * (Allocation is still up to the derived class)
 *
 * @note ObjectConnectionI deallocates itself in the destructor.
 *
 * Release notes
 * ==========================================================================
 *
 * Version |    Date    | Notes
 * ------: | :--------: | :----
 * 1.0     | 16/09/2024 | initial release
 * 1.1     | 31/07/2025 | tested and linted version
 *
 *
 */

class ObjectConnectionI : public StaticList<AnyType*>, public ReferenceContainer, public MessageI {

public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor.
     */
    ObjectConnectionI();

    /**
     * @brief Destructor.
     */
    virtual ~ObjectConnectionI();

    /**
     * @brief
     */
    virtual bool Initialise(StructuredDataI & data);

    /**
     * @brief
     * @warning This method shall be implemented in all subclasses
     */
    virtual ErrorManagement::ErrorType UpdateParameters();

    /**
     * @brief Remove all parameters, deallocate memory.
     */
    virtual ErrorManagement::ErrorType Clean();

    /**
     * @brief Returns the name of the parameter in the n-th position of the list
     */
    StreamString GetParameterName(uint32 parameterIndex);

    /**
     * @brief Return the ErrorType status of this interface.
     */
    ErrorManagement::ErrorType GetStatus();

protected:

    ErrorManagement::ErrorType TransposeAndCopy(void *const destination, const void *const source, const TypeDescriptor typeDesc,
                                                const uint32 numberOfRows, const uint32 numberOfColumns, const uint32 numberOfPages);

    template<typename T>
    void TransposeAndCopyT(void *const destination, const void *const source,
                           const uint32 numberOfRows, const uint32 numberOfColumns, const uint32 numberOfPages);

    StaticList<StreamString*> paramNames;

    ErrorManagement::ErrorType status;

};

} /* namespace MARTe */

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* OBJECTCONNECTIONI_H_ */
