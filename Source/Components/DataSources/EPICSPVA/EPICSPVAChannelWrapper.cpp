/**
 * @file EPICSPVAChannelWrapper.cpp
 * @brief Source file for class EPICSPVAChannelWrapper
 * @date 21/10/2018
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
 * the class EPICSPVAChannelWrapper (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <pva/client.h>
#include <pv/pvData.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "EPICSPVAChannelWrapper.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {
EPICSPVAChannelWrapper::EPICSPVAChannelWrapper() {
    (void) memoryBackend.MoveToRoot();
    numberOfSignals = 0u;
    numberOfRequestedSignals = 0u;
    cachedSignals = NULL_PTR(EPICSPVAChannelWrapperCachedSignal *);
}

EPICSPVAChannelWrapper::~EPICSPVAChannelWrapper() {
    if (cachedSignals != NULL_PTR(EPICSPVAChannelWrapperCachedSignal *)) {
        delete[] cachedSignals;
    }
    channel = pvac::ClientChannel();
    monitor = pvac::MonitorSync();
}

bool EPICSPVAChannelWrapper::Setup(StructuredDataI &data) {
    bool ok = true;
    unliasedChannelName = data.GetName();
    if (data.Read("Alias", channelName)) {
        ok = data.Delete("Alias");
    }
    else {
        channelName = unliasedChannelName.Buffer();
    }
    REPORT_ERROR_STATIC(ErrorManagement::Information, "Registering channel %s", channelName.Buffer());
    if (ok) {
        ok = LoadSignalStructure(data, "", "");
    }
    return ok;
}

bool EPICSPVAChannelWrapper::LoadBasicType(TypeDescriptor &td, uint32 numberOfElements, uint32 numberOfDimensions,
                                           StreamString fullNodeName, StreamString relativeNodeName) {
    uint32 memorySize = td.numberOfBits;
    bool ok = (numberOfDimensions < 2u);
    if (ok) {
        if (numberOfElements == 0u) {
            numberOfElements = 1u;
        }
        if (numberOfElements > 1u) {
            numberOfDimensions = 1u;
        }
        memorySize *= numberOfElements;
        memorySize /= 8u;
    }
    else {
        REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "Only 0 and 1 dimension signals are currently supported");
    }
    if (ok) {
        const char8 * const typeName = TypeDescriptor::GetTypeNameFromTypeDescriptor(td);
        REPORT_ERROR_STATIC(ErrorManagement::Information, "Registering signal %s of type %s [%d bytes]", fullNodeName.Buffer(), typeName,
                            memorySize);
        void *mem = GlobalObjectsDatabase::Instance()->GetStandardHeap()->Malloc(memorySize);
        AnyType at(td, 0u, mem);
        at.SetNumberOfElements(0u, numberOfElements);
        at.SetNumberOfDimensions(numberOfDimensions);
        ok = memoryBackend.Write(relativeNodeName.Buffer(), at);
        if (ok) {
            numberOfSignals++;
        }
        GlobalObjectsDatabase::Instance()->GetStandardHeap()->Free(mem);
    }
    return ok;
}

bool EPICSPVAChannelWrapper::LoadStructuredType(const char8 * const typeName, StreamString fullNodeName, StreamString relativeNodeName,
                                                const IntrospectionEntry *entry) {
    bool ok = true;
    TypeDescriptor td = TypeDescriptor::GetTypeDescriptorFromTypeName(typeName);
    if (td == voidAnyType.GetTypeDescriptor()) {
        ClassRegistryItem *cri = ClassRegistryDatabase::Instance()->Find(typeName);
        bool ok = (cri != NULL_PTR(ClassRegistryItem *));
        if (ok) {
            const Introspection *intro = cri->GetIntrospection();
            IntrospectionEntry introEntry;
            ok = (intro != NULL_PTR(const Introspection *));
            if (ok) {
                uint32 i;
                uint32 nOfMembers = intro->GetNumberOfMembers();
                for (i = 0u; (i < nOfMembers) && (ok); i++) {
                    introEntry = (intro->operator [](i));
                    relativeNodeName = introEntry.GetMemberName();
                    ok = memoryBackend.CreateRelative(relativeNodeName.Buffer());
                    if (ok) {
                        StreamString fullBranchNodeName = fullNodeName;
                        if (fullBranchNodeName.Size() != 0u) {
                            fullBranchNodeName += ".";
                            fullBranchNodeName += relativeNodeName;
                        }
                        else {
                            fullBranchNodeName = relativeNodeName;
                        }
                        ok = LoadStructuredType(introEntry.GetMemberTypeName(), fullBranchNodeName.Buffer(), relativeNodeName.Buffer(),
                                                &introEntry);
                    }
                    if (ok) {
                        ok = memoryBackend.MoveToAncestor(1u);
                    }
                }

            }
            else {
                REPORT_ERROR_STATIC(ErrorManagement::Information, "Type %s has no introspection", typeName);
            }
        }
        else {
            REPORT_ERROR_STATIC(ErrorManagement::Information, "Type %s not found", typeName);
        }
    }
    else {
        if (entry != NULL_PTR(const IntrospectionEntry *)) {
            uint32 numberOfDimensions = entry->GetNumberOfDimensions();
            uint32 numberOfElements = entry->GetNumberOfElements(0u);
            ok = LoadBasicType(td, numberOfElements, numberOfDimensions, fullNodeName.Buffer(), relativeNodeName.Buffer());
        }
    }
    return ok;
}

bool EPICSPVAChannelWrapper::LoadSignalStructure(StructuredDataI &data, StreamString fullNodeName, StreamString relativeNodeName) {
    bool ok = true;

    StreamString typeName;
    StreamString ignore;
    //If it is already a signal it shall have the type defined
    bool isSignal = data.Read("Type", typeName);
    if (isSignal) {
        TypeDescriptor td = TypeDescriptor::GetTypeDescriptorFromTypeName(typeName.Buffer());
        if (td == voidAnyType.GetTypeDescriptor()) {
            ok = LoadStructuredType(typeName.Buffer(), fullNodeName.Buffer(), relativeNodeName.Buffer(),
                                    NULL_PTR(const IntrospectionEntry *));
        }
        else {
            uint32 numberOfDimensions = 0u;
            uint32 numberOfElements = 1u;
            (void) data.Read("NumberOfDimensions", numberOfDimensions);
            (void) data.Read("NumberOfElements", numberOfElements);
            ok = LoadBasicType(td, numberOfElements, numberOfDimensions, fullNodeName.Buffer(), relativeNodeName.Buffer());
        }
    }
    else {
        //If it is not a signal, must be a node
        uint32 nOfChildren = data.GetNumberOfChildren();
        ok = (nOfChildren > 0u);
        if (!ok) {
            REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "Found an invalid leaf which is not a signal");
        }
        uint32 n;
        for (n = 0u; (n < nOfChildren) && (ok); n++) {
            relativeNodeName = data.GetChildName(n);
            StreamString fullBranchNodeName = fullNodeName;
            if (fullBranchNodeName.Size() != 0u) {
                fullBranchNodeName += ".";
                fullBranchNodeName += relativeNodeName;
            }
            else {
                fullBranchNodeName = relativeNodeName;
            }
            if (data.MoveRelative(relativeNodeName.Buffer())) {
                ok = memoryBackend.CreateRelative(relativeNodeName.Buffer());
                if (ok) {
                    ok = LoadSignalStructure(data, fullBranchNodeName.Buffer(), relativeNodeName.Buffer());
                }
                if (ok) {
                    ok = data.MoveToAncestor(1u);
                }
                if (ok) {
                    ok = memoryBackend.MoveToAncestor(1u);
                }
            }
        }
    }
    return ok;
}

/*lint -e{613} cachedSignal cannot be NULL (see first if)*/
void EPICSPVAChannelWrapper::GetSignalMemory(const char8 * const qualifiedName, void *&mem) {
    if (cachedSignals == NULL_PTR(EPICSPVAChannelWrapperCachedSignal *)) {
        if (numberOfSignals > 0u) {
            cachedSignals = new EPICSPVAChannelWrapperCachedSignal[numberOfSignals];
        }
    }
    bool alreadyAdded = false;
    uint32 n;
    for (n = 0u; (n < numberOfRequestedSignals) && (!alreadyAdded); n++) {
        alreadyAdded = (cachedSignals[n].qualifiedName == qualifiedName);
        mem = cachedSignals[n].memory;
    }
    if (!alreadyAdded) {
        mem = NULL_PTR(void *);
        StreamString qualifiedNameStr = qualifiedName;
        StreamString token;
        StreamString qualifiedLeafName;
        bool ok = memoryBackend.MoveToRoot();
        if (ok) {
            ok = qualifiedNameStr.Seek(0LLU);
        }
        if (ok) {
            char8 ignore;
            while (qualifiedNameStr.GetToken(token, ".", ignore)) {
                (void) memoryBackend.MoveRelative(token.Buffer());
                qualifiedLeafName = token;
                token = "";
            }
        }

        AnyType storedType = memoryBackend.GetType(qualifiedLeafName.Buffer());
        if (storedType.GetTypeDescriptor() == voidAnyType.GetTypeDescriptor()) {
            REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "Signal with qualified name %s not found!", qualifiedName);
        }
        else {
            mem = storedType.GetDataPointer();
            if (numberOfRequestedSignals < numberOfSignals) {
                uint32 nOfElements = 1u;
                uint32 nOfDimensions = storedType.GetNumberOfDimensions();
                uint32 n;
                for (n = 0u; n < nOfDimensions; n++) {
                    nOfElements *= storedType.GetNumberOfElements(n);
                }
                cachedSignals[numberOfRequestedSignals].qualifiedName = qualifiedName;
                cachedSignals[numberOfRequestedSignals].typeDescriptor = storedType.GetTypeDescriptor();
                cachedSignals[numberOfRequestedSignals].memory = mem;
                cachedSignals[numberOfRequestedSignals].numberOfElements = nOfElements;
                numberOfRequestedSignals++;
            }
        }
    }
}

bool EPICSPVAChannelWrapper::Put() {
    bool ok = false;
    try {
        if (!channel.valid()) {
            provider = pvac::ClientProvider("pva");
            REPORT_ERROR_STATIC(ErrorManagement::Information, "Connected to channel %s", channelName.Buffer());
            channel = pvac::ClientChannel(provider.connect(channelName.Buffer()));
        }
        ok = channel.valid();
        if (ok) {
            uint32 n;
            pvac::detail::PutBuilder putBuilder = channel.put();
            for (n = 0u; n < numberOfRequestedSignals; n++) {
                if (cachedSignals[n].typeDescriptor == UnsignedInteger8Bit) {
                    PutHelper<uint8>(putBuilder, n);
                }
                else if (cachedSignals[n].typeDescriptor == UnsignedInteger16Bit) {
                    PutHelper<uint16>(putBuilder, n);
                }
                else if (cachedSignals[n].typeDescriptor == UnsignedInteger32Bit) {
                    PutHelper<uint32>(putBuilder, n);
                }
                else if (cachedSignals[n].typeDescriptor == UnsignedInteger64Bit) {
                    PutHelper<unsigned long int>(putBuilder, n);
                }
                else if (cachedSignals[n].typeDescriptor == SignedInteger8Bit) {
                    PutHelper<int8>(putBuilder, n);
                }
                else if (cachedSignals[n].typeDescriptor == SignedInteger16Bit) {
                    PutHelper<int16>(putBuilder, n);
                }
                else if (cachedSignals[n].typeDescriptor == SignedInteger32Bit) {
                    PutHelper<int32>(putBuilder, n);
                }
                else if (cachedSignals[n].typeDescriptor == SignedInteger64Bit) {
                    PutHelper<long int>(putBuilder, n);
                }
                else if (cachedSignals[n].typeDescriptor == Float32Bit) {
                    PutHelper<float32>(putBuilder, n);
                }
                else if (cachedSignals[n].typeDescriptor == Float64Bit) {
                    PutHelper<float64>(putBuilder, n);
                }
#if 0
                //This code compiles and should work BUT I have no way of testing this, since the builder does not support signals which are strings.
                else if ((cachedSignals[n].typeDescriptor.type == CArray) || (cachedSignals[n].typeDescriptor.type == BT_CCString)
                        || (cachedSignals[n].typeDescriptor.type == PCString) || (cachedSignals[n].typeDescriptor.type == SString)) {
                    if ((cachedSignals[n].numberOfElements) == 0u) {
                        if (cachedSignals[n].typeDescriptor.type == SString) {
                            StreamString *src = static_cast<StreamString *>(cachedSignals[n].memory);
                            putBuilder.set(cachedSignals[n].qualifiedName.Buffer(), std::string(src->Buffer()));
                        }
                        else {
                            char8 *src = static_cast<char8 *>(cachedSignals[n].memory);
                            putBuilder.set(cachedSignals[n].qualifiedName.Buffer(), std::string(src));
                        }
                    }
                    else {
                        epics::pvData::shared_vector<const std::string> out;
                        out.resize(cachedSignals[n].numberOfElements);
                        if (cachedSignals[n].typeDescriptor.type == SString) {
                            StreamString *src = static_cast<StreamString *>(cachedSignals[n].memory);
                            uint32 i;
                            for (i = 0; i < cachedSignals[n].numberOfElements; i++) {
                                *const_cast<std::string *>(reinterpret_cast<const std::string *>(&out[i])) = src[i].Buffer();
                            }
                        }
                        else {
                            const char8 **src = static_cast<const char8 **>(cachedSignals[n].memory);
                            uint32 i;
                            for (i = 0; i < cachedSignals[n].numberOfElements; i++) {
                                *const_cast<std::string *>(reinterpret_cast<const std::string *>(&out[i])) = src[i];
                            }
                        }
                        putBuilder.set(cachedSignals[n].qualifiedName.Buffer(), out);
                    }
                }
#endif
                else {
                    //Should never reach here...
                    REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "Unsupported type");
                    ok = false;
                }
            }
            putBuilder.exec();
        }
    }
    catch (epics::pvData::detail::ExceptionMixed<epics::pvData::BaseException> &ignored) {
        REPORT_ERROR_STATIC(ErrorManagement::Information, "Failed to connect to channel %s [s]", channelName.Buffer(), ignored.what());
        ok = false;
    }
    return ok;
}

bool EPICSPVAChannelWrapper::Monitor() {
    bool ok = false;
    try {
        if (!channel.valid()) {
            provider = pvac::ClientProvider("pva");
            REPORT_ERROR_STATIC(ErrorManagement::Information, "Connected to channel %s", channelName.Buffer());
            channel = pvac::ClientChannel(provider.connect(channelName.Buffer()));
        }
        ok = channel.valid();
        if (ok) {
            if (!monitor.valid()) {
                monitor = pvac::MonitorSync(channel.monitor());
            }
            ok = monitor.valid();
        }
        if (ok) {
            if (monitor.wait(0.2)) {
                if (monitor.event.event == pvac::MonitorEvent::Data) {
                    while (monitor.poll()) {
                        uint32 n;
                        for (n = 0u; (n < numberOfRequestedSignals) && (ok); n++) {
                            epics::pvData::PVStructure::const_shared_pointer readStruct = monitor.root;
                            epics::pvData::PVScalar::const_shared_pointer scalarFieldPtr;
                            epics::pvData::PVScalarArray::const_shared_pointer scalarArrayPtr;
                            if ((cachedSignals[n].numberOfElements) == 1u) {
                                scalarFieldPtr = std::dynamic_pointer_cast<const epics::pvData::PVScalar>(
                                        readStruct->getSubField(cachedSignals[n].qualifiedName.Buffer()));
                                ok = (scalarFieldPtr ? true : false);
                                if (ok) {
                                    if (cachedSignals[n].typeDescriptor == UnsignedInteger8Bit) {
                                        *reinterpret_cast<uint8 *>(cachedSignals[n].memory) = scalarFieldPtr->getAs<uint8>();
                                    }
                                    else if (cachedSignals[n].typeDescriptor == UnsignedInteger16Bit) {
                                        *reinterpret_cast<uint16 *>(cachedSignals[n].memory) = scalarFieldPtr->getAs<uint16>();
                                    }
                                    else if (cachedSignals[n].typeDescriptor == UnsignedInteger32Bit) {
                                        *reinterpret_cast<uint32 *>(cachedSignals[n].memory) = scalarFieldPtr->getAs<uint32>();
                                    }
                                    else if (cachedSignals[n].typeDescriptor == UnsignedInteger64Bit) {
                                        *reinterpret_cast<uint64 *>(cachedSignals[n].memory) = scalarFieldPtr->getAs<long unsigned int>();
                                    }
                                    else if (cachedSignals[n].typeDescriptor == SignedInteger8Bit) {
                                        *reinterpret_cast<int8 *>(cachedSignals[n].memory) = scalarFieldPtr->getAs<int8>();
                                    }
                                    else if (cachedSignals[n].typeDescriptor == SignedInteger16Bit) {
                                        *reinterpret_cast<int16 *>(cachedSignals[n].memory) = scalarFieldPtr->getAs<int16>();
                                    }
                                    else if (cachedSignals[n].typeDescriptor == SignedInteger32Bit) {
                                        *reinterpret_cast<int32 *>(cachedSignals[n].memory) = scalarFieldPtr->getAs<int32>();
                                    }
                                    else if (cachedSignals[n].typeDescriptor == SignedInteger64Bit) {
                                        *reinterpret_cast<int64 *>(cachedSignals[n].memory) = scalarFieldPtr->getAs<long int>();
                                    }
                                    else if (cachedSignals[n].typeDescriptor == Float32Bit) {
                                        *reinterpret_cast<float32 *>(cachedSignals[n].memory) = scalarFieldPtr->getAs<float32>();
                                    }
                                    else if (cachedSignals[n].typeDescriptor == Float64Bit) {
                                        *reinterpret_cast<float64 *>(cachedSignals[n].memory) = scalarFieldPtr->getAs<float64>();
                                    }
                                    else {
                                        REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "Unsupported read type");
                                        ok = false;
                                    }
                                }
                            }
                            else {
                                scalarArrayPtr = std::dynamic_pointer_cast<const epics::pvData::PVScalarArray>(
                                        readStruct->getSubField(cachedSignals[n].qualifiedName.Buffer()));
                                ok = (scalarArrayPtr ? true : false);
                                if (ok) {
                                    if (cachedSignals[n].typeDescriptor == UnsignedInteger8Bit) {
                                        ok = GetArrayHelper<uint8>(scalarArrayPtr, n);
                                    }
                                    else if (cachedSignals[n].typeDescriptor == UnsignedInteger16Bit) {
                                        ok = GetArrayHelper<uint16>(scalarArrayPtr, n);
                                    }
                                    else if (cachedSignals[n].typeDescriptor == UnsignedInteger32Bit) {
                                        ok = GetArrayHelper<uint32>(scalarArrayPtr, n);
                                    }
                                    else if (cachedSignals[n].typeDescriptor == UnsignedInteger64Bit) {
                                        ok = GetArrayHelper<unsigned long int>(scalarArrayPtr, n);
                                    }
                                    else if (cachedSignals[n].typeDescriptor == SignedInteger8Bit) {
                                        ok = GetArrayHelper<int8>(scalarArrayPtr, n);
                                    }
                                    else if (cachedSignals[n].typeDescriptor == SignedInteger16Bit) {
                                        ok = GetArrayHelper<uint16>(scalarArrayPtr, n);
                                    }
                                    else if (cachedSignals[n].typeDescriptor == SignedInteger32Bit) {
                                        ok = GetArrayHelper<int32>(scalarArrayPtr, n);
                                    }
                                    else if (cachedSignals[n].typeDescriptor == SignedInteger64Bit) {
                                        ok = GetArrayHelper<long int>(scalarArrayPtr, n);
                                    }
                                    else if (cachedSignals[n].typeDescriptor == Float32Bit) {
                                        ok = GetArrayHelper<float32>(scalarArrayPtr, n);
                                    }
                                    else if (cachedSignals[n].typeDescriptor == Float64Bit) {
                                        ok = GetArrayHelper<float64>(scalarArrayPtr, n);
                                    }
                                    else {
                                        REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "Unsupported read array type");
                                        ok = false;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    catch (epics::pvData::detail::ExceptionMixed<epics::pvData::BaseException> &ignored) {
        REPORT_ERROR_STATIC(ErrorManagement::Information, "Failed to connect to channel %s [s]", channelName.Buffer(), ignored.what());
        ok = false;
    }
    return ok;
}

const char8 * const EPICSPVAChannelWrapper::GetChannelName() {
    return channelName.Buffer();
}

const char8 * const EPICSPVAChannelWrapper::GetChannelUnaliasedName() {
    return unliasedChannelName.Buffer();
}

}

