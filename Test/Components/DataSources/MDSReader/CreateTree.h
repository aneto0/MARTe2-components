/**
 * @file CreateTree.h
 * @brief Header file for class CreateTree
 * @date 16/01/2023
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

 * @details This header file contains the declaration of the class CreateTree
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef MDSREADER_CREATETREE_H_
#define MDSREADER_CREATETREE_H_

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

namespace MARTe {

/**
 * @brief Create a tree for testing
 * @details Create a tree called test_tree with the following nodes (in parenthesis the  type of the data and the type of node):
 *    S_uint8   (uint8, SIGNAL)
 *    S_int8    (int8, SIGNAL)
 *    S_uint16  (uint16, SIGNAL)
 *    S_int16   (int16, SIGNAL)
 *    S_uint32  (uint32, SIGNAL)
 *    S_int32   (int32, SIGNAL)
 *    S_uint64  (uint64, SIGNAL)
 *    S_int64   (int64, SIGNAL)
 *    S_float32 (float32, SIGNAL)
 *    S_float64 (float64, SIGNAL)
 *    Info      (String, TEXT)
 * A pulse 1, 2, 3 and 4 are created filling the SIGNAL nodes with ramps (with different step increments).
 * The environmental variable test_tree_path is automatically created in the scope of the tests.
 * In each test a tree with two poses are created and then are removed.
 */
class CreateTree {
public:
    CreateTree(StreamString name,
               uint32 nElementsPerSeg = 20, //200000,//20,
               uint32 nSegments = 100, //1000,//100,
               float64 elapsetTimeSeg = 2.0);

    virtual ~CreateTree();

private:
    void CreateModel();

    void CreatePulseNumber();

    void AddSignal_uint8(bool toggle = false,
                         bool initialTrigger = true,
                         uint32 segmentsOn = 1u);

    void AddSignal_int8(bool toggle = false,
                        bool initialTrigger = true,
                        uint32 segmentsOn = 1u);

    void AddSignal_uint16(bool toggle = false,
                          bool initialTrigger = true,
                          uint32 segmentsOn = 1u);

    void AddSignal_int16(bool toggle = false,
                         bool initialTrigger = true,
                         uint32 segmentsOn = 1u);

    void AddSignal_uint32(bool toggle = false,
                          bool initialTrigger = true,
                          uint32 segmentsOn = 1u);

    void AddSignal_int32(bool toggle = false,
                         bool initialTrigger = true,
                         uint32 segmentsOn = 1u);

    void AddSignal_uint64(bool toggle = false,
                          bool initialTrigger = true,
                          uint32 segmentsOn = 1u);

    void AddSignal_int64(bool toggle = false,
                         bool initialTrigger = true,
                         uint32 segmentsOn = 1u);

    void AddSignal_float32(bool toggle = false,
                           bool initialTrigger = true,
                           uint32 segmentsOn = 1u);

    void AddSignal_float64(bool toggle = false,
                           bool initialTrigger = true,
                           uint32 segmentsOn = 1u);

    void AddSignal_Info(bool toggle = false);

    MARTe::StreamString treeName;
    MARTe::uint32 pulseNumber;
    MARTe::uint32 numberOfElementsPerSeg;
    MARTe::uint32 numberOfSegments;
    MARTe::float64 period;
    MARTe::float64 elapsedTimeSegment;
};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* MDSREADER_CREATETREE_H_ */

