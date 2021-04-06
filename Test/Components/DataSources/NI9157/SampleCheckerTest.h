/**
 * @file SampleCheckerTest.h
 * @brief Header file for class SampleCheckerTest.
 * @date 26/03/2021
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
 * SampleCheckerTest with all of its public, protected and private members.
 * It may also include definitions for inline methods which need to be visible
 * to the compiler.
 */

#ifndef NI9157SAMPLECHECKERTEST_H_
#define NI9157SAMPLECHECKERTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "ConfigurationDatabase.h"
#include "SampleChecker.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
using namespace MARTe;

/**
 * @brief Tests the SampleCheckerTest methods.
 */
class SampleCheckerTest {
public:

    /**
     * @brief SampleCheckerTest Constructor.
     */
    SampleCheckerTest();

    /**
     * @brief SampleCheckerTest Destructor.
     */
    virtual ~SampleCheckerTest();

    /**
     * @brief Tests the SampleChecker Constructor.
     */
    bool TestConstructor();

    /**
     * @brief Tests the SampleChecker::Initialise method.
     */
    bool TestInitialise();

    /**
     * @brief Tests the SampleChecker::Initialise method with the 
     * NumOfFrameForSync parameter not set.
     */
    bool TestInitialise_NoNumOfFrameForSync();

    /**
     * @brief Tests the SampleChecker::Initialise method which retuns false
     * when the SampleSize parameter is not defined.
     */
    bool TestInitialise_NoSampleSize();

    /**
     * @brief Tests the SampleChecker::Initialise method which retuns false
     * when the NumOfFrameForSync parameter is zero.
     */
    bool TestInitialise_NumOfFrameForSyncZero();

    /**
     * @brief Tests the SampleChecker::GetNumberOfFramesToSync method.
     */
    bool TestGetNumberOfFramesToSync();

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* NI9157SAMPLECHECKERTEST_H_ */
