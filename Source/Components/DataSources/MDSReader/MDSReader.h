/**
 * @file MDSReader.h
 * @brief Header file for class MDSReader
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

 * @details This header file contains the declaration of the class MDSReader
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef DATASOURCES_MDSREADER_MDSREADER_H_
#define DATASOURCES_MDSREADER_MDSREADER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*lint -u__cplusplus This is required as otherwise lint will get confused after including this header file.*/
#include "mdsobjects.h"
/*lint -D__cplusplus*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "DataSourceI.h"
#include "MessageI.h"
#include "StreamString.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

class MDSReader:public DataSourceI, public MessageI {
//TODO Add the macro DLL_API to the class declaration (i.e. class DLL_API MDSReader)
public:
    CLASS_REGISTER_DECLARATION()

    MDSReader();
    virtual ~MDSReader();

    virtual bool Synchronise();

    /**
     * @brief Reads, checks and initialises the DataSource parameters
     * @details Load from a configuration file the DataSource parameters.
     * If no errors occurs the following operations are performed:
     * <ul>
     * <li>Reads tree name </li>
     * <li>Reads the shot number </li>
     * <li>Opens the tree with the shot number </li>
     * </ul>
     */
    virtual bool Initialise(StructuredDataI & data);

    /**
     * @brief Read, checks and initialises the Signals
     * @details If no errors occurs the following operations are performed:
     * <ul>
     * <li>Reads nodes name (could be 1 or several nodes)</li>
     * <li>Opens nodes</li>
     * <li>Gets the node type</li>
     * <li>Verifies the node type</li>
     * <li>Gets number of elements per node (or signal).
     * <li>Gets the the size of the type in bytes</li>
     * <li>Allocates memory
     * </ul>
     */
    virtual bool SetConfiguredDatabase(StructuredDataI & data);

    virtual bool PrepareNextState(const char8 * const currentStateName,
                const char8 * const nextStateName);

    virtual bool AllocateMemory();

    virtual uint32 GetNumberOfMemoryBuffers();

    virtual bool GetSignalMemoryBuffer(const uint32 signalIdx,
            const uint32 bufferIdx,
            void *&signalAddress);

    virtual const char8 *GetBrokerName(StructuredDataI &data,
            const SignalDirection direction);

    virtual bool GetInputBrokers(ReferenceContainer &inputBrokers,
            const char8* const functionName,
            void * const gamMemPtr);

    virtual bool GetOutputBrokers(ReferenceContainer &outputBrokers,
            const char8* const functionName,
            void * const gamMemPtr);
private:
    /**
     * @brief Open MDS tree
     * @details Open the treeName and copy the pointer of the object to tree variables.
     */
    bool OpenTree();

    /**
     * @brief Open MDS node.
     * @details Open MDS node called nodeNames[idx] and copy the object pointer to nodes[idx].
     * The order of the nodes is the other of the nodes names given in the configuration file.
     */
    bool OpenNode(uint32 idx);

    /**
     * @brief Gets the MDS type of the node [idx].
     */
    bool GetTypeNode(uint32 idx);

    /**
     * @brief validates the type of the node idx
     * @return true if the type is supported
     */
    bool IsValidTypeNode(uint32 idx);

    /**
     * @brief Gets the size of the type in bytes of the node idx.
     */
    void GetByteSize(uint32 idx);

    bool CheckTypeAgainstMdsNodeTypes(uint32 idx);

    TypeDescriptor ConvertMDStypeToMARTeType(StreamString mdsType);
    StreamString treeName;
    MDSplus::Tree *tree;
    StreamString *nodeName;
    MDSplus::TreeNode **nodes;
    uint32 numberOfNodeNames;
    /**
     * MDSplus signal type. If signalTypes is given as input, consistency between mdsSignalType signalType
     */
    StreamString *mdsNodeTypes;
    uint32 *byteSizeNodes;

    /**
     * In SetConfiguredDatabase() the information is modified. I.e the node name is not copied because is unknown parameter for MARTe.
     */
    ConfigurationDatabase originalSignalInformation;



    /**
     * Indicates the pulse number to open. If it is not specified -1 by default.
     */
    int32 shotNumber;

    /**
     * signal type expected to read. It is optional parameter on the configuration file. If exist it is check against signalType.
     */
    TypeDescriptor *type;

    uint32 *numberOfElements;

    char *dataSourceMemory;

    uint32 *offsets;

    /**
     *
     */


};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* DATASOURCES_MDSREADER_MDSREADER_H_ */

