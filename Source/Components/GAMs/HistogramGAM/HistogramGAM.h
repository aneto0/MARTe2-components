/**
 * @file HistogramGAM.h
 * @brief Header file for class HistogramGAM
 * @date 28/08/2018
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

 * @details This header file contains the declaration of the class HistogramGAM
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef HISTOGRAMGAM_H_
#define HISTOGRAMGAM_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "GAM.h"
#include "HistogramComparatorT.h"
#include "TypeDescriptor.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief A GAM that computes histograms from the input signal values.
 *
 * @details For each input signal the following parameters can be defined:\n
 *   MinLim = [the minimum value of the signal]\n
 *   MaxLim = [the maximum value of the signal]\n
 * All the signals in input must be scalar and the number of samples per cycle must be one.\n
 * Each output signal represents the histogram of the relative input signal, so the number of input and output
 * signals must coincide. The output signal must be an array of at least three elements and the number of elements
 * is the desired number of bins of the histogram. The algorithm divides the signal range (maxLim - minLim)
 * between (numberOfElements-2) to compute each histogram bin range. The bin 0 shows all the occurrences of the signal value
 * below \a minLim, while the last bin shows all the occurrences above \a maxLim. The rest of the occurrences, that fall in
 * the configured range, are divided into the other (numberOfElements-2) bins.\n
 *
 * As an example, let's suppose to have an integer signal and to define:\n
 * In the input signal:\n
 * <pre>
 *   minLim=10
 *   MaxLim=100
 * </pre>
 * In the output signal:\n
 * <pre>
 *   NumberOfElements = 11
 * </pre>
 * Then we will obtain the following histogram with 11 bins that will be incremented if the occurrence \a x is:\n
 *   bin 0: (x < 10)\n
 *   bin 1: (10 <= x < 20)\n
 *   bin 2: (20 <= x < 30)\n
 *   bin 3: (30 <= x < 40)\n
 *   bin 4: (40 <= x < 50)\n
 *   bin 5: (50 <= x < 60)\n
 *   bin 6: (60 <= x < 70)\n
 *   bin 7: (70 <= x < 80)\n
 *   bin 8: (80 <= x < 90)\n
 *   bin 9: (90 <= x < 100)\n
 *   bin 10: (x >= 100)\n
 *
 * The output signals type must be uint32.\n
 * The user can also define the GAM parameter \a BeginCycleNumber that enables the histogram
 * to start counting only after the specified number of MARTe cycles has passed. Default for this parameter is zero.
 *
 * @details
 * The configuration syntax is (names and signal quantities are only given as an example):
 * <pre>
 * +HistogramGAM1 = {
 *     Class = HistogramGAM
 *     BeginCycleNumber = 0 //Optional. Start to compute histogram only after BeginCycleNumber cycles
 *     StateChangeResetName = All //Optional. If set it will reset when the PrepareNextState, nextStaName == StateChangeResetName. If the StateChangeResetName is set to "All", it will always reset.
 *     InputSignals = {
 *         BeginCycleNumber = 10
 *         Signal1 = {
 *             DataSource = "Drv1"
 *             Type = uint32
 *             Samples = 1
 *             NumberOfElements = 1
 *             MinLim = 10
 *             MaxLim = 100
 *         }
 *         Signal1 = {
 *             DataSource = "Drv1"
 *             Type = float32
 *             MinLim = 10.5
 *             MaxLim = 20.5
 *         }
 *     }
 *     OutputSignals = {
 *         Histogram1 = {
 *             DataSource = "DDB"
 *             Type = uint3232
 *             NumberOfElements = 11
 *         }
*          Histogram1 = {
 *             DataSource = "DDB"
 *             Type = uint32
 *             NumberOfElements = 120
 *         }
 *     }
 * }
 * </pre>
 */
class HistogramGAM: public GAM, public StatefulI {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor
     */
    HistogramGAM();

    /**
     * @brief Destructor
     */
    virtual ~HistogramGAM();

    /**
     * @see GAM::Initialise()
     * @details The following parameter can be defined in the configuration:\n
     *   BeginCycleNumber: how many cycles to wait before starting to compute the histogram.
     */
    virtual bool Initialise(StructuredDataI &data);

    /**
     * @see GAM::Setup()
     * @details Checks that:\n
     *   (NumberOfInputSignals==NumberOfOutputSignals)\n
     *   (NumberOfElements == 1) for each input signal\n
     *   (NumberOfSamples == 1) for each input signal\n
     *   (NumberOfElements >= 3) for each output signal\n
     *   (Type == uint32) for each output signal\n
     *  @return true if the conditions above are met.
     */
    virtual bool Setup();

    /**
     * @see GAM::Excute()
     * @details Executes the histogram algorithm counting and dividing the occurrences of the signals
     * among the defined number of bins.
     * @return true
     */
    virtual bool Execute();

    /**
     * @brief Resets the histogram counters if the nextStateName is equal to the StateChangeReset name.
     * @return true.
     */
    virtual bool PrepareNextState(const char8 * const currentStateName,
                                  const char8 * const nextStateName);

protected:

    /**
     * A list of Comparator objects
     * (one for each signal) to
     * return in which bin the occurrence has fallen
     */
    HistogramComparator **comps;

    /**
     * Holds how many cycles to skip before starting
     * the histogram algorithm
     */
    uint32 beginCycle;

    /**
     * Counts the elapsed number of cycles.
     */
    uint32 cycleCounter;

    /**
     * The name of the state to reset the histogram counters.
     */
    StreamString stateChangeResetName;
};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/


#endif /* HISTOGRAMGAM_H_ */

