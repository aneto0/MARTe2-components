/**
 * @file MDSStructuredDataI.h
 * @brief Header file for class MDSStructuredDataI
 * @date 04/09/2018
 * @author Andre Neto
 * @author Llorenc Capella
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

 * @details This header file contains the declaration of the class MDSStructuredDataI
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef MDSSTRUCTUREDDATAI_H_
#define MDSSTRUCTUREDDATAI_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
#include <mdsobjects.h>

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "Object.h"
#include "StructuredDataI.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {
class MDSStructuredDataI: public Object, public StructuredDataI {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * TODO
     */
MDSStructuredDataI    ();

    /**
     * TODO
     */
    virtual ~MDSStructuredDataI();

    /**
     * TODO
     */
    virtual bool Read(const char8 * const name, const AnyType &value);

    /**
     * TODO
     */
    virtual AnyType GetType(const char8 * const name);

    /**
     * TODO
     */
    virtual bool Write(const char8 * const name, const AnyType &value);

    /**
     * TODO
     */
    virtual bool Copy(StructuredDataI &destination);

    /**
     * TODO
     */
    virtual bool AddToCurrentNode(Reference node);

    /**
     * TODO
     */
    virtual bool MoveToRoot();

    /**
     * TODO
     */
    virtual bool MoveToAncestor(uint32 generations);

    /**
     * TODO.
     */
    virtual bool MoveAbsolute(const char8 * const path);

    /**
     * TODO.
     */
    virtual bool MoveRelative(const char8 * const path);

    /**
     * TODO
     */
    virtual bool MoveToChild(const uint32 childIdx);

    /**
     * TODO
     */
    virtual bool CreateAbsolute(const char8 * const path);

    /**
     * TODO
     */
    virtual bool CreateRelative(const char8 * const path);

    /**
     * TODO
     */
    virtual bool Delete(const char8 * const name);

    /**
     * TODO
     */
    virtual const char8 *GetName();

    /**
     * TODO
     */
    virtual const char8 *GetChildName(const uint32 index);

    /**
     * TODO
     */
    virtual uint32 GetNumberOfChildren();

    /**
     * TODO
     */
    void SetTree(MDSplus::Tree *treeIn);

    /**
     * TODO
     */
    void SetEditMode(bool edit);

    /**
     * TODO
     */
    bool OpenTree(const char8 *const treeName, uint32 pulseNumber);

    /**
     * TODO
     */
    bool CreateTree(const char8 *const treeName);

private:
    /**
     * The MDSplus tree.
     */
    MDSplus::Tree *tree;

    /**
     * The MDSplus tree root node.
     */
    MDSplus::TreeNode *rootNode;

    /**
     * The current MDSplus tree node.
     */
    MDSplus::TreeNode *currentNode;

    /**
     * True if the tree can be edited.
     */
    bool editModeSet;

};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* MDSSTRUCTUREDDATAI_H_ */
