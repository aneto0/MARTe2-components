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
#include <pv/pvData.h>
#include <pva/client.h>

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
                ok = signalsIndexCache.CreateAbsolute(tempCachedSignals[numberOfSignals].qualifiedName.Buffer());
                if (ok) {
                    ok = signalsIndexCache.Write("Index", numberOfSignals);
                }
                else {
                    REPORT_ERROR_STATIC(ErrorManagement::Warning, "Failed to add signal %s to signalsIndexCache [%d]", tempCachedSignals[numberOfSignals].qualifiedName.Buffer(), numberOfSignals);
                }
            }
            if (ok) {
                ok = dataSource.GetSignalMemoryBuffer(n, 0u, tempCachedSignals[numberOfSignals].memory);
            }
            REPORT_ERROR_STATIC(ErrorManagement::Information, "Registering signal %s [%s]", tempCachedSignals[numberOfSignals].qualifiedName.Buffer(), channelName.Buffer());
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
                epics::pvData::PVStructurePtr getPVStruct = std::const_pointer_cast<epics::pvData::PVStructure>(channel.get());
                ok = (getPVStruct) ? true : false;
                if (ok) {
                    ok = ResolveStructure(getPVStruct, "");
                    if (ok) {
                        putPVStruct = getPVStruct;
                    }
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
                else if (cachedSignals[n].typeDescriptor == Character8Bit) {
                    PutHelper<char8>(n);
                }
                else if (cachedSignals[n].typeDescriptor == CharString) {
                    ok = false;
                    REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "For strings use Type = char8; for bytes use Type = uint8");
                }
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

bool EPICSPVAChannelWrapper::ResolveStructure(epics::pvData::PVFieldPtr pvField, const char8 * const nodeName) {
    epics::pvData::PVStructurePtr pvStruct = std::dynamic_pointer_cast<epics::pvData::PVStructure>(pvField);
    bool ok = (pvStruct ? true : false);
    if (ok) {
        const epics::pvData::PVFieldPtrArray & fields = pvStruct->getPVFields();
        uint32 nOfFields = fields.size();
        uint32 n;

        for (n = 0u; (n < nOfFields) && (ok); n++) {
            epics::pvData::PVFieldPtr field = fields[n];
            epics::pvData::Type fieldType = field->getField()->getType();
            REPORT_ERROR_STATIC(ErrorManagement::Debug, "ResolveStructure -- fields [%d of %d] -- [%s]", n, nOfFields, field->getFieldName().c_str());

            StreamString fullFieldName = nodeName;
            if (fullFieldName.Size() > 0u) {
                fullFieldName += ".";
            }
            fullFieldName += field->getFieldName().c_str();

            if (fieldType == epics::pvData::structureArray) {
                epics::pvData::PVStructureArray::const_svector arr(static_cast<const epics::pvData::PVStructureArray*>(field.operator ->())->view());
                uint32 z;
                REPORT_ERROR_STATIC(ErrorManagement::Debug, "Resolving structureArray [%s - %s] - [%d]", nodeName, field->getFieldName().c_str(), static_cast<int32>(arr.size()));
                StreamString indexFullFieldName = fullFieldName;
                ok = (arr.size() > 0);
                for (z = 0u; (z < arr.size()) && (ok); z++) {
                    indexFullFieldName = fullFieldName;
                    indexFullFieldName.Printf("[%d]", z);
                    //This assumes that only linear arrays are supported, otherwise the field name will be wrong.
                    ok = ResolveStructure(arr[z], indexFullFieldName.Buffer());
                }
            }
            else {
                if ((fieldType == epics::pvData::scalar) || (fieldType == epics::pvData::scalarArray)) {
                    REPORT_ERROR_STATIC(ErrorManagement::Debug, "Resolving scalar (or array of) [%s]", fullFieldName.Buffer());
                    uint32 index = 0u;
                    bool found = signalsIndexCache.MoveAbsolute(fullFieldName.Buffer());
                    if (found) {
                        ok = signalsIndexCache.Read("Index", index);
                    }
                    ok = found;
                    if (ok) {
                        cachedSignals[index].pvField = field;
                        REPORT_ERROR_STATIC(ErrorManagement::Debug, "Assigned PV to signal with name [%s]", fullFieldName.Buffer());
                    }
                    if (!ok) {
                        REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "Could not find signal with name [%s]", fullFieldName.Buffer());
                    }
                }
                else if (fieldType == epics::pvData::structure) {
                    REPORT_ERROR_STATIC(ErrorManagement::Debug, "Resolving structure [%s]", fullFieldName.Buffer());
                    ok = ResolveStructure(field, fullFieldName.Buffer());
                }
            }
        }
    }
    return ok;
}

bool EPICSPVAChannelWrapper::Monitor() {
    bool ok = false;
    try {
        ok = (channel ? true : false);
        if (!ok) {
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
        }
        if (ok) {
            if (monitor.wait(2.0)) {
                if (monitor.event.event == pvac::MonitorEvent::Data) {
                    while (monitor.poll()) {
                        structureResolved = (monitorRoot ? true : false);
                        if (structureResolved) {
                            structureResolved = (monitorRoot.get() == monitor.root.get());
                        }
                        if (!structureResolved) {
                            if (ok) {
                                monitorRoot = monitor.root;
                                ok = ResolveStructure(std::const_pointer_cast<epics::pvData::PVStructure>(monitorRoot), "");
                            }
                            structureResolved = ok;
                        }
                        uint32 n;
                        for (n = 0u; (n < numberOfSignals) && (ok); n++) {
                            epics::pvData::PVScalar::const_shared_pointer scalarFieldPtr = std::dynamic_pointer_cast<const epics::pvData::PVScalar>(cachedSignals[n].pvField);
                            if (cachedSignals[n].typeDescriptor == Character8Bit) {
                                ok = (scalarFieldPtr ? true : false);
                                if (ok) {
                                    std::string value = scalarFieldPtr->getAs<std::string>();
                                    uint32 maxSize = value.size();
                                    if (maxSize > cachedSignals[n].numberOfElements) {
                                        maxSize = cachedSignals[n].numberOfElements;
                                    }
                                    StringHelper::CopyN(reinterpret_cast<char8 *>(cachedSignals[n].memory), value.c_str(), maxSize);
                                }
                            }
                            else if (cachedSignals[n].typeDescriptor == CharString) {
                                ok = false;
                                REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "For strings use Type = char8; for bytes use Type = uint8");
                            }
                            else if ((cachedSignals[n].numberOfElements) == 1u) {
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
                                        REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "Unsupported read type for signal %s", cachedSignals[n].qualifiedName.Buffer());
                                        ok = false;
                                    }
                                }
                            }
                            else {
                                epics::pvData::PVScalarArray::const_shared_pointer scalarArrayPtr = std::dynamic_pointer_cast<const epics::pvData::PVScalarArray>(cachedSignals[n].pvField);
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
                                        REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "Unsupported read array type for signal %s", cachedSignals[n].qualifiedName.Buffer());
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

