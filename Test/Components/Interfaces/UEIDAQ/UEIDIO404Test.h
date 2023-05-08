/**
 * @file UEIDIO404Test.h
 * @brief Header file for class UEIDIO404Test
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

 * @details This header file contains the declaration of the class UEIDIO404Test
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef UEIDIO404_UEIDIO404TEST_H_
#define UEIDIO404_UEIDIO404TEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "ConfigurationDatabase.h"
#include "UEIDIO404.h"
#include "UEIDIO404Test.h"
#include "SafeMath.h"
#include "GlobalObjectsDatabase.h"
#include "UEIDAQMockup.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;
/**
 * @brief Tests all the UEIDIO404 methods
 */
class UEIDIO404Test : public Object {
    public:
        /**
        * @brief Constructs the Test Unit environment basics
        */
        UEIDIO404Test();

        /**
        * @brief Destructs the Test Unit environment
        */
        ~UEIDIO404Test();

        /**
        * @brief Tests the UEIDIO404 constructor
        */
        bool TestConstructor();
        bool TestFixedParameters();
        bool TestAcceptedSignalType();
        bool TestInitialise();
        bool TestInitialise_InvalidVoltageReference();
        bool TestInitialise_NoVoltageReference();
        bool TestInitialise_IncompleteHysteresis();
        bool TestInitialise_TooLowHysteresis();
        bool TestInitialise_TooHighHysteresis();
        bool TestInitialise_InvalidHysteresis();
        bool TestInitialise_ValidHysteresis();
        bool TestInitialise_class();
        bool TestCheckChannelAndDirection();
        bool TestConfigureChannel();
        bool TestSignalScaling();
        bool TestConfigureDevice();
        bool TestOutputSignalSetter();        
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* UEIDIO404_UEIDIO404TEST_H_ */

