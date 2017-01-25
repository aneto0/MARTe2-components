/**
 * @file ConversionGAM.h
 * @brief Header file for class ConversionGAM
 * @date 16/01/2017
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

 * @details This header file contains the declaration of the class ConversionGAM
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef CONVERSION_GAM_H_
#define CONVERSION_GAM_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "ConversionHelper.h"
#include "GAM.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {
/**
 * @brief GAM which allows to convert between different signal types.
 *
 * @details This GAM converts and copies the input signals to the output signals. A gain can
 * also be specified so that outputSignal[i] = gain[i] * inputSignal[i], where i is the input signal index (see GetNumberOfInputSignals()).
 * If the signal is an array (or has more than one sample), this operation is applied to each element/sample.
 *
 * The number of input and output signals shall be the same, i.e. GetNumberOfInputSignals() == GetNumberOfOutputSignals().
 *
 * For each input signal, the number of elements multiplied by the number of samples shall be the
 * same as the number of elements multiplied by the number of samples in the correspondent output signal.
 * Note that the number of elements or samples can be different, provided that the equality above is respected.
 *
 * The configuration syntax is (names and signal quantities are only given as an example):
 * +Conversion1 = {
 *     Class = ConversionGAM
 *     InputSignals = {
 *         Signal1 = {
 *             DataSource = "Drv1"
 *             Type = uint32
 *             Samples = 20
 *             Elements = 10
 *         }
 *         Signal2 = {
 *             DataSource = "Drv2"
 *             Type = int16
 *         }
 *     }
 *     OutputSignals = {
 *         Signal1 = {
 *             DataSource = "LCD"
 *             Type = float32
 *             Elements = 200
 *             Gain = 3
 *         }
 *         Signal2 = {
 *             DataSource = "LCD"
 *             Type = int32
 *         }
 *     }
 * }
 */
class ConversionGAM: public GAM {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor. NOOP.
     */
ConversionGAM    ();

    /**
     * @brief Destructor. NOOP.
     */
    virtual ~ConversionGAM();

    /**
     * @brief Verifies the correctness of the GAM configuration.
     * @details Checks that the number of input signals is equal to the number of output signals and that the rules below are met.
     * @return is the pre-conditions are met.
     * @pre
     *   SetConfiguredDatabase() &&
     *   GetNumberOfInputSignals() == GetNumberOfOutputSignals() &&
     *   for each signal i: GetSignalByteSize(InputSignals, i) * GetSignalNumberOfSamples(InputSignals, i) == GetSignalByteSize(OutputSignals, i) * GetSignalNumberOfSamples(OutputSignals, i) &&
     *   for each signal i: GetSignalType(InputSignals, i) == uint8 or int8 or uint16 or int16 or uint32 or int32 or uint64 or int64 or float32 or float64
     *   for each signal i: GetSignalType(OutputSignals, i) == uint8 or int8 or uint16 or int16 or uint32 or int32 or uint64 or int64 or float32 or float64
     */
    virtual bool Setup();

    /**
     * @brief see GAM::Initialise.
     * @details Stores the GAM configuration in order to read the Gain of each OutputSignal
     */
    virtual bool Initialise(StructuredDataI & data);

    /**
     * @brief Convert the input signals memory to the output signal memory, eventually multiplying by a gain factor.
     * @return true if all the signals memory can be successfully copied.
     */
    virtual bool Execute();

private:
    /**
     * One ConversionHelperT for each signal.
     */
    ConversionHelper **conversionHelpers;

    /**
     * Number of signals to copy.
     */
    uint32 numberOfSignals;

    /**
     * StructuredData received at Initialise.
     */
    ConfigurationDatabase cdb;
};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* CONVERSION_GAM_H_ */

