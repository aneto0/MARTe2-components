/**
 * @file UEICircularBufferTest.h
 * @brief Header file for class UEICircularBufferTest
 * @date 22/03/2023
 * @author Xavier Ruche
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

 * @details This header file contains the declaration of the class UEICircularBufferTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef UEICircularBuffer_UEICircularBufferTEST_H_
#define UEICircularBuffer_UEICircularBufferTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "ConfigurationDatabase.h"
#include "UEICircularBuffer.h"
#include "UEICircularBufferTest.h"
#include "SafeMath.h"
#include "GlobalObjectsDatabase.h"
#include "UEIDAQMockup/PDNA.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;
/**
 * @brief Tests all the UEICircularBuffer methods
 */
class UEICircularBufferTest : public Object {
    public:
        /**
        * @brief Constructs the Test Unit environment basics
        */
        UEICircularBufferTest();

        /**
        * @brief Destructs the Test Unit environment
        */
        ~UEICircularBufferTest();

        /**
        * @brief Tests the UEICircularBuffer constructor
        */
        bool TestConstructor();
        bool TestInitialiseBuffer();
        bool TestInitialiseBuffer_NChannelsFail();
        bool TestInitialiseBuffer_MapSamplesFail();
        bool TestInitialiseBuffer_ReadSamplesFail();
        bool TestInitialiseBuffer_SampleSize();
        bool TestInitialiseBuffer_nBuffersFail();
        bool FunctionalTest();    
        bool FunctionalTest_WithTimestamp();  
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* UEICircularBuffer_UEICircularBufferTEST_H_ */

