/**
 * @file HighResolutionTimestampProvider.h
 * @brief Header file for class HighResolutionTimestampProvider
 * @date 10/09/2021
 * @author Luca Porzio
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

 * @details This header file contains the declaration of the class HighResolutionTimestampProvider
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SOURCE_COMPONENTS_DATASOURCES_UARTDATASOURCE_HIGHRESOLUTIONTIMESTAMPPROVIDER_H_
#define SOURCE_COMPONENTS_DATASOURCES_UARTDATASOURCE_HIGHRESOLUTIONTIMESTAMPPROVIDER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "TimestampProvider.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Default plugin which provides time to the UARTDataSource.
   Relies on the MARTe2 underlying TimeStamp component.
 */
class HighResolutionTimestampProvider: public TimestampProvider {
public:CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor.
     */
    HighResolutionTimestampProvider();

    /**
     * @brief Destructor.
     */
    virtual ~HighResolutionTimestampProvider();

    /**
     * @brief return the time-stamp using the MARTe2 HighResolutionTimer.
     * @return the time-stamp using the MARTe2 HighResolutionTimer.
     */
    virtual uint64 Timestamp();

private:

    /**
     * Last time-stamp provided.
     */
    TimeStamp ts;
};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SOURCE_COMPONENTS_DATASOURCES_UARTDATASOURCE_HIGHRESOLUTIONTIMESTAMPPROVIDER_H_ */

