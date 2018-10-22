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

struct EPICSPVAChannelWrapperCachedSignal {
    uint32 numberOfElements;
    StreamString qualifiedName;
    TypeDescriptor typeDescriptor;
    void *memory;
};

/**
 * @brief TODO
 */
class EPICSPVAChannelWrapper {
public:
    /**
     * @brief TODO
     */
    EPICSPVAChannelWrapper();

    /**
     * @brief TODO
     */
    ~EPICSPVAChannelWrapper();

    /**
     * @brief TODO
     */
    bool Setup(StructuredDataI &data);

    /**
     * @brief TODO
     */
    void GetSignalMemory(const char8 * const qualifiedName, void *&mem);

    /**
     * @brief TODO
     */
    bool Put();

    /**
     * @brief TODO
     */
    const char8 * const GetChannelName();

private:

    /**
     * @brief TODO
     */
    template<typename T>
    void PutHelper(pvac::detail::PutBuilder &putBuilder, uint32 n);

    /**
     * @brief Recursively load the signal structures into an array of EPICSPVAStructureDataI.
     * TODO
     */
    bool LoadSignalStructure(StructuredDataI &cdbSignalStructure, StreamString fullNodeName, StreamString relativeNodeName);

    /**
     * The EPICSPVA channel
     */
    pvac::ClientChannel channel;
    pvac::ClientProvider provider;

    /**
     * The EPICSPVA channelName
     */
    StreamString channelName;

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
     * The cached signals/
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
        epics::pvData::shared_vector<const T> vec = epics::pvData::shared_vector<const T>(reinterpret_cast<T *>(cachedSignals[n].memory), 0u, cachedSignals[n].numberOfElements);
        putBuilder.set(cachedSignals[n].qualifiedName.Buffer(), vec);
    }
}
}

#endif /* EPICSPVA_EPICSPVACHANNELWRAPPER_H_ */
