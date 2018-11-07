/**
 * @file CRCHelperTTest.h
 * @brief Header file for class CRCHelperTTest
 * @date Oct 30, 2018
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

 * @details This header file contains the declaration of the class CRCHelperTTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef TEST_COMPONENTS_GAMS_CRCGAM_CRCHELPERTTEST_H_
#define TEST_COMPONENTS_GAMS_CRCGAM_CRCHELPERTTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief Test class for CRCHelperT
 */
class CRCHelperTTest {
public:

    /**
     * @brief Generic test constructor function to be used by TestConstructorUint8, TestConstructorUint16
     * TestConstructorUint32
     */
    template <typename T>
    bool TestConstructor();

    /**
     * @brief TestConstructor with type = uint8
     */
    bool TestConstructorUint8();

    /**
     * @brief TestConstructor with type = uint16
     */
    bool TestConstructorUint16();

    /**
     * @brief TestConstructor with type = uint32
     */
    bool TestConstructorUint32();

    /**
     * @brief Test the function ComputeTable for all the supported types
     */
    bool TestComputeTable();

    /**
     * @brief Test the function Compute for all the supported types
     */
    bool TestCompute();

};


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* TEST_COMPONENTS_GAMS_CRCGAM_CRCHELPERTTEST_H_ */
	
