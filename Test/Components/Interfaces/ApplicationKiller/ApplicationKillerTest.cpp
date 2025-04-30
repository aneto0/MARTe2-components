/**
 * @file ApplicationKillerTest.cpp
 * @brief Source file for class ApplicationKillerTest
 * @date 30/04/2025
 * @author Llorenc Capella
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

 * @details This source file contains the definition of all the methods for
 * the class ApplicationKillerTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ApplicationKillerTest.h"
#include "ApplicationKiller.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace {

}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

ApplicationKillerTest::ApplicationKillerTest() {
//Auto-generated constructor stub for ApplicationKillerTest

//TODO Verify if manual additions are needed here
}

ApplicationKillerTest::~ApplicationKillerTest() {
//Auto-generated destructor stub for ApplicationKillerTest

//TODO Verify if manual additions are needed here
}

bool ApplicationKillerTest::Test_KillOnSucceed(){
    ApplicationKiller myKiller;
    myKiller.Kill();
    return false;
}

}

