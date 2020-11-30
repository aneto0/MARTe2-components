/**
 * @file EPICSPVAChannelWrapper.h
 * @brief Header file for class EPICSPVAChannelWrapper
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

 * @details This header file contains the declaration of the class EPICSPVAChannelWrapper
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef EPICSPVA_EPICSPVACHANNELWRAPPER_H_
#define EPICSPVA_EPICSPVACHANNELWRAPPER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
#include <pv/pvaClient.h>
#include <pva/client.h>

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "BinaryTree.h"
#include "ConfigurationDatabase.h"
#include "DataSourceI.h"
#include "DjbHashFunction.h"
#include "StreamString.h"
#include "StructuredDataI.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {

/**
 * Wraps a record signal memory
 */
struct EPICSPVAChannelWrapperCachedSignal {
    /**
     * Number of elements in the record signal.
     */
    uint32 numberOfElements;
    /**
     * Full qualified name of the signal inside the record (dot separated).
     */
    StreamString qualifiedName;
    /**
     * The signal type.
     */
    TypeDescriptor typeDescriptor;
    /**
     * Allocate memory with sufficient space to hold a copy of the signal.
     */
    void *memory;
    /**
     * The pv field
     */
    epics::pvData::PVFieldPtr pvField;
};

/**
 * @brief Helper class which encapsulates a PVA signal (record) and allows to put/monitor.
 */
class EPICSPVAChannelWrapper: public pvac::ClientChannel::PutCallback {
public:
    /**
     * @brief Constructor. NOOP.
     */
    EPICSPVAChannelWrapper();

    /**
     * @brief Destructor. NOOP.
     */
    virtual ~EPICSPVAChannelWrapper();

    /**
     * @brief Registers the channel with a name given by data.Read("Alias") or data.GetName(), if the former does not exist.
     * @details Also tries to read the field name data.Read("Field"). If not set Field = value is assumed. The Field cannot be set as
     *  node as this would prevent the RealTimeApplicationConfigurationBuilder from properly expanding the structured types.
     * @param[in] data the structure to be registered against the channel.
     */
    bool SetAliasAndField(StructuredDataI &data);

    /**
     * @brief Setup all the signal memory information.
     * @param[in] dataSource the data source holding the signals.
     * @return true if the memory setup was successfully set.
     */
    bool Setup(DataSourceI &dataSource);

    /**
     * @brief Copies from each signal memory (see GetSignalMemory) into the relevant PVA structure fields and commit the changes.
     * @return true if all the signals have been successfully committed into the network.
     */
    bool Put();

    /**
     * @brief Copies from relevant PVA structure fields into each signal memory.
     * @details This method has a fixed timeout of 0.2 second.
     * @return true if all the record and the monitor are valid.
     */
    bool Monitor();

    /**
     * @brief Gets this channel name (see SetAliasAndField).
     * @return this channel name .
     */
    const char8 * const GetChannelName();

    /**
     * @brief Gets the field name (see SetAliasAndField).
     * @return the field name.
     */
    const char8 * const GetFieldName();

    /**
     * @brief The callback function that is called when the channel.put method is called. Sets the args.root to pvStruct.
     * @param[in] build see pvac::ClientChannel::PutCallback
     * @param[in,out] args see pvac::ClientChannel::PutCallback
     */
    virtual void putBuild(const epics::pvData::StructureConstPtr& build, pvac::ClientChannel::PutCallback::Args& args);

    /**
     * @brief The callback function that is called when the put operation concludes.
     * @param[in] evt see pvac::ClientChannel::PutCallback
     */
    virtual void putDone(const pvac::PutEvent& evt);

private:

    /**
     * @brief Recursively populates the cachedSignals (flat list of the structure identified with the qualifiedName) array from the input structure.
     * @param[in] pvStruct the structure to be resolved.
     * @param[in] nodeName the name of structure.
     * @return true if the structure can be fully resolved with no errors.
     */
    bool ResolveStructure(epics::pvData::PVFieldPtr pvField, const char8 * const nodeName);

    /**
     * @brief Helper method which set signal at index \a in the \a putBuilder.
     * @param[in] n the index of the signal to write.
     */
    template<typename T>
    void PutHelper(uint32 n);

    /**
     * @brief Helper method which gets the value of the signal at index \a from the relevant field in the \a scalarArrayPtr.
     * @param[in] scalarArrayPtr valid PVScalarArray from where to read the array values.
     * @param[in] n the index of the signal to be updated with the read array values.
     * @return true if the array was successfully read.
     */
    template<typename T>
    bool GetArrayHelper(epics::pvData::PVScalarArray::const_shared_pointer scalarArrayPtr, uint32 n);

    /**
     * The EPICS PVA channel
     */
    pvac::ClientChannel channel;

    /**
     * The EPICS PVA provider. pvac::ClientProvider("pva") is currently hardcoded.
     */
    pvac::ClientProvider provider;

    /**
     * The MonitorSync that is used to asynchronously update the signal values.
     */
    pvac::MonitorSync monitor;

    /**
     * The EPICSPVA channelName
     */
    StreamString channelName;

    /**
     * The original signal name
     */
    StreamString originalName;

    /**
     * The EPICSPVA fieldName
     */
    StreamString fieldName;

    /**
     * The number of signals that were found while loading the structure.
     */
    uint32 numberOfSignals;

    /**
     * The cached signals (flat list of the structure identified with the qualifiedName).
     */
    EPICSPVAChannelWrapperCachedSignal *cachedSignals;

    /**
     * Was the structured resolved at least once?
     */
    bool structureResolved;

    /**
     * The structure which is pvput (see putBuild)
     */
    epics::pvData::PVStructure::const_shared_pointer putPVStruct;

    /**
     * The monitored root structure.
     */
    epics::pvData::PVStructure::const_shared_pointer monitorRoot;

    /**
     * Set to true when the put has finished.
     */
    bool putFinished;

    /**
     * Cache the index of the signals.
     */
    ConfigurationDatabase signalsIndexCache;
};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/
namespace MARTe {
template<typename T>
void EPICSPVAChannelWrapper::PutHelper(uint32 n) {
    if ((cachedSignals[n].numberOfElements) == 1u) {
        epics::pvData::PVScalarPtr scalarFieldPtr = std::dynamic_pointer_cast < epics::pvData::PVScalar > (cachedSignals[n].pvField);
        if (scalarFieldPtr ? true : false) {
            scalarFieldPtr->putFrom<T>(*static_cast<T *>(cachedSignals[n].memory));
        }
        else {
            REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Signal %s has an invalid pv field", cachedSignals[n].qualifiedName.Buffer());
        }
    }
    else {
        epics::pvData::PVScalarArrayPtr scalarArrayPtr = std::dynamic_pointer_cast < epics::pvData::PVScalarArray > (cachedSignals[n].pvField);
        if (scalarArrayPtr ? true : false) {
            epics::pvData::shared_vector<T> out;
            out.resize(cachedSignals[n].numberOfElements);
            (void) MemoryOperationsHelper::Copy(reinterpret_cast<void *>(out.data()), cachedSignals[n].memory, cachedSignals[n].numberOfElements * sizeof(T));
            epics::pvData::shared_vector<const T> outF = freeze(out);
            scalarArrayPtr->putFrom<T>(outF);
        }
        else {
            REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Signal %s has an invalid pv field", cachedSignals[n].qualifiedName.Buffer());
        }
    }
}

template<>
inline void EPICSPVAChannelWrapper::PutHelper<char8>(uint32 n) {
    epics::pvData::PVScalarPtr scalarFieldPtr = std::dynamic_pointer_cast < epics::pvData::PVScalar > (cachedSignals[n].pvField);
    if (scalarFieldPtr ? true : false) {
        std::string value = reinterpret_cast<char8 *>(cachedSignals[n].memory);
        scalarFieldPtr->putFrom<std::string>(value);
    }
    else {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError,
                            "Signal %s has an invalid pv field. Note that arrays of strings are not supported in the data-source yet",
                            cachedSignals[n].qualifiedName.Buffer());
    }
}

template<typename T>
bool EPICSPVAChannelWrapper::GetArrayHelper(epics::pvData::PVScalarArray::const_shared_pointer scalarArrayPtr, uint32 n) {
    bool ok = true;
    epics::pvData::shared_vector<const T> out;
    scalarArrayPtr->getAs<T>(out);
    uint32 i;
    Vector<T> readVec(reinterpret_cast<T *>(cachedSignals[n].memory), cachedSignals[n].numberOfElements);
    Vector<T> srcVec(const_cast<T *>(reinterpret_cast<const T *>(out.data())), cachedSignals[n].numberOfElements);
    for (i = 0u; i < cachedSignals[n].numberOfElements; i++) {
        readVec[i] = srcVec[i];
    }

    return ok;
}

}

#endif /* EPICSPVA_EPICSPVACHANNELWRAPPER_H_ */
