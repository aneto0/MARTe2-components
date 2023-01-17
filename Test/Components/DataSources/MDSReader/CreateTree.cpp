/**
 * @file CreateTree.cpp
 * @brief Source file for class CreateTree
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

 * @details This source file contains the definition of all the methods for
 * the class CreateTree (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include "mdsobjects.h"
/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "CreateTree.h"
#include "AdvancedErrorManagement.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace {

}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

CreateTree::CreateTree(StreamString name,
                       uint32 nElementsPerSeg, //200000,//20,
                       uint32 nSegments, //1000,//100,
                       float64 elapsetTimeSeg0) {
    treeName = name;
    pulseNumber = 1;
    numberOfElementsPerSeg = nElementsPerSeg; //400000000;
    numberOfSegments = nSegments; //100;
    elapsedTimeSegment = elapsetTimeSeg0;
    period = elapsedTimeSegment / numberOfElementsPerSeg;
    CreateModel();
    CreatePulseNumber();
    AddSignal_uint8();
    AddSignal_int8();
    AddSignal_uint16();
    AddSignal_int16();
    AddSignal_uint32();
    AddSignal_int32();
    AddSignal_uint64();
    AddSignal_int64();
    AddSignal_float32();
    AddSignal_float64();
    AddSignal_Info();

    //1 segment with data and the other empty
    pulseNumber++;
    CreatePulseNumber();
    AddSignal_uint8(true);
    AddSignal_int8(true);
    AddSignal_uint16(true);
    AddSignal_int16(true);
    AddSignal_uint32(true);
    AddSignal_int32(true);
    AddSignal_uint64(true);
    AddSignal_int64(true);
    AddSignal_float32(true);
    AddSignal_float64(true);

    //1 segment empty another with data
    pulseNumber++; // shotNumber = 3
    CreatePulseNumber();
    AddSignal_uint8(true, false, 1);
    AddSignal_int8(true, false, 1);
    AddSignal_uint16(true, false, 1);
    AddSignal_int16(true, false, 1);
    AddSignal_uint32(true, false, 1);
    AddSignal_int32(true, false, 1);
    AddSignal_uint64(true, false, 1);
    AddSignal_int64(true, false, 1);
    AddSignal_float32(true, false, 1);
    AddSignal_float64(true, false, 1);

    //2 segment with data and 2 segment empty
    pulseNumber++;
    CreatePulseNumber();
    AddSignal_uint8(true, true, 2);
    AddSignal_int8(true, true, 2);
    AddSignal_uint16(true, true, 2);
    AddSignal_int16(true, true, 2);
    AddSignal_uint32(true, true, 2);
    AddSignal_int32(true, true, 2);
    AddSignal_uint64(true, true, 2);
    AddSignal_int64(true, true, 2);
    AddSignal_float32(true, true, 2);
    AddSignal_float64(true, true, 2);
}
CreateTree::~CreateTree() {
}

void CreateTree::CreateModel() {
    MDSplus::Tree *tree = NULL_PTR(MDSplus::Tree *);
    try {
        tree = new MDSplus::Tree(treeName.Buffer(), -1, "NEW");
    }
    catch (const MDSplus::MdsException &exc) {
        REPORT_ERROR_STATIC(ErrorManagement::Warning, "Failed opening tree %s with the pulseNumber = %d. Error: %s", treeName.Buffer(), -1, exc.what());
    }
    tree->addNode("S_uint8", "SIGNAL");
    tree->addNode("S_int8", "SIGNAL");
    tree->addNode("S_uint16", "SIGNAL");
    tree->addNode("S_int16", "SIGNAL");
    tree->addNode("S_uint32", "SIGNAL");
    tree->addNode("S_int32", "SIGNAL");
    tree->addNode("S_uint64", "SIGNAL");
    tree->addNode("S_int64", "SIGNAL");
    tree->addNode("S_float32", "SIGNAL");
    tree->addNode("S_float64", "SIGNAL");
    tree->addNode("EmptyNode", "SIGNAL");
    tree->addNode("Info", "TEXT");
    tree->write();
    delete tree;
}

void CreateTree::CreatePulseNumber() {
    MDSplus::Tree *tree = new MDSplus::Tree(treeName.Buffer(), -1);
    uint32 shotNumber = tree->getCurrent(treeName.Buffer());
    shotNumber++;
    tree->setCurrent(treeName.Buffer(), shotNumber);
    tree->createPulse(shotNumber);
    delete tree;
}

void CreateTree::AddSignal_uint8(bool toggle,
                                 bool initialTrigger,
                                 uint32 segmentsOn) {
    //printf("uint8 node\n");
    MARTe::uint8 *data = new MARTe::uint8[numberOfElementsPerSeg];
    MDSplus::Tree *tree = new MDSplus::Tree(treeName.Buffer(), pulseNumber);
    MDSplus::TreeNode *node1 = tree->getNode("S_uint8");
    MARTe::float64 start = 0.0;
    MDSplus::Data *Dstart = NULL_PTR(MDSplus::Data *);
    MARTe::float64 end = period * (numberOfElementsPerSeg - 1u);
    MDSplus::Data *Dend = NULL_PTR(MDSplus::Data *);
    MDSplus::Array *dataArray = NULL_PTR(MDSplus::Array *);
    MARTe::uint8 accum = 0u;
    uint32 countSegments = 0;
    bool trigger = initialTrigger;
    for (uint32 j = 0u; j < numberOfSegments; j++) {
        if (trigger) {
            Dstart = new MDSplus::Float64(start);
            Dend = new MDSplus::Float64(end);
            MDSplus::Data *dimension = new MDSplus::Range(Dstart, Dend, new MDSplus::Float64(period));
            for (MARTe::uint32 i = 0u; i < numberOfElementsPerSeg; i++) {
                data[i] = accum;
                accum += 1;

            }
            dataArray = new MDSplus::Uint8Array(data, numberOfElementsPerSeg);
            //std::cout << "dataArray uin8 = " << dataArray << std::endl;
            node1->makeSegment(Dstart, Dend, dimension, dataArray);
            MDSplus::deleteData(dimension); //it deletes Dstart and Dend as well!!!!
            MDSplus::deleteData(dataArray);
        }
        start = start + elapsedTimeSegment;
        end = end + elapsedTimeSegment;
        if (toggle) {
            countSegments++;
            if (countSegments == segmentsOn) {
                if (trigger) {
                    trigger = false;
                }
                else {
                    trigger = true;
                }
                countSegments = 0u;
            }
        }

    }
    delete[] data;
    delete tree;
    delete node1;
    return;
}

void CreateTree::AddSignal_int8(bool toggle,
                                bool initialTrigger,
                                uint32 segmentsOn) {
    //printf("int8 node\n");
    MARTe::int8 *data = new MARTe::int8[numberOfElementsPerSeg];
    MDSplus::Tree *tree = new MDSplus::Tree(treeName.Buffer(), pulseNumber);
    MDSplus::TreeNode *node1 = tree->getNode("S_int8");
    MARTe::float64 start = 0.0;
    MDSplus::Data *Dstart = NULL_PTR(MDSplus::Data *);
    MARTe::float64 end = period * (numberOfElementsPerSeg - 1u);
    MDSplus::Data *Dend = NULL_PTR(MDSplus::Data *);
    MDSplus::Array *dataArray = NULL_PTR(MDSplus::Array *);
    MARTe::int8 accum = -20;
    uint32 countSegments = 0;
    bool trigger = initialTrigger;
    for (uint32 j = 0u; j < numberOfSegments; j++) {
        if (trigger == 1) {
            Dstart = new MDSplus::Float64(start);
            Dend = new MDSplus::Float64(end);
            MDSplus::Data *dimension = new MDSplus::Range(Dstart, Dend, new MDSplus::Float64(period));
            for (MARTe::uint32 i = 0u; i < numberOfElementsPerSeg; i++) {
                data[i] = accum;
                accum += 1;
            }
            dataArray = new MDSplus::Int8Array((const char*) (data), numberOfElementsPerSeg); // int8 * give an error, so const char * convertion was done
            node1->makeSegment(Dstart, Dend, dimension, dataArray);
            MDSplus::deleteData(dimension); //it deletes Dstart and Dend as well!!!!
            MDSplus::deleteData(dataArray);
        }
        start = start + elapsedTimeSegment;
        end = end + elapsedTimeSegment;
        if (toggle) {
            countSegments++;
            if (countSegments == segmentsOn) {
                if (trigger) {
                    trigger = false;
                }
                else {
                    trigger = true;
                }
                countSegments = 0u;
            }
        }
    }
    delete[] data;
    delete tree;
    delete node1;
    return;
}

void CreateTree::AddSignal_uint16(bool toggle,
                                  bool initialTrigger,
                                  uint32 segmentsOn) {
    //printf("uint16 node\n");
    MARTe::uint16 *data = new MARTe::uint16[numberOfElementsPerSeg];
    MDSplus::Tree *tree = new MDSplus::Tree(treeName.Buffer(), pulseNumber);
    MDSplus::TreeNode *node1 = tree->getNode("S_uint16");
    MARTe::float64 start = 0.0;
    MDSplus::Data *Dstart = NULL_PTR(MDSplus::Data *);
    MARTe::float64 end = period * (numberOfElementsPerSeg - 1u);
    MDSplus::Data *Dend = NULL_PTR(MDSplus::Data *);
    MDSplus::Array *dataArray = NULL_PTR(MDSplus::Array *);
    MARTe::uint16 accum = 0u;
    uint32 countSegments = 0;
    bool trigger = initialTrigger;
    for (uint32 j = 0u; j < numberOfSegments; j++) {
        if (trigger == 1) {
            Dstart = new MDSplus::Float64(start);
            Dend = new MDSplus::Float64(end);
            MDSplus::Data *dimension = new MDSplus::Range(Dstart, Dend, new MDSplus::Float64(period));
            for (MARTe::uint32 i = 0u; i < numberOfElementsPerSeg; i++) {
                data[i] = accum;
                accum += 2;
            }
            dataArray = new MDSplus::Uint16Array(data, numberOfElementsPerSeg);
            node1->makeSegment(Dstart, Dend, dimension, dataArray);
            MDSplus::deleteData(dimension); //it deletes Dstart and Dend as well!!!!
            MDSplus::deleteData(dataArray);
        }
        start = start + elapsedTimeSegment;
        end = end + elapsedTimeSegment;
        if (toggle) {
            countSegments++;
            if (countSegments == segmentsOn) {
                if (trigger) {
                    trigger = false;
                }
                else {
                    trigger = true;
                }
                countSegments = 0u;
            }
        }
    }
    delete[] data;
    delete tree;
    delete node1;
    return;
}

void CreateTree::AddSignal_int16(bool toggle,
                                 bool initialTrigger,
                                 uint32 segmentsOn) {
    //printf("int16 node\n");
    MARTe::int16 *data = new MARTe::int16[numberOfElementsPerSeg];
    MDSplus::Tree *tree = new MDSplus::Tree(treeName.Buffer(), pulseNumber);
    MDSplus::TreeNode *node1 = tree->getNode("S_int16");
    MARTe::float64 start = 0.0;
    MDSplus::Data *Dstart = NULL_PTR(MDSplus::Data *);
    MARTe::float64 end = period * (numberOfElementsPerSeg - 1u);
    MDSplus::Data *Dend = NULL_PTR(MDSplus::Data *);
    MDSplus::Array *dataArray = NULL_PTR(MDSplus::Array *);
    MARTe::int16 accum = -40;
    uint32 countSegments = 0;
    bool trigger = initialTrigger;
    for (uint32 j = 0u; j < numberOfSegments; j++) {
        if (trigger == 1) {
            Dstart = new MDSplus::Float64(start);
            Dend = new MDSplus::Float64(end);
            MDSplus::Data *dimension = new MDSplus::Range(Dstart, Dend, new MDSplus::Float64(period));
            for (MARTe::uint32 i = 0u; i < numberOfElementsPerSeg; i++) {
                data[i] = accum;
                accum += 2;
            }
            dataArray = new MDSplus::Int16Array(data, numberOfElementsPerSeg);
            node1->makeSegment(Dstart, Dend, dimension, dataArray);
            MDSplus::deleteData(dimension); //it deletes Dstart and Dend as well!!!!
            MDSplus::deleteData(dataArray);
        }
        start = start + elapsedTimeSegment;
        end = end + elapsedTimeSegment;
        if (toggle) {
            countSegments++;
            if (countSegments == segmentsOn) {
                if (trigger) {
                    trigger = false;
                }
                else {
                    trigger = true;
                }
                countSegments = 0u;
            }
        }
    }
    delete[] data;
    delete tree;
    delete node1;
    return;
}

void CreateTree::AddSignal_uint32(bool toggle,
                                  bool initialTrigger,
                                  uint32 segmentsOn) {
    //printf("uint32 node\n");
    MARTe::uint32 *data = new MARTe::uint32[numberOfElementsPerSeg];
    MDSplus::Tree *tree = new MDSplus::Tree(treeName.Buffer(), pulseNumber);
    MDSplus::TreeNode *node1 = tree->getNode("S_uint32");
    MARTe::float64 start = 0.0;
    MDSplus::Data *Dstart = NULL_PTR(MDSplus::Data *);
    MARTe::float64 end = period * (numberOfElementsPerSeg - 1u);
    MDSplus::Data *Dend = NULL_PTR(MDSplus::Data *);
    MDSplus::Array *dataArray = NULL_PTR(MDSplus::Array *);
    MARTe::uint32 accum = 0;
    uint32 countSegments = 0;
    bool trigger = initialTrigger;
    for (uint32 j = 0u; j < numberOfSegments; j++) {
        if (trigger == 1) {
            Dstart = new MDSplus::Float64(start);
            Dend = new MDSplus::Float64(end);
            MDSplus::Data *dimension = new MDSplus::Range(Dstart, Dend, new MDSplus::Float64(period));
            for (MARTe::uint32 i = 0u; i < numberOfElementsPerSeg; i++) {
                data[i] = accum;
                accum += 4;
            }
            dataArray = new MDSplus::Uint32Array(data, numberOfElementsPerSeg);
            node1->makeSegment(Dstart, Dend, dimension, dataArray);
            MDSplus::deleteData(dimension); //it deletes Dstart and Dend as well!!!!
            MDSplus::deleteData(dataArray);
        }
        start = start + elapsedTimeSegment;
        end = end + elapsedTimeSegment;
        if (toggle) {
            countSegments++;
            if (countSegments == segmentsOn) {
                if (trigger) {
                    trigger = false;
                }
                else {
                    trigger = true;
                }
                countSegments = 0u;
            }
        }
    }
    delete[] data;
    delete tree;
    delete node1;
    return;
}

void CreateTree::AddSignal_int32(bool toggle,
                                 bool initialTrigger,
                                 uint32 segmentsOn) {
    //printf("int32 node\n");
    MARTe::int32 *data = new MARTe::int32[numberOfElementsPerSeg];
    MDSplus::Tree *tree = new MDSplus::Tree(treeName.Buffer(), pulseNumber);
    MDSplus::TreeNode *node1 = tree->getNode("S_int32");
    MARTe::float64 start = 0.0;
    MDSplus::Data *Dstart = NULL_PTR(MDSplus::Data *);
    MARTe::float64 end = period * (numberOfElementsPerSeg - 1u);
    MDSplus::Data *Dend = NULL_PTR(MDSplus::Data *);
    MDSplus::Array *dataArray = NULL_PTR(MDSplus::Array *);
    MARTe::int32 accum = -80;
    uint32 countSegments = 0;
    bool trigger = initialTrigger;
    for (uint32 j = 0u; j < numberOfSegments; j++) {
        if (trigger == 1) {
            Dstart = new MDSplus::Float64(start);
            Dend = new MDSplus::Float64(end);
            MDSplus::Data *dimension = new MDSplus::Range(Dstart, Dend, new MDSplus::Float64(period));
            for (MARTe::uint32 i = 0u; i < numberOfElementsPerSeg; i++) {
                data[i] = accum;
                accum += 4;
            }
            dataArray = new MDSplus::Int32Array(data, numberOfElementsPerSeg);
            node1->makeSegment(Dstart, Dend, dimension, dataArray);
            MDSplus::deleteData(dimension); //it deletes Dstart and Dend as well!!!!
            MDSplus::deleteData(dataArray);
        }
        start = start + elapsedTimeSegment;
        end = end + elapsedTimeSegment;
        if (toggle) {
            countSegments++;
            if (countSegments == segmentsOn) {
                if (trigger) {
                    trigger = false;
                }
                else {
                    trigger = true;
                }
                countSegments = 0u;
            }
        }
    }
    delete[] data;
    delete tree;
    delete node1;
    return;
}

void CreateTree::AddSignal_uint64(bool toggle,
                                  bool initialTrigger,
                                  uint32 segmentsOn) {
    //printf("uint64 node\n");
    MARTe::uint64 *data = new MARTe::uint64[numberOfElementsPerSeg];
    MDSplus::Tree *tree = new MDSplus::Tree(treeName.Buffer(), pulseNumber);
    MDSplus::TreeNode *node1 = tree->getNode("S_uint64");
    MARTe::float64 start = 0.0;
    MDSplus::Data *Dstart = NULL_PTR(MDSplus::Data *);
    MARTe::float64 end = period * (numberOfElementsPerSeg - 1u);
    MDSplus::Data *Dend = NULL_PTR(MDSplus::Data *);
    MDSplus::Array *dataArray = NULL_PTR(MDSplus::Array *);
    MARTe::uint64 accum = 0;
    uint32 countSegments = 0;
    bool trigger = initialTrigger;
    for (uint32 j = 0u; j < numberOfSegments; j++) {
        if (trigger == 1) {
            Dstart = new MDSplus::Float64(start);
            Dend = new MDSplus::Float64(end);
            MDSplus::Data *dimension = new MDSplus::Range(Dstart, Dend, new MDSplus::Float64(period));
            for (MARTe::uint32 i = 0u; i < numberOfElementsPerSeg; i++) {
                data[i] = accum;
                accum += 8;
            }
            dataArray = new MDSplus::Uint64Array((const long unsigned int *) (data), numberOfElementsPerSeg);
            node1->makeSegment(Dstart, Dend, dimension, dataArray);
            MDSplus::deleteData(dimension); //it deletes Dstart and Dend as well!!!!
            MDSplus::deleteData(dataArray);
        }
        start = start + elapsedTimeSegment;
        end = end + elapsedTimeSegment;
        if (toggle) {
            countSegments++;
            if (countSegments == segmentsOn) {
                if (trigger) {
                    trigger = false;
                }
                else {
                    trigger = true;
                }
                countSegments = 0u;
            }
        }
    }
    delete[] data;
    delete tree;
    delete node1;
    return;
}

void CreateTree::AddSignal_int64(bool toggle,
                                 bool initialTrigger,
                                 uint32 segmentsOn) {
    //printf("int64 node\n");
    MARTe::int64 *data = new MARTe::int64[numberOfElementsPerSeg];
    MDSplus::Tree *tree = new MDSplus::Tree(treeName.Buffer(), pulseNumber);
    MDSplus::TreeNode *node1 = tree->getNode("S_int64");
    MARTe::float64 start = 0.0;
    MDSplus::Data *Dstart = NULL_PTR(MDSplus::Data *);
    MARTe::float64 end = period * (numberOfElementsPerSeg - 1u);
    MDSplus::Data *Dend = NULL_PTR(MDSplus::Data *);
    MDSplus::Array *dataArray = NULL_PTR(MDSplus::Array *);
    MARTe::int64 accum = -160;
    uint32 countSegments = 0;
    bool trigger = initialTrigger;
    for (uint32 j = 0u; j < numberOfSegments; j++) {
        if (trigger == 1) {
            Dstart = new MDSplus::Float64(start);
            Dend = new MDSplus::Float64(end);
            MDSplus::Data *dimension = new MDSplus::Range(Dstart, Dend, new MDSplus::Float64(period));
            for (MARTe::uint32 i = 0u; i < numberOfElementsPerSeg; i++) {
                data[i] = accum;
                accum += 8;
            }
            dataArray = new MDSplus::Int64Array((const long int *) (data), numberOfElementsPerSeg);
            node1->makeSegment(Dstart, Dend, dimension, dataArray);
            MDSplus::deleteData(dimension); //it deletes Dstart and Dend as well!!!!
            MDSplus::deleteData(dataArray);
        }
        start = start + elapsedTimeSegment;
        end = end + elapsedTimeSegment;
        if (toggle) {
            countSegments++;
            if (countSegments == segmentsOn) {
                if (trigger) {
                    trigger = false;
                }
                else {
                    trigger = true;
                }
                countSegments = 0u;
            }
        }
    }
    delete[] data;
    delete tree;
    delete node1;
    return;
}

void CreateTree::AddSignal_float32(bool toggle,
                                   bool initialTrigger,
                                   uint32 segmentsOn) {
    //printf("float32 node\n");
    MARTe::float32 *data = new MARTe::float32[numberOfElementsPerSeg];
    MDSplus::Tree *tree = new MDSplus::Tree(treeName.Buffer(), pulseNumber);
    MDSplus::TreeNode *node1 = tree->getNode("S_float32");
    MARTe::float64 start = 0.0;
    MDSplus::Data *Dstart = NULL_PTR(MDSplus::Data *);
    MARTe::float64 end = period * (numberOfElementsPerSeg - 1u);
    MDSplus::Data *Dend = NULL_PTR(MDSplus::Data *);
    MDSplus::Array *dataArray = NULL_PTR(MDSplus::Array *);
    MARTe::float32 accum = 0.0;
    uint32 countSegments = 0;
    bool trigger = initialTrigger;
    for (uint32 j = 0u; j < numberOfSegments; j++) {
        if (trigger == 1) {
            Dstart = new MDSplus::Float64(start);
            Dend = new MDSplus::Float64(end);
            MDSplus::Data *dimension = new MDSplus::Range(Dstart, Dend, new MDSplus::Float64(period));
            for (MARTe::uint32 i = 0u; i < numberOfElementsPerSeg; i++) {
                data[i] = accum;
                accum += 0.1;
            }
            dataArray = new MDSplus::Float32Array(data, numberOfElementsPerSeg);
            node1->makeSegment(Dstart, Dend, dimension, dataArray);
            MDSplus::deleteData(dimension); //it deletes Dstart and Dend as well!!!!
            MDSplus::deleteData(dataArray);
        }
        start = start + elapsedTimeSegment;
        end = end + elapsedTimeSegment;
        if (toggle) {
            countSegments++;
            if (countSegments == segmentsOn) {
                if (trigger) {
                    trigger = false;
                }
                else {
                    trigger = true;
                }
                countSegments = 0u;
            }
        }
    }
    delete[] data;
    delete tree;
    delete node1;
    return;
}

void CreateTree::AddSignal_float64(bool toggle,
                                   bool initialTrigger,
                                   uint32 segmentsOn) {
    //printf("float64 node\n");
    MARTe::float64 *data = new MARTe::float64[numberOfElementsPerSeg];
    MDSplus::Tree *tree = new MDSplus::Tree(treeName.Buffer(), pulseNumber);
    MDSplus::TreeNode *node1 = tree->getNode("S_float64");
    MARTe::float64 start = 0.0;
    MDSplus::Data *Dstart = NULL_PTR(MDSplus::Data *);
    MARTe::float64 end = period * (numberOfElementsPerSeg - 1u);
    MDSplus::Data *Dend = NULL_PTR(MDSplus::Data *);
    MDSplus::Array *dataArray = NULL_PTR(MDSplus::Array *);
    MARTe::float64 accum = 0.0;
    uint32 countSegments = 0;
    bool trigger = initialTrigger;
    for (uint32 j = 0u; j < numberOfSegments; j++) {
        if (trigger == 1) {
            Dstart = new MDSplus::Float64(start);
            Dend = new MDSplus::Float64(end);
            MDSplus::Data *dimension = new MDSplus::Range(Dstart, Dend, new MDSplus::Float64(period));
            for (MARTe::uint32 i = 0u; i < numberOfElementsPerSeg; i++) {
                data[i] = accum;
                accum += 0.1;
            }
            dataArray = new MDSplus::Float64Array(data, numberOfElementsPerSeg);
            node1->makeSegment(Dstart, Dend, dimension, dataArray);
            MDSplus::deleteData(dimension); //it deletes Dstart and Dend as well!!!!
            MDSplus::deleteData(dataArray);
        }
        start = start + elapsedTimeSegment;
        end = end + elapsedTimeSegment;
        if (toggle) {
            countSegments++;
            if (countSegments == segmentsOn) {
                if (trigger) {
                    trigger = false;
                }
                else {
                    trigger = true;
                }
                countSegments = 0u;
            }
        }
    }
    delete[] data;
    delete tree;
    delete node1;
    return;
}

void CreateTree::AddSignal_Info(bool toggle) {
    //printf("AddInfo node\n");
    MARTe::StreamString data = "It is a tree for testing MDSReader\nThis node will be used to check an error of unsupported type";
    MDSplus::Tree *tree = new MDSplus::Tree(treeName.Buffer(), pulseNumber);
    MDSplus::TreeNode *node1 = tree->getNode("Info");
    MDSplus::Data *Ddata = new MDSplus::String(data.Buffer());
    node1->putData(Ddata);

    MDSplus::deleteData(Ddata);
    delete node1;
    delete tree;

    return;
}

}

