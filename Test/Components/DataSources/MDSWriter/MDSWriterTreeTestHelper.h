/**
 * @file MDSWriterTreeTestHelper.h
 * @brief Header file for class MDSWriterTreeTestHelper
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

 * @details This header file contains the declaration of the class MDSWriterTreeTestHelper
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef TEST_COMPONENTS_DATASOURCES_MDSWRITER_MDSWRITERTREETESTHELPER_H_
#define TEST_COMPONENTS_DATASOURCES_MDSWRITER_MDSWRITERTREETESTHELPER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "StreamString.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * Class which creates and deletes the MDS+ tree for the tests.
 */
class MDSWriterTreeTestHelper {
public:
    /**
     * @brief Constructor. NOOP.
     */
    MDSWriterTreeTestHelper();

    /**
     * @brief Destructor. NOOP.
     */
    ~MDSWriterTreeTestHelper();

    /**
     * @brief Creates the tree.
     * @param[in] treeNameIn the name of the tree to create.
     * @returns true if the tree was successfully created
     */
    void Create(MARTe::StreamString treeNameIn);

    /**
     * @brief Deletes a tree previously created.
     */
    void Destroy();

private:
    /**
     * The name of the tree
     */
    MARTe::StreamString treeName;

    /**
     * The tree full path
     */
    MARTe::StreamString fullPath;
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* TEST_COMPONENTS_DATASOURCES_MDSWRITER_MDSWRITERTREETESTHELPER_H_ */
