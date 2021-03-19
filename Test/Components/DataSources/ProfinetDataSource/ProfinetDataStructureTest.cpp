/**
 * @file ProfinetDataStructureTest.cpp
 * @brief Source file for class  ProfinetDataStructureTest
 * @date 14/01/2021
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
 * @details This source file contains the definition of all the methods for
 * the class ProfinetDataStructureTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "ProfinetDataSourceConstants.h"
#include "ProfinetDataStructure.h"
#include "ProfinetDataStructureTest.h"
#include "CompilerTypes.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
bool ProfinetDataStructureTest::TestConstructor() {
    using namespace ProfinetDataSourceDriver;
    SimpleLinkedList<MARTe::uint32> aLinkedList;
    return true;
}

bool ProfinetDataStructureTest::TestInsertHead() {
    using namespace ProfinetDataSourceDriver;
    SimpleLinkedList<MARTe::uint32> aLinkedList;
    MARTe::uint32 data = 128u;
    bool ok = true;
    if (ok) {
        ok = aLinkedList.InsertHead(data);
    }
    if (ok) {
        MARTe::uint32 value = aLinkedList.RemoveHead();
        ok = (data == value);
    }
    if (ok) {
        ok = !aLinkedList.LinkedListStatus();
    }
    return ok;
}

bool ProfinetDataStructureTest::TestInsertTail() {
    using namespace ProfinetDataSourceDriver;
    SimpleLinkedList<MARTe::uint32> aLinkedList;
    MARTe::uint32 data = 128u;
    bool ok = true;
    if (ok) {
        ok = aLinkedList.InsertTail(data);
    }
    if (ok) {
        MARTe::uint32 value = aLinkedList.RemoveTail();
        ok = (data == value);
    }
    if (ok) {
        ok = !aLinkedList.LinkedListStatus();
    }
    return ok;
}

bool ProfinetDataStructureTest::TestRemoveHead() {
    using namespace ProfinetDataSourceDriver;
    SimpleLinkedList<MARTe::uint32> aLinkedList;
    bool ok = true;
    if (ok) {
        ok = (aLinkedList.RemoveHead() == 0u);
    }
    return ok;
}

bool ProfinetDataStructureTest::TestRemoveTail() {
    using namespace ProfinetDataSourceDriver;
    SimpleLinkedList<MARTe::uint32> aLinkedList;
    bool ok = true;
    if (ok) {
        ok = (aLinkedList.RemoveTail() == 0u);
    }
    return ok;
}

bool ProfinetDataStructureTest::TestEmptyLinkedList() {
    using namespace ProfinetDataSourceDriver;
    SimpleLinkedList<MARTe::uint32> aLinkedList;
    MARTe::uint32 testData[] = {8,16,32};
    MARTe::uint32 testDataSize = 3;
    MARTe::uint32 out = 0;
    bool ok = true;
    if (ok) {
        for (MARTe::uint32 i = 0; i<testDataSize; i++) {
            if (ok) {
                ok = aLinkedList.InsertTail(testData[i]);
            }
        }
    }
    aLinkedList.EmptyLinkedList();
    if (ok) {
        ok = !aLinkedList.LinkedListStatus();
    }
    if (ok) {
        for (MARTe::uint32 i = 0; i<testDataSize; i++) {
            if (ok) {
                ok = aLinkedList.InsertHead(testData[i]);
            }
        }
    }
    aLinkedList.EmptyLinkedList();
    if (ok) {
        ok = !aLinkedList.LinkedListStatus();
    }
    return ok;
}

bool ProfinetDataStructureTest::TestLinkedListStatus() {
    using namespace ProfinetDataSourceDriver;
    SimpleLinkedList<MARTe::uint32> aLinkedList;
    MARTe::uint32 testData[] = {8,16,32};
    MARTe::uint32 testDataSize = 3;
    MARTe::uint32 out = 0;
    bool ok = true;
    if (ok) {
        for (MARTe::uint32 i = 0; i<testDataSize; i++) {
            if (ok) {
                ok = aLinkedList.InsertTail(testData[i]);
            }
        }
    }
    if (ok) {
        ok = aLinkedList.LinkedListStatus();
    }
    if (ok) {
        aLinkedList.EmptyLinkedList();
    }
    if (ok) {
        ok = !aLinkedList.LinkedListStatus();
    }
    return ok;
}

bool ProfinetDataStructureTest::TestGetIterator() {
   using namespace ProfinetDataSourceDriver;
    SimpleLinkedList<MARTe::uint32> aLinkedList;
    SimpleLinkedListIterator<MARTe::uint32> *aLinkedListIterator = NULL;
    MARTe::uint32 testData[] = {8,16,32};
    MARTe::uint32 testDataSize = 3;
    MARTe::uint32 out = 0;
    bool ok = true;
    if (ok) {
        for (MARTe::uint32 i = 0; i<testDataSize; i++) {
            if (ok) {
                ok = aLinkedList.InsertTail(testData[i]);
                aLinkedListIterator = aLinkedList.GetIterator();
                ok = (aLinkedListIterator != NULL);
            }
            if (ok){
                aLinkedListIterator->Last();
                out = aLinkedListIterator->Previous();
                ok = (testData[i] == out);
            }
            if (aLinkedListIterator != NULL) {
                delete aLinkedListIterator;
                aLinkedListIterator = NULL;
            }
        }
    }
    if (ok) {
        for (MARTe::uint32 i = 0; i<testDataSize; i++) {
            if (ok) {
                ok = aLinkedList.InsertHead(testData[i]);
                aLinkedListIterator = aLinkedList.GetIterator();
                ok = (aLinkedListIterator != NULL);
            }
            if (ok) {
                aLinkedListIterator->First();
                out = aLinkedListIterator->Next();
                ok = (testData[i] == out);
            }
            if(aLinkedListIterator != NULL){
                delete aLinkedListIterator;
                aLinkedListIterator = NULL;
            }
        }
    }
    aLinkedList.EmptyLinkedList();
    if (aLinkedListIterator != NULL) {
        delete aLinkedListIterator;
        aLinkedListIterator = NULL;
    }
    return ok;
}

bool ProfinetDataStructureTest::TestFirst() {
    using namespace ProfinetDataSourceDriver;
    SimpleLinkedList<MARTe::uint32> aLinkedList;
    SimpleLinkedListIterator<MARTe::uint32> *aLinkedListIterator = NULL;
    MARTe::uint32 testData[] = {8,16,32};
    MARTe::uint32 testDataSize = 3;
    MARTe::uint32 out = 0;
    bool ok = true;
    if (ok) {
        for (MARTe::uint32 i = 0; i<testDataSize; i++) {
            if (ok) {
                ok = aLinkedList.InsertTail(testData[i]);
            }
        }
    }
    if (ok) {
        aLinkedListIterator = aLinkedList.GetIterator();
        ok = (aLinkedListIterator != NULL);
    }
    if (ok) {
        aLinkedListIterator->First();
        out = aLinkedListIterator->Next();
        ok = (testData[0] == out);
    }
    aLinkedList.EmptyLinkedList();
    if (aLinkedListIterator != NULL) {
        delete aLinkedListIterator;
        aLinkedListIterator = NULL;
    }
    return ok;
}

bool ProfinetDataStructureTest::TestNext() {
    using namespace ProfinetDataSourceDriver;
    SimpleLinkedList<MARTe::uint32> aLinkedList;
    SimpleLinkedListIterator<MARTe::uint32> *aLinkedListIterator = NULL;
    MARTe::uint32 testData[] = {8,16,32};
    MARTe::uint32 testDataSize = 3;
    MARTe::uint32 out = 0;
    bool ok = true;
    if (ok) {
        for (MARTe::uint32 i = 0; i<testDataSize; i++) {
            if (ok) {
                ok = aLinkedList.InsertTail(testData[i]);
            }
        }
    }
    if (ok) {
        aLinkedListIterator = aLinkedList.GetIterator();
        ok = (aLinkedListIterator != NULL);
    }
    if (ok) {
        aLinkedListIterator->First();
        for (MARTe::uint32 i = 0; i<testDataSize; i++) {
            if (ok) {
                out = aLinkedListIterator->Next();
                ok = (testData[i] == out);
            }
        }
    }
    aLinkedList.EmptyLinkedList();
    if (aLinkedListIterator != NULL) {
        delete aLinkedListIterator;
        aLinkedListIterator = NULL;
    }
    return ok;
}

bool ProfinetDataStructureTest::TestLast() {
    using namespace ProfinetDataSourceDriver;
    SimpleLinkedList<MARTe::uint32> aLinkedList;
    SimpleLinkedListIterator<MARTe::uint32> *aLinkedListIterator = NULL;
    MARTe::uint32 testData[] = {8,16,32};
    MARTe::uint32 testDataSize = 3;
    MARTe::uint32 out = 0;
    bool ok = true;
    if (ok) {
        for (MARTe::uint32 i = 0; i<testDataSize; i++) {
            if (ok) {
                ok = aLinkedList.InsertHead(testData[i]);
            }
        }
    }
    if (ok) {
        aLinkedListIterator = aLinkedList.GetIterator();
        ok = (aLinkedListIterator != NULL);
    }
    if (ok) {
        aLinkedListIterator->Last();
        out = aLinkedListIterator->Next();
        ok = (testData[0] == out);
    }
    aLinkedList.EmptyLinkedList();
    if (aLinkedListIterator != NULL) {
        delete aLinkedListIterator;
        aLinkedListIterator = NULL;
    }
    return ok;
}

bool ProfinetDataStructureTest::TestPrevious() {
    using namespace ProfinetDataSourceDriver;
    SimpleLinkedList<MARTe::uint32> aLinkedList;
    SimpleLinkedListIterator<MARTe::uint32> *aLinkedListIterator = NULL;
    MARTe::uint32 testData[] = {8,16,32};
    MARTe::uint32 testDataSize = 3;
    MARTe::uint32 out = 0;
    bool ok = true;
    if (ok) {
        for (MARTe::uint32 i = 0; i<testDataSize; i++) {
            if (ok) {
                ok = aLinkedList.InsertHead(testData[i]);
            }
        }
    }
    if (ok) {
        aLinkedListIterator = aLinkedList.GetIterator();
        ok = (aLinkedListIterator != NULL);
    }
    if (ok) {
        aLinkedListIterator->Last();
        for (MARTe::uint32 i = 0; i<testDataSize; i++) {
            if (ok) {
                out = aLinkedListIterator->Previous();
                ok = (testData[i] == out);
            }
        }
    }
    aLinkedList.EmptyLinkedList();
    if (aLinkedListIterator != NULL) {
        delete aLinkedListIterator;
        aLinkedListIterator = NULL;
    }
    return ok;
}
