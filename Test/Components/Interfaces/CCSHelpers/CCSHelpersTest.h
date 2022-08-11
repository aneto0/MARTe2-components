/**
 * @file CCSHelpersTest.h
 * @brief Header file for class CCSHelpersTest
 * @date 25/05/2022
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

 * @details This header file contains the declaration of the class CCSHelpersTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef CCS_HELPERSTEST_H_
#define CCS_HELPERSTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "CCSHelpers.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
/**
 * @brief Tests the CCSHelpers public methods.
 */
class CCSHelpersTest {
public:
    /**
     * @brief CCSHelpersTest contructor.
     * NOOP
     */
    CCSHelpersTest();

    /**
     * @brief CCSHelpersTest destructor.
     * NOOP
     */
    virtual ~CCSHelpersTest();

    /**
     * @brief Tests the MARTeToCCSAnyValue method from an AnyType.
     */
    bool TestMARTeToCCSAnyValue();

    /**
     * @brief Tests the MARTeToCCSAnyValue method from a StructuredDataI.
     */
    bool TestMARTeToCCSAnyValue_StructuredDataI();

    /**
     * @brief Tests the GetMARTeBasicType method.
     */
    bool TestGetMARTeBasicType();

    /**
     * @brief Tests the CCSToMARTeAnyObject method.
     */
    bool TestCCSToMARTeAnyObject();

    /**
     * @brief Tests the CCSToMARTeAnyObject method with a string array.
     */
    bool TestCCSToMARTeAnyObject_StringArray();

    /**
     * @brief Tests the CCSToMARTeAnyObject method with an input structure.
     */
    bool TestCCSToMARTeAnyObject_IntrospectedType();

    /**
     * @brief Tests the CCSToMARTeAnyObject method with a StructuredDataI output.
     */
    bool TestCCSToMARTeAnyObject_StructuredData();

    /**
     * @brief Tests the CCSToMARTeStructuredDataI method.
     */
    bool TestCCSToMARTeStructuredDataI();

    /**
     * @brief Tests the GetCCSBasicType method.
     */
    bool TestGetCCSBasicType();
};
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* CCS_HELPERSTEST_H_ */
