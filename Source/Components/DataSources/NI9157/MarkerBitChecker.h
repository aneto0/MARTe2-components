/**
 * @file MarkerBitChecker.h
 * @brief Header file for class MarkerBitChecker
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
 * @details This header file contains the declaration of the class MarkerBitChecker
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef NI9157MARKERBITCHECKER_H_
#define NI9157MARKERBITCHECKER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "SampleChecker.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {

class MarkerBitChecker: public SampleChecker {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief TODO - WAS MISSING.
     * @details TODO - WAS MISSING.
     */
    MarkerBitChecker();

    /**
     * @brief TODO - WAS MISSING.
     * @details TODO - WAS MISSING.
     */
    virtual ~MarkerBitChecker();

    /**
     * @brief TODO - WAS MISSING.
     * @details TODO - WAS MISSING.
     * @param[in] data TODO - WAS MISSING.
     * @return TODO - WAS MISSING.
     */
    virtual bool Initialise(StructuredDataI &data);

    /**
     * @brief TODO - WAS MISSING.
     * @details TODO - WAS MISSING.
     * @param[in] sample TODO - WAS MISSING.
     * @param[in] write TODO - WAS MISSING.
     * @return TODO - WAS MISSING.
     */
    virtual bool Check(uint8 *sample,
                       bool &write);

    /**
     * @brief TODO - WAS MISSING.
     * @details TODO - WAS MISSING.
     * @param[in] frames TODO - WAS MISSING.
     * @param[in] sizeToRead TODO - WAS MISSING.
     * @param[in] idx TODO - WAS MISSING.
     * @param[in] write TODO - WAS MISSING.
     * @return TODO - WAS MISSING.
     */
    virtual bool Synchronise(uint8 *frames,
                             uint32 sizeToRead,
                             uint32 &idx,
                             bool &write);

private:

    /**
     * TODO - WAS MISSING.
     */
    uint64 bitMask;

    /**
     * TODO - WAS MISSING.
     */
    uint64 resetBitMask;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* NI9157MARKERBITCHECKER_H_ */
