/**
 * @file MDSStructuredDataITest.cpp
 * @brief Source file for class MDSStructuredDataITest
 * @date 04/09/2018
 * @author Andre Neto
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
 * the class MDSStructuredDataITest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "ConfigurationDatabase.h"
#include "MDSStructuredDataITest.h"
#include "StandardParser.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

MDSStructuredDataITest::MDSStructuredDataITest() {
    remove_mds_sdi = false;
    remove_mds_sdi2 = false;
    char *home = getenv("HOME");
    treeName = "mds_sdi";
    fullPath = treeName.Buffer();
    fullPath += "_path=";
    fullPath += home;
    //Important detail: fullPath must exist in all places where the environment variables is needed.
    //In other words, the scope of the environment variables is defined by the scope of the string (char *)
    //given to putenv() function.
    putenv((char *) (fullPath.Buffer()));
    treeName2 = "mds_sdi2";
    fullPath2 = treeName2.Buffer();
    fullPath2 += "_path=";
    fullPath2 += home;
    //Important detail: fullPath must exist in all places where the environment variables is needed.
    //In other words, the scope of the environment variables is defined by the scope of the string (char *)
    //given to putenv() function.
    putenv((char *) (fullPath2.Buffer()));
}

MDSStructuredDataITest::~MDSStructuredDataITest() {
    if (remove_mds_sdi) {
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
    }
    if (remove_mds_sdi2) {
        MARTe::StreamString strChar = getenv("HOME");
        strChar += "/";
        strChar += treeName2.Buffer();
        strChar += "_model.characteristics";
        if (0 != remove(strChar.Buffer())) {
            printf("Error while removing %s\n", strChar.Buffer());
        }
        MARTe::StreamString strData = getenv("HOME");
        strData += "/";
        strData += treeName2.Buffer();
        strData += "_model.datafile";
        if (0 != remove(strData.Buffer())) {
            printf("Error while removing %s\n", strData.Buffer());
        }
        MARTe::StreamString strTree = getenv("HOME");
        strTree += "/";
        strTree += treeName2.Buffer();
        strTree += "_model.tree";
        if (0 != remove(strTree.Buffer())) {
            printf("Error while removing %s\n", strTree.Buffer());
        }
    }
}

bool MDSStructuredDataITest::TestCreateTree() {
    using namespace MARTe;
    MDSStructuredDataI mdsStructuredDataI;
    bool ret = mdsStructuredDataI.CreateTree(treeName.Buffer());
    if (ret) {
        remove_mds_sdi = true;
    }
    return ret;
}

bool MDSStructuredDataITest::TestCreateTree2() {
    using namespace MARTe;
    MDSStructuredDataI mdsStructuredDataI;
    bool ret = mdsStructuredDataI.CreateTree(treeName.Buffer());
    if (ret) {
        remove_mds_sdi = true;
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateTree(treeName.Buffer());
    }
    return ret;
}

bool MDSStructuredDataITest::TestCreateTree_force() {
    using namespace MARTe;
    MDSStructuredDataI mdsStructuredDataI;
    bool ret = mdsStructuredDataI.CreateTree(treeName.Buffer());
    if (ret) {
        remove_mds_sdi = true;
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateTree(treeName.Buffer(), true);
    }
    return ret;
}

bool MDSStructuredDataITest::TestOpentree() {
    using namespace MARTe;
    MDSStructuredDataI mdsStructuredDataI;
    bool ret = mdsStructuredDataI.CreateTree(treeName.Buffer(), true);
    if (ret) {
        remove_mds_sdi = true;
    }
    if (ret) {
        ret = mdsStructuredDataI.OpenTree(treeName.Buffer(), -1);
    }
    return ret;
}

bool MDSStructuredDataITest::TestOpentree2() {
    using namespace MARTe;
    MDSStructuredDataI mdsStructuredDataI;
    bool ret = mdsStructuredDataI.CreateTree(treeName.Buffer(), true);
    if (ret) {
        remove_mds_sdi = true;
    }
    if (ret) {
        mdsStructuredDataI.SetEditMode(true);
        ret = mdsStructuredDataI.OpenTree(treeName.Buffer(), -1);
    }
    return ret;
}

bool MDSStructuredDataITest::TestOpentree_sameTree() {
    using namespace MARTe;
    MDSStructuredDataI mdsStructuredDataI;
    bool ret = mdsStructuredDataI.CreateTree(treeName.Buffer(), true);
    if (ret) {
        remove_mds_sdi = true;
    }
    if (ret) {
        ret = mdsStructuredDataI.OpenTree(treeName.Buffer(), -1);
    }
    if (ret) {
        ret = mdsStructuredDataI.OpenTree(treeName.Buffer(), -1);
    }
    return ret;
}

bool MDSStructuredDataITest::TestOpentree_diffTree() {
    using namespace MARTe;
    MDSStructuredDataI mdsStructuredDataI;
    bool ret = mdsStructuredDataI.CreateTree(treeName.Buffer(), true);
    if (ret) {
        remove_mds_sdi = true;
    }
    if (ret) {
        ret = mdsStructuredDataI.OpenTree(treeName.Buffer(), -1);
    }
    if (ret) {
        ret = !mdsStructuredDataI.OpenTree(treeName2.Buffer(), -1);
    }
    return ret;
}

bool MDSStructuredDataITest::TestOpentree_NoCreate() {
    using namespace MARTe;
    MDSStructuredDataI mdsStructuredDataI;
    bool ret = !mdsStructuredDataI.OpenTree(treeName.Buffer(), -1);
    return ret;
}

bool MDSStructuredDataITest::TestSetTree() {
    using namespace MARTe;
    MDSStructuredDataI mdsStructuredDataI;
    bool force = true;
    bool ret = mdsStructuredDataI.CreateTree(treeName.Buffer(), force);
    if (ret) {
        remove_mds_sdi = true;
    }
    MDSplus::Tree *extTree = NULL_PTR(MDSplus::Tree *);
    try {
        extTree = new MDSplus::Tree(treeName.Buffer(), -1, "EDIT");
    }
    catch (const MDSplus::MdsException &exc) {
        REPORT_ERROR_STATIC(MARTe::ErrorManagement::Warning, "Error opening tree %s. Error: %s", treeName.Buffer(), exc.what());
        ret = false;
    }
    if (ret) {
        ret = mdsStructuredDataI.SetTree(extTree);
        mdsStructuredDataI.SetEditMode(true);
    }
    return ret;
}

bool MDSStructuredDataITest::TestSetTree_openTree() {
    using namespace MARTe;
    MDSStructuredDataI mdsStructuredDataI;
    bool force = true;
    bool ret = mdsStructuredDataI.CreateTree(treeName.Buffer(), force);
    if (ret) {
        remove_mds_sdi = true;
    }
    if (ret) {
        ret = mdsStructuredDataI.OpenTree(treeName.Buffer(), -1);
    }
    MDSplus::Tree *extTree = NULL_PTR(MDSplus::Tree *);
    try {
        extTree = new MDSplus::Tree(treeName.Buffer(), -1, "EDIT");
    }
    catch (const MDSplus::MdsException &exc) {
        REPORT_ERROR_STATIC(MARTe::ErrorManagement::Warning, "Error opening tree %s. Error: %s", treeName.Buffer(), exc.what());
        ret = false;
    }

    if (ret) {
        ret = !mdsStructuredDataI.SetTree(extTree);
        mdsStructuredDataI.SetEditMode(true);
    }
    return ret;
}

bool MDSStructuredDataITest::TestcloseTree() {
    using namespace MARTe;
    MDSStructuredDataI mdsStructuredDataI;
    bool force = true;
    bool ret = mdsStructuredDataI.CreateTree(treeName.Buffer(), force);
    if (ret) {
        remove_mds_sdi = true;
    }
    if (ret) {
        ret = mdsStructuredDataI.OpenTree(treeName.Buffer(), -1);
    }
    if (ret) {
        ret = mdsStructuredDataI.CloseTree();
    }
    return ret;
}

bool MDSStructuredDataITest::TestcloseTree_SetTree() {
    using namespace MARTe;
    MDSStructuredDataI mdsStructuredDataI;
    bool force = true;
    bool ret = mdsStructuredDataI.CreateTree(treeName.Buffer(), force);
    if (ret) {
        remove_mds_sdi = true;
    }
    MDSplus::Tree *extTree = NULL_PTR(MDSplus::Tree *);
    try {
        extTree = new MDSplus::Tree(treeName.Buffer(), -1, "EDIT");
    }
    catch (const MDSplus::MdsException &exc) {
        REPORT_ERROR_STATIC(MARTe::ErrorManagement::Warning, "Error opening tree %s. Error: %s", treeName.Buffer(), exc.what());
        ret = false;
    }
    if (ret) {
        ret = mdsStructuredDataI.SetTree(extTree);
        mdsStructuredDataI.SetEditMode(true);
    }
    if (extTree != NULL_PTR(MDSplus::Tree *)) {
        delete extTree;
    }
    if (ret) {
        ret = mdsStructuredDataI.CloseTree();
    }
    return ret;
}

bool MDSStructuredDataITest::TestcloseTree_noOpened() {
    using namespace MARTe;
    MDSStructuredDataI mdsStructuredDataI;
    bool force = true;
    bool ret = mdsStructuredDataI.CreateTree(treeName.Buffer(), force);
    if (ret) {
        remove_mds_sdi = true;
    }
    if (ret) {
        ret = !mdsStructuredDataI.CloseTree();
    }
    return ret;
}

bool MDSStructuredDataITest::TestDelete() {
    using namespace MARTe;
    MDSStructuredDataI mdsStructuredDataI;
    bool force = true;
    bool ret = mdsStructuredDataI.CreateTree(treeName.Buffer(), force);
    if (ret) {
        remove_mds_sdi = true;
    }
    if (ret) {
        ret = mdsStructuredDataI.SetEditMode(true);
    }
    if (ret) {
        ret = mdsStructuredDataI.OpenTree(treeName.Buffer(), -1);
    }
    StreamString a = "Hi";
    if (ret) {
        ret = mdsStructuredDataI.Write("AString", a.Buffer());
    }
    StreamString b = "Bye";
    if (ret) {
        ret = mdsStructuredDataI.Write("BString", b.Buffer());
    }
    if (ret) {
        ret = mdsStructuredDataI.SaveTree();
    }
    if (ret) {
        ret = mdsStructuredDataI.Delete("BString");
    }
    if (ret) {
        ret = mdsStructuredDataI.SaveTree();
    }
    return ret;
}

bool MDSStructuredDataITest::TestDelete_noEditable() {
    using namespace MARTe;
    MDSStructuredDataI mdsStructuredDataI;
    bool force = true;
    bool ret = mdsStructuredDataI.CreateTree(treeName.Buffer(), force);
    if (ret) {
        remove_mds_sdi = true;
    }
    if (ret) {
        mdsStructuredDataI.SetEditMode(true);
        ret = mdsStructuredDataI.OpenTree(treeName.Buffer(), -1);
    }
    StreamString a = "Hi";
    if (ret) {
        ret = mdsStructuredDataI.Write("AString", a.Buffer());
    }
    StreamString b = "Bye";
    if (ret) {
        ret = mdsStructuredDataI.Write("BString", b.Buffer());
    }
    if (ret) {
        ret = mdsStructuredDataI.SaveTree();
    }
    if (ret) {
        mdsStructuredDataI.SetEditMode(false);
    }
    if (ret) {
        ret = !mdsStructuredDataI.Delete("BString");
    }
    return ret;
}

bool MDSStructuredDataITest::TestDelete_noOpened() {
    using namespace MARTe;
    MDSStructuredDataI mdsStructuredDataI;
    bool force = true;
    bool ret = mdsStructuredDataI.CreateTree(treeName.Buffer(), force);
    if (ret) {
        remove_mds_sdi = true;
    }
    if (ret) {
        mdsStructuredDataI.SetEditMode(true);
        ret = mdsStructuredDataI.OpenTree(treeName.Buffer(), -1);
    }
    StreamString a = "Hi";
    if (ret) {
        ret = mdsStructuredDataI.Write("AString", a.Buffer());
    }
    StreamString b = "Bye";
    if (ret) {
        ret = mdsStructuredDataI.Write("BString", b.Buffer());
    }
    if (ret) {
        ret = mdsStructuredDataI.SaveTree();
    }
    if (ret) {
        ret = mdsStructuredDataI.CloseTree();
    }
    if (ret) {
        ret = !mdsStructuredDataI.Delete("BString");
    }
    return ret;
}

bool MDSStructuredDataITest::TestDelete_noNode() {
    using namespace MARTe;
    MDSStructuredDataI mdsStructuredDataI;
    bool force = true;
    bool ret = mdsStructuredDataI.CreateTree(treeName.Buffer(), force);
    if (ret) {
        remove_mds_sdi = true;
    }
    if (ret) {
        mdsStructuredDataI.SetEditMode(true);
        ret = mdsStructuredDataI.OpenTree(treeName.Buffer(), -1);
    }
    StreamString a = "Hi";
    if (ret) {
        ret = mdsStructuredDataI.Write("AString", a.Buffer());
    }
    StreamString b = "Bye";
    if (ret) {
        ret = mdsStructuredDataI.Write("BString", b.Buffer());
    }
    if (ret) {
        ret = mdsStructuredDataI.SaveTree();
    }
    if (ret) {
        ret = !mdsStructuredDataI.Delete("noExistingNode");
    }
    if (ret) {
        ret = mdsStructuredDataI.SaveTree();
    }
    return ret;
}

bool MDSStructuredDataITest::TestSetEditMode() {
    using namespace MARTe;
    MDSStructuredDataI mdsStructuredDataI;
    bool force = true;
    bool ret = mdsStructuredDataI.CreateTree(treeName.Buffer(), force);
    if (ret) {
        remove_mds_sdi = true;
    }
    if (ret) {
        ret = mdsStructuredDataI.OpenTree(treeName.Buffer(), -1);
    }
    if (ret) {
        mdsStructuredDataI.SetEditMode(true);
    }
    StreamString a = "Hi";
    if (ret) {
        ret = mdsStructuredDataI.Write("AString", a.Buffer());
    }
    StreamString b = "Bye";
    if (ret) {
        ret = mdsStructuredDataI.Write("BString", b.Buffer());
    }
    if (ret) {
        ret = mdsStructuredDataI.SaveTree();
    }
    if (ret) {
        ret = mdsStructuredDataI.CloseTree();
    }
    return ret;
}

bool MDSStructuredDataITest::TestSetEditMode2() {
    using namespace MARTe;
    MDSStructuredDataI mdsStructuredDataI;
    bool force = true;
    bool ret = mdsStructuredDataI.CreateTree(treeName.Buffer(), force);
    if (ret) {
        remove_mds_sdi = true;
    }
    if (ret) {

        mdsStructuredDataI.SetEditMode(true);
        ret = mdsStructuredDataI.OpenTree(treeName.Buffer(), -1);
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateRelative("A");
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateRelative("B");
    }
    if (ret) {
        ret = mdsStructuredDataI.SaveTree();
    }
    if (ret) {
        mdsStructuredDataI.SetEditMode(false);
        mdsStructuredDataI.SetEditMode(true);
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateRelative("C");
    }
    if (ret) {
        ret = mdsStructuredDataI.SaveTree();
    }
    return ret;
}

bool MDSStructuredDataITest::TestSetEditMode3() {
    using namespace MARTe;
    MDSStructuredDataI mdsStructuredDataI;
    bool force = true;
    bool ret = mdsStructuredDataI.CreateTree(treeName.Buffer(), force);
    if (ret) {
        remove_mds_sdi = true;
    }
    if (ret) {

        mdsStructuredDataI.SetEditMode(true);
        ret = mdsStructuredDataI.OpenTree(treeName.Buffer(), -1);
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateRelative("A");
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateRelative("B");
    }
    if (ret) {
        ret = !mdsStructuredDataI.SetEditMode(false);
    }
    return ret;
}

bool MDSStructuredDataITest::TestSaveTree() {
    using namespace MARTe;
    MDSStructuredDataI mdsStructuredDataI;
    bool force = true;
    bool ret = mdsStructuredDataI.CreateTree(treeName.Buffer(), force);
    if (ret) {
        remove_mds_sdi = true;
    }
    if (ret) {
        mdsStructuredDataI.SetEditMode(true);
        ret = mdsStructuredDataI.OpenTree(treeName.Buffer(), -1);
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateRelative("A");
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateRelative("B");
    }
    if (ret) {
        ret = mdsStructuredDataI.SaveTree();
    }
    return ret;
}

bool MDSStructuredDataITest::TestSaveTree_noEditable() {
    using namespace MARTe;
    MDSStructuredDataI mdsStructuredDataI;
    bool force = true;
    bool ret = mdsStructuredDataI.CreateTree(treeName.Buffer(), force);
    if (ret) {
        remove_mds_sdi = true;
    }
    if (ret) {
        mdsStructuredDataI.SetEditMode(true);
        ret = mdsStructuredDataI.OpenTree(treeName.Buffer(), -1);
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateRelative("A");
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateRelative("B");
    }
    if (ret) {
        ret = mdsStructuredDataI.SaveTree();
        mdsStructuredDataI.SetEditMode(false);
    }
    if (ret) {
        ret = !mdsStructuredDataI.SaveTree();
    }
    return ret;
}

bool MDSStructuredDataITest::TestSaveTree_noOpen() {
    using namespace MARTe;
    MDSStructuredDataI mdsStructuredDataI;
    bool force = true;
    bool ret = mdsStructuredDataI.CreateTree(treeName.Buffer(), force);
    if (ret) {
        remove_mds_sdi = true;
    }
    if (ret) {
        mdsStructuredDataI.SetEditMode(true);
        ret = mdsStructuredDataI.OpenTree(treeName.Buffer(), -1);
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateRelative("A");
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateRelative("B");
    }
    if (ret) {
        ret = mdsStructuredDataI.CloseTree();
    }
    if (ret) {
        ret = !mdsStructuredDataI.SaveTree();
    }
    return ret;
}

bool MDSStructuredDataITest::TestGetNumberOfChildren() {
    using namespace MARTe;
    MDSStructuredDataI mdsStructuredDataI;
    bool force = true;
    bool ret = mdsStructuredDataI.CreateTree(treeName.Buffer(), force);
    if (ret) {
        remove_mds_sdi = true;
    }
    if (ret) {
        mdsStructuredDataI.SetEditMode(true);
        ret = mdsStructuredDataI.OpenTree(treeName.Buffer(), -1);
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A");
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A.1");
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A.2");
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A.3");
    }
    if (ret) {
        ret = mdsStructuredDataI.SaveTree();
    }
    if (ret) {
        ret = mdsStructuredDataI.MoveToAncestor(1u);
        ret = (3 == mdsStructuredDataI.GetNumberOfChildren());
    }
    return ret;
}

bool MDSStructuredDataITest::TestGetNumberOfChildren2() {
    using namespace MARTe;
    MDSStructuredDataI mdsStructuredDataI;
    bool force = true;
    bool ret = mdsStructuredDataI.CreateTree(treeName.Buffer(), force);
    if (ret) {
        remove_mds_sdi = true;
    }
    if (ret) {
        mdsStructuredDataI.SetEditMode(true);
        ret = mdsStructuredDataI.OpenTree(treeName.Buffer(), -1);
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A");
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A.1");
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A.2");
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A.3");
    }
    if (ret) {
        ret = mdsStructuredDataI.MoveToAncestor(1u);
        ret = (3 == mdsStructuredDataI.GetNumberOfChildren());
    }
    return ret;
}

bool MDSStructuredDataITest::TestGetNumberOfChildren_closedTree() {
    using namespace MARTe;
    MDSStructuredDataI mdsStructuredDataI;
    bool force = true;
    bool ret = mdsStructuredDataI.CreateTree(treeName.Buffer(), force);
    if (ret) {
        remove_mds_sdi = true;
    }
    if (ret) {
        mdsStructuredDataI.SetEditMode(true);
        ret = mdsStructuredDataI.OpenTree(treeName.Buffer(), -1);
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A");
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A.1");
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A.2");
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A.3");
    }
    if (ret) {
        ret = mdsStructuredDataI.SaveTree();
    }
    if (ret) {
        ret = mdsStructuredDataI.CloseTree();
    }
    if (ret) {
        ret = (0 == mdsStructuredDataI.GetNumberOfChildren());
    }
    return ret;
}

bool MDSStructuredDataITest::TestGetChildName() {
    using namespace MARTe;
    MDSStructuredDataI mdsStructuredDataI;
    bool force = true;
    bool ret = mdsStructuredDataI.CreateTree(treeName.Buffer(), force);
    if (ret) {
        remove_mds_sdi = true;
    }
    if (ret) {
        mdsStructuredDataI.SetEditMode(true);
        ret = mdsStructuredDataI.OpenTree(treeName.Buffer(), -1);
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A");
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A.1");
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A.2");
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A.3");
    }

    if (ret) {
        ret = mdsStructuredDataI.MoveToAncestor(1u);
        StreamString refStr1 = "1";
        ret = (refStr1 == mdsStructuredDataI.GetChildName(0));
    }
    if (ret) {
        StreamString refStr2 = "2";
        ret = (refStr2 == mdsStructuredDataI.GetChildName(1));
    }
    if (ret) {
        StreamString refStr3 = "3";
        ret = (refStr3 == mdsStructuredDataI.GetChildName(2));
    }
    return ret;
}

bool MDSStructuredDataITest::TestGetChildName_treeClosed() {
    using namespace MARTe;
    MDSStructuredDataI mdsStructuredDataI;
    bool force = true;
    bool ret = mdsStructuredDataI.CreateTree(treeName.Buffer(), force);
    if (ret) {
        remove_mds_sdi = true;
    }
    if (ret) {
        mdsStructuredDataI.SetEditMode(true);
        ret = mdsStructuredDataI.OpenTree(treeName.Buffer(), -1);
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A.1");
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A.2");
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A.3");
    }
    if (ret) {
        ret = mdsStructuredDataI.MoveToAncestor(1u);
    }
    if (ret) {
        ret = mdsStructuredDataI.CloseTree();
    }
    if (ret) {
        ret = (NULL_PTR(const char8 *) == mdsStructuredDataI.GetChildName(0));
    }
    return ret;
}

bool MDSStructuredDataITest::TestGetChildName_noExistingNode() {
    using namespace MARTe;
    MDSStructuredDataI mdsStructuredDataI;
    bool force = true;
    bool ret = mdsStructuredDataI.CreateTree(treeName.Buffer(), force);
    if (ret) {
        remove_mds_sdi = true;
    }
    if (ret) {
        mdsStructuredDataI.SetEditMode(true);
        ret = mdsStructuredDataI.OpenTree(treeName.Buffer(), -1);
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A");
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A.1");
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A.2");
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A.3");
    }
    if (ret) {
        ret = mdsStructuredDataI.MoveToAncestor(1u);
        ret = (NULL_PTR(char8 *) == mdsStructuredDataI.GetChildName(3));
    }
    return ret;
}

bool MDSStructuredDataITest::TestGetChildName_noExistingNode2() {
    using namespace MARTe;
    MDSStructuredDataI mdsStructuredDataI;
    bool force = true;
    bool ret = mdsStructuredDataI.CreateTree(treeName.Buffer(), force);
    if (ret) {
        remove_mds_sdi = true;
    }
    if (ret) {
        mdsStructuredDataI.SetEditMode(true);
        ret = mdsStructuredDataI.OpenTree(treeName.Buffer(), -1);
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A");
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A.1");
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A.2");
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A.3");
    }
    if (ret) {
        ret = mdsStructuredDataI.MoveToAncestor(1u);
        ret = (NULL_PTR(char8 *) == mdsStructuredDataI.GetChildName(4));
    }
    return ret;
}

bool MDSStructuredDataITest::TestGetChildName_setAndDestroy() {
    using namespace MARTe;
    MDSStructuredDataI mdsStructuredDataI;
    bool force = true;
    bool ret = mdsStructuredDataI.CreateTree(treeName.Buffer(), force);
    if (ret) {
        remove_mds_sdi = true;
    }
    MDSplus::Tree *tree = NULL_PTR(MDSplus::Tree *);
    try {
        tree = new MDSplus::Tree(treeName.Buffer(), -1, "EDIT");
    }
    catch (const MDSplus::MdsException &exc) {
        REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "Fail opening tree %s with shotNumber -1:  %s", treeName.Buffer(), exc.what());
        ret = false;
    }
    if (ret) {
        ret = mdsStructuredDataI.SetTree(tree);
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A.1");
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A.2");
    }
    if (ret) {
        ret = mdsStructuredDataI.MoveToAncestor(1u);
    }
    delete tree;
    if (ret) {
        ret = mdsStructuredDataI.CloseTree();
    }
    if (ret) {
        ret = (NULL_PTR(char8 *) == mdsStructuredDataI.GetChildName(0));
    }
    return ret;
}

bool MDSStructuredDataITest::TestGetName() {
    using namespace MARTe;
    MDSStructuredDataI mdsStructuredDataI;
    bool force = true;
    bool ret = mdsStructuredDataI.CreateTree(treeName.Buffer(), force);
    if (ret) {
        remove_mds_sdi = true;
    }
    if (ret) {
        mdsStructuredDataI.SetEditMode(true);
        ret = mdsStructuredDataI.OpenTree(treeName.Buffer(), -1);
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A");
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A.1");
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A.2");
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A.3");
    }
    if (ret) {
        StreamString auxStr = "3";
        ret = (auxStr == mdsStructuredDataI.GetName());
    }
    return ret;
}

bool MDSStructuredDataITest::TestGetName_closedTree() {
    using namespace MARTe;
    MDSStructuredDataI mdsStructuredDataI;
    bool force = true;
    bool ret = mdsStructuredDataI.CreateTree(treeName.Buffer(), force);
    if (ret) {
        remove_mds_sdi = true;
    }
    if (ret) {
        mdsStructuredDataI.SetEditMode(true);
        ret = mdsStructuredDataI.OpenTree(treeName.Buffer(), -1);
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A");
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A.1");
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A.2");
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A.3");
    }
    if (ret) {
        ret = mdsStructuredDataI.SaveTree();
    }
    if (ret) {
        ret = mdsStructuredDataI.CloseTree();
    }
    if (ret) {
        ret = (NULL_PTR(char8 *) == mdsStructuredDataI.GetName());
    }
    return ret;
}

bool MDSStructuredDataITest::TestCreateRelative() {
    using namespace MARTe;
    MDSStructuredDataI mdsStructuredDataI;
    bool force = true;
    bool ret = mdsStructuredDataI.CreateTree(treeName.Buffer(), force);
    if (ret) {
        remove_mds_sdi = true;
    }
    if (ret) {
        mdsStructuredDataI.SetEditMode(true);
        ret = mdsStructuredDataI.OpenTree(treeName.Buffer(), -1);
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateRelative("A");
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateRelative("1");
    }
    if (ret) {
        StreamString auxStr = "1";
        ret = (auxStr == mdsStructuredDataI.GetName());
    }
    return ret;
}

bool MDSStructuredDataITest::TestCreateRelative2() {
    using namespace MARTe;
    MDSStructuredDataI mdsStructuredDataI;
    bool force = true;
    bool ret = mdsStructuredDataI.CreateTree(treeName.Buffer(), force);
    if (ret) {
        remove_mds_sdi = true;
    }
    if (ret) {
        mdsStructuredDataI.SetEditMode(true);
        ret = mdsStructuredDataI.OpenTree(treeName.Buffer(), -1);
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateRelative("A");
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateRelative("B.C.D");
    }
    if (ret) {
        StreamString auxStr = "D";
        ret = (auxStr == mdsStructuredDataI.GetName());
    }
    return ret;
}

bool MDSStructuredDataITest::TestCreateRelative3() {
    using namespace MARTe;
    MDSStructuredDataI mdsStructuredDataI;
    bool force = true;
    bool ret = mdsStructuredDataI.CreateTree(treeName.Buffer(), force);
    if (ret) {
        remove_mds_sdi = true;
    }
    if (ret) {
        mdsStructuredDataI.SetEditMode(true);
        ret = mdsStructuredDataI.OpenTree(treeName.Buffer(), -1);
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateRelative("A");
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateRelative("B.C.D");
    }
    if (ret) {
        ret = mdsStructuredDataI.MoveToAncestor(2);
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateRelative("C.D.E");
    }
    if (ret) {
        StreamString auxStr = "E";
        ret = (auxStr == mdsStructuredDataI.GetName());
    }
    if (ret) {
        ret = mdsStructuredDataI.MoveToAncestor(1u);
    }
    if (ret) {
        StreamString auxStr = "D";
        ret = (auxStr == mdsStructuredDataI.GetName());
    }
    if (ret) {
        ret = mdsStructuredDataI.MoveToAncestor(1u);
    }
    if (ret) {
        StreamString auxStr = "C";
        ret = (auxStr == mdsStructuredDataI.GetName());
    }
    if (ret) {
        ret = mdsStructuredDataI.MoveToAncestor(1u);
    }
    if (ret) {
        StreamString auxStr = "B";
        ret = (auxStr == mdsStructuredDataI.GetName());
    }
    if (ret) {
        ret = mdsStructuredDataI.MoveToAncestor(1u);
    }
    if (ret) {
        StreamString auxStr = "A";
        ret = (auxStr == mdsStructuredDataI.GetName());
    }
    return ret;
}

bool MDSStructuredDataITest::TestCreateRelative_AllNodesExist() {
    using namespace MARTe;
    MDSStructuredDataI mdsStructuredDataI;
    bool force = true;
    bool ret = mdsStructuredDataI.CreateTree(treeName.Buffer(), force);
    if (ret) {
        remove_mds_sdi = true;
    }
    if (ret) {
        mdsStructuredDataI.SetEditMode(true);
        ret = mdsStructuredDataI.OpenTree(treeName.Buffer(), -1);
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateRelative("A");
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateRelative("B.C.D");
    }
    if (ret) {
        ret = mdsStructuredDataI.MoveToAncestor(2);
    }
    if (ret) {
        ret = !mdsStructuredDataI.CreateRelative("C.D");
    }
    if (ret) {
        StreamString auxStr = "B";
        ret = (auxStr == mdsStructuredDataI.GetName());
    }
    return ret;
}

bool MDSStructuredDataITest::TestCreateRelative_NoEditable() {
    using namespace MARTe;
    MDSStructuredDataI mdsStructuredDataI;
    bool force = true;
    bool ret = mdsStructuredDataI.CreateTree(treeName.Buffer(), force);
    if (ret) {
        remove_mds_sdi = true;
    }
    if (ret) {
        mdsStructuredDataI.SetEditMode(true);
        ret = mdsStructuredDataI.OpenTree(treeName.Buffer(), -1);
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateRelative("A");
    }
    if (ret) {
        ret = mdsStructuredDataI.SaveTree();
    }
    if (ret) {
        mdsStructuredDataI.SetEditMode(false);
        ret = !mdsStructuredDataI.CreateRelative("1");
    }
    if (ret) {
        StreamString auxStr = "A";
        ret = (auxStr == mdsStructuredDataI.GetName());
    }
    return ret;
}

bool MDSStructuredDataITest::TestCreateRelative_closedTree() {
    using namespace MARTe;
    MDSStructuredDataI mdsStructuredDataI;
    bool force = true;
    bool ret = mdsStructuredDataI.CreateTree(treeName.Buffer(), force);
    if (ret) {
        remove_mds_sdi = true;
    }
    if (ret) {
        mdsStructuredDataI.SetEditMode(true);
        ret = mdsStructuredDataI.OpenTree(treeName.Buffer(), -1);
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateRelative("A");
    }
    if (ret) {
        ret = mdsStructuredDataI.CloseTree();
    }
    if (ret) {
        ret = !mdsStructuredDataI.CreateRelative("1");
    }
    return ret;
}

bool MDSStructuredDataITest::TestCreateRelative_setTreeAndDestroy() {
    using namespace MARTe;
    MDSStructuredDataI mdsStructuredDataI;
    bool force = true;
    bool ret = mdsStructuredDataI.CreateTree(treeName.Buffer(), force);
    if (ret) {
        remove_mds_sdi = true;
    }
    MDSplus::Tree *tree = NULL_PTR(MDSplus::Tree *);
    try {
        tree = new MDSplus::Tree(treeName.Buffer(), -1, "EDIT");
    }
    catch (const MDSplus::MdsException &exc) {
        REPORT_ERROR_STATIC(MARTe::ErrorManagement::Warning, "Error opening tree %s. Error: %s", treeName.Buffer(), exc.what());
        ret = false;
    }
    if (ret) {
        ret = mdsStructuredDataI.SetTree(tree);
    }
    delete tree;
    if (ret) {
        ret = mdsStructuredDataI.CloseTree();
    }
    if (ret) {
        ret = !mdsStructuredDataI.CreateRelative("A");
    }
    return ret;
}

bool MDSStructuredDataITest::TestCreateAbsolute() {
    using namespace MARTe;
    MDSStructuredDataI mdsStructuredDataI;
    bool force = true;
    bool ret = mdsStructuredDataI.CreateTree(treeName.Buffer(), force);
    if (ret) {
        remove_mds_sdi = true;
    }
    if (ret) {
        mdsStructuredDataI.SetEditMode(true);
        ret = mdsStructuredDataI.OpenTree(treeName.Buffer(), -1);
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A");
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A.A");
    }
    if (ret) {
        StreamString auxStr = "A";
        ret = (auxStr == mdsStructuredDataI.GetName());
    }
    if (ret) {
        StreamString auxStr = "A";
        ret = mdsStructuredDataI.MoveToAncestor(1u);
        if (ret) {
            ret = (auxStr == mdsStructuredDataI.GetName());
        }
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A.B");
    }
    if (ret) {
        StreamString auxStr = "B";
        ret = (auxStr == mdsStructuredDataI.GetName());
    }
    return ret;
}

bool MDSStructuredDataITest::TestCreateAbsolute_noEditable() {
    using namespace MARTe;
    MDSStructuredDataI mdsStructuredDataI;
    bool force = true;
    bool ret = mdsStructuredDataI.CreateTree(treeName.Buffer(), force);
    if (ret) {
        remove_mds_sdi = true;
    }
    if (ret) {
        mdsStructuredDataI.SetEditMode(true);
        ret = mdsStructuredDataI.OpenTree(treeName.Buffer(), -1);
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A");
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A.A");
    }
    if (ret) {
        StreamString auxStr = "A";
        ret = (auxStr == mdsStructuredDataI.GetName());
    }
    if (ret) {
        StreamString auxStr = "A";
        ret = mdsStructuredDataI.MoveToAncestor(1u);
        if (ret) {
            ret = (auxStr == mdsStructuredDataI.GetName());
        }
    }
    if (ret) {
        ret = mdsStructuredDataI.SaveTree();
    }
    if (ret) {
        ret = mdsStructuredDataI.SetEditMode(false);
    }
    if (ret) {
        ret = !mdsStructuredDataI.CreateAbsolute("A.B");
    }
    return ret;
}

bool MDSStructuredDataITest::TestCreateAbsolute_closedTree() {
    using namespace MARTe;
    MDSStructuredDataI mdsStructuredDataI;
    bool force = true;
    bool ret = mdsStructuredDataI.CreateTree(treeName.Buffer(), force);
    if (ret) {
        remove_mds_sdi = true;
    }
    if (ret) {
        mdsStructuredDataI.SetEditMode(true);
        ret = mdsStructuredDataI.OpenTree(treeName.Buffer(), -1);
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A");
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A.A");
    }
    if (ret) {
        StreamString auxStr = "A";
        ret = (auxStr == mdsStructuredDataI.GetName());
    }
    if (ret) {
        StreamString auxStr = "A";
        ret = mdsStructuredDataI.MoveToAncestor(1u);
        if (ret) {
            ret = (auxStr == mdsStructuredDataI.GetName());
        }
    }
    if (ret) {
        ret = mdsStructuredDataI.SaveTree();
    }
    if (ret) {
        ret = mdsStructuredDataI.CloseTree();
    }
    if (ret) {
        ret = !mdsStructuredDataI.CreateAbsolute("A.B");
    }
    return ret;
}

bool MDSStructuredDataITest::TestCreateAbsolute_AllNodesExist() {
    using namespace MARTe;
    MDSStructuredDataI mdsStructuredDataI;
    bool force = true;
    bool ret = mdsStructuredDataI.CreateTree(treeName.Buffer(), force);
    if (ret) {
        remove_mds_sdi = true;
    }
    if (ret) {
        mdsStructuredDataI.SetEditMode(true);
        ret = mdsStructuredDataI.OpenTree(treeName.Buffer(), -1);
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A");
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A.B.C.D");
    }
    if (ret) {
        ret = mdsStructuredDataI.MoveToAncestor(2);
    }
    if (ret) {
        ret = !mdsStructuredDataI.CreateAbsolute("A.B.C.D");
    }
    if (ret) {
        StreamString auxStr = "B";
        ret = (auxStr == mdsStructuredDataI.GetName());
    }
    return ret;
}

bool MDSStructuredDataITest::TestCreateAbsolute_setTreeAndDestroy() {
    using namespace MARTe;
    MDSStructuredDataI mdsStructuredDataI;
    bool force = true;
    bool ret = mdsStructuredDataI.CreateTree(treeName.Buffer(), force);
    if (ret) {
        remove_mds_sdi = true;
    }
    MDSplus::Tree *tree = NULL_PTR(MDSplus::Tree *);
    try {
        tree = new MDSplus::Tree(treeName.Buffer(), -1, "EDIT");
    }
    catch (const MDSplus::MdsException &exc) {
        REPORT_ERROR_STATIC(MARTe::ErrorManagement::Warning, "Error opening tree %s. Error: %s", treeName.Buffer(), exc.what());
        ret = false;
    }
    if (ret) {
        ret = mdsStructuredDataI.SetTree(tree);
    }
    delete tree;
    if (ret) {
        ret = mdsStructuredDataI.CloseTree();
    }
    if (ret) {
        ret = !mdsStructuredDataI.CreateAbsolute("A.B");
    }
    return ret;
}

bool MDSStructuredDataITest::TestMoveToChild() {
    using namespace MARTe;
    MDSStructuredDataI mdsStructuredDataI;
    bool force = true;
    bool ret = mdsStructuredDataI.CreateTree(treeName.Buffer(), force);
    if (ret) {
        remove_mds_sdi = true;
    }
    if (ret) {
        mdsStructuredDataI.SetEditMode(true);
        ret = mdsStructuredDataI.OpenTree(treeName.Buffer(), -1);
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A");
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A.B");
    }
    if (ret) {
        ret = mdsStructuredDataI.MoveToAncestor(1u);
    }
    if (ret) {
        ret = mdsStructuredDataI.MoveToChild(0);
    }
    if (ret) {
        StreamString auxStr = "B";
        ret = (auxStr == mdsStructuredDataI.GetName());
    }
    return ret;
}

bool MDSStructuredDataITest::TestMoveToChild2() {
    using namespace MARTe;
    MDSStructuredDataI mdsStructuredDataI;
    bool force = true;
    bool ret = mdsStructuredDataI.CreateTree(treeName.Buffer(), force);
    if (ret) {
        remove_mds_sdi = true;
    }
    if (ret) {
        mdsStructuredDataI.SetEditMode(true);
        ret = mdsStructuredDataI.OpenTree(treeName.Buffer(), -1);
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A.A1");
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A.A2");
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A.A3");
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A.A4");
    }
    if (ret) {
        ret = mdsStructuredDataI.MoveToAncestor(1u);
    }
    if (ret) {
        ret = mdsStructuredDataI.MoveAbsolute("A");
    }
    if (ret) {
        ret = mdsStructuredDataI.Delete("A3");
    }
    if (ret) {
        ret = mdsStructuredDataI.MoveToChild(2);
    }
    if (ret) {
        StreamString auxStr = "A4";
        ret = (auxStr == mdsStructuredDataI.GetName());
    }
    return ret;
}

bool MDSStructuredDataITest::TestMoveToChild_invalidNode() {
    using namespace MARTe;
    MDSStructuredDataI mdsStructuredDataI;
    bool force = true;
    bool ret = mdsStructuredDataI.CreateTree(treeName.Buffer(), force);
    if (ret) {
        remove_mds_sdi = true;
    }
    if (ret) {
        mdsStructuredDataI.SetEditMode(true);
        ret = mdsStructuredDataI.OpenTree(treeName.Buffer(), -1);
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A");
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A.B");
    }
    if (ret) {
        ret = mdsStructuredDataI.MoveToAncestor(1u);
    }
    if (ret) {
        ret = !mdsStructuredDataI.MoveToChild(1);
    }
    return ret;
}

bool MDSStructuredDataITest::TestMoveToChild_ClosedTree() {
    using namespace MARTe;
    MDSStructuredDataI mdsStructuredDataI;
    bool force = true;
    bool ret = mdsStructuredDataI.CreateTree(treeName.Buffer(), force);
    if (ret) {
        remove_mds_sdi = true;
    }
    if (ret) {
        mdsStructuredDataI.SetEditMode(true);
        ret = mdsStructuredDataI.OpenTree(treeName.Buffer(), -1);
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A");
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A.B");
    }
    if (ret) {
        ret = mdsStructuredDataI.SaveTree();
    }
    if (ret) {
        ret = mdsStructuredDataI.CloseTree();
    }
    if (ret) {
        ret = !mdsStructuredDataI.MoveToChild(0);
    }
    return ret;
}

bool MDSStructuredDataITest::TestMoveToChild_setTreeAndDestroy() {
    using namespace MARTe;
    MDSStructuredDataI mdsStructuredDataI;
    bool force = true;
    bool ret = mdsStructuredDataI.CreateTree(treeName.Buffer(), force);
    if (ret) {
        remove_mds_sdi = true;
    }
    MDSplus::Tree *tree = NULL_PTR(MDSplus::Tree *);
    try {
        tree = new MDSplus::Tree(treeName.Buffer(), -1, "EDIT");
    }
    catch (const MDSplus::MdsException &exc) {
        REPORT_ERROR_STATIC(MARTe::ErrorManagement::Warning, "Error opening tree %s. Error: %s", treeName.Buffer(), exc.what());
        ret = false;
    }
    if (ret) {
        ret = mdsStructuredDataI.SetTree(tree);
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A.B");
    }
    if (ret) {
        ret = mdsStructuredDataI.MoveToAncestor(1u);
    }
    delete tree;
    if (ret) {
        ret = mdsStructuredDataI.CloseTree();
    }
    if (ret) {
        ret = !mdsStructuredDataI.MoveToChild(0);
    }
    return ret;
}

bool MDSStructuredDataITest::TestMoveRelative() {
    using namespace MARTe;
    MDSStructuredDataI mdsStructuredDataI;
    bool force = true;
    bool ret = mdsStructuredDataI.CreateTree(treeName.Buffer(), force);
    if (ret) {
        remove_mds_sdi = true;
    }
    if (ret) {
        mdsStructuredDataI.SetEditMode(true);
        ret = mdsStructuredDataI.OpenTree(treeName.Buffer(), -1);
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A");
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A.B");
    }
    if (ret) {
        ret = mdsStructuredDataI.MoveToAncestor(1u);
    }
    if (ret) {
        ret = mdsStructuredDataI.MoveRelative("B");
    }
    if (ret) {
        StreamString auxStr = "B";
        ret = (auxStr == mdsStructuredDataI.GetName());
    }
    return ret;
}

bool MDSStructuredDataITest::TestMoveRelative2() {
    using namespace MARTe;
    MDSStructuredDataI mdsStructuredDataI;
    bool force = true;
    bool ret = mdsStructuredDataI.CreateTree(treeName.Buffer(), force);
    if (ret) {
        remove_mds_sdi = true;
    }
    if (ret) {
        mdsStructuredDataI.SetEditMode(true);
        ret = mdsStructuredDataI.OpenTree(treeName.Buffer(), -1);
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A.B.C.D");
    }
    if (ret) {
        ret = mdsStructuredDataI.MoveToAncestor(2u);
    }
    if (ret) {
        ret = mdsStructuredDataI.MoveRelative("C.D");
    }
    if (ret) {
        StreamString auxStr = "D";
        ret = (auxStr == mdsStructuredDataI.GetName());
    }
    return ret;
}

bool MDSStructuredDataITest::TestMoveRelative_InvalidNode() {
    using namespace MARTe;
    MDSStructuredDataI mdsStructuredDataI;
    bool force = true;
    bool ret = mdsStructuredDataI.CreateTree(treeName.Buffer(), force);
    if (ret) {
        remove_mds_sdi = true;
    }
    if (ret) {
        mdsStructuredDataI.SetEditMode(true);
        ret = mdsStructuredDataI.OpenTree(treeName.Buffer(), -1);
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A");
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A.B");
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A.B.C");
    }
    if (ret) {
        ret = mdsStructuredDataI.MoveToAncestor(1);
    }
    if (ret) {
        ret = !mdsStructuredDataI.MoveRelative("B.C");
    }
    return ret;
}

bool MDSStructuredDataITest::TestMoveRelative_closedTree() {
    using namespace MARTe;
    MDSStructuredDataI mdsStructuredDataI;
    bool force = true;
    bool ret = mdsStructuredDataI.CreateTree(treeName.Buffer(), force);
    if (ret) {
        remove_mds_sdi = true;
    }
    if (ret) {
        mdsStructuredDataI.SetEditMode(true);
        ret = mdsStructuredDataI.OpenTree(treeName.Buffer(), -1);
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A");
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A.B");
    }
    if (ret) {
        ret = mdsStructuredDataI.SaveTree();
    }
    if (ret) {
        ret = mdsStructuredDataI.CloseTree();
    }
    if (ret) {
        ret = !mdsStructuredDataI.MoveRelative("B");
    }
    return ret;
}

bool MDSStructuredDataITest::TestMoveRelative_setTreeAndDestroy() {
    using namespace MARTe;
    MDSStructuredDataI mdsStructuredDataI;
    bool force = true;
    bool ret = mdsStructuredDataI.CreateTree(treeName.Buffer(), force);
    if (ret) {
        remove_mds_sdi = true;
    }
    MDSplus::Tree *tree = NULL_PTR(MDSplus::Tree *);
    try {
        tree = new MDSplus::Tree(treeName.Buffer(), -1, "EDIT");
    }
    catch (const MDSplus::MdsException &exc) {
        REPORT_ERROR_STATIC(MARTe::ErrorManagement::Warning, "Error opening tree %s. Error: %s", treeName.Buffer(), exc.what());
        ret = false;
    }
    if (ret) {
        ret = mdsStructuredDataI.SetTree(tree);
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A.B");
    }
    if (ret) {
        ret = mdsStructuredDataI.MoveToAncestor(1u);
    }
    delete tree;
    if (ret) {
        ret = mdsStructuredDataI.CloseTree();
    }
    if (ret) {
        ret = !mdsStructuredDataI.MoveRelative("A");
    }
    return ret;
}

bool MDSStructuredDataITest::TestMoveAbsolute() {
    using namespace MARTe;
    MDSStructuredDataI mdsStructuredDataI;
    bool force = true;
    bool ret = mdsStructuredDataI.CreateTree(treeName.Buffer(), force);
    if (ret) {
        remove_mds_sdi = true;
    }
    if (ret) {
        mdsStructuredDataI.SetEditMode(true);
        ret = mdsStructuredDataI.OpenTree(treeName.Buffer(), -1);
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A");
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A.B");
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("D");
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("D.E");
    }
    if (ret) {
        ret = mdsStructuredDataI.MoveAbsolute("A.B");
    }
    if (ret) {
        StreamString auxStr = "B";
        ret = (auxStr == mdsStructuredDataI.GetName());
    }
    return ret;
}

bool MDSStructuredDataITest::TestMoveAbsolute2() {
    using namespace MARTe;
    MDSStructuredDataI mdsStructuredDataI;
    bool force = true;
    bool ret = mdsStructuredDataI.CreateTree(treeName.Buffer(), force);
    if (ret) {
        remove_mds_sdi = true;
    }
    if (ret) {
        mdsStructuredDataI.SetEditMode(true);
        ret = mdsStructuredDataI.OpenTree(treeName.Buffer(), -1);
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A.B.C.D.E");
    }
    if (ret) {
        ret = mdsStructuredDataI.MoveAbsolute("A.B");
    }
    if (ret) {
        StreamString auxStr = "B";
        ret = (auxStr == mdsStructuredDataI.GetName());
    }
    return ret;
}

bool MDSStructuredDataITest::TestMoveAbsolute_InvalidNode() {
    using namespace MARTe;
    MDSStructuredDataI mdsStructuredDataI;
    bool force = true;
    bool ret = mdsStructuredDataI.CreateTree(treeName.Buffer(), force);
    if (ret) {
        remove_mds_sdi = true;
    }
    if (ret) {
        mdsStructuredDataI.SetEditMode(true);
        ret = mdsStructuredDataI.OpenTree(treeName.Buffer(), -1);
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A");
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A.B");
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("D");
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("D.E");
    }
    if (ret) {
        ret = !mdsStructuredDataI.MoveAbsolute("A.E");
    }
    return ret;
}

bool MDSStructuredDataITest::TestMoveAbsolute_closedTree() {
    using namespace MARTe;
    MDSStructuredDataI mdsStructuredDataI;
    bool force = true;
    bool ret = mdsStructuredDataI.CreateTree(treeName.Buffer(), force);
    if (ret) {
        remove_mds_sdi = true;
    }
    if (ret) {
        mdsStructuredDataI.SetEditMode(true);
        ret = mdsStructuredDataI.OpenTree(treeName.Buffer(), -1);
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A");
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A.B");
    }
    if (ret) {
        ret = mdsStructuredDataI.SaveTree();
    }
    if (ret) {
        ret = mdsStructuredDataI.CloseTree();
    }
    if (ret) {
        ret = !mdsStructuredDataI.MoveAbsolute("A.B");
    }
    return ret;
}

bool MDSStructuredDataITest::TestMoveAbsolute_setTreeAndDestroy() {
    using namespace MARTe;
    MDSStructuredDataI mdsStructuredDataI;
    bool force = true;
    bool ret = mdsStructuredDataI.CreateTree(treeName.Buffer(), force);
    if (ret) {
        remove_mds_sdi = true;
    }
    MDSplus::Tree *tree = NULL_PTR(MDSplus::Tree *);
    try {
        tree = new MDSplus::Tree(treeName.Buffer(), -1, "EDIT");
    }
    catch (const MDSplus::MdsException &exc) {
        REPORT_ERROR_STATIC(MARTe::ErrorManagement::Warning, "Error opening tree %s. Error: %s", treeName.Buffer(), exc.what());
        ret = false;
    }
    if (ret) {
        ret = mdsStructuredDataI.SetTree(tree);
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A.B");
    }
    delete tree;
    if (ret) {
        ret = mdsStructuredDataI.CloseTree();
    }
    if (ret) {
        ret = !mdsStructuredDataI.MoveAbsolute("A");
    }
    return ret;
}

bool MDSStructuredDataITest::TestMoveToAncestor() {
    using namespace MARTe;
    MDSStructuredDataI mdsStructuredDataI;
    bool force = true;
    bool ret = mdsStructuredDataI.CreateTree(treeName.Buffer(), force);
    if (ret) {
        remove_mds_sdi = true;
    }
    if (ret) {
        mdsStructuredDataI.SetEditMode(true);
        ret = mdsStructuredDataI.OpenTree(treeName.Buffer(), -1);
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A");
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A.B");
    }
    if (ret) {
        ret = mdsStructuredDataI.MoveToAncestor(1);
    }
    if (ret) {
        StreamString auxStr = "A";
        ret = auxStr == mdsStructuredDataI.GetName();
    }
    return ret;
}

bool MDSStructuredDataITest::TestMoveToAncestor2() {
    using namespace MARTe;
    MDSStructuredDataI mdsStructuredDataI;
    bool force = true;
    bool ret = mdsStructuredDataI.CreateTree(treeName.Buffer(), force);
    if (ret) {
        remove_mds_sdi = true;
    }
    if (ret) {
        mdsStructuredDataI.SetEditMode(true);
        ret = mdsStructuredDataI.OpenTree(treeName.Buffer(), -1);
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A");
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A.B");
    }
    if (ret) {
        ret = mdsStructuredDataI.MoveToAncestor(2);
    }
    if (ret) {
        StreamString auxStr = "TOP";
        ret = auxStr == mdsStructuredDataI.GetName();
    }
    return ret;
}

bool MDSStructuredDataITest::TestMoveToAncestor3() {
    using namespace MARTe;
    MDSStructuredDataI mdsStructuredDataI;
    bool force = true;
    bool ret = mdsStructuredDataI.CreateTree(treeName.Buffer(), force);
    if (ret) {
        remove_mds_sdi = true;
    }
    if (ret) {
        mdsStructuredDataI.SetEditMode(true);
        ret = mdsStructuredDataI.OpenTree(treeName.Buffer(), -1);
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A");
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A.B");
    }
    if (ret) {
        ret = mdsStructuredDataI.MoveToAncestor(3);
    }
    if (ret) {
        StreamString auxStr = "TOP";
        ret = auxStr == mdsStructuredDataI.GetName();
    }
    return ret;
}

bool MDSStructuredDataITest::TestMoveToAncestor4() {
    using namespace MARTe;
    MDSStructuredDataI mdsStructuredDataI;
    bool force = true;
    bool ret = mdsStructuredDataI.CreateTree(treeName.Buffer(), force);
    if (ret) {
        remove_mds_sdi = true;
    }
    if (ret) {
        mdsStructuredDataI.SetEditMode(true);
        ret = mdsStructuredDataI.OpenTree(treeName.Buffer(), -1);
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A");
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A.B");
    }
    if (ret) {
        ret = mdsStructuredDataI.MoveToAncestor(4);
    }
    if (ret) {
        StreamString auxStr = "TOP";
        ret = auxStr == mdsStructuredDataI.GetName();
    }
    return ret;
}

bool MDSStructuredDataITest::TestMoveToAncestor_treeClosed() {
    using namespace MARTe;
    MDSStructuredDataI mdsStructuredDataI;
    bool force = true;
    bool ret = mdsStructuredDataI.CreateTree(treeName.Buffer(), force);
    if (ret) {
        remove_mds_sdi = true;
    }
    if (ret) {
        mdsStructuredDataI.SetEditMode(true);
        ret = mdsStructuredDataI.OpenTree(treeName.Buffer(), -1);
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A");
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A.B");
    }
    if (ret) {
        ret = mdsStructuredDataI.SaveTree();
    }
    if (ret) {
        ret = mdsStructuredDataI.CloseTree();
    }
    if (ret) {
        ret = !mdsStructuredDataI.MoveToAncestor(2);
    }
    return ret;
}

bool MDSStructuredDataITest::TestMoveToAncestor_setTreeAndDestroy() {
    using namespace MARTe;
    MDSStructuredDataI mdsStructuredDataI;
    bool force = true;
    bool ret = mdsStructuredDataI.CreateTree(treeName.Buffer(), force);
    if (ret) {
        remove_mds_sdi = true;
    }
    MDSplus::Tree *tree = NULL_PTR(MDSplus::Tree *);
    try {
        tree = new MDSplus::Tree(treeName.Buffer(), -1, "EDIT");
    }
    catch (const MDSplus::MdsException &exc) {
        REPORT_ERROR_STATIC(MARTe::ErrorManagement::Warning, "Error opening tree %s. Error: %s", treeName.Buffer(), exc.what());
        ret = false;
    }
    if (ret) {
        ret = mdsStructuredDataI.SetTree(tree);
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A.B");
    }
    delete tree;
    if (ret) {
        ret = mdsStructuredDataI.CloseTree();
    }
    if (ret) {
        ret = !mdsStructuredDataI.MoveToAncestor(1u);
    }
    return ret;
}

bool MDSStructuredDataITest::TestMoveToRoot() {
    using namespace MARTe;
    MDSStructuredDataI mdsStructuredDataI;
    bool force = true;
    bool ret = mdsStructuredDataI.CreateTree(treeName.Buffer(), force);
    if (ret) {
        remove_mds_sdi = true;
    }
    if (ret) {
        mdsStructuredDataI.SetEditMode(true);
        ret = mdsStructuredDataI.OpenTree(treeName.Buffer(), -1);
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A");
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A.B");
    }
    if (ret) {
        ret = mdsStructuredDataI.MoveToRoot();
    }
    if (ret) {
        StreamString auxStr = "TOP";
        ret = (auxStr == mdsStructuredDataI.GetName());
    }
    return ret;
}

bool MDSStructuredDataITest::TestMoveToRoot_treeClosed() {
    using namespace MARTe;
    MDSStructuredDataI mdsStructuredDataI;
    bool force = true;
    bool ret = mdsStructuredDataI.CreateTree(treeName.Buffer(), force);
    if (ret) {
        remove_mds_sdi = true;
    }
    if (ret) {
        mdsStructuredDataI.SetEditMode(true);
        ret = mdsStructuredDataI.OpenTree(treeName.Buffer(), -1);
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A");
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A.B");
    }
    if (ret) {
        ret = mdsStructuredDataI.SaveTree();
    }
    if (ret) {
        ret = mdsStructuredDataI.CloseTree();
    }
    if (ret) {
        ret = !mdsStructuredDataI.MoveToRoot();
    }
    return ret;
}

bool MDSStructuredDataITest::TestMoveToRoot_setTreeAndDestroy() {
    using namespace MARTe;
    MDSStructuredDataI mdsStructuredDataI;
    bool force = true;
    bool ret = mdsStructuredDataI.CreateTree(treeName.Buffer(), force);
    if (ret) {
        remove_mds_sdi = true;
    }
    MDSplus::Tree *tree = NULL_PTR(MDSplus::Tree *);
    try {
        tree = new MDSplus::Tree(treeName.Buffer(), -1, "EDIT");
    }
    catch (const MDSplus::MdsException &exc) {
        REPORT_ERROR_STATIC(MARTe::ErrorManagement::Warning, "Error opening tree %s. Error: %s", treeName.Buffer(), exc.what());
        ret = false;
    }
    if (ret) {
        ret = mdsStructuredDataI.SetTree(tree);
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A.B");
    }
    delete tree;
    if (ret) {
        ret = mdsStructuredDataI.CloseTree();
    }
    if (ret) {
        ret = !mdsStructuredDataI.MoveToRoot();
    }
    return ret;
}

bool MDSStructuredDataITest::TestCopy() {
    using namespace MARTe;
    MDSStructuredDataI mdsStructuredDataI;
    MDSStructuredDataI mdsStructuredDataI2;
    bool force = true;
    bool ret = mdsStructuredDataI.CreateTree(treeName.Buffer(), force);
    if (ret) {
        remove_mds_sdi = true;
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateTree(treeName2.Buffer(), force);
    }
    if (ret) {
        remove_mds_sdi2 = true;
    }
    if (ret) {
        mdsStructuredDataI.SetEditMode(true);
        ret = mdsStructuredDataI.OpenTree(treeName.Buffer(), -1);
    }
    if (ret) {
        ret = mdsStructuredDataI2.SetEditMode(true);
    }
    if (ret) {
        ret = mdsStructuredDataI2.OpenTree(treeName2.Buffer(), -1);
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A");
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("B");
    }
    if (ret) {
        ret = mdsStructuredDataI.MoveToRoot();
    }
    if (ret) {
        ret = mdsStructuredDataI.Copy(mdsStructuredDataI2);
    }
    if (ret) {
        ret = (mdsStructuredDataI.GetNumberOfChildren() == mdsStructuredDataI2.GetNumberOfChildren());
    }
    for (uint32 i = 0; (i < mdsStructuredDataI.GetNumberOfChildren()) && ret; i++) {
        StreamString auxStr = mdsStructuredDataI.GetChildName(i);
        ret = (auxStr == mdsStructuredDataI2.GetChildName(i));
    }
    return ret;
}

bool MDSStructuredDataITest::TestCopy2() {
    using namespace MARTe;
    MDSStructuredDataI mdsStructuredDataI;
    ConfigurationDatabase cdb;
    bool force = true;
    bool ret = mdsStructuredDataI.CreateTree(treeName.Buffer(), force);
    if (ret) {
        remove_mds_sdi = true;
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateTree(treeName2.Buffer(), force);
    }
    if (ret) {
        remove_mds_sdi2 = true;
    }
    if (ret) {
        mdsStructuredDataI.SetEditMode(true);
        ret = mdsStructuredDataI.OpenTree(treeName.Buffer(), -1);
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A");
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("B");
    }
    if (ret) {
        ret = mdsStructuredDataI.MoveToRoot();
    }
    if (ret) {
        ret = mdsStructuredDataI.Copy(cdb);
    }
    if (ret) {
        ret = (mdsStructuredDataI.GetNumberOfChildren() == cdb.GetNumberOfChildren());
    }

    for (uint32 i = 0; (i < mdsStructuredDataI.GetNumberOfChildren()) && ret; i++) {
        StreamString auxStr = mdsStructuredDataI.GetChildName(i);
        ret = (auxStr == cdb.GetChildName(i));
    }
    return ret;
}

bool MDSStructuredDataITest::TestCopy_treeClosed() {
    using namespace MARTe;
    MDSStructuredDataI mdsStructuredDataI;
    MDSStructuredDataI mdsStructuredDataI2;
    bool force = true;
    bool ret = mdsStructuredDataI.CreateTree(treeName.Buffer(), force);
    if (ret) {
        remove_mds_sdi = true;
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateTree(treeName2.Buffer(), force);
    }
    if (ret) {
        remove_mds_sdi2 = true;
    }
    if (ret) {
        mdsStructuredDataI.SetEditMode(true);
        ret = mdsStructuredDataI.OpenTree(treeName.Buffer(), -1);
    }
    if (ret) {
        ret = mdsStructuredDataI2.SetEditMode(true);
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A");
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("B");
    }
    if (ret) {
        ret = mdsStructuredDataI.MoveToRoot();
    }
    if (ret) {
        ret = !mdsStructuredDataI.Copy(mdsStructuredDataI2);
    }
    return ret;
}

bool MDSStructuredDataITest::TestCopy_treeClosed2() {
    using namespace MARTe;
    MDSStructuredDataI mdsStructuredDataI;
    MDSStructuredDataI mdsStructuredDataI2;
    bool force = true;
    bool ret = mdsStructuredDataI.CreateTree(treeName.Buffer(), force);
    if (ret) {
        remove_mds_sdi = true;
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateTree(treeName2.Buffer(), force);
    }
    if (ret) {
        remove_mds_sdi2 = true;
    }
    if (ret) {
        mdsStructuredDataI.SetEditMode(true);
        ret = mdsStructuredDataI.OpenTree(treeName.Buffer(), -1);
    }
    if (ret) {
        ret = mdsStructuredDataI2.SetEditMode(true);
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A");
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("B");
    }
    if (ret) {
        ret = mdsStructuredDataI.CloseTree();
    }
    if (ret) {
        ret = !mdsStructuredDataI.Copy(mdsStructuredDataI2);
    }
    return ret;
}

bool MDSStructuredDataITest::TestWrite_string() {
    using namespace MARTe;
    MDSStructuredDataI mdsStructuredDataI;
    bool force = true;
    bool ret = mdsStructuredDataI.CreateTree(treeName.Buffer(), force);
    if (ret) {
        remove_mds_sdi = true;
    }
    if (ret) {
        mdsStructuredDataI.SetEditMode(true);
        ret = mdsStructuredDataI.OpenTree(treeName.Buffer(), -1);
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A");
    }
    StreamString value = "F4E";
    if (ret) {
        ret = mdsStructuredDataI.Write("NodeVal", value.Buffer());
    }
    if (ret) {
        ret = mdsStructuredDataI.SaveTree();
    }
    AnyType returnValue = mdsStructuredDataI.GetType("NodeVal");
    char *retVal = new char8[returnValue.GetNumberOfElements(0u) + 1];
    if (ret) {
        ret = mdsStructuredDataI.Read("NodeVal", retVal);
    }
    if (ret) {
        StreamString auxStreamString(retVal);
        ret = (auxStreamString == "F4E");
    }
    delete[] retVal;
    return ret;
}

bool MDSStructuredDataITest::TestWrite_constString() {
    using namespace MARTe;
    MDSStructuredDataI mdsStructuredDataI;
    bool force = true;
    bool ret = mdsStructuredDataI.CreateTree(treeName.Buffer(), force);
    if (ret) {
        remove_mds_sdi = true;
    }
    if (ret) {
        mdsStructuredDataI.SetEditMode(true);
        ret = mdsStructuredDataI.OpenTree(treeName.Buffer(), -1);
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A");
    }
    const char8 value[] = "F4E";
    const char8 *pValue = value;
    if (ret) {
        ret = mdsStructuredDataI.Write("NodeVal", pValue);
    }
    if (ret) {
        ret = mdsStructuredDataI.SaveTree();
    }
    AnyType returnValue = mdsStructuredDataI.GetType("NodeVal");
    char *retVal = new char8[returnValue.GetNumberOfElements(0u) + 1];
    if (ret) {
        ret = mdsStructuredDataI.Read("NodeVal", retVal);
    }
    if (ret) {
        StreamString auxStreamString(retVal);
        ret = (auxStreamString == "F4E");
    }
    delete[] retVal;
    return ret;
}

bool MDSStructuredDataITest::TestWrite_char() {
    using namespace MARTe;
    MDSStructuredDataI mdsStructuredDataI;
    bool force = true;
    bool ret = mdsStructuredDataI.CreateTree(treeName.Buffer(), force);
    if (ret) {
        remove_mds_sdi = true;
    }
    if (ret) {
        mdsStructuredDataI.SetEditMode(true);
        ret = mdsStructuredDataI.OpenTree(treeName.Buffer(), -1);
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A");
    }
    char8 value = 'F';
    if (ret) {
        ret = mdsStructuredDataI.Write("NodeVal", value);
    }
    if (ret) {
        ret = mdsStructuredDataI.SaveTree();
    }
    AnyType returnValue = mdsStructuredDataI.GetType("NodeVal");
    char *retVal = new char8[returnValue.GetNumberOfElements(0u) + 1];
    if (ret) {
        ret = mdsStructuredDataI.Read("NodeVal", retVal);
    }
    if (ret) {
        StreamString auxStreamString(retVal);
        ret = (auxStreamString == "F");
    }
    delete[] retVal;
    return ret;
}

bool MDSStructuredDataITest::TestWrite_charArray() {
    using namespace MARTe;
    MDSStructuredDataI mdsStructuredDataI;
    bool force = true;
    bool ret = mdsStructuredDataI.CreateTree(treeName.Buffer(), force);
    if (ret) {
        remove_mds_sdi = true;
    }
    if (ret) {
        mdsStructuredDataI.SetEditMode(true);
        ret = mdsStructuredDataI.OpenTree(treeName.Buffer(), -1);
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A");
    }
    char8 value[] = { 'F', '4', 'E' };
    if (ret) {
        ret = mdsStructuredDataI.Write("NodeVal", value);
    }
    if (ret) {
        ret = mdsStructuredDataI.SaveTree();
    }
    AnyType returnValue = mdsStructuredDataI.GetType("NodeVal");
    char *retVal = new char8[returnValue.GetNumberOfElements(0u) + 1];
    if (ret) {
        ret = mdsStructuredDataI.Read("NodeVal", retVal);
    }
    if (ret) {
        StreamString auxStreamString(retVal);
        ret = (auxStreamString == "F4E");
    }
    delete[] retVal;
    return ret;
}

bool MDSStructuredDataITest::TestWrite_Recursively() {
    using namespace MARTe;
    MDSStructuredDataI mdsStructuredDataI;
    bool force = true;
    bool ret = mdsStructuredDataI.CreateTree(treeName.Buffer(), force);
    if (ret) {
        remove_mds_sdi = true;
    }
    if (ret) {
        mdsStructuredDataI.SetEditMode(true);
        ret = mdsStructuredDataI.OpenTree(treeName.Buffer(), -1);
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A.B");
    }
    float32 value = 31.5;
    if (ret) {
        ret = mdsStructuredDataI.Write("NodeVal", value);
    }
    if (ret) {
        ret = mdsStructuredDataI.SaveTree();
    }
    if (ret) {
        ret = mdsStructuredDataI.MoveToRoot();
    }
    float returnValue;
    if (ret) {
        ret = mdsStructuredDataI.Read("A.B.NodeVal", returnValue);
    }
    if (ret) {
        ret = (returnValue == value);
    }
    return ret;
}

bool MDSStructuredDataITest::TestWrite_noEditable() {
    using namespace MARTe;
    MDSStructuredDataI mdsStructuredDataI;
    bool force = true;
    bool ret = mdsStructuredDataI.CreateTree(treeName.Buffer(), force);
    if (ret) {
        remove_mds_sdi = true;
    }
    if (ret) {
        mdsStructuredDataI.SetEditMode(true);
        ret = mdsStructuredDataI.OpenTree(treeName.Buffer(), -1);
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A.B");
    }
    if (ret) {
        ret = mdsStructuredDataI.SaveTree();
    }
    if (ret) {
        ret = mdsStructuredDataI.SetEditMode(false);
    }
    float32 value = 31.5;
    if (ret) {
        ret = !mdsStructuredDataI.Write("NodeVal", value);
    }
    return ret;
}

bool MDSStructuredDataITest::TestWrite_InvalidType() {
    using namespace MARTe;
    MDSStructuredDataI mdsStructuredDataI;
    bool force = true;
    bool ret = mdsStructuredDataI.CreateTree(treeName.Buffer(), force);
    if (ret) {
        remove_mds_sdi = true;
    }
    if (ret) {
        mdsStructuredDataI.SetEditMode(true);
        ret = mdsStructuredDataI.OpenTree(treeName.Buffer(), -1);
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A");
    }
    StreamString value = "F4E";
    if (ret) {
        ret = !mdsStructuredDataI.Write("NodeVal", value);
    }
    return ret;
}

bool MDSStructuredDataITest::TestWrite_treeClosed() {
    using namespace MARTe;
    MDSStructuredDataI mdsStructuredDataI;
    bool force = true;
    bool ret = mdsStructuredDataI.CreateTree(treeName.Buffer(), force);
    if (ret) {
        remove_mds_sdi = true;
    }
    if (ret) {
        mdsStructuredDataI.SetEditMode(true);
        ret = mdsStructuredDataI.OpenTree(treeName.Buffer(), -1);
    }
    if (ret) {
        ret = mdsStructuredDataI.CloseTree();
    }
    if (ret) {
        ret = !mdsStructuredDataI.Write("node", 3);
    }
    return ret;
}

bool MDSStructuredDataITest::TestWrite_setTreeAndDestroy() {
    using namespace MARTe;
    MDSStructuredDataI mdsStructuredDataI;
    bool force = true;
    bool ret = mdsStructuredDataI.CreateTree(treeName.Buffer(), force);
    if (ret) {
        remove_mds_sdi = true;
    }
    MDSplus::Tree *tree = NULL_PTR(MDSplus::Tree *);
    try {
        tree = new MDSplus::Tree(treeName.Buffer(), -1, "EDIT");
    }
    catch (const MDSplus::MdsException &exc) {
        REPORT_ERROR_STATIC(MARTe::ErrorManagement::Warning, "Error opening tree %s. Error: %s", treeName.Buffer(), exc.what());
        ret = false;
    }
    if (ret) {
        ret = mdsStructuredDataI.SetTree(tree);
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A.B");
    }
    delete tree;
    if (ret) {
        ret = mdsStructuredDataI.CloseTree();
    }
    if (ret) {
        ret = !mdsStructuredDataI.Write("SomeNodeName", 3.0);
    }
    return ret;
}

bool MDSStructuredDataITest::TestGetType_Recursively() {
    using namespace MARTe;
    MDSStructuredDataI mdsStructuredDataI;
    bool force = true;
    bool ret = mdsStructuredDataI.CreateTree(treeName.Buffer(), force);
    if (ret) {
        remove_mds_sdi = true;
    }
    if (ret) {
        ret = mdsStructuredDataI.SetEditMode(true);
    }
    if (ret) {
        ret = mdsStructuredDataI.OpenTree(treeName.Buffer(), -1);
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A.B");
    }
    if (ret) {
        float32 aux = 4.56;
        ret = mdsStructuredDataI.Write("F4E", aux);
    }
    if (ret) {
        ret = mdsStructuredDataI.MoveToRoot();
    }
    if (ret) {
        AnyType inputType(mdsStructuredDataI.GetType("A.B.F4E"));
        ret = !inputType.IsVoid();
        if (ret) {
            ret = inputType.GetNumberOfElements(0u) == 1;
        }
    }
    return ret;
}

bool MDSStructuredDataITest::TestGetType_treeClosed() {
    using namespace MARTe;
    MDSStructuredDataI mdsStructuredDataI;
    bool force = true;
    bool ret = mdsStructuredDataI.CreateTree(treeName.Buffer(), force);
    if (ret) {
        remove_mds_sdi = true;
    }
    if (ret) {
        ret = mdsStructuredDataI.SetEditMode(true);
    }
    if (ret) {
        ret = mdsStructuredDataI.OpenTree(treeName.Buffer(), -1);
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A.B");
    }
    if (ret) {
        float32 aux = 4.56;
        ret = mdsStructuredDataI.Write("F4E", aux);
    }
    if (ret) {
        ret = mdsStructuredDataI.CloseTree();
    }
    if (ret) {
        AnyType inputType(mdsStructuredDataI.GetType("F4E"));
        ret = inputType.IsVoid();
    }
    return ret;
}

bool MDSStructuredDataITest::TestGetType_setTreeAndDestroy() {
    using namespace MARTe;
    MDSStructuredDataI mdsStructuredDataI;
    bool force = true;
    bool ret = mdsStructuredDataI.CreateTree(treeName.Buffer(), force);
    if (ret) {
        remove_mds_sdi = true;
    }
    MDSplus::Tree *tree = NULL_PTR(MDSplus::Tree *);
    try {
        tree = new MDSplus::Tree(treeName.Buffer(), -1, "EDIT");
    }
    catch (const MDSplus::MdsException &exc) {
        REPORT_ERROR_STATIC(MARTe::ErrorManagement::Warning, "Error opening tree %s. Error: %s", treeName.Buffer(), exc.what());
        ret = false;
    }
    if (ret) {
        ret = mdsStructuredDataI.SetTree(tree);
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A.B");
    }
    if (ret) {
        float32 aux = 4.56;
        ret = mdsStructuredDataI.Write("F4E", aux);
    }
    delete tree;
    if (ret) {
        ret = mdsStructuredDataI.CloseTree();
    }
    if (ret) {
        AnyType inputType(mdsStructuredDataI.GetType("F4E"));
        ret = inputType.IsVoid();
    }
    return ret;
}

bool MDSStructuredDataITest::TestRead_String(MARTe::StreamString val) {
    using namespace MARTe;
    MDSStructuredDataI mdsStructuredDataI;
    bool force = true;
    bool ret = mdsStructuredDataI.CreateTree(treeName.Buffer(), force);
    if (ret) {
        remove_mds_sdi = true;
    }
    if (ret) {
        mdsStructuredDataI.SetEditMode(true);
        ret = mdsStructuredDataI.OpenTree(treeName.Buffer(), -1);
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A");
    }
    if (ret) {
        ret = mdsStructuredDataI.Write("NodeVal", val.Buffer());
    }
    if (ret) {
        ret = mdsStructuredDataI.SaveTree();
    }
    AnyType auxAnyType = mdsStructuredDataI.GetType("NodeVal");
    char8 * charPointer = new char8[auxAnyType.GetNumberOfElements(0u) + 1];
    if (ret) {
        ret = mdsStructuredDataI.Read("NodeVal", charPointer);
    }
    if (ret) {
        StreamString auxStream = charPointer;
        ret = (auxStream == val);
    }
    return ret;
}

bool MDSStructuredDataITest::TestRead_Recursively() {
    using namespace MARTe;
    MDSStructuredDataI mdsStructuredDataI;
    bool force = true;
    bool ret = mdsStructuredDataI.CreateTree(treeName.Buffer(), force);
    if (ret) {
        remove_mds_sdi = true;
    }
    if (ret) {
        mdsStructuredDataI.SetEditMode(true);
        ret = mdsStructuredDataI.OpenTree(treeName.Buffer(), -1);
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A.B");
    }
    float32 val = 32.1;
    if (ret) {
        ret = mdsStructuredDataI.Write("NodeVal", val);
    }
    if (ret) {
        ret = mdsStructuredDataI.SaveTree();
    }
    float32 returnValue;
    if (ret) {
        ret = mdsStructuredDataI.MoveToRoot();
    }
    if (ret) {
        ret = mdsStructuredDataI.Read("A.B.NodeVal", returnValue);
    }
    if (ret) {
        ret = (returnValue == val);
    }
    return ret;
}

bool MDSStructuredDataITest::TestRead_StreamString(MARTe::StreamString val) {
    using namespace MARTe;
    MDSStructuredDataI mdsStructuredDataI;
    bool force = true;
    bool ret = mdsStructuredDataI.CreateTree(treeName.Buffer(), force);
    if (ret) {
        remove_mds_sdi = true;
    }
    if (ret) {
        mdsStructuredDataI.SetEditMode(true);
        ret = mdsStructuredDataI.OpenTree(treeName.Buffer(), -1);
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A");
    }
    if (ret) {
        ret = mdsStructuredDataI.Write("NodeVal", val.Buffer());
    }
    if (ret) {
        ret = mdsStructuredDataI.SaveTree();
    }
    StreamString auxStreamString;
    if (ret) {
        ret = !mdsStructuredDataI.Read("NodeVal", auxStreamString);
    }
    return ret;
}

bool MDSStructuredDataITest::TestRead_NoOpenTree() {
    using namespace MARTe;
    MDSStructuredDataI mdsStructuredDataI;
    ConfigurationDatabase cdb;
    bool force = true;
    bool ret = mdsStructuredDataI.CreateTree(treeName.Buffer(), force);
    if (ret) {
        remove_mds_sdi = true;
    }
    float value;
    if (ret) {
        ret = !mdsStructuredDataI.Read("whatever", value);
    }
    return ret;
}

bool MDSStructuredDataITest::TestRead_InvalidNode() {
    using namespace MARTe;
    MDSStructuredDataI mdsStructuredDataI;
    ConfigurationDatabase cdb;
    bool force = true;
    bool ret = mdsStructuredDataI.CreateTree(treeName.Buffer(), force);
    if (ret) {
        remove_mds_sdi = true;
    }
    if (ret) {
        ret = mdsStructuredDataI.OpenTree(treeName.Buffer(), -1);
    }
    float value;
    if (ret) {
        ret = !mdsStructuredDataI.Read("whatever", value);
    }
    return ret;
}

bool MDSStructuredDataITest::TestRead_setTreeAndDestroy() {
    using namespace MARTe;
    MDSStructuredDataI mdsStructuredDataI;
    ConfigurationDatabase cdb;
    bool force = true;
    bool ret = mdsStructuredDataI.CreateTree(treeName.Buffer(), force);
    if (ret) {
        remove_mds_sdi = true;
    }

    MDSplus::Tree *tree = NULL_PTR(MDSplus::Tree *);
    try {
        tree = new MDSplus::Tree(treeName.Buffer(), -1, "EDIT");
    }
    catch (const MDSplus::MdsException &exc) {
        REPORT_ERROR_STATIC(MARTe::ErrorManagement::Warning, "Error opening tree %s. Error: %s", treeName.Buffer(), exc.what());
        ret = false;
    }
    if (ret) {
        mdsStructuredDataI.SetTree(tree);
    }
    if (ret) {
        delete tree;
    }
    if (ret) {
        ret = mdsStructuredDataI.CloseTree();
    }
    float value;
    if (ret) {
        ret = !mdsStructuredDataI.Read("whatever", value);
    }
    return ret;
}

bool MDSStructuredDataITest::TestCfgDB() {
    using namespace MARTe;
    MDSStructuredDataI mdsStructuredDataI;
    bool ret = mdsStructuredDataI.CreateTree(treeName.Buffer());
    if (ret) {
        remove_mds_sdi = true;
    }
    if (ret) {
        ret = mdsStructuredDataI.SetEditMode(true);
    }
    if (ret) {
        ret = mdsStructuredDataI.OpenTree(treeName.Buffer(), -1);
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A");
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A.B");
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A.B.C");
    }
    float32 f = 9;
    if (ret) {
        ret = mdsStructuredDataI.Write("AFloat", f);
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A.B.D");
    }
    StreamString b = "HELLO!";
    if (ret) {
        ret = mdsStructuredDataI.Write("AString", b.Buffer());
    }

    const char* config = "Test = {"
            "    Class = RealTimeApplication"
            "    Functions = {"
            "        Class = ReferenceContainer"
            "        GAM1 = {"
            "            Class = MDSWriterGAMTriggerTestHelper"
            "            Signal =   {8 1 2 3 4 5 6 7 }"
            "            Trigger =  {0 1 0 1 0 1 0 1 }"
            "            OutputSignals = {"
            "                Trigger = {"
            "                    Type = uint8"
            "                    DataSource = Drv1"
            "                }"
            "                Time = {"
            "                    Type = float64"
            "                    DataSource = Drv1"
            "                }"
            "                SignalUInt16F = {"
            "                    Type = uint16"
            "                    DataSource = Drv1"
            "                }"
            "                SignalInt16F = {"
            "                    Type = int16"
            "                    DataSource = Drv1"
            "                }"
            "            }"
            "        }"
            "    }"
            "    Data = {"
            "        Class = ReferenceContainer"
            "        DefaultDataSource = DDB1"
            "        Timings = {"
            "            Class = TimingDataSource"
            "        }"
            "        Drv1 = {"
            "            Class = MDSWriter"
            "            NumberOfBuffers = 10"
            "            CPUMask = 15"
            "            StackSize = 10000000"
            "            TreeName = \"mds_m2test\""
            "            StoreOnTrigger = 1"
            "            EventName = \"updatejScope\""
            "            TimeRefresh = 5"
            "            NumberOfPreTriggers = 1"
            "            NumberOfPostTriggers = 1"
            "            Signals = {"
            "                Trigger = {"
            "                    Type = uint8"
            "                }"
            "                Time = {"
            "                    Type = float64"
            "                    TimeSignal = 1"
            "                }"
            "                SignalUInt16F = {"
            "                    NodeName = \"SIGUINT16F\""
            "                    Period = 2"
            "                    MakeSegmentAfterNWrites = 4"
            "                    DecimatedNodeName = \"SIGUINT16D\""
            "                    MinMaxResampleFactor = 4"
            "                    AutomaticSegmentation = 0"
            "                }"
            "                SignalInt16F = {"
            "                    NodeName = \"SIGINT16F\""
            "                    Period = 2"
            "                    MakeSegmentAfterNWrites = 4"
            "                    DecimatedNodeName = \"SIGINT16D\""
            "                    MinMaxResampleFactor = 4"
            "                    AutomaticSegmentation = 0"
            "                }"
            "            }"
            "        }"
            "    }"
            "    States = {"
            "        Class = ReferenceContainer"
            "        State1 = {"
            "            Class = RealTimeState"
            "            Threads = {"
            "                Class = ReferenceContainer"
            "                Thread1 = {"
            "                    Class = RealTimeThread"
            "                    Functions = {GAM1}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    Scheduler = {"
            "        Class = MDSWriterSchedulerTestHelper"
            "        TimingDataSource = Timings"
            "    }"
            "}";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StreamString err;
    StandardParser parser(configStream, cdb, &err);

    bool ok = parser.Parse();
    if (!ok) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "%s", err.Buffer());
    }
    cdb.MoveToRoot();
    if (ret) {
        ret = cdb.Copy(mdsStructuredDataI);
    }
    if (ret) {
        ret = mdsStructuredDataI.SaveTree();
    }
    return ret;
}

