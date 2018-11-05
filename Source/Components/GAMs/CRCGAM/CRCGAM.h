/**
 * @file CRCGAM.h
 * @brief Header file for class CRCGAM
 * @date 30 Oct 2018
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

 * @details This header file contains the declaration of the class CRCGAM
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SOURCE_COMPONENTS_GAMS_CRCGAM_CRCGAM_H_
#define SOURCE_COMPONENTS_GAMS_CRCGAM_CRCGAM_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "GAM.h"
#include "CRCHelperT.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief GAM which implements the computation of a CRC checksum.
 *
 * @details This GAM computes a CRC checksum on the InputSignals memory area and returns it on the output signal.
 * Output types supported are: uint8 - uint16 - uint32.
 *
 * The configuration setup must include:
 * - The divisor Polynomial with (n+1) bits for the CRC-n type chosen.
 *   Common values for polynomial are: 0x7 (uint8), 0x1021 (uint16), 0x4C11DB7 (uint32).
 * - The initialCRCvalue.
 * - The Inverted option: 1 is you want the CRC checksum reflected, 0 otherwise.
 *
 * The number of OutputSignals must be equal to 1.
 *
 * The configuration syntax is (names and signal quantities are only given as an example):
 * <pre>
 * +CRCGAM1 = {
 *     Class = CRCGAM
 *     Polynomial = 0x1021
 *     InitialValue = 0x0
 *     Inverted = 0
 *     InputSignals = {
 *         inputArea = {
 *             DataSource = DDB1
 *         }
 *     }
 *     OutputSignals = {
 *         crc = {
 *             DataSource = DDB1
 *             Type = uint16
 *         }
 *     }
 * }
 *</pre>
 */

    class CRCGAM : public GAM {
    public:
        CLASS_REGISTER_DECLARATION()

        /**
         * @brief Default Constructor. NOOP.
         */
        CRCGAM();

        /**
         * @brief Default Destructor. NOOP.
         */
        virtual ~CRCGAM();

        /**
         * @brief see GAM::Initialise.
         * @details Stores the GAM configuration in order to read Polynomial, InitialValue and Inverted option.
         */
        virtual bool Initialise(StructuredDataI &data);

        /**
         * @brief Verifies the correctness of the GAM configuration.
         * @details Retrieves byte size of input signals, computes the table from the polynomial and checks that the rules below are met.
         * @return true if all the preconditions are met.
         * @pre
         *     GetNumberOfOutputSignals() == 1 &&
         *     GetSignalType(OutputSignals, 0u) == uint8 or uint16 or uint32
         */
        virtual bool Setup();

        /**
         * @brief Computes the CRC checksum from the stored data.
         */
        virtual bool Execute();

    private:

        /**
         * Memory of the CRCGAM input signals.
         */
        uint8 * inputData;

        /**
         * Memory of the CRCGAM output signals.
         */
        void * outputData;

        /**
         * The CRCHelper for type independent CRC computation.
         */
        CRCHelper * crcHelper;

        /**
         * The divisor polynomial.
         */
        uint32 polynomial;

        /**
         * Initial value for the CRC computation.
         */
        uint32 initialCRCValue;

        /**
         * Byte size of InputSignals.
         */
        uint32 size;

        /**
         * TypeDescriptor for the out signal type.
         */
        TypeDescriptor outputSignalType;

        /**
         * Flag for inverted result.
         */
        uint8 isReflected;

    };

}


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SOURCE_COMPONENTS_GAMS_CRCGAM_CRCGAM_H_ */

