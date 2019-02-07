/**
 * @file EPICSRPCClientTest.cpp
 * @brief Source file for class EPICSRPCClientTest
 * @date 13/06/2018
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

 * @details This source file contains the definition of all the methods for
 * the class EPICSRPCClientTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <pv/rpcClient.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "ConfigurationDatabase.h"
#include "EPICSPVAStructureDataI.h"
#include "File.h"
#include "ObjectRegistryDatabase.h"
#include "StandardParser.h"
#include "Vector.h"
#include "EPICSRPCClientTest.h"
#include "EPICSRPCClient.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
bool EPICSRPCClientTest::TestConstructor() {
    using namespace MARTe;
    EPICSRPCClient rpcClient;

    return (rpcClient.NumberOfReferences() == 0u);
}

bool EPICSRPCClientTest::TestInitialise() {
    using namespace MARTe;
    EPICSRPCClient rpcClient;
    ConfigurationDatabase cdb;
    cdb.Write("Timeout", 15);
    bool ok = rpcClient.Initialise(cdb);
    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}
