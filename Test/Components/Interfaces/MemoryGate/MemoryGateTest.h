/**
 * @file MemoryGateTest.h
 * @brief Header file for class MemoryGateTest
 * @date 24/07/2018
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

 * @details This header file contains the declaration of the class MemoryGateTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef MEMORYGATE_MEMORYGATETEST_H_
#define MEMORYGATE_MEMORYGATETEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "MemoryGate.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

/**
 * @brief Test all the MemoryGateTest methods
 */
class MemoryGateTest {
public:

    /**
     * @brief Constructor
     */
    MemoryGateTest();

    /**
     * @brief Destructor
     */
    virtual ~MemoryGateTest();

    /**
     * @brief Tests the constructor
     */
    bool TestConstructor();

    /**
     * @brief Tests the MemoryGateTest::Initialise method
     */
    bool TestInitialise();

    /**
     * @brief Tests the MemoryGateTest::Initialise method that fails if a number
     * of buffers greater than 64 is declared in the configuration
     */
    bool TestInitialise_FalseTooManyBuffers();

    /**
     * @brief Tests the MemoryGateTest::Initialise method with default overflow timeout
     */
    bool TestInitialise_DefaultTimeout();

    /**
     * @brief Tests the MemoryGateTest::Initialise method with default number of buffers
     */
    bool TestInitialise_DefaultNBuffers();

    /**
     * @brief Tests the MemoryGateTest::Initialise method without the definition
     * of the memory size in configuration
     */
    bool TestInitialise_NoMemSize();

    /**
     * @brief Tests the MemoryGateTest::SetMemorySize method when the memory
     * size is specified in the configuration
     */
    bool TestSetMemorySize_Initialised();

    /**
     * @brief Tests the MemoryGateTest::SetMemorySize method when the memory
     * size is not specified in the configuration
     */
    bool TestSetMemorySize_NotInitialised();

    /**
     * @brief Tests the MemoryGateTest::MemoryRead method
     */
    bool TestMemoryRead();

    /**
     * @brief Tests the MemoryGateTest::MemoryWrite method
     */
    bool TestMemoryWrite();

    /**
     * @brief Tests the MemoryGateTest::MemoryWrite method when all the
     * write operations counters are reset in case of overflow.
     */
    bool TestMemoryWrite_Overflow();

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* MEMORYGATETEST_H_ */

