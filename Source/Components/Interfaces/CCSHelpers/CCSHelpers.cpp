/**
 * @file CCSHelpers.cpp
 * @brief Source file for class CCSHelpers
 * @date 25/05/2022
 * @author Giuseppe Ferro
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

 * @details This source file contains the definition of all the methods for
 * the class CCSHelpers (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API
/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <common/BasicTypes.h>
#include <common/CyclicRedundancyCheck.h>
#include <common/StringTools.h>
#include <common/LookUpTable.h>
#include <common/SharedReference.h>
#include <common/log-api.h>
#include <common/AnyTypeDatabase.h>
#include <common/ObjectFactory.h>
#include <common/types.h>
#include <common/tools.h>
#include <common/AnyObject.h>
#include <common/AnyType.h>
#include <common/ScalarType.h>
#include <common/AnyTypeHelper.h>
#include <common/AnyValue.h>
#include <common/AnyValueHelper.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AnyType.h"
#include "AdvancedErrorManagement.h"
#include "CCSHelpers.h"
#include "ConfigurationDatabase.h"
#include "Reference.h"
#include "ReferenceContainer.h"
#include "TypeConversion.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {
namespace CCSHelpers {

static bool IsStringArray(MARTe::TypeDescriptor td, uint32 numberOfDimensions, uint32 numberOfElements1) {
    bool isStringArray = (td == MARTe::Character8Bit);
    if (!isStringArray) {
        isStringArray = (td == MARTe::CharString);
    }
    if (isStringArray) {
        isStringArray = (numberOfDimensions > 0u);
    }
    if (isStringArray) {
        isStringArray = (numberOfElements1 > 1u); //1D arrays are not string arrays... they are char8 * arrays
    }
    return isStringArray;
}

static bool CCSCompoundTypeFromStructuredDataI(ccs::types::CompoundType *retType,
                                              MARTe::StructuredDataI *sdi) {
    bool ret = (sdi != NULL);
    if (ret) {
        if (sdi->GetName() != NULL) {
            StreamString nname;
            //Need to guarantee that have branch "type" is uniquely identified
            (void) nname.Printf("%s_%d", sdi->GetName(), HighResolutionTimer::Counter());
            retType->SetName(nname.Buffer());
        }
        else {
            retType->SetName("Root");
        }
        ccs::types::uint32 numberOfChildren = sdi->GetNumberOfChildren();
        for (ccs::types::uint32 i = 0u; (i < numberOfChildren) && ret; i++) {
            const ccs::types::char8 *childName = sdi->GetChildName(i);
            if (sdi->MoveRelative(childName)) {
                ccs::types::CompoundType *memberType = new ccs::types::CompoundType;
                //go recursive here!
                ret = CCSCompoundTypeFromStructuredDataI(memberType, sdi);
                if (ret) {
                    retType->AddAttribute(childName, memberType);
                }
                if (ret) {
                    ret = sdi->MoveToAncestor(1u);
                }
            }
            else {
                MARTe::AnyType leaf = sdi->GetType(childName);
                ret = (leaf.GetDataPointer() != NULL);
                if (ret) {
                    MARTe::TypeDescriptor td = leaf.GetTypeDescriptor();
                    ccs::types::AnyType *leafType = NULL;
                    uint32 nElems = leaf.GetNumberOfElements(0u);
                    uint32 nDims = leaf.GetNumberOfDimensions();
                    if (nElems == 0u) {
                        nElems = 1u;
                    }
                    leafType = GetCCSBasicType(td, nDims, nElems);
                    ret = (leafType != NULL);
                    if (ret) {
                        retType->AddAttribute(childName, leafType);
                    }
                }
            }
        }
    }
    return ret;
}

static bool MARTeToCCSBasicType(ccs::types::AnyValue &valueOut,
                                MARTe::AnyType &element) {

    //sup does not support matrices or greater
    MARTe::TypeDescriptor td = element.GetTypeDescriptor();
    ccs::types::uint32 numberOfDimensions = element.GetNumberOfDimensions();
    ccs::types::uint32 numberOfElements = element.GetNumberOfElements(0u);

    //resolve the type
    bool ret = (numberOfDimensions < 2u);
    if (ret) {
        ccs::types::AnyType *retType = GetCCSBasicType(td, numberOfDimensions, numberOfElements);
        if (td == MARTe::StructuredDataInterfaceType) {
            MARTe::StructuredDataI *sdi = reinterpret_cast<MARTe::StructuredDataI*>(element.GetDataPointer());
            ret = CCSCompoundTypeFromStructuredDataI(dynamic_cast<ccs::types::CompoundType*>(retType), sdi);
        }
        ccs::base::SharedReference<const ccs::types::AnyType> basicType(retType);
        if (ret) {
            ret = basicType.IsValid();
        }
        if (ret) {
            bool isStringArray = IsStringArray(td, element.GetNumberOfDimensions(), element.GetNumberOfElements(1u));
            valueOut = ccs::types::AnyValue(basicType);
            if (td == MARTe::StructuredDataInterfaceType) {
                MARTe::StructuredDataI *sdi = reinterpret_cast<MARTe::StructuredDataI*>(element.GetDataPointer());
                ret = MARTeToCCSAnyValue(valueOut, *sdi);
            }
            else if (isStringArray) {
                const char8 **mStrings = static_cast<const char8 **>(element.GetDataPointer());
                const ::ccs::base::SharedReference<const ::ccs::types::ArrayType> atype = valueOut.GetType();
                uint32 asize = atype->GetMultiplicity();
                for (uint32 index = 0u; index < asize; index++) {
                    (void)::ccs::HelperTools::SafeStringCopy(
                            static_cast<char8 *>(::ccs::HelperTools::GetElementReference(&valueOut, index)),
                            mStrings[index], ::ccs::types::MaxStringLength);
                }
            }
            else {
                ret = MemoryOperationsHelper::Copy(valueOut.GetInstance(), element.GetDataPointer(), (element.GetByteSize() * numberOfElements));
            }
        }
    }

    return ret;
}

static ccs::types::CompoundType* GetStructuredType(MARTe::TypeDescriptor td) {

    const MARTe::ClassRegistryItem *item = MARTe::ClassRegistryDatabase::Instance()->Peek(td.structuredDataIdCode);
    bool ret = (item != NULL);
    ccs::types::CompoundType *structuredType = NULL;
    if (ret) {
        structuredType = new ccs::types::CompoundType;
        const MARTe::ClassProperties *classProp = item->GetClassProperties();
        if (classProp != NULL) {
            structuredType->SetName(classProp->GetName());
        }
        const MARTe::Introspection *introspection = item->GetIntrospection();
        if (introspection != NULL) {
            ccs::types::uint32 numberOfMembers = introspection->GetNumberOfMembers();
            for (ccs::types::uint32 i = 0u; i < numberOfMembers; i++) {
                const MARTe::IntrospectionEntry introEntry = (*introspection)[i];
                const ccs::types::char8 *memberName = introEntry.GetMemberName();
                const ccs::types::char8 *memberTypeName = introEntry.GetMemberTypeName();

                ccs::types::uint8 numberOfDimensions = introEntry.GetNumberOfDimensions();
                ccs::types::uint32 numberOfElements = introEntry.GetNumberOfElements(0u);
                MARTe::TypeDescriptor memberTd = introEntry.GetMemberTypeDescriptor();
                //above vectors not supported (exception for array of char8 *)
                bool isStringArray = IsStringArray(memberTd, numberOfDimensions, introEntry.GetNumberOfElements(1u));
                ret = (numberOfDimensions < 2u);
                if (!ret) {
                    //Allow for char[][]
                    if (numberOfDimensions == 2u) {
                        ret = isStringArray;
                    }
                }
                if (ret) {
                    if (memberTd.isStructuredData) {
                        //go recursive
                        ccs::base::SharedReference < ccs::types::AnyType > newType(GetStructuredType(memberTd));
                        ret = (newType.IsValid());
                        if (ret) {
                            if (numberOfDimensions > 0u) {
                                std::string arrayName = memberTypeName;
                                arrayName += "Array";
                                ccs::base::SharedReference < ccs::types::AnyType> newArrayType(new ccs::types::ArrayType(arrayName.c_str(), newType, numberOfElements));
                                structuredType->AddAttribute(memberName, newArrayType);
                            }
                            else {
                                //get the inner type
                                structuredType->AddAttribute(memberName, newType);
                            }
                        }
                    }
                    else {
                        //basic type, get Scalar or Array
                        if (isStringArray) {
                            //Force the type for string arrays to guarantee that it works for char8[][], char8[] and string *
                            memberTd = MARTe::CharString;
                        }
                        ccs::base::SharedReference<const ccs::types::AnyType> newType(GetCCSBasicType(memberTd, numberOfDimensions, numberOfElements));
                        structuredType->AddAttribute(memberName, newType);
                    }
                }
            }
        }
        if (!ret) {
            delete structuredType;
            structuredType = NULL;
        }
    }

    return structuredType;
}

static bool MARTeToCCSStructuredType(ccs::types::AnyValue &valueOut,
                                     MARTe::AnyType &element) {

    MARTe::TypeDescriptor td = element.GetTypeDescriptor();

    bool ret = true;
    //needs to navigate the introspection twice if we want to avoid continuous memory allocation
    ccs::base::SharedReference<const ccs::types::CompoundType> structuredType(valueOut.GetType());
    if (!structuredType.IsValid()) {
        structuredType = ccs::base::SharedReference<const ccs::types::CompoundType>(GetStructuredType(td));
        ret = structuredType.IsValid();
        if (ret) {
            valueOut = ccs::types::AnyValue(structuredType);
        }
    }
    if (ret) {

        const MARTe::ClassRegistryItem *item = MARTe::ClassRegistryDatabase::Instance()->Peek(td.structuredDataIdCode);

        ret = (item != NULL);
        ccs::types::uint8 *sourcePtr = reinterpret_cast<ccs::types::uint8*>(element.GetDataPointer());
        ccs::types::uint8 *destPtr = reinterpret_cast<ccs::types::uint8*>(valueOut.GetInstance());
        if (ret) {
            const MARTe::Introspection *introspection = item->GetIntrospection();
            if (introspection != NULL) {
                ccs::types::uint32 numberOfMembers = introspection->GetNumberOfMembers();
                for (ccs::types::uint32 i = 0u; (i < numberOfMembers) && ret; i++) {
                    const MARTe::IntrospectionEntry introEntry = (*introspection)[i];
                    const ccs::types::char8 *memberName = introEntry.GetMemberName();
                    ccs::types::uint32 memberSize = introEntry.GetMemberSize();
                    MARTe::TypeDescriptor memberTd = introEntry.GetMemberTypeDescriptor();

                    ccs::types::uint32 dstMemberOffset = structuredType->GetAttributeOffset(memberName);
                    ccs::types::uint8 *dstMemberPtr = &destPtr[dstMemberOffset];

                    ccs::types::uint32 srcMemberOffset = introEntry.GetMemberByteOffset();
                    ccs::types::uint8 *sourceMemberPtr = &sourcePtr[srcMemberOffset];

                    //create a new AnyValue with the right pointer
                    ccs::base::SharedReference<const ccs::types::AnyType> innerType = structuredType->GetAttributeType(memberName);
                    //consider number of elements
                    if (memberTd.isStructuredData) {
                        //wrap the memory and go recursive!
                        ccs::types::uint8 numberOfDimensions = introEntry.GetNumberOfDimensions();

                        if (numberOfDimensions > 0u) {
                            ccs::types::uint32 numberOfElements = introEntry.GetNumberOfElements(0u);
                            ccs::types::uint32 supMemberSize = innerType->GetSize();
                            for (ccs::types::uint32 k = 0u; (k < numberOfElements) && (ret); k++) {
                                ccs::types::AnyValue newVal(innerType, &dstMemberPtr[k * supMemberSize]);
                                MARTe::AnyType newElement(memberTd, 0u, &sourceMemberPtr[k * memberSize]);
                                ret = MARTeToCCSStructuredType(newVal, newElement);
                            }
                        }
                        else {
                            ccs::types::AnyValue newVal(innerType, dstMemberPtr);
                            MARTe::AnyType newElement(memberTd, 0u, sourceMemberPtr);
                            ret = MARTeToCCSStructuredType(newVal, newElement);
                        }
                    }
                    else {
                        bool isStringArray = IsStringArray(memberTd, introEntry.GetNumberOfDimensions(), introEntry.GetNumberOfElements(1u));
                        if (isStringArray) {
                            uint32 numberOfElements = introEntry.GetNumberOfElements(0u);
                            ccs::base::SharedReference<const ccs::types::ArrayType> strArrType = innerType;
                            ret = strArrType.IsValid();
                            uint32 sOffset = 0u;
                            for (uint32 index = 0u; (index < numberOfElements) && (ret); index++) {
                                const char8 *mString = reinterpret_cast<const char8 *>(&sourceMemberPtr[sOffset]);
                                ret = ::ccs::HelperTools::SafeStringCopy(
                                    static_cast<char8 *>(strArrType->GetElementReference(dstMemberPtr, index)),
                                    mString, ::ccs::types::MaxStringLength);
                                sOffset += introEntry.GetNumberOfElements(1u);
                            }
                        }
                        else {
                            uint32 ne = 1u;
                            for (uint32 d=0u; d<introEntry.GetNumberOfDimensions(); d++) {
                                uint32 ned = introEntry.GetNumberOfElements(d);
                                if (ned > 0u) {
                                    ne *= ned;
                                }
                            }
                            //Take into account numberOfElements
                            ret = MemoryOperationsHelper::Copy(dstMemberPtr, sourceMemberPtr, memberSize * ne);
                        }
                    }
                }
            }
        }
    }
    return ret;
}

static bool CCSToMARTeScalarType(MARTe::AnyObject &valueOut,
                                 const ccs::types::AnyValue &valueIn,
                                 ccs::types::uint32 numberOfDimensions = 0u,
                                 ccs::types::uint32 numberOfElements = 1u) {
    MARTe::TypeDescriptor td;
    if (numberOfDimensions == 0u) {
        td = GetMARTeBasicType(valueIn.GetType());
    }
    else {
        ccs::base::SharedReference<const ccs::types::ArrayType> arrayType(valueIn.GetType());
        td = GetMARTeBasicType(arrayType->GetElementType());
    }

    bool ret = (td != MARTe::InvalidType);
    if (ret) {
        uint32 nElements1 = 0u;
        if (td == MARTe::CharString) { //Array of strings. For string array
            nElements1 = numberOfElements;
        }
        bool isStringArray = IsStringArray(td, numberOfDimensions, nElements1);
        if (isStringArray) {
            ccs::base::SharedReference<const ccs::types::ArrayType> strArrayType(valueIn.GetType());
            Vector<StreamString> destMem(numberOfElements);
            for (uint32 index=0; index<numberOfElements; index++) {
                const char8 *src = static_cast<const char8 *>(strArrayType->GetElementReference(valueIn.GetInstance(), index));
                destMem[index] = src;
            }
            MARTe::AnyType tempType(destMem);
            valueOut.Serialise(tempType);
        }
        else {
            ccs::types::uint32 memSize = (td.numberOfBits / 8u);
            ccs::types::uint8 *memory = new MARTe::uint8[memSize * numberOfElements];
            ret = MemoryOperationsHelper::Copy(memory, valueIn.GetInstance(), memSize * numberOfElements);
            MARTe::AnyType tempType(td, 0, memory);
            tempType.SetNumberOfDimensions(numberOfDimensions);
            tempType.SetNumberOfElements(0u, numberOfElements);
            valueOut.Serialise(tempType);
            delete[] memory;
        }
    }
    return ret;
}

static bool CreateMARTeStructType(ccs::base::SharedReference<const ccs::types::CompoundType> typeIn,
                                  MARTe::ConfigurationDatabase &globalCdb) {
    bool ret = typeIn.IsValid();
    StreamString typeName = "+";
    typeName += typeIn->GetName();
    //check if the type is already registered
    const MARTe::ClassRegistryItem *registeredMember = MARTe::ClassRegistryDatabase::Instance()->Find(typeIn->GetName());
    if (registeredMember == NULL) {

        MARTe::ConfigurationDatabase newTypeCdb;

        newTypeCdb.CreateAbsolute(typeName.Buffer());
        newTypeCdb.Write("Class", "IntrospectionStructure");
        if (ret) {
            //loop on the attributes
            ccs::types::uint32 numberOfAttributes = typeIn->GetAttributeNumber();
            for (ccs::types::uint32 i = 0u; (i < numberOfAttributes) && (ret); i++) {
                ccs::base::SharedReference<const ccs::types::AnyType> memberType(typeIn->GetAttributeType(i));
                //create a node with the attribute name
                newTypeCdb.CreateRelative(typeIn->GetAttributeName(i));
                //write the type name
                ccs::base::SharedReference<const ccs::types::CompoundType> compType = memberType;
                ccs::base::SharedReference<const ccs::types::ArrayType> arrayType = memberType;
                if (arrayType) {
                    newTypeCdb.Write("NumberOfElements", arrayType->GetElementNumber());
                    ccs::base::SharedReference<const ccs::types::AnyType> baseType(arrayType->GetElementType());
                    newTypeCdb.Write("Type", baseType->GetName());
                    ccs::base::SharedReference<const ccs::types::CompoundType> compArrType = baseType;
                    if (compArrType.IsValid()) {
                        ccs::base::SharedReference<const ccs::types::ArrayType> arrArrType = baseType;
                        //array of array not supported!
                        ret = (!arrArrType);
                        if (ret) {
                            ret = CreateMARTeStructType(compArrType, globalCdb);
                        }
                        else {
                            REPORT_ERROR_STATIC_0(ErrorManagement::Information, "Array of array not supported");
                        }
                    }
                }

                else if (compType) {
                    newTypeCdb.Write("Type", memberType->GetName());
                    ret = CreateMARTeStructType(compType, globalCdb);
                }
                else {
                    ret = newTypeCdb.Write("Type", memberType->GetName());
                }
                if (ret) {
                    newTypeCdb.MoveToAncestor(1u);
                }
            }
        }
        if (ret) {
            ret = newTypeCdb.MoveToRoot();
        }

        //do this to keep the references?
        if (ret) {
            ret = globalCdb.MoveToRoot();
        }
        if (ret) {
            ret = newTypeCdb.Copy(globalCdb);
        }
    }
    return ret;
}

static MARTe::ReferenceContainer ccsToMARTeGlobalTypesContainer;
static bool CCSToMARTeIntrospectableType(MARTe::AnyObject &valueOut,
                                         const ccs::types::AnyValue &valueIn,
                                         MARTe::AnyType recType = MARTe::voidAnyType) {
    //need to create the Introspection from the type before (using cdb and IntrospectionStructure)
    ccs::base::SharedReference<const ccs::types::CompoundType> typeIn = valueIn.GetType();
    bool ret = true;
    bool serialise = (recType.GetDataPointer() == NULL);
    if (serialise) {
        //create the MARTe introspection
        MARTe::ConfigurationDatabase globalCdb;
        ret = CreateMARTeStructType(typeIn, globalCdb);
        //this way we should keep adding types without destroying them
        //instead of using ObjectRegistryDatabase, use a global ReferenceContainer
        if (ret) {
            MARTe::ReferenceT < MARTe::ReferenceContainer > initialiser(MARTe::GlobalObjectsDatabase::Instance()->GetStandardHeap());
            initialiser->Initialise(globalCdb);
            ret = ccsToMARTeGlobalTypesContainer.Insert(initialiser);
        }
    }
    if (ret) {
        //loop on the attributes. The structure can be packed or not on MARTe side
        const MARTe::ClassRegistryItem *registeredMember = MARTe::ClassRegistryDatabase::Instance()->Find(typeIn->GetName());
        ret = (registeredMember != NULL);
        if (ret) {
            const MARTe::ClassProperties *registeredProp = registeredMember->GetClassProperties();
            ret = (registeredProp != NULL);

            if (ret) {
                //create the MARTe::AnyType
                ccs::types::uint32 typeSize = typeIn->GetSize();
                ccs::types::uint8 *dstMem = reinterpret_cast<ccs::types::uint8*>(recType.GetDataPointer());

                //first creation
                MARTe::AnyType retType;
                if (serialise) {
                    dstMem = new ccs::types::uint8[typeSize];
                    MARTe::ClassUID structId = registeredProp->GetUniqueIdentifier();
                    MARTe::TypeDescriptor structTd(false, structId);
                    retType = MARTe::AnyType(structTd, 0u, dstMem);
                }

                ccs::types::uint8 *srcMem = reinterpret_cast<ccs::types::uint8*>(valueIn.GetInstance());
                const MARTe::Introspection *intro = registeredMember->GetIntrospection();

                ret = (intro != NULL);

                if (ret) {
                    ccs::types::uint32 numberOfAttributes = typeIn->GetAttributeNumber();

                    for (ccs::types::uint32 i = 0u; (i < numberOfAttributes) && (ret); i++) {
                        ccs::types::uint32 dstOffset = (*intro)[i].GetMemberByteOffset();
                        ccs::types::uint32 srcOffset = typeIn->GetAttributeOffset(i);
                        ccs::types::uint32 marteMemberSize = (*intro)[i].GetMemberSize();
                        ccs::base::SharedReference<const ccs::types::AnyType> memberType = typeIn->GetAttributeType(i);
                        ccs::base::SharedReference<const ccs::types::ScalarType> typeScalar = memberType;
                        ccs::base::SharedReference<const ccs::types::ArrayType> typeArray = memberType;
                        if (typeScalar.IsValid()) {
                            ret = MemoryOperationsHelper::Copy(&dstMem[dstOffset], &srcMem[srcOffset], marteMemberSize);
                        }
                        else if (typeArray.IsValid())  {
                            ccs::base::SharedReference<const ccs::types::AnyType> innerType(typeArray->GetElementType());
                            ccs::base::SharedReference<const ccs::types::ScalarType> inScalarType = innerType;
                            if (inScalarType) {
                                ret = !IsStringArray((*intro)[i].GetMemberTypeDescriptor(), 1u, (*intro)[i].GetNumberOfElements(1u));
                                if (ret) {
                                    //it is the same since number of elements is considered in the size
                                    ret = MemoryOperationsHelper::Copy(&dstMem[dstOffset], &srcMem[srcOffset], typeArray->GetSize());
                                }
                                else {
                                    REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "Array of strings serialisation currently not implemented for Instrospectable types");
                                }
                            }
                            else {
                                ccs::base::SharedReference<const ccs::types::ArrayType> inArrayType = innerType;
                                ret = (!inArrayType.IsValid());
                                if (ret) {
                                    ccs::types::uint32 numberOfElements = typeArray->GetMultiplicity();

                                    for (ccs::types::uint32 k = 0u; (k < numberOfElements) && ret; k++) {
                                        //wrap the AnyType and go recursive
                                        MARTe::TypeDescriptor memberTd = (*intro)[i].GetMemberTypeDescriptor();
                                        recType = MARTe::AnyType(memberTd, 0u, &dstMem[dstOffset + (k * marteMemberSize)]);
                                        ccs::types::uint32 supMemberSize = innerType->GetSize();
                                        ccs::types::AnyValue recAvalue(innerType, &srcMem[srcOffset + (k * supMemberSize)]);
                                        ret = CCSToMARTeIntrospectableType(valueOut, recAvalue, recType);
                                    }
                                }
                            }
                        }
                        else {
                            //normal compound type
                            //wrap the AnyType and go recursive
                            MARTe::TypeDescriptor memberTd = (*intro)[i].GetMemberTypeDescriptor();
                            recType = MARTe::AnyType(memberTd, 0u, &dstMem[dstOffset]);
                            ccs::types::AnyValue recAvalue(memberType, &srcMem[srcOffset]);
                            ret = CCSToMARTeIntrospectableType(valueOut, recAvalue, recType);
                        }
                    }
                }
                if (serialise) {
                    //serialise the AnyObject and free the mem
                    //todo bug in marte... this will not work
                    valueOut.Serialise(retType);

                    void *ptr = valueOut.GetType().GetDataPointer();
                    uint32 totalSize = typeIn->GetSize();
                    void *newPtr = HeapManager::Realloc(ptr, totalSize);
                    ret = (ptr == newPtr);
                    if (ret) {
                        MemoryOperationsHelper::Copy(ptr, dstMem, totalSize);
                    }
                    delete[] dstMem;
                }
            }
        }
    }
    return ret;
}

static bool CCSToMARTeStructuredType(MARTe::AnyObject &valueOut,
                                     const ccs::types::AnyValue &valueIn) {
    ccs::base::SharedReference<const ccs::types::CompoundType> typeIn = valueIn.GetType();
    bool ret = typeIn.IsValid();
    if (ret) {
        std::string typeName = typeIn->GetName();
        bool isConfigurationDatabase = ((typeName.rfind("StructuredDataI") == 0) || (typeName.rfind("ConfigurationDatabase") == 0));

        if (isConfigurationDatabase) {
            //ConfigurationDatabase type
            MARTe::ConfigurationDatabase *newCdb = new MARTe::ConfigurationDatabase;
            ret = CCSToMARTeStructuredDataI(*newCdb, valueIn);
            //special case! Pass the pointer but remember to destroy it
            newCdb->MoveToRoot();

            MARTe::AnyType cdbPtrAt((void*) newCdb);
            valueOut.Serialise(cdbPtrAt);
            //this cannot be done!
            //delete newCdb;
            /*StreamString debug;
            debug.Printf("%!", *newCdb);
            printf("%s\n", debug.Buffer());*/
        }
        else {
            //introspectable structured type
            ret = CCSToMARTeIntrospectableType(valueOut, valueIn);
        }
    }
    return ret;
}
}
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

namespace CCSHelpers {

bool MARTeToCCSAnyValue(ccs::types::AnyValue &valueOut, MARTe::StructuredDataI &valueIn) {
    ccs::base::SharedReference<const ccs::types::CompoundType> outType = valueOut.GetType();
    bool ret = outType.IsValid();
    //If the type does not exist, create it
    if (!ret) {
        ccs::types::CompoundType *retType = new ccs::types::CompoundType("StructuredDataI");
        ret = CCSCompoundTypeFromStructuredDataI(retType, &valueIn);
        if (ret) {
            ccs::base::SharedReference<const ccs::types::AnyType> basicType(retType);
            if (ret) {
                ret = basicType.IsValid();
            }
            if (ret) {
                valueOut = ccs::types::AnyValue(basicType);
            }
        }
        if (ret) {
            outType = valueOut.GetType();
        }
    }
    ccs::types::uint8 *memPtr;
    if (ret) {
        memPtr = reinterpret_cast<ccs::types::uint8*>(valueOut.GetInstance());
    }
    if (ret) {
        ccs::types::uint32 numberOfChildren = valueIn.GetNumberOfChildren();
        for (ccs::types::uint32 i = 0u; (i < numberOfChildren) && ret; i++) {
            const ccs::types::char8 *childName = valueIn.GetChildName(i);
            if (valueIn.MoveRelative(childName)) {
                ccs::base::SharedReference<const ccs::types::CompoundType> innerType = outType->GetAttributeType(childName);
                ccs::types::uint32 memberOffset = outType->GetAttributeOffset(childName);

                ccs::types::AnyValue memberVal(innerType, &memPtr[memberOffset]);
                //go recursive here!
                ret = MARTeToCCSAnyValue(memberVal, valueIn);
                valueIn.MoveToAncestor(1u);
            }
            else {
                MARTe::AnyType leaf = valueIn.GetType(childName);
                ccs::types::uint32 memberSize = outType->GetAttributeSize(childName);
                ccs::types::uint32 memberOffset = outType->GetAttributeOffset(childName);
                bool isStringArray = IsStringArray(leaf.GetTypeDescriptor(), leaf.GetNumberOfDimensions(), leaf.GetNumberOfElements(1u));
                if (isStringArray) {
                    ccs::base::SharedReference<const ::ccs::types::ArrayType> atype = outType->GetAttributeType(childName);
                    ret = atype.IsValid();
                    if (ret) {
                        const char8 **leafStrings = static_cast<const char8 **>(leaf.GetDataPointer());
                        uint32 asize = atype->GetMultiplicity();
                        for (uint32 index = 0u; (index < asize) && (ret); index++) {
                            ret = ::ccs::HelperTools::SafeStringCopy(
                                static_cast<char8 *>(atype->GetElementReference(&memPtr[memberOffset], index)),
                                leafStrings[index], ::ccs::types::MaxStringLength);
                        }
                    }
                }
                else {
                    ccs::types::uint8 *srcPtr = reinterpret_cast<ccs::types::uint8*>(leaf.GetDataPointer());
                    ret = (srcPtr != NULL);
                    if (ret) {
                        ret = MemoryOperationsHelper::Copy(&memPtr[memberOffset], srcPtr, memberSize);
                    }
                }
            }
        }
    }
    return ret;
}

bool MARTeToCCSAnyValue(ccs::types::AnyValue &valueOut,
                        MARTe::AnyType &valueIn) {
    bool ret = !valueIn.IsVoid();
    if (ret) {

        MARTe::TypeDescriptor td = valueIn.GetTypeDescriptor();
        bool isStructuredData = td.isStructuredData;
        if (!isStructuredData) {
            //basic type
            ret = MARTeToCCSBasicType(valueOut, valueIn);
        }
        else {
            ret = MARTeToCCSStructuredType(valueOut, valueIn);
        }
    }
    return ret;
}

MARTe::TypeDescriptor GetMARTeBasicType(ccs::base::SharedReference<const ccs::types::ScalarType> typeScalar) {
    MARTe::TypeDescriptor ret = InvalidType;

    std::string typeName = typeScalar->GetName();
    if (typeName == "uint8") {
        ret = MARTe::UnsignedInteger8Bit;
    }
    else if (typeName == "uint16") {
        ret = MARTe::UnsignedInteger16Bit;
    }
    else if (typeName == "uint32") {
        ret = MARTe::UnsignedInteger32Bit;
    }
    else if (typeName == "uint64") {
        ret = MARTe::UnsignedInteger64Bit;
    }
    else if (typeName == "int8") {
        ret = MARTe::SignedInteger8Bit;
    }
    else if (typeName == "int16") {
        ret = MARTe::SignedInteger16Bit;
    }
    else if (typeName == "int32") {
        ret = MARTe::SignedInteger32Bit;
    }
    else if (typeName == "int64") {
        ret = MARTe::SignedInteger64Bit;
    }
    else if (typeName == "float32") {
        ret = MARTe::Float32Bit;
    }
    else if (typeName == "float64") {
        ret = MARTe::Float64Bit;
    }
    else if (typeName == "string") {
        ret = MARTe::CharString;
    }
    else if (typeName == "char8") {
        ret = MARTe::Character8Bit;
    }
    else if (typeName == "bool") {
        ret = MARTe::BooleanType;
    }
    else {

    }

    return ret;
}

ccs::types::AnyType* GetCCSBasicType(MARTe::TypeDescriptor td,
                                     ccs::types::uint32 numberOfDimensions,
                                     ccs::types::uint32 numberOfElements) {

    ccs::types::AnyType *ret = NULL;
    //resolve the type
    if (td == MARTe::UnsignedInteger8Bit) {
        //vector type
        if (numberOfDimensions > 0u) {
            //can always set a name later
            ret = (new ccs::types::ArrayType("Uint8Array", ccs::types::UnsignedInteger8, numberOfElements));
        }
        else {
            ret = (new ccs::types::ScalarTypeT<ccs::types::uint8>);
        }
    }
    else if (td == MARTe::UnsignedInteger16Bit) {
        if (numberOfDimensions > 0u) {
            ret = (new ccs::types::ArrayType("Uint16Array", ccs::types::UnsignedInteger16, numberOfElements));
        }
        else {
            ret = (new ccs::types::ScalarTypeT<ccs::types::uint16>);
        }
    }
    else if (td == MARTe::UnsignedInteger32Bit) {
        if (numberOfDimensions > 0u) {
            ret = (new ccs::types::ArrayType("Uint32Array", ccs::types::UnsignedInteger32, numberOfElements));
        }
        else {
            ret = (new ccs::types::ScalarTypeT<ccs::types::uint32>);
        }
    }
    else if (td == MARTe::UnsignedInteger64Bit) {
        if (numberOfDimensions > 0u) {
            ret = (new ccs::types::ArrayType("Uint64Array", ccs::types::UnsignedInteger64, numberOfElements));
        }
        else {
            ret = (new ccs::types::ScalarTypeT<ccs::types::uint64>);
        }
    }
    else if (td == MARTe::SignedInteger8Bit) {
        if (numberOfDimensions > 0u) {
            ret = (new ccs::types::ArrayType("Int8Array", ccs::types::SignedInteger8, numberOfElements));
        }
        else {
            ret = (new ccs::types::ScalarTypeT<ccs::types::int8>);
        }
    }
    else if (td == MARTe::SignedInteger16Bit) {
        if (numberOfDimensions > 0u) {
            ret = (new ccs::types::ArrayType("Int16Array", ccs::types::SignedInteger16, numberOfElements));
        }
        else {
            ret = (new ccs::types::ScalarTypeT<ccs::types::int16>);
        }
    }
    else if (td == MARTe::SignedInteger32Bit) {
        if (numberOfDimensions > 0u) {
            ret = (new ccs::types::ArrayType("Int32Array", ccs::types::SignedInteger32, numberOfElements));
        }
        else {
            ret = (new ccs::types::ScalarTypeT<ccs::types::int32>);
        }
    }
    else if (td == MARTe::SignedInteger64Bit) {
        if (numberOfDimensions > 0u) {
            ret = (new ccs::types::ArrayType("Int64Array", ccs::types::SignedInteger64, numberOfElements));
        }
        else {
            ret = (new ccs::types::ScalarTypeT<ccs::types::int64>);
        }
    }
    else if (td == MARTe::Float32Bit) {
        if (numberOfDimensions > 0u) {
            ret = (new ccs::types::ArrayType("Float32Array", ccs::types::Float32, numberOfElements));
        }
        else {
            ret = (new ccs::types::ScalarTypeT<ccs::types::float32>);
        }
    }
    else if (td == MARTe::Float64Bit) {
        if (numberOfDimensions > 0u) {
            ret = (new ccs::types::ArrayType("FloatArray", ccs::types::Float64, numberOfElements));
        }
        else {
            ret = (new ccs::types::ScalarTypeT<ccs::types::float64>);
        }
    }
    else if (td == MARTe::Character8Bit) {
        if (numberOfDimensions > 0u) {
            ret = (new ccs::types::ArrayType("Char8Array", ccs::types::Character8, numberOfElements));
        }
        else {
            ret = (new ccs::types::ScalarTypeT<ccs::types::char8>);
        }
    }
    else if (td == MARTe::CharString) {
        if (numberOfDimensions > 0u) {
            ret = (new ccs::types::ArrayType("StringArray", ccs::types::String, numberOfElements));
        }
        else {
            ret = (new ccs::types::ScalarTypeT<ccs::types::string>);
        }
    }
    else if (td == MARTe::BooleanType) {
        if (numberOfDimensions > 0u) {
            ret = (new ccs::types::ArrayType("BooleanArray", ccs::types::Boolean, numberOfElements));
        }
        else {
            ret = (new ccs::types::ScalarTypeT<ccs::types::boolean>);
        }
    }
    else if (td == MARTe::StructuredDataInterfaceType) {
        //treat this differently!! (as a structured data actually)
        std::string sdiTypeName = "StructuredDataI";
        ret = (new ccs::types::CompoundType(sdiTypeName.c_str()));
    }

    return ret;
}

bool CCSToMARTeStructuredDataI(MARTe::StructuredDataI &valueOut,
                               const ccs::types::AnyValue &valueIn) {
    //need to create the Introspection from the type before (using cdb and IntrospectionStructure)
    ccs::base::SharedReference<const ccs::types::CompoundType> typeIn = valueIn.GetType();
    bool ret = typeIn.IsValid();
    ccs::types::uint8 *valMem = reinterpret_cast<ccs::types::uint8*>(valueIn.GetInstance());

    //loop on the attributes. The structure can be packed or not on MARTe side
    if (ret) {
        ccs::types::uint32 numberOfAttributes = typeIn->GetAttributeNumber();

        for (ccs::types::uint32 i = 0u; (i < numberOfAttributes) && (ret); i++) {
            const ccs::types::char8 *memberName = typeIn->GetAttributeName(i);
            ccs::types::uint32 offset = typeIn->GetAttributeOffset(i);
            ccs::base::SharedReference<const ccs::types::AnyType> memberType = typeIn->GetAttributeType(i);
            ccs::base::SharedReference<const ccs::types::CompoundType> nodeType = memberType;
            ccs::base::SharedReference<const ccs::types::ArrayType> arrType(memberType);
            if(arrType.IsValid()){
                ccs::types::uint32 size = typeIn->GetAttributeSize(i);
                MARTe::TypeDescriptor td = GetMARTeBasicType(arrType->GetElementType());
                uint32 numberOfElements = arrType->GetMultiplicity();
                uint32 nElements1 = 0u;
                if (td == MARTe::CharString) { //Array of strings. For string array
                    nElements1 = numberOfElements;
                }
                bool isStringArray = IsStringArray(td, 1u, nElements1);
                if (isStringArray) {
                    Vector<StreamString> destMem(numberOfElements);
                    for (uint32 index=0; index<numberOfElements; index++) {
                        const char8 *src = static_cast<const char8 *>(arrType->GetElementReference(&valMem[offset], index));
                        destMem[index] = src;
                    }
                    MARTe::AnyType destType(destMem);
                    //it serialises the AnyType inside so we are free to destroy the mem
                    valueOut.Write(memberName, destType);
                }
                else {
                    ccs::types::uint8 *destMem = new ccs::types::uint8[size];
                    ret = MemoryOperationsHelper::Copy(destMem, &valMem[offset], size);
                    MARTe::AnyType destType(td, 0u, destMem);
                    destType.SetNumberOfDimensions(1u);
                    destType.SetNumberOfElements(0u, numberOfElements);
                    //it serialises the AnyType inside so we are free to destroy the mem
                    valueOut.Write(memberName, destType);
                    delete[] destMem;
                }
            }
            else if (nodeType) {
                valueOut.CreateRelative(memberName);
                ccs::types::AnyValue childVal(memberType, &valMem[offset]);
                //go recursive on the node
                ret = CCSToMARTeStructuredDataI(valueOut, childVal);
                valueOut.MoveToAncestor(1u);
            }
            else {
                ccs::types::uint32 size = typeIn->GetAttributeSize(i);
                MARTe::TypeDescriptor td;
                ccs::base::SharedReference<const ccs::types::ScalarType> scalarType(memberType);
                td = GetMARTeBasicType(scalarType);

                ccs::types::uint8 *destMem = new ccs::types::uint8[size];
                ret = MemoryOperationsHelper::Copy(destMem, &valMem[offset], size);
                MARTe::AnyType destType(td, 0u, destMem);
                //it serialises the AnyType inside so we are free to destroy the mem
                valueOut.Write(memberName, destType);
                delete[] destMem;
            }
        }
    }
    return ret;
}

bool CCSToMARTeAnyObject(MARTe::AnyObject &valueOut,
                         const ccs::types::AnyValue &valueIn) {
    ccs::base::SharedReference<const ccs::types::AnyType> typeIn(valueIn.GetType());
    ccs::base::SharedReference<const ccs::types::ScalarType> typeScalar = typeIn;
    bool isScalar = typeScalar.IsValid();
    bool ret = true;
    if (isScalar) {
        //basic scalar type
        ret = CCSToMARTeScalarType(valueOut, valueIn);
    }
    else {
        ccs::base::SharedReference<const ccs::types::ArrayType> typeArray = typeIn;
        if (typeArray.IsValid()) {
            ccs::base::SharedReference<const ccs::types::AnyType> innerType(typeArray->GetElementType());
            ccs::base::SharedReference<const ccs::types::ArrayType> inTypeArray = innerType;
            ret = (!inTypeArray.IsValid());
            if (ret) {
                ccs::base::SharedReference<const ccs::types::ScalarType> inTypeScalar = innerType;
                if (inTypeScalar.IsValid()) {
                    ccs::types::uint32 numberOfElements = typeArray->GetMultiplicity();
                    ret = CCSToMARTeScalarType(valueOut, valueIn, 1u, numberOfElements);
                }
                else {
                    REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "Array of structures not supported");
                }
            }
            else {
                REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "Array of array not supported");
            }
        }
        else {
            ret = CCSToMARTeStructuredType(valueOut, valueIn);
        }
    }
    return ret;
}
}
}
