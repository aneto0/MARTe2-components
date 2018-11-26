/**
 * @file EPICSPVAHelperTest.h
 * @brief Header file for class EPICSPVAHelperTest
 * @date 21/11/2018
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

 * @details This header file contains the declaration of the class EPICSPVAHelperTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef EPICSPVA_EPICSPVAHELPERTEST_H_
#define EPICSPVA_EPICSPVAHELPERTEST_H_

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
 * @brief Tests the EPICSPVAHelper public methods.
 */
class EPICSPVAHelperTest {
public:
    /**
     * @brief Tests the GetStructure method.
     */
    bool TestGetStructure();

    /**
     * @brief Tests the InitStructure method.
     */
    bool TestInitStructure();

    /**
     * @brief Tests the ReplaceStructureArray method with a structured array at the root level.
     */
    bool TestReplaceStructureArray();

    /**
     * @brief Tests the GetStructure method with an unregistered type. Not sure how I can force this error...
     */
    //bool TestGetStructure_False_UnregisteredType();
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* EPICSPVA_EPICSPVAHELPERTEST_H_ */

