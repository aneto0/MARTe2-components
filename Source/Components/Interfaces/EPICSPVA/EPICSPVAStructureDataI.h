/**
 * @file EPICSPVAStructureDataI.h
 * @brief Header file for class EPICSPVAStructureDataI
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

 * @details This header file contains the declaration of the class EPICSPVAStructureDataI
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef EPICSPVA_EPICSPVASTRUCTUREDATAI_H_
#define EPICSPVA_EPICSPVASTRUCTUREDATAI_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
#include "pv/pvData.h"
#include "pv/pvIntrospect.h"
#include "pv/sharedVector.h"

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "ConfigurationDatabase.h"
#include "Object.h"
#include "StructuredDataI.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Wraps an epics::pvData::PVStructure as a StructuredDataI.
 * @details This class implements the StructuredDataI interface using an epics::pvData::PVStructure as the backend database.
 *
 * Given the way that the PVStructure has to be built (by recursively calling addNestedStructure on a FieldBuilderPtr), it is not
 *  possible to dynamically change the structure once it has been finalised (see FinaliseStructure).
 *
 * The structure can be created either by calling InitStructure, followed by all the node creation methods (CreateAbsolute, CreateRelative), complemented by the Write;
 *  or it can be directly initialised and finalised by passing an existent PVStructure (see SetStructure).
 *
 * The Read method cannot be called until the structure has been finalised.
 *
 * It is also possible to directly access to the underlying PVStructure with the GetRootStruct method.
 */
class EPICSPVAStructureDataI: public StructuredDataI, public Object {
public:
    CLASS_REGISTER_DECLARATION()
    /**
     * @brief NOOP. InitStructure or SetStructure shall be called for this class to be useful.
     * @post
     *    IsStructureFinalised()
     */
    EPICSPVAStructureDataI ();

    /**
     * @brief NOOP.
     */
    virtual ~EPICSPVAStructureDataI();

    /**
     * @brief See StructuredDataI::Read.
     * @pre
     *    IsStructureFinalised().
     */
    virtual bool Read(const char8 * const name, const AnyType &value);

    /**
     * @brief See StructuredDataI::GetType.
     * @pre
     *    IsStructureFinalised().
     */
    virtual AnyType GetType(const char8 * const name);

    /**
     * @brief See StructuredDataI::Write. Can be called before FinaliseStructure.
     */
    virtual bool Write(const char8 * const name, const AnyType &value);

    /**
     * @brief See StructuredDataI::Copy.
     * @pre
     *    IsStructureFinalised().
     */
    virtual bool Copy(StructuredDataI &destination);

    /**
     * @brief See StructuredDataI::AddToCurrentNode.
     * @pre
     *    !IsStructureFinalised().
     */
    virtual bool AddToCurrentNode(Reference node);

    /**
     * @brief See StructuredDataI::MoveToRoot.
     */
    virtual bool MoveToRoot();

    /**
     * @brief See StructuredDataI::MoveToAncestor.
     */
    virtual bool MoveToAncestor(uint32 generations);

    /**
     * @brief See StructuredDataI::MoveAbsolute.
     */
    virtual bool MoveAbsolute(const char8 * const path);

    /**
     * @brief See StructuredDataI::MoveRelative.
     */
    virtual bool MoveRelative(const char8 * const path);

    /**
     * @brief See StructuredDataI::MoveToChild.
     */
    virtual bool MoveToChild(const uint32 childIdx);

    /**
     * @brief See StructuredDataI::CreateAbsolute.
     * @pre
     *    !IsStructureFinalised().
     */
    virtual bool CreateAbsolute(const char8 * const path);

    /**
     * @brief See StructuredDataI::CreateRelative.
     * @pre
     *    !IsStructureFinalised().
     */
    virtual bool CreateRelative(const char8 * const path);

    /**
     * @brief See StructuredDataI::Delete.
     * @pre
     *    !IsStructureFinalised().
     */
    virtual bool Delete(const char8 * const name);

    /**
     * @brief See StructuredDataI::GetName.
     * @pre
     *    IsStructureFinalised().
     */
    virtual const char8 *GetName();

    /**
     * @brief See StructuredDataI::GetChildName.
     * @pre
     *    IsStructureFinalised().
     */
    virtual const char8 *GetChildName(const uint32 index);

    /**
     * @brief See StructuredDataI::GetNumberOfChildren.
     * @pre
     *    IsStructureFinalised().
     */
    virtual uint32 GetNumberOfChildren();

    /**
     * @brief Constructs the structure mapped by this StructuredDataI directly from an existent epics::pvData::PVStructure.
     * @param[in] structPtrToSet the existent epics::pvData::PVStructure.
     * @post
     *    IsStructureFinalised().
     */
    void SetStructure(epics::pvData::PVStructurePtr structPtrToSet);

    /**
     * @brief Setup of the class so that the methods that allow modifying the structure can be called.
     */
    void InitStructure();

    /**
     * @brief Finalises the structure. Allows for the Read method to be used and disallows any further modification of the class structure.
     * @return true if the structure could be successfully created.
     * @post
     *     IsStructureFinalised()
     */
    bool FinaliseStructure();

    /**
     * @brief Returns the epics::pvData::PVStructure mapped by this StructuredDataI.
     * @return the epics::pvData::PVStructure mapped by this StructuredDataI.
     */
    epics::pvData::PVStructurePtr GetRootStruct();

    /**
     * @brief Returns true if the structure has been finalised.
     * @return true if the structure has been finalised.
     */
    bool IsStructureFinalised();

private:
    /**
     * @brief Helper method to read an array from an epics::pvData::PVScalarArray into an AnyType. Should allow to convert from any numeric type to any numeric type.
     * @param[in] scalarArrayPtr the array where to read the data from.
     * @param[in] storedType the type of data to be read (as stored in the backed)
     * @param[out] value where to store the read array.
     * @return true if the array can be successfully read.
     */
    template<typename T>
    bool ReadArray(epics::pvData::PVScalarArrayPtr scalarArrayPtr, AnyType &storedType, const AnyType &value);

    /**
     * @brief Helper method to write an array from an AnyType into an epics::pvData::PVScalarArray.
     * @param[out] scalarArrayPtr the array where to write the data into.
     * @param[in] storedType the type of data to be written (as stored in the backed)
     * @param[in] value where to read the array.
     * @param[in] size number of elements in the array.
     * @return true if the array can be successfully written.
     */
    template<typename T>
    bool WriteArray(epics::pvData::PVScalarArrayPtr scalarArrayPtr, AnyType &storedType, const AnyType &value, const uint32 &size);

    /**
     * @brief Helper method that writes the value into the backend PVScalarPtr or PVScalarArrayPtr.
     * @param[in] name the name of the parameter to write.
     * @param[in] storedType the type of data to be written (as stored in the backend)
     * @param[in] value the value to write.
     * @return true if the value can be successfully written.
     */
    bool WriteStoredType(const char8 * const name, AnyType &storedType, const AnyType &value);

    /**
     * @brief Helper method to add a typed leaf to the backend PVStructure.
     * @param[in] name the name of the parameter to create.
     * @param[in] storedType the type of data to be stored in the backend against this leaf.
     * @return true if the leaf can be successfully created.
     */
    bool CreateFromStoredType(const char8 * const name, AnyType &storedType);

    /**
     * @brief Helper method which transforms the cached ConfigurationDatabase into a PVStructure.
     * @param[in] currentNode the node where to start from.
     * @param[in] create if true the nodes are added to the PVStructure, otherwise only the values are written.
     * @return true if the ConfigurationDatabase is successfully created.
     */
    bool ConfigurationDataBaseToPVStructurePtr(ReferenceT<ReferenceContainer> currentNode, bool create = true);

    /**
     * Cached pointer to the current node.
     */
    epics::pvData::PVStructurePtr currentStructPtr;

    /**
     * Cached pointer to the root node.
     */
    epics::pvData::PVStructurePtr rootStructPtr;

    /**
     * True if the structure was finalised.
     */
    bool structureFinalised;

    /**
     * The FieldBuilderPtr that is used to create the nested PVStructure.
     */
    epics::pvData::FieldBuilderPtr fieldBuilder;

    /**
     * The cached ConfigurationDatabase that is used until the FinaliseStructure is called.
     */
    ConfigurationDatabase cachedCDB;
};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/
namespace MARTe {
template<typename T>
bool EPICSPVAStructureDataI::ReadArray(epics::pvData::PVScalarArrayPtr scalarArrayPtr, AnyType &storedType, const AnyType &value) {
    bool ok = true;
    epics::pvData::shared_vector<const T> out;
    scalarArrayPtr->getAs < T > (out);
    uint32 numberOfElements = storedType.GetNumberOfElements(0u);
    uint32 i;
    //Should allow to convert from numeric anytype to anytype. Note that this is called with the T of the value.
    Vector<T> readVec (reinterpret_cast<T *>(value.GetDataPointer()), numberOfElements);
    Vector<T> srcVec (const_cast<T *>(reinterpret_cast<const T *>(out.data())), numberOfElements);
    for (i = 0u; i<numberOfElements; i++) {
        readVec[i] = srcVec[i];
    }

    return ok;
}

template<typename T>
bool EPICSPVAStructureDataI::WriteArray(epics::pvData::PVScalarArrayPtr scalarArrayPtr, AnyType &storedType, const AnyType &value, const uint32 &size) {
    epics::pvData::shared_vector<T> out;
    out.resize(storedType.GetNumberOfElements(0u));
    bool ok = MemoryOperationsHelper::Copy(reinterpret_cast<void *>(out.data()), value.GetDataPointer(), size);
    epics::pvData::shared_vector<const T> outF = freeze(out);
    scalarArrayPtr->putFrom < T > (outF);
    return ok;
}

}
#endif /* EPICSPVA_EPICSPVASTRUCTUREDATAI_H_ */
