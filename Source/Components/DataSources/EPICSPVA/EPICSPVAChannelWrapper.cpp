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
#include "MutexSem.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {
EPICSPVAChannelWrapper::EPICSPVAChannelWrapper() {
    numberOfSignals = 0u;
    resolvedStructIndexMap = NULL_PTR(uint32 *);
    cachedSignals = NULL_PTR(EPICSPVAFieldWrapperI **);
    structureResolved = false;
    putFinished = false;
}

EPICSPVAChannelWrapper::~EPICSPVAChannelWrapper() {
    if (cachedSignals != NULL_PTR(EPICSPVAFieldWrapperI **)) {
        for (uint32 n = 0u; (n < numberOfSignals); n++) {
            if (cachedSignals[n] != NULL_PTR(EPICSPVAFieldWrapperI *)) {
                delete cachedSignals[n];
            }
        }
        delete[] cachedSignals;
    }
    if (resolvedStructIndexMap != NULL_PTR(uint32 *)) {
        delete [] resolvedStructIndexMap;
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
    EPICSPVAFieldWrapperI **tempCachedSignals = new EPICSPVAFieldWrapperI*[maxNumberOfSignals];
    uint32 n;
    numberOfSignals = 0u;
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
            StreamString qualifiedName;
            uint32 numberOfElements;
            void *memory;
            TypeDescriptor typeDescriptor;
            if (ok) {
                if (signalName.Position() == signalName.Size()) {
                    ok = qualifiedName.Printf("%s", fieldName.Buffer());
                }
                else {
                    if (term == '.') {
                        ok = qualifiedName.Printf("%s.%s", fieldName.Buffer(), signalName);
                    }
                    else {
                        ok = qualifiedName.Printf("%s[%s", fieldName.Buffer(), signalName);
                    }
                }
            }
            if (ok) {
                typeDescriptor = dataSource.GetSignalType(n);
                ok = dataSource.GetSignalNumberOfElements(n, numberOfElements);
            }
            if (ok) {
                ok = signalsIndexCache.CreateAbsolute(qualifiedName.Buffer());
                if (ok) {
                    ok = signalsIndexCache.Write("Index", numberOfSignals);
                }
                else {
                    REPORT_ERROR_STATIC(ErrorManagement::Warning, "Failed to add signal %s to signalsIndexCache [%d]", qualifiedName.Buffer(), numberOfSignals);
                }
            }
            if (ok) {
                ok = dataSource.GetSignalMemoryBuffer(n, 0u, memory);
            }
            if (ok) {
                if (typeDescriptor == UnsignedInteger8Bit) {
                    tempCachedSignals[numberOfSignals] = new EPICSPVAFieldWrapper<uint8>();
                }
                else if (typeDescriptor == UnsignedInteger16Bit) {
                    tempCachedSignals[numberOfSignals] = new EPICSPVAFieldWrapper<uint16>();
                }
                else if (typeDescriptor == UnsignedInteger32Bit) {
                    tempCachedSignals[numberOfSignals] = new EPICSPVAFieldWrapper<uint32>();
                }
                else if (typeDescriptor == UnsignedInteger64Bit) {
                    tempCachedSignals[numberOfSignals] = new EPICSPVAFieldWrapper<unsigned long int>();
                }
                else if (typeDescriptor == SignedInteger8Bit) {
                    tempCachedSignals[numberOfSignals] = new EPICSPVAFieldWrapper<int8>();
                }
                else if (typeDescriptor == SignedInteger16Bit) {
                    tempCachedSignals[numberOfSignals] = new EPICSPVAFieldWrapper<int16>();
                }
                else if (typeDescriptor == SignedInteger32Bit) {
                    tempCachedSignals[numberOfSignals] = new EPICSPVAFieldWrapper<int32>();
                }
                else if (typeDescriptor == SignedInteger64Bit) {
                    tempCachedSignals[numberOfSignals] = new EPICSPVAFieldWrapper<long int>();
                }
                else if (typeDescriptor == Float32Bit) {
                    tempCachedSignals[numberOfSignals] = new EPICSPVAFieldWrapper<float32>();
                }
                else if (typeDescriptor == Float64Bit) {
                    tempCachedSignals[numberOfSignals] = new EPICSPVAFieldWrapper<float64>();
                }
                else if (typeDescriptor == Character8Bit) {
                    tempCachedSignals[numberOfSignals] = new EPICSPVAFieldWrapper<char8>();
                }
                else if (typeDescriptor == BooleanType) {
                    tempCachedSignals[numberOfSignals] = new EPICSPVAFieldWrapper<bool>();
                }
                else if (typeDescriptor == CharString) {
                    ok = false;
                    REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "For strings use Type = char8; for bytes use Type = uint8");
                }
            }
            if (ok) {
                tempCachedSignals[numberOfSignals]->SetMemory(numberOfElements, qualifiedName.Buffer(), memory);
            }
            REPORT_ERROR_STATIC(ErrorManagement::Information, "Registering signal %s [%s]", qualifiedName.Buffer(), channelName.Buffer());
            numberOfSignals++;
        }
    }
    if (ok) {
        if (cachedSignals != NULL_PTR(EPICSPVAFieldWrapperI**)) {
            delete[] cachedSignals;
        }
        if (numberOfSignals > 0u) {
            cachedSignals = new EPICSPVAFieldWrapperI*[numberOfSignals];
            for (n = 0u; (n < numberOfSignals); n++) {
                cachedSignals[n] = tempCachedSignals[n];
            }
        }
        if (tempCachedSignals != NULL_PTR(EPICSPVAFieldWrapperI **)) {
            delete[] tempCachedSignals;
        }
    }
    if (ok) {
        resolvedStructIndexMap = new uint32[numberOfSignals];
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
    bool ok = true;
    try {
        if (!channel.valid()) {
            provider = pvac::ClientProvider("pva");
            REPORT_ERROR_STATIC(ErrorManagement::Information, "Connected to channel %s", channelName.Buffer());
            channel = pvac::ClientChannel(provider.connect(channelName.Buffer()));
        }
        ok = channel.valid();

        if (ok) {
            if (!structureResolved) {
                uint32 absIndex = 0u;
                epics::pvData::PVStructurePtr getPVStruct;
                try {
                    getPVStruct  = std::const_pointer_cast<epics::pvData::PVStructure>(channel.get());
                }
                catch (std::runtime_error &ignored) {
                    REPORT_ERROR_STATIC(ErrorManagement::Warning, "Failed to get() channel: %s - %s", channelName.Buffer(), ignored.what());
                    ok = false;
                }
                /*catch (pvac::Timeout &ignored) {
                    REPORT_ERROR_STATIC(ErrorManagement::Warning, "Failed to get() channel: %s - %s", channelName.Buffer(), ignored.what());
                    ok = false;
                }*/
                if (ok) {
                    ok = (getPVStruct) ? true : false;
                }
                if (ok) {
                    ok = ResolveStructure(getPVStruct, "", absIndex);
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
                cachedSignals[n]->Put();
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
    catch (std::runtime_error &ignored) {
        REPORT_ERROR_STATIC(ErrorManagement::Information, "Failed to connect to channel %s [%s]", channelName.Buffer(), ignored.what());
        ok = false;
    }
    /*catch (pvac::Timeout &ignored) {
        REPORT_ERROR_STATIC(ErrorManagement::Warning, "Failed to connect to channel: %s - [%s]", channelName.Buffer(), ignored.what());
        ok = false;
    }*/

    return ok;
}

bool EPICSPVAChannelWrapper::RefreshStructure(epics::pvData::PVFieldPtr pvField, uint32 &absIndex) {
    epics::pvData::PVStructurePtr pvStruct = std::dynamic_pointer_cast<epics::pvData::PVStructure>(pvField);
    bool ok = (pvStruct ? true : false);
    if (ok) {
        const epics::pvData::PVFieldPtrArray & fields = pvStruct->getPVFields();
        uint32 nOfFields = fields.size();
        uint32 n;

        for (n = 0u; (n < nOfFields) && (ok); n++) {
            epics::pvData::PVFieldPtr field = fields[n];
            epics::pvData::Type fieldType = field->getField()->getType();
            if (fieldType == epics::pvData::structureArray) {
                epics::pvData::PVStructureArray::const_svector arr(static_cast<const epics::pvData::PVStructureArray*>(field.operator ->())->view());
                uint32 z;
                ok = (arr.size() > 0);
                for (z = 0u; (z < arr.size()) && (ok); z++) {
                    //This assumes that only linear arrays are supported, otherwise the field name will be wrong.
                    ok = RefreshStructure(arr[z], absIndex);
                }
            }
            else {
                if ((fieldType == epics::pvData::scalar) || (fieldType == epics::pvData::scalarArray)) {
                    uint32 index = resolvedStructIndexMap[absIndex];
                    cachedSignals[index]->SetPVAField(field);
                    ok = cachedSignals[index]->Get();
                    absIndex++;
                }
                else if (fieldType == epics::pvData::structure) {
                    ok = RefreshStructure(field, absIndex);
                }
            }
        }
    }
    return ok;
}

bool EPICSPVAChannelWrapper::ResolveStructure(epics::pvData::PVFieldPtr pvField, const char8 * const nodeName, uint32 &absIndex) {
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
                    ok = ResolveStructure(arr[z], indexFullFieldName.Buffer(), absIndex);
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
                        resolvedStructIndexMap[absIndex] = index;
                        cachedSignals[index]->SetPVAField(field);
                        absIndex++;
                        REPORT_ERROR_STATIC(ErrorManagement::Debug, "Assigned PV to signal with name [%s]", fullFieldName.Buffer());
                    }
                    if (!ok) {
                        REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "Could not find signal with name [%s] [%s]", fullFieldName.Buffer(), channelName.Buffer());
                    }
                }
                else if (fieldType == epics::pvData::structure) {
                    REPORT_ERROR_STATIC(ErrorManagement::Debug, "Resolving structure [%s]", fullFieldName.Buffer());
                    ok = ResolveStructure(field, fullFieldName.Buffer(), absIndex);
                }
            }
        }
    }
    return ok;
}

bool EPICSPVAChannelWrapper::Monitor() {
    bool ok = true;
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
                        uint32 absIndex = 0u;
                        if (!structureResolved) {
                            if (ok) {
                                monitorRoot = monitor.root;
                                REPORT_ERROR_STATIC(ErrorManagement::Information, "Resolving structure for channel %s", channelName.Buffer());
                                ok = ResolveStructure(std::const_pointer_cast<epics::pvData::PVStructure>(monitorRoot), "", absIndex);
                            }
                            structureResolved = ok;
                        }
                        if (ok) {
                            absIndex = 0u;
                            ok = RefreshStructure(std::const_pointer_cast<epics::pvData::PVStructure>(monitorRoot), absIndex);
                        }
                    }
                }
            }
        }
    }
    catch (std::runtime_error &ignored) {
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

