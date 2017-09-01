/**
 * @file SSMGAMTest.cpp
 * @brief Source file for class SSMGAMTest
 * @date 01/09/2017
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
 * the class SSMGAMTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include "stdio.h"
/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "SSMGAMTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace {

}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

SSMGAMTest::SSMGAMTest() {
//Auto-generated constructor stub for SSMGAMTest

//TODO Verify if manual additions are needed here
}

SSMGAMTest::~SSMGAMTest() {
//Auto-generated destructor stub for SSMGAMTest

//TODO Verify if manual additions are needed here
}

bool SSMGAMTest::TestInitialise() {
    bool ok;
    SSMGAM gam;
    ConfigurationDatabase config;
    float64 stateMatrix[2][3];
    stateMatrix[0][0] = 11;
    stateMatrix[0][1] = 12;
    stateMatrix[0][2] = 13;
    stateMatrix[1][0] = 21;
    stateMatrix[1][1] = 22;
    stateMatrix[1][2] = 23;
    Matrix<float64> matrix(&stateMatrix[0][0], 2u, 3u);
    ok = config.Write("StateMatrix", matrix);
    StreamString a;
    printf("%d\n", a.Printf("%!", config));
    printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
    printf("%s\n", a.Buffer());
    printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
    if (ok) {
        ok &= gam.Initialise(config);
    }
    return ok;
}

}

