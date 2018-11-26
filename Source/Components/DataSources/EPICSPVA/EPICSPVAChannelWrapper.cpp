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
    numberOfSignals = 0u;
    cachedSignals = NULL_PTR(EPICSPVAChannelWrapperCachedSignal *);
    structureResolved = false;
    putFinished = false;
}

EPICSPVAChannelWrapper::~EPICSPVAChannelWrapper() {
    if (cachedSignals != NULL_PTR(EPICSPVAChannelWrapperCachedSignal *)) {
        delete[] cachedSignals;
    }
    channel = pvac::ClientChannel();
    monitor = pvac::MonitorSync();
}

bool EPICSPVAChannelWrapper::SetAliasAndField(StructuredDataI &data) {
    bool ok = true;
    if (data.Read("Alias", channelName)) {
        ok = data.Delete("Alias");
    }
    else {
        channelName = data.GetName();
    }
    if (data.Read("Field", fieldName)) {
        ok = data.Delete("Field");
    }
    else {
        fieldName = "value";
    }
    originalName = data.GetName();
    REPORT_ERROR_STATIC(ErrorManagement::Information, "Registering channel %s [%s]", channelName.Buffer(), fieldName.Buffer());
    return ok;
}

bool EPICSPVAChannelWrapper::Setup(DataSourceI &dataSource) {
    bool ok = true;
    uint32 maxNumberOfSignals = dataSource.GetNumberOfSignals();
    EPICSPVAChannelWrapperCachedSignal *tempCachedSignals = new EPICSPVAChannelWrapperCachedSignal[maxNumberOfSignals];
    numberOfSignals = 0u;
    uint32 n;
    for (n = 0u; (n < maxNumberOfSignals) && (ok); n++) {
        StreamString signalName;
        ok = dataSource.GetSignalName(n, signalName);
        if (ok) {
            ok = signalName.Seek(0LLU);
        }
        bool signalBelongsToThisWrapper = false;
        char8 term;
        if (ok) {
            //Remove the first part of the signal (which is also the record name). Take into account that it could be an array!
            StreamString token;
            ok = signalName.GetToken(token, ".[", term);
            if (ok) {
                ok = token.Seek(0LLU);
            }
            if (ok) {
                signalBelongsToThisWrapper = (token == originalName);
            }
        }
        if (signalBelongsToThisWrapper) {
            if (ok) {
                tempCachedSignals[numberOfSignals].qualifiedName = "";
                if (signalName.Position() == signalName.Size()) {
                    ok = tempCachedSignals[numberOfSignals].qualifiedName.Printf("%s", fieldName.Buffer());
                }
                else {
                    if (term == '.') {
                        ok = tempCachedSignals[numberOfSignals].qualifiedName.Printf("%s.%s", fieldName.Buffer(), signalName);
                    }
                    else {
                        ok = tempCachedSignals[numberOfSignals].qualifiedName.Printf("%s[%s", fieldName.Buffer(), signalName);
                    }
                }
            }
            if (ok) {
                tempCachedSignals[numberOfSignals].typeDescriptor = dataSource.GetSignalType(n);
                ok = dataSource.GetSignalNumberOfElements(n, tempCachedSignals[numberOfSignals].numberOfElements);
            }
            if (ok) {
                ok = dataSource.GetSignalMemoryBuffer(n, 0u, tempCachedSignals[numberOfSignals].memory);
            }
            REPORT_ERROR_STATIC(ErrorManagement::Information, "Registering signal %s [%s]",
                                tempCachedSignals[numberOfSignals].qualifiedName.Buffer(), channelName.Buffer());
            numberOfSignals++;
        }
    }
    if (ok) {
        if (cachedSignals != NULL_PTR(EPICSPVAChannelWrapperCachedSignal *)) {
            delete[] cachedSignals;
        }
        if (numberOfSignals > 0u) {
            cachedSignals = new EPICSPVAChannelWrapperCachedSignal[numberOfSignals];
            for (n = 0u; (n < numberOfSignals); n++) {
                cachedSignals[n] = tempCachedSignals[n];
            }
        }
        if (tempCachedSignals != NULL_PTR(EPICSPVAChannelWrapperCachedSignal *)) {
            delete[] tempCachedSignals;
        }
    }

    return ok;
}

void EPICSPVAChannelWrapper::putBuild(const epics::pvData::StructureConstPtr& build, pvac::ClientChannel::PutCallback::Args& args) {
    args.root = putPVStruct;
    args.tosend.set(0);
    putFinished = true;
}

void EPICSPVAChannelWrapper::putDone(const pvac::PutEvent& evt) {
    if (evt.event == 0) {
        std::cout << evt.event << std::endl;
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
            if (!structureResolved) {
                epics::pvData::PVStructure::const_shared_pointer getPVStruct = channel.get();
                ok = (getPVStruct) ? true : false;
                if (ok) {
                    epics::pvData::FieldCreatePtr fieldCreate = epics::pvData::getFieldCreate();
                    epics::pvData::FieldBuilderPtr fieldBuilder = fieldCreate->createFieldBuilder();
                    epics::pvData::StructureConstPtr topStructure = fieldBuilder->createStructure();
                    putPVStruct = epics::pvData::getPVDataCreate()->createPVStructure(getPVStruct->getStructure());
                    ok = ResolveStructure(putPVStruct.operator ->(), "");
                    structureResolved = ok;
                }
            }
        }
        if (ok) {
            uint32 n;
            for (n = 0u; n < numberOfSignals; n++) {
                if (cachedSignals[n].typeDescriptor == UnsignedInteger8Bit) {
                    PutHelper<uint8>(n);
                }
                else if (cachedSignals[n].typeDescriptor == UnsignedInteger16Bit) {
                    PutHelper<uint16>(n);
                }
                else if (cachedSignals[n].typeDescriptor == UnsignedInteger32Bit) {
                    PutHelper<uint32>(n);
                }
                else if (cachedSignals[n].typeDescriptor == UnsignedInteger64Bit) {
                    PutHelper<unsigned long int>(n);
                }
                else if (cachedSignals[n].typeDescriptor == SignedInteger8Bit) {
                    PutHelper<int8>(n);
                }
                else if (cachedSignals[n].typeDescriptor == SignedInteger16Bit) {
                    PutHelper<int16>(n);
                }
                else if (cachedSignals[n].typeDescriptor == SignedInteger32Bit) {
                    PutHelper<int32>(n);
                }
                else if (cachedSignals[n].typeDescriptor == SignedInteger64Bit) {
                    PutHelper<long int>(n);
                }
                else if (cachedSignals[n].typeDescriptor == Float32Bit) {
                    PutHelper<float32>(n);
                }
                else if (cachedSignals[n].typeDescriptor == Float64Bit) {
                    PutHelper<float64>(n);
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
        }
        putFinished = false;
        pvac::Operation op(channel.put(this));
        uint32 timeout = 10u;
        while (ok && (!putFinished)) {
            Sleep::Sec(0.1);
            timeout--;
            ok = (timeout > 0u);
        }
    }
    catch (epics::pvData::detail::ExceptionMixed<epics::pvData::BaseException> &ignored) {
        REPORT_ERROR_STATIC(ErrorManagement::Information, "Failed to connect to channel %s [s]", channelName.Buffer(), ignored.what());
        ok = false;
    }
    return ok;
}

bool EPICSPVAChannelWrapper::ResolveStructure(const epics::pvData::PVStructure* pvStruct, const char8 * const nodeName, int32 idx) {
    bool ok = (pvStruct != NULL_PTR(const epics::pvData::PVStructure*));
    if (ok) {
        const epics::pvData::PVFieldPtrArray & fields = pvStruct->getPVFields();
        uint32 nOfFields = fields.size();
        uint32 n;

        for (n = 0u; (n < nOfFields) && (ok); n++) {
            epics::pvData::PVFieldPtr field = fields[n];
            epics::pvData::Type fieldType = field->getField()->getType();
            REPORT_ERROR_STATIC(ErrorManagement::Debug, "ResolveStructure -- fields [%d of %d] -- [%s]", n, nOfFields, field->getFieldName().c_str());


            if (fieldType == epics::pvData::structureArray) {
                epics::pvData::PVStructureArray::const_svector arr(
                        static_cast<const epics::pvData::PVStructureArray*>(field.operator ->())->view());
                uint32 z;
                REPORT_ERROR_STATIC(ErrorManagement::Debug, "Resolving structureArray [%s - %s] - [%d]", nodeName, field->getFieldName().c_str(), static_cast<int32>(arr.size()));
                for (z = 0u; z < arr.size(); z++) {
                    //This assumes that only linear arrays are supported, otherwise the field name will be wrong.
                    ok = ResolveStructure(arr[z].get(), nodeName, static_cast<int32>(z));
                }
            }
            else {
                StreamString fullFieldName = nodeName;
                if (fullFieldName.Size() > 0u) {
                    fullFieldName += ".";
                }
                fullFieldName += field->getFieldName().c_str();
                if (idx != -1) {
                    fullFieldName.Printf("[%d]", idx);
                }
                if ((fieldType == epics::pvData::scalar) || (fieldType == epics::pvData::scalarArray)) {
                    REPORT_ERROR_STATIC(ErrorManagement::Debug, "Resolving scalar (or array of) [%s]", fullFieldName.Buffer());
                    uint32 k;
                    bool found = false;
                    for (k = 0u; (k < numberOfSignals) && (ok) && (!found); k++) {
                        found = (cachedSignals[k].qualifiedName == fullFieldName);
                        if (found) {
                            cachedSignals[k].pvField = field;
                        }
                    }
                }
                else if (fieldType == epics::pvData::structure) {
                    REPORT_ERROR_STATIC(ErrorManagement::Debug, "Resolving structure [%s]", fullFieldName.Buffer());
                    ok = ResolveStructure(static_cast<const epics::pvData::PVStructure*>(field.operator ->()), fullFieldName.Buffer());
                }
            }
        }
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
                structureResolved = false;
            }
            ok = monitor.valid();
        }
        if (ok) {
            if (monitor.wait(0.2)) {
                if (monitor.event.event == pvac::MonitorEvent::Data) {
                    while (monitor.poll()) {
                        if (!structureResolved) {
                            if (ok) {
                                ok = ResolveStructure(monitor.root.operator ->(), "");
                            }
                            structureResolved = ok;
                        }
                        uint32 n;
                        for (n = 0u; (n < numberOfSignals) && (ok); n++) {
                            if ((cachedSignals[n].numberOfElements) == 1u) {
                                epics::pvData::PVScalar::const_shared_pointer scalarFieldPtr = std::dynamic_pointer_cast<
                                        const epics::pvData::PVScalar>(cachedSignals[n].pvField);
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
                                epics::pvData::PVScalarArray::const_shared_pointer scalarArrayPtr = std::dynamic_pointer_cast<
                                        const epics::pvData::PVScalarArray>(cachedSignals[n].pvField);
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

const char8 * const EPICSPVAChannelWrapper::GetFieldName() {
    return fieldName.Buffer();
}

}

