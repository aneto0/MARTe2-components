/**
 * @file LoggerDataSource.h
 * @brief Header file for class LoggerDataSource
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

 * @details This header file contains the declaration of the class LoggerDataSource
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef LOGGERDATASOURCE_H_
#define LOGGERDATASOURCE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "DataSourceI.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {
/**
 * @brief A DataSource which prints in the MARTe logger the current value of any signal.
 * @details The LoggerDataSource will call REPORT_ERROR with the current value of
 *  all the signals listed on its Signals section.
 *
 * The configuration syntax is (names are only given as an example):
 * +LoggerDS = {
 *     Class = LoggerDataSource
 *     CyclePeriod = 0u //Optional, defaults to 0. Period of cycles must pass before logger produces an output.
 *                      //CyclePeriod = 0u means print every sample received.
 * }
 *
 * A signal will be added for each GAM signal that writes to this instance of the DataSourceI.
 */
class LoggerDataSource: public DataSourceI {
public:
    CLASS_REGISTER_DECLARATION()
    /**
     * @brief Default constructor. NOOP.
     */
LoggerDataSource    ();

    /**
     * @brief Destructor. NOOP.
     */
    virtual ~LoggerDataSource();


    virtual bool Initialise(StructuredDataI & data);

    /**
     * @brief NOOP.
     * @return true.
     */
    virtual bool Synchronise();

    /**
     * @brief NOOP.
     * @return true.
     */
    virtual bool AllocateMemory();

    /**
     * @brief NOOP.
     * @return 0u.
     */
    virtual uint32 GetNumberOfMemoryBuffers();

    /**
     * @brief NOOP.
     * @return true.
     */
    virtual bool GetSignalMemoryBuffer(const uint32 signalIdx,
            const uint32 bufferIdx,
            void *&signalAddress);

    /**
     * @brief Returns "LoggerBroker".
     * @param[in] data is not used as LoggerBroker is always returned.
     * @param[in] direction is not used as LoggerBroker is always returned.
     * @return "LoggerBroker"
     */
    virtual const char8 *GetBrokerName(StructuredDataI &data,
            const SignalDirection direction);

    /**
     * @brief Only OutputBrokers are supported.
     * @return false.
     */
    virtual bool GetInputBrokers(ReferenceContainer &inputBrokers,
            const char8* const functionName,
            void * const gamMemPtr);

    /**
     * @brief Creates a new instance of a LoggerBroker and adds it to \a outputBrokers.
     * @param[out] outputBrokers where the BrokerI instances have to be added to.
     * @param[in] functionName name of the function being queried.
     * @param[in] gamMemPtr the GAM memory where the signals will be written to.
     * @return true iff the LoggerBroker has been successfully initialised and added to the \a outputBrokers.
     */
    virtual bool GetOutputBrokers(ReferenceContainer &outputBrokers,
            const char8* const functionName,
            void * const gamMemPtr);

    /**
     * @brief NOOP.
     * @return true.
     */
    virtual bool PrepareNextState(const char8 * const currentStateName,
            const char8 * const nextStateName);

protected:
    /**
     * @brief Holds the period of cycles must pass before logger produces an output.
     */
    uint32 cyclePeriod;
};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* LOGGERDATASOURCE_H_ */

