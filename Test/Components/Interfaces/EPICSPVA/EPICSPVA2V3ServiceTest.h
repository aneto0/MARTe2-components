/**
 * @file EPICSPVA2V3ServiceTest.h
 * @brief Header file for class EPICSPVA2V3ServiceTest
 * @date 16/10/2018
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

 * @details This header file contains the declaration of the class EPICSPVA2V3ServiceTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef EPICSPVA_EPICSPVA2V3SERVICETEST_H_
#define EPICSPVA_EPICSPVA2V3SERVICETEST_H_

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
 * @brief Tests the EPICSPVA2V3Service public methods.
 */
class EPICSPVA2V3ServiceTest {
public:
    /**
     * @brief Tests the constructor.
     */
    bool TestConstructor();

    /**
     * @brief Tests the Initialise method.
     */
    bool TestInitialise();

    /**
     * @brief Tests that the Initialise method fails if no Polynomial is set.
     */
    bool TestInitialise_False_Polynomial();

    /**
     * @brief Tests that the Initialise method fails if no CRCInitialValue is set.
     */
    bool TestInitialise_False_CRCInitialValue();

    /**
     * @brief Tests that the Initialise method fails if no CRCFinalXOR is set.
     */
    bool TestInitialise_False_CRCFinalXOR();

    /**
     * @brief Tests that the Initialise method fails if no Structure is set.
     */
    bool TestInitialise_False_Structure();

    /**
     * @brief Tests the request method.
     */
    bool Testrequest();

    /**
     * @brief Tests that the request method fails if no qualifier is set.
     */
    bool Testrequest_False_qualifier();

    /**
     * @brief Tests that the request method fails if no hash is set when loading.
     */
    bool Testrequest_False_hash();

    /**
     * @brief Tests that the request method fails if no value is set when loading.
     */
    bool Testrequest_False_value();

    /**
     * @brief Tests that the request method fails if there is an hash mismatch when loading.
     */
    bool Testrequest_False_hash_mismatch();

};




/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* EPICSPVA_EPICSPVA2V3SERVICETEST_H_ */
