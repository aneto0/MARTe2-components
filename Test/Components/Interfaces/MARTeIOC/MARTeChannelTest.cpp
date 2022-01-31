/**
 * @file MARTeChannelTest.cpp
 * @brief Source file for class MARTeChannelTest
 * @date 02/09/2019
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

 * @details This source file contains the definition of all the methods for
 * the class MARTeChannelTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include "casCtx.h"
#include <stdio.h>
#include "dbMapper.h"
#include "db_access.h"
#include "gddApps.h"
/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "MARTeChannelTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

MARTeChannelTest::MARTeChannelTest() {
    // Auto-generated constructor stub for MARTeChannelTest
    // TODO Verify if manual additions are needed
}

MARTeChannelTest::~MARTeChannelTest() {
    // Auto-generated destructor stub for MARTeChannelTest
    // TODO Verify if manual additions are needed
}

bool MARTeChannelTest::TestsetOwner() {
    casCtx *ctx = new casCtx;
    MARTeChannel channel(*ctx);
    channel.setOwner("username", "hostname");
    delete ctx;
    return true;
}

bool MARTeChannelTest::TestreadAccess() {
    casCtx *ctx = new casCtx;
    MARTeChannel channel(*ctx);
    bool ret = channel.readAccess();
    delete ctx;
    return ret;
}

bool MARTeChannelTest::TestwriteAccess() {
    casCtx *ctx = new casCtx;
    MARTeChannel channel(*ctx);
    bool ret = channel.writeAccess();
    delete ctx;
    return ret;
}
