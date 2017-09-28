/**
 * @file MDSReaderTest.cpp
 * @brief Source file for class MDSReaderTest
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

 * @details This source file contains the definition of all the methods for
 * the class MDSReaderTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

#include "stdio.h"
#include <iostream>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "MDSReaderTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

class CreateTree {
public:
    CreateTree(StreamString name) {
        treeName = name;
        pulseNumber = 1;
        numberOfElementsPerSeg = 400; //400000000;
        numberOfSegments = 10; //100;
        elapsedTimeSegment = 1.0;
        period = elapsedTimeSegment / numberOfElementsPerSeg;
        CreateModel();
        CreatePulseNumber(pulseNumber);
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
        //WriteTypes();
    }
    virtual ~CreateTree() {

    }
private:
    void CreateModel() {
        MDSplus::Tree *tree = new MDSplus::Tree(treeName.Buffer(), -1, "NEW");
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
        tree->addNode("Info", "TEXT");
        tree->write();
        delete tree;
    }

    void CreatePulseNumber(MARTe::uint32 pNumber) {
        MDSplus::Tree *tree = new MDSplus::Tree(treeName.Buffer(), -1);
        tree->createPulse(pNumber);
        delete tree;
    }

    void AddSignal_uint8() {
        MARTe::uint8 *data = new MARTe::uint8[numberOfElementsPerSeg];
        MDSplus::Tree *tree = new MDSplus::Tree(treeName.Buffer(), pulseNumber);
        MDSplus::TreeNode *node1 = tree->getNode("S_uint8");
        MARTe::float64 start = 0.0;
        MDSplus::Data *Dstart = NULL_PTR(MDSplus::Data *);
        MARTe::float64 end = period * (numberOfElementsPerSeg - 1u);
        MDSplus::Data *Dend = NULL_PTR(MDSplus::Data *);
        MDSplus::Array *dataArray = NULL_PTR(MDSplus::Array *);
        MARTe::uint8 accum = 0u;
        for (uint32 j = 0u; j < numberOfSegments; j++) {
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
            start = start + elapsedTimeSegment;
            end = end + elapsedTimeSegment;
        }
        delete[] data;
        delete tree;
        delete node1;
        return;
    }

    void AddSignal_int8() {
        MARTe::int8 *data = new MARTe::int8[numberOfElementsPerSeg];
        MDSplus::Tree *tree = new MDSplus::Tree(treeName.Buffer(), pulseNumber);
        MDSplus::TreeNode *node1 = tree->getNode("S_int8");
        MARTe::float64 start = 0.0;
        MDSplus::Data *Dstart = NULL_PTR(MDSplus::Data *);
        MARTe::float64 end = period * (numberOfElementsPerSeg - 1u);
        MDSplus::Data *Dend = NULL_PTR(MDSplus::Data *);
        MDSplus::Array *dataArray = NULL_PTR(MDSplus::Array *);
        MARTe::int8 accum = -20;
        for (uint32 j = 0u; j < numberOfSegments; j++) {
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
            start = start + elapsedTimeSegment;
            end = end + elapsedTimeSegment;
        }
        delete[] data;
        delete tree;
        delete node1;
        return;
    }

    void AddSignal_uint16() {
        MARTe::uint16 *data = new MARTe::uint16[numberOfElementsPerSeg];
        MDSplus::Tree *tree = new MDSplus::Tree(treeName.Buffer(), pulseNumber);
        MDSplus::TreeNode *node1 = tree->getNode("S_uint16");
        MARTe::float64 start = 0.0;
        MDSplus::Data *Dstart = NULL_PTR(MDSplus::Data *);
        MARTe::float64 end = period * (numberOfElementsPerSeg - 1u);
        MDSplus::Data *Dend = NULL_PTR(MDSplus::Data *);
        MDSplus::Array *dataArray = NULL_PTR(MDSplus::Array *);
        MARTe::uint16 accum = 0u;
        for (uint32 j = 0u; j < numberOfSegments; j++) {
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
            start = start + elapsedTimeSegment;
            end = end + elapsedTimeSegment;
        }
        delete[] data;
        delete tree;
        delete node1;
        return;
    }

    void AddSignal_int16() {
        MARTe::int16 *data = new MARTe::int16[numberOfElementsPerSeg];
        MDSplus::Tree *tree = new MDSplus::Tree(treeName.Buffer(), pulseNumber);
        MDSplus::TreeNode *node1 = tree->getNode("S_int16");
        MARTe::float64 start = 0.0;
        MDSplus::Data *Dstart = NULL_PTR(MDSplus::Data *);
        MARTe::float64 end = period * (numberOfElementsPerSeg - 1u);
        MDSplus::Data *Dend = NULL_PTR(MDSplus::Data *);
        MDSplus::Array *dataArray = NULL_PTR(MDSplus::Array *);
        MARTe::int16 accum = -40;
        for (uint32 j = 0u; j < numberOfSegments; j++) {
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
            start = start + elapsedTimeSegment;
            end = end + elapsedTimeSegment;
        }
        delete[] data;
        delete tree;
        delete node1;
        return;
    }

    void AddSignal_uint32() {
        MARTe::uint32 *data = new MARTe::uint32[numberOfElementsPerSeg];
        MDSplus::Tree *tree = new MDSplus::Tree(treeName.Buffer(), pulseNumber);
        MDSplus::TreeNode *node1 = tree->getNode("S_uint32");
        MARTe::float64 start = 0.0;
        MDSplus::Data *Dstart = NULL_PTR(MDSplus::Data *);
        MARTe::float64 end = period * (numberOfElementsPerSeg - 1u);
        MDSplus::Data *Dend = NULL_PTR(MDSplus::Data *);
        MDSplus::Array *dataArray = NULL_PTR(MDSplus::Array *);
        MARTe::uint32 accum = 0;
        for (uint32 j = 0u; j < numberOfSegments; j++) {
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
            start = start + elapsedTimeSegment;
            end = end + elapsedTimeSegment;
        }
        delete[] data;
        delete tree;
        delete node1;
        return;
    }

    void AddSignal_int32() {
        MARTe::int32 *data = new MARTe::int32[numberOfElementsPerSeg];
        MDSplus::Tree *tree = new MDSplus::Tree(treeName.Buffer(), pulseNumber);
        MDSplus::TreeNode *node1 = tree->getNode("S_int32");
        MARTe::float64 start = 0.0;
        MDSplus::Data *Dstart = NULL_PTR(MDSplus::Data *);
        MARTe::float64 end = period * (numberOfElementsPerSeg - 1u);
        MDSplus::Data *Dend = NULL_PTR(MDSplus::Data *);
        MDSplus::Array *dataArray = NULL_PTR(MDSplus::Array *);
        MARTe::int32 accum = -80;
        for (uint32 j = 0u; j < numberOfSegments; j++) {
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
            start = start + elapsedTimeSegment;
            end = end + elapsedTimeSegment;
        }
        delete[] data;
        delete tree;
        delete node1;
        return;
    }

    void AddSignal_uint64() {
        MARTe::uint64 *data = new MARTe::uint64[numberOfElementsPerSeg];
        MDSplus::Tree *tree = new MDSplus::Tree(treeName.Buffer(), pulseNumber);
        MDSplus::TreeNode *node1 = tree->getNode("S_uint64");
        MARTe::float64 start = 0.0;
        MDSplus::Data *Dstart = NULL_PTR(MDSplus::Data *);
        MARTe::float64 end = period * (numberOfElementsPerSeg - 1u);
        MDSplus::Data *Dend = NULL_PTR(MDSplus::Data *);
        MDSplus::Array *dataArray = NULL_PTR(MDSplus::Array *);
        MARTe::uint64 accum = 0;
        for (uint32 j = 0u; j < numberOfSegments; j++) {
            Dstart = new MDSplus::Float64(start);
            Dend = new MDSplus::Float64(end);
            MDSplus::Data *dimension = new MDSplus::Range(Dstart, Dend, new MDSplus::Float64(period));
            for (MARTe::uint32 i = 0u; i < numberOfElementsPerSeg; i++) {
                data[i] = accum;
                accum += 4;
            }
            dataArray = new MDSplus::Uint64Array((const long unsigned int *) (data), numberOfElementsPerSeg);
            node1->makeSegment(Dstart, Dend, dimension, dataArray);
            MDSplus::deleteData(dimension); //it deletes Dstart and Dend as well!!!!
            MDSplus::deleteData(dataArray);
            start = start + elapsedTimeSegment;
            end = end + elapsedTimeSegment;
        }
        delete[] data;
        delete tree;
        delete node1;
        return;
    }

    void AddSignal_int64() {
        MARTe::int64 *data = new MARTe::int64[numberOfElementsPerSeg];
        MDSplus::Tree *tree = new MDSplus::Tree(treeName.Buffer(), pulseNumber);
        MDSplus::TreeNode *node1 = tree->getNode("S_int64");
        MARTe::float64 start = 0.0;
        MDSplus::Data *Dstart = NULL_PTR(MDSplus::Data *);
        MARTe::float64 end = period * (numberOfElementsPerSeg - 1u);
        MDSplus::Data *Dend = NULL_PTR(MDSplus::Data *);
        MDSplus::Array *dataArray = NULL_PTR(MDSplus::Array *);
        MARTe::int64 accum = -160;
        for (uint32 j = 0u; j < numberOfSegments; j++) {
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
            start = start + elapsedTimeSegment;
            end = end + elapsedTimeSegment;
        }
        delete[] data;
        delete tree;
        delete node1;
        return;
    }

    void AddSignal_float32() {
        MARTe::float32 *data = new MARTe::float32[numberOfElementsPerSeg];
        MDSplus::Tree *tree = new MDSplus::Tree(treeName.Buffer(), pulseNumber);
        MDSplus::TreeNode *node1 = tree->getNode("S_float32");
        MARTe::float64 start = 0.0;
        MDSplus::Data *Dstart = NULL_PTR(MDSplus::Data *);
        MARTe::float64 end = period * (numberOfElementsPerSeg - 1u);
        MDSplus::Data *Dend = NULL_PTR(MDSplus::Data *);
        MDSplus::Array *dataArray = NULL_PTR(MDSplus::Array *);
        MARTe::float32 accum = 0.0;
        for (uint32 j = 0u; j < numberOfSegments; j++) {
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
            start = start + elapsedTimeSegment;
            end = end + elapsedTimeSegment;
        }
        delete[] data;
        delete tree;
        delete node1;
        return;
    }

    void AddSignal_float64() {
        MARTe::float64 *data = new MARTe::float64[numberOfElementsPerSeg];
        MDSplus::Tree *tree = new MDSplus::Tree(treeName.Buffer(), pulseNumber);
        MDSplus::TreeNode *node1 = tree->getNode("S_float64");
        MARTe::float64 start = 0.0;
        MDSplus::Data *Dstart = NULL_PTR(MDSplus::Data *);
        MARTe::float64 end = period * (numberOfElementsPerSeg - 1u);
        MDSplus::Data *Dend = NULL_PTR(MDSplus::Data *);
        MDSplus::Array *dataArray = NULL_PTR(MDSplus::Array *);
        MARTe::float64 accum = 0.0;
        for (uint32 j = 0u; j < numberOfSegments; j++) {
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
            start = start + elapsedTimeSegment;
            end = end + elapsedTimeSegment;
        }
        delete[] data;
        delete tree;
        delete node1;
        return;
    }

    void AddSignal_Info() {
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
    /*
     void WriteTypes(){
     MDSplus::Tree *tree = new MDSplus::Tree("test_tree", 1);
     MDSplus::TreeNode *node1 = tree->getNode("S_uint8");
     printf("S_uint8 = %s\n", node1->getDType());
     delete node1;

     node1 = tree->getNode("S_int8");
     printf("S_int8 = %s\n", node1->getDType());
     delete node1;

     node1 = tree->getNode("S_uint16");
     printf("S_uint16 = %s\n", node1->getDType());
     delete node1;

     node1 = tree->getNode("S_int16");
     printf("S_int16 = %s\n", node1->getDType());
     delete node1;

     node1 = tree->getNode("S_uint32");
     printf("S_uint32 = %s\n", node1->getDType());
     delete node1;

     node1 = tree->getNode("S_int32");
     printf("S_int32 = %s\n", node1->getDType());
     delete node1;

     node1 = tree->getNode("S_uint64");
     printf("S_uint64 = %s\n", node1->getDType());
     delete node1;

     node1 = tree->getNode("S_int64");
     printf("S_int64 = %s\n", node1->getDType());
     delete node1;

     node1 = tree->getNode("S_float32");
     printf("S_float32 = %s\n", node1->getDType());
     delete node1;

     node1 = tree->getNode("S_float64");
     printf("S_float64 = %s\n", node1->getDType());

     delete node1;
     delete tree;
     return;
     }
     */
    MARTe::StreamString treeName;
    MARTe::uint32 pulseNumber;
    MARTe::uint32 numberOfElementsPerSeg;
    MARTe::uint32 numberOfSegments;
    MARTe::float64 period;
    MARTe::float64 elapsedTimeSegment;
};

MDSReaderTest::MDSReaderTest() {
    treeName = "test_tree";
    char *home = getenv("HOME");
    fullPath = treeName.Buffer();
    fullPath += "_path=";
    fullPath += home;
    //Important detail: fullPath must exist in all places where the environment variables is needed.
    //In other words, the scope of the environment variables is defined by the scope of the string (char *)
    //given to putenv() function.
    putenv((char *) (fullPath.Buffer()));
    CreateTree myTreeCreated(treeName);
    numberOfValidNodes = 10u;
    numberOfNodes = 11u;
    allValidNodeNames = new StreamString[numberOfValidNodes];
    allValidNodeNames[0] = "S_uint8";
    allValidNodeNames[1] = "S_int8";
    allValidNodeNames[2] = "S_uint16";
    allValidNodeNames[3] = "S_int16";
    allValidNodeNames[4] = "S_uint32";
    allValidNodeNames[5] = "S_int32";
    allValidNodeNames[6] = "S_uint64";
    allValidNodeNames[7] = "S_int64";
    allValidNodeNames[8] = "S_float32";
    allValidNodeNames[9] = "S_float64";
    allNodeNames = new StreamString[11];
    for (uint32 i = 0; i < numberOfValidNodes; i++) {
        allNodeNames[i] = allValidNodeNames[i];
    }
    allNodeNames[10] = "Info";
}

MDSReaderTest::~MDSReaderTest() {

}

bool MDSReaderTest::TestInitialiseNoTreeName() {
    bool ok;
    MDSReader dS;
    ConfigurationDatabase config;
    ok = !dS.Initialise(config);
    return ok;
}

bool MDSReaderTest::TestInitialiseWrongShotNumber() {
    CreateTree myTreeCreated(treeName);
    bool ok;
    MDSReader dS;
    ConfigurationDatabase config;
    config.Write("TreeName", treeName.Buffer());
    config.Write("ShotNumber", 0); //Wrong ShotNumber
    ok = !dS.Initialise(config);
    return ok;
}

bool MDSReaderTest::TestInitialiseWrongTreeName() {
    CreateTree myTreeCreated(treeName);
    bool ok;
    MDSReader dS;
    ConfigurationDatabase config;
    config.Write("TreeName", "test_tree1"); // wrong tree name
    config.Write("ShotNumber", 1);
    ok = !dS.Initialise(config);
    return ok;
}

bool MDSReaderTest::TestInitialiseUnexistingShotNumber() {
    CreateTree myTreeCreated(treeName);
    bool ok;
    MDSReader dS;
    ConfigurationDatabase config;
    config.Write("TreeName", treeName.Buffer());
    config.Write("ShotNumber", 666); //unexisting ShotNumber
    ok = !dS.Initialise(config);
    return ok;
}

bool MDSReaderTest::TestInitialiseNoSignals() {
    CreateTree myTreeCreated(treeName);
    bool ok;
    MDSReader dS;
    ConfigurationDatabase config;
    config.Write("TreeName", treeName.Buffer());
    config.Write("ShotNumber", 1);
    ok = !dS.Initialise(config);
    return ok;
}

bool MDSReaderTest::TestSetConfiguredDatabaseNoSignals() {
    CreateTree myTreeCreated(treeName);
    bool ok;
    MDSReader dS;
    ConfigurationDatabase config;
    config.Write("TreeName", treeName.Buffer());
    config.Write("ShotNumber", 1);
/*
     //example how to print a ConfigurationDatabase
    StreamString a;
    a.Printf("%!", config);
    printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
    printf("%s\n", a.Buffer());
    printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
*/
    config.CreateAbsolute(".Signals");
    config.MoveToRoot();
    ok = dS.Initialise(config);
    config.MoveToRoot();
    config.Delete("Signals");
    if(ok){
        ok = !dS.SetConfiguredDatabase(config);
    }
    return ok;
}

bool MDSReaderTest::TestSetConfiguredDatabase0Functions() {
    CreateTree myTreeCreated(treeName);
    bool ok;
    MDSReader dS;
    ConfigurationDatabase config;
    config.Write("TreeName", treeName.Buffer());
    config.Write("ShotNumber", 1);
    config.CreateAbsolute(".Signals");
    config.MoveToRoot();

    ok = dS.Initialise(config);
    config.MoveToRoot();

    if(ok){
        ok = !dS.SetConfiguredDatabase(config);
    }
    return ok;
}

bool MDSReaderTest::TestSetConfiguredDatabase2Functions() {
    CreateTree myTreeCreated(treeName);
    bool ok;
    MDSReader dS;
    ConfigurationDatabase config;
    config.Write("TreeName", treeName.Buffer());
    config.Write("ShotNumber", 1);
    config.CreateAbsolute(".Signals");
    config.CreateRelative("0");
    config.MoveToAncestor(1u);
    config.CreateRelative("1");
    config.CreateAbsolute("Functions");
    config.CreateRelative("0");
    config.MoveToAncestor(1u);
    config.CreateRelative("1");
    config.MoveToRoot();
    ok = dS.Initialise(config);
    config.MoveToRoot();
    if(ok){
        ok = !dS.SetConfiguredDatabase(config);
    }
    return ok;
}

bool MDSReaderTest::TestSetConfiguredDatabase0Signals() {
    CreateTree myTreeCreated(treeName);
    bool ok;
    MDSReader dS;
    ConfigurationDatabase config;
    config.Write("TreeName", treeName.Buffer());
    config.Write("ShotNumber", 1);
    config.CreateAbsolute(".Signals");
    config.CreateRelative("0");
    config.MoveToAncestor(1u);
    config.CreateRelative("1");
    config.CreateAbsolute("Functions");
    config.CreateRelative("0");
    config.MoveToRoot();

    ok = dS.Initialise(config);

    config.MoveToRoot();
    if(ok){
        ok = !dS.SetConfiguredDatabase(config);
    }
    return ok;
}

bool MDSReaderTest::TestSetConfiguredDatabaseWrongSamples() {
    CreateTree myTreeCreated(treeName);
    bool ok;
    MDSReader dS;
    ConfigurationDatabase config;
    config.Write("TreeName", treeName.Buffer());
    config.Write("ShotNumber", 1);
    config.CreateAbsolute(".Signals");
    config.CreateRelative("0");
    config.MoveToAncestor(1u);
    config.CreateRelative("1");
    config.CreateAbsolute("Functions");
    config.CreateRelative("0");
    config.CreateRelative("InputSignals");
    config.CreateRelative("0");
    config.Write("Samples", 2);
    config.MoveToAncestor(1u);
    config.Write("ByteSize", 10);
    config.MoveToRoot();

    ok = dS.Initialise(config);

    config.MoveToRoot();
    if(ok){
        ok = !dS.SetConfiguredDatabase(config);
    }
    return ok;
}

bool MDSReaderTest::TestSetConfiguredDatabaseDiffSignalsAndFunctions() {
    CreateTree myTreeCreated(treeName);
    bool ok;
    MDSReader dS;
    ConfigurationDatabase config;
    config.Write("TreeName", treeName.Buffer());
    config.Write("ShotNumber", 1);
    config.CreateAbsolute(".Signals");
    config.CreateRelative("0");
    config.MoveToAncestor(1u);
    config.CreateRelative("1");
    config.CreateAbsolute("Functions");
    config.CreateRelative("0");
    config.CreateRelative("InputSignals");
    config.CreateRelative("0");
    config.Write("Samples", 1);
    config.MoveToAncestor(1u);
    config.Write("ByteSize", 10);
    config.MoveToRoot();

    ok = dS.Initialise(config);

    config.MoveToRoot();
    StreamString a;
    a.Printf("%!", config);
    printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
    printf("%s\n", a.Buffer());
    printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
    if(ok){
        ok = !dS.SetConfiguredDatabase(config);
    }
    return ok;
}

bool MDSReaderTest::TestSetConfiguredDatabaseNoNodeName() {
    CreateTree myTreeCreated(treeName);
    bool ok;
    MDSReader dS;
    ConfigurationDatabase config;
    config.Write("TreeName", treeName.Buffer());
    config.Write("ShotNumber", 1);
    config.CreateAbsolute(".Signals");
    config.MoveToRoot();
    ok = dS.Initialise(config);
    config.CreateRelative("0");
    config.MoveToAncestor(1u);
    config.CreateRelative("1");
    config.CreateAbsolute("Functions");
    config.CreateRelative("0");
    config.CreateRelative("InputSignals");
    config.CreateRelative("0");
    config.Write("Samples", 1);
    config.MoveToAncestor(1u);
    config.CreateRelative("1");
    config.MoveToAncestor(1u);
    config.Write("ByteSize", 10);

    config.MoveToRoot();

    if(ok){
        ok = !dS.SetConfiguredDatabase(config);
    }
    return ok;
}

bool MDSReaderTest::TestSetConfiguredDatabaseNoNodeName_2() {
    CreateTree myTreeCreated(treeName);
    bool ok;
    MDSReader dS;
    ConfigurationDatabase config;
    config.Write("TreeName", treeName.Buffer());
    config.Write("ShotNumber", 1);
    config.CreateAbsolute(".Signals");
    config.CreateRelative("0");
    config.MoveToAncestor(1u);
    config.CreateRelative("1");
    config.CreateAbsolute("Functions");
    config.CreateRelative("0");
    config.CreateRelative("InputSignals");
    config.CreateRelative("0");
    config.Write("Samples", 1);
    config.MoveToAncestor(1u);
    config.CreateRelative("1");
    config.MoveToAncestor(1u);
    config.Write("ByteSize", 10);
    config.MoveToRoot();

    ok = dS.Initialise(config);

    config.MoveToRoot();

    if(ok){
        ok = !dS.SetConfiguredDatabase(config);
    }
    return ok;
}

bool MDSReaderTest::TestSetConfiguredDatabaseInvalidNodeName() {
    CreateTree myTreeCreated(treeName);
    bool ok;
    MDSReader dS;
    ConfigurationDatabase config;
    config.Write("TreeName", treeName.Buffer());
    config.Write("ShotNumber", 1);
    config.CreateAbsolute(".Signals");
    config.CreateRelative("0");
    config.Write("NodeName", "S_uint8");
    config.MoveToAncestor(1u);
    config.CreateRelative("1");
    config.Write("NodeName", "InvalidNodeName");//invalid name

    config.CreateAbsolute("Functions");
    config.CreateRelative("0");
    config.CreateRelative("InputSignals");
    config.CreateRelative("0");
    config.Write("Samples", 1);
    config.MoveToAncestor(1u);
    config.CreateRelative("1");
    config.MoveToAncestor(1u);
    config.Write("ByteSize", 10);
    config.MoveToRoot();

    ok = dS.Initialise(config);

    config.MoveToRoot();

    if(ok){
        ok = !dS.SetConfiguredDatabase(config);
    }
    return ok;
}

bool MDSReaderTest::TestSetConfiguredDatabaseEqualNodeName() {
    CreateTree myTreeCreated(treeName);
    bool ok;
    MDSReader dS;
    ConfigurationDatabase config;
    config.Write("TreeName", treeName.Buffer());
    config.Write("ShotNumber", 1);
    config.CreateAbsolute(".Signals");
    config.CreateRelative("0");
    config.Write("NodeName", "S_uint8");
    config.MoveToAncestor(1u);
    config.CreateRelative("1");
    config.Write("NodeName", "S_uint8");//repeated

    config.CreateAbsolute("Functions");
    config.CreateRelative("0");
    config.CreateRelative("InputSignals");
    config.CreateRelative("0");
    config.Write("Samples", 1);
    config.MoveToAncestor(1u);
    config.CreateRelative("1");
    config.MoveToAncestor(1u);
    config.Write("ByteSize", 10);
    config.MoveToRoot();

    ok = dS.Initialise(config);

    config.MoveToRoot();

    if(ok){
        ok = !dS.SetConfiguredDatabase(config);
    }
    return ok;
}

bool MDSReaderTest::TestSetConfiguredDatabaseInvalidNodeType() {
    CreateTree myTreeCreated(treeName);
    bool ok;
    MDSReader dS;
    ConfigurationDatabase config;
    config.Write("TreeName", treeName.Buffer());
    config.Write("ShotNumber", 1);
    config.CreateAbsolute(".Signals");
    config.CreateRelative("0");
    config.Write("NodeName", "S_uint8");
    config.MoveToAncestor(1u);
    config.CreateRelative("1");
    config.Write("NodeName", "S_int8");
    config.MoveToAncestor(1u);
    config.CreateRelative("2");
    config.Write("NodeName", "Info");

    config.CreateAbsolute("Functions");
    config.CreateRelative("0");
    config.CreateRelative("InputSignals");
    config.CreateRelative("0");
    config.Write("Samples", 1);
    config.MoveToAncestor(1u);
    config.CreateRelative("1");
    config.MoveToAncestor(1u);
    config.CreateRelative("2");
    config.MoveToAncestor(1u);
    config.Write("ByteSize", 10);
    config.MoveToRoot();

    ok = dS.Initialise(config);

    config.MoveToRoot();

    if(ok){
        ok = !dS.SetConfiguredDatabase(config);
    }
    return ok;
}

bool MDSReaderTest::TestSetConfiguredDatabaseWrongType() {
    CreateTree myTreeCreated(treeName);
    bool ok;
    MDSReader dS;
    ConfigurationDatabase config;
    config.Write("TreeName", treeName.Buffer());
    config.Write("ShotNumber", 1);
    config.CreateAbsolute(".Signals");
    config.CreateRelative("0");
    config.Write("NodeName", "S_uint8");
    config.Write("Type", "uint8");
    config.MoveToAncestor(1u);
    config.CreateRelative("1");
    config.Write("NodeName", "S_int8");
    config.Write("Type", "WrongType");

    config.CreateAbsolute("Functions");
    config.CreateRelative("0");
    config.CreateRelative("InputSignals");
    config.CreateRelative("0");
    config.Write("Samples", 1);
    config.MoveToAncestor(1u);
    config.CreateRelative("1");
    config.MoveToAncestor(1u);
    config.Write("ByteSize", 10);
    config.MoveToRoot();

    ok = dS.Initialise(config);

    config.MoveToRoot();

    if(ok){
        ok = !dS.SetConfiguredDatabase(config);
    }
    return ok;
}

bool MDSReaderTest::TestSetConfiguredDatabaseNoNumberOfElements() {
    CreateTree myTreeCreated(treeName);
    bool ok;
    MDSReader dS;
    ConfigurationDatabase config;
    config.Write("TreeName", treeName.Buffer());
    config.Write("ShotNumber", 1);
    config.CreateAbsolute(".Signals");
    config.CreateRelative("0");
    config.Write("NodeName", "S_uint8");
    config.Write("Type", "uint8");
    config.MoveToAncestor(1u);
    config.CreateRelative("1");
    config.Write("NodeName", "S_int8");
    config.Write("Type", "int8");

    config.CreateAbsolute("Functions");
    config.CreateRelative("0");
    config.CreateRelative("InputSignals");
    config.CreateRelative("0");
    config.Write("Samples", 1);
    config.MoveToAncestor(1u);
    config.CreateRelative("1");
    config.MoveToAncestor(1u);
    config.Write("ByteSize", 10);
    config.MoveToRoot();

    ok = dS.Initialise(config);

    config.MoveToRoot();

    if(ok){
        ok = !dS.SetConfiguredDatabase(config);
    }
    return ok;
}

bool MDSReaderTest::TestSetConfiguredDatabaseInvalidNumberOfElements() {
    CreateTree myTreeCreated(treeName);
    bool ok;
    MDSReader dS;
    ConfigurationDatabase config;
    config.Write("TreeName", treeName.Buffer());
    config.Write("ShotNumber", 1);
    config.CreateAbsolute(".Signals");
    config.CreateRelative("0");
    config.Write("NodeName", "S_uint8");
    config.Write("Type", "uint8");
    config.Write("NumberOfElements", 100);
    config.MoveToAncestor(1u);
    config.CreateRelative("1");
    config.Write("NodeName", "S_int8");
    config.Write("Type", "int8");
    config.Write("NumberOfElements", 0);

    config.CreateAbsolute("Functions");
    config.CreateRelative("0");
    config.CreateRelative("InputSignals");
    config.CreateRelative("0");
    config.Write("Samples", 1);
    config.MoveToAncestor(1u);
    config.CreateRelative("1");
    config.MoveToAncestor(1u);
    config.Write("ByteSize", 10);
    config.MoveToRoot();

    ok = dS.Initialise(config);

    config.MoveToRoot();

    if(ok){
        ok = !dS.SetConfiguredDatabase(config);
    }
    return ok;
}


/*
bool MDSReaderTest::TestInitialiseWrongNodeNames() {
    CreateTree myTreeCreated(treeName);
    bool ok;
    MDSReader dS;
    ConfigurationDatabase config;
    config.Write("TreeName", treeName.Buffer());
    config.Write("ShotNumber", 1);
    config.Write("NodeNames", "S_uint8");
    ok = !dS.Initialise(config);
    return ok;
}

bool MDSReaderTest::TestInitialiseWrongNodeNames2() {
    CreateTree myTreeCreated(treeName);
    bool ok;
    MDSReader dS;
    ConfigurationDatabase config;
    config.Write("TreeName", treeName.Buffer());
    config.Write("ShotNumber", 1);
    config.Write("NodeNames", 1);
    ok = !dS.Initialise(config);
    return ok;
}

bool MDSReaderTest::TestInitialiseWrongNodeNames3() {
    CreateTree myTreeCreated(treeName);
    bool ok;
    MDSReader dS;
    ConfigurationDatabase config;
    config.Write("TreeName", treeName.Buffer());
    config.Write("ShotNumber", 1);
    uint32 numberOfNames = 3u;
    StreamString *names = new StreamString[numberOfNames];
    names[0] = "S_uint32";
    names[1] = "InvalidName";
    names[2] = "S_float32";
    Vector<StreamString> vector(names, numberOfNames);
    config.Write("NodeNames", vector);
    ok = !dS.Initialise(config);
    return ok;
}

bool MDSReaderTest::TestInitialiseInvalidTypeNode() {
    CreateTree myTreeCreated(treeName);
    bool ok;
    MDSReader dS;
    ConfigurationDatabase config;
    config.Write("TreeName", treeName.Buffer());
    config.Write("ShotNumber", 1);
    Vector<StreamString> vector(allNodeNames, numberOfNodes);
    config.Write("NodeNames", vector);
    ok = !dS.Initialise(config);
    return ok;
}

bool MDSReaderTest::TestInitialiseInvalidTypeSpecified() {
    CreateTree myTreeCreated(treeName);
    bool ok;
    MDSReader dS;
    ConfigurationDatabase config;
    config.Write("TreeName", treeName.Buffer());
    config.Write("ShotNumber", 1);
    Vector<StreamString> vector(allValidNodeNames, numberOfValidNodes);
    config.Write("NodeNames", vector);
    StreamString *types = new StreamString[2];
    types[0] = "S_uint8";
    types[1] = "S_int8";
    Vector<StreamString> vector1(types, 2);
    config.Write("SignalTypes", vector1);
    ok = !dS.Initialise(config);
    return ok;
}

bool MDSReaderTest::TestInitialiseInvalidTypeSpecified2() {
    CreateTree myTreeCreated(treeName);
    bool ok;
    MDSReader dS;
    ConfigurationDatabase config;
    config.Write("TreeName", treeName.Buffer());
    config.Write("ShotNumber", 1);
    Vector<StreamString> vector(allValidNodeNames, numberOfValidNodes);
    config.Write("NodeNames", vector);
    StreamString *types = new StreamString[10];
    types[0] = "uint8";
    types[1] = "int8";
    types[2] = "uint16";
    types[3] = "int16";
    types[4] = "uint32";
    types[5] = "int32";
    types[6] = "uint64";
    types[7] = "int32";//wrong type
    types[8] = "flota32";
    types[9] = "flota64";
    Vector<StreamString> vector1(types, 10);
    config.Write("SignalTypes", vector1);
    ok = !dS.Initialise(config);
    return ok;
}
*/
