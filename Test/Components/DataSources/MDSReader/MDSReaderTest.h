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
//TODO Add the macro DLL_API to the class declaration (i.e. class DLL_API MDSReaderTest)
public:
    MDSReaderTest();

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
    bool TestInitialiseNoSignals();

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
      */
    bool TestSetConfiguredDatabaseNoNumberOfElements();

    /**
      * @brief Test message errors of MDSReader::SetConfiguredDatabase().
      */
    bool TestSetConfiguredDatabaseInvalidNumberOfElements();


    /**
     * @brief Test message errors of MDSReader::Initialise().
     * @details the node specified is not a vector
     */
    //bool TestInitialiseWrongNodeNames();

    /**
     * @brief Test message errors of MDSReader::Initialise().
     * @brief the node name specified is a number
     */
    //bool TestInitialiseWrongNodeNames2();

    /**
     * @brief Test message errors of MDSReader::Initialise().
     * @brief the node name specified is not a node of the tree.
     */
    //bool TestInitialiseWrongNodeNames3();

    /**
     * @brief Test message errors of MDSReader::Initialise().
     */
    //bool TestInitialiseInvalidTypeNode();

    /**
     * @brief Test message errors of MDSReader::Initialise().
     * @details number of NodeNames different from the number of SignalTypes
     */
    //bool TestInitialiseInvalidTypeSpecified();

    /**
     * @brief Test message errors of MDSReader::Initialise().
     * @details SignalTypes different from the node types.
     */
    //bool TestInitialiseInvalidTypeSpecified2();


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

