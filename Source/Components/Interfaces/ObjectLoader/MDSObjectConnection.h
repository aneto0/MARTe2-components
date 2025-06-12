/**
 * @file MDSObjectConnection.h
 * @brief Header file for class MDSObjectConnection
 * @date 16/09/2024
 * @author Cristian Galperti
 * @author Nicolo Ferron
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
 * @details This header file contains the declaration of the class MDSObjectConnection
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef MDSOBJECTCONNECTION_H_
#define MDSOBJECTCONNECTION_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "ConfigurationDatabase.h"
#include "ObjectConnectionI.h"
#include "ReferenceContainer.h"

#include "mdsdescrip.h"
#include "mdsobjects.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {

enum MDSClientType {
    ThinClient,
    DistributedClient,
    InvalidClient
};

/**
 * @brief Loads MARTe::AnyObject from MDSplus trees
 * @details This class is a StaticList<AnyType*>. It populates itself
 *          with the information declared in the `Parameters` node
 *          of its configuration (see below).
 *
 * The StaticList<AnyType*> is meant to be read and serialised by a
 * MARTe::ObjectLoader class that makes the parameters available
 * to any component of the RealTimeApplication.
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * The configuration syntax is (names and signal quantity are only given
 * as an example):
 *
 * +ParamLoader1 = {
 *     Class = ObjectLoader                         // Compulsory
 *     Shot = "-1"                                  // Optional if defined in MDSObjectConnection
 *
 *     // All parameters declared here (except "Class") are also copied inside the Connections.
 *
 *     +MDSConnection1 = {
 *         Class = MDSObjectConnection              // Compulsory
 *         ClientType = ( "Thin" | "Distributed" )  // Optional. Default: "Thin"
 *         Server = "localhost:8000"                // Compulsory if ClientType = "Thin"
 *         Tree = "tree_name"                       // Compulsory (unless already defined in ObjectLoader)
 *         Shot = "-1"                              // Compulsory (unless already defined in ObjectLoader)
 *
 *         // Parameters can be declared in a structured fashion:
 *         gain1 = {
 *             Path  = "\\TREE_NAME::TOP:NODE1"                     // Compulsory
 *         }
 *         gainStruct1 = {
 *             gain_a = { Path  = "\\TREE_NAME::TOP:NODE2" }        // Compulsory
 *             gain_b = { Path  = "\\TREE_NAME::TOP:NODE3" }        // Compulsory
 *         }
 *
 *         // Parameters can also be declared in a flattened fashion:
 *         gainStruct2.gain_one = { Path  = "\\TREE_NAME::TOP:NODE4" }
 *         gainStruct2.gain_two = { Path  = "\\TREE_NAME::TOP:NODE5" }
 *
 *         // Both dots `.` and dashes `-` can be used as separatos:
 *         gainStruct3-gain_1 = { Path  = "\\TREE_NAME::TOP:NODE6" }
 *         gainStruct3-gain_2 = { Path  = "\\TREE_NAME::TOP:NODE7" }
 *     }
 *
 *     +MDSConnection2 = {
 *         ...
 *     }
 * }
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * @note Structured nodes can be declared in a structured fashion or in
 *       a flattened fashion. When flattened, both dots (`.`) and dashes
 *       (`-`) can be used as separators.
 *
 * @warning Do not use `-` character for node names except for denoting structures
 *          (see note above).
 *
 * Details of the configuration syntax:
 * - **MDSObjConnection**
 *    - *Shot*: Number of the shot of the MDS trees to be used globally in all the trees. If
 *          this parameter is set, Shot parameter in MDSObjConnection overwrites the value for
 *          that connection only. This parameter is optional if it is defined in every MDSObjConnection.
 *    - *ClientType*: Optional. Its valid values are "Thin" or "Distributed". If not set, "Thin" is
 *          used by default. Read more about remote accessing to MDS+ for understanding this parameter.
 *    - *Server*: Only required if ClientType = "Thin". It is a string which defines the IP address
 *          of the remote MDSplus server, and its access port. Instead of IP address, domains can be used,
 *          as well as "localhost" if the server is in the same machine. If ClientType = "Distributed",
 *          an environment variable must be set in your machine, with the form: "treename_path=<IP>:<PORT>::<PATH>".
 *    - *Tree*: name of the tree to connect to.
 *    - *Shot*: Number of the shot of the MDS tree. It is optional if it was defined in MDSObjLoader.
 *          If set here, it overwrites the value in MDSObjLoader for this connection only.
 *    - *Path*: MDS+ path to the node. The format is "\\TREENAME::TOP:PATH_TO_NODE".
 *    - *DataOrientation*: Only for nodes which contain 2D matrices. It sets the way to linealize the
 *          data in memory. Its valid values are "RowMajor" or "ColumnMajor". If not set, the default value
 *          is "ColumnMajor".
 *    - *Dim*: ????
 *    - *StartIdx*: ????
 *    - *StopIdx*: ????
 *
 */
class MDSObjectConnection : public ObjectConnectionI {

public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor.
     */
    MDSObjectConnection();

    /**
     * @brief Destructor.
     */
    virtual ~MDSObjectConnection();

    /**
     * @brief
     */
    virtual bool Initialise(StructuredDataI & data);

private:

    ErrorManagement::ErrorType ConnectParameter(StreamString nodeName, ConfigurationDatabase nodeParams);

    ErrorManagement::ErrorType AddAnyType(StreamString nodeName, StreamString orientation, MDSplus::Data* const nodeData);

    /**
     * @brief Holds a copy of the parameters database
     */
    ConfigurationDatabase parametersCdb;

    StaticList<void*> deallocationList;       //<! Row-major copies of the parameters to be deallocated.

    StreamString treeName;
    StreamString serverName;
    int32        shotNumber;
    StreamString clientTypeName;

    MDSClientType clientType;
};

} /* namespace MARTe */

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* MDSOBJECTCONNECTION_H_ */
