/**
 * @file TcnTimeProvider.h
 * @brief Header file for class TcnTimeProvider
 * @date 31/10/2019
 * @author Giuseppe Ferrò
 * @author Giuseppe Avon
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
* @brief Default frequency for the TCN provider underlying peripheral
*/
const uint64 TCNTIMEPROVIDER_DEFAULT_FREQUENCY = 1000000000ull;

/**
* @brief Default tolerance for the TCN provider High Resolution modes
*/
const uint32 TCNTIMEPROVIDER_DEFAULT_TOLERANCE = 1000u;

/**
* @brief An interface which implements the TimeProvider generic plugin interface to provide the base primitive for time and sleep source, backed by TCN.
* @details The interface has essentially two main methods, beside ancillary ones, which can be used to:
* - Provide a source for a time-based incrementing counter;
* - Provide a sleeping method.
*
* Provider supports several operating modes, which are backed by primitives found in tcn.h header.
* For each of the supported modes, two factors are changing: one is the Counter() and the other the Sleep() behaviour.
*
* - NoPollLegacyMode:   Counter() = Falls back to HighResolutionTimer::Counter() for the source;
*                       Sleep() = Spinning on the counter value to measure intervals;
* - PollLegacyMode:     Counter() = Polls its own Counter() method which is backed by the tcn_get_time;
*                       Sleep() = Spinning on the counter to measure intervals;
* - WaitUntilMode:      Counter() = Polls its own Counter() method which is backed by the tcn_get_time;
*                       Sleep() = Blocks current thread execution by relying on the backing tcn_wait_until function;
* - WaitUntilHRMode:    Counter() = Polls its own Counter() method which is backed by the tcn_get_time;
*                       Sleep() = Blocks current thread execution by relying on the backing tcn_wait_until_hr function;
*                                 Backing function accepts a Tolerance parameter and returns the wakeup time to compute difference;
* - SleepMode:          Counter() = Polls its own Counter() method which is backed by the tcn_get_time;
*                       Sleep() = Uses the backing tcn_sleep function to achieve POSIX style sleep() using the sourcing peripheral;
* - SleepHRMode:        Counter() = Polls its own Counter() method which is backed by the tcn_get_time;
*                       Sleep() = Uses the backing tcn_sleep_hr function to achieve POSIX style sleep() using the sourcing peripheral;
*                       Backing function accepts a Tolerance parameter and returns the difference between expected and effective sleep duration;
* Period and frequency methods
* They return the current provider Period and Frequency, which are bound to the p = 1/f relationship.
*
* Usage
* The TcnTimeProvider plugin is generally configured inside the timer instance, as the LinuxTimer discovers and instantiates it as long as
* the MARTe2 configuration file is parsed. The general structure is:
* +Timer = {
*        Class = LinuxTimer
*        [...] Linux Timer specific configuration options [...]
*        +TcnProvider = {
            Class = TcnTimeProvider                             //Mandatory.
            TcnDevice = "/path/to/tcndevice/configuration.xml"  //Mandatory, path to the xml configuration for the tcn provider
            TcnPoll = 0|1                                       //Optional, parameter required for backward compatibility (Behaviour is 0 = NoPollLegacyMode | 1 = PollLegacyMode)
                                                                //The TcnPoll parameter presence overrides OperationMode. If both are present, only TcnPoll is considered.
            OperationMode = "[see above supported]"             //Optional, defaults to NoPollLegacyMode.
            Tolerance = 1000                                    //Optional, value in us only valid for HR modes (see above), defaults to 1000us.
*        }
* }
* 
* Caveats
* If the TCN initialisation fails, the DataSource immediately fails. Otherwise if an unsupported mode is selected, the Sleep function verbosely fails but the DataSource
* may keep on running with the TCN library fallback implementation. Specific errors which may be produced from the TCN underlying library are propagated using either the
* ERRNO naming (EACCES, ENOSYS, ENODEV, ENODATA, ENOENT, ENOKEY, EPERM, EBUSY). As the library brings also proprietary error codes, when they cannot be led back to their
* standard binding they are printed as integer.
*/
class TcnTimeProvider: public TimeProvider {
    public:
        CLASS_REGISTER_DECLARATION()

        /**
        * @brief Default constructor for the TCN Time Provider
        */
        TcnTimeProvider();
        
        /**
        * @brief Destructor for the TCN Time Provider
        */
        virtual ~TcnTimeProvider();

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
        virtual bool Sleep(uint64 start, uint64 delta);

        /**
        * @brief For full motivation of this method, see interface documentation. In this specific case, brings down
        *        to the plugin, the Tcn related configuration parameters which still reside in the Datasource, which now is only in charge
        *        of the plugin instantiation. In particular, the TcnPoll parameter is brought to the plugin.
        * @param[in] compatibilityData Data which is injected from the plugin management DataSource
        * @return True if configuration operation succeeds. False otherwise.
        */
        virtual bool BackwardCompatibilityInit(StructuredDataI &compatibilityData);

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
        * @brief Pointer to the specific sleep strategy implementation
        */
        bool (TcnTimeProvider::*BusySleepProvider)(uint64, uint64);

        /**
        * @brief HighResolutionTimer counter source
        * This is kept here due to legacy reasons (backward compatibility to support TcnPoll = 0)
        */
        uint64 HRTCounter() const;

        /**
        * @brief TCN counter source
        */
        uint64 TCNCounter() const;

        /**
        * @brief Pointer to the specific counter strategy implementation
        */
        uint64 (TcnTimeProvider::*CounterProvider)() const;

        bool InnerInitialize(StructuredDataI &data);

        /**
        * @brief Null delegate as dummy for initial configuration, to avoid erratic default behaviour.
        * Essentially it does nothing, only fails.
        */
        bool NullDelegate(const uint64 start, const uint64 delta);

        /**
        * @brief Uses the HighResolutionTimer as source for the sleep, assuming the driver correction happens transparently.
        */
        bool NoPollBSP(const uint64 start, const uint64 delta);

        /**
        * @brief Polls the Counter() method to compute the sleep duration and wakeup instant
        */
        bool PollBSP(const uint64 start, const uint64 delta);

        /**
        * @brief Uses the TCN wait until method to sleep (Refer to tcn_wait_until docs)
        */
        bool WaitUntilBSP(const uint64 start, const uint64 delta);

        /**
        * @brief Uses the TCN wait until with high resolution method to sleep (Refer to tcn_wait_until_hr docs)
        */
        bool WaitUntilHRBSP(const uint64 start, const uint64 delta);

        /**
        * @brief Uses the TCN native sleep (POSIX style) method to sleep (Refer to tcn_sleep docs)
        */
        bool SleepBSP(const uint64 start, const uint64 delta);

        /**
        * @brief Uses the TCN native sleep (POSIX style) with high resolution method to sleep (Refer to tcn_sleep docs)
        */
        bool SleepHRBSP(const uint64 start, const uint64 delta);
    };
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SOURCE_COMPONENTS_DATASOURCES_LINUXTIMER_TCNTIMEPROVIDER_H_ */

