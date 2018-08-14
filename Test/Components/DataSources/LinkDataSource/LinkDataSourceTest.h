/**
 * @file LinkDataSourceTest.h
 * @brief Header file for class LinkDataSourceTest
 * @date 23/07/2018
 * @author Giuseppe Ferro
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

 * @details This header file contains the declaration of the class LinkDataSourceTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef LINKDATASOURCETEST_H_
#define LINKDATASOURCETEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "LinkDataSource.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

/**
 * @brief Tests all the LinkDataSource methods
 */
class LinkDataSourceTest {
public:

    /**
     * @brief Constructor
     */
    LinkDataSourceTest();

    /**
     * @brief Destructor
     */
    virtual ~LinkDataSourceTest();

    /**
     * @brief Tests the constructor
     */
    bool TestConstructor();

    /**
     * @brief Tests the LinkDataSource::Initialise() method
     */
    bool TestInitialise();

    /**
     * @brief Tests the LinkDataSource::Initialise() method that fails if
     * IsWriter is not defined
     */
    bool TestInitialise_FalseNoIsWriter();

    /**
     * @brief Tests the LinkDataSource::Initialise() method that fails if
     * Link is not defined
     */
    bool TestInitialise_FalseNoLink();

    /**
     * @brief Tests the LinkDataSource::Initialise() method that fails if
     * Link points to an invalid path.
     */
    bool TestInitialise_FalseInvalidLink();

    /**
     * @brief Tests the LinkDataSource::AllocateMemory() method
     */
    bool TestAllocateMemory();

    /**
     * @brief Tests the LinkDataSource::AllocateMemory() method that fails if
     * two or more components declare different memory sizes
     */
    bool TestAllocateMemory_FalseDifferentSizes();

    /**
     * @brief Tests the LinkDataSource::PrepareNextState() method
     */
    bool TestPrepareNextState();

    /**
     * @brief Tests the LinkDataSource::Synchronise() method
     */
    bool TestSynchronise();

    /**
     * @brief Tests the LinkDataSource::GetBrokerName() method
     */
    bool TestGetBrokerName();

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* LINKDATASOURCETEST_H_ */

