/**
 * @file MarkerBitCheckerTest.h
 * @brief Header file for class MarkerBitCheckerTest.
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
 * MarkerBitCheckerTest with all of its public, protected and private members.
 * It may also include definitions for inline methods which need to be visible
 * to the compiler.
 */
	
#ifndef NI9157MARKERNITCHECKERTEST_H_
#define NI9157MARKERNITCHECKERTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "ConfigurationDatabase.h"
#include "MarkerBitChecker.h"
#include "ObjectRegistryDatabase.h"
#include "StandardParser.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
using namespace MARTe;

/**
 * @brief Tests the MarkerBitCheckerTest methods.
 */
class MarkerBitCheckerTest {
public:

    /**
     * @brief MarkerBitCheckerTest Constructor.
     */
    MarkerBitCheckerTest();

    /**
     * @brief MarkerBitCheckerTest Destructor.
     */
    virtual ~MarkerBitCheckerTest();

    /**
     * @brief Tests the MarkerBitChecker Constructor.
     */
    bool TestConstructor();

    /**
     * @brief Tests the MarkerBitChecker::Initialise method.
     */
    bool TestInitialise();

    /**
     * @brief Tests the MarkerBitChecker::Initialise method which retruns false
     * when the SampleChecker::Initialise method retruns false in the absence 
     * of the SampleChecker parameter.
     */
    bool TestInitialise_FalseSampleChecker();

    /**
     * @brief Tests the MarkerBitChecker::Initialise method which retruns false
     * when the parameter NumOfFrameForSyncDiffOne from 
     * SampleChecker::Initialise method is different from 1.
     */
    bool TestInitialise_FalseNumOfFrameForSyncDiffOne();

    /**
     * @brief Tests the MarkerBitChecker::Initialise method which retruns false
     * when the parameter MarkerBitMask is not defined.
     */
    bool TestInitialise_FalseNoMarkerBitMask();

    /**
     * @brief Tests the MarkerBitChecker::Initialise method when the parameter
     * ResetBitMask is not defined.
     */
    bool TestInitialise_NoResetBitMask();

    /**
     * @brief Tests the MarkerBitChecker::Check method.
     */
    bool TestCheck();

    /**
     * @brief Tests the MarkerBitChecker::Check method which fails when the 
     * Marker and the MarkerBitMask do not match.
     */
    bool TestCheck_FailMarkerMismatch();

    /**
     * @brief Tests the MarkerBitChecker::Synchronise method.
     */
    bool TestSynchronise();

    /**
     * @brief Tests the MarkerBitChecker::Synchronise method with the marker in
     * an index at the middle of the frames.
     */
    bool TestSynchronise_MarkerInTheMidle();

    /**
     * @brief Tests the MarkerBitChecker::Synchronise method with the marker in
     * an index at the middle of the last frame.
     */ 
    bool TestSynchronise_MarkerInTheTail();

    /**
     * @brief Tests the MarkerBitChecker::Synchronise method which retruns
     * false when the marker is not found in the frames.
     */
    bool TestSynchronise_FalseMarkerNotFound();

    /**
     * @brief Tests the MarkerBitChecker::Synchronise method which retruns
     * false when the sizeToRead argument is zero.
     */
    bool TestSynchronise_FalseSizeToReadZero();

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* NI9157MARKERNITCHECKERTEST_H_ */
