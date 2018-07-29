/**
 * @file TimeCorrectionGAM.h
 * @brief Header file for class TimeCorrectionGAM
 * @date 07/06/2018
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

 * @details This header file contains the declaration of the class TimeCorrectionGAM
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef TIMECORRECTIONGAM_H_
#define TIMECORRECTIONGAM_H_

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

namespace MARTe{
/**
 * @brief GAM which allows to estimate the next time-stamp value in a continuous time stream.
 *
 * @details In systems that are expected to produce a continuous time stream at fixed frequency, this GAM can be used to handle
 *  situations where a given point in time is missing (e.g. the time variation between two consecutive cycles is not the expected period).

 * The user must configure the expected time difference (delta) between two consecutive acquired time-stamps, together
 * with the maximum allowed delta tolerance. If the delta is in the admissible range, it is used to compute an estimation that
 * will later be used when invalid time-stamps are detected (i.e when the delta is out of the admissible range). The estimation is computed
 * with a first order filter:\n
 *   delta_estimated=(1-filter_gain)*delta_estimated+filter_gain*delta\n
 * with \a filter_gain to be configured by the user as a value between 0 and 1. Values of the gain which are closer to one will force the the filter to quickly
 * follow the current delta values, otherwise, if the gain is close to zero, the filter will have has a slower dynamic and weight previous estimations of delta.
 *
 * When a wrong time-stamp is detected, the output corrected time-stamp is computed as:\n
 *   timestamp_out=last_valid_timestamp+delta_estimated\n
 *
 * If the input time-stamp is valid (namely it belongs to the range defined by the user in the configuration), the time-stamp is exactly copied to
 * the output and the delta difference with the previous time-stamp is used to compute the estimation of the delta.
 *
 * @details This GAM accepts only one uint64 as the input signal, containing the acquired time-stamp. In the output, the first uint64 signal must be
 * the corrected time-stamp and, optionally, a uint8 signal can be used to check if the GAM has corrected (=1) or not (=0) the time_stamp
 * in the current cycle.
 *
 * @details The configuration syntax is (names and signal quantity are only given as an example):
 * <pre>
 * +TimeCorrectionGAM_0 = {
 *     Class = TimeCorrectionGAMTestGAM
 *     ExpectedDelta=1000000 //Between two cycle the InputTime difference shall be 1000000
 *     DeltaTolerance=20 //With a maximum absolute error of 20
 *     FilterGain=0.1 //The following gain will be used to compute the delta to be used when the difference between two time-stamps is greater than 20
 *     InputSignals = {
 *         InputTime = {
 *             DataSource = Drv1
 *             Type = uint64
 *             Frequency = 0
 *          }
 *      }
 *      OutputSignals = {
 *          CorrectedTime = {
 *              DataSource = DDB
 *              Type = uint64
 *          }
 *          IsCorrected = {
 *              DataSource = DDB
 *              Type = uint8
 *          }
 *      }
 *  }
 * </pre>
 */
class TimeCorrectionGAM: public GAM {

public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor.
     * @details Initialises the variables.
     */
    TimeCorrectionGAM();

    /**
     * @brief Destructor
     */
    virtual ~TimeCorrectionGAM();

    /**
     * @see GAM::Initialise
     * @details The user must configure the following parameters:\n
     *   ExpectedDelta (uint64): the expected delta difference between two consecutive time-stamps\n
     *   DeltaTolerance (uint64): defines the range in which an acquired time-stamp is considered valid and, hence,
     *     it will not be corrected. The range is [ExpectedDelta-DeltaTolerance, ExpectedDelta+DeltaTolerance].\n
     *   FilterGain (float32): is the gain used in the first-order filter to compute the delta estimation that is used
     *     to correct the wrong time-stamps\n
     * @param[in] data @see GAM::Initialise
     * @return true if all the parameters above are specified.
     */
    virtual bool Initialise(StructuredDataI & data);

    /**
     * @see GAM::Setup
     * @brief Checks if the input signal (acquired time-stamp) is unique, scalar and if it is uint64 type. Checks that the first output signal
     * (corrected time-stamp) is scalar and uint64 type and that the second signal (is corrected flag) is scalar and uint8 type.
     * @return true if all the checks succeeds, false otherwise.
     */
    virtual bool Setup();

    /**
     * @brief If necessary, corrects the input time-stamp.
     * @details If the input time-stamp is valid (namely it belongs to the range defined by the user in the configuration), the time-stamp is exactly copied to
     * the output and the delta difference with the previous time-stamp is used to compute the estimation of the delta. If the time-stamp is wrong,
     * the corrected time-stamp is computed as the last time-stamp output plus the estimated delta.
     * @return true.
     */
    virtual bool Execute();

protected:

    /**
     * The expected delta difference between two consecutive
     * time-stamps in input
     */
    uint64 expectedDelta;

    /**
     * The admissible delta tolerance
     */
    uint64 deltaTolerance;

    /**
     * The gain of the first order filter used
     * to compute the delta estimation
     */
    float32 filterGain;

    /**
     * The estimation of delta
     */
    float64 estimatedDelta;

    /**
     * Accelerator to the memory of the input time-stamp signal
     */
    uint64 *inputTime;

    /**
     * Accelerator to the memory of the output time-stamo
     * corrected signal
     */
    uint64 *correctedTime;

    /**
     * Accelerator to the memory of the output "is corrected" flag
     */
    uint8* corrected;

    /**
     * Used to store the last time-stamp output
     */
    uint64 lastValidTime;

    /**
     * Used to store the first time-stamp
     * (delta cannot be computed at the first cycle)
     */
    uint8 iterationCounter;

};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* TIMECORRECTIONGAM_H_ */

