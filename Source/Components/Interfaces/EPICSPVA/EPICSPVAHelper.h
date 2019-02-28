/**
 * @file EPICSPVAHelper.h
 * @brief Header file for class EPICSPVAHelper
 * @date 20/11/2018
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

 * @details This header file contains the declaration of the class EPICSPVAHelper
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef EPICSPVA_EPICSPVAHELPER_H_
#define EPICSPVA_EPICSPVAHELPER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
#include "pv/pvData.h"

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "IntrospectionT.h"
#include "StreamString.h"
#include "StructuredDataI.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {
/**
 * @brief Helper class for PVA methods.
 */
class EPICSPVAHelper {
public:
    /**
     * @brief Creates an epics::pvData::Structure from an introspection type.
     * @param[in] intro the type from which to create the structure.
     * @param[in] typeName the type name to associate to the structure.
     * @return an epics::pvData::Structure which maps into the introspection type.
     * Note that an invalid epics::pvData::StructureConstPtr will be returned if the operation fails.
     */
    static epics::pvData::StructureConstPtr GetStructure(const Introspection *intro, const char8 * const typeName = NULL_PTR(const char8 * const));

    /**
     * @brief Creates an epics::pvData::Structure from a StructuredDataI.
     * @param[in] data the parent node from which to create the structure.
     * @param[in] typeName the type name to associate to the structure.
     * @return an epics::pvData::Structure which maps into the introspection type.
     * Note that an invalid epics::pvData::StructureConstPtr will be returned if the operation fails.
     */
    static epics::pvData::StructureConstPtr GetStructure(StructuredDataI &data, const char8 * const typeName = NULL_PTR(const char8 * const));

    /**
     * @brief Initialises PVStructureArrays and basic type arrays in the portion of the epics::pvData::PVStructurePtr associated to the structure wrapped by the \a intro parameter.
     * @param[in] intro the type identifying the structure.
     * @param[in] pvStructPtr the PVStructurePtr to be initialised (in particular the PVStructureArray elements).
     * @return true if the structure is successfully initialised.
     */
    static bool InitStructure(const Introspection *intro, epics::pvData::PVStructurePtr pvStructPtr);

    /**
     * @brief Initialises PVStructureArrays and basic type arrays in the portion of the epics::pvData::PVStructurePtr associated to the structure wrapped by the \a data parameter.
     * @param[in] data the parent node from which to create the structure.
     * @param[in] pvStructPtr the PVStructurePtr to be initialised (in particular the PVStructureArray elements).
     * @return true if the structure is successfully initialised.
     */
    static bool InitStructure(StructuredDataI &data, epics::pvData::PVStructurePtr pvStructPtr);

    /**
     * @brief Helper macro which dumps the pvaStruct in a MARTe log stream.
     * @param[in] err the MARTe error type.
     * @param[in] pvaStruct the structure to output.
     */
    static void REPORT_ERROR_PVA_STRUCT(ErrorManagement::ErrorType err, epics::pvData::StructureConstPtr pvaStruct);

    /**
     * @brief Gets the epics::pvData::ScalarType from a MARTe type name.
     * @param[in] memberTypeName the MARTe type name.
     * @param[out] epicsType the corresponding epics::pvData::ScalarType.
     * @return true if the type is supported.
     */
    static bool GetType(const char8 * const memberTypeName, epics::pvData::ScalarType &epicsType);

    /**
     * @brief Gets the epics::pvData::ScalarType from a MARTe TypeDescriptor.
     * @param[in] typeDesc the MARTe TypeDescriptor.
     * @param[out] epicsType the corresponding epics::pvData::ScalarType.
     * @return true if the type is supported.
     */
    static bool GetType(TypeDescriptor typeDesc, epics::pvData::ScalarType &epicsType);

    /**
     * @brief Initialises a PVScalarArray with the specified number of elements.
     * @param[in] pvScalarArr the array to be initialised.
     * @param[in] numberOfElements the number of elements to initialise.
     * @return true if the pvScalarArr is valid and if the type is supported.
     */
    static bool InitArray(epics::pvData::PVScalarArrayPtr pvScalarArr, const uint32 &numberOfElements);

    /**
     * @brief Helper code which replaces the PVStructureArrays with the equivalent set of PVStructures.
     * @param[in] intro the type identifying the structure.
     * @param[in] pvStructPtr the PVStructurePtr containing the arrays to be replaced.
     * @param[in] numberOfElements the number of elements to be replaced.
     * @param[in] typeName the type name of the structured type.
     * @return true if the array is successfully replaced by a set of of equivalent PVStructures of type (typeName) and with size = \a numberOfElements.
     */
    static bool ReplaceStructureArray(const Introspection *intro, epics::pvData::PVStructureArrayPtr pvStructPtr, uint32 numberOfElements = 0u,
                                      const char8 * const typeName = NULL_PTR(const char8 * const));

    /**
     * @brief Helper code which replaces the PVStructureArrays with the equivalent set of PVStructures.
     * @param[in] data the type identifying the structure.
     * @param[in] pvStructPtr the PVStructurePtr containing the arrays to be replaced.
     * @param[in] numberOfElements the number of elements to be replaced.
     * @param[in] typeName the type name of the structured type.
     * @return true if the array is successfully replaced by a set of of equivalent PVStructures of type (typeName) and with size = \a numberOfElements.
     */
    static bool ReplaceStructureArray(StructuredDataI &data, epics::pvData::PVStructureArrayPtr pvStructPtr, uint32 numberOfElements = 0u,
                                      const char8 * const typeName = NULL_PTR(const char8 * const));

};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* EPICSPVA_EPICSPVAHELPER_H_ */
