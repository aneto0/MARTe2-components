/**
 * @file CRCHelper.h
 * @brief Header file for class CRCHelper
 * @date Oct 29, 2018
 * @author Luca Porzio
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

 * @details This header file contains the declaration of the class CRCHelper
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SOURCE_COMPONENTS_GAMS_CRCGAM_CRCHELPER_H_
#define SOURCE_COMPONENTS_GAMS_CRCGAM_CRCHELPER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {

class CRCHelper {
public:
    /**
     * @brief Destructor. NOOP.
     */
    virtual inline ~CRCHelper();

    /**
     * @brief To be specialised by CRCHelperT for all the supported types.
     * @details Compute a lookup table for a given polynomial.
     * @param[in] The given divisor polynomial.
     */
    virtual void ComputeTable(void * pol) = 0;

    /**
     * @brief To be specialised by CRCHelperT for all the supported types.
     * @details Compute the CRC checksum for given parameters.
     * @param[in] data the bytes against which the CRC will be computed.
     * @param[in] size the number of bytes in \a data.
     * @param[in] initCRC the initial CRC value.
     * @param[in] inputInverted if true, the order of the bytes to compute the CRC will be data[0], data[-1], data[-2] ... data[-size + 1].
     * @param[out] retval the computed CRC checksum.
     */
    virtual void Compute(const uint8 * const data, int32 size, void * initCRC, bool inputInverted, void * retVal) = 0;

};
}



/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

namespace MARTe {

    CRCHelper::~CRCHelper() {

    }

}

#endif /* SOURCE_COMPONENTS_GAMS_CRCGAM_CRCHELPER_H_ */
	
