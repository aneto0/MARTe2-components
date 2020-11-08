/**
 * @file LoggerBroker.h
 * @brief Header file for class LoggerBroker
 * @date 09/11/2016
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

 * @details This header file contains the declaration of the class LoggerBroker
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef LOGGERBROKER_H_
#define LOGGERBROKER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "BrokerI.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {

/**
 * @brief a BrokerI implementation for the LoggerDataSource.
 * @details The Execute method prints to the REPORT_ERROR stream the value of all
 *  the registered signals, using the AnyType Printf.
 */
class LoggerBroker: public BrokerI {

public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor. NOOP
     */
    LoggerBroker();

    /**
     * @brief Destructor. Frees memory allocated for the output signals.
     */
    virtual ~LoggerBroker();

    /**
     * @brief For every signal from the functionName that interacts with this DataSourceI, stores
     *  the signal type and dimension in an AnyType which gets printed in the Execute method.
     * @param[in] direction only OutputSignals is supported.
     * @param[in] dataSourceIn the data source which holds the signals definition.
     * @param[in] functionName the name of the function holding the signal memory.
     * @param[in] gamMemoryAddress the base address of the GAM memory (where signal data is stored).
     * @return true if all the copy information related to \a functionName can be successfully retrieved.
     */
    virtual bool Init(SignalDirection direction,
            DataSourceI &dataSourceIn,
            const char8 * const functionName,
            void *gamMemoryAddress);


    void SetPeriod(const uint32 cyclePeriodIn);

    /**
     * @brief For all the signals print their AnyType value in the logger stream.
     * @return true.
     */
    virtual bool Execute();

private:

    /**
     * Hold the AnyType value of the signals.
     */
    AnyType *outputSignals;

    /**
     * Holds the signal names.
     */
    StreamString *signalNames;

    /**
     * Log run cycles passed since a period occurred.
     */
    uint32 cycleCounter;

    /**
     * Holds the period of cycles must pass before logger produces an output.
     */
    uint32 cyclePeriod;
};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SOURCE_COMPONENTS_DATASOURCES_LOGGERDATASOURCE_LOGGERBROKER_H_ */

