/**
 * @file EPICSPVAChannelWrapper.h
 * @brief Header file for class EPICSPVAChannelWrapper
 * @date Oct 21, 2018
 * @author aneto
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
#include "ConfigurationDatabase.h"
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
};

/**
 * @brief Helper class which encapsulates a PVA signal (record) and allows to put/monitor.
 */
class EPICSPVAChannelWrapper {
public:
    /**
     * @brief Constructor. NOOP.
     */
    EPICSPVAChannelWrapper();

    /**
     * @brief Destructor. NOOP.
     */
    ~EPICSPVAChannelWrapper();

    /**
     * @brief Registers the channel with a name given by data.Read("Alias") or data.GetName(), if the former does not exist.
     * @param[in] data the structure to be registered against the channel.
     */
    bool Setup(StructuredDataI &data);

    /**
     * @brief Gets the memory associated to the signal with the provided \a qualifiedName (that shall exist in the structure provide in Setup).
     * @param[in] qualifiedName the full signal name inside the structure (dot separated).
     * @param[out] mem the signal memory or NULL if the signal cannot be found.
     */
    void GetSignalMemory(const char8 * const qualifiedName, void *&mem);

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
     * @brief Gets this channel name (see Setup).
     * @return this channel name .
     */
    const char8 * const GetChannelName();

    /**
     * @brief Gets this channel name irrespectively of the Alias.
     * @return this channel name irrespectively of the Alias.
     */
    const char8 * const GetChannelUnaliasedName();

private:

    /**
     * @brief Helper method which set signal at index \a in the \a putBuilder.
     * @param[in] putBuilder a valid builder which will be updated with the signal[n] value.
     * @param[in] n the index of the signal to write.
     */
    template<typename T>
    void PutHelper(pvac::detail::PutBuilder &putBuilder, uint32 n);

    /**
     * @brief Helper method which gets the value of the signal at index \a from the relevant field in the \a scalarArrayPtr.
     * @param[in] scalarArrayPtr valid PVScalarArray from where to read the array values.
     * @param[in] n the index of the signal to be updated with the read array values.
     * @return true if the array was successfully read.
     */
    template<typename T>
    bool GetArrayHelper(epics::pvData::PVScalarArray::const_shared_pointer scalarArrayPtr, uint32 n);

    /**
     * @brief Recursively loads the signal structures into a ConfigurationDatabase (which is used as a memory backend).
     * @param[in] cdbSignalStructure the signals to be loaded (see Setup).
     * @param[in] fullNodeName the full name of the node currently being queried.
     * @param[in] relativeNodeName the name of the node currently being queried.
     * @return true if all the ConfigurationDatabase read/write operations are successful.
     */
    bool LoadSignalStructure(StructuredDataI &cdbSignalStructure, StreamString fullNodeName, StreamString relativeNodeName);

    /**
     * @brief Loads a basic type into the memory backend configuration database.
     * @param[in] td the type to load.
     * @param[in] numberOfElements the number of elements associated to the type.
     * @param[in] numberOfDimensions the number of dimensions associated to the type.
     * @param[in] fullNodeName the full name of the node currently being queried.
     * @param[in] relativeNodeName the name of the node currently being queried.
     * @return true if all the ConfigurationDatabase write operation is successful.
     */
    bool LoadBasicType(TypeDescriptor &td, uint32 numberOfElements, uint32 numberOfDimensions, StreamString fullNodeName,
                       StreamString relativeNodeName);

    /**
     * @brief Recursively loads a structure type into the memory backend configuration database.
     * @param[in] type name the name of the type to load.
     * @param[in] fullNodeName the full name of the node currently being queried.
     * @param[in] relativeNodeName the name of the node currently being queried.
     * @param[in] entry introspection information about the type.
     * @return true if all the ConfigurationDatabase write operation is successful.
     */
    bool LoadStructuredType(const char8 * const typeName, StreamString fullNodeName, StreamString relativeNodeName,
                            const IntrospectionEntry *entry);

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
     * The channelName without considering the alias
     */
    StreamString unliasedChannelName;

    /**
     * The backend memory
     */
    ConfigurationDatabase memoryBackend;

    /**
     * The number of signals that were found while loading the structure.
     */
    uint32 numberOfSignals;

    /**
     * Number of signals actually being used by the broker.
     */
    uint32 numberOfRequestedSignals;

    /**
     * The cached signals (flat list of the structure identified with the qualifiedName).
     */
    EPICSPVAChannelWrapperCachedSignal *cachedSignals;
};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/
namespace MARTe {
template<typename T>
void EPICSPVAChannelWrapper::PutHelper(pvac::detail::PutBuilder &putBuilder, uint32 n) {
    if ((cachedSignals[n].numberOfElements) == 1u) {
        putBuilder.set(cachedSignals[n].qualifiedName.Buffer(), *static_cast<T *>(cachedSignals[n].memory));
    }
    else {
        epics::pvData::shared_vector<T> out;
        out.resize(cachedSignals[n].numberOfElements);
        (void) MemoryOperationsHelper::Copy(reinterpret_cast<void *>(out.data()), cachedSignals[n].memory,
                                            cachedSignals[n].numberOfElements * sizeof(T));
        epics::pvData::shared_vector<const T> outF = freeze(out);
        putBuilder.set(cachedSignals[n].qualifiedName.Buffer(), outF);
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
