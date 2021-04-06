/**
 * @file NI9157MemoryOperationsHelperTest.h
 * @brief Header file for class NI9157MemoryOperationsHelperTest.
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
 * NI9157MemoryOperationsHelperTest with all of its public, protected and
 * private members. It may also include definitions for inline methods which
 * need to be visible to the compiler.
 */

#ifndef NI9157MEMORYOPERATIONSHELPERTEST_H_
#define NI9157MEMORYOPERATIONSHELPERTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "ConfigurationDatabase.h"
#include "NI9157MemoryOperationsHelper.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
using namespace MARTe;

/**
 * @brief Tests the NI9157MemoryOperationsHelperTest methods.
 */
class NI9157MemoryOperationsHelperTest {
public:

    /**
     * @brief NI9157MemoryOperationsHelperTest Constructor.
     */
    NI9157MemoryOperationsHelperTest();

    /**
     * @brief NI9157MemoryOperationsHelperTest Destructor.
     */
    virtual ~NI9157MemoryOperationsHelperTest();

    /**
     * @brief Tests the NI9157MemoryOperationsHelper::InterleavedToFlat method.
     * @details The original MemoryOperationsHelper::InterleavedToFlat test
     * from MARTe2 core was reused here.
     */
    bool TestInterleavedToFlat();

    /**
     * @brief Tests the NI9157MemoryOperationsHelper::FlatToInterleaved method.
     * @details The original MemoryOperationsHelper::FlatToInterleaved test
     * from MARTe2 core was reused here.
     */
    bool TestFlatToInterleaved();

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* NI9157MEMORYOPERATIONSHELPERTEST_H_ */
