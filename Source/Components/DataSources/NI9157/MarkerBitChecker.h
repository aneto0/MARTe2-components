/**
 * @file MarkerBitChecker.h
 * @brief Header file for class MarkerBitChecker.
 * @date 11/02/2021
 * @author Giuseppe Ferro
 * @author Pedro Lourenco
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
 *
 * @details This header file contains the declaration of the class
 * MarkerBitChecker with all of its public, protected and private members. It
 * may also include definitions for inline methods which need to be visible to
 * the compiler.
 */

#ifndef NI9157MARKERBITCHECKER_H_
#define NI9157MARKERBITCHECKER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "SampleChecker.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {

/**
 * @brief The MarkerBitChecker class.
 * @see MarkerBitChecker::Initialise for details on the configuration
 * parameters.
 */
class MarkerBitChecker: public SampleChecker {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor.
     * @post
     *  resetBitMask = 0ull;
     *  bitMask = 0ull;
     */
    MarkerBitChecker();

    /**
     * @brief Destructor.
     */
    virtual ~MarkerBitChecker();

    /**
     * @see SampleChecker::Initialise.
     * @details Follows the list of parameters to be configured by the user
     * for initialisation: \n
     * MarkerBitMask: The bit mask to be used in the checking process. This is
     * parameter must be set. \n
     * ResetBitMask: The reset bit layout for the bit mask. If not set, the
     * value of the MarkerBitMask is used by default.
     * @details Please notice that NumOfFrameForSync must be equal to 1.
     * @return true if NumOfFrameForSync is equal to 1 and MarkerBitMask is
     * set, othewise retruns false.
     */
    virtual bool Initialise(StructuredDataI &data);

    /**
     * @see SampleChecker::Check.
     * @details Checks if the value of sample corresponds to the value of the
     * set BitMask.
     * @param[in,out] sample is the samples value to be bit checked.
     * @param[in,out] write is always set too true.
     * @return true if the result of the bitwise AND between (i) sample and
     * (ii) BitMask values is not zero. Then the sample value is written back
     * with the result of the bitwise AND operation between (i) the sample
     * value and (ii) the bitwise NOT of ResetBitMask value.
     */
    virtual bool Check(uint8 *sample,
                       bool &write);

    /**
     * @see SampleChecker::Synchronise.
     * @details Calls Check and searches for the MarkerBitMask along frames
     * with size sizeToRead.
     * @param[in,out] frames the frames to be checked.
     * @param[in] sizeToRead the size in bytes to read in the previous frames.
     * @param[in,out] idx the index corresponding to the position in frames
     * where the MarkerBitChecker::Check call first returns true.
     * @param[in,out] write is always set to true.
     * @return true if the Check retruns true for a given index and updates
     * the idx value.
     */
    virtual bool Synchronise(uint8 *frames,
                             uint32 sizeToRead,
                             uint32 &idx,
                             bool &write);

protected:

    /**
     * The bit mask to be used in the checking process.
     */
    uint64 bitMask;

    /**
     * The reset bit layout for the bitmask.
     */
    uint64 resetBitMask;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* NI9157MARKERBITCHECKER_H_ */
