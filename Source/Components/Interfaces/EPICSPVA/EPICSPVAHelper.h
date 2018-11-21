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
    static epics::pvData::StructureConstPtr GetStructure(const Introspection *intro,
                                                         const char8 * const typeName = NULL_PTR(const char8 * const));

    /**
     * @brief Inits PVStructureArrays in the portion of the epics::pvData::PVStructurePtr associated to the structure wrapped by the \a intro parameter and identified by the \a introStructName
     * @param[in] intro the type identifying the structure.
     * @param[in] pvStructPtr the PVStructurePtr to be initialised (in particular the PVStructureArray elements)
     * @param[in] introTypeName the name of the field where intro can be found in \a pvStructPtr.
     * @return true if the structure is successfully initialised.
     */
    static bool InitPVStructureArrays(const Introspection *intro, epics::pvData::PVStructurePtr pvStructPtr,
                                      const char8 * const introFieldName);

};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* EPICSPVA_EPICSPVAHELPER_H_ */
