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

class MDSReaderTestHelper: public MDSReader {
public:
    MDSReaderTestHelper(StreamString name) {
        treeName = name;
        elementsRead = 10;
        sum_uint8 = 0;
        sum_int8 = -20;
        sum_uint16 = 0;
        sum_int16 = -40;
        sum_uint32 = 0;
        sum_int32 = -80;
        sum_uint64 = 0;
        sum_int64 = -160;
        sum_float32 = 0;
        sum_float64 = 0;
    }

    virtual ~MDSReaderTestHelper() {

    }
    bool CreateConfigurationFile(float64 samplingTime = 0.1,
                                 uint8 dataManagement = 0u,
                                 uint8 holeManagement = 0u,
                                 int32 shot = 1) { //DataManagement = 0, HoleManagement = 0
        bool ok = true;
        ok &= config.Write("TreeName", treeName.Buffer());
        ok &= config.Write("ShotNumber", shot);
        float64 CycleFrequnecy = 1 / (samplingTime * elementsRead);
        ok &= config.Write("Frequency", CycleFrequnecy); //0.1 is the sample time of test_tree pulse 1

        ok &= config.CreateAbsolute(".Signals");
        ok &= config.CreateRelative("0");
        ok &= config.Write("NodeName", "S_uint8");
        ok &= config.Write("Type", "uint8");
        ok &= config.Write("NumberOfElements", elementsRead);
        ok &= config.Write("ByteSize", elementsRead * 1);
        ok &= config.Write("DataManagement", dataManagement);
        ok &= config.Write("HoleManagement", holeManagement);
        ok &= config.MoveToAncestor(1u);
        ok &= config.CreateRelative("1");
        ok &= config.Write("NodeName", "S_int8");
        ok &= config.Write("Type", "int8");
        ok &= config.Write("NumberOfElements", elementsRead);
        ok &= config.Write("ByteSize", elementsRead * 1);
        ok &= config.Write("DataManagement", dataManagement);
        ok &= config.Write("HoleManagement", holeManagement);
        ok &= config.MoveToAncestor(1u);
        ok &= config.CreateRelative("2");
        ok &= config.Write("NodeName", "S_uint16");
        ok &= config.Write("Type", "uint16");
        ok &= config.Write("NumberOfElements", elementsRead);
        ok &= config.Write("ByteSize", elementsRead * 2);
        ok &= config.Write("DataManagement", dataManagement);
        ok &= config.Write("HoleManagement", holeManagement);
        ok &= config.MoveToAncestor(1u);
        ok &= config.CreateRelative("3");
        ok &= config.Write("NodeName", "S_int16");
        ok &= config.Write("Type", "int16");
        ok &= config.Write("NumberOfElements", elementsRead);
        ok &= config.Write("ByteSize", elementsRead * 2);
        ok &= config.Write("DataManagement", dataManagement);
        ok &= config.Write("HoleManagement", holeManagement);
        ok &= config.MoveToAncestor(1u);
        ok &= config.CreateRelative("4");
        ok &= config.Write("NodeName", "S_uint32");
        ok &= config.Write("Type", "uint32");
        ok &= config.Write("NumberOfElements", elementsRead);
        ok &= config.Write("ByteSize", elementsRead * 4);
        ok &= config.Write("DataManagement", dataManagement);
        ok &= config.Write("HoleManagement", holeManagement);
        ok &= config.MoveToAncestor(1u);
        ok &= config.CreateRelative("5");
        ok &= config.Write("NodeName", "S_int32");
        ok &= config.Write("Type", "int32");
        ok &= config.Write("NumberOfElements", elementsRead);
        ok &= config.Write("ByteSize", elementsRead * 4);
        ok &= config.Write("DataManagement", dataManagement);
        ok &= config.Write("HoleManagement", holeManagement);
        ok &= config.MoveToAncestor(1u);
        ok &= config.CreateRelative("6");
        ok &= config.Write("NodeName", "S_uint64");
        ok &= config.Write("Type", "uint64");
        ok &= config.Write("NumberOfElements", elementsRead);
        ok &= config.Write("ByteSize", elementsRead * 8);
        ok &= config.Write("DataManagement", dataManagement);
        ok &= config.Write("HoleManagement", holeManagement);
        ok &= config.MoveToAncestor(1u);
        ok &= config.CreateRelative("7");
        ok &= config.Write("NodeName", "S_int64");
        ok &= config.Write("Type", "int64");
        ok &= config.Write("NumberOfElements", elementsRead);
        ok &= config.Write("ByteSize", elementsRead * 8);
        ok &= config.Write("DataManagement", dataManagement);
        ok &= config.Write("HoleManagement", holeManagement);
        ok &= config.MoveToAncestor(1u);
        ok &= config.CreateRelative("8");
        ok &= config.Write("NodeName", "S_float32");
        ok &= config.Write("Type", "float32");
        ok &= config.Write("NumberOfElements", elementsRead);
        ok &= config.Write("ByteSize", elementsRead * 4);
        ok &= config.Write("DataManagement", dataManagement);
        ok &= config.Write("HoleManagement", holeManagement);
        ok &= config.MoveToAncestor(1u);
        ok &= config.CreateRelative("9");
        ok &= config.Write("NodeName", "S_float64");
        ok &= config.Write("Type", "float64");
        ok &= config.Write("NumberOfElements", elementsRead);
        ok &= config.Write("ByteSize", elementsRead * 8);
        ok &= config.Write("DataManagement", dataManagement);
        ok &= config.Write("HoleManagement", holeManagement);
        ok &= config.MoveToAncestor(1u);

        ok &= config.CreateAbsolute("Functions");
        ok &= config.CreateRelative("0");
        ok &= config.CreateRelative("InputSignals");
        ok &= config.CreateRelative("0");
        ok &= config.Write("Samples", 1);
        ok &= config.MoveToAncestor(1u);
        ok &= config.CreateRelative("1");
        ok &= config.MoveToAncestor(1u);
        ok &= config.CreateRelative("2");
        ok &= config.MoveToAncestor(1u);
        ok &= config.CreateRelative("3");
        ok &= config.MoveToAncestor(1u);
        ok &= config.CreateRelative("4");
        ok &= config.MoveToAncestor(1u);
        ok &= config.CreateRelative("5");
        ok &= config.MoveToAncestor(1u);
        ok &= config.CreateRelative("6");
        ok &= config.MoveToAncestor(1u);
        ok &= config.CreateRelative("7");
        ok &= config.MoveToAncestor(1u);
        ok &= config.CreateRelative("8");
        ok &= config.MoveToAncestor(1u);
        ok &= config.CreateRelative("9");
        ok &= config.MoveToAncestor(1u);
        ok &= config.Write("ByteSize", 10);
        ok &= config.MoveToRoot();
        return ok;
    }

    bool CompareS_uint8(uint8 *ptr,
                        uint32 iteration,
                        float64 increment = +1,
                        uint32 elementsToCompare = 0u) {
        bool ok = true;
        for (uint32 i = 0u; (i < elementsToCompare) && ok; i++) {
            ok = (ptr[i] == static_cast<uint8>(round(sum_uint8)));
            if (!ok) {
                printf("uint8 value = %u != ref = %u. iteration = %u, position in the iteration = %d\n", ptr[i], static_cast<uint8>(round(sum_uint8)),
                       iteration, i);
            }
            sum_uint8 += increment;
        }
        return ok;
    }
    bool CompareS_int8(int8 *ptr,
                       uint32 iteration,
                       float64 increment = +1,
                       uint32 elementsToCompare = 0u) {
        bool ok = true;
        for (uint32 i = 0u; (i < elementsToCompare) && ok; i++) {
            ok = ptr[i] == static_cast<int8>(round(sum_int8));
            if (!ok) {
                printf("int8 value = %d != ref = %d. iteration = %u, position in the iteration = %d\n", ptr[i], static_cast<int8>(round(sum_int8)), iteration,
                       i);
            }
            sum_int8 += increment;
        }

        return ok;
    }
    bool CompareS_uint16(uint16 * ptr,
                         uint32 iteration,
                         float64 increment = +2,
                         uint32 elementsToCompare = 0u) {
        bool ok = true;
        for (uint32 i = 0u; (i < elementsToCompare) && ok; i++) {
            ok = ptr[i] == static_cast<uint16>(round(sum_uint16));
            if (!ok) {
                printf("uint16 value = %u != ref = %u. iteration = %u, position in the iteration = %d\n", ptr[i], static_cast<uint16>(round(sum_uint16)),
                       iteration, i);
            }
            sum_uint16 += increment;
        }
        return ok;
    }
    bool CompareS_int16(int16 * ptr,
                        uint32 iteration,
                        float64 increment = +2,
                        uint32 elementsToCompare = 0u) {
        bool ok = true;
        for (uint32 i = 0u; (i < elementsToCompare) && ok; i++) {
            ok = ptr[i] == static_cast<int16>(round(sum_int16));
            if (!ok) {
                printf("int16 value = %d != ref = %d. iteration = %u, position in the iteration = %d\n", ptr[i], static_cast<int16>(round(sum_int16)),
                       iteration, i);
            }
            sum_int16 += increment;
        }
        return ok;
    }
    bool CompareS_uint32(uint32 * ptr,
                         uint32 iteration,
                         float64 increment = +4,
                         uint32 elementsToCompare = 0u) {
        bool ok = true;
        for (uint32 i = 0u; (i < elementsToCompare) && ok; i++) {
            ok = ptr[i] == static_cast<uint32>(round(sum_uint32));
            if (!ok) {
                printf("uint32 value = %u != ref = %u. iteration = %u, position in the iteration = %d\n", ptr[i], static_cast<uint32>(round(sum_uint32)),
                       iteration, i);
            }
            sum_uint32 += increment;
        }
        return ok;
    }
    bool CompareS_int32(int32 * ptr,
                        uint32 iteration,
                        float64 increment = +4,
                        uint32 elementsToCompare = 0u) {
        bool ok = true;
        for (uint32 i = 0u; (i < elementsToCompare) && ok; i++) {
            ok = ptr[i] == static_cast<int32>(round(sum_int32));
            if (!ok) {
                printf("int32 value = %d != ref = %d. iteration = %u, position in the iteration = %d\n", ptr[i], static_cast<int32>(round(sum_int32)),
                       iteration, i);
            }
            sum_int32 += increment;
        }
        return ok;
    }
    bool CompareS_uint64(uint64 * ptr,
                         uint32 iteration,
                         float64 increment = +8,
                         uint32 elementsToCompare = 0u) {
        bool ok = true;
        for (uint32 i = 0u; (i < elementsToCompare) && ok; i++) {
            ok = ptr[i] == static_cast<uint64>(round(sum_uint64));
            if (!ok) {
                printf("uint64 value = %llu != ref = %llu. iteration = %u, position in the iteration = %d\n", ptr[i], static_cast<uint64>(round(sum_uint64)),
                       iteration, i);
            }
            sum_uint64 += increment;
        }
        return ok;
    }
    bool CompareS_int64(int64 * ptr,
                        uint32 iteration,
                        float64 increment = +8,
                        uint32 elementsToCompare = 0u) {
        bool ok = true;
        for (uint32 i = 0u; (i < elementsToCompare) && ok; i++) {
//            ok = ptr[i] == static_cast<int64>(round(sum_int64));
            if (!ok) {
                printf("int64 value = %lld!= ref = %lld. iteration = %u, position in the iteration = %d\n", ptr[i], static_cast<uint64>(round(sum_uint64)),
                       iteration, i);
            }
            sum_int64 += increment;
        }
        return ok;
    }
    bool CompareS_float32(float32 * ptr,
                          uint32 iteration,
                          float32 increment = 0.1,
                          uint32 elementsToCompare = 0u) {
        bool ok = true;
        for (uint32 i = 0u; (i < elementsToCompare) && ok; i++) {
            ok = IsEqualLargeMargins(ptr[i], sum_float32);
            if (!ok) {
                printf("float32 value = %.15f != ref = %.15f, iteration = %u, position in the iteration = %u\n", ptr[i], sum_float32, iteration, i);
            }
            sum_float32 += increment;
        }
        return ok;
    }
    bool CompareS_float64(float64 * ptr,
                          uint32 iteration,
                          float64 increment = 0.1,
                          uint32 elementsToCompare = 0u) {
        bool ok = true;
        for (uint32 i = 0u; (i < elementsToCompare) && ok; i++) {
            ok = IsEqualLargeMargins(ptr[i], sum_float64);
            if (!ok) {
                printf("float64 value = %.17lf != ref = %.17lf, iteration = %u, position in the iteration = %u\n", ptr[i], sum_float64, iteration, i);
            }
            sum_float64 += increment;
        }
        return ok;
    }

    template<typename T>
    bool CompareIntegerValue(T *ptr,
                             uint32 iteration,
                             T value,
                             uint32 elementsToCompare) {
        bool ok = true;
        for (uint32 i = 0u; (i < elementsToCompare) && ok; i++) {
            ok = (ptr[i] == value);
            if (!ok) {
                printf("iteration = %u, position in the iteration = %u\n", iteration, i);
            }
        }
        return ok;
    }

    template<typename T>
    bool CompareFloatPointValue(T *ptr,
                                uint32 iteration,
                                T value,
                                uint32 elementsToCompare) {
        bool ok = true;
        for (uint32 i = 0u; (i < elementsToCompare) && ok; i++) {
            ok = IsEqualLargeMargins(ptr[i], value);

            if (!ok) {
                printf("iteration = %u, position in the iteration = %u\n", iteration, i);
            }
        }
        return ok;
    }
    bool IsEqualLargeMargins(float32 f1,
                             float32 f2) {
        bool ret;
        float32 margin = 0.00005;
        float32 diff = f1 - f2;
        ret = (diff < margin) && (diff > -margin);
        return ret;
    }

    bool IsEqualLargeMargins(float64 f1,
                             float64 f2) {
        bool ret;
        float64 margin = 0.0000005;
        float64 diff = f1 - f2;
        ret = (diff < margin) && (diff > -margin);
        return ret;
    }
    ConfigurationDatabase config;
    uint32 elementsRead;

private:
    StreamString treeName;
    float64 sum_uint8;
    float64 sum_int8;
    float64 sum_uint16;
    float64 sum_int16;
    float64 sum_uint32;
    float64 sum_int32;
    float64 sum_uint64;
    float64 sum_int64;
    float32 sum_float32;
    float64 sum_float64;
};

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
 * A pulse 1 and 2 are created filling the SIGNAL nodes with ramps (with different step increments).
 * The environmental variable test_tree_path is automatically created in the scope of the tests.
 * In each test a tree with two poses are created and then are removed.
 */
class CreateTree {
public:
    CreateTree(StreamString name) {
        treeName = name;
        pulseNumber = 1;
        numberOfElementsPerSeg = 20; //400000000;
        numberOfSegments = 100; //100;
        elapsedTimeSegment = 2;
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

        //1 segment with data and the other empty
        pulseNumber++;
        CreatePulseNumber(pulseNumber);
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
        CreatePulseNumber(pulseNumber);
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
        CreatePulseNumber(pulseNumber);
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

    void AddSignal_uint8(bool toggle = false,
                         bool initialTrigger = true,
                         uint32 segmentsOn = 1u) {
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

    void AddSignal_int8(bool toggle = false,
                        bool initialTrigger = true,
                        uint32 segmentsOn = 1u) {
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

    void AddSignal_uint16(bool toggle = false,
                          bool initialTrigger = true,
                          uint32 segmentsOn = 1u) {
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

    void AddSignal_int16(bool toggle = false,
                         bool initialTrigger = true,
                         uint32 segmentsOn = 1u) {
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

    void AddSignal_uint32(bool toggle = false,
                          bool initialTrigger = true,
                          uint32 segmentsOn = 1u) {
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

    void AddSignal_int32(bool toggle = false,
                         bool initialTrigger = true,
                         uint32 segmentsOn = 1u) {
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

    void AddSignal_uint64(bool toggle = false,
                          bool initialTrigger = true,
                          uint32 segmentsOn = 1u) {
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

    void AddSignal_int64(bool toggle = false,
                         bool initialTrigger = true,
                         uint32 segmentsOn = 1u) {
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

    void AddSignal_float32(bool toggle = false,
                           bool initialTrigger = true,
                           uint32 segmentsOn = 1u) {
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

    void AddSignal_float64(bool toggle = false,
                           bool initialTrigger = true,
                           uint32 segmentsOn = 1u) {
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

    void AddSignal_Info(bool toggle = false) {
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
    config.Write("Frequency", 1000.0);
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
    if (ok) {
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
    config.Write("Frequency", 1000.0);
    config.CreateAbsolute(".Signals");
    config.MoveToRoot();

    ok = dS.Initialise(config);
    config.MoveToRoot();

    if (ok) {
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
    config.Write("Frequency", 1000.0);
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
    if (ok) {
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
    config.Write("Frequency", 1000.0);
    config.CreateAbsolute(".Signals");
    config.CreateRelative("0");
    config.MoveToAncestor(1u);
    config.CreateRelative("1");
    config.CreateAbsolute("Functions");
    config.CreateRelative("0");
    config.MoveToRoot();

    ok = dS.Initialise(config);

    config.MoveToRoot();
    if (ok) {
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
    config.Write("Frequency", 1000.0);
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
    if (ok) {
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
    config.Write("Frequency", 1000.0);
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
    if (ok) {
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
    config.Write("Frequency", 1000.0);
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

    if (ok) {
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
    config.Write("Frequency", 1000.0);
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

    if (ok) {
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
    config.Write("Frequency", 1000.0);
    config.CreateAbsolute(".Signals");
    config.CreateRelative("0");
    config.Write("NodeName", "S_uint8");
    config.MoveToAncestor(1u);
    config.CreateRelative("1");
    config.Write("NodeName", "InvalidNodeName"); //invalid name

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

    if (ok) {
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
    config.Write("Frequency", 1000.0);
    config.CreateAbsolute(".Signals");
    config.CreateRelative("0");
    config.Write("NodeName", "S_uint8");
    config.MoveToAncestor(1u);
    config.CreateRelative("1");
    config.Write("NodeName", "S_uint8"); //repeated

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

    if (ok) {
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
    config.Write("Frequency", 1000.0);
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

    if (ok) {
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
    config.Write("Frequency", 1000.0);
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

    if (ok) {
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
    config.Write("Frequency", 1000.0);
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

    if (ok) {
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
    config.Write("Frequency", 1000.0);
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

    if (ok) {
        ok = !dS.SetConfiguredDatabase(config);
    }
    return ok;
}

bool MDSReaderTest::TestSetConfiguredDatabaseInvalidBytesSize() {
    CreateTree myTreeCreated(treeName);
    bool ok;
    MDSReader dS;
    ConfigurationDatabase config;
    config.Write("TreeName", treeName.Buffer());
    config.Write("ShotNumber", 1);
    config.Write("Frequency", 1000.0);
    config.CreateAbsolute(".Signals");
    config.CreateRelative("0");
    config.Write("NodeName", "S_uint8");
    config.Write("Type", "uint8");
    config.Write("NumberOfElements", 100);
    config.MoveToAncestor(1u);
    config.CreateRelative("1");
    config.Write("NodeName", "S_int8");
    config.Write("Type", "int8");
    config.Write("NumberOfElements", 100);

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

    if (ok) {
        ok = !dS.SetConfiguredDatabase(config);
    }
    return ok;
}

bool MDSReaderTest::TestSetConfiguredDatabaseNoDataManagement() {
    CreateTree myTreeCreated(treeName);
    bool ok;
    MDSReader dS;
    ConfigurationDatabase config;
    config.Write("TreeName", treeName.Buffer());
    config.Write("ShotNumber", 1);
    config.Write("Frequency", 1000.0);
    config.CreateAbsolute(".Signals");
    config.CreateRelative("0");
    config.Write("NodeName", "S_uint8");
    config.Write("Type", "uint8");
    config.Write("NumberOfElements", 100);
    config.Write("ByteSize", 100);

    config.Write("HoleManagement", 0);
    config.MoveToAncestor(1u);
    config.CreateRelative("1");
    config.Write("NodeName", "S_int8");
    config.Write("Type", "int8");
    config.Write("NumberOfElements", 100);
    config.Write("ByteSize", 100);
    config.Write("DataManagement", 2);
    config.Write("HoleManagement", 0);

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

    if (ok) {
        ok = !dS.SetConfiguredDatabase(config);
    }
    return ok;
}

bool MDSReaderTest::TestSetConfiguredDatabaseInvalidDataManagement() {
    CreateTree myTreeCreated(treeName);
    bool ok;
    MDSReader dS;
    ConfigurationDatabase config;
    config.Write("TreeName", treeName.Buffer());
    config.Write("ShotNumber", 1);
    config.Write("Frequency", 1000.0);
    config.CreateAbsolute(".Signals");
    config.CreateRelative("0");
    config.Write("NodeName", "S_uint8");
    config.Write("Type", "uint8");
    config.Write("NumberOfElements", 100);
    config.Write("ByteSize", 100);
    config.Write("DataManagement", 4);
    config.Write("HoleManagement", 0);
    config.MoveToAncestor(1u);
    config.CreateRelative("1");
    config.Write("NodeName", "S_int8");
    config.Write("Type", "int8");
    config.Write("NumberOfElements", 100);
    config.Write("ByteSize", 100);
    config.Write("DataManagement", 2);
    config.Write("HoleManagement", 0);

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

    if (ok) {
        ok = !dS.SetConfiguredDatabase(config);
    }
    return ok;
}

bool MDSReaderTest::TestSetConfiguredDatabaseDataManagement0() {
    CreateTree myTreeCreated(treeName);
    bool ok;
    MDSReader dS;
    ConfigurationDatabase config;
    config.Write("TreeName", treeName.Buffer());
    config.Write("ShotNumber", 1);
    config.Write("Frequency", 1);
    config.CreateAbsolute(".Signals");
    config.CreateRelative("0");
    config.Write("NodeName", "S_uint8");
    config.Write("Type", "uint8");
    config.Write("NumberOfElements", 400);
    config.Write("ByteSize", 400);
    config.Write("DataManagement", 0);
    config.Write("HoleManagement", 0);
    config.MoveToAncestor(1u);
    config.CreateRelative("1");
    config.Write("NodeName", "S_int8");
    config.Write("Type", "int8");
    config.Write("NumberOfElements", 100);
    config.Write("ByteSize", 100);
    config.Write("DataManagement", 0);
    config.Write("HoleManagement", 0);

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

    if (ok) {
        ok = !dS.SetConfiguredDatabase(config);
    }
    return ok;
}

bool MDSReaderTest::TestSetConfiguredDatabaseNoHoleManagement() {
    CreateTree myTreeCreated(treeName);
    bool ok;
    MDSReader dS;
    ConfigurationDatabase config;
    config.Write("TreeName", treeName.Buffer());
    config.Write("ShotNumber", 1);
    config.Write("Frequency", 1000.0);
    config.CreateAbsolute(".Signals");
    config.CreateRelative("0");
    config.Write("NodeName", "S_uint8");
    config.Write("Type", "uint8");
    config.Write("NumberOfElements", 100);
    config.Write("ByteSize", 100);
    config.Write("DataManagement", 2);

    config.MoveToAncestor(1u);
    config.CreateRelative("1");
    config.Write("NodeName", "S_int8");
    config.Write("Type", "int8");
    config.Write("NumberOfElements", 100);
    config.Write("ByteSize", 100);
    config.Write("DataManagement", 2);
    config.Write("HoleManagement", 0);

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

    if (ok) {
        ok = !dS.SetConfiguredDatabase(config);
    }
    return ok;
}

bool MDSReaderTest::TestSetConfiguredDatabaseInvalidHoleManagement() {
    CreateTree myTreeCreated(treeName);
    bool ok;
    MDSReader dS;
    ConfigurationDatabase config;
    config.Write("TreeName", treeName.Buffer());
    config.Write("ShotNumber", 1);
    config.Write("Frequency", 1000.0);
    config.CreateAbsolute(".Signals");
    config.CreateRelative("0");
    config.Write("NodeName", "S_uint8");
    config.Write("Type", "uint8");
    config.Write("NumberOfElements", 100);
    config.Write("ByteSize", 100);
    config.Write("DataManagement", 2);
    config.Write("HoleManagement", 2);
    config.MoveToAncestor(1u);
    config.CreateRelative("1");
    config.Write("NodeName", "S_int8");
    config.Write("Type", "int8");
    config.Write("NumberOfElements", 100);
    config.Write("ByteSize", 100);
    config.Write("DataManagement", 2);
    config.Write("HoleManagement", 0);

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

    if (ok) {
        ok = !dS.SetConfiguredDatabase(config);
    }
    return ok;
}

bool MDSReaderTest::TestSetConfiguredDatabase() {
    CreateTree myTreeCreated(treeName);
    bool ok;
    MDSReader dS;
    ConfigurationDatabase config;
    config.Write("TreeName", treeName.Buffer());
    config.Write("ShotNumber", 1);
    config.Write("Frequency", 1000.0);
    config.CreateAbsolute(".Signals");
    config.CreateRelative("0");
    config.Write("NodeName", "S_uint8");
    config.Write("Type", "uint8");
    config.Write("NumberOfElements", 100);
    config.Write("ByteSize", 100);
    config.Write("DataManagement", 1);
    config.Write("HoleManagement", 0);
    config.MoveToAncestor(1u);
    config.CreateRelative("1");
    config.Write("NodeName", "S_int8");
    config.Write("Type", "int8");
    config.Write("NumberOfElements", 100);
    config.Write("ByteSize", 100);
    config.Write("DataManagement", 2);
    config.Write("HoleManagement", 0);

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

    if (ok) {
        ok = dS.SetConfiguredDatabase(config);
    }
    return ok;
}

bool MDSReaderTest::TestPrepareNextState() {
    MDSReader dS;
    return dS.PrepareNextState("state1", "state2");
}

bool MDSReaderTest::TestAllocateMemory() {
    MDSReader dS;
    return dS.AllocateMemory();
}

bool MDSReaderTest::TestGetNumberOfMemoryBuffers() {
    MDSReader dS;
    return dS.GetNumberOfMemoryBuffers();
}

bool MDSReaderTest::TestGetSignalMemoryBufferNoMemory() {
    MDSReader dS;
    void *ptr = NULL_PTR(void *);
    return (!dS.GetSignalMemoryBuffer(1, 0, ptr));
}

bool MDSReaderTest::TestGetSignalMemoryBuffer() {
    MDSReaderTestHelper dS(treeName);
    bool ok;
    ok = dS.CreateConfigurationFile();
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void *ptr = NULL_PTR(void *);
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr);
        if (ok) {
            ok = (ptr != NULL_PTR(void *));
        }
    }
    return ok;
}

bool MDSReaderTest::Test() {
    char8 *ptr = new char8[8];
    for (uint32 i = 0u; i < 8; i += 4) {
        ptr[i] = 0;
        ptr[i + 1] = 1;
        ptr[i + 2] = 0;
        ptr[i + 3] = 0;
    }
    printf("ptr[0] = %d, ptr[1] = %d\n", ((int32 *) ptr)[0], ((int32 *) ptr)[1]);

    MemoryOperationsHelper::Set(reinterpret_cast<void *>(ptr), 4, 8u);
    printf("ptr[0] = %d, ptr[1] = %d\n", ((int32 *) ptr)[0], ((int32 *) ptr)[1]);
    for (uint32 i = 0u; i < 10; i++) {
        std::cout << "ptr[0] = " << ptr[i] << std::endl;
    }
    for (uint32 i = 0u; i < 10; i++) {
        printf("ptr[%d] = %c\n", i, ptr[i]);
    }
    printf("\n%d, %d\n", static_cast<uint32>(3.1), static_cast<uint32>(3.99));
    return false;

}

bool MDSReaderTest::TestSynchronise() {
    MDSReaderTestHelper dS(treeName);
    bool ok;
    ok = dS.CreateConfigurationFile();
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    if (ok) {
        for (uint32 i = 0u; (i < 20) && ok; i++) {
            ok = dS.Synchronise();
            if (ok) {
                ok &= dS.CompareS_uint8(((uint8 *) ptr[0]), i, dS.elementsRead);
                ok &= dS.CompareS_int8(((int8 *) ptr[1]), i, dS.elementsRead);
                ok &= dS.CompareS_uint16(((uint16 *) ptr[2]), i, dS.elementsRead);
                ok &= dS.CompareS_int16(((int16 *) ptr[3]), i, dS.elementsRead);
                ok &= dS.CompareS_uint32(((uint32 *) ptr[4]), i, dS.elementsRead);
                ok &= dS.CompareS_int32(((int32 *) ptr[5]), i, dS.elementsRead);
                ok &= dS.CompareS_uint64(((uint64 *) ptr[6]), i, dS.elementsRead);
                ok &= dS.CompareS_int64(((int64 *) ptr[7]), i, dS.elementsRead);
                ok &= dS.CompareS_float32(((float32 *) ptr[8]), i, dS.elementsRead);
                ok &= dS.CompareS_float64(((float64 *) ptr[9]), i, dS.elementsRead);
            }
        }
    }
    delete[] ptr;
    return ok;
}

bool MDSReaderTest::TestSynchronise1() {
    MDSReaderTestHelper dS(treeName);
    bool ok;
    dS.elementsRead = 20;
    ok = dS.CreateConfigurationFile();
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    if (ok) { //last read not verified
        for (uint32 i = 0u; (i < 10) && ok; i++) {
            ok = dS.Synchronise();
            /*
             for(uint32 j = 0u; j <10; j++) {
             printf("%d\n", ((int8 *)ptr[1])[j]);
             }
             */
            if (ok) {
                ok &= dS.CompareS_uint8(((uint8 *) ptr[0]), i, dS.elementsRead);
                ok &= dS.CompareS_int8(((int8 *) ptr[1]), i, dS.elementsRead);
                ok &= dS.CompareS_uint16(((uint16 *) ptr[2]), i, dS.elementsRead);
                ok &= dS.CompareS_int16(((int16 *) ptr[3]), i, dS.elementsRead);
                ok &= dS.CompareS_uint32(((uint32 *) ptr[4]), i, dS.elementsRead);
                ok &= dS.CompareS_int32(((int32 *) ptr[5]), i, dS.elementsRead);
                ok &= dS.CompareS_uint64(((uint64 *) ptr[6]), i, dS.elementsRead);
                ok &= dS.CompareS_int64(((int64 *) ptr[7]), i, dS.elementsRead);
                ok &= dS.CompareS_float32(((float32 *) ptr[8]), i, dS.elementsRead);
                ok &= dS.CompareS_float64(((float64 *) ptr[9]), i, dS.elementsRead);
            }
        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise2() {
    MDSReaderTestHelper dS(treeName);
    bool ok;
    dS.elementsRead = 40;
    ok = dS.CreateConfigurationFile();
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    if (ok) { //last read not verified
        for (uint32 i = 0u; (i < 5) && ok; i++) {
            ok = dS.Synchronise();
            if (ok) {
                ok &= dS.CompareS_uint8(((uint8 *) ptr[0]), i, dS.elementsRead);
                ok &= dS.CompareS_int8(((int8 *) ptr[1]), i, dS.elementsRead);
                ok &= dS.CompareS_uint16(((uint16 *) ptr[2]), i, dS.elementsRead);
                ok &= dS.CompareS_int16(((int16 *) ptr[3]), i, dS.elementsRead);
                ok &= dS.CompareS_uint32(((uint32 *) ptr[4]), i, dS.elementsRead);
                ok &= dS.CompareS_int32(((int32 *) ptr[5]), i, dS.elementsRead);
                ok &= dS.CompareS_uint64(((uint64 *) ptr[6]), i, dS.elementsRead);
                ok &= dS.CompareS_int64(((int64 *) ptr[7]), i, dS.elementsRead);
                ok &= dS.CompareS_float32(((float32 *) ptr[8]), i, dS.elementsRead);
                ok &= dS.CompareS_float64(((float64 *) ptr[9]), i, dS.elementsRead);
            }
        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise3() {
    MDSReaderTestHelper dS(treeName);
    bool ok;
    dS.elementsRead = 15;
    ok = dS.CreateConfigurationFile();
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    if (ok) { //last read not verified
        for (uint32 i = 0u; (i < 5) && ok; i++) {
            ok = dS.Synchronise();
            if (ok) {
                ok &= dS.CompareS_uint8(((uint8 *) ptr[0]), i, dS.elementsRead);
                ok &= dS.CompareS_int8(((int8 *) ptr[1]), i, dS.elementsRead);
                ok &= dS.CompareS_uint16(((uint16 *) ptr[2]), i, dS.elementsRead);
                ok &= dS.CompareS_int16(((int16 *) ptr[3]), i, dS.elementsRead);
                ok &= dS.CompareS_uint32(((uint32 *) ptr[4]), i, dS.elementsRead);
                ok &= dS.CompareS_int32(((int32 *) ptr[5]), i, dS.elementsRead);
                ok &= dS.CompareS_uint64(((uint64 *) ptr[6]), i, dS.elementsRead);
                ok &= dS.CompareS_int64(((int64 *) ptr[7]), i, dS.elementsRead);
                ok &= dS.CompareS_float32(((float32 *) ptr[8]), i, dS.elementsRead);
                ok &= dS.CompareS_float64(((float64 *) ptr[9]), i, dS.elementsRead);
            }
        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise4() {
    MDSReaderTestHelper dS(treeName);
    bool ok;
    ok = dS.CreateConfigurationFile(0.2, 1, 0);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    if (ok) { //last read not verified
        for (uint32 i = 0u; (i < 5) && ok; i++) {
            ok = dS.Synchronise();
            if (ok) {
                ok &= dS.CompareS_uint8(((uint8 *) ptr[0]), i, 2, dS.elementsRead);
                ok &= dS.CompareS_int8(((int8 *) ptr[1]), i, 2, dS.elementsRead);
                ok &= dS.CompareS_uint16(((uint16 *) ptr[2]), i, 4, dS.elementsRead);
                ok &= dS.CompareS_int16(((int16 *) ptr[3]), i, 4, dS.elementsRead);
                ok &= dS.CompareS_uint32(((uint32 *) ptr[4]), i, 8, dS.elementsRead);
                ok &= dS.CompareS_int32(((int32 *) ptr[5]), i, 8, dS.elementsRead);
                ok &= dS.CompareS_uint64(((uint64 *) ptr[6]), i, 16, dS.elementsRead);
                ok &= dS.CompareS_int64(((int64 *) ptr[7]), i, 16, dS.elementsRead);
                ok &= dS.CompareS_float32(((float32 *) ptr[8]), i, 0.2, dS.elementsRead);
                ok &= dS.CompareS_float64(((float64 *) ptr[9]), i, 0.2, dS.elementsRead);
            }
        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise5() {
    MDSReaderTestHelper dS(treeName);
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 1, 0);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    if (ok) { //last read not verified
        for (uint32 i = 0u; (i < 5) && ok; i++) {
            ok = dS.Synchronise();
            if (ok) {
                ok &= dS.CompareS_uint8(((uint8 *) ptr[0]), i, 1, dS.elementsRead);
                ok &= dS.CompareS_int8(((int8 *) ptr[1]), i, 1, dS.elementsRead);
                ok &= dS.CompareS_uint16(((uint16 *) ptr[2]), i, 2, dS.elementsRead);
                ok &= dS.CompareS_int16(((int16 *) ptr[3]), i, 2, dS.elementsRead);
                ok &= dS.CompareS_uint32(((uint32 *) ptr[4]), i, 4, dS.elementsRead);
                ok &= dS.CompareS_int32(((int32 *) ptr[5]), i, 4, dS.elementsRead);
                ok &= dS.CompareS_uint64(((uint64 *) ptr[6]), i, 8, dS.elementsRead);
                ok &= dS.CompareS_int64(((int64 *) ptr[7]), i, 8, dS.elementsRead);
                ok &= dS.CompareS_float32(((float32 *) ptr[8]), i, 0.1, dS.elementsRead);
                ok &= dS.CompareS_float64(((float64 *) ptr[9]), i, 0.1, dS.elementsRead);
            }
        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise6() {
    MDSReaderTestHelper dS(treeName);
    bool ok;
    ok = dS.CreateConfigurationFile(0.08, 1, 0);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    if (ok) { //last read not verified
        for (uint32 i = 0u; (i < 5) && ok; i++) {
            ok = dS.Synchronise();
            if (ok) {
                ok &= dS.CompareS_uint8(((uint8 *) ptr[0]), i, 0.8, dS.elementsRead);
                ok &= dS.CompareS_int8(((int8 *) ptr[1]), i, 0.8, dS.elementsRead);
                ok &= dS.CompareS_uint16(((uint16 *) ptr[2]), i, 1.6, dS.elementsRead);
                ok &= dS.CompareS_int16(((int16 *) ptr[3]), i, 1.6, dS.elementsRead);
                ok &= dS.CompareS_uint32(((uint32 *) ptr[4]), i, 3.2, dS.elementsRead);
                ok &= dS.CompareS_int32(((int32 *) ptr[5]), i, 3.2, dS.elementsRead);
                ok &= dS.CompareS_uint64(((uint64 *) ptr[6]), i, 6.4, dS.elementsRead);
                ok &= dS.CompareS_int64(((int64 *) ptr[7]), i, 6.4, dS.elementsRead);
                ok &= dS.CompareS_float32(((float32 *) ptr[8]), i, 0.08, dS.elementsRead);
                ok &= dS.CompareS_float64(((float64 *) ptr[9]), i, 0.08, dS.elementsRead);
            }
        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise7() {
    MDSReaderTestHelper dS(treeName);
    bool ok;
    dS.elementsRead = 32;
    ok = dS.CreateConfigurationFile(0.08, 1, 0);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    if (ok) { //last read not verified
        for (uint32 i = 0u; (i < 5) && ok; i++) {
            ok = dS.Synchronise();
            if (ok) {
                ok &= dS.CompareS_uint8(((uint8 *) ptr[0]), i, 0.8, dS.elementsRead);
                ok &= dS.CompareS_int8(((int8 *) ptr[1]), i, 0.8, dS.elementsRead);
                ok &= dS.CompareS_uint16(((uint16 *) ptr[2]), i, 1.6, dS.elementsRead);
                ok &= dS.CompareS_int16(((int16 *) ptr[3]), i, 1.6, dS.elementsRead);
                ok &= dS.CompareS_uint32(((uint32 *) ptr[4]), i, 3.2, dS.elementsRead);
                ok &= dS.CompareS_int32(((int32 *) ptr[5]), i, 3.2, dS.elementsRead);
                ok &= dS.CompareS_uint64(((uint64 *) ptr[6]), i, 6.4, dS.elementsRead);
                ok &= dS.CompareS_int64(((int64 *) ptr[7]), i, 6.4, dS.elementsRead);
                ok &= dS.CompareS_float32(((float32 *) ptr[8]), i, 0.08, dS.elementsRead);
                ok &= dS.CompareS_float64(((float64 *) ptr[9]), i, 0.08, dS.elementsRead);
            }
        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise8() {
    MDSReaderTestHelper dS(treeName);
    bool ok;
    ok = dS.CreateConfigurationFile(0.2, 2, 0);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    if (ok) { //last read not verified
        for (uint32 i = 0u; (i < 10) && ok; i++) {
            ok = dS.Synchronise();
            if (ok) {
                ok &= dS.CompareS_uint8(((uint8 *) ptr[0]), i, 2, dS.elementsRead);
                ok &= dS.CompareS_int8(((int8 *) ptr[1]), i, 2, dS.elementsRead);
                ok &= dS.CompareS_uint16(((uint16 *) ptr[2]), i, 4, dS.elementsRead);
                ok &= dS.CompareS_int16(((int16 *) ptr[3]), i, 4, dS.elementsRead);
                ok &= dS.CompareS_uint32(((uint32 *) ptr[4]), i, 8, dS.elementsRead);
                ok &= dS.CompareS_int32(((int32 *) ptr[5]), i, 8, dS.elementsRead);
                ok &= dS.CompareS_uint64(((uint64 *) ptr[6]), i, 16, dS.elementsRead);
                ok &= dS.CompareS_int64(((int64 *) ptr[7]), i, 16, dS.elementsRead);
                ok &= dS.CompareS_float32(((float32 *) ptr[8]), i, 0.2, dS.elementsRead);
                ok &= dS.CompareS_float64(((float64 *) ptr[9]), i, 0.2, dS.elementsRead);
            }
        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise9() {
    MDSReaderTestHelper dS(treeName);
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 2, 0);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    if (ok) { //last read not verified
        for (uint32 i = 0u; (i < 10) && ok; i++) {
            ok = dS.Synchronise();
            if (ok) {
                ok &= dS.CompareS_uint8(((uint8 *) ptr[0]), i, 1, dS.elementsRead);
                ok &= dS.CompareS_int8(((int8 *) ptr[1]), i, 1, dS.elementsRead);
                ok &= dS.CompareS_uint16(((uint16 *) ptr[2]), i, 2, dS.elementsRead);
                ok &= dS.CompareS_int16(((int16 *) ptr[3]), i, 2, dS.elementsRead);
                ok &= dS.CompareS_uint32(((uint32 *) ptr[4]), i, 4, dS.elementsRead);
                ok &= dS.CompareS_int32(((int32 *) ptr[5]), i, 4, dS.elementsRead);
                ok &= dS.CompareS_uint64(((uint64 *) ptr[6]), i, 8, dS.elementsRead);
                ok &= dS.CompareS_int64(((int64 *) ptr[7]), i, 8, dS.elementsRead);
                ok &= dS.CompareS_float32(((float32 *) ptr[8]), i, 0.1, dS.elementsRead);
                ok &= dS.CompareS_float64(((float64 *) ptr[9]), i, 0.1, dS.elementsRead);
            }
        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise10() {
    MDSReaderTestHelper dS(treeName);
    dS.elementsRead = 32;
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 2, 0);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    if (ok) { //last read not verified
        for (uint32 i = 0u; (i < 10) && ok; i++) {
            ok = dS.Synchronise();
            if (ok) {
                ok &= dS.CompareS_uint8(((uint8 *) ptr[0]), i, 1, dS.elementsRead);
                ok &= dS.CompareS_int8(((int8 *) ptr[1]), i, 1, dS.elementsRead);
                ok &= dS.CompareS_uint16(((uint16 *) ptr[2]), i, 2, dS.elementsRead);
                ok &= dS.CompareS_int16(((int16 *) ptr[3]), i, 2, dS.elementsRead);
                ok &= dS.CompareS_uint32(((uint32 *) ptr[4]), i, 4, dS.elementsRead);
                ok &= dS.CompareS_int32(((int32 *) ptr[5]), i, 4, dS.elementsRead);
                ok &= dS.CompareS_uint64(((uint64 *) ptr[6]), i, 8, dS.elementsRead);
                ok &= dS.CompareS_int64(((int64 *) ptr[7]), i, 8, dS.elementsRead);
                ok &= dS.CompareS_float32(((float32 *) ptr[8]), i, 0.1, dS.elementsRead);
                ok &= dS.CompareS_float64(((float64 *) ptr[9]), i, 0.1, dS.elementsRead);
            }
        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise11() {
    MDSReaderTestHelper dS(treeName);
    dS.elementsRead = 15;
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 0, 0, 2);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    uint interation = 0u;
    if (ok) { //last read not verified
        for (uint32 i = 0u; (i < 1) && ok; i++) {
            ok = dS.Synchronise();
            if (ok) {
                ok &= dS.CompareS_uint8(((uint8 *) ptr[0]), i, 1, dS.elementsRead);
                ok &= dS.CompareS_int8(((int8 *) ptr[1]), i, 1, dS.elementsRead);
                ok &= dS.CompareS_uint16(((uint16 *) ptr[2]), i, 2, dS.elementsRead);
                ok &= dS.CompareS_int16(((int16 *) ptr[3]), i, 2, dS.elementsRead);
                ok &= dS.CompareS_uint32(((uint32 *) ptr[4]), i, 4, dS.elementsRead);
                ok &= dS.CompareS_int32(((int32 *) ptr[5]), i, 4, dS.elementsRead);
                ok &= dS.CompareS_uint64(((uint64 *) ptr[6]), i, 8, dS.elementsRead);
                ok &= dS.CompareS_int64(((int64 *) ptr[7]), i, 8, dS.elementsRead);
                ok &= dS.CompareS_float32(((float32 *) ptr[8]), i, 0.1, dS.elementsRead);
                ok &= dS.CompareS_float64(((float64 *) ptr[9]), i, 0.1, dS.elementsRead);
            }
        }
    }
    if (ok) {
        ok = dS.Synchronise();
        interation++;
        if (ok) {
            ok &= dS.CompareS_uint8(((uint8 *) ptr[0]), interation, 1, 5);
            ok &= dS.CompareS_int8(((int8 *) ptr[1]), interation, 1, 5);
            ok &= dS.CompareS_uint16(((uint16 *) ptr[2]), interation, 2, 5);
            ok &= dS.CompareS_int16(((int16 *) ptr[3]), interation, 2, 5);
            ok &= dS.CompareS_uint32(((uint32 *) ptr[4]), interation, 4, 5);
            ok &= dS.CompareS_int32(((int32 *) ptr[5]), interation, 4, 5);
            ok &= dS.CompareS_uint64(((uint64 *) ptr[6]), 1, 8, 5);
            ok &= dS.CompareS_int64(((int64 *) ptr[7]), interation, 8, 5);
            ok &= dS.CompareS_float32(((float32 *) ptr[8]), interation, 0.1, 5);
            ok &= dS.CompareS_float64(((float64 *) ptr[9]), interation, 0.1, 5);
        }
        for (uint32 i = 6u; (i < 15) && ok; i++) {
            ok &= (((uint8 *) ptr[0])[i] == 0);
            ok &= (((int8 *) ptr[1])[i] == 0);
            ok &= (((uint16 *) ptr[2])[i] == 0);
            ok &= (((int16 *) ptr[3])[i] == 0);
            ok &= (((uint32 *) ptr[4])[i] == 0);
            ok &= (((int32 *) ptr[5])[i] == 0);
            ok &= (((uint64 *) ptr[6])[i] == 0);
            ok &= (((int64 *) ptr[7])[i] == 0);
            ok &= (((float32 *) ptr[8])[i] == 0);
            ok &= (((float64 *) ptr[9])[i] == 0);
        }
    }
    if (ok) {
        ok = dS.Synchronise();
        interation++;
        for (uint32 i = 0u; (i < 10) && ok; i++) {
            ok &= (((uint8 *) ptr[0])[i] == 0);
            ok &= (((int8 *) ptr[1])[i] == 0);
            ok &= (((uint16 *) ptr[2])[i] == 0);
            ok &= (((int16 *) ptr[3])[i] == 0);
            ok &= (((uint32 *) ptr[4])[i] == 0);
            ok &= (((int32 *) ptr[5])[i] == 0);
            ok &= (((uint64 *) ptr[6])[i] == 0);
            ok &= (((int64 *) ptr[7])[i] == 0);
            ok &= (((float32 *) ptr[8])[i] == 0);
            ok &= (((float64 *) ptr[9])[i] == 0);
        }
        ok &= dS.CompareS_uint8((&((uint8 *) ptr[0])[10]), interation, 1, 5);
        ok &= dS.CompareS_int8((&((int8 *) ptr[1])[10]), interation, 1, 5);
        ok &= dS.CompareS_uint16((&((uint16 *) ptr[2])[10]), interation, 2, 5);
        ok &= dS.CompareS_int16((&((int16 *) ptr[3])[10]), interation, 2, 5);
        ok &= dS.CompareS_uint32((&((uint32 *) ptr[4])[10]), interation, 4, 5);
        ok &= dS.CompareS_int32((&((int32 *) ptr[5])[10]), interation, 4, 5);
        ok &= dS.CompareS_uint64((&((uint64 *) ptr[6])[10]), interation, 8, 5);
        ok &= dS.CompareS_int64((&((int64 *) ptr[7])[10]), interation, 8, 5);
        ok &= dS.CompareS_float32((&((float32 *) ptr[8])[10]), interation, 0.1, 5);
        ok &= dS.CompareS_float64((&((float64 *) ptr[9])[10]), interation, 0.1, 5);
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise11_1() {
    MDSReaderTestHelper dS(treeName);
    dS.elementsRead = 40;
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 0, 0, 2);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    uint32 samplesToCheck = 0u;
    uint32 from = 0;
    uint32 iteration = 0;
    if (ok) {
        ok = dS.Synchronise();
        from = 0;
        samplesToCheck = 20;
        ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
        ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
        ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
        ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
        ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
        ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
        ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
        ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
        ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
        ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);

        for (uint32 i = 20u; (i < 40) && ok; i++) {
            ok &= (((uint8 *) ptr[0])[i] == 0);
            ok &= (((int8 *) ptr[1])[i] == 0);
            ok &= (((uint16 *) ptr[2])[i] == 0);
            ok &= (((int16 *) ptr[3])[i] == 0);
            ok &= (((uint32 *) ptr[4])[i] == 0);
            ok &= (((int32 *) ptr[5])[i] == 0);
            ok &= (((uint64 *) ptr[6])[i] == 0);
            ok &= (((int64 *) ptr[7])[i] == 0);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[i], static_cast<float32>(0));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[i], 0);
        }
    }
    if (ok) {
        ok = dS.Synchronise();
        iteration++;
        from = 0;
        samplesToCheck = 20;
        ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
        ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
        ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
        ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
        ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
        ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
        ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], 1, 8, samplesToCheck);
        ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
        ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
        ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);

        for (uint32 i = 20u; (i < 40) && ok; i++) {
            ok &= (((uint8 *) ptr[0])[i] == 0);
            ok &= (((int8 *) ptr[1])[i] == 0);
            ok &= (((uint16 *) ptr[2])[i] == 0);
            ok &= (((int16 *) ptr[3])[i] == 0);
            ok &= (((uint32 *) ptr[4])[i] == 0);
            ok &= (((int32 *) ptr[5])[i] == 0);
            ok &= (((uint64 *) ptr[6])[i] == 0);
            ok &= (((int64 *) ptr[7])[i] == 0);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[i], static_cast<float32>(0.0));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[i], 0.0);
        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise11_2() {
    MDSReaderTestHelper dS(treeName);
    dS.elementsRead = 40;
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 1, 0, 2);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    uint32 samplesToCheck = 0u;
    uint32 from = 0;
    uint32 iteration = 0;
    if (ok) {
        ok = dS.Synchronise();
        from = 0;
        samplesToCheck = 20;
        ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
        ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
        ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
        ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
        ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
        ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
        ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
        ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
        ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
        ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);

        for (uint32 i = 20u; (i < 40) && ok; i++) {
            ok &= (((uint8 *) ptr[0])[i] == 0);
            ok &= (((int8 *) ptr[1])[i] == 0);
            ok &= (((uint16 *) ptr[2])[i] == 0);
            ok &= (((int16 *) ptr[3])[i] == 0);
            ok &= (((uint32 *) ptr[4])[i] == 0);
            ok &= (((int32 *) ptr[5])[i] == 0);
            ok &= (((uint64 *) ptr[6])[i] == 0);
            ok &= (((int64 *) ptr[7])[i] == 0);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[i], static_cast<float32>(0));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[i], 0);
        }
    }
    if (ok) {
        ok = dS.Synchronise();
        iteration++;
        from = 0;
        samplesToCheck = 20;
        ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
        ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
        ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
        ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
        ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
        ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
        ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], 1, 8, samplesToCheck);
        ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
        ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
        ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);

        for (uint32 i = 20u; (i < 40) && ok; i++) {
            ok &= (((uint8 *) ptr[0])[i] == 0);
            ok &= (((int8 *) ptr[1])[i] == 0);
            ok &= (((uint16 *) ptr[2])[i] == 0);
            ok &= (((int16 *) ptr[3])[i] == 0);
            ok &= (((uint32 *) ptr[4])[i] == 0);
            ok &= (((int32 *) ptr[5])[i] == 0);
            ok &= (((uint64 *) ptr[6])[i] == 0);
            ok &= (((int64 *) ptr[7])[i] == 0);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[i], static_cast<float32>(0.0));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[i], 0.0);
        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise11_3() {
    MDSReaderTestHelper dS(treeName);
    dS.elementsRead = 40;
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 2, 0, 2);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    uint32 samplesToCheck = 0u;
    uint32 from = 0;
    uint32 iteration = 0;
    if (ok) {
        ok = dS.Synchronise();
        from = 0;
        samplesToCheck = 20;
        ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
        ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
        ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
        ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
        ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
        ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
        ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
        ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
        ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
        ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);

        for (uint32 i = 20u; (i < 40) && ok; i++) {
            ok &= (((uint8 *) ptr[0])[i] == 0);
            ok &= (((int8 *) ptr[1])[i] == 0);
            ok &= (((uint16 *) ptr[2])[i] == 0);
            ok &= (((int16 *) ptr[3])[i] == 0);
            ok &= (((uint32 *) ptr[4])[i] == 0);
            ok &= (((int32 *) ptr[5])[i] == 0);
            ok &= (((uint64 *) ptr[6])[i] == 0);
            ok &= (((int64 *) ptr[7])[i] == 0);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[i], static_cast<float32>(0));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[i], 0);
        }
    }
    if (ok) {
        ok = dS.Synchronise();
        iteration++;
        from = 0;
        samplesToCheck = 20;
        ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
        ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
        ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
        ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
        ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
        ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
        ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], 1, 8, samplesToCheck);
        ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
        ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
        ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);

        for (uint32 i = 20u; (i < 40) && ok; i++) {
            ok &= (((uint8 *) ptr[0])[i] == 0);
            ok &= (((int8 *) ptr[1])[i] == 0);
            ok &= (((uint16 *) ptr[2])[i] == 0);
            ok &= (((int16 *) ptr[3])[i] == 0);
            ok &= (((uint32 *) ptr[4])[i] == 0);
            ok &= (((int32 *) ptr[5])[i] == 0);
            ok &= (((uint64 *) ptr[6])[i] == 0);
            ok &= (((int64 *) ptr[7])[i] == 0);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[i], static_cast<float32>(0.0));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[i], 0.0);
        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise11_4() {
    MDSReaderTestHelper dS(treeName);
    dS.elementsRead = 40;
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 1, 1, 2);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    uint32 samplesToCheck = 0u;
    uint32 from = 0;
    uint32 iteration = 0;
    if (ok) {
        ok = dS.Synchronise();
        from = 0;
        samplesToCheck = 20;
        ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
        ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
        ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
        ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
        ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
        ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
        ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
        ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
        ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
        ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);

        for (uint32 i = 20u; (i < 40) && ok; i++) {
            ok &= (((uint8 *) ptr[0])[i] == 19);
            ok &= (((int8 *) ptr[1])[i] == -1);
            ok &= (((uint16 *) ptr[2])[i] == 38);
            ok &= (((int16 *) ptr[3])[i] == -2);
            ok &= (((uint32 *) ptr[4])[i] == 76);
            ok &= (((int32 *) ptr[5])[i] == -4);
            ok &= (((uint64 *) ptr[6])[i] == 152);
            ok &= (((int64 *) ptr[7])[i] == -8);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[i], static_cast<float32>(1.9));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[i], 1.9);
        }
    }
    if (ok) {
        ok = dS.Synchronise();
        iteration++;
        from = 0;
        samplesToCheck = 20;
        ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
        ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
        ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
        ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
        ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
        ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
        ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], 1, 8, samplesToCheck);
        ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
        ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
        ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);

        for (uint32 i = 20u; (i < 40) && ok; i++) {
            ok &= (((uint8 *) ptr[0])[i] == 39);
            ok &= (((int8 *) ptr[1])[i] == 19);
            ok &= (((uint16 *) ptr[2])[i] == 78);
            ok &= (((int16 *) ptr[3])[i] == 38);
            ok &= (((uint32 *) ptr[4])[i] == 156);
            ok &= (((int32 *) ptr[5])[i] == 76);
            ok &= (((uint64 *) ptr[6])[i] == 312);
            ok &= (((int64 *) ptr[7])[i] == 152);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[i], static_cast<float32>(3.9));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[i], 3.9);
        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise11_5() {
    MDSReaderTestHelper dS(treeName);
    dS.elementsRead = 40;
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 2, 1, 2);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    uint32 samplesToCheck = 0u;
    uint32 from = 0;
    uint32 iteration = 0;
    if (ok) {
        ok = dS.Synchronise();
        from = 0;
        samplesToCheck = 20;
        ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
        ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
        ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
        ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
        ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
        ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
        ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
        ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
        ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
        ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);

        for (uint32 i = 20u; (i < 40) && ok; i++) {
            ok &= (((uint8 *) ptr[0])[i] == 19);
            ok &= (((int8 *) ptr[1])[i] == -1);
            ok &= (((uint16 *) ptr[2])[i] == 38);
            ok &= (((int16 *) ptr[3])[i] == -2);
            ok &= (((uint32 *) ptr[4])[i] == 76);
            ok &= (((int32 *) ptr[5])[i] == -4);
            ok &= (((uint64 *) ptr[6])[i] == 152);
            ok &= (((int64 *) ptr[7])[i] == -8);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[i], static_cast<float32>(1.9));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[i], 1.9);
        }
    }
    if (ok) {
        ok = dS.Synchronise();
        iteration++;
        from = 0;
        samplesToCheck = 20;
        ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
        ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
        ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
        ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
        ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
        ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
        ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], 1, 8, samplesToCheck);
        ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
        ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
        ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);

        for (uint32 i = 20u; (i < 40) && ok; i++) {
            ok &= (((uint8 *) ptr[0])[i] == 39);
            ok &= (((int8 *) ptr[1])[i] == 19);
            ok &= (((uint16 *) ptr[2])[i] == 78);
            ok &= (((int16 *) ptr[3])[i] == 38);
            ok &= (((uint32 *) ptr[4])[i] == 156);
            ok &= (((int32 *) ptr[5])[i] == 76);
            ok &= (((uint64 *) ptr[6])[i] == 312);
            ok &= (((int64 *) ptr[7])[i] == 152);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[i], static_cast<float32>(3.9));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[i], 3.9);
        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise12() {
    MDSReaderTestHelper dS(treeName);
    dS.elementsRead = 25;
    uint32 elementsToCheck = 0;
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 0, 0, 2);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    if (ok) {

        ok = dS.Synchronise();
        elementsToCheck = 20;
        if (ok) {
            ok &= dS.CompareS_uint8(((uint8 *) ptr[0]), 0, 1, elementsToCheck);
            ok &= dS.CompareS_int8(((int8 *) ptr[1]), 0, 1, elementsToCheck);
            ok &= dS.CompareS_uint16(((uint16 *) ptr[2]), 0, 2, elementsToCheck);
            ok &= dS.CompareS_int16(((int16 *) ptr[3]), 0, 2, elementsToCheck);
            ok &= dS.CompareS_uint32(((uint32 *) ptr[4]), 0, 4, elementsToCheck);
            ok &= dS.CompareS_int32(((int32 *) ptr[5]), 0, 4, elementsToCheck);
            ok &= dS.CompareS_uint64(((uint64 *) ptr[6]), 0, 8, elementsToCheck);
            ok &= dS.CompareS_int64(((int64 *) ptr[7]), 0, 8, elementsToCheck);
            ok &= dS.CompareS_float32(((float32 *) ptr[8]), 0, 0.1, elementsToCheck);
            ok &= dS.CompareS_float64(((float64 *) ptr[9]), 0, 0.1, elementsToCheck);
        }
        for (uint32 i = elementsToCheck; (i < dS.elementsRead) && ok; i++) {
            ok &= (((uint8 *) ptr[0])[i] == 0);
            ok &= (((int8 *) ptr[1])[i] == 0);
            ok &= (((uint16 *) ptr[2])[i] == 0);
            ok &= (((int16 *) ptr[3])[i] == 0);
            ok &= (((uint32 *) ptr[4])[i] == 0);
            ok &= (((int32 *) ptr[5])[i] == 0);
            ok &= (((uint64 *) ptr[6])[i] == 0);
            ok &= (((int64 *) ptr[7])[i] == 0);
            ok &= (((float32 *) ptr[8])[i] == 0);
            ok &= (((float64 *) ptr[9])[i] == 0);
        }
    }
    if (ok) {
        elementsToCheck = 10;
        ok = dS.Synchronise();
        for (uint32 i = 0u; (i < 15) && ok; i++) {
            ok &= (((uint8 *) ptr[0])[i] == 0);
            ok &= (((int8 *) ptr[1])[i] == 0);
            ok &= (((uint16 *) ptr[2])[i] == 0);
            ok &= (((int16 *) ptr[3])[i] == 0);
            ok &= (((uint32 *) ptr[4])[i] == 0);
            ok &= (((int32 *) ptr[5])[i] == 0);
            ok &= (((uint64 *) ptr[6])[i] == 0);
            ok &= (((int64 *) ptr[7])[i] == 0);
            ok &= (((float32 *) ptr[8])[i] == 0);
            ok &= (((float64 *) ptr[9])[i] == 0);
        }
        ok &= dS.CompareS_uint8((&((uint8 *) ptr[0])[15]), 1, 1, elementsToCheck);
        ok &= dS.CompareS_int8((&((int8 *) ptr[1])[15]), 1, 1, elementsToCheck);
        ok &= dS.CompareS_uint16((&((uint16 *) ptr[2])[15]), 1, 2, elementsToCheck);
        ok &= dS.CompareS_int16((&((int16 *) ptr[3])[15]), 1, 2, elementsToCheck);
        ok &= dS.CompareS_uint32((&((uint32 *) ptr[4])[15]), 1, 4, elementsToCheck);
        ok &= dS.CompareS_int32((&((int32 *) ptr[5])[15]), 1, 4, elementsToCheck);
        ok &= dS.CompareS_uint64((&((uint64 *) ptr[6])[15]), 1, 8, elementsToCheck);
        ok &= dS.CompareS_int64((&((int64 *) ptr[7])[15]), 1, 8, elementsToCheck);
        ok &= dS.CompareS_float32((&((float32 *) ptr[8])[15]), 1, 0.1, elementsToCheck);
        ok &= dS.CompareS_float64((&((float64 *) ptr[9])[15]), 1, 0.1, elementsToCheck);

    }
    if (ok) {
        elementsToCheck = 10;
        ok = dS.Synchronise();
        ok &= dS.CompareS_uint8((&((uint8 *) ptr[0])[0]), 2, 1, elementsToCheck);
        ok &= dS.CompareS_int8((&((int8 *) ptr[1])[0]), 2, 1, elementsToCheck);
        ok &= dS.CompareS_uint16((&((uint16 *) ptr[2])[0]), 2, 2, elementsToCheck);
        ok &= dS.CompareS_int16((&((int16 *) ptr[3])[0]), 2, 2, elementsToCheck);
        ok &= dS.CompareS_uint32((&((uint32 *) ptr[4])[0]), 2, 4, elementsToCheck);
        ok &= dS.CompareS_int32((&((int32 *) ptr[5])[0]), 2, 4, elementsToCheck);
        ok &= dS.CompareS_uint64((&((uint64 *) ptr[6])[0]), 2, 8, elementsToCheck);
        ok &= dS.CompareS_int64((&((int64 *) ptr[7])[0]), 2, 8, elementsToCheck);
        ok &= dS.CompareS_float32((&((float32 *) ptr[8])[0]), 2, 0.1, elementsToCheck);
        ok &= dS.CompareS_float64((&((float64 *) ptr[9])[0]), 2, 0.1, elementsToCheck);

        for (uint32 i = 10; (i < 25) && ok; i++) {
            ok &= (((uint8 *) ptr[0])[i] == 0);
            ok &= (((int8 *) ptr[1])[i] == 0);
            ok &= (((uint16 *) ptr[2])[i] == 0);
            ok &= (((int16 *) ptr[3])[i] == 0);
            ok &= (((uint32 *) ptr[4])[i] == 0);
            ok &= (((int32 *) ptr[5])[i] == 0);
            ok &= (((uint64 *) ptr[6])[i] == 0);
            ok &= (((int64 *) ptr[7])[i] == 0);
            ok &= (((float32 *) ptr[8])[i] == 0);
            ok &= (((float64 *) ptr[9])[i] == 0);
        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise13() {
    MDSReaderTestHelper dS(treeName);
    dS.elementsRead = 5;
    uint32 elementsToCheck = 0;
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 0, 0, 2);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    if (ok) {
        for (uint32 m = 0u; m < 4; m++) {

            for (uint32 i = 0u; i < 4; i++) {
                ok = dS.Synchronise();
                elementsToCheck = 5;
                if (ok) {
                    ok &= dS.CompareS_uint8(((uint8 *) ptr[0]), m + i, 1, elementsToCheck);
                    ok &= dS.CompareS_int8(((int8 *) ptr[1]), m + i, 1, elementsToCheck);
                    ok &= dS.CompareS_uint16(((uint16 *) ptr[2]), m + i, 2, elementsToCheck);
                    ok &= dS.CompareS_int16(((int16 *) ptr[3]), m + i, 2, elementsToCheck);
                    ok &= dS.CompareS_uint32(((uint32 *) ptr[4]), m + i, 4, elementsToCheck);
                    ok &= dS.CompareS_int32(((int32 *) ptr[5]), m + i, 4, elementsToCheck);
                    ok &= dS.CompareS_uint64(((uint64 *) ptr[6]), m + i, 8, elementsToCheck);
                    ok &= dS.CompareS_int64(((int64 *) ptr[7]), m + i, 8, elementsToCheck);
                    ok &= dS.CompareS_float32(((float32 *) ptr[8]), m + i, 0.1, elementsToCheck);
                    ok &= dS.CompareS_float64(((float64 *) ptr[9]), m + i, 0.1, elementsToCheck);
                }
            }
            for (uint32 i = 0u; i < 4; i++) {
                ok = dS.Synchronise();
                for (uint32 j = 0; (j < dS.elementsRead) && ok; j++) {
                    ok &= (((uint8 *) ptr[0])[j] == 0);
                    ok &= (((int8 *) ptr[1])[j] == 0);
                    ok &= (((uint16 *) ptr[2])[j] == 0);
                    ok &= (((int16 *) ptr[3])[j] == 0);
                    ok &= (((uint32 *) ptr[4])[j] == 0);
                    ok &= (((int32 *) ptr[5])[j] == 0);
                    ok &= (((uint64 *) ptr[6])[j] == 0);
                    ok &= (((int64 *) ptr[7])[j] == 0);
                    ok &= (((float32 *) ptr[8])[j] == 0);
                    ok &= (((float64 *) ptr[9])[j] == 0);
                }
            }
        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise14() {
    MDSReaderTestHelper dS(treeName);
    dS.elementsRead = 15;
    uint32 elementsToCheck = 0;
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 0, 1, 2);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    if (ok) {

        ok = dS.Synchronise();
        elementsToCheck = 15;
        if (ok) {
            ok &= dS.CompareS_uint8(((uint8 *) ptr[0]), 0, 1, elementsToCheck);
            ok &= dS.CompareS_int8(((int8 *) ptr[1]), 0, 1, elementsToCheck);
            ok &= dS.CompareS_uint16(((uint16 *) ptr[2]), 0, 2, elementsToCheck);
            ok &= dS.CompareS_int16(((int16 *) ptr[3]), 0, 2, elementsToCheck);
            ok &= dS.CompareS_uint32(((uint32 *) ptr[4]), 0, 4, elementsToCheck);
            ok &= dS.CompareS_int32(((int32 *) ptr[5]), 0, 4, elementsToCheck);
            ok &= dS.CompareS_uint64(((uint64 *) ptr[6]), 0, 8, elementsToCheck);
            ok &= dS.CompareS_int64(((int64 *) ptr[7]), 0, 8, elementsToCheck);
            ok &= dS.CompareS_float32(((float32 *) ptr[8]), 0, 0.1, elementsToCheck);
            ok &= dS.CompareS_float64(((float64 *) ptr[9]), 0, 0.1, elementsToCheck);
        }
    }
    if (ok) {
        ok = dS.Synchronise();
        elementsToCheck = 5;
        if (ok) {
            ok &= dS.CompareS_uint8(((uint8 *) ptr[0]), 1, 1, elementsToCheck);
            ok &= dS.CompareS_int8(((int8 *) ptr[1]), 1, 1, elementsToCheck);
            ok &= dS.CompareS_uint16(((uint16 *) ptr[2]), 1, 2, elementsToCheck);
            ok &= dS.CompareS_int16(((int16 *) ptr[3]), 1, 2, elementsToCheck);
            ok &= dS.CompareS_uint32(((uint32 *) ptr[4]), 1, 4, elementsToCheck);
            ok &= dS.CompareS_int32(((int32 *) ptr[5]), 1, 4, elementsToCheck);
            ok &= dS.CompareS_uint64(((uint64 *) ptr[6]), 1, 8, elementsToCheck);
            ok &= dS.CompareS_int64(((int64 *) ptr[7]), 1, 8, elementsToCheck);
            ok &= dS.CompareS_float32(((float32 *) ptr[8]), 1, 0.1, elementsToCheck);
            ok &= dS.CompareS_float64(((float64 *) ptr[9]), 1, 0.1, elementsToCheck);
        }
        for (uint32 i = elementsToCheck; (i < 15) && ok; i++) {
            ok &= (((uint8 *) ptr[0])[i] == 19);
            ok &= (((int8 *) ptr[1])[i] == -1);
            ok &= (((uint16 *) ptr[2])[i] == 38);
            ok &= (((int16 *) ptr[3])[i] == -2);
            ok &= (((uint32 *) ptr[4])[i] == 76);
            ok &= (((int32 *) ptr[5])[i] == -4);
            ok &= (((uint64 *) ptr[6])[i] == 152);
            ok &= (((int64 *) ptr[7])[i] == -8);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[i], static_cast<float32>(1.9));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[i], 1.9);
        }
    }
    if (ok) {
        ok = dS.Synchronise();
        for (uint32 i = 0u; (i < 10) && ok; i++) {
            ok &= (((uint8 *) ptr[0])[i] == 19);
            ok &= (((int8 *) ptr[1])[i] == -1);
            ok &= (((uint16 *) ptr[2])[i] == 38);
            ok &= (((int16 *) ptr[3])[i] == -2);
            ok &= (((uint32 *) ptr[4])[i] == 76);
            ok &= (((int32 *) ptr[5])[i] == -4);
            ok &= (((uint64 *) ptr[6])[i] == 152);
            ok &= (((int64 *) ptr[7])[i] == -8);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[i], static_cast<float32>(1.9));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[i], 1.9);
        }
        elementsToCheck = 5;
        ok &= dS.CompareS_uint8((&((uint8 *) ptr[0])[10]), 2, 1, elementsToCheck);
        ok &= dS.CompareS_int8((&((int8 *) ptr[1])[10]), 2, 1, elementsToCheck);
        ok &= dS.CompareS_uint16((&((uint16 *) ptr[2])[10]), 2, 2, elementsToCheck);
        ok &= dS.CompareS_int16((&((int16 *) ptr[3])[10]), 2, 2, elementsToCheck);
        ok &= dS.CompareS_uint32((&((uint32 *) ptr[4])[10]), 2, 4, elementsToCheck);
        ok &= dS.CompareS_int32((&((int32 *) ptr[5])[10]), 2, 4, elementsToCheck);
        ok &= dS.CompareS_uint64((&((uint64 *) ptr[6])[10]), 2, 8, elementsToCheck);
        ok &= dS.CompareS_int64((&((int64 *) ptr[7])[10]), 2, 8, elementsToCheck);
        ok &= dS.CompareS_float32((&((float32 *) ptr[8])[10]), 2, 0.1, elementsToCheck);
        ok &= dS.CompareS_float64((&((float64 *) ptr[9])[10]), 2, 0.1, elementsToCheck);
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise15() {
    MDSReaderTestHelper dS(treeName);
    dS.elementsRead = 25;
    uint32 elementsToCheck = 0;
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 0, 1, 2);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    if (ok) {

        ok = dS.Synchronise();
        elementsToCheck = 20;
        if (ok) {
            ok &= dS.CompareS_uint8(((uint8 *) ptr[0]), 0, 1, elementsToCheck);
            ok &= dS.CompareS_int8(((int8 *) ptr[1]), 0, 1, elementsToCheck);
            ok &= dS.CompareS_uint16(((uint16 *) ptr[2]), 0, 2, elementsToCheck);
            ok &= dS.CompareS_int16(((int16 *) ptr[3]), 0, 2, elementsToCheck);
            ok &= dS.CompareS_uint32(((uint32 *) ptr[4]), 0, 4, elementsToCheck);
            ok &= dS.CompareS_int32(((int32 *) ptr[5]), 0, 4, elementsToCheck);
            ok &= dS.CompareS_uint64(((uint64 *) ptr[6]), 0, 8, elementsToCheck);
            ok &= dS.CompareS_int64(((int64 *) ptr[7]), 0, 8, elementsToCheck);
            ok &= dS.CompareS_float32(((float32 *) ptr[8]), 0, 0.1, elementsToCheck);
            ok &= dS.CompareS_float64(((float64 *) ptr[9]), 0, 0.1, elementsToCheck);
        }
        for (uint32 i = elementsToCheck; (i < dS.elementsRead) && ok; i++) {
            ok &= (((uint8 *) ptr[0])[i] == 19);
            ok &= (((int8 *) ptr[1])[i] == -1);
            ok &= (((uint16 *) ptr[2])[i] == 38);
            ok &= (((int16 *) ptr[3])[i] == -2);
            ok &= (((uint32 *) ptr[4])[i] == 76);
            ok &= (((int32 *) ptr[5])[i] == -4);
            ok &= (((uint64 *) ptr[6])[i] == 152);
            ok &= (((int64 *) ptr[7])[i] == -8);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[i], static_cast<float32>(1.9));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[i], 1.9);
        }
    }
    if (ok) {
        ok = dS.Synchronise();
        elementsToCheck = 10;
        for (uint32 i = 0u; (i < 15) && ok; i++) {
            ok &= (((uint8 *) ptr[0])[i] == 19);
            ok &= (((int8 *) ptr[1])[i] == -1);
            ok &= (((uint16 *) ptr[2])[i] == 38);
            ok &= (((int16 *) ptr[3])[i] == -2);
            ok &= (((uint32 *) ptr[4])[i] == 76);
            ok &= (((int32 *) ptr[5])[i] == -4);
            ok &= (((uint64 *) ptr[6])[i] == 152);
            ok &= (((int64 *) ptr[7])[i] == -8);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[i], static_cast<float32>(1.9));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[i], 1.9);
        }
        ok &= dS.CompareS_uint8((&((uint8 *) ptr[0])[15]), 1, 1, elementsToCheck);
        ok &= dS.CompareS_int8((&((int8 *) ptr[1])[15]), 1, 1, elementsToCheck);
        ok &= dS.CompareS_uint16((&((uint16 *) ptr[2])[15]), 1, 2, elementsToCheck);
        ok &= dS.CompareS_int16((&((int16 *) ptr[3])[15]), 1, 2, elementsToCheck);
        ok &= dS.CompareS_uint32((&((uint32 *) ptr[4])[15]), 1, 4, elementsToCheck);
        ok &= dS.CompareS_int32((&((int32 *) ptr[5])[15]), 1, 4, elementsToCheck);
        ok &= dS.CompareS_uint64((&((uint64 *) ptr[6])[15]), 1, 8, elementsToCheck);
        ok &= dS.CompareS_int64((&((int64 *) ptr[7])[15]), 1, 8, elementsToCheck);
        ok &= dS.CompareS_float32((&((float32 *) ptr[8])[15]), 1, 0.1, elementsToCheck);
        ok &= dS.CompareS_float64((&((float64 *) ptr[9])[15]), 1, 0.1, elementsToCheck);

    }
    if (ok) {
        elementsToCheck = 10;
        ok = dS.Synchronise();
        ok &= dS.CompareS_uint8((&((uint8 *) ptr[0])[0]), 2, 1, elementsToCheck);
        ok &= dS.CompareS_int8((&((int8 *) ptr[1])[0]), 2, 1, elementsToCheck);
        ok &= dS.CompareS_uint16((&((uint16 *) ptr[2])[0]), 2, 2, elementsToCheck);
        ok &= dS.CompareS_int16((&((int16 *) ptr[3])[0]), 2, 2, elementsToCheck);
        ok &= dS.CompareS_uint32((&((uint32 *) ptr[4])[0]), 2, 4, elementsToCheck);
        ok &= dS.CompareS_int32((&((int32 *) ptr[5])[0]), 2, 4, elementsToCheck);
        ok &= dS.CompareS_uint64((&((uint64 *) ptr[6])[0]), 2, 8, elementsToCheck);
        ok &= dS.CompareS_int64((&((int64 *) ptr[7])[0]), 2, 8, elementsToCheck);
        ok &= dS.CompareS_float32((&((float32 *) ptr[8])[0]), 2, 0.1, elementsToCheck);
        ok &= dS.CompareS_float64((&((float64 *) ptr[9])[0]), 2, 0.1, elementsToCheck);

        for (uint32 i = 10; (i < 25) && ok; i++) {
            ok &= (((uint8 *) ptr[0])[i] == 39);
            ok &= (((int8 *) ptr[1])[i] == 19);
            ok &= (((uint16 *) ptr[2])[i] == 78);
            ok &= (((int16 *) ptr[3])[i] == 38);
            ok &= (((uint32 *) ptr[4])[i] == 156);
            ok &= (((int32 *) ptr[5])[i] == 76);
            ok &= (((uint64 *) ptr[6])[i] == 312);
            ok &= (((int64 *) ptr[7])[i] == 152);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[i], static_cast<float32>(3.9));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[i], 3.9);
        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise16() {
    MDSReaderTestHelper dS(treeName);
    dS.elementsRead = 5;
    uint32 elementsToCheck = 0;
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 0, 1, 2);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    if (ok) {
        for (uint32 m = 0u; m < 4; m++) {

            for (uint32 i = 0u; i < 4; i++) {
                ok = dS.Synchronise();
                elementsToCheck = 5;
                if (ok) {
                    ok &= dS.CompareS_uint8(((uint8 *) ptr[0]), (m * 4) + i, 1, elementsToCheck);
                    ok &= dS.CompareS_int8(((int8 *) ptr[1]), (m * 4) + i, 1, elementsToCheck);
                    ok &= dS.CompareS_uint16(((uint16 *) ptr[2]), (m * 4) + i, 2, elementsToCheck);
                    ok &= dS.CompareS_int16(((int16 *) ptr[3]), (m * 4) + i, 2, elementsToCheck);
                    ok &= dS.CompareS_uint32(((uint32 *) ptr[4]), (m * 4) + i, 4, elementsToCheck);
                    ok &= dS.CompareS_int32(((int32 *) ptr[5]), (m * 4) + i, 4, elementsToCheck);
                    ok &= dS.CompareS_uint64(((uint64 *) ptr[6]), (m * 4) + i, 8, elementsToCheck);
                    ok &= dS.CompareS_int64(((int64 *) ptr[7]), (m * 4) + i, 8, elementsToCheck);
                    ok &= dS.CompareS_float32(((float32 *) ptr[8]), (m * 4) + i, 0.1, elementsToCheck);
                    ok &= dS.CompareS_float64(((float64 *) ptr[9]), (m * 4) + i, 0.1, elementsToCheck);
                }
            }
            for (uint32 i = 0u; i < 4; i++) {
                ok = dS.Synchronise();
                for (uint32 j = 0; (j < dS.elementsRead) && ok; j++) {
                    ok &= (((uint8 *) ptr[0])[j] == 20 * (m + 1) - 1);
                    int8 aux = int8(20 * (m + 1) - 1) - 20;
                    ok &= (((int8 *) ptr[1])[j] == aux);
                    ok &= (((uint16 *) ptr[2])[j] == 40 * (m + 1) - 2);
                    int16 aux16 = int16(40 * (m + 1) - 2) - 40;
                    ok &= (((int16 *) ptr[3])[j] == aux16);
                    ok &= (((uint32 *) ptr[4])[j] == 80 * (m + 1) - 4);
                    int32 aux32 = int32(80 * (m + 1) - 4) - 80;
                    ok &= (((int32 *) ptr[5])[j] == aux32);
                    ok &= (((uint64 *) ptr[6])[j] == 160 * (m + 1) - 8);
                    int64 aux64 = int64(160 * (m + 1) - 8) - 160;
                    ok &= (((int64 *) ptr[7])[j] == aux64);
                    ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[i], static_cast<float32>(2 * (m + 1) - 0.1));
                    ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[i], 2 * (m + 1) - 0.1);
                }
            }

        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise17() {
    MDSReaderTestHelper dS(treeName);
    dS.elementsRead = 15;
    uint32 elementsToCheck = 0;
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 1, 0, 2);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    if (ok) {

        ok = dS.Synchronise();
        elementsToCheck = 15;
        if (ok) {
            ok &= dS.CompareS_uint8(((uint8 *) ptr[0]), 0, 1, elementsToCheck);
            ok &= dS.CompareS_int8(((int8 *) ptr[1]), 0, 1, elementsToCheck);
            ok &= dS.CompareS_uint16(((uint16 *) ptr[2]), 0, 2, elementsToCheck);
            ok &= dS.CompareS_int16(((int16 *) ptr[3]), 0, 2, elementsToCheck);
            ok &= dS.CompareS_uint32(((uint32 *) ptr[4]), 0, 4, elementsToCheck);
            ok &= dS.CompareS_int32(((int32 *) ptr[5]), 0, 4, elementsToCheck);
            ok &= dS.CompareS_uint64(((uint64 *) ptr[6]), 0, 8, elementsToCheck);
            ok &= dS.CompareS_int64(((int64 *) ptr[7]), 0, 8, elementsToCheck);
            ok &= dS.CompareS_float32(((float32 *) ptr[8]), 0, 0.1, elementsToCheck);
            ok &= dS.CompareS_float64(((float64 *) ptr[9]), 0, 0.1, elementsToCheck);
        }
    }
    if (ok) {
        ok = dS.Synchronise();
        elementsToCheck = 5;
        if (ok) {
            ok &= dS.CompareS_uint8(((uint8 *) ptr[0]), 1, 1, elementsToCheck);
            ok &= dS.CompareS_int8(((int8 *) ptr[1]), 1, 1, elementsToCheck);
            ok &= dS.CompareS_uint16(((uint16 *) ptr[2]), 1, 2, elementsToCheck);
            ok &= dS.CompareS_int16(((int16 *) ptr[3]), 1, 2, elementsToCheck);
            ok &= dS.CompareS_uint32(((uint32 *) ptr[4]), 1, 4, elementsToCheck);
            ok &= dS.CompareS_int32(((int32 *) ptr[5]), 1, 4, elementsToCheck);
            ok &= dS.CompareS_uint64(((uint64 *) ptr[6]), 1, 8, elementsToCheck);
            ok &= dS.CompareS_int64(((int64 *) ptr[7]), 1, 8, elementsToCheck);
            ok &= dS.CompareS_float32(((float32 *) ptr[8]), 1, 0.1, elementsToCheck);
            ok &= dS.CompareS_float64(((float64 *) ptr[9]), 1, 0.1, elementsToCheck);
        }
        for (uint32 i = elementsToCheck; (i < 15) && ok; i++) {
            ok &= (((uint8 *) ptr[0])[i] == 0);
            ok &= (((int8 *) ptr[1])[i] == 0);
            ok &= (((uint16 *) ptr[2])[i] == 0);
            ok &= (((int16 *) ptr[3])[i] == 0);
            ok &= (((uint32 *) ptr[4])[i] == 0);
            ok &= (((int32 *) ptr[5])[i] == 0);
            ok &= (((uint64 *) ptr[6])[i] == 0);
            ok &= (((int64 *) ptr[7])[i] == 0);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[i], static_cast<float32>(0));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[i], 0);
        }
    }
    if (ok) {
        ok = dS.Synchronise();
        for (uint32 i = 0u; (i < 10) && ok; i++) {
            ok &= (((uint8 *) ptr[0])[i] == 0);
            ok &= (((int8 *) ptr[1])[i] == 0);
            ok &= (((uint16 *) ptr[2])[i] == 0);
            ok &= (((int16 *) ptr[3])[i] == 0);
            ok &= (((uint32 *) ptr[4])[i] == 0);
            ok &= (((int32 *) ptr[5])[i] == 0);
            ok &= (((uint64 *) ptr[6])[i] == 0);
            ok &= (((int64 *) ptr[7])[i] == 0);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[i], static_cast<float32>(0));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[i], 0);
        }
        elementsToCheck = 5;
        ok &= dS.CompareS_uint8((&((uint8 *) ptr[0])[10]), 2, 1, elementsToCheck);
        ok &= dS.CompareS_int8((&((int8 *) ptr[1])[10]), 2, 1, elementsToCheck);
        ok &= dS.CompareS_uint16((&((uint16 *) ptr[2])[10]), 2, 2, elementsToCheck);
        ok &= dS.CompareS_int16((&((int16 *) ptr[3])[10]), 2, 2, elementsToCheck);
        ok &= dS.CompareS_uint32((&((uint32 *) ptr[4])[10]), 2, 4, elementsToCheck);
        ok &= dS.CompareS_int32((&((int32 *) ptr[5])[10]), 2, 4, elementsToCheck);
        ok &= dS.CompareS_uint64((&((uint64 *) ptr[6])[10]), 2, 8, elementsToCheck);
        ok &= dS.CompareS_int64((&((int64 *) ptr[7])[10]), 2, 8, elementsToCheck);
        ok &= dS.CompareS_float32((&((float32 *) ptr[8])[10]), 2, 0.1, elementsToCheck);
        ok &= dS.CompareS_float64((&((float64 *) ptr[9])[10]), 2, 0.1, elementsToCheck);
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise18() {
    MDSReaderTestHelper dS(treeName);
    dS.elementsRead = 25;
    uint32 elementsToCheck = 0;
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 1, 0, 2);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    if (ok) {

        ok = dS.Synchronise();
        elementsToCheck = 20;
        if (ok) {
            ok &= dS.CompareS_uint8(((uint8 *) ptr[0]), 0, 1, elementsToCheck);
            ok &= dS.CompareS_int8(((int8 *) ptr[1]), 0, 1, elementsToCheck);
            ok &= dS.CompareS_uint16(((uint16 *) ptr[2]), 0, 2, elementsToCheck);
            ok &= dS.CompareS_int16(((int16 *) ptr[3]), 0, 2, elementsToCheck);
            ok &= dS.CompareS_uint32(((uint32 *) ptr[4]), 0, 4, elementsToCheck);
            ok &= dS.CompareS_int32(((int32 *) ptr[5]), 0, 4, elementsToCheck);
            ok &= dS.CompareS_uint64(((uint64 *) ptr[6]), 0, 8, elementsToCheck);
            ok &= dS.CompareS_int64(((int64 *) ptr[7]), 0, 8, elementsToCheck);
            ok &= dS.CompareS_float32(((float32 *) ptr[8]), 0, 0.1, elementsToCheck);
            ok &= dS.CompareS_float64(((float64 *) ptr[9]), 0, 0.1, elementsToCheck);
        }
        for (uint32 i = elementsToCheck; (i < dS.elementsRead) && ok; i++) {
            ok &= (((uint8 *) ptr[0])[i] == 0);
            ok &= (((int8 *) ptr[1])[i] == 0);
            ok &= (((uint16 *) ptr[2])[i] == 0);
            ok &= (((int16 *) ptr[3])[i] == 0);
            ok &= (((uint32 *) ptr[4])[i] == 0);
            ok &= (((int32 *) ptr[5])[i] == 0);
            ok &= (((uint64 *) ptr[6])[i] == 0);
            ok &= (((int64 *) ptr[7])[i] == 0);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[i], static_cast<float32>(0));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[i], 0);
        }
    }
    if (ok) {
        ok = dS.Synchronise();
        elementsToCheck = 10;
        for (uint32 i = 0u; (i < 15) && ok; i++) {
            ok &= (((uint8 *) ptr[0])[i] == 0);
            ok &= (((int8 *) ptr[1])[i] == 0);
            ok &= (((uint16 *) ptr[2])[i] == 0);
            ok &= (((int16 *) ptr[3])[i] == 0);
            ok &= (((uint32 *) ptr[4])[i] == 0);
            ok &= (((int32 *) ptr[5])[i] == 0);
            ok &= (((uint64 *) ptr[6])[i] == 0);
            ok &= (((int64 *) ptr[7])[i] == 0);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[i], static_cast<float32>(0));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[i], 0);
        }
        ok &= dS.CompareS_uint8((&((uint8 *) ptr[0])[15]), 1, 1, elementsToCheck);
        ok &= dS.CompareS_int8((&((int8 *) ptr[1])[15]), 1, 1, elementsToCheck);
        ok &= dS.CompareS_uint16((&((uint16 *) ptr[2])[15]), 1, 2, elementsToCheck);
        ok &= dS.CompareS_int16((&((int16 *) ptr[3])[15]), 1, 2, elementsToCheck);
        ok &= dS.CompareS_uint32((&((uint32 *) ptr[4])[15]), 1, 4, elementsToCheck);
        ok &= dS.CompareS_int32((&((int32 *) ptr[5])[15]), 1, 4, elementsToCheck);
        ok &= dS.CompareS_uint64((&((uint64 *) ptr[6])[15]), 1, 8, elementsToCheck);
        ok &= dS.CompareS_int64((&((int64 *) ptr[7])[15]), 1, 8, elementsToCheck);
        ok &= dS.CompareS_float32((&((float32 *) ptr[8])[15]), 1, 0.1, elementsToCheck);
        ok &= dS.CompareS_float64((&((float64 *) ptr[9])[15]), 1, 0.1, elementsToCheck);

    }
    if (ok) {
        elementsToCheck = 10;
        ok = dS.Synchronise();
        ok &= dS.CompareS_uint8((&((uint8 *) ptr[0])[0]), 2, 1, elementsToCheck);
        ok &= dS.CompareS_int8((&((int8 *) ptr[1])[0]), 2, 1, elementsToCheck);
        ok &= dS.CompareS_uint16((&((uint16 *) ptr[2])[0]), 2, 2, elementsToCheck);
        ok &= dS.CompareS_int16((&((int16 *) ptr[3])[0]), 2, 2, elementsToCheck);
        ok &= dS.CompareS_uint32((&((uint32 *) ptr[4])[0]), 2, 4, elementsToCheck);
        ok &= dS.CompareS_int32((&((int32 *) ptr[5])[0]), 2, 4, elementsToCheck);
        ok &= dS.CompareS_uint64((&((uint64 *) ptr[6])[0]), 2, 8, elementsToCheck);
        ok &= dS.CompareS_int64((&((int64 *) ptr[7])[0]), 2, 8, elementsToCheck);
        ok &= dS.CompareS_float32((&((float32 *) ptr[8])[0]), 2, 0.1, elementsToCheck);
        ok &= dS.CompareS_float64((&((float64 *) ptr[9])[0]), 2, 0.1, elementsToCheck);

        for (uint32 i = 10; (i < 25) && ok; i++) {
            ok &= (((uint8 *) ptr[0])[i] == 0);
            ok &= (((int8 *) ptr[1])[i] == 0);
            ok &= (((uint16 *) ptr[2])[i] == 0);
            ok &= (((int16 *) ptr[3])[i] == 0);
            ok &= (((uint32 *) ptr[4])[i] == 0);
            ok &= (((int32 *) ptr[5])[i] == 0);
            ok &= (((uint64 *) ptr[6])[i] == 0);
            ok &= (((int64 *) ptr[7])[i] == 0);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[i], static_cast<float32>(0));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[i], 0);
        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise19() {
    MDSReaderTestHelper dS(treeName);
    dS.elementsRead = 5;
    uint32 elementsToCheck = 0;
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 1, 0, 2);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    if (ok) {
        for (uint32 m = 0u; m < 4; m++) {

            for (uint32 i = 0u; i < 4; i++) {
                ok = dS.Synchronise();
                elementsToCheck = 5;
                if (ok) {
                    ok &= dS.CompareS_uint8(((uint8 *) ptr[0]), (m * 4) + i, 1, elementsToCheck);
                    ok &= dS.CompareS_int8(((int8 *) ptr[1]), (m * 4) + i, 1, elementsToCheck);
                    ok &= dS.CompareS_uint16(((uint16 *) ptr[2]), (m * 4) + i, 2, elementsToCheck);
                    ok &= dS.CompareS_int16(((int16 *) ptr[3]), (m * 4) + i, 2, elementsToCheck);
                    ok &= dS.CompareS_uint32(((uint32 *) ptr[4]), (m * 4) + i, 4, elementsToCheck);
                    ok &= dS.CompareS_int32(((int32 *) ptr[5]), (m * 4) + i, 4, elementsToCheck);
                    ok &= dS.CompareS_uint64(((uint64 *) ptr[6]), (m * 4) + i, 8, elementsToCheck);
                    ok &= dS.CompareS_int64(((int64 *) ptr[7]), (m * 4) + i, 8, elementsToCheck);
                    ok &= dS.CompareS_float32(((float32 *) ptr[8]), (m * 4) + i, 0.1, elementsToCheck);
                    ok &= dS.CompareS_float64(((float64 *) ptr[9]), (m * 4) + i, 0.1, elementsToCheck);
                }
            }
            for (uint32 i = 0u; i < 4; i++) {
                ok = dS.Synchronise();
                for (uint32 j = 0; (j < dS.elementsRead) && ok; j++) {
                    ok &= (((uint8 *) ptr[0])[j] == 0);
                    int8 aux = int8(0);
                    ok &= (((int8 *) ptr[1])[j] == aux);
                    ok &= (((uint16 *) ptr[2])[j] == 0);
                    int16 aux16 = int16(0);
                    ok &= (((int16 *) ptr[3])[j] == aux16);
                    ok &= (((uint32 *) ptr[4])[j] == 0);
                    int32 aux32 = int32(0);
                    ok &= (((int32 *) ptr[5])[j] == aux32);
                    ok &= (((uint64 *) ptr[6])[j] == 0);
                    int64 aux64 = int64(0);
                    ok &= (((int64 *) ptr[7])[j] == aux64);
                    ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[i], static_cast<float32>(0));
                    ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[i], 0);
                }
            }

        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise20() {
    MDSReaderTestHelper dS(treeName);
    dS.elementsRead = 15;
    uint32 elementsToCheck = 0;
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 1, 1, 2);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    if (ok) {

        ok = dS.Synchronise();
        elementsToCheck = 15;
        if (ok) {
            ok &= dS.CompareS_uint8(((uint8 *) ptr[0]), 0, 1, elementsToCheck);
            ok &= dS.CompareS_int8(((int8 *) ptr[1]), 0, 1, elementsToCheck);
            ok &= dS.CompareS_uint16(((uint16 *) ptr[2]), 0, 2, elementsToCheck);
            ok &= dS.CompareS_int16(((int16 *) ptr[3]), 0, 2, elementsToCheck);
            ok &= dS.CompareS_uint32(((uint32 *) ptr[4]), 0, 4, elementsToCheck);
            ok &= dS.CompareS_int32(((int32 *) ptr[5]), 0, 4, elementsToCheck);
            ok &= dS.CompareS_uint64(((uint64 *) ptr[6]), 0, 8, elementsToCheck);
            ok &= dS.CompareS_int64(((int64 *) ptr[7]), 0, 8, elementsToCheck);
            ok &= dS.CompareS_float32(((float32 *) ptr[8]), 0, 0.1, elementsToCheck);
            ok &= dS.CompareS_float64(((float64 *) ptr[9]), 0, 0.1, elementsToCheck);
        }
    }
    if (ok) {
        ok = dS.Synchronise();
        elementsToCheck = 5;
        if (ok) {
            ok &= dS.CompareS_uint8(((uint8 *) ptr[0]), 1, 1, elementsToCheck);
            ok &= dS.CompareS_int8(((int8 *) ptr[1]), 1, 1, elementsToCheck);
            ok &= dS.CompareS_uint16(((uint16 *) ptr[2]), 1, 2, elementsToCheck);
            ok &= dS.CompareS_int16(((int16 *) ptr[3]), 1, 2, elementsToCheck);
            ok &= dS.CompareS_uint32(((uint32 *) ptr[4]), 1, 4, elementsToCheck);
            ok &= dS.CompareS_int32(((int32 *) ptr[5]), 1, 4, elementsToCheck);
            ok &= dS.CompareS_uint64(((uint64 *) ptr[6]), 1, 8, elementsToCheck);
            ok &= dS.CompareS_int64(((int64 *) ptr[7]), 1, 8, elementsToCheck);
            ok &= dS.CompareS_float32(((float32 *) ptr[8]), 1, 0.1, elementsToCheck);
            ok &= dS.CompareS_float64(((float64 *) ptr[9]), 1, 0.1, elementsToCheck);
        }
        for (uint32 i = elementsToCheck; (i < 15) && ok; i++) {
            ok &= (((uint8 *) ptr[0])[i] == 19);
            ok &= (((int8 *) ptr[1])[i] == -1);
            ok &= (((uint16 *) ptr[2])[i] == 38);
            ok &= (((int16 *) ptr[3])[i] == -2);
            ok &= (((uint32 *) ptr[4])[i] == 76);
            ok &= (((int32 *) ptr[5])[i] == -4);
            ok &= (((uint64 *) ptr[6])[i] == 152);
            ok &= (((int64 *) ptr[7])[i] == -8);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[i], static_cast<float32>(1.9));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[i], 1.9);
        }
    }
    if (ok) {
        ok = dS.Synchronise();
        for (uint32 i = 0u; (i < 10) && ok; i++) {
            ok &= (((uint8 *) ptr[0])[i] == 19);
            ok &= (((int8 *) ptr[1])[i] == -1);
            ok &= (((uint16 *) ptr[2])[i] == 38);
            ok &= (((int16 *) ptr[3])[i] == -2);
            ok &= (((uint32 *) ptr[4])[i] == 76);
            ok &= (((int32 *) ptr[5])[i] == -4);
            ok &= (((uint64 *) ptr[6])[i] == 152);
            ok &= (((int64 *) ptr[7])[i] == -8);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[i], static_cast<float32>(1.9));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[i], 1.9);
        }
        elementsToCheck = 5;
        ok &= dS.CompareS_uint8((&((uint8 *) ptr[0])[10]), 2, 1, elementsToCheck);
        ok &= dS.CompareS_int8((&((int8 *) ptr[1])[10]), 2, 1, elementsToCheck);
        ok &= dS.CompareS_uint16((&((uint16 *) ptr[2])[10]), 2, 2, elementsToCheck);
        ok &= dS.CompareS_int16((&((int16 *) ptr[3])[10]), 2, 2, elementsToCheck);
        ok &= dS.CompareS_uint32((&((uint32 *) ptr[4])[10]), 2, 4, elementsToCheck);
        ok &= dS.CompareS_int32((&((int32 *) ptr[5])[10]), 2, 4, elementsToCheck);
        ok &= dS.CompareS_uint64((&((uint64 *) ptr[6])[10]), 2, 8, elementsToCheck);
        ok &= dS.CompareS_int64((&((int64 *) ptr[7])[10]), 2, 8, elementsToCheck);
        ok &= dS.CompareS_float32((&((float32 *) ptr[8])[10]), 2, 0.1, elementsToCheck);
        ok &= dS.CompareS_float64((&((float64 *) ptr[9])[10]), 2, 0.1, elementsToCheck);
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise21() {
    MDSReaderTestHelper dS(treeName);
    dS.elementsRead = 25;
    uint32 elementsToCheck = 0;
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 1, 1, 2);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    if (ok) {

        ok = dS.Synchronise();
        elementsToCheck = 20;
        if (ok) {
            ok &= dS.CompareS_uint8(((uint8 *) ptr[0]), 0, 1, elementsToCheck);
            ok &= dS.CompareS_int8(((int8 *) ptr[1]), 0, 1, elementsToCheck);
            ok &= dS.CompareS_uint16(((uint16 *) ptr[2]), 0, 2, elementsToCheck);
            ok &= dS.CompareS_int16(((int16 *) ptr[3]), 0, 2, elementsToCheck);
            ok &= dS.CompareS_uint32(((uint32 *) ptr[4]), 0, 4, elementsToCheck);
            ok &= dS.CompareS_int32(((int32 *) ptr[5]), 0, 4, elementsToCheck);
            ok &= dS.CompareS_uint64(((uint64 *) ptr[6]), 0, 8, elementsToCheck);
            ok &= dS.CompareS_int64(((int64 *) ptr[7]), 0, 8, elementsToCheck);
            ok &= dS.CompareS_float32(((float32 *) ptr[8]), 0, 0.1, elementsToCheck);
            ok &= dS.CompareS_float64(((float64 *) ptr[9]), 0, 0.1, elementsToCheck);
        }
        for (uint32 i = elementsToCheck; (i < dS.elementsRead) && ok; i++) {
            ok &= (((uint8 *) ptr[0])[i] == 19);
            ok &= (((int8 *) ptr[1])[i] == -1);
            ok &= (((uint16 *) ptr[2])[i] == 38);
            ok &= (((int16 *) ptr[3])[i] == -2);
            ok &= (((uint32 *) ptr[4])[i] == 76);
            ok &= (((int32 *) ptr[5])[i] == -4);
            ok &= (((uint64 *) ptr[6])[i] == 152);
            ok &= (((int64 *) ptr[7])[i] == -8);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[i], static_cast<float32>(1.9));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[i], 1.9);
        }
    }
    if (ok) {
        ok = dS.Synchronise();
        elementsToCheck = 10;
        for (uint32 i = 0u; (i < 15) && ok; i++) {
            ok &= (((uint8 *) ptr[0])[i] == 19);
            ok &= (((int8 *) ptr[1])[i] == -1);
            ok &= (((uint16 *) ptr[2])[i] == 38);
            ok &= (((int16 *) ptr[3])[i] == -2);
            ok &= (((uint32 *) ptr[4])[i] == 76);
            ok &= (((int32 *) ptr[5])[i] == -4);
            ok &= (((uint64 *) ptr[6])[i] == 152);
            ok &= (((int64 *) ptr[7])[i] == -8);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[i], static_cast<float32>(1.9));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[i], 1.9);
        }
        ok &= dS.CompareS_uint8((&((uint8 *) ptr[0])[15]), 1, 1, elementsToCheck);
        ok &= dS.CompareS_int8((&((int8 *) ptr[1])[15]), 1, 1, elementsToCheck);
        ok &= dS.CompareS_uint16((&((uint16 *) ptr[2])[15]), 1, 2, elementsToCheck);
        ok &= dS.CompareS_int16((&((int16 *) ptr[3])[15]), 1, 2, elementsToCheck);
        ok &= dS.CompareS_uint32((&((uint32 *) ptr[4])[15]), 1, 4, elementsToCheck);
        ok &= dS.CompareS_int32((&((int32 *) ptr[5])[15]), 1, 4, elementsToCheck);
        ok &= dS.CompareS_uint64((&((uint64 *) ptr[6])[15]), 1, 8, elementsToCheck);
        ok &= dS.CompareS_int64((&((int64 *) ptr[7])[15]), 1, 8, elementsToCheck);
        ok &= dS.CompareS_float32((&((float32 *) ptr[8])[15]), 1, 0.1, elementsToCheck);
        ok &= dS.CompareS_float64((&((float64 *) ptr[9])[15]), 1, 0.1, elementsToCheck);

    }
    if (ok) {
        elementsToCheck = 10;
        ok = dS.Synchronise();
        ok &= dS.CompareS_uint8((&((uint8 *) ptr[0])[0]), 2, 1, elementsToCheck);
        ok &= dS.CompareS_int8((&((int8 *) ptr[1])[0]), 2, 1, elementsToCheck);
        ok &= dS.CompareS_uint16((&((uint16 *) ptr[2])[0]), 2, 2, elementsToCheck);
        ok &= dS.CompareS_int16((&((int16 *) ptr[3])[0]), 2, 2, elementsToCheck);
        ok &= dS.CompareS_uint32((&((uint32 *) ptr[4])[0]), 2, 4, elementsToCheck);
        ok &= dS.CompareS_int32((&((int32 *) ptr[5])[0]), 2, 4, elementsToCheck);
        ok &= dS.CompareS_uint64((&((uint64 *) ptr[6])[0]), 2, 8, elementsToCheck);
        ok &= dS.CompareS_int64((&((int64 *) ptr[7])[0]), 2, 8, elementsToCheck);
        ok &= dS.CompareS_float32((&((float32 *) ptr[8])[0]), 2, 0.1, elementsToCheck);
        ok &= dS.CompareS_float64((&((float64 *) ptr[9])[0]), 2, 0.1, elementsToCheck);

        for (uint32 i = 10; (i < 25) && ok; i++) {
            ok &= (((uint8 *) ptr[0])[i] == 39);
            ok &= (((int8 *) ptr[1])[i] == 19);
            ok &= (((uint16 *) ptr[2])[i] == 78);
            ok &= (((int16 *) ptr[3])[i] == 38);
            ok &= (((uint32 *) ptr[4])[i] == 156);
            ok &= (((int32 *) ptr[5])[i] == 76);
            ok &= (((uint64 *) ptr[6])[i] == 312);
            ok &= (((int64 *) ptr[7])[i] == 152);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[i], static_cast<float32>(3.9));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[i], 3.9);
        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise22() {
    MDSReaderTestHelper dS(treeName);
    dS.elementsRead = 5;
    uint32 elementsToCheck = 0;
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 1, 1, 2);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    if (ok) {
        for (uint32 m = 0u; m < 4; m++) {

            for (uint32 i = 0u; i < 4; i++) {
                ok = dS.Synchronise();
                elementsToCheck = 5;
                if (ok) {
                    ok &= dS.CompareS_uint8(((uint8 *) ptr[0]), (m * 4) + i, 1, elementsToCheck);
                    ok &= dS.CompareS_int8(((int8 *) ptr[1]), (m * 4) + i, 1, elementsToCheck);
                    ok &= dS.CompareS_uint16(((uint16 *) ptr[2]), (m * 4) + i, 2, elementsToCheck);
                    ok &= dS.CompareS_int16(((int16 *) ptr[3]), (m * 4) + i, 2, elementsToCheck);
                    ok &= dS.CompareS_uint32(((uint32 *) ptr[4]), (m * 4) + i, 4, elementsToCheck);
                    ok &= dS.CompareS_int32(((int32 *) ptr[5]), (m * 4) + i, 4, elementsToCheck);
                    ok &= dS.CompareS_uint64(((uint64 *) ptr[6]), (m * 4) + i, 8, elementsToCheck);
                    ok &= dS.CompareS_int64(((int64 *) ptr[7]), (m * 4) + i, 8, elementsToCheck);
                    ok &= dS.CompareS_float32(((float32 *) ptr[8]), (m * 4) + i, 0.1, elementsToCheck);
                    ok &= dS.CompareS_float64(((float64 *) ptr[9]), (m * 4) + i, 0.1, elementsToCheck);
                }
            }
            for (uint32 i = 0u; i < 4; i++) {
                ok = dS.Synchronise();
                for (uint32 j = 0; (j < dS.elementsRead) && ok; j++) {
                    ok &= (((uint8 *) ptr[0])[j] == 20 * (m + 1) - 1);
                    int8 aux = int8(20 * (m + 1) - 1) - 20;
                    ok &= (((int8 *) ptr[1])[j] == aux);
                    ok &= (((uint16 *) ptr[2])[j] == 40 * (m + 1) - 2);
                    int16 aux16 = int16(40 * (m + 1) - 2) - 40;
                    ok &= (((int16 *) ptr[3])[j] == aux16);
                    ok &= (((uint32 *) ptr[4])[j] == 80 * (m + 1) - 4);
                    int32 aux32 = int32(80 * (m + 1) - 4) - 80;
                    ok &= (((int32 *) ptr[5])[j] == aux32);
                    ok &= (((uint64 *) ptr[6])[j] == 160 * (m + 1) - 8);
                    int64 aux64 = int64(160 * (m + 1) - 8) - 160;
                    ok &= (((int64 *) ptr[7])[j] == aux64);
                    ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[i], static_cast<float32>(2 * (m + 1) - 0.1));
                    ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[i], 2 * (m + 1) - 0.1);
                }
            }

        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise23() {
    MDSReaderTestHelper dS(treeName);
    dS.elementsRead = 15;
    uint32 elementsToCheck = 0;
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 2, 0, 2);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    if (ok) {

        ok = dS.Synchronise();
        elementsToCheck = 15;
        if (ok) {
            ok &= dS.CompareS_uint8(((uint8 *) ptr[0]), 0, 1, elementsToCheck);
            ok &= dS.CompareS_int8(((int8 *) ptr[1]), 0, 1, elementsToCheck);
            ok &= dS.CompareS_uint16(((uint16 *) ptr[2]), 0, 2, elementsToCheck);
            ok &= dS.CompareS_int16(((int16 *) ptr[3]), 0, 2, elementsToCheck);
            ok &= dS.CompareS_uint32(((uint32 *) ptr[4]), 0, 4, elementsToCheck);
            ok &= dS.CompareS_int32(((int32 *) ptr[5]), 0, 4, elementsToCheck);
            ok &= dS.CompareS_uint64(((uint64 *) ptr[6]), 0, 8, elementsToCheck);
            ok &= dS.CompareS_int64(((int64 *) ptr[7]), 0, 8, elementsToCheck);
            ok &= dS.CompareS_float32(((float32 *) ptr[8]), 0, 0.1, elementsToCheck);
            ok &= dS.CompareS_float64(((float64 *) ptr[9]), 0, 0.1, elementsToCheck);
        }
    }
    if (ok) {
        ok = dS.Synchronise();
        elementsToCheck = 5;
        if (ok) {
            ok &= dS.CompareS_uint8(((uint8 *) ptr[0]), 1, 1, elementsToCheck);
            ok &= dS.CompareS_int8(((int8 *) ptr[1]), 1, 1, elementsToCheck);
            ok &= dS.CompareS_uint16(((uint16 *) ptr[2]), 1, 2, elementsToCheck);
            ok &= dS.CompareS_int16(((int16 *) ptr[3]), 1, 2, elementsToCheck);
            ok &= dS.CompareS_uint32(((uint32 *) ptr[4]), 1, 4, elementsToCheck);
            ok &= dS.CompareS_int32(((int32 *) ptr[5]), 1, 4, elementsToCheck);
            ok &= dS.CompareS_uint64(((uint64 *) ptr[6]), 1, 8, elementsToCheck);
            ok &= dS.CompareS_int64(((int64 *) ptr[7]), 1, 8, elementsToCheck);
            ok &= dS.CompareS_float32(((float32 *) ptr[8]), 1, 0.1, elementsToCheck);
            ok &= dS.CompareS_float64(((float64 *) ptr[9]), 1, 0.1, elementsToCheck);
        }
        for (uint32 i = elementsToCheck; (i < 15) && ok; i++) {
            ok &= (((uint8 *) ptr[0])[i] == 0);
            ok &= (((int8 *) ptr[1])[i] == 0);
            ok &= (((uint16 *) ptr[2])[i] == 0);
            ok &= (((int16 *) ptr[3])[i] == 0);
            ok &= (((uint32 *) ptr[4])[i] == 0);
            ok &= (((int32 *) ptr[5])[i] == 0);
            ok &= (((uint64 *) ptr[6])[i] == 0);
            ok &= (((int64 *) ptr[7])[i] == 0);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[i], static_cast<float32>(0));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[i], 0);
        }
    }
    if (ok) {
        ok = dS.Synchronise();
        for (uint32 i = 0u; (i < 10) && ok; i++) {
            ok &= (((uint8 *) ptr[0])[i] == 0);
            ok &= (((int8 *) ptr[1])[i] == 0);
            ok &= (((uint16 *) ptr[2])[i] == 0);
            ok &= (((int16 *) ptr[3])[i] == 0);
            ok &= (((uint32 *) ptr[4])[i] == 0);
            ok &= (((int32 *) ptr[5])[i] == 0);
            ok &= (((uint64 *) ptr[6])[i] == 0);
            ok &= (((int64 *) ptr[7])[i] == 0);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[i], static_cast<float32>(0));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[i], 0);
        }
        elementsToCheck = 5;
        ok &= dS.CompareS_uint8((&((uint8 *) ptr[0])[10]), 2, 1, elementsToCheck);
        ok &= dS.CompareS_int8((&((int8 *) ptr[1])[10]), 2, 1, elementsToCheck);
        ok &= dS.CompareS_uint16((&((uint16 *) ptr[2])[10]), 2, 2, elementsToCheck);
        ok &= dS.CompareS_int16((&((int16 *) ptr[3])[10]), 2, 2, elementsToCheck);
        ok &= dS.CompareS_uint32((&((uint32 *) ptr[4])[10]), 2, 4, elementsToCheck);
        ok &= dS.CompareS_int32((&((int32 *) ptr[5])[10]), 2, 4, elementsToCheck);
        ok &= dS.CompareS_uint64((&((uint64 *) ptr[6])[10]), 2, 8, elementsToCheck);
        ok &= dS.CompareS_int64((&((int64 *) ptr[7])[10]), 2, 8, elementsToCheck);
        ok &= dS.CompareS_float32((&((float32 *) ptr[8])[10]), 2, 0.1, elementsToCheck);
        ok &= dS.CompareS_float64((&((float64 *) ptr[9])[10]), 2, 0.1, elementsToCheck);
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise24() {
    MDSReaderTestHelper dS(treeName);
    dS.elementsRead = 25;
    uint32 elementsToCheck = 0;
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 2, 0, 2);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    if (ok) {

        ok = dS.Synchronise();
        elementsToCheck = 20;
        if (ok) {
            ok &= dS.CompareS_uint8(((uint8 *) ptr[0]), 0, 1, elementsToCheck);
            ok &= dS.CompareS_int8(((int8 *) ptr[1]), 0, 1, elementsToCheck);
            ok &= dS.CompareS_uint16(((uint16 *) ptr[2]), 0, 2, elementsToCheck);
            ok &= dS.CompareS_int16(((int16 *) ptr[3]), 0, 2, elementsToCheck);
            ok &= dS.CompareS_uint32(((uint32 *) ptr[4]), 0, 4, elementsToCheck);
            ok &= dS.CompareS_int32(((int32 *) ptr[5]), 0, 4, elementsToCheck);
            ok &= dS.CompareS_uint64(((uint64 *) ptr[6]), 0, 8, elementsToCheck);
            ok &= dS.CompareS_int64(((int64 *) ptr[7]), 0, 8, elementsToCheck);
            ok &= dS.CompareS_float32(((float32 *) ptr[8]), 0, 0.1, elementsToCheck);
            ok &= dS.CompareS_float64(((float64 *) ptr[9]), 0, 0.1, elementsToCheck);
        }
        for (uint32 i = elementsToCheck; (i < dS.elementsRead) && ok; i++) {
            ok &= (((uint8 *) ptr[0])[i] == 0);
            ok &= (((int8 *) ptr[1])[i] == 0);
            ok &= (((uint16 *) ptr[2])[i] == 0);
            ok &= (((int16 *) ptr[3])[i] == 0);
            ok &= (((uint32 *) ptr[4])[i] == 0);
            ok &= (((int32 *) ptr[5])[i] == 0);
            ok &= (((uint64 *) ptr[6])[i] == 0);
            ok &= (((int64 *) ptr[7])[i] == 0);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[i], static_cast<float32>(0));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[i], 0);
        }
    }
    if (ok) {
        ok = dS.Synchronise();
        elementsToCheck = 10;
        for (uint32 i = 0u; (i < 15) && ok; i++) {
            ok &= (((uint8 *) ptr[0])[i] == 0);
            ok &= (((int8 *) ptr[1])[i] == 0);
            ok &= (((uint16 *) ptr[2])[i] == 0);
            ok &= (((int16 *) ptr[3])[i] == 0);
            ok &= (((uint32 *) ptr[4])[i] == 0);
            ok &= (((int32 *) ptr[5])[i] == 0);
            ok &= (((uint64 *) ptr[6])[i] == 0);
            ok &= (((int64 *) ptr[7])[i] == 0);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[i], static_cast<float32>(0));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[i], 0);
        }
        ok &= dS.CompareS_uint8((&((uint8 *) ptr[0])[15]), 1, 1, elementsToCheck);
        ok &= dS.CompareS_int8((&((int8 *) ptr[1])[15]), 1, 1, elementsToCheck);
        ok &= dS.CompareS_uint16((&((uint16 *) ptr[2])[15]), 1, 2, elementsToCheck);
        ok &= dS.CompareS_int16((&((int16 *) ptr[3])[15]), 1, 2, elementsToCheck);
        ok &= dS.CompareS_uint32((&((uint32 *) ptr[4])[15]), 1, 4, elementsToCheck);
        ok &= dS.CompareS_int32((&((int32 *) ptr[5])[15]), 1, 4, elementsToCheck);
        ok &= dS.CompareS_uint64((&((uint64 *) ptr[6])[15]), 1, 8, elementsToCheck);
        ok &= dS.CompareS_int64((&((int64 *) ptr[7])[15]), 1, 8, elementsToCheck);
        ok &= dS.CompareS_float32((&((float32 *) ptr[8])[15]), 1, 0.1, elementsToCheck);
        ok &= dS.CompareS_float64((&((float64 *) ptr[9])[15]), 1, 0.1, elementsToCheck);

    }
    if (ok) {
        elementsToCheck = 10;
        ok = dS.Synchronise();
        ok &= dS.CompareS_uint8((&((uint8 *) ptr[0])[0]), 2, 1, elementsToCheck);
        ok &= dS.CompareS_int8((&((int8 *) ptr[1])[0]), 2, 1, elementsToCheck);
        ok &= dS.CompareS_uint16((&((uint16 *) ptr[2])[0]), 2, 2, elementsToCheck);
        ok &= dS.CompareS_int16((&((int16 *) ptr[3])[0]), 2, 2, elementsToCheck);
        ok &= dS.CompareS_uint32((&((uint32 *) ptr[4])[0]), 2, 4, elementsToCheck);
        ok &= dS.CompareS_int32((&((int32 *) ptr[5])[0]), 2, 4, elementsToCheck);
        ok &= dS.CompareS_uint64((&((uint64 *) ptr[6])[0]), 2, 8, elementsToCheck);
        ok &= dS.CompareS_int64((&((int64 *) ptr[7])[0]), 2, 8, elementsToCheck);
        ok &= dS.CompareS_float32((&((float32 *) ptr[8])[0]), 2, 0.1, elementsToCheck);
        ok &= dS.CompareS_float64((&((float64 *) ptr[9])[0]), 2, 0.1, elementsToCheck);

        for (uint32 i = 10; (i < 25) && ok; i++) {
            ok &= (((uint8 *) ptr[0])[i] == 0);
            ok &= (((int8 *) ptr[1])[i] == 0);
            ok &= (((uint16 *) ptr[2])[i] == 0);
            ok &= (((int16 *) ptr[3])[i] == 0);
            ok &= (((uint32 *) ptr[4])[i] == 0);
            ok &= (((int32 *) ptr[5])[i] == 0);
            ok &= (((uint64 *) ptr[6])[i] == 0);
            ok &= (((int64 *) ptr[7])[i] == 0);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[i], static_cast<float32>(0));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[i], 0);
        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise25() {
    MDSReaderTestHelper dS(treeName);
    dS.elementsRead = 5;
    uint32 elementsToCheck = 0;
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 2, 0, 2);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    if (ok) {
        for (uint32 m = 0u; m < 4; m++) {

            for (uint32 i = 0u; i < 4; i++) {
                ok = dS.Synchronise();
                elementsToCheck = 5;
                if (ok) {
                    ok &= dS.CompareS_uint8(((uint8 *) ptr[0]), (m * 4) + i, 1, elementsToCheck);
                    ok &= dS.CompareS_int8(((int8 *) ptr[1]), (m * 4) + i, 1, elementsToCheck);
                    ok &= dS.CompareS_uint16(((uint16 *) ptr[2]), (m * 4) + i, 2, elementsToCheck);
                    ok &= dS.CompareS_int16(((int16 *) ptr[3]), (m * 4) + i, 2, elementsToCheck);
                    ok &= dS.CompareS_uint32(((uint32 *) ptr[4]), (m * 4) + i, 4, elementsToCheck);
                    ok &= dS.CompareS_int32(((int32 *) ptr[5]), (m * 4) + i, 4, elementsToCheck);
                    ok &= dS.CompareS_uint64(((uint64 *) ptr[6]), (m * 4) + i, 8, elementsToCheck);
                    ok &= dS.CompareS_int64(((int64 *) ptr[7]), (m * 4) + i, 8, elementsToCheck);
                    ok &= dS.CompareS_float32(((float32 *) ptr[8]), (m * 4) + i, 0.1, elementsToCheck);
                    ok &= dS.CompareS_float64(((float64 *) ptr[9]), (m * 4) + i, 0.1, elementsToCheck);
                }
            }
            for (uint32 i = 0u; i < 4; i++) {
                ok = dS.Synchronise();
                for (uint32 j = 0; (j < dS.elementsRead) && ok; j++) {
                    ok &= (((uint8 *) ptr[0])[j] == 0);
                    int8 aux = int8(0);
                    ok &= (((int8 *) ptr[1])[j] == aux);
                    ok &= (((uint16 *) ptr[2])[j] == 0);
                    int16 aux16 = int16(0);
                    ok &= (((int16 *) ptr[3])[j] == aux16);
                    ok &= (((uint32 *) ptr[4])[j] == 0);
                    int32 aux32 = int32(0);
                    ok &= (((int32 *) ptr[5])[j] == aux32);
                    ok &= (((uint64 *) ptr[6])[j] == 0);
                    int64 aux64 = int64(0);
                    ok &= (((int64 *) ptr[7])[j] == aux64);
                    ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[i], static_cast<float32>(0));
                    ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[i], 0);
                }
            }

        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise26() {
    MDSReaderTestHelper dS(treeName);
    dS.elementsRead = 15;
    uint32 elementsToCheck = 0;
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 2, 1, 2);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    if (ok) {

        ok = dS.Synchronise();
        elementsToCheck = 15;
        if (ok) {
            ok &= dS.CompareS_uint8(((uint8 *) ptr[0]), 0, 1, elementsToCheck);
            ok &= dS.CompareS_int8(((int8 *) ptr[1]), 0, 1, elementsToCheck);
            ok &= dS.CompareS_uint16(((uint16 *) ptr[2]), 0, 2, elementsToCheck);
            ok &= dS.CompareS_int16(((int16 *) ptr[3]), 0, 2, elementsToCheck);
            ok &= dS.CompareS_uint32(((uint32 *) ptr[4]), 0, 4, elementsToCheck);
            ok &= dS.CompareS_int32(((int32 *) ptr[5]), 0, 4, elementsToCheck);
            ok &= dS.CompareS_uint64(((uint64 *) ptr[6]), 0, 8, elementsToCheck);
            ok &= dS.CompareS_int64(((int64 *) ptr[7]), 0, 8, elementsToCheck);
            ok &= dS.CompareS_float32(((float32 *) ptr[8]), 0, 0.1, elementsToCheck);
            ok &= dS.CompareS_float64(((float64 *) ptr[9]), 0, 0.1, elementsToCheck);
        }
    }
    if (ok) {
        ok = dS.Synchronise();
        elementsToCheck = 5;
        if (ok) {
            ok &= dS.CompareS_uint8(((uint8 *) ptr[0]), 1, 1, elementsToCheck);
            ok &= dS.CompareS_int8(((int8 *) ptr[1]), 1, 1, elementsToCheck);
            ok &= dS.CompareS_uint16(((uint16 *) ptr[2]), 1, 2, elementsToCheck);
            ok &= dS.CompareS_int16(((int16 *) ptr[3]), 1, 2, elementsToCheck);
            ok &= dS.CompareS_uint32(((uint32 *) ptr[4]), 1, 4, elementsToCheck);
            ok &= dS.CompareS_int32(((int32 *) ptr[5]), 1, 4, elementsToCheck);
            ok &= dS.CompareS_uint64(((uint64 *) ptr[6]), 1, 8, elementsToCheck);
            ok &= dS.CompareS_int64(((int64 *) ptr[7]), 1, 8, elementsToCheck);
            ok &= dS.CompareS_float32(((float32 *) ptr[8]), 1, 0.1, elementsToCheck);
            ok &= dS.CompareS_float64(((float64 *) ptr[9]), 1, 0.1, elementsToCheck);
        }
        for (uint32 i = elementsToCheck; (i < 15) && ok; i++) {
            ok &= (((uint8 *) ptr[0])[i] == 19);
            ok &= (((int8 *) ptr[1])[i] == -1);
            ok &= (((uint16 *) ptr[2])[i] == 38);
            ok &= (((int16 *) ptr[3])[i] == -2);
            ok &= (((uint32 *) ptr[4])[i] == 76);
            ok &= (((int32 *) ptr[5])[i] == -4);
            ok &= (((uint64 *) ptr[6])[i] == 152);
            ok &= (((int64 *) ptr[7])[i] == -8);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[i], static_cast<float32>(1.9));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[i], 1.9);
        }
    }
    if (ok) {
        ok = dS.Synchronise();
        for (uint32 i = 0u; (i < 10) && ok; i++) {
            ok &= (((uint8 *) ptr[0])[i] == 19);
            ok &= (((int8 *) ptr[1])[i] == -1);
            ok &= (((uint16 *) ptr[2])[i] == 38);
            ok &= (((int16 *) ptr[3])[i] == -2);
            ok &= (((uint32 *) ptr[4])[i] == 76);
            ok &= (((int32 *) ptr[5])[i] == -4);
            ok &= (((uint64 *) ptr[6])[i] == 152);
            ok &= (((int64 *) ptr[7])[i] == -8);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[i], static_cast<float32>(1.9));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[i], 1.9);
        }
        elementsToCheck = 5;
        ok &= dS.CompareS_uint8((&((uint8 *) ptr[0])[10]), 2, 1, elementsToCheck);
        ok &= dS.CompareS_int8((&((int8 *) ptr[1])[10]), 2, 1, elementsToCheck);
        ok &= dS.CompareS_uint16((&((uint16 *) ptr[2])[10]), 2, 2, elementsToCheck);
        ok &= dS.CompareS_int16((&((int16 *) ptr[3])[10]), 2, 2, elementsToCheck);
        ok &= dS.CompareS_uint32((&((uint32 *) ptr[4])[10]), 2, 4, elementsToCheck);
        ok &= dS.CompareS_int32((&((int32 *) ptr[5])[10]), 2, 4, elementsToCheck);
        ok &= dS.CompareS_uint64((&((uint64 *) ptr[6])[10]), 2, 8, elementsToCheck);
        ok &= dS.CompareS_int64((&((int64 *) ptr[7])[10]), 2, 8, elementsToCheck);
        ok &= dS.CompareS_float32((&((float32 *) ptr[8])[10]), 2, 0.1, elementsToCheck);
        ok &= dS.CompareS_float64((&((float64 *) ptr[9])[10]), 2, 0.1, elementsToCheck);
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise27() {
    MDSReaderTestHelper dS(treeName);
    dS.elementsRead = 25;
    uint32 elementsToCheck = 0;
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 2, 1, 2);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    if (ok) {

        ok = dS.Synchronise();
        elementsToCheck = 20;
        if (ok) {
            ok &= dS.CompareS_uint8(((uint8 *) ptr[0]), 0, 1, elementsToCheck);
            ok &= dS.CompareS_int8(((int8 *) ptr[1]), 0, 1, elementsToCheck);
            ok &= dS.CompareS_uint16(((uint16 *) ptr[2]), 0, 2, elementsToCheck);
            ok &= dS.CompareS_int16(((int16 *) ptr[3]), 0, 2, elementsToCheck);
            ok &= dS.CompareS_uint32(((uint32 *) ptr[4]), 0, 4, elementsToCheck);
            ok &= dS.CompareS_int32(((int32 *) ptr[5]), 0, 4, elementsToCheck);
            ok &= dS.CompareS_uint64(((uint64 *) ptr[6]), 0, 8, elementsToCheck);
            ok &= dS.CompareS_int64(((int64 *) ptr[7]), 0, 8, elementsToCheck);
            ok &= dS.CompareS_float32(((float32 *) ptr[8]), 0, 0.1, elementsToCheck);
            ok &= dS.CompareS_float64(((float64 *) ptr[9]), 0, 0.1, elementsToCheck);
        }
        for (uint32 i = elementsToCheck; (i < dS.elementsRead) && ok; i++) {
            ok &= (((uint8 *) ptr[0])[i] == 19);
            ok &= (((int8 *) ptr[1])[i] == -1);
            ok &= (((uint16 *) ptr[2])[i] == 38);
            ok &= (((int16 *) ptr[3])[i] == -2);
            ok &= (((uint32 *) ptr[4])[i] == 76);
            ok &= (((int32 *) ptr[5])[i] == -4);
            ok &= (((uint64 *) ptr[6])[i] == 152);
            ok &= (((int64 *) ptr[7])[i] == -8);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[i], static_cast<float32>(1.9));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[i], 1.9);
        }
    }
    if (ok) {
        ok = dS.Synchronise();
        elementsToCheck = 10;
        for (uint32 i = 0u; (i < 15) && ok; i++) {
            ok &= (((uint8 *) ptr[0])[i] == 19);
            ok &= (((int8 *) ptr[1])[i] == -1);
            ok &= (((uint16 *) ptr[2])[i] == 38);
            ok &= (((int16 *) ptr[3])[i] == -2);
            ok &= (((uint32 *) ptr[4])[i] == 76);
            ok &= (((int32 *) ptr[5])[i] == -4);
            ok &= (((uint64 *) ptr[6])[i] == 152);
            ok &= (((int64 *) ptr[7])[i] == -8);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[i], static_cast<float32>(1.9));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[i], 1.9);
        }
        ok &= dS.CompareS_uint8((&((uint8 *) ptr[0])[15]), 1, 1, elementsToCheck);
        ok &= dS.CompareS_int8((&((int8 *) ptr[1])[15]), 1, 1, elementsToCheck);
        ok &= dS.CompareS_uint16((&((uint16 *) ptr[2])[15]), 1, 2, elementsToCheck);
        ok &= dS.CompareS_int16((&((int16 *) ptr[3])[15]), 1, 2, elementsToCheck);
        ok &= dS.CompareS_uint32((&((uint32 *) ptr[4])[15]), 1, 4, elementsToCheck);
        ok &= dS.CompareS_int32((&((int32 *) ptr[5])[15]), 1, 4, elementsToCheck);
        ok &= dS.CompareS_uint64((&((uint64 *) ptr[6])[15]), 1, 8, elementsToCheck);
        ok &= dS.CompareS_int64((&((int64 *) ptr[7])[15]), 1, 8, elementsToCheck);
        ok &= dS.CompareS_float32((&((float32 *) ptr[8])[15]), 1, 0.1, elementsToCheck);
        ok &= dS.CompareS_float64((&((float64 *) ptr[9])[15]), 1, 0.1, elementsToCheck);

    }
    if (ok) {
        elementsToCheck = 10;
        ok = dS.Synchronise();
        ok &= dS.CompareS_uint8((&((uint8 *) ptr[0])[0]), 2, 1, elementsToCheck);
        ok &= dS.CompareS_int8((&((int8 *) ptr[1])[0]), 2, 1, elementsToCheck);
        ok &= dS.CompareS_uint16((&((uint16 *) ptr[2])[0]), 2, 2, elementsToCheck);
        ok &= dS.CompareS_int16((&((int16 *) ptr[3])[0]), 2, 2, elementsToCheck);
        ok &= dS.CompareS_uint32((&((uint32 *) ptr[4])[0]), 2, 4, elementsToCheck);
        ok &= dS.CompareS_int32((&((int32 *) ptr[5])[0]), 2, 4, elementsToCheck);
        ok &= dS.CompareS_uint64((&((uint64 *) ptr[6])[0]), 2, 8, elementsToCheck);
        ok &= dS.CompareS_int64((&((int64 *) ptr[7])[0]), 2, 8, elementsToCheck);
        ok &= dS.CompareS_float32((&((float32 *) ptr[8])[0]), 2, 0.1, elementsToCheck);
        ok &= dS.CompareS_float64((&((float64 *) ptr[9])[0]), 2, 0.1, elementsToCheck);

        for (uint32 i = 10; (i < 25) && ok; i++) {
            ok &= (((uint8 *) ptr[0])[i] == 39);
            ok &= (((int8 *) ptr[1])[i] == 19);
            ok &= (((uint16 *) ptr[2])[i] == 78);
            ok &= (((int16 *) ptr[3])[i] == 38);
            ok &= (((uint32 *) ptr[4])[i] == 156);
            ok &= (((int32 *) ptr[5])[i] == 76);
            ok &= (((uint64 *) ptr[6])[i] == 312);
            ok &= (((int64 *) ptr[7])[i] == 152);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[i], static_cast<float32>(3.9));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[i], 3.9);
        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise28() {
    MDSReaderTestHelper dS(treeName);
    dS.elementsRead = 5;
    uint32 elementsToCheck = 0;
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 2, 1, 2);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    if (ok) {
        for (uint32 m = 0u; m < 4; m++) {

            for (uint32 i = 0u; i < 4; i++) {
                ok = dS.Synchronise();
                elementsToCheck = 5;
                if (ok) {
                    ok &= dS.CompareS_uint8(((uint8 *) ptr[0]), (m * 4) + i, 1, elementsToCheck);
                    ok &= dS.CompareS_int8(((int8 *) ptr[1]), (m * 4) + i, 1, elementsToCheck);
                    ok &= dS.CompareS_uint16(((uint16 *) ptr[2]), (m * 4) + i, 2, elementsToCheck);
                    ok &= dS.CompareS_int16(((int16 *) ptr[3]), (m * 4) + i, 2, elementsToCheck);
                    ok &= dS.CompareS_uint32(((uint32 *) ptr[4]), (m * 4) + i, 4, elementsToCheck);
                    ok &= dS.CompareS_int32(((int32 *) ptr[5]), (m * 4) + i, 4, elementsToCheck);
                    ok &= dS.CompareS_uint64(((uint64 *) ptr[6]), (m * 4) + i, 8, elementsToCheck);
                    ok &= dS.CompareS_int64(((int64 *) ptr[7]), (m * 4) + i, 8, elementsToCheck);
                    ok &= dS.CompareS_float32(((float32 *) ptr[8]), (m * 4) + i, 0.1, elementsToCheck);
                    ok &= dS.CompareS_float64(((float64 *) ptr[9]), (m * 4) + i, 0.1, elementsToCheck);
                }
            }
            for (uint32 i = 0u; i < 4; i++) {
                ok = dS.Synchronise();
                for (uint32 j = 0; (j < dS.elementsRead) && ok; j++) {
                    ok &= (((uint8 *) ptr[0])[j] == 20 * (m + 1) - 1);
                    int8 aux = int8(20 * (m + 1) - 1) - 20;
                    ok &= (((int8 *) ptr[1])[j] == aux);
                    ok &= (((uint16 *) ptr[2])[j] == 40 * (m + 1) - 2);
                    int16 aux16 = int16(40 * (m + 1) - 2) - 40;
                    ok &= (((int16 *) ptr[3])[j] == aux16);
                    ok &= (((uint32 *) ptr[4])[j] == 80 * (m + 1) - 4);
                    int32 aux32 = int32(80 * (m + 1) - 4) - 80;
                    ok &= (((int32 *) ptr[5])[j] == aux32);
                    ok &= (((uint64 *) ptr[6])[j] == 160 * (m + 1) - 8);
                    int64 aux64 = int64(160 * (m + 1) - 8) - 160;
                    ok &= (((int64 *) ptr[7])[j] == aux64);
                    ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[i], static_cast<float32>(2 * (m + 1) - 0.1));
                    ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[i], 2 * (m + 1) - 0.1);
                }
            }

        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise29() {
    MDSReaderTestHelper dS(treeName);
    dS.elementsRead = 15;
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 0, 0, 3);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    uint32 iteration = 0;
    uint32 samplesToCheck = 0u;
    uint32 from = 0;
    if (ok) {
        ok = dS.Synchronise();
        for (uint32 j = 6u; (j < 15) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 0);
            ok &= (((int8 *) ptr[1])[j] == 0);
            ok &= (((uint16 *) ptr[2])[j] == 0);
            ok &= (((int16 *) ptr[3])[j] == 0);
            ok &= (((uint32 *) ptr[4])[j] == 0);
            ok &= (((int32 *) ptr[5])[j] == 0);
            ok &= (((uint64 *) ptr[6])[j] == 0);
            ok &= (((int64 *) ptr[7])[j] == 0);
            ok &= (((float32 *) ptr[8])[j] == 0);
            ok &= (((float64 *) ptr[9])[j] == 0);
        }
    }
    if (ok) {
        ok = dS.Synchronise();
        iteration++;
        for (uint32 j = 0u; (j < 5) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 0);
            ok &= (((int8 *) ptr[1])[j] == 0);
            ok &= (((uint16 *) ptr[2])[j] == 0);
            ok &= (((int16 *) ptr[3])[j] == 0);
            ok &= (((uint32 *) ptr[4])[j] == 0);
            ok &= (((int32 *) ptr[5])[j] == 0);
            ok &= (((uint64 *) ptr[6])[j] == 0);
            ok &= (((int64 *) ptr[7])[j] == 0);
            ok &= (((float32 *) ptr[8])[j] == 0);
            ok &= (((float64 *) ptr[9])[j] == 0);
        }
        if (ok) {
            samplesToCheck = 10;
            from = 5;
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }
    }
    if (ok) {
        ok = dS.Synchronise();
        iteration++;
        if (ok) {
            samplesToCheck = 10;
            from = 0;
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }
        for (uint32 j = samplesToCheck; (j < 15) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 0);
            ok &= (((int8 *) ptr[1])[j] == 0);
            ok &= (((uint16 *) ptr[2])[j] == 0);
            ok &= (((int16 *) ptr[3])[j] == 0);
            ok &= (((uint32 *) ptr[4])[j] == 0);
            ok &= (((int32 *) ptr[5])[j] == 0);
            ok &= (((uint64 *) ptr[6])[j] == 0);
            ok &= (((int64 *) ptr[7])[j] == 0);
            ok &= (((float32 *) ptr[8])[j] == 0);
            ok &= (((float64 *) ptr[9])[j] == 0);
        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise30() {
    MDSReaderTestHelper dS(treeName);
    dS.elementsRead = 25;
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 0, 0, 3);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    uint32 samplesToCheck = 0u;
    uint32 from = 0;
    uint32 iteration = 0;
    if (ok) {
        ok = dS.Synchronise();
        from = 20;
        samplesToCheck = 5;
        for (uint32 j = 6u; (j < from) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 0);
            ok &= (((int8 *) ptr[1])[j] == 0);
            ok &= (((uint16 *) ptr[2])[j] == 0);
            ok &= (((int16 *) ptr[3])[j] == 0);
            ok &= (((uint32 *) ptr[4])[j] == 0);
            ok &= (((int32 *) ptr[5])[j] == 0);
            ok &= (((uint64 *) ptr[6])[j] == 0);
            ok &= (((int64 *) ptr[7])[j] == 0);
            ok &= (((float32 *) ptr[8])[j] == 0);
            ok &= (((float64 *) ptr[9])[j] == 0);
        }

        ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
        ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
        ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
        ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
        ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
        ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
        ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], 1, 8, samplesToCheck);
        ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
        ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
        ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);

    }
    if (ok) {
        ok = dS.Synchronise();
        iteration++;
        from = 0;
        samplesToCheck = 15;
        ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
        ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
        ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
        ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
        ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
        ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
        ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
        ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
        ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
        ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        for (uint32 j = 15u; (j < 25) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 0);
            ok &= (((int8 *) ptr[1])[j] == 0);
            ok &= (((uint16 *) ptr[2])[j] == 0);
            ok &= (((int16 *) ptr[3])[j] == 0);
            ok &= (((uint32 *) ptr[4])[j] == 0);
            ok &= (((int32 *) ptr[5])[j] == 0);
            ok &= (((uint64 *) ptr[6])[j] == 0);
            ok &= (((int64 *) ptr[7])[j] == 0);
            ok &= (((float32 *) ptr[8])[j] == 0);
            ok &= (((float64 *) ptr[9])[j] == 0);
        }
    }
    if (ok) {
        ok = dS.Synchronise();
        iteration++;
        from = 10;
        samplesToCheck = 15;
        for (uint32 j = 0u; (j < from) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 0);
            ok &= (((int8 *) ptr[1])[j] == 0);
            ok &= (((uint16 *) ptr[2])[j] == 0);
            ok &= (((int16 *) ptr[3])[j] == 0);
            ok &= (((uint32 *) ptr[4])[j] == 0);
            ok &= (((int32 *) ptr[5])[j] == 0);
            ok &= (((uint64 *) ptr[6])[j] == 0);
            ok &= (((int64 *) ptr[7])[j] == 0);
            ok &= (((float32 *) ptr[8])[j] == 0);
            ok &= (((float64 *) ptr[9])[j] == 0);
        }
        if (ok) {
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise31() {
    MDSReaderTestHelper dS(treeName);
    dS.elementsRead = 15;
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 0, 1, 3);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    uint32 iteration = 0;
    uint32 samplesToCheck = 0u;
    uint32 from = 0;
    if (ok) {
        ok = dS.Synchronise();
        for (uint32 j = 6u; (j < 15) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 0);
            ok &= (((int8 *) ptr[1])[j] == 0);
            ok &= (((uint16 *) ptr[2])[j] == 0);
            ok &= (((int16 *) ptr[3])[j] == 0);
            ok &= (((uint32 *) ptr[4])[j] == 0);
            ok &= (((int32 *) ptr[5])[j] == 0);
            ok &= (((uint64 *) ptr[6])[j] == 0);
            ok &= (((int64 *) ptr[7])[j] == 0);
            ok &= (((float32 *) ptr[8])[j] == 0);
            ok &= (((float64 *) ptr[9])[j] == 0);
        }
    }
    if (ok) {
        ok = dS.Synchronise();
        iteration++;
        for (uint32 j = 0u; (j < 5) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 0);
            ok &= (((int8 *) ptr[1])[j] == 0);
            ok &= (((uint16 *) ptr[2])[j] == 0);
            ok &= (((int16 *) ptr[3])[j] == 0);
            ok &= (((uint32 *) ptr[4])[j] == 0);
            ok &= (((int32 *) ptr[5])[j] == 0);
            ok &= (((uint64 *) ptr[6])[j] == 0);
            ok &= (((int64 *) ptr[7])[j] == 0);
            ok &= (((float32 *) ptr[8])[j] == 0);
            ok &= (((float64 *) ptr[9])[j] == 0);
        }
        if (ok) {
            samplesToCheck = 10;
            from = 5;
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }
    }
    if (ok) {
        ok = dS.Synchronise();
        iteration++;
        if (ok) {
            samplesToCheck = 10;
            from = 0;
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }

        for (uint32 i = samplesToCheck; (i < 15) && ok; i++) {
            ok &= (((uint8 *) ptr[0])[i] == 19);
            ok &= (((int8 *) ptr[1])[i] == -1);
            ok &= (((uint16 *) ptr[2])[i] == 38);
            ok &= (((int16 *) ptr[3])[i] == -2);
            ok &= (((uint32 *) ptr[4])[i] == 76);
            ok &= (((int32 *) ptr[5])[i] == -4);
            ok &= (((uint64 *) ptr[6])[i] == 152);
            ok &= (((int64 *) ptr[7])[i] == -8);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[i], static_cast<float32>(1.9));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[i], 1.9);
        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise32() {
    MDSReaderTestHelper dS(treeName);
    dS.elementsRead = 25;
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 0, 1, 3);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    uint32 samplesToCheck = 0u;
    uint32 from = 0;
    uint32 iteration = 0;
    if (ok) {
        ok = dS.Synchronise();
        from = 20;
        samplesToCheck = 5;
        for (uint32 j = 6u; (j < from) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 0);
            ok &= (((int8 *) ptr[1])[j] == 0);
            ok &= (((uint16 *) ptr[2])[j] == 0);
            ok &= (((int16 *) ptr[3])[j] == 0);
            ok &= (((uint32 *) ptr[4])[j] == 0);
            ok &= (((int32 *) ptr[5])[j] == 0);
            ok &= (((uint64 *) ptr[6])[j] == 0);
            ok &= (((int64 *) ptr[7])[j] == 0);
            ok &= (((float32 *) ptr[8])[j] == 0);
            ok &= (((float64 *) ptr[9])[j] == 0);
        }

        ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
        ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
        ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
        ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
        ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
        ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
        ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], 1, 8, samplesToCheck);
        ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
        ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
        ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);

    }
    if (ok) {
        ok = dS.Synchronise();
        iteration++;
        from = 0;
        samplesToCheck = 15;
        ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
        ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
        ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
        ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
        ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
        ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
        ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
        ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
        ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
        ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);

        for (uint32 i = 15u; (i < 25) && ok; i++) {
            ok &= (((uint8 *) ptr[0])[i] == 19);
            ok &= (((int8 *) ptr[1])[i] == -1);
            ok &= (((uint16 *) ptr[2])[i] == 38);
            ok &= (((int16 *) ptr[3])[i] == -2);
            ok &= (((uint32 *) ptr[4])[i] == 76);
            ok &= (((int32 *) ptr[5])[i] == -4);
            ok &= (((uint64 *) ptr[6])[i] == 152);
            ok &= (((int64 *) ptr[7])[i] == -8);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[i], static_cast<float32>(1.9));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[i], 1.9);
        }
    }
    if (ok) {
        ok = dS.Synchronise();
        iteration++;
        from = 10;
        samplesToCheck = 15;

        for (uint32 i = 0u; (i < from) && ok; i++) {
            ok &= (((uint8 *) ptr[0])[i] == 19);
            ok &= (((int8 *) ptr[1])[i] == -1);
            ok &= (((uint16 *) ptr[2])[i] == 38);
            ok &= (((int16 *) ptr[3])[i] == -2);
            ok &= (((uint32 *) ptr[4])[i] == 76);
            ok &= (((int32 *) ptr[5])[i] == -4);
            ok &= (((uint64 *) ptr[6])[i] == 152);
            ok &= (((int64 *) ptr[7])[i] == -8);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[i], static_cast<float32>(1.9));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[i], 1.9);
        }
        if (ok) {
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise33() {
    MDSReaderTestHelper dS(treeName);
    dS.elementsRead = 15;
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 1, 0, 3);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    uint32 iteration = 0;
    uint32 samplesToCheck = 0u;
    uint32 from = 0;
    if (ok) {
        ok = dS.Synchronise();
        for (uint32 j = 6u; (j < 15) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 0);
            ok &= (((int8 *) ptr[1])[j] == 0);
            ok &= (((uint16 *) ptr[2])[j] == 0);
            ok &= (((int16 *) ptr[3])[j] == 0);
            ok &= (((uint32 *) ptr[4])[j] == 0);
            ok &= (((int32 *) ptr[5])[j] == 0);
            ok &= (((uint64 *) ptr[6])[j] == 0);
            ok &= (((int64 *) ptr[7])[j] == 0);
            ok &= (((float32 *) ptr[8])[j] == 0);
            ok &= (((float64 *) ptr[9])[j] == 0);
        }
    }
    if (ok) {
        ok = dS.Synchronise();
        iteration++;
        for (uint32 j = 0u; (j < 5) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 0);
            ok &= (((int8 *) ptr[1])[j] == 0);
            ok &= (((uint16 *) ptr[2])[j] == 0);
            ok &= (((int16 *) ptr[3])[j] == 0);
            ok &= (((uint32 *) ptr[4])[j] == 0);
            ok &= (((int32 *) ptr[5])[j] == 0);
            ok &= (((uint64 *) ptr[6])[j] == 0);
            ok &= (((int64 *) ptr[7])[j] == 0);
            ok &= (((float32 *) ptr[8])[j] == 0);
            ok &= (((float64 *) ptr[9])[j] == 0);
        }
        if (ok) {
            samplesToCheck = 10;
            from = 5;
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }
    }
    if (ok) {
        ok = dS.Synchronise();
        iteration++;
        if (ok) {
            samplesToCheck = 10;
            from = 0;
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }
        for (uint32 j = samplesToCheck; (j < 15) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 0);
            ok &= (((int8 *) ptr[1])[j] == 0);
            ok &= (((uint16 *) ptr[2])[j] == 0);
            ok &= (((int16 *) ptr[3])[j] == 0);
            ok &= (((uint32 *) ptr[4])[j] == 0);
            ok &= (((int32 *) ptr[5])[j] == 0);
            ok &= (((uint64 *) ptr[6])[j] == 0);
            ok &= (((int64 *) ptr[7])[j] == 0);
            ok &= (((float32 *) ptr[8])[j] == 0);
            ok &= (((float64 *) ptr[9])[j] == 0);
        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise34() {
    MDSReaderTestHelper dS(treeName);
    dS.elementsRead = 25;
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 1, 0, 3);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    uint32 samplesToCheck = 0u;
    uint32 from = 0;
    uint32 iteration = 0;
    if (ok) {
        ok = dS.Synchronise();
        from = 20;
        samplesToCheck = 5;
        for (uint32 j = 6u; (j < from) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 0);
            ok &= (((int8 *) ptr[1])[j] == 0);
            ok &= (((uint16 *) ptr[2])[j] == 0);
            ok &= (((int16 *) ptr[3])[j] == 0);
            ok &= (((uint32 *) ptr[4])[j] == 0);
            ok &= (((int32 *) ptr[5])[j] == 0);
            ok &= (((uint64 *) ptr[6])[j] == 0);
            ok &= (((int64 *) ptr[7])[j] == 0);
            ok &= (((float32 *) ptr[8])[j] == 0);
            ok &= (((float64 *) ptr[9])[j] == 0);
        }

        ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
        ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
        ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
        ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
        ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
        ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
        ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], 1, 8, samplesToCheck);
        ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
        ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
        ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);

    }
    if (ok) {
        ok = dS.Synchronise();
        iteration++;
        from = 0;
        samplesToCheck = 15;
        ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
        ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
        ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
        ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
        ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
        ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
        ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
        ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
        ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
        ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        for (uint32 j = 15u; (j < 25) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 0);
            ok &= (((int8 *) ptr[1])[j] == 0);
            ok &= (((uint16 *) ptr[2])[j] == 0);
            ok &= (((int16 *) ptr[3])[j] == 0);
            ok &= (((uint32 *) ptr[4])[j] == 0);
            ok &= (((int32 *) ptr[5])[j] == 0);
            ok &= (((uint64 *) ptr[6])[j] == 0);
            ok &= (((int64 *) ptr[7])[j] == 0);
            ok &= (((float32 *) ptr[8])[j] == 0);
            ok &= (((float64 *) ptr[9])[j] == 0);
        }
    }
    if (ok) {
        ok = dS.Synchronise();
        iteration++;
        from = 10;
        samplesToCheck = 15;
        for (uint32 j = 0u; (j < from) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 0);
            ok &= (((int8 *) ptr[1])[j] == 0);
            ok &= (((uint16 *) ptr[2])[j] == 0);
            ok &= (((int16 *) ptr[3])[j] == 0);
            ok &= (((uint32 *) ptr[4])[j] == 0);
            ok &= (((int32 *) ptr[5])[j] == 0);
            ok &= (((uint64 *) ptr[6])[j] == 0);
            ok &= (((int64 *) ptr[7])[j] == 0);
            ok &= (((float32 *) ptr[8])[j] == 0);
            ok &= (((float64 *) ptr[9])[j] == 0);
        }
        if (ok) {
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise35() {
    MDSReaderTestHelper dS(treeName);
    dS.elementsRead = 15;
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 1, 1, 3);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    uint32 iteration = 0;
    uint32 samplesToCheck = 0u;
    uint32 from = 0;
    if (ok) {
        ok = dS.Synchronise();
        for (uint32 j = 6u; (j < 15) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 0);
            ok &= (((int8 *) ptr[1])[j] == 0);
            ok &= (((uint16 *) ptr[2])[j] == 0);
            ok &= (((int16 *) ptr[3])[j] == 0);
            ok &= (((uint32 *) ptr[4])[j] == 0);
            ok &= (((int32 *) ptr[5])[j] == 0);
            ok &= (((uint64 *) ptr[6])[j] == 0);
            ok &= (((int64 *) ptr[7])[j] == 0);
            ok &= (((float32 *) ptr[8])[j] == 0);
            ok &= (((float64 *) ptr[9])[j] == 0);
        }
    }
    if (ok) {
        ok = dS.Synchronise();
        iteration++;
        for (uint32 j = 0u; (j < 5) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 0);
            ok &= (((int8 *) ptr[1])[j] == 0);
            ok &= (((uint16 *) ptr[2])[j] == 0);
            ok &= (((int16 *) ptr[3])[j] == 0);
            ok &= (((uint32 *) ptr[4])[j] == 0);
            ok &= (((int32 *) ptr[5])[j] == 0);
            ok &= (((uint64 *) ptr[6])[j] == 0);
            ok &= (((int64 *) ptr[7])[j] == 0);
            ok &= (((float32 *) ptr[8])[j] == 0);
            ok &= (((float64 *) ptr[9])[j] == 0);
        }
        if (ok) {
            samplesToCheck = 10;
            from = 5;
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }
    }
    if (ok) {
        ok = dS.Synchronise();
        iteration++;
        if (ok) {
            samplesToCheck = 10;
            from = 0;
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }

        for (uint32 i = samplesToCheck; (i < 15) && ok; i++) {
            ok &= (((uint8 *) ptr[0])[i] == 19);
            ok &= (((int8 *) ptr[1])[i] == -1);
            ok &= (((uint16 *) ptr[2])[i] == 38);
            ok &= (((int16 *) ptr[3])[i] == -2);
            ok &= (((uint32 *) ptr[4])[i] == 76);
            ok &= (((int32 *) ptr[5])[i] == -4);
            ok &= (((uint64 *) ptr[6])[i] == 152);
            ok &= (((int64 *) ptr[7])[i] == -8);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[i], static_cast<float32>(1.9));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[i], 1.9);
        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise36() {
    MDSReaderTestHelper dS(treeName);
    dS.elementsRead = 25;
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 1, 1, 3);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    uint32 samplesToCheck = 0u;
    uint32 from = 0;
    uint32 iteration = 0;
    if (ok) {
        ok = dS.Synchronise();
        from = 20;
        samplesToCheck = 5;
        for (uint32 j = 6u; (j < from) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 0);
            ok &= (((int8 *) ptr[1])[j] == 0);
            ok &= (((uint16 *) ptr[2])[j] == 0);
            ok &= (((int16 *) ptr[3])[j] == 0);
            ok &= (((uint32 *) ptr[4])[j] == 0);
            ok &= (((int32 *) ptr[5])[j] == 0);
            ok &= (((uint64 *) ptr[6])[j] == 0);
            ok &= (((int64 *) ptr[7])[j] == 0);
            ok &= (((float32 *) ptr[8])[j] == 0);
            ok &= (((float64 *) ptr[9])[j] == 0);
        }

        ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
        ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
        ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
        ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
        ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
        ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
        ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], 1, 8, samplesToCheck);
        ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
        ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
        ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);

    }
    if (ok) {
        ok = dS.Synchronise();
        iteration++;
        from = 0;
        samplesToCheck = 15;
        ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
        ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
        ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
        ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
        ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
        ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
        ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
        ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
        ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
        ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);

        for (uint32 i = 15u; (i < 25) && ok; i++) {
            ok &= (((uint8 *) ptr[0])[i] == 19);
            ok &= (((int8 *) ptr[1])[i] == -1);
            ok &= (((uint16 *) ptr[2])[i] == 38);
            ok &= (((int16 *) ptr[3])[i] == -2);
            ok &= (((uint32 *) ptr[4])[i] == 76);
            ok &= (((int32 *) ptr[5])[i] == -4);
            ok &= (((uint64 *) ptr[6])[i] == 152);
            ok &= (((int64 *) ptr[7])[i] == -8);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[i], static_cast<float32>(1.9));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[i], 1.9);
        }
    }
    if (ok) {
        ok = dS.Synchronise();
        iteration++;
        from = 10;
        samplesToCheck = 15;

        for (uint32 i = 0u; (i < from) && ok; i++) {
            ok &= (((uint8 *) ptr[0])[i] == 19);
            ok &= (((int8 *) ptr[1])[i] == -1);
            ok &= (((uint16 *) ptr[2])[i] == 38);
            ok &= (((int16 *) ptr[3])[i] == -2);
            ok &= (((uint32 *) ptr[4])[i] == 76);
            ok &= (((int32 *) ptr[5])[i] == -4);
            ok &= (((uint64 *) ptr[6])[i] == 152);
            ok &= (((int64 *) ptr[7])[i] == -8);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[i], static_cast<float32>(1.9));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[i], 1.9);
        }
        if (ok) {
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise37() {
    MDSReaderTestHelper dS(treeName);
    dS.elementsRead = 15;
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 2, 0, 3);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    uint32 iteration = 0;
    uint32 samplesToCheck = 0u;
    uint32 from = 0;
    if (ok) {
        ok = dS.Synchronise();
        for (uint32 j = 6u; (j < 15) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 0);
            ok &= (((int8 *) ptr[1])[j] == 0);
            ok &= (((uint16 *) ptr[2])[j] == 0);
            ok &= (((int16 *) ptr[3])[j] == 0);
            ok &= (((uint32 *) ptr[4])[j] == 0);
            ok &= (((int32 *) ptr[5])[j] == 0);
            ok &= (((uint64 *) ptr[6])[j] == 0);
            ok &= (((int64 *) ptr[7])[j] == 0);
            ok &= (((float32 *) ptr[8])[j] == 0);
            ok &= (((float64 *) ptr[9])[j] == 0);
        }
    }
    if (ok) {
        ok = dS.Synchronise();
        iteration++;
        for (uint32 j = 0u; (j < 5) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 0);
            ok &= (((int8 *) ptr[1])[j] == 0);
            ok &= (((uint16 *) ptr[2])[j] == 0);
            ok &= (((int16 *) ptr[3])[j] == 0);
            ok &= (((uint32 *) ptr[4])[j] == 0);
            ok &= (((int32 *) ptr[5])[j] == 0);
            ok &= (((uint64 *) ptr[6])[j] == 0);
            ok &= (((int64 *) ptr[7])[j] == 0);
            ok &= (((float32 *) ptr[8])[j] == 0);
            ok &= (((float64 *) ptr[9])[j] == 0);
        }
        if (ok) {
            samplesToCheck = 10;
            from = 5;
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }
    }
    if (ok) {
        ok = dS.Synchronise();
        iteration++;
        if (ok) {
            samplesToCheck = 10;
            from = 0;
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }
        for (uint32 j = samplesToCheck; (j < 15) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 0);
            ok &= (((int8 *) ptr[1])[j] == 0);
            ok &= (((uint16 *) ptr[2])[j] == 0);
            ok &= (((int16 *) ptr[3])[j] == 0);
            ok &= (((uint32 *) ptr[4])[j] == 0);
            ok &= (((int32 *) ptr[5])[j] == 0);
            ok &= (((uint64 *) ptr[6])[j] == 0);
            ok &= (((int64 *) ptr[7])[j] == 0);
            ok &= (((float32 *) ptr[8])[j] == 0);
            ok &= (((float64 *) ptr[9])[j] == 0);
        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise38() {
    MDSReaderTestHelper dS(treeName);
    dS.elementsRead = 25;
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 2, 0, 3);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    uint32 samplesToCheck = 0u;
    uint32 from = 0;
    uint32 iteration = 0;
    if (ok) {
        ok = dS.Synchronise();
        from = 20;
        samplesToCheck = 5;
        for (uint32 j = 6u; (j < from) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 0);
            ok &= (((int8 *) ptr[1])[j] == 0);
            ok &= (((uint16 *) ptr[2])[j] == 0);
            ok &= (((int16 *) ptr[3])[j] == 0);
            ok &= (((uint32 *) ptr[4])[j] == 0);
            ok &= (((int32 *) ptr[5])[j] == 0);
            ok &= (((uint64 *) ptr[6])[j] == 0);
            ok &= (((int64 *) ptr[7])[j] == 0);
            ok &= (((float32 *) ptr[8])[j] == 0);
            ok &= (((float64 *) ptr[9])[j] == 0);
        }

        ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
        ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
        ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
        ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
        ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
        ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
        ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], 1, 8, samplesToCheck);
        ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
        ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
        ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);

    }
    if (ok) {
        ok = dS.Synchronise();
        iteration++;
        from = 0;
        samplesToCheck = 15;
        ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
        ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
        ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
        ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
        ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
        ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
        ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
        ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
        ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
        ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        for (uint32 j = 15u; (j < 25) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 0);
            ok &= (((int8 *) ptr[1])[j] == 0);
            ok &= (((uint16 *) ptr[2])[j] == 0);
            ok &= (((int16 *) ptr[3])[j] == 0);
            ok &= (((uint32 *) ptr[4])[j] == 0);
            ok &= (((int32 *) ptr[5])[j] == 0);
            ok &= (((uint64 *) ptr[6])[j] == 0);
            ok &= (((int64 *) ptr[7])[j] == 0);
            ok &= (((float32 *) ptr[8])[j] == 0);
            ok &= (((float64 *) ptr[9])[j] == 0);
        }
    }
    if (ok) {
        ok = dS.Synchronise();
        iteration++;
        from = 10;
        samplesToCheck = 15;
        for (uint32 j = 0u; (j < from) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 0);
            ok &= (((int8 *) ptr[1])[j] == 0);
            ok &= (((uint16 *) ptr[2])[j] == 0);
            ok &= (((int16 *) ptr[3])[j] == 0);
            ok &= (((uint32 *) ptr[4])[j] == 0);
            ok &= (((int32 *) ptr[5])[j] == 0);
            ok &= (((uint64 *) ptr[6])[j] == 0);
            ok &= (((int64 *) ptr[7])[j] == 0);
            ok &= (((float32 *) ptr[8])[j] == 0);
            ok &= (((float64 *) ptr[9])[j] == 0);
        }
        if (ok) {
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise39() {
    MDSReaderTestHelper dS(treeName);
    dS.elementsRead = 15;
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 2, 1, 3);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    uint32 iteration = 0;
    uint32 samplesToCheck = 0u;
    uint32 from = 0;
    if (ok) {
        ok = dS.Synchronise();
        for (uint32 j = 6u; (j < 15) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 0);
            ok &= (((int8 *) ptr[1])[j] == 0);
            ok &= (((uint16 *) ptr[2])[j] == 0);
            ok &= (((int16 *) ptr[3])[j] == 0);
            ok &= (((uint32 *) ptr[4])[j] == 0);
            ok &= (((int32 *) ptr[5])[j] == 0);
            ok &= (((uint64 *) ptr[6])[j] == 0);
            ok &= (((int64 *) ptr[7])[j] == 0);
            ok &= (((float32 *) ptr[8])[j] == 0);
            ok &= (((float64 *) ptr[9])[j] == 0);
        }
    }
    if (ok) {
        ok = dS.Synchronise();
        iteration++;
        for (uint32 j = 0u; (j < 5) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 0);
            ok &= (((int8 *) ptr[1])[j] == 0);
            ok &= (((uint16 *) ptr[2])[j] == 0);
            ok &= (((int16 *) ptr[3])[j] == 0);
            ok &= (((uint32 *) ptr[4])[j] == 0);
            ok &= (((int32 *) ptr[5])[j] == 0);
            ok &= (((uint64 *) ptr[6])[j] == 0);
            ok &= (((int64 *) ptr[7])[j] == 0);
            ok &= (((float32 *) ptr[8])[j] == 0);
            ok &= (((float64 *) ptr[9])[j] == 0);
        }
        if (ok) {
            samplesToCheck = 10;
            from = 5;
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }
    }
    if (ok) {
        ok = dS.Synchronise();
        iteration++;
        if (ok) {
            samplesToCheck = 10;
            from = 0;
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }

        for (uint32 i = samplesToCheck; (i < 15) && ok; i++) {
            ok &= (((uint8 *) ptr[0])[i] == 19);
            ok &= (((int8 *) ptr[1])[i] == -1);
            ok &= (((uint16 *) ptr[2])[i] == 38);
            ok &= (((int16 *) ptr[3])[i] == -2);
            ok &= (((uint32 *) ptr[4])[i] == 76);
            ok &= (((int32 *) ptr[5])[i] == -4);
            ok &= (((uint64 *) ptr[6])[i] == 152);
            ok &= (((int64 *) ptr[7])[i] == -8);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[i], static_cast<float32>(1.9));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[i], 1.9);
        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise40() {
    MDSReaderTestHelper dS(treeName);
    dS.elementsRead = 25;
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 2, 1, 3);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    uint32 samplesToCheck = 0u;
    uint32 from = 0;
    uint32 iteration = 0;
    if (ok) {
        ok = dS.Synchronise();
        from = 20;
        samplesToCheck = 5;
        for (uint32 j = 6u; (j < from) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 0);
            ok &= (((int8 *) ptr[1])[j] == 0);
            ok &= (((uint16 *) ptr[2])[j] == 0);
            ok &= (((int16 *) ptr[3])[j] == 0);
            ok &= (((uint32 *) ptr[4])[j] == 0);
            ok &= (((int32 *) ptr[5])[j] == 0);
            ok &= (((uint64 *) ptr[6])[j] == 0);
            ok &= (((int64 *) ptr[7])[j] == 0);
            ok &= (((float32 *) ptr[8])[j] == 0);
            ok &= (((float64 *) ptr[9])[j] == 0);
        }

        ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
        ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
        ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
        ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
        ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
        ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
        ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], 1, 8, samplesToCheck);
        ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
        ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
        ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);

    }
    if (ok) {
        ok = dS.Synchronise();
        iteration++;
        from = 0;
        samplesToCheck = 15;
        ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
        ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
        ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
        ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
        ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
        ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
        ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
        ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
        ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
        ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);

        for (uint32 i = 15u; (i < 25) && ok; i++) {
            ok &= (((uint8 *) ptr[0])[i] == 19);
            ok &= (((int8 *) ptr[1])[i] == -1);
            ok &= (((uint16 *) ptr[2])[i] == 38);
            ok &= (((int16 *) ptr[3])[i] == -2);
            ok &= (((uint32 *) ptr[4])[i] == 76);
            ok &= (((int32 *) ptr[5])[i] == -4);
            ok &= (((uint64 *) ptr[6])[i] == 152);
            ok &= (((int64 *) ptr[7])[i] == -8);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[i], static_cast<float32>(1.9));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[i], 1.9);
        }
    }
    if (ok) {
        ok = dS.Synchronise();
        iteration++;
        from = 10;
        samplesToCheck = 15;

        for (uint32 i = 0u; (i < from) && ok; i++) {
            ok &= (((uint8 *) ptr[0])[i] == 19);
            ok &= (((int8 *) ptr[1])[i] == -1);
            ok &= (((uint16 *) ptr[2])[i] == 38);
            ok &= (((int16 *) ptr[3])[i] == -2);
            ok &= (((uint32 *) ptr[4])[i] == 76);
            ok &= (((int32 *) ptr[5])[i] == -4);
            ok &= (((uint64 *) ptr[6])[i] == 152);
            ok &= (((int64 *) ptr[7])[i] == -8);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[i], static_cast<float32>(1.9));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[i], 1.9);
        }
        if (ok) {
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise41() {
    MDSReaderTestHelper dS(treeName);
    dS.elementsRead = 15;
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 0, 0, 4);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    uint32 iteration = 0;
    uint32 samplesToCheck = 0u;
    uint32 from = 0;
    if (ok) {
        ok = dS.Synchronise();
        samplesToCheck = 15;
        from = 0;
        if (ok) {
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }
    }
    if (ok) {
        ok = dS.Synchronise();
        samplesToCheck = 15;
        from = 0;
        if (ok) {
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }
    }
    if (ok) {
        ok = dS.Synchronise();
        samplesToCheck = 10;
        from = 0;
        if (ok) {
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }
        for (uint32 j = samplesToCheck; (j < 15) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 0);
            ok &= (((int8 *) ptr[1])[j] == 0);
            ok &= (((uint16 *) ptr[2])[j] == 0);
            ok &= (((int16 *) ptr[3])[j] == 0);
            ok &= (((uint32 *) ptr[4])[j] == 0);
            ok &= (((int32 *) ptr[5])[j] == 0);
            ok &= (((uint64 *) ptr[6])[j] == 0);
            ok &= (((int64 *) ptr[7])[j] == 0);
            ok &= (((float32 *) ptr[8])[j] == 0);
            ok &= (((float64 *) ptr[9])[j] == 0);
        }
    }
    if (ok) {
        ok = dS.Synchronise();
        samplesToCheck = 0;
        from = 0;
        for (uint32 j = samplesToCheck; (j < 15) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 0);
            ok &= (((int8 *) ptr[1])[j] == 0);
            ok &= (((uint16 *) ptr[2])[j] == 0);
            ok &= (((int16 *) ptr[3])[j] == 0);
            ok &= (((uint32 *) ptr[4])[j] == 0);
            ok &= (((int32 *) ptr[5])[j] == 0);
            ok &= (((uint64 *) ptr[6])[j] == 0);
            ok &= (((int64 *) ptr[7])[j] == 0);
            ok &= (((float32 *) ptr[8])[j] == 0);
            ok &= (((float64 *) ptr[9])[j] == 0);
        }
    }
    if (ok) {
        ok = dS.Synchronise();
        samplesToCheck = 0;
        from = 0;
        for (uint32 j = samplesToCheck; (j < 15) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 0);
            ok &= (((int8 *) ptr[1])[j] == 0);
            ok &= (((uint16 *) ptr[2])[j] == 0);
            ok &= (((int16 *) ptr[3])[j] == 0);
            ok &= (((uint32 *) ptr[4])[j] == 0);
            ok &= (((int32 *) ptr[5])[j] == 0);
            ok &= (((uint64 *) ptr[6])[j] == 0);
            ok &= (((int64 *) ptr[7])[j] == 0);
            ok &= (((float32 *) ptr[8])[j] == 0);
            ok &= (((float64 *) ptr[9])[j] == 0);
        }
    }
    if (ok) {
        ok = dS.Synchronise();
        samplesToCheck = 10;
        from = 5;
        for (uint32 j = 0; (j < from) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 0);
            ok &= (((int8 *) ptr[1])[j] == 0);
            ok &= (((uint16 *) ptr[2])[j] == 0);
            ok &= (((int16 *) ptr[3])[j] == 0);
            ok &= (((uint32 *) ptr[4])[j] == 0);
            ok &= (((int32 *) ptr[5])[j] == 0);
            ok &= (((uint64 *) ptr[6])[j] == 0);
            ok &= (((int64 *) ptr[7])[j] == 0);
            ok &= (((float32 *) ptr[8])[j] == 0);
            ok &= (((float64 *) ptr[9])[j] == 0);
        }
        if (ok) {
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise42() {
    MDSReaderTestHelper dS(treeName);
    dS.elementsRead = 100;
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 0, 0, 2);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    uint32 iteration = 0;
    uint32 samplesToCheck = 0u;
    uint32 from = 0;
    if (ok) {
        ok = dS.Synchronise();
        samplesToCheck = 20;
        from = 0;
        if (ok) {
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }
        for (uint32 j = samplesToCheck; (j < (samplesToCheck + 20)) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 0);
            ok &= (((int8 *) ptr[1])[j] == 0);
            ok &= (((uint16 *) ptr[2])[j] == 0);
            ok &= (((int16 *) ptr[3])[j] == 0);
            ok &= (((uint32 *) ptr[4])[j] == 0);
            ok &= (((int32 *) ptr[5])[j] == 0);
            ok &= (((uint64 *) ptr[6])[j] == 0);
            ok &= (((int64 *) ptr[7])[j] == 0);
            ok &= (((float32 *) ptr[8])[j] == 0);
            ok &= (((float64 *) ptr[9])[j] == 0);
        }
        samplesToCheck = 20;
        from = 40;
        iteration++;
        if (ok) {
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }
        for (uint32 j = 60; (j < 80) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 0);
            ok &= (((int8 *) ptr[1])[j] == 0);
            ok &= (((uint16 *) ptr[2])[j] == 0);
            ok &= (((int16 *) ptr[3])[j] == 0);
            ok &= (((uint32 *) ptr[4])[j] == 0);
            ok &= (((int32 *) ptr[5])[j] == 0);
            ok &= (((uint64 *) ptr[6])[j] == 0);
            ok &= (((int64 *) ptr[7])[j] == 0);
            ok &= (((float32 *) ptr[8])[j] == 0);
            ok &= (((float64 *) ptr[9])[j] == 0);
        }
        from = 80;
        iteration++;
        if (ok) {
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise43() {
    MDSReaderTestHelper dS(treeName);
    dS.elementsRead = 100;
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 0, 0, 2);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    uint32 iteration = 0;
    uint32 samplesToCheck = 0u;
    uint32 from = 0;
    if (ok) {
        ok = dS.Synchronise();
        samplesToCheck = 20;
        from = 0;
        if (ok) {
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }
        for (uint32 j = samplesToCheck; (j < (samplesToCheck + 20)) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 0);
            ok &= (((int8 *) ptr[1])[j] == 0);
            ok &= (((uint16 *) ptr[2])[j] == 0);
            ok &= (((int16 *) ptr[3])[j] == 0);
            ok &= (((uint32 *) ptr[4])[j] == 0);
            ok &= (((int32 *) ptr[5])[j] == 0);
            ok &= (((uint64 *) ptr[6])[j] == 0);
            ok &= (((int64 *) ptr[7])[j] == 0);
            ok &= (((float32 *) ptr[8])[j] == 0);
            ok &= (((float64 *) ptr[9])[j] == 0);
        }
        samplesToCheck = 20;
        from = 40;
        iteration++;
        if (ok) {
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }
        for (uint32 j = 60; (j < 80) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 0);
            ok &= (((int8 *) ptr[1])[j] == 0);
            ok &= (((uint16 *) ptr[2])[j] == 0);
            ok &= (((int16 *) ptr[3])[j] == 0);
            ok &= (((uint32 *) ptr[4])[j] == 0);
            ok &= (((int32 *) ptr[5])[j] == 0);
            ok &= (((uint64 *) ptr[6])[j] == 0);
            ok &= (((int64 *) ptr[7])[j] == 0);
            ok &= (((float32 *) ptr[8])[j] == 0);
            ok &= (((float64 *) ptr[9])[j] == 0);
        }
        from = 80;
        iteration++;
        if (ok) {
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise44() {
    MDSReaderTestHelper dS(treeName);
    dS.elementsRead = 100;
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 0, 0, 2);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    uint32 iteration = 0;
    uint32 samplesToCheck = 0u;
    uint32 from = 0;
    if (ok) {
        ok = dS.Synchronise();
        samplesToCheck = 20;
        from = 0;
        if (ok) {
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }
        for (uint32 j = samplesToCheck; (j < (samplesToCheck + 20)) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 0);
            ok &= (((int8 *) ptr[1])[j] == 0);
            ok &= (((uint16 *) ptr[2])[j] == 0);
            ok &= (((int16 *) ptr[3])[j] == 0);
            ok &= (((uint32 *) ptr[4])[j] == 0);
            ok &= (((int32 *) ptr[5])[j] == 0);
            ok &= (((uint64 *) ptr[6])[j] == 0);
            ok &= (((int64 *) ptr[7])[j] == 0);
            ok &= (((float32 *) ptr[8])[j] == 0);
            ok &= (((float64 *) ptr[9])[j] == 0);
        }
        samplesToCheck = 20;
        from = 40;
        iteration++;
        if (ok) {
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }
        for (uint32 j = 60; (j < 80) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 0);
            ok &= (((int8 *) ptr[1])[j] == 0);
            ok &= (((uint16 *) ptr[2])[j] == 0);
            ok &= (((int16 *) ptr[3])[j] == 0);
            ok &= (((uint32 *) ptr[4])[j] == 0);
            ok &= (((int32 *) ptr[5])[j] == 0);
            ok &= (((uint64 *) ptr[6])[j] == 0);
            ok &= (((int64 *) ptr[7])[j] == 0);
            ok &= (((float32 *) ptr[8])[j] == 0);
            ok &= (((float64 *) ptr[9])[j] == 0);
        }
        from = 80;
        iteration++;
        if (ok) {
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise45() {
    MDSReaderTestHelper dS(treeName);
    dS.elementsRead = 100;
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 0, 1, 2);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    uint32 iteration = 0;
    uint32 samplesToCheck = 0u;
    uint32 from = 0;
    if (ok) {
        ok = dS.Synchronise();
        samplesToCheck = 20;
        from = 0;
        if (ok) {
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }
        for (uint32 j = samplesToCheck; (j < (samplesToCheck + 20)) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 19);
            ok &= (((int8 *) ptr[1])[j] == -1);
            ok &= (((uint16 *) ptr[2])[j] == 38);
            ok &= (((int16 *) ptr[3])[j] == -2);
            ok &= (((uint32 *) ptr[4])[j] == 76);
            ok &= (((int32 *) ptr[5])[j] == -4);
            ok &= (((uint64 *) ptr[6])[j] == 152);
            ok &= (((int64 *) ptr[7])[j] == -8);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[j], static_cast<float32>(1.9));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[j], 1.9);
        }
        samplesToCheck = 20;
        from = 40;
        iteration++;
        if (ok) {
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }
        for (uint32 j = 60; (j < 80) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 39);
            ok &= (((int8 *) ptr[1])[j] == 19);
            ok &= (((uint16 *) ptr[2])[j] == 78);
            ok &= (((int16 *) ptr[3])[j] == 38);
            ok &= (((uint32 *) ptr[4])[j] == 156);
            ok &= (((int32 *) ptr[5])[j] == 76);
            ok &= (((uint64 *) ptr[6])[j] == 312);
            ok &= (((int64 *) ptr[7])[j] == 152);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[j], static_cast<float32>(3.9));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[j], 3.9);
        }
        from = 80;
        iteration++;
        if (ok) {
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise46() {
    MDSReaderTestHelper dS(treeName);
    dS.elementsRead = 100;
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 1, 1, 2);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    uint32 iteration = 0;
    uint32 samplesToCheck = 0u;
    uint32 from = 0;
    if (ok) {
        ok = dS.Synchronise();
        samplesToCheck = 20;
        from = 0;
        if (ok) {
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }
        for (uint32 j = samplesToCheck; (j < (samplesToCheck + 20)) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 19);
            ok &= (((int8 *) ptr[1])[j] == -1);
            ok &= (((uint16 *) ptr[2])[j] == 38);
            ok &= (((int16 *) ptr[3])[j] == -2);
            ok &= (((uint32 *) ptr[4])[j] == 76);
            ok &= (((int32 *) ptr[5])[j] == -4);
            ok &= (((uint64 *) ptr[6])[j] == 152);
            ok &= (((int64 *) ptr[7])[j] == -8);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[j], static_cast<float32>(1.9));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[j], 1.9);
        }
        samplesToCheck = 20;
        from = 40;
        iteration++;
        if (ok) {
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }
        for (uint32 j = 60; (j < 80) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 39);
            ok &= (((int8 *) ptr[1])[j] == 19);
            ok &= (((uint16 *) ptr[2])[j] == 78);
            ok &= (((int16 *) ptr[3])[j] == 38);
            ok &= (((uint32 *) ptr[4])[j] == 156);
            ok &= (((int32 *) ptr[5])[j] == 76);
            ok &= (((uint64 *) ptr[6])[j] == 312);
            ok &= (((int64 *) ptr[7])[j] == 152);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[j], static_cast<float32>(3.9));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[j], 3.9);
        }
        from = 80;
        iteration++;
        if (ok) {
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise47() {
    MDSReaderTestHelper dS(treeName);
    dS.elementsRead = 100;
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 2, 1, 2);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    uint32 iteration = 0;
    uint32 samplesToCheck = 0u;
    uint32 from = 0;
    if (ok) {
        ok = dS.Synchronise();
        samplesToCheck = 20;
        from = 0;
        if (ok) {
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }
        for (uint32 j = samplesToCheck; (j < (samplesToCheck + 20)) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 19);
            ok &= (((int8 *) ptr[1])[j] == -1);
            ok &= (((uint16 *) ptr[2])[j] == 38);
            ok &= (((int16 *) ptr[3])[j] == -2);
            ok &= (((uint32 *) ptr[4])[j] == 76);
            ok &= (((int32 *) ptr[5])[j] == -4);
            ok &= (((uint64 *) ptr[6])[j] == 152);
            ok &= (((int64 *) ptr[7])[j] == -8);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[j], static_cast<float32>(1.9));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[j], 1.9);
        }
        samplesToCheck = 20;
        from = 40;
        iteration++;
        if (ok) {
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }
        for (uint32 j = 60; (j < 80) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 39);
            ok &= (((int8 *) ptr[1])[j] == 19);
            ok &= (((uint16 *) ptr[2])[j] == 78);
            ok &= (((int16 *) ptr[3])[j] == 38);
            ok &= (((uint32 *) ptr[4])[j] == 156);
            ok &= (((int32 *) ptr[5])[j] == 76);
            ok &= (((uint64 *) ptr[6])[j] == 312);
            ok &= (((int64 *) ptr[7])[j] == 152);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[j], static_cast<float32>(3.9));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[j], 3.9);
        }
        from = 80;
        iteration++;
        if (ok) {
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise48() {
    MDSReaderTestHelper dS(treeName);
    dS.elementsRead = 100;
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 0, 0, 3);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    uint32 iteration = 0;
    uint32 samplesToCheck = 0u;
    uint32 from = 0;
    if (ok) {
        ok = dS.Synchronise();

        for (uint32 j = 0; j < 20 && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 0);
            ok &= (((int8 *) ptr[1])[j] == 0);
            ok &= (((uint16 *) ptr[2])[j] == 0);
            ok &= (((int16 *) ptr[3])[j] == 0);
            ok &= (((uint32 *) ptr[4])[j] == 0);
            ok &= (((int32 *) ptr[5])[j] == 0);
            ok &= (((uint64 *) ptr[6])[j] == 0);
            ok &= (((int64 *) ptr[7])[j] == 0);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[j], static_cast<float32>(0.0));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[j], 0.0);
        }
        samplesToCheck = 20;
        from = 20;
        if (ok) {
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }

        for (uint32 j = 40; (j < 60) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 0);
            ok &= (((int8 *) ptr[1])[j] == 0);
            ok &= (((uint16 *) ptr[2])[j] == 0);
            ok &= (((int16 *) ptr[3])[j] == 0);
            ok &= (((uint32 *) ptr[4])[j] == 0);
            ok &= (((int32 *) ptr[5])[j] == 0);
            ok &= (((uint64 *) ptr[6])[j] == 0);
            ok &= (((int64 *) ptr[7])[j] == 0);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[j], static_cast<float32>(0));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[j], 0);
        }
        samplesToCheck = 20;
        from = 60;
        iteration++;
        if (ok) {
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }
        for (uint32 j = 80; (j < 100) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 0);
            ok &= (((int8 *) ptr[1])[j] == 0);
            ok &= (((uint16 *) ptr[2])[j] == 0);
            ok &= (((int16 *) ptr[3])[j] == 0);
            ok &= (((uint32 *) ptr[4])[j] == 0);
            ok &= (((int32 *) ptr[5])[j] == 0);
            ok &= (((uint64 *) ptr[6])[j] == 0);
            ok &= (((int64 *) ptr[7])[j] == 0);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[j], static_cast<float32>(0));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[j], 0);
        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise49() {
    MDSReaderTestHelper dS(treeName);
    dS.elementsRead = 100;
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 1, 0, 3);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    uint32 iteration = 0;
    uint32 samplesToCheck = 0u;
    uint32 from = 0;
    if (ok) {
        ok = dS.Synchronise();

        for (uint32 j = 0; j < 20 && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 0);
            ok &= (((int8 *) ptr[1])[j] == 0);
            ok &= (((uint16 *) ptr[2])[j] == 0);
            ok &= (((int16 *) ptr[3])[j] == 0);
            ok &= (((uint32 *) ptr[4])[j] == 0);
            ok &= (((int32 *) ptr[5])[j] == 0);
            ok &= (((uint64 *) ptr[6])[j] == 0);
            ok &= (((int64 *) ptr[7])[j] == 0);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[j], static_cast<float32>(0.0));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[j], 0.0);
        }
        samplesToCheck = 20;
        from = 20;
        if (ok) {
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }

        for (uint32 j = 40; (j < 60) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 0);
            ok &= (((int8 *) ptr[1])[j] == 0);
            ok &= (((uint16 *) ptr[2])[j] == 0);
            ok &= (((int16 *) ptr[3])[j] == 0);
            ok &= (((uint32 *) ptr[4])[j] == 0);
            ok &= (((int32 *) ptr[5])[j] == 0);
            ok &= (((uint64 *) ptr[6])[j] == 0);
            ok &= (((int64 *) ptr[7])[j] == 0);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[j], static_cast<float32>(0));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[j], 0);
        }
        samplesToCheck = 20;
        from = 60;
        iteration++;
        if (ok) {
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }
        for (uint32 j = 80; (j < 100) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 0);
            ok &= (((int8 *) ptr[1])[j] == 0);
            ok &= (((uint16 *) ptr[2])[j] == 0);
            ok &= (((int16 *) ptr[3])[j] == 0);
            ok &= (((uint32 *) ptr[4])[j] == 0);
            ok &= (((int32 *) ptr[5])[j] == 0);
            ok &= (((uint64 *) ptr[6])[j] == 0);
            ok &= (((int64 *) ptr[7])[j] == 0);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[j], static_cast<float32>(0));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[j], 0);
        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise50() {
    MDSReaderTestHelper dS(treeName);
    dS.elementsRead = 100;
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 2, 0, 3);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    uint32 iteration = 0;
    uint32 samplesToCheck = 0u;
    uint32 from = 0;
    if (ok) {
        ok = dS.Synchronise();

        for (uint32 j = 0; j < 20 && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 0);
            ok &= (((int8 *) ptr[1])[j] == 0);
            ok &= (((uint16 *) ptr[2])[j] == 0);
            ok &= (((int16 *) ptr[3])[j] == 0);
            ok &= (((uint32 *) ptr[4])[j] == 0);
            ok &= (((int32 *) ptr[5])[j] == 0);
            ok &= (((uint64 *) ptr[6])[j] == 0);
            ok &= (((int64 *) ptr[7])[j] == 0);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[j], static_cast<float32>(0.0));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[j], 0.0);
        }
        samplesToCheck = 20;
        from = 20;
        if (ok) {
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }

        for (uint32 j = 40; (j < 60) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 0);
            ok &= (((int8 *) ptr[1])[j] == 0);
            ok &= (((uint16 *) ptr[2])[j] == 0);
            ok &= (((int16 *) ptr[3])[j] == 0);
            ok &= (((uint32 *) ptr[4])[j] == 0);
            ok &= (((int32 *) ptr[5])[j] == 0);
            ok &= (((uint64 *) ptr[6])[j] == 0);
            ok &= (((int64 *) ptr[7])[j] == 0);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[j], static_cast<float32>(0));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[j], 0);
        }
        samplesToCheck = 20;
        from = 60;
        iteration++;
        if (ok) {
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }
        for (uint32 j = 80; (j < 100) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 0);
            ok &= (((int8 *) ptr[1])[j] == 0);
            ok &= (((uint16 *) ptr[2])[j] == 0);
            ok &= (((int16 *) ptr[3])[j] == 0);
            ok &= (((uint32 *) ptr[4])[j] == 0);
            ok &= (((int32 *) ptr[5])[j] == 0);
            ok &= (((uint64 *) ptr[6])[j] == 0);
            ok &= (((int64 *) ptr[7])[j] == 0);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[j], static_cast<float32>(0));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[j], 0);
        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise51() {
    MDSReaderTestHelper dS(treeName);
    dS.elementsRead = 100;
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 0, 1, 3);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    uint32 iteration = 0;
    uint32 samplesToCheck = 0u;
    uint32 from = 0;
    if (ok) {
        ok = dS.Synchronise();
        samplesToCheck = 20;
        from = 0;
        for (uint32 j = 0; (j < 20) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 0);
            ok &= (((int8 *) ptr[1])[j] == 0);
            ok &= (((uint16 *) ptr[2])[j] == 0);
            ok &= (((int16 *) ptr[3])[j] == 0);
            ok &= (((uint32 *) ptr[4])[j] == 0);
            ok &= (((int32 *) ptr[5])[j] == 0);
            ok &= (((uint64 *) ptr[6])[j] == 0);
            ok &= (((int64 *) ptr[7])[j] == 0);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[j], static_cast<float32>(0));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[j], 0);
        }
        samplesToCheck = 20;
        from = 20;

        if (ok) {
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }

        for (uint32 j = 40; (j < 60) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 19);
            ok &= (((int8 *) ptr[1])[j] == -1);
            ok &= (((uint16 *) ptr[2])[j] == 38);
            ok &= (((int16 *) ptr[3])[j] == -2);
            ok &= (((uint32 *) ptr[4])[j] == 76);
            ok &= (((int32 *) ptr[5])[j] == -4);
            ok &= (((uint64 *) ptr[6])[j] == 152);
            ok &= (((int64 *) ptr[7])[j] == -8);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[j], static_cast<float32>(1.9));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[j], 1.9);
        }

        samplesToCheck = 20;
        from = 60;
        iteration++;
        if (ok) {
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }

        for (uint32 j = 80; (j < 100) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 39);
            ok &= (((int8 *) ptr[1])[j] == 19);
            ok &= (((uint16 *) ptr[2])[j] == 78);
            ok &= (((int16 *) ptr[3])[j] == 38);
            ok &= (((uint32 *) ptr[4])[j] == 156);
            ok &= (((int32 *) ptr[5])[j] == 76);
            ok &= (((uint64 *) ptr[6])[j] == 312);
            ok &= (((int64 *) ptr[7])[j] == 152);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[j], static_cast<float32>(3.9));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[j], 3.9);
        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise52() {
    MDSReaderTestHelper dS(treeName);
    dS.elementsRead = 100;
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 1, 1, 3);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    uint32 iteration = 0;
    uint32 samplesToCheck = 0u;
    uint32 from = 0;
    if (ok) {
        ok = dS.Synchronise();
        samplesToCheck = 20;
        from = 0;
        for (uint32 j = 0; (j < 20) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 0);
            ok &= (((int8 *) ptr[1])[j] == 0);
            ok &= (((uint16 *) ptr[2])[j] == 0);
            ok &= (((int16 *) ptr[3])[j] == 0);
            ok &= (((uint32 *) ptr[4])[j] == 0);
            ok &= (((int32 *) ptr[5])[j] == 0);
            ok &= (((uint64 *) ptr[6])[j] == 0);
            ok &= (((int64 *) ptr[7])[j] == 0);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[j], static_cast<float32>(0));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[j], 0);
        }
        samplesToCheck = 20;
        from = 20;

        if (ok) {
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }

        for (uint32 j = 40; (j < 60) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 19);
            ok &= (((int8 *) ptr[1])[j] == -1);
            ok &= (((uint16 *) ptr[2])[j] == 38);
            ok &= (((int16 *) ptr[3])[j] == -2);
            ok &= (((uint32 *) ptr[4])[j] == 76);
            ok &= (((int32 *) ptr[5])[j] == -4);
            ok &= (((uint64 *) ptr[6])[j] == 152);
            ok &= (((int64 *) ptr[7])[j] == -8);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[j], static_cast<float32>(1.9));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[j], 1.9);
        }

        samplesToCheck = 20;
        from = 60;
        iteration++;
        if (ok) {
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }

        for (uint32 j = 80; (j < 100) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 39);
            ok &= (((int8 *) ptr[1])[j] == 19);
            ok &= (((uint16 *) ptr[2])[j] == 78);
            ok &= (((int16 *) ptr[3])[j] == 38);
            ok &= (((uint32 *) ptr[4])[j] == 156);
            ok &= (((int32 *) ptr[5])[j] == 76);
            ok &= (((uint64 *) ptr[6])[j] == 312);
            ok &= (((int64 *) ptr[7])[j] == 152);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[j], static_cast<float32>(3.9));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[j], 3.9);
        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise53() {
    MDSReaderTestHelper dS(treeName);
    dS.elementsRead = 100;
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 2, 1, 3);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    uint32 iteration = 0;
    uint32 samplesToCheck = 0u;
    uint32 from = 0;
    if (ok) {
        ok = dS.Synchronise();
        samplesToCheck = 20;
        from = 0;
        for (uint32 j = 0; (j < 20) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 0);
            ok &= (((int8 *) ptr[1])[j] == 0);
            ok &= (((uint16 *) ptr[2])[j] == 0);
            ok &= (((int16 *) ptr[3])[j] == 0);
            ok &= (((uint32 *) ptr[4])[j] == 0);
            ok &= (((int32 *) ptr[5])[j] == 0);
            ok &= (((uint64 *) ptr[6])[j] == 0);
            ok &= (((int64 *) ptr[7])[j] == 0);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[j], static_cast<float32>(0));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[j], 0);
        }
        samplesToCheck = 20;
        from = 20;

        if (ok) {
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }

        for (uint32 j = 40; (j < 60) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 19);
            ok &= (((int8 *) ptr[1])[j] == -1);
            ok &= (((uint16 *) ptr[2])[j] == 38);
            ok &= (((int16 *) ptr[3])[j] == -2);
            ok &= (((uint32 *) ptr[4])[j] == 76);
            ok &= (((int32 *) ptr[5])[j] == -4);
            ok &= (((uint64 *) ptr[6])[j] == 152);
            ok &= (((int64 *) ptr[7])[j] == -8);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[j], static_cast<float32>(1.9));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[j], 1.9);
        }

        samplesToCheck = 20;
        from = 60;
        iteration++;
        if (ok) {
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }

        from = 80;
        samplesToCheck = 20;
        if (ok) {
            ok &= dS.CompareIntegerValue<uint8>(&(((uint8 *) (ptr[0]))[from]), iteration, 39, samplesToCheck);
            ok &= dS.CompareIntegerValue<int8>(&(((int8 *) ptr[1])[from]), iteration, 19, samplesToCheck);
            ok &= dS.CompareIntegerValue<uint16>(&(((uint16 *) ptr[2])[from]), iteration, 78, samplesToCheck);
            ok &= dS.CompareIntegerValue<int16>(&(((int16 *) ptr[3])[from]), iteration, 38, samplesToCheck);
            ok &= dS.CompareIntegerValue<uint32>(&(((uint32 *) ptr[4])[from]), iteration, 156, samplesToCheck);
            ok &= dS.CompareIntegerValue<int32>(&(((int32 *) ptr[5])[from]), iteration, 76, samplesToCheck);
            ok &= dS.CompareIntegerValue<uint64>(&(((uint64 *) ptr[6])[from]), iteration, 312, samplesToCheck);
            ok &= dS.CompareIntegerValue<int64>(&(((int64 *) ptr[7])[from]), iteration, 152, samplesToCheck);
            ok &= dS.CompareFloatPointValue<float32>(&(((float32 *) ptr[8])[from]), iteration, 3.9, samplesToCheck);
            ok &= dS.CompareFloatPointValue<float64>(&(((float64 *) ptr[9])[from]), iteration, 3.9, samplesToCheck);
        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise54() {
    MDSReaderTestHelper dS(treeName);
    dS.elementsRead = 100;
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 2, 0, 3);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    uint32 iteration = 0;
    uint32 samplesToCheck = 0u;
    uint32 from = 0;
    if (ok) {
        ok = dS.Synchronise();

        from = 0;
        samplesToCheck = 20;
        if (ok) {
            ok &= dS.CompareIntegerValue<uint8>(&(((uint8 *) (ptr[0]))[from]), iteration, 0, samplesToCheck);
            ok &= dS.CompareIntegerValue<int8>(&(((int8 *) ptr[1])[from]), iteration, 0, samplesToCheck);
            ok &= dS.CompareIntegerValue<uint16>(&(((uint16 *) ptr[2])[from]), iteration, 0, samplesToCheck);
            ok &= dS.CompareIntegerValue<int16>(&(((int16 *) ptr[3])[from]), iteration, 0, samplesToCheck);
            ok &= dS.CompareIntegerValue<uint32>(&(((uint32 *) ptr[4])[from]), iteration, 0, samplesToCheck);
            ok &= dS.CompareIntegerValue<int32>(&(((int32 *) ptr[5])[from]), iteration, 0, samplesToCheck);
            ok &= dS.CompareIntegerValue<uint64>(&(((uint64 *) ptr[6])[from]), iteration, 0, samplesToCheck);
            ok &= dS.CompareIntegerValue<int64>(&(((int64 *) ptr[7])[from]), iteration, 0, samplesToCheck);
            ok &= dS.CompareFloatPointValue<float32>(&(((float32 *) ptr[8])[from]), iteration, 0, samplesToCheck);
            ok &= dS.CompareFloatPointValue<float64>(&(((float64 *) ptr[9])[from]), iteration, 0, samplesToCheck);
        }
        samplesToCheck = 20;
        from = 20;
        if (ok) {
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }

        from = 40;
        samplesToCheck = 20;
        if (ok) {
            ok &= dS.CompareIntegerValue<uint8>(&(((uint8 *) (ptr[0]))[from]), iteration, 0, samplesToCheck);
            ok &= dS.CompareIntegerValue<int8>(&(((int8 *) ptr[1])[from]), iteration, 0, samplesToCheck);
            ok &= dS.CompareIntegerValue<uint16>(&(((uint16 *) ptr[2])[from]), iteration, 0, samplesToCheck);
            ok &= dS.CompareIntegerValue<int16>(&(((int16 *) ptr[3])[from]), iteration, 0, samplesToCheck);
            ok &= dS.CompareIntegerValue<uint32>(&(((uint32 *) ptr[4])[from]), iteration, 0, samplesToCheck);
            ok &= dS.CompareIntegerValue<int32>(&(((int32 *) ptr[5])[from]), iteration, 0, samplesToCheck);
            ok &= dS.CompareIntegerValue<uint64>(&(((uint64 *) ptr[6])[from]), iteration, 0, samplesToCheck);
            ok &= dS.CompareIntegerValue<int64>(&(((int64 *) ptr[7])[from]), iteration, 0, samplesToCheck);
            ok &= dS.CompareFloatPointValue<float32>(&(((float32 *) ptr[8])[from]), iteration, 0, samplesToCheck);
            ok &= dS.CompareFloatPointValue<float64>(&(((float64 *) ptr[9])[from]), iteration, 0, samplesToCheck);
        }
        samplesToCheck = 20;
        from = 60;
        iteration++;
        if (ok) {
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }
        from = 80;
        samplesToCheck = 20;
        if (ok) {
            ok &= dS.CompareIntegerValue<uint8>(&(((uint8 *) (ptr[0]))[from]), iteration, 0, samplesToCheck);
            ok &= dS.CompareIntegerValue<int8>(&(((int8 *) ptr[1])[from]), iteration, 0, samplesToCheck);
            ok &= dS.CompareIntegerValue<uint16>(&(((uint16 *) ptr[2])[from]), iteration, 0, samplesToCheck);
            ok &= dS.CompareIntegerValue<int16>(&(((int16 *) ptr[3])[from]), iteration, 0, samplesToCheck);
            ok &= dS.CompareIntegerValue<uint32>(&(((uint32 *) ptr[4])[from]), iteration, 0, samplesToCheck);
            ok &= dS.CompareIntegerValue<int32>(&(((int32 *) ptr[5])[from]), iteration, 0, samplesToCheck);
            ok &= dS.CompareIntegerValue<uint64>(&(((uint64 *) ptr[6])[from]), iteration, 0, samplesToCheck);
            ok &= dS.CompareIntegerValue<int64>(&(((int64 *) ptr[7])[from]), iteration, 0, samplesToCheck);
            ok &= dS.CompareFloatPointValue<float32>(&(((float32 *) ptr[8])[from]), iteration, 0, samplesToCheck);
            ok &= dS.CompareFloatPointValue<float64>(&(((float64 *) ptr[9])[from]), iteration, 0, samplesToCheck);
        }
    }

    if (ok) {
        ok = dS.Synchronise();
        samplesToCheck = 20;
        from = 0;
        if (ok) {
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }

        for (uint32 j = 20; (j < 40) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 0);
            ok &= (((int8 *) ptr[1])[j] == 0);
            ok &= (((uint16 *) ptr[2])[j] == 0);
            ok &= (((int16 *) ptr[3])[j] == 0);
            ok &= (((uint32 *) ptr[4])[j] == 0);
            ok &= (((int32 *) ptr[5])[j] == 0);
            ok &= (((uint64 *) ptr[6])[j] == 0);
            ok &= (((int64 *) ptr[7])[j] == 0);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[j], static_cast<float32>(0));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[j], 0);
        }
        samplesToCheck = 20;
        from = 40;
        iteration++;
        if (ok) {
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }
        for (uint32 j = 60; (j < 80) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 0);
            ok &= (((int8 *) ptr[1])[j] == 0);
            ok &= (((uint16 *) ptr[2])[j] == 0);
            ok &= (((int16 *) ptr[3])[j] == 0);
            ok &= (((uint32 *) ptr[4])[j] == 0);
            ok &= (((int32 *) ptr[5])[j] == 0);
            ok &= (((uint64 *) ptr[6])[j] == 0);
            ok &= (((int64 *) ptr[7])[j] == 0);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[j], static_cast<float32>(0));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[j], 0);
        }
        samplesToCheck = 20;
        from = 80;
        iteration++;
        if (ok) {
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise55() {
    MDSReaderTestHelper dS(treeName);
    bool ok;
    ok = dS.CreateConfigurationFile();
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    if (ok) {
        for (uint32 i = 0u; (i < 199) && ok; i++) {
            ok = dS.Synchronise();
            if (ok) {
                ok &= dS.CompareS_uint8(((uint8 *) ptr[0]), i, dS.elementsRead);
                ok &= dS.CompareS_int8(((int8 *) ptr[1]), i, dS.elementsRead);
                ok &= dS.CompareS_uint16(((uint16 *) ptr[2]), i, dS.elementsRead);
                ok &= dS.CompareS_int16(((int16 *) ptr[3]), i, dS.elementsRead);
                ok &= dS.CompareS_uint32(((uint32 *) ptr[4]), i, dS.elementsRead);
                ok &= dS.CompareS_int32(((int32 *) ptr[5]), i, dS.elementsRead);
                ok &= dS.CompareS_uint64(((uint64 *) ptr[6]), i, dS.elementsRead);
                ok &= dS.CompareS_int64(((int64 *) ptr[7]), i, dS.elementsRead);
                ok &= dS.CompareS_float32(((float32 *) ptr[8]), i, dS.elementsRead);
                ok &= dS.CompareS_float64(((float64 *) ptr[9]), i, dS.elementsRead);
            }
        }
    }
    uint32 iteration = 199;
    if (ok) {
        ok = !dS.Synchronise();
        if (ok) {
            ok &= dS.CompareS_uint8(((uint8 *) ptr[0]), iteration, dS.elementsRead);
            ok &= dS.CompareS_int8(((int8 *) ptr[1]), iteration, dS.elementsRead);
            ok &= dS.CompareS_uint16(((uint16 *) ptr[2]), iteration, dS.elementsRead);
            ok &= dS.CompareS_int16(((int16 *) ptr[3]), iteration, dS.elementsRead);
            ok &= dS.CompareS_uint32(((uint32 *) ptr[4]), iteration, dS.elementsRead);
            ok &= dS.CompareS_int32(((int32 *) ptr[5]), iteration, dS.elementsRead);
            ok &= dS.CompareS_uint64(((uint64 *) ptr[6]), iteration, dS.elementsRead);
            ok &= dS.CompareS_int64(((int64 *) ptr[7]), iteration, dS.elementsRead);
            ok &= dS.CompareS_float32(((float32 *) ptr[8]), iteration, dS.elementsRead);
            ok &= dS.CompareS_float64(((float64 *) ptr[9]), iteration, dS.elementsRead);
        }
    }
    if (ok) {
        iteration++;
        ok = !dS.Synchronise();
        uint32 from = 0;
        if (ok) {
            ok &= dS.CompareIntegerValue<uint8>(&(((uint8 *) (ptr[0]))[from]), iteration, 0, dS.elementsRead);
            ok &= dS.CompareIntegerValue<int8>(&(((int8 *) ptr[1])[from]), iteration, 0, dS.elementsRead);
            ok &= dS.CompareIntegerValue<uint16>(&(((uint16 *) ptr[2])[from]), iteration, 0, dS.elementsRead);
            ok &= dS.CompareIntegerValue<int16>(&(((int16 *) ptr[3])[from]), iteration, 0, dS.elementsRead);
            ok &= dS.CompareIntegerValue<uint32>(&(((uint32 *) ptr[4])[from]), iteration, 0, dS.elementsRead);
            ok &= dS.CompareIntegerValue<int32>(&(((int32 *) ptr[5])[from]), iteration, 0, dS.elementsRead);
            ok &= dS.CompareIntegerValue<uint64>(&(((uint64 *) ptr[6])[from]), iteration, 0, dS.elementsRead);
            ok &= dS.CompareIntegerValue<int64>(&(((int64 *) ptr[7])[from]), iteration, 0, dS.elementsRead);
            ok &= dS.CompareFloatPointValue<float32>(&(((float32 *) ptr[8])[from]), iteration, 0, dS.elementsRead);
            ok &= dS.CompareFloatPointValue<float64>(&(((float64 *) ptr[9])[from]), iteration, 0, dS.elementsRead);
        }
    }
    delete[] ptr;
    return ok;
}
