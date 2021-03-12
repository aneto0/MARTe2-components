/**
 * @file TcnTimeProvider.h
 * @brief Header file for class TcnTimeProvider
 * @date 31 ott 2019
 * @author pc
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

 * @details This header file contains the declaration of the class TcnTimeProvider
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SOURCE_COMPONENTS_DATASOURCES_LINUXTIMER_TCNTIMEPROVIDER_H_
#define SOURCE_COMPONENTS_DATASOURCES_LINUXTIMER_TCNTIMEPROVIDER_H_

#define CALL_MEMBER_FUN(object, ptrToMemberFun)  ((object).*(ptrToMemberFun))

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "TimeProvider.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
* @brief
*/
const uint8 TCNTIMEPROVIDER_DEFAULT_POLL = 0u;

/**
* @brief
*/
const uint64 TCNTIMEPROVIDER_DEFAULT_FREQUENCY = 1000000000ull;

/**
* @brief
*/
const uint32 TCNTIMEPROVIDER_DEFAULT_TOLERANCE = 1000u;

/**
* @brief
*/
typedef enum {
    TCNTimeProvider_NoPollLegacyMode,
    TCNTimeProvider_PollLegacyMode,
    TCNTimeProvider_WaitUntilMode,
    TCNTimeProvider_WaitUntilHRMode,
    TCNTimeProvider_SleepMode,
    TCNTimeProvider_SleepHRMode
}TCNTimeProviderOperationMode;

/**
* @brief
*/
class TCNTimeProvider: public TimeProvider {
    public:
        CLASS_REGISTER_DECLARATION()

        /**
        * @brief Default constructor for the TCN Time Provider
        */
        TCNTimeProvider();
        
        /**
        * @brief Destructor for the TCN Time Provider
        */
        virtual ~TCNTimeProvider();

        /**
        * @brief Initialises the TCN Time provider using MARTe2 provided data
        */
        virtual bool Initialise(StructuredDataI &data);

        /**
        * @brief Returns the actual internal counter value
        * @return The internal counter value
        */
        virtual uint64 Counter();

        /**
        * @brief Returns the internal period value
        * @return The internal period value
        */
        virtual float64 Period();

        /**
        * @brief Returns the internal frequency value
        * @return The internal frequency value
        *
        */
        virtual uint64 Frequency();

        /**
        * @brief Sleeps for delta ticks from the start marker, using the specific provider-configured strategy.
        * @param[in] start Ticks to start sleeping
        * @param[in] delta Amount of ticks to sleep
        */
        virtual void BusySleep(uint64 start, uint64 delta);

    private:
        /**
        * @brief Holds the provider internal frequency, which in turn is used to compute the period
        */
        uint64 tcnFrequency;

        /**
        * @brief Holds the provider allowable tolerance on the sleep duration
        */
        uint32 tolerance;

        /**
        * @brief Tells if the TCN plugin is working in closed loop mode
        */
        uint8 closedLoopMode;

        /**
        * @brief Holds the accumulated error among all the sleep calls
        */
        int64 cumulativeError;

        /**
        * @brief Holds the error derived from the last call
        */
        int64 lastCallError;

        /**
        * @brief Pointer to the specific sleep strategy implementation
        */
        void (TCNTimeProvider::*BusySleepProvider)(uint64, uint64);

        /**
        * @brief Uses the HighResolutionTimer as source for the sleep, assuming the driver correction happens transparently.
        */
        void NoPollBSP(uint64 start, uint64 delta);

        /**
        * @brief Polls the Counter() method to compute the sleep duration and wakeup instant
        */
        void PollBSP(uint64 start, uint64 delta);

        /**
        * @brief Uses the TCN wait until method to sleep (Refer to tcn_wait_until docs)
        */
        void WaitUntilBSP(uint64 start, uint64 delta);

        /**
        * @brief Uses the TCN wait until with high resolution method to sleep (Refer to tcn_wait_until_hr docs)
        */
        void WaitUntilHRBSP(uint64 start, uint64 delta);

        /**
        * @brief Uses the TCN native sleep (POSIX style) method to sleep (Refer to tcn_sleep docs)
        */
        void SleepBSP(uint64 start, uint64 delta);

        /**
        * @brief Uses the TCN native sleep (POSIX style) with high resolution method to sleep (Refer to tcn_sleep docs)
        */
        void SleepHRBSP(uint64 start, uint64 delta);
    };
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SOURCE_COMPONENTS_DATASOURCES_LINUXTIMER_TCNTIMEPROVIDER_H_ */

