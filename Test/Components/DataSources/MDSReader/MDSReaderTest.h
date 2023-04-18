/**
 * @file MDSReaderTest.h
 * @brief Header file for class MDSReaderTest
 * @date 22/09/2017
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

 * @details This header file contains the declaration of the class MDSReaderTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef MDSREADER_MDSREADERTEST_H_
#define MDSREADER_MDSREADERTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

#include "mdsobjects.h"

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "MDSReader.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

class MDSReaderTest {
public:
    MDSReaderTest();
    MDSReaderTest(MARTe::StreamString name,
                  MARTe::uint32 nElementsPerSeg,
                  MARTe::uint32 nSegments,
                  MARTe::float64 elapsetTimeSeg);

    /**
     * @brief Sets the environment variable test_tree_path and creates a tree for testing.
     */
    virtual ~MDSReaderTest();

    /**
     * @brief Test message errors of MDSReader::Initialise().
     */
    bool TestInitialiseNoTreeName();

    /**
     * @brief Test message errors of MDSReader::Initialise().
     */
    bool TestInitialiseNoShotNumber();

    /**
     * @brief Test message errors of MDSReader::Initialise().
     */
    bool TestInitialiseWrongShotNumber();

    /**
     * @brief Test message errors of MDSReader::Initialise().
     */
    bool TestInitialiseWrongTreeName();

    /**
     * @brief Test message errors of MDSReader::Initialise().
     */
    bool TestInitialiseUnexistingShotNumber();

    /**
     * @brief Test message errors of MDSReader::SetConfiguredDatabase().
     */
    bool TestInitialiseNoFrequency();

    /**
     * @brief Test message errors of MDSReader::SetConfiguredDatabase().
     */
    bool TestInitialiseNoSignals();

    /**
     * @brief MDSReader::SetConfiguredDatabase().
     */
    bool TestInitialise();

    /**
     * @brief Test message errors of MDSReader::SetConfiguredDatabase().
     */
    bool TestSetConfiguredDatabaseNoSignals();

    /**
     * @brief Test message errors of MDSReader::SetConfiguredDatabase().
     */
    bool TestSetConfiguredDatabase0Functions();

    /**
     * @brief Test message errors of MDSReader::SetConfiguredDatabase().
     */
    bool TestSetConfiguredDatabase2Functions();

    /**
     * @brief Test message errors of MDSReader::SetConfiguredDatabase().
     */
    bool TestSetConfiguredDatabase0Signals();

    /**
     * @brief Test message errors of MDSReader::SetConfiguredDatabase().
     */
    bool TestSetConfiguredDatabaseWrongSamples();

    /**
     * @brief Test message errors of MDSReader::SetConfiguredDatabase().
     */
    bool TestSetConfiguredDatabaseDiffSignalsAndFunctions();

    /**
     * @brief Test message errors of MDSReader::SetConfiguredDatabase().
     * @details The configuration file of MDSReader::Initialise() contains an empty Signals node
     */
    bool TestSetConfiguredDatabaseNoNodeName();

    /**
     * @brief Test message errors of MDSReader::SetConfiguredDatabase().
     */
    bool TestSetConfiguredDatabaseNoNodeName_2();

    /**
     * @brief Test message errors of MDSReader::SetConfiguredDatabase().
     */
    bool TestSetConfiguredDatabaseInvalidNodeName();

    /**
     * @brief Test message errors of MDSReader::SetConfiguredDatabase().
     */
    bool TestSetConfiguredDatabaseEqualNodeName();

    /**
     * @brief Test message errors of MDSReader::SetConfiguredDatabase().
     */
    bool TestSetConfiguredDatabaseInvalidNodeType();

    /**
     * @brief Test message errors of MDSReader::SetConfiguredDatabase().
     */
    bool TestSetConfiguredDatabaseWrongType();

    /**
     * @brief Test message errors of MDSReader::SetConfiguredDatabase().
     * @details the node type and the type specified in the configuration file are different.
     */
    bool TestSetConfiguredDatabaseInconsistentType();

    /**
     * @brief Test message errors of MDSReader::SetConfiguredDatabase().
     */
    bool TestSetConfiguredDatabaseNoNumberOfElements();

    /**
     * @brief Test message errors of MDSReader::SetConfiguredDatabase().
     */
    bool TestSetConfiguredDatabaseInvalidNumberOfElements();

    /**
     * @brief Test message errors of MDSReader::SetConfiguredDatabase().
     */
    bool TestSetConfiguredDatabaseNoTimeNumberOfElements();

    /**
     * @brief Test message errors of MDSReader::SetConfiguredDatabase().
     */
    bool TestSetConfiguredDatabaseInvalidTimeNumberOfElements();

    /**
     * @brief Test message errors of MDSReader::SetConfiguredDatabase().
     */
    bool TestSetConfiguredDatabaseInvalidBytesSize();

    /**
     * @brief Test message errors of MDSReader::SetConfiguredDatabase().
     */
    bool TestSetConfiguredDatabaseInvalidTimeBytesSize();

    /**
     * @brief Test message errors of MDSReader::SetConfiguredDatabase().
     */
    bool TestSetConfiguredDatabaseNoDataManagement();

    /**
     * @brief Test message errors of MDSReader::SetConfiguredDatabase().
     */
    bool TestSetConfiguredDatabaseInvalidDataManagement();

    /**
     * @brief Test message errors of MDSReader::SetConfiguredDatabase().
     */
    bool TestSetConfiguredDatabaseDataManagement0();

    /**
     * @brief Test message errors of MDSReader::SetConfiguredDatabase().
     */
    bool TestSetConfiguredDatabaseNoHoleManagement();

    /**
     * @brief Test message errors of MDSReader::SetConfiguredDatabase().
     */
    bool TestSetConfiguredDatabaseInvalidHoleManagement();

    /**
     * @brief Test message errors of MDSReader::SetConfiguredDatabase().
     */
    bool TestSetConfiguredDatabaseInvalidTimeType();

    /**
     * @brief Test message errors of MDSReader::SetConfiguredDatabase().
     * @details 1 segment with only one element per segment-> the sampling time cannot be calculated.
     */
    bool TestSetConfiguredDatabaseInvalidSamplingTime();

    /**
     * @brief Test MDSReader::SetConfiguredDatabase().
     */
    bool TestSetConfiguredDatabase();
    /**
     * @brief Test MDSReader::SetConfiguredDatabase().
     * @details shot number = -1.
     */
    bool TestSetConfiguredDatabaseShotNumber();

    /**
     * @brief Test MDSReader::PrepareNextState().
     */
    bool TestPrepareNextState();

    /**
     * @brief Test MDSReader::AllocateMemory().
     */
    bool TestAllocateMemory();

    /**
     * @brief Test MDSReader::GetBrokerName().
     */
    bool TestGetBrokerName();

    /**
     * @brief Test MDSReader::GetInputBroker().
     */
    bool TestGetInputBroker();

    /**
     * @brief Test MDSReader::GetOutputBroker().
     */
    bool TestGetOutputBroker();

    /**
     * @brief Test MDSReader::GetNumberOfMemoryBuffers().
     */
    bool TestGetNumberOfMemoryBuffers();

    /**
     * @brief Test message errors of MDSReader::GetSignalMemoryBuffer().
     */
    bool TestGetSignalMemoryBufferNoMemory();

    /**
     * @brief Test MDSReader::GetSignalMemoryBuffer().
     */
    bool TestGetSignalMemoryBuffer();

    /**
     * @brief Test Synchronise and compare the output against the expected values
     * @details Each MARTe cycle reads half of a segment node.
     * dataManagement[i] = 0
     * hole management[i] = 0 (irrelevant no hole on the data)
     */
    bool TestSynchronise();

    /**
     * @brief Test Synchronise and compare the output against the expected values
     * @details Each MARTe cycle reads one segment.
     * dataManagement[i] = 0
     * hole management[i] = 0 (irrelevant no hole on the data)
     */
    bool TestSynchronise1();

    /**
     * @brief Test Synchronise and compare the output against the expected values
     * @details Each MARTe cycle reads two segments.
     * dataManagement[i] = 0
     * hole management[i] = 0 (irrelevant no hole on the data)
     */
    bool TestSynchronise2();

    /**
     * @brief Test Synchronise and compare the output against the expected values
     * @details Each MARTe cycle reads 3/4 segment.
     * dataManagement[i] = 0
     * hole management[i] = 0 (irrelevant no hole on the data)
     */
    bool TestSynchronise3();

    /**
     * @brief Test Synchronise and compare the output against the expected values
     * @details node sampling time 0.1. Read sampling time = 0.2, elementsRead = 10 (In one cycle whole segment is read)
     * dataManagement[i] = 1 (linear interpolation)
     * hole management[i] = 0 (irrelevant no hole on the data)
     */
    bool TestSynchronise4();

    /**
     * @brief Test Synchronise and compare the output against the expected values
     * @details node sampling time = 0.1. Read sampling time = 0.1 elementsRead = 10 (In two cycle whole segment is read)
     * dataManagement[i] = 1 (linear interpolation)
     * hole management[i] = 0 (irrelevant no hole on the data)
     */
    bool TestSynchronise5();

    /**
     * @brief Test Synchronise and compare the output against the expected values
     * @details node sampling time = 0.1. Read sampling time = 0.08 elementsRead = 10
     * dataManagement[i] = 1 (linear interpolation)
     * hole management[i] = 0 (irrelevant no hole on the data)
     */
    bool TestSynchronise6();

    /**
     * @brief Test Synchronise and compare the output against the expected values
     * @details node sampling time = 0.1. Read sampling time = 0.08 elementsRead = 32
     * dataManagement[i] = 1 (linear interpolation)
     * hole management[i] = 0 (irrelevant no hole on the data)
     */
    bool TestSynchronise7();

    /**
     * @brief Test Synchronise and compare the output against the expected values
     * @details node sampling time 0.1. Read sampling time = 0.2, elementsRead = 10
     * dataManagement[i] = 2 (hold last value)
     * holeManagement[i] = 0 (irrelevant no hole on the data)
     */
    bool TestSynchronise8();

    /**
     * @brief Test Synchronise and compare the output against the expected values
     * @details node sampling time 0.1. Read sampling time = 0.1, elementsRead = 10
     * dataManagement[i] = 2 (hold last value)
     * holeManagement[i] = 0 (irrelevant no hole on the data)
     */
    bool TestSynchronise9();

    /**
     * @brief Test Synchronise and compare the output against the expected values
     * @details node sampling time 0.1. Read sampling time = 0.1, elementsRead = 32
     * dataManagement[i] = 2 (hold last value)
     * holeManagement[i] = 0 (irrelevant no hole on the data)
     */
    bool TestSynchronise10();

    /**
     * @brief Test Synchronise and compare the output against the expected values
     * @details node sampling time 0.1. Read sampling time = 0.1, elementsRead = 15
     * shotNumber = 2
     * dataManagement[i] = 0
     * holeManagement[i] = 0
     */
    bool TestSynchronise11();

    /**
     * @brief Test Synchronise and compare the output against the expected values
     * @details node sampling time 0.1. Read sampling time = 0.1, elementsRead = 40
     * shotNumber = 2
     * dataManagement[i] = 0
     * holeManagement[i] = 0
     */
    bool TestSynchronise11_1();

    /**
     * @brief Test Synchronise and compare the output against the expected values
     * @details node sampling time 0.1. Read sampling time = 0.1, elementsRead = 40
     * shotNumber = 2
     * dataManagement[i] = 1
     * holeManagement[i] = 0
     */
    bool TestSynchronise11_2();

    /**
     * @brief Test Synchronise and compare the output against the expected values
     * @details node sampling time 0.1. Read sampling time = 0.1, elementsRead = 40
     * shotNumber = 2
     * dataManagement[i] = 2
     * holeManagement[i] = 0
     */
    bool TestSynchronise11_3();

    /**
     * @brief Test Synchronise and compare the output against the expected values
     * @details node sampling time 0.1. Read sampling time = 0.1, elementsRead = 40
     * shotNumber = 2
     * dataManagement[i] = 1
     * holeManagement[i] = 1
     */
    bool TestSynchronise11_4();

    /**
     * @brief Test Synchronise and compare the output against the expected values
     * @details node sampling time 0.1. Read sampling time = 0.1, elementsRead = 40
     * shotNumber = 2
     * dataManagement[i] = 2
     * holeManagement[i] = 1
     */
    bool TestSynchronise11_5();

    /**
     * @brief Test Synchronise and compare the output against the expected values
     * @details node sampling time 0.1. Read sampling time = 0.1, elementsRead = 25
     * shotNumber = 2
     * dataManagement[i] = 0
     * holeManagement[i] = 0
     */
    bool TestSynchronise12();

    /**
     * @brief Test Synchronise and compare the output against the expected values
     * @details node sampling time 0.1. Read sampling time = 0.1, elementsRead = 5
     * shotNumber = 2
     * dataManagement[i] = 0
     * holeManagement[i] = 0
     */
    bool TestSynchronise13();

    /**
     * @brief Test Synchronise and compare the output against the expected values
     * @details node sampling time 0.1. Read sampling time = 0.1, elementsRead = 15
     * shotNumber = 2
     * dataManagement[i] = 0
     * holeManagement[i] = 1
     */
    bool TestSynchronise14();

    /**
     * @brief Test Synchronise and compare the output against the expected values
     * @details node sampling time 0.1. Read sampling time = 0.1, elementsRead = 25
     * shotNumber = 2
     * dataManagement[i] = 0
     * holeManagement[i] = 1
     */
    bool TestSynchronise15();

    /**
     * @brief Test Synchronise and compare the output against the expected values
     * @details node sampling time 0.1. Read sampling time = 0.1, elementsRead = 5
     * shotNumber = 2
     * dataManagement[i] = 0
     * holeManagement[i] = 1
     */
    bool TestSynchronise16();

    /**
     * @brief Test Synchronise and compare the output against the expected values
     * @details node sampling time 0.1. Read sampling time = 0.1, elementsRead = 15
     * shotNumber = 2
     * dataManagement[i] = 1
     * holeManagement[i] = 0
     */
    bool TestSynchronise17();

    /**
     * @brief Test Synchronise and compare the output against the expected values
     * @details node sampling time 0.1. Read sampling time = 0.1, elementsRead = 25
     * shotNumber = 2
     * dataManagement[i] = 1
     * holeManagement[i] = 0
     */
    bool TestSynchronise18();

    /**
     * @brief Test Synchronise and compare the output against the expected values
     * @details node sampling time 0.1. Read sampling time = 0.1, elementsRead = 5
     * shotNumber = 2
     * dataManagement[i] = 1
     * holeManagement[i] = 0
     */
    bool TestSynchronise19();

    /**
     * @brief Test Synchronise and compare the output against the expected values
     * @details node sampling time 0.1. Read sampling time = 0.1, elementsRead = 15
     * shotNumber = 2
     * dataManagement[i] = 1
     * holeManagement[i] = 1
     */
    bool TestSynchronise20();

    /**
     * @brief Test Synchronise and compare the output against the expected values
     * @details node sampling time 0.1. Read sampling time = 0.1, elementsRead = 25
     * shotNumber = 2
     * dataManagement[i] = 1
     * holeManagement[i] = 1
     */
    bool TestSynchronise21();

    /**
     * @brief Test Synchronise and compare the output against the expected values
     * @details node sampling time 0.1. Read sampling time = 0.1, elementsRead = 5
     * shotNumber = 2
     * dataManagement[i] = 1
     * holeManagement[i] = 1
     */
    bool TestSynchronise22();

    /**
     * @brief Test Synchronise and compare the output against the expected values
     * @details node sampling time 0.1. Read sampling time = 0.1, elementsRead = 15
     * shotNumber = 2
     * dataManagement[i] = 2
     * holeManagement[i] = 0
     */
    bool TestSynchronise23();

    /**
     * @brief Test Synchronise and compare the output against the expected values
     * @details node sampling time 0.1. Read sampling time = 0.1, elementsRead = 25
     * shotNumber = 2
     * dataManagement[i] = 2
     * holeManagement[i] = 0
     */
    bool TestSynchronise24();

    /**
     * @brief Test Synchronise and compare the output against the expected values
     * @details node sampling time 0.1. Read sampling time = 0.1, elementsRead = 5
     * shotNumber = 2
     * dataManagement[i] = 2
     * holeManagement[i] = 0
     */
    bool TestSynchronise25();

    /**
     * @brief Test Synchronise and compare the output against the expected values
     * @details node sampling time 0.1. Read sampling time = 0.1, elementsRead = 15
     * shotNumber = 2
     * dataManagement[i] = 2
     * holeManagement[i] = 1
     */
    bool TestSynchronise26();

    /**
     * @brief Test Synchronise and compare the output against the expected values
     * @details node sampling time 0.1. Read sampling time = 0.1, elementsRead = 25
     * shotNumber = 2
     * dataManagement[i] = 2
     * holeManagement[i] = 1
     */
    bool TestSynchronise27();

    /**
     * @brief Test Synchronise and compare the output against the expected values
     * @details node sampling time 0.1. Read sampling time = 0.1, elementsRead = 5
     * shotNumber = 2
     * dataManagement[i] = 2
     * holeManagement[i] = 1
     */
    bool TestSynchronise28();

    /**
     * @brief Test Synchronise and compare the output against the expected values
     * @details node sampling time 0.1. Read sampling time = 0.1, elementsRead = 15
     * shotNumber = 3
     * dataManagement[i] = 0
     * holeManagement[i] = 0
     */
    bool TestSynchronise29();

    /**
     * @brief Test Synchronise and compare the output against the expected values
     * @details node sampling time 0.1. Read sampling time = 0.1, elementsRead = 25
     * shotNumber = 3
     * dataManagement[i] = 0
     * holeManagement[i] = 0
     */
    bool TestSynchronise30();

    /**
     * @brief Test Synchronise and compare the output against the expected values
     * @details node sampling time 0.1. Read sampling time = 0.1, elementsRead = 15
     * shotNumber = 3
     * dataManagement[i] = 0
     * holeManagement[i] = 1
     */
    bool TestSynchronise31();

    /**
     * @brief Test Synchronise and compare the output against the expected values
     * @details node sampling time 0.1. Read sampling time = 0.1, elementsRead = 25
     * shotNumber = 3
     * dataManagement[i] = 0
     * holeManagement[i] = 1
     */
    bool TestSynchronise32();

    /**
     * @brief Test Synchronise and compare the output against the expected values
     * @details node sampling time 0.1. Read sampling time = 0.1, elementsRead = 15
     * shotNumber = 3
     * dataManagement[i] = 1
     * holeManagement[i] = 0
     */
    bool TestSynchronise33();

    /**
     * @brief Test Synchronise and compare the output against the expected values
     * @details node sampling time 0.1. Read sampling time = 0.1, elementsRead = 25
     * shotNumber = 3
     * dataManagement[i] = 1
     * holeManagement[i] = 0
     */
    bool TestSynchronise34();

    /**
     * @brief Test Synchronise and compare the output against the expected values
     * @details node sampling time 0.1. Read sampling time = 0.1, elementsRead = 15
     * shotNumber = 3
     * dataManagement[i] = 1
     * holeManagement[i] = 1
     */
    bool TestSynchronise35();

    /**
     * @brief Test Synchronise and compare the output against the expected values
     * @details node sampling time 0.1. Read sampling time = 0.1, elementsRead = 25
     * shotNumber = 3
     * dataManagement[i] = 1
     * holeManagement[i] = 1
     */
    bool TestSynchronise36();

    /**
     * @brief Test Synchronise and compare the output against the expected values
     * @details node sampling time 0.1. Read sampling time = 0.1, elementsRead = 15
     * shotNumber = 3
     * dataManagement[i] = 2
     * holeManagement[i] = 0
     */
    bool TestSynchronise37();

    /**
     * @brief Test Synchronise and compare the output against the expected values
     * @details node sampling time 0.1. Read sampling time = 0.1, elementsRead = 25
     * shotNumber = 3
     * dataManagement[i] = 2
     * holeManagement[i] = 0
     */
    bool TestSynchronise38();

    /**
     * @brief Test Synchronise and compare the output against the expected values
     * @details node sampling time 0.1. Read sampling time = 0.1, elementsRead = 15
     * shotNumber = 3
     * dataManagement[i] = 2
     * holeManagement[i] = 1
     */
    bool TestSynchronise39();

    /**
     * @brief Test Synchronise and compare the output against the expected values
     * @details node sampling time 0.1. Read sampling time = 0.1, elementsRead = 25
     * shotNumber = 3
     * dataManagement[i] = 2
     * holeManagement[i] = 1
     */
    bool TestSynchronise40();

    /**
     * @brief Test Synchronise and compare the output against the expected values
     * @details node sampling time 0.1. Read sampling time = 0.1, elementsRead = 15
     * shotNumber = 4
     * dataManagement[i] = 0
     * holeManagement[i] = 0
     */
    bool TestSynchronise41();

    /**
     * @brief Test Synchronise and compare the output against the expected values
     * @details node sampling time 0.1. Read sampling time = 0.1, elementsRead = 100;
     * shotNumber = 2
     * dataManagement[i] = 0
     * holeManagement[i] = 0
     */
    bool TestSynchronise42();

    /**
     * @brief Test Synchronise and compare the output against the expected values
     * @details node sampling time 0.1. Read sampling time = 0.1, elementsRead = 100;
     * shotNumber = 2
     * dataManagement[i] = 1
     * holeManagement[i] = 0
     */
    bool TestSynchronise43();

    /**
     * @brief Test Synchronise and compare the output against the expected values
     * @details node sampling time 0.1. Read sampling time = 0.1, elementsRead = 100;
     * shotNumber = 2
     * dataManagement[i] = 2
     * holeManagement[i] = 0
     */
    bool TestSynchronise44();

    /**
     * @brief Test Synchronise and compare the output against the expected values
     * @details node sampling time 0.1. Read sampling time = 0.1, elementsRead = 100;
     * shotNumber = 2
     * dataManagement[i] = 0
     * holeManagement[i] = 1
     */
    bool TestSynchronise45();

    /**
     * @brief Test Synchronise and compare the output against the expected values
     * @details node sampling time 0.1. Read sampling time = 0.1, elementsRead = 100;
     * shotNumber = 2
     * dataManagement[i] = 1
     * holeManagement[i] = 1
     */
    bool TestSynchronise46();

    /**
     * @brief Test Synchronise and compare the output against the expected values
     * @details node sampling time 0.1. Read sampling time = 0.1, elementsRead = 100;
     * shotNumber = 2
     * dataManagement[i] = 2
     * holeManagement[i] = 1
     */
    bool TestSynchronise47();

    /**
     * @brief Test Synchronise and compare the output against the expected values
     * @details node sampling time 0.1. Read sampling time = 0.1, elementsRead = 100;
     * shotNumber = 3
     * dataManagement[i] = 0
     * holeManagement[i] = 0
     */
    bool TestSynchronise48();

    /**
     * @brief Test Synchronise and compare the output against the expected values
     * @details node sampling time 0.1. Read sampling time = 0.1, elementsRead = 100;
     * shotNumber = 3
     * dataManagement[i] = 1
     * holeManagement[i] = 0
     */
    bool TestSynchronise49();

    /**
     * @brief Test Synchronise and compare the output against the expected values
     * @details node sampling time 0.1. Read sampling time = 0.1, elementsRead = 100;
     * shotNumber = 3
     * dataManagement[i] = 2
     * holeManagement[i] = 0
     */
    bool TestSynchronise50();

    /**
     * @brief Test Synchronise and compare the output against the expected values
     * @details node sampling time 0.1. Read sampling time = 0.1, elementsRead = 100;
     * shotNumber = 3
     * dataManagement[i] = 0
     * holeManagement[i] = 1
     */
    bool TestSynchronise51();

    /**
     * @brief Test Synchronise and compare the output against the expected values
     * @details node sampling time 0.1. Read sampling time = 0.1, elementsRead = 100;
     * shotNumber = 3
     * dataManagement[i] = 1
     * holeManagement[i] = 1
     */
    bool TestSynchronise52();

    /**
     * @brief Test Synchronise and compare the output against the expected values
     * @details node sampling time 0.1. Read sampling time = 0.1, elementsRead = 100;
     * shotNumber = 3
     * dataManagement[i] = 2
     * holeManagement[i] = 1
     */
    bool TestSynchronise53();

    /**
     * @brief Test Synchronise and compare the output against the expected values
     * @details node sampling time 0.1. Read sampling time = 0.1, elementsRead = 100;
     * shotNumber = 3
     * dataManagement[i] = 2
     * holeManagement[i] = 0
     */
    bool TestSynchronise54();

    /**
     * @brief Test Synchronise and compare the output against the expected values
     * @details node sampling time 0.1. Read sampling time = 0.1, elementsRead = 10;
     * Read all data of the tree
     * shotNumber = 1
     * dataManagement[i] = 0
     * hole management[i] = 0 (irrelevant no hole on the data)
     */
    bool TestSynchronise55();

    /**
     * @brief Test Synchronise and compare the output against the expected values
     * @details node sampling time 0.1. Read sampling time = 0.1, elementsRead = 10;
     * Read all data of the tree
     * shotNumber = 2
     * dataManagement[i] = 1
     * hole management[i] = 0 (irrelevant no hole on the data)
     */
    bool TestSynchronise56();

    /**
     * @brief Test Synchronise and compare the time against the expected value.
     * @details node sampling time 0.1. Read sampling time = 0.1, elementsRead = 10;
     * time type = uint32.
     * Read all data of the tree
     * shotNumber = 2
     * dataManagement[i] = 1
     * hole management[i] = 0 (irrelevant no hole on the data)
     */
    bool TestSynchronise57();

    /**
     * @brief Test Synchronise and compare the time against the expected value.
     * @details node sampling time 0.1. Read sampling time = 0.1, elementsRead = 10;
     * time type = int32.
     * Read all data of the tree
     * shotNumber = 2
     * dataManagement[i] = 1
     * hole management[i] = 0 (irrelevant no hole on the data)
     */
    bool TestSynchronise58();

    /**
     * @brief Test Synchronise and compare the time against the expected value.
     * @details node sampling time 0.1. Read sampling time = 0.1, elementsRead = 10;
     * time type = uint64.
     * Read all data of the tree
     * shotNumber = 2
     * dataManagement[i] = 1
     * hole management[i] = 0 (irrelevant no hole on the data)
     */
    bool TestSynchronise59();

    /**
     * @brief Test Synchronise and compare the time against the expected value.
     * @details node sampling time 0.1. Read sampling time = 0.1, elementsRead = 10;
     * time type = int64.
     * Read all data of the tree
     * shotNumber = 2
     * dataManagement[i] = 1
     * hole management[i] = 0 (irrelevant no hole on the data)
     */
    bool TestSynchronise60();

    /**
     * @brief Test Synchronise and compare the output against the expected values
     * @details node sampling time 0.1. Read sampling time = 21, elementsRead = 10;
     * time type = int64.
     * Single element per node.
     * shotNumber = 1
     * dataManagement[i] = 0
     * hole management[i] = 0 (irrelevant no hole on the data)
     */
    bool TestSynchronise61();

    /**
     * @brief Test Synchronise and compare the output against the expected values
     * @details node sampling time 0.1. Read sampling time = 0.1, elementsRead = 10;
     * time type = int64.
     * Single element per node.
     * shotNumber = 1
     * dataManagement[i] = 1
     * hole management[i] = 0 (irrelevant no hole on the data)
     */
    bool TestSynchronise62();

    /**
     * @brief Test Synchronise and compare the output against the expected values
     * @details node sampling time 0.1. Read sampling time = 2, elementsRead = 10;
     * time type = int64.
     * Single element per node.
     * shotNumber = 1
     * dataManagement[i] = 2
     * hole management[i] = 0 (irrelevant no hole on the data)
     */
    bool TestSynchronise63();

    /**
     * @brief Test Synchronise and compare the output against the expected values
     * @details node sampling time 0.1. Read sampling time = 0.1, elementsRead = 42;
     * time type = int64.
     * shotNumber = 2
     * dataManagement[i] = 0
     * hole management[i] = 0
     */
    bool TestSynchronise64();

    /**
     * @brief Test Synchronise and compare the output against the expected values
     * @details node sampling time 0.1. Read sampling time = 0.1, elementsRead = 42;
     * time type = int64.
     * shotNumber = 3
     * dataManagement[i] = 0
     * hole management[i] = 0
     */
    bool TestSynchronise65();

    /**
     * @brief Test Synchronise and compare the output against the expected values
     * @details node sampling time 0.1. Read sampling time = 0.02, elementsRead = 40;
     * shotNumber = 1
     * dataManagement[i] = 2
     * hole management[i] = 0 (irrelevant no hole on the data)
     */
    bool TestSynchronise66();

    /**
     * @brief Test Synchronise and compare the output against the expected values
     * @details node sampling time 0.1. Read sampling time = 0.02, elementsRead = 40;
     * shotNumber = 1
     * dataManagement[i] = 1
     * hole management[i] = 0 (irrelevant no hole on the data)
     */
    bool TestSynchronise67();

    /**
     * @brief Test Synchronise recursively
     * @details node sampling time 0.1. Read sampling time = 0.02, elementsRead = 12800;
     * shotNumber = 1
     * dataManagement[i] = 0
     * hole management[i] = 0 (irrelevant no hole on the data)
     */
    bool TestSynchronise68(uint32 nOfSegments);

private:
    StreamString treeName;
    StreamString fullPath;

    /**
     * List of node names which are supported by the MDSRead
     */
    StreamString *allValidNodeNames;

    uint32 numberOfValidNodes;

    /**
     * list of nodes names used in the tests
     */
    StreamString *allNodeNames;

    uint32 numberOfNodes;

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* MDSREADER_MDSREADERTEST_H_ */

