/**
 * @file MDSWriterTreeTestHelper.cpp
 * @brief Source file for class MDSWriterTreeTestHelper
 * @date 29/07/2018
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
 * the class MDSWriterTreeTestHelper (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include "mdsobjects.h"
#include <stdio.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"

#include "MDSWriterTreeTestHelper.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

MDSWriterTreeTestHelper::MDSWriterTreeTestHelper() {

}

MDSWriterTreeTestHelper::~MDSWriterTreeTestHelper() {

}

void MDSWriterTreeTestHelper::Create(MARTe::StreamString treeNameIn) {
    using namespace MARTe;
    treeName = treeNameIn;
    char *home = getenv("HOME");
    fullPath = treeName.Buffer();
    fullPath += "_path=";
    fullPath += home;
    //Important detail: fullPath must exist in all places where the environment variables is needed.
    //In other words, the scope of the environment variables is defined by the scope of the string (char *)
    //given to putenv() function.
    putenv((char *) (fullPath.Buffer()));
    MDSplus::Tree *tree = NULL;
    try {
        tree = new MDSplus::Tree(treeName.Buffer(), -1, "NEW");
    }
    catch (const MDSplus::MdsException &exc) {
        REPORT_ERROR_STATIC(MARTe::ErrorManagement::Warning, "Error opening tree %s. Error: %s", treeName.Buffer(), exc.what());
    }
    if (tree != NULL) {
        try {
            tree->addNode(":SIGUINT8F", "SIGNAL");
            tree->addNode(":SIGUINT8D", "SIGNAL");
            tree->addNode(":SIGUINT8", "SIGNAL");

            tree->addNode(":SIGUINT16F", "SIGNAL");
            tree->addNode(":SIGUINT16D", "SIGNAL");
            tree->addNode(":SIGUINT16", "SIGNAL");

            tree->addNode(":SIGUINT32F", "SIGNAL");
            tree->addNode(":SIGUINT32D", "SIGNAL");
            tree->addNode(":SIGUINT32", "SIGNAL");

            tree->addNode(":SIGUINT64F", "SIGNAL");
            tree->addNode(":SIGUINT64D", "SIGNAL");
            tree->addNode(":SIGUINT64", "SIGNAL");

            tree->addNode(":SIGINT8F", "SIGNAL");
            tree->addNode(":SIGINT8D", "SIGNAL");
            tree->addNode(":SIGINT8", "SIGNAL");

            tree->addNode(":SIGINT16F", "SIGNAL");
            tree->addNode(":SIGINT16D", "SIGNAL");
            tree->addNode(":SIGINT16", "SIGNAL");

            tree->addNode(":SIGINT32F", "SIGNAL");
            tree->addNode(":SIGINT32D", "SIGNAL");
            tree->addNode(":SIGINT32", "SIGNAL");

            tree->addNode(":SIGINT64F", "SIGNAL");
            tree->addNode(":SIGINT64D", "SIGNAL");
            tree->addNode(":SIGINT64", "SIGNAL");

            tree->addNode(":SIGFLT32F", "SIGNAL");
            tree->addNode(":SIGFLT32D", "SIGNAL");
            tree->addNode(":SIGFLT32", "SIGNAL");

            tree->addNode(":SIGFLT64F", "SIGNAL");
            tree->addNode(":SIGFLT64D", "SIGNAL");
            tree->addNode(":SIGFLT64", "SIGNAL");

            tree->write();
            delete tree;
        }
        catch (const MDSplus::MdsException &exc) {
            REPORT_ERROR_STATIC(MARTe::ErrorManagement::Warning, "Error adding node: %s", treeName.Buffer(), exc.what());
        }
    }
}

void MDSWriterTreeTestHelper::Destroy() {
    MDSplus::Tree *tree = new MDSplus::Tree(treeName.Buffer(), -1);
    MARTe::uint32 shotNumber = tree->getCurrent(treeName.Buffer());
    if (shotNumber > 0) {
        tree->deletePulse(shotNumber);
        delete tree;
    }
    MARTe::StreamString strChar = getenv("HOME");
    strChar += "/";
    strChar += treeName.Buffer();
    strChar += "_model.characteristics";
    if (0 != remove(strChar.Buffer())) {
        printf("Error while removing %s\n", strChar.Buffer());
    }

    MARTe::StreamString strData = getenv("HOME");
    strData += "/";
    strData += treeName.Buffer();
    strData += "_model.datafile";
    if (0 != remove(strData.Buffer())) {
        printf("Error while removing %s\n", strData.Buffer());
    }

    MARTe::StreamString strTree = getenv("HOME");
    strTree += "/";
    strTree += treeName.Buffer();
    strTree += "_model.tree";

    if (0 != remove(strTree.Buffer())) {
        printf("Error while removing %s\n", strTree.Buffer());
    }
    MARTe::StreamString strShot = getenv("HOME");
    strShot += "/";
    strShot += "shotid.sys";
    remove(strShot.Buffer());
    
    ///////////////////////////////////////////////////////////////////////////////////////////
    // Remove the environment variable so other static instances in MDSObjectConnectionTest 
    // does not try to destroy the tree with the environment variable created here 
    ///////////////////////////////////////////////////////////////////////////////////////////
    MARTe::StreamString treeNameEnv = treeName;
    treeNameEnv += "_path";
    unsetenv(treeNameEnv.Buffer());
}

