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
 * @brief Loads `MARTe::AnyObject` from MDSplus trees
 * @details This class is a `StaticList<AnyType*>`. It populates itself
 *          with the information declared in the `Parameters` node
 *          of its configuration (see below). It is meant to be declared
 *          inside a `ObjectLoader` container, upon initialisation the
 *          `ObjectLoader` will then loop on this object and load any of
 *          the contained `AnyType` as `AnyObject`s, thus making them
 *          available to the rest of the framework.
 *
 * Generic description                                          {#generic-desc}
 * ============================================================================
 *
 * The `StaticList<AnyType*>` is meant to be read and serialised by a
 * `MARTe::ObjectLoader` class that makes the parameters available
 * to any component of the RealTimeApplication.
 *
 * The configuration syntax is (names and signal quantity are only given
 * as an example):
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 *
 * +ParamLoader1 = {
 *     Class = ObjectLoader                         // Compulsory
 *     Shot = "-1"                                  // Optional if defined in MDSObjectConnection
 *
 *     // All parameters declared here (except "Class") are also copied inside
 *     // each subnode of class Connection.
 *
 *     +MDSConnection1 = {
 *         Class = MDSObjectConnection              // Compulsory
 *         ClientType = ( "Thin" | "Distributed" )  // Optional. Default: "Thin" if "Server" is declared, "Distributed" otherwise
 *         Server = "localhost:8000"                // Compulsory if ClientType = "Thin"
 *         Tree = "tree_name"                       // Compulsory (unless already defined in ObjectLoader)
 *         Shot = "-1"                              // Compulsory (unless already defined in ObjectLoader)
 *
 *         gain1 = {
 *             Path  = "\\TREE_NAME::TOP:NODE1"                     // Compulsory
 *         }
 *
 *         // Parameters can be declared in a structured fashion:
 *         gainStruct1 = {
 *             gain_a = { Path  = "\\TREE_NAME::TOP:NODE2" }        // Compulsory
 *             gain_b = { Path  = "\\TREE_NAME::TOP:NODE3" }        // Compulsory
 *         }
 *
 *         // Structured parameters can also be declared in a flattened fashion, using dots `.`:
 *         gainStruct2.gain_one = { Path  = "\\TREE_NAME::TOP:NODE4" }
 *         gainStruct2.gain_two = {
 *             Path  = "\\TREE_NAME::TOP:NODE5"
 *             DataOrientation = ( "RowMajor" | "ColumnMajor" )     // Optional. Default: "ColumnMajor"
 *         }
 *
 *         // Dashes `-` can be used instead of dots `.` as separatos:
 *         gainStruct3-gain_1 = {
 *             Path  = "\\TREE_NAME::TOP:NODE6"
 *             Dim = 2                               // Optional.
 *         }
 *         gainStruct3-gain_2 = {
 *             Path  = "\\TREE_NAME::TOP:NODE%d"
 *             StartIdx = 1   StopIdx = 10           // Optional.
 *         }
 *     }
 *
 *     +Connection2 = {
 *         ...
 *     }
 * }
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * @note Structured nodes can be declared in a structured fashion or in
 *       a flattened fashion. When flattened, both dots (`.`) and dashes
 *       (`-`) may be used as separators.
 *
 * @warning Do not use `-` character for node names except for denoting
 *          structures (see note above). This character is always substituted
 *          with a dot (`.`).
 *
 * Details of the configuration syntax                           {#conf-syntax}
 * ----------------------------------------------------------------------------
 *
 * - **MDSObjectConnection parameters**:
 *     - *Tree*: name of the tree to connect to.
 *     - *Shot*: Number of the shot of the MDSplus tree from which to load the
 *               parameters. This parameter can also be defined in the
 *               `ObjectLoader` container, but defining it in the
 *               `MDSObjectConnection` will overwrite the one in the
 *               `ObjectContainer`.
 *     - *ClientType*: Optional. Its valid values are `Thin` or `Distributed`.
 *               If not set, `Thin` is used by default if the `Server` parameter
 *               is declared, and `Distributed` is used otherwise. Please refer
 *               to the MDSplus documentation for the distinction between thin
 *               client and distributed client.
 *     - *Server*: Only required if `ClientType = "Thin"`. It is a string which
 *               defines the IP address of the remote MDSplus server, and its
 *               access port. Instead of IP address, domains can be used, as
 *               well as `"localhost"` if the server is in the same machine.
 *               If `ClientType = "Distributed"`, an environment variable
 *               must be set in your machine, in the form:
 *               `treename_path=<IP>:<PORT>::<PATH>`.
 * - **Node parameters**:
 *     - *Path*: MDSplus path to the node. The format is `"\\TREENAME::TOP:PATH.TO:NODE"`.
 *     - *DataOrientation*: Only relevant for 2D or 3D matrices. It specifies
 *               the data orientation of the source parameter in the MDSplus tree.
 *               Note that the parameter should always be loaded as row-major
 *               for compatibility with the rest of the framework. Thus,
 *               specifying the orientation is important. For example, a
 *               parameter that has been stored as row-major in the MDSplus tree
 *               shall be declared as `DataOrientation = "RowMajor"`, and
 *               `MDSObjectConnection` will then import it as-is in the
 *               RealTimeApplication. Conversely, a parameter that has been
 *               stored as column-major in the MDSplus tree shall be declared
 *               as `DataOrientation = "ColumnMajor"`, and `MDSObjectConnection`
 *               will convert it to row-major before making it available to the
 *               framework. For details on how to set matrices in the source tree,
 *               see the [Setting matrices in the source tree](#setting-matrices)
 *               section below. Default value: `"ColumnMajor"`.
 *     - *TargetDim*: Optional parameter. Trims or pads a vector with zeroes to
 *               achieve the specified length.
 *     - *StartIdx*, *StopIdx*: Optional parameter. Used to concatenate scalar
 *               values in an array in the form: `[\DATA001, \DATA002, \DATA003, ...]`,
 *               where the number is specified in the `Path` parameter using the
 *               flag \%u (e.g. \\DATA00\%u).
 *
 *
 * Setting matrices in the source tree                      {#setting-matrices}
 * ----------------------------------------------------------------------------
 *
 * Matrices can be loaded in the source tree using the MDSplus APIs for that
 * environment (e.g. `putData()` or `mdsput()`). However, matrices shall be
 * declared in the environment beforehand. Use the following syntaxes to set
 * row-major or column-major matrices in the source tree:
 *
 * ### 2D matrices ###
 *
 * A 2x3 matrix \f$ M_{rm} = \left( \matrix{ 1 & 2 & 3 \cr 4 & 5 & 6 \cr} \right) \f$
 * is declared:
 * - in **row-major** orientation:
 *   - in *C* as `M_rm[2][3] = { {1, 2, 3}, {4, 5, 6} };`
 *   - in *MDSplus/TDI* as `_Mrm = [ [1, 2, 3], [4, 5, 6] ]`
 *   - in *ML*: only column-major format is available (see below)
 * - in **column-major** orientation:
 *   - *in C* as `M_rm = { {1, 4}, {2, 5}, {3, 6} };`
 *   - *in MDSplus/TDI* as `_Mrm = [ [1, 4], [2, 5], [3, 6] ]`
 *   - *in ML* as `M_rm = [ 1 2 3; 4 5 6 ];`
 *
 * ### 3D matrices ###
 *
 * A 3x4x2 matrix
 * \f$ M_{rm}(i,j,1) = \left( \matrix{ 1 & 2 & 3 & 4 \cr 5 & 6 & 7 & 8 \cr 9 & 10 & 11 & 12 \cr} \right) \f$
 * \f$ M_{rm}(i,j,2) = \left( \matrix{ 13 & 14 & 15 & 16 \cr 17 & 18 & 19 & 20 \cr 21 & 22 & 23 & 24 \cr} \right) \f$
 * is declared:
 * - in **row-major** orientation:
 *   - in *C* as `M_rm[3][4][2] = { { {1, 13}, {2, 14}, {3, 15}, {4, 16} }, { {5, 17}, {6, 18}, {7, 19}, {8, 20} }, { {9, 21}, {10, 22}, {11, 23}, {12, 24} } }`
 *   - in *MDSplus/TDI* as `_Mrm = [ [ [1,13], [2,14], [3,15], [4,16] ], [ [5,17], [6,18], [7,19], [8,20] ], [ [9,21], [10,22], [11,23], [12,24] ] ]`
 *   - in *ML* as: only column-major format is available (see below)
 * - in **column-major** orientation:
 *   - in *C* as `M_rm[2][4][3] = { { {1,  5,  9},  {2,  6, 10},  {3,  7, 11},  {4,  8, 12} }, { {13, 17, 21}, {14, 18, 22}, {15, 19, 23}, {16, 20, 24} } };`
 *   - in *MDSplus/TDI* as `_Mrm = [ [ [1,  5,  9],  [2,  6, 10],  [3,  7, 11],  [4,  8, 12] ], [ [13, 17, 21], [14, 18, 22], [15, 19, 23], [16, 20, 24] ] ]`
 *   - in *ML* as `M_rm(:,:,1) = [ 1 2 3 4; 5 6 7 8; 9 10 11 12 ]; M_rm(:,:,2) = [ 13 14 15 16; 17 18 19 20; 21 22 23 24 ];`
 *
 *
 * Release notes
 * ==========================================================================
 *
 * Version |    Date    | Notes
 * ------: | :--------: | :----
 * 1.0     | 16/09/2024 | initial release
 * 1.1     | 31/07/2025 | tested and linted version
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
