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
 * This class is meant to be the base class for all the connections used
 * in `ObjectLoader`, and provides the common elements and methods for
 * that purpose.
 *
 * The derived classes can shall load numeric parameters (scalars, vectors
 * and matrices up to 3D) as `AnyType` list items of this `StaticList`.
 * Use the superclass method `Add()` to add `AnyType`.
 * For each loaded parameter, the derived classes shall also add the parameter
 * name to the `paramNames` array provided by this class.
 *
 * @note ObjectConnectionI deallocates its list items in the destructor, so
 *       deallocation in the derived class is not necessary.
 *
 * This class provides a `TransposeAndCopy()` method to be used if the loading
 * of a parameter requires orientation change from row-major to column-major.
 * (Allocation is still up to the derived class)
 *
 * Derived classes shall load structured data element by element, in a flattened
 * fashion, with the naming convention of MARTe2 structured data, e.g.:
 *
 * Structure:
 *
 * `paramNames[i]`           | `AnyType` value
 * :------------------------ | :--------------
 * "myStruct.par1"           | 1
 * "myStruct.par2"           | {1, 2, 3}
 * "myStruct.par3"           | "Hello"
 * "myStruct.subStruct.par1" | 1.5
 * "myStruct.subStruct.par2" | {1.5, 2.5, 3.5}
 *
 * Array of structures:
 *
 *  `paramNames[i]`    | `AnyType` value
 * :------------------ | :--------------
 * "myStruct[0].par1"  | 1
 * "myStruct[0].par2"  | {1, 2, 3}
 * "myStruct[0].par3"  | "Hello"
 * "myStruct[1].par1"  | 2
 * "myStruct[1].par2"  | {4, 5, 6}
 * "myStruct[1].par3"  | "World"
 *
 *
 * Release notes
 * ==========================================================================
 *
 * Version |    Date    | Notes
 * ------: | :--------: | :----
 * 1.0     | 16/09/2024 | initial release
 * 2.0     | 31/07/2025 | tested and linted version
 *
 *
 */

//lint -e{1509, 1790} Justification: Cannot change the virtuality of the StaticList destructor
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
     * @brief Initialise method.
     */
    virtual bool Initialise(StructuredDataI & data);

    /**
     * @brief Update the value of all parameters.
     * @details This method is expected to be called by the parent
     *          `ObjectLoader` class before updating its own `AnyObjects`.
     * @warning This method shall be implemented in all subclasses
     */
    virtual ErrorManagement::ErrorType UpdateParameters();

    /**
     * @brief Remove all parameters, deallocate memory.
     */
    virtual ErrorManagement::ErrorType CleanUp();

    /**
     * @brief Returns the name of the parameter in the n-th position of the list
     * @param[in] parameterIndex the index of the parameter whose name has to be returned
     * @returns the n-th parameter name
     */
    StreamString GetParameterName(uint32 parameterIndex);

    /**
     * @brief Return the ErrorType status of this interface.
     */
    ErrorManagement::ErrorType GetStatus();

protected:

    /**
     * @brief Change the orientation of the source data
     * @details When called for row-major data, this method changes the orientation
     *          of the data to column-major. When called for colum-major data, this
     *          method changed its orientation to row-major.
     * @param[in] destination     pointer to the destination data
     * @param[in] source          pointer to the source data
     * @param[in] typeDesc        the type of the data to be reoriented
     * @param[in] numberOfRows    the number of rows of the source data
     * @param[in] numberOfColumns the number of columns of the source data
     * @param[in] numberOfPages   the number of pages of the source data
     */
    ErrorManagement::ErrorType TransposeAndCopy(void *const destination, const void *const source, const TypeDescriptor typeDesc,
                                                const uint32 numberOfRows, const uint32 numberOfColumns, const uint32 numberOfPages);

    /**
     * @brief The names of the loaded parameters.
     */
    StaticList<StreamString*> paramNames;

    /**
     * @brief Error status of this object.
     */
    ErrorManagement::ErrorType status;

private:

    /**
     * @brief Change the orientation of the source data.
     *
     * Called by TransposeAndCopy.
     */
    template<typename T>
    void TransposeAndCopyT(void *const destination, const void *const source,
                           const uint32 numberOfRows, const uint32 numberOfColumns, const uint32 numberOfPages);

};

} /* namespace MARTe */

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* OBJECTCONNECTIONI_H_ */
