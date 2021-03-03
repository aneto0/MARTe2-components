/**
 * @file TimeProvider.h
 * @brief Header file for class TimeProvider
 * @date 31 ott 2019
 * @author Giuseppe Ferrò
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

 * @details This header file contains the declaration of the class TimeProvider
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SOURCE_COMPONENTS_DATASOURCES_LINUXTIMER_TIMEPROVIDER_H_
#define SOURCE_COMPONENTS_DATASOURCES_LINUXTIMER_TIMEPROVIDER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "Object.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
* @brief Interface for TimeProvider plugins on the Linux Timer DataSource
*/
class TimeProvider: public Object {
public:

	/**
	* @brief Default constructor
	*/
    TimeProvider();

	/**
	* @brief Destructor
	*/
    virtual ~TimeProvider();

	/**
	* @brief Returns the actual ticks count referred from the internal source
	* @return The actual ticks value
	*/
    virtual uint64 Counter() = 0;

	/**
	* @brief Returns the actual period once every ticks occurs
	* @return The actual period between ticks
	*/
    virtual float64 Period() = 0;

	/**
	* @brief Returns the actual frequency, referred as inverse of the interval between two periods
	* @return The actual tick frequency
	*/
    virtual uint64 Frequency() = 0;

	/**
	* @brief Busy sleeps for an amount of time
	* @param[in] start Tick start
	* @param[in] delta Amount of ticks to busy slee
	*/
    virtual void BusySleep(const uint64 start, const uint64 delta) = 0;
};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SOURCE_COMPONENTS_DATASOURCES_LINUXTIMER_TIMEPROVIDER_H_ */

