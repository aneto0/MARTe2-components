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

#ifndef STATISTICSGAM_H_
#define STATISTICSGAM_H_

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
 * @brief GAM which provides average, standard deviation, minimum and maximum of
 * its input signal over a moving time window.
 * @details This GAM provides the average, standard deviation, minimum and maximum
 * of its input signal over a moving time window. 
 * The GAM accepts any type of scalar
 * input signal, i.e. (u)int8, (u)int16, (uint32), (u)int64, float32 and float64, and
 * produces the statistics computation in the same native type. As such, the output
 * signals are required to conform to the type of the input signal.
 *
 * The configuration syntax is (names and signal quantity are only given as an example):
 * <pre>
 * +Statistics = {
 *     Class = StatisticsGAM
 *     WindowSize = 1000 // Optional - Defaults to 1024, the window size for integer types is limited to power
 *                          of 2, e.g. 64, 1024, etc. In case a different window size is specified, the
 *                          highest power of 2 smaller than the specified size is being considered.
 *     StartCycleNumber = 0u // Optional - Defaults to 0. GAM cycles to skip before starting the accumulation.
 *     InfiniteMaxMin = 0u // Optional - Defaults to 0 (false). If true, Max and Min are referred to the GAM lifecycle (until reset).
 *                            If false, Max and Min are referred only to the Sliding Window.
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
 *         ExecutionTime_std = {  // Optional - Standard deviation computed in case of 
 *             DataSource = "DDB" //            this second output signal, incl. square 
 *             Type = uint64      //            root computation.
 *         }
 *         ExecutionTime_min = {  // Optional but constrained to having added the _std signal before
 *             DataSource = "DDB"
 *             Type = uint64
 *         }
 *         ExecutionTime_max = {  // Optional but constrained to having added the _min signal before
 *             DataSource = "DDB"
 *             Type = uint64
 *         }
 *     }
 * }
 * </pre>
 *
 * \b TODO Receive inputs signal depth in lieu of storing history internally.
 *
 * \b TODO Since the RMS is the native computed value being the STD, it can be promoted
 * to be available on an output signal without additional penalty.
 *
 * \b TODO Deliver histogram as output array.
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
     *   stats = (void*) new StatisticsHelperT<signalType> (windowSize);
     */
    virtual bool Setup();

    /**
     * @brief Execute method. Statistical computation of the input signal.
     * @details Delegates execution of the statistical computation and update
     * of the output signals to the Execute<signaType> method.
     * @return true.
     */
    virtual bool Execute();

    /**
     * @brief Resets the sample history buffer.
     * @return true.
     */
    virtual bool PrepareNextState(const char8 * const currentStateName,
                                  const char8 * const nextStateName);

  private:
    /**
     * @brief The reference to the statistics computation templated class.
     * @details The void * stores the reference to the StatisticsHelperT<>
     * instance which is created with the Setup() method. This attribute
     * requires a static_cast<StatisticsHelperT<signalType> *> before use.
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
    template <typename Type> bool ExecuteT();

    /**
     * Start computing the statistics only after startCycleNumber cycles.
     */
    uint32 startCycleNumber;

    /**
     * Number of ellapsed cycles.
     */
    uint32 cycleCounter;

    /**
     * Always compute max/min? See class description.
     */
    bool infiniteMaxMin;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* STATISTICSGAM_H_ */

