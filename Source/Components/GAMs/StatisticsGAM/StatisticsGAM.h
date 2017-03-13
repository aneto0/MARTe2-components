/**
 * @file StatisticsGAM.h
 * @brief Header file for class StatisticsGAM
 * @date 08/03/2017
 * @author Bertrand Bauvir
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

 * @details This header file contains the declaration of the class StatisticsGAM
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef StatisticsGAM_H_
#define StatisticsGAM_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "GAM.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief GAM which provides average, standard devisation, minimum and maximum of
 * its input signal over a moving time window.
 * @details This GAM provides the average, standard devisation, minimum and maximum
 * of its input signal over a moving time window.
 *
 * The configuration syntax is (names and signal quantity are only given as an example):
 * +Statistics = {
 *     Class = StatisticsGAM
 *     WindowSize = 1000 // Optional - Default to 1024
 *     InputSignals = {
 *         ExecutionTime = {
 *             DataSource = "DDB"
 *             Type = uint64
 *             Default = 0
 *         }
 *     }
 *     OutputSignals = {
 *         ExecutionTime_avg = {
 *             DataSource = "DDB"
 *             Type = uint64
 *         }
 *         ExecutionTime_std = { // Optional - Standard deviation comp;uted in presence of 
 *             DataSource = "DDB" //           this second output signal
 *             Type = uint64
 *         }
 *         ExecutionTime_min = {
 *             DataSource = "DDB"
 *             Type = uint64
 *         }
 *         ExecutionTime_max = {
 *             DataSource = "DDB"
 *             Type = uint64
 *         }
 *     }
 * }
 *
 * @todo Receive inputs signal depth in lieu of storing history internally.
 *
 * @todo Configurable size of time window.
 *
 */

class StatisticsGAM: public GAM, public StatefulI {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor.
     * @post 
     *   signalType = InvalidType
     *   stats = NULL_PTR(void *)
     *   windowSize = 1024
     */
    StatisticsGAM();

    /**
     * @brief Destructor. NOOP.
     */
    virtual ~StatisticsGAM();

    /**
     * @brief Initialise the GAM from a configuration file.
     * @param[in] data the GAM configuration.
     * @return true on success.
     */
    virtual bool Initialise(StructuredDataI & data);

    /**
     * @brief Verifies all signals have the same scalar type.
     * @return if the pre-conditions are met.
     * @pre
     *   SetConfiguredDatabase() && GetNumberOfInputSignals() > 0 &&
     *   GetNumberOfOutputSignals() > 0 &&
     *   All signals are scalar and share the same type.
     * @post 
     *   stats = new StatisticsHelperT<signalType> (windowSize)
     */
    virtual bool Setup();

    /**
     * @brief Execute method. Statistical computation of the input signal.
     * @detail Delegates execution of the statistical computation and update
     * of the output signals to the Execute<signaType> method.
     * @return true.
     */
    virtual bool Execute();

    /**
     * @brief Resets the sample buffer.
     * @return true.
     */
    virtual bool PrepareNextState(const char8 * const currentStateName,
                                  const char8 * const nextStateName);

  private:
    /**
     * @brief The reference to the statistics computation templated class.
     */
    void * stats;

    /**
     * @brief The common signal type attribute.
     */
    TypeDescriptor signalType;

    /**
     * @brief The size of the moving window attribute.
     */
    uint32 windowSize;

    /**
     * @brief Templated Execute method. Statistical computation of the input signal.
     * @return true.
     */
    template <typename Type> bool Execute();

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* StatisticsGAM_H_ */

