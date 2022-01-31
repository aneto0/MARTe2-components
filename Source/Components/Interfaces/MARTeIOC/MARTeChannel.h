/**
 * @file MARTeChannel.h
 * @brief Header file for class MARTeChannel
 * @date 02/07/2019
 * @author Giuseppe Ferro
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

 * @details This header file contains the declaration of the class MARTeChannel
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SOURCE_COMPONENTS_INTERFACES_MARTEIOC_MARTECHANNEL_H_
#define SOURCE_COMPONENTS_INTERFACES_MARTEIOC_MARTECHANNEL_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "casdef.h"
#include "GeneralDefinitions.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Dummy implementation of a casChannel to be used inside MARTe
 */
class MARTeChannel: public casChannel {
public:

    /**
     * @brief Constructor
     */
    /*lint -e{1712} default constructor not needed here*/
    MARTeChannel(const casCtx & ctxIn);

    /**
     * @brief Destructor
     */
    virtual ~MARTeChannel();

    /**
     * @brief NOOP
     */
    virtual void setOwner(const char8 * const pUserName,
                          const char8 * const pHostName);

    /**
     * @brief Returns true
     */
    virtual bool readAccess() const;

    /**
     * @brief Returns true
     */
    virtual bool writeAccess() const;
    /*lint -e{1712} default constructor not needed here*/
};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SOURCE_COMPONENTS_INTERFACES_MARTEIOC_MARTECHANNEL_H_ */

