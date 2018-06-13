/**
 * @file EPICSPVStructureDataI.h
 * @brief Header file for class EPICSPVStructureDataI
 * @date 13/06/2018
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

 * @details This header file contains the declaration of the class EPICSPVStructureDataI
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef EPICSPVA_EPICSPVSTRUCTUREDATAI_H_
#define EPICSPVA_EPICSPVSTRUCTUREDATAI_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
#include "pv/pvData.h"
#include "pv/pvIntrospect.h"

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "Object.h"
#include "StructuredDataI.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief TODO
 */
class EPICSPVStructureDataI: public StructuredDataI, public Object {
public:
    CLASS_REGISTER_DECLARATION()
    /**
     * @brief TODO
     */
    EPICSPVStructureDataI();

    /**
     * @brief TODO
     */
    virtual ~EPICSPVStructureDataI();

    /**
     * @brief TODO
     */
    virtual bool Read(const char8 * const name, const AnyType &value);

    /**
     * @brief TODO
     */
    virtual AnyType GetType(const char8 * const name);

    /**
     * @brief TODO
     */
    virtual bool Write(const char8 * const name, const AnyType &value);

    /**
     * @brief TODO
     */
    virtual bool Copy(StructuredDataI &destination);

    /**
     * @brief TODO
     */
    virtual bool AddToCurrentNode(Reference node);

    /**
     * @brief TODO
     */
    virtual bool MoveToRoot();

    /**
     * @brief TODO
     */
    virtual bool MoveToAncestor(uint32 generations);

    /**
     * @brief TODO
     */
    virtual bool MoveAbsolute(const char8 * const path);

    /**
     * @brief TODO
     */
    virtual bool MoveRelative(const char8 * const path);

    /**
     * @brief TODO
     */
    virtual bool MoveToChild(const uint32 childIdx);

    /**
     * @brief TODO
     */
    virtual bool CreateAbsolute(const char8 * const path);

    /**
     * @brief TODO
     */
    virtual bool CreateRelative(const char8 * const path);

    /**
     * @brief TODO
     */
    virtual bool Delete(const char8 * const name);

    /**
     * @brief TODO
     */
    virtual const char8 *GetName();

    /**
     * @brief TODO
     */
    virtual const char8 *GetChildName(const uint32 index);

    /**
     * @brief TODO
     */
    virtual uint32 GetNumberOfChildren();

    /**
     * @brief TODO
     */
    void SetStructure(epics::pvData::PVStructurePtr structPtrToSet);

    /**
     * @brief TODO
     */
    void InitStructure();

private:
    /**
     * @brief TODO
     */
    epics::pvData::PVStructurePtr currentStructPtr;

    /**
     * @brief TODO
     */
    epics::pvData::PVStructurePtr rootStructPtr;
};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* EPICSPVSTRUCTUREDATAI_H_ */
