/**
 * @file CRCHelperT.h
 * @brief Header file for class CRCHelperT
 * @date Oct 29, 2018 TODO Verify the value and format of the date
 * @author root TODO Verify the name and format of the author
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

 * @details This header file contains the declaration of the class CRCHelperT
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SOURCE_COMPONENTS_GAMS_CRCGAM_CRCHELPERT_H_
#define SOURCE_COMPONENTS_GAMS_CRCGAM_CRCHELPERT_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "CRC.h"
#include "CRCHelper.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {
template <typename T>
    class CRCHelperT : public CRCHelper {
    public:
        /**
         * @brief Destructor. NOOP.
         */
        virtual ~CRCHelperT();

        /**
         * @see CRCHelper::ComputeTable
         * @details A cast to the declared output type is performed.
         */
        virtual void ComputeTable(void * pol);

        /**
         * @see CRCHelper::Compute
         * @details A cast to the declared output type is performed.
         */
        virtual void Compute(const uint8 * const data, int32 size, void * initCRC, bool inputInverted, void * retVal);

    private:

        /**
         * CRC template class.
         */
        CRC<T> crc;
    };
}



/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/
namespace MARTe {
    template <typename T>
    CRCHelperT<T>::~CRCHelperT() {

    }

    template <typename T>
    void CRCHelperT<T>::ComputeTable(void * pol) {
        crc.ComputeTable(*static_cast<T*>(pol));
    }

    template <typename T>
    void CRCHelperT<T>::Compute(const uint8 * const data, int32 size, void * initCRC, bool inputInverted, void * retVal) {
        T crcValue = crc.Compute(data, size, * static_cast<T*>(initCRC), inputInverted);
        *static_cast<T*>(retVal) = crcValue;
    }
}
#endif /* SOURCE_COMPONENTS_GAMS_CRCGAM_CRCHELPERT_H_ */
	
