/**
 * @file ProfinetDataStructureTest.h
 * @brief Header file for class ProfinetDataStructureTest
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
 * @details This header file contains the declaration of the class ProfinetDataStructureTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef DATASOURCES_PROFINET_PROFINETDATASTRUCTURETEST_H_
#define DATASOURCES_PROFINET_PROFINETDATASTRUCTURETEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief Tests the ProfinetDataStructure public methods.
 */
class ProfinetDataStructureTest {
public:

    /**
     * @brief Tests the Constructor method.
     */
    bool TestConstructor();

    /**
     * @brief Tests the InsertHead method.
     */
    bool TestInsertHead();

    /**
     * @brief Tests the InsertTail method.
     */
    bool TestInsertTail();

    /**
     * @brief Tests the RemoveHead method.
     */
    bool TestRemoveHead();

    /**
     * @brief Tests the RemoveTail method.
     */
    bool TestRemoveTail();

    /**
     * @brief Tests the EmptyLinkedList method.
     */
    bool TestEmptyLinkedList();

    /**
     * @brief Tests the LinkedListStatus method.
     */
    bool TestLinkedListStatus();

    /**
     * @brief Tests the GetIterator method.
     */
    bool TestGetIterator();

    /**
     * @brief Tests the First method.
     */
    bool TestFirst();

    /**
     * @brief Tests the Next method.
     */
    bool TestNext();

    /**
     * @brief Tests the Last method.
     */
    bool TestLast();

    /**
     * @brief Tests the Previous method.
     */
    bool TestPrevious();
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* DATASOURCES_PROFINET_PROFINETDATASTRUCTURETEST_H_ */
