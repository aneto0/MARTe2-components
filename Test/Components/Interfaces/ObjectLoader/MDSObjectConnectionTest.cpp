/**
 * @file MDSObjectConnectionTest.cpp
 * @brief Source file for class MDSObjectConnectionTest
 * @date 15/5/2025
 * @author nferron
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
 * the class MDSObjectConnectionTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "MDSObjectConnectionTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/


class MDSObjectConnectionTestEnvironment {
public:
    MDSObjectConnectionTestEnvironment() {
        SetupTestEnvironment();
    }

    ~MDSObjectConnectionTestEnvironment() {
        // TODO debug: uncomment this line
        //DeleteTestEnvironment();
    }

    void SetupTestEnvironment();

    void DeleteTestEnvironment();

    bool AddNodes(MDSplus::Tree* const testTree, const StreamString nodePrefix, const uint32 numOfNodesToAdd /* = 1u */);

    /**
     * @brief Add nodes to testTree.
     * @details If testDict is specified, then nodes are inserted in testDict
     *          and then testDict is added to testTree.
     */
    bool AddNodeValues(TypeDescriptor typeIn, StreamString nodePrefix, MDSplus::Dictionary* const testDict /* = NULL_PTR(MDSplus::Dictionary) */);

    MDSplus::Tree* testTree;

    StreamString treeName;
};

bool MDSObjectConnectionTestEnvironment::AddNodes(MDSplus::Tree* const testTree, const StreamString nodePrefix, const uint32 numOfNodesToAdd = 1u) {

    Vector<StreamString> nodeTypeNames;
    nodeTypeNames.SetSize(10u);

    nodeTypeNames[0u] = "UINT8";
    nodeTypeNames[1u] = "UINT16";
    nodeTypeNames[2u] = "UINT32";
    nodeTypeNames[3u] = "UINT64";
    nodeTypeNames[4u] = "INT8";
    nodeTypeNames[5u] = "INT16";
    nodeTypeNames[6u] = "INT32";
    nodeTypeNames[7u] = "INT64";
    nodeTypeNames[8u] = "FLOAT32";
    nodeTypeNames[9u] = "FLOAT64";

    bool ok = false;

    try {
        for (uint32 vecIdx = 0u; vecIdx < nodeTypeNames.GetNumberOfElements(); vecIdx++) {
            StreamString nodeName = nodePrefix;
            nodeName += nodeTypeNames[vecIdx];
            if (numOfNodesToAdd == 1u) {
                testTree->addNode(nodeName.Buffer(), "NUMERIC");
            } else {
                for (uint32 nodeIdx = 1u; nodeIdx <= numOfNodesToAdd; nodeIdx++) {
                    StreamString nthName = nodeName;
                    nthName.Printf("_%u", nodeIdx);
                    testTree->addNode(nthName.Buffer(), "NUMERIC");
                }
            }
        }
        testTree->write();
        ok = true;
    } catch (const MDSplus::MdsException &exc) {
        ok = false;
        REPORT_ERROR_STATIC(ErrorManagement::Exception, "Failed adding nodes. MDSplus exception: %s", exc.what());
    }

    return ok;
}

bool MDSObjectConnectionTestEnvironment::AddNodeValues(TypeDescriptor typeIn, StreamString nodePrefix, MDSplus::Dictionary* const testDict = NULL_PTR(MDSplus::Dictionary*)) {

    bool ok = false;
    StreamString scalarNodeName   = nodePrefix;
    StreamString vectorNodeName   = nodePrefix;
    StreamString matrixNodeName   = nodePrefix;
    StreamString matrix3DNodeName = nodePrefix;
    StreamString scalarSequenceNodeName = nodePrefix;
    scalarNodeName   += "SCAL";
    vectorNodeName   += "VEC";
    matrixNodeName   += "MAT";
    matrix3DNodeName += "MAT3D";
    scalarSequenceNodeName += "NS_";
    StreamString typeSuffix = "";
    MDSplus::Data* scalarData   = NULL_PTR(MDSplus::Data*);
    MDSplus::Data* vectorData   = NULL_PTR(MDSplus::Data*);
    MDSplus::Data* matrixData   = NULL_PTR(MDSplus::Data*);
    MDSplus::Data* matrix3DData = NULL_PTR(MDSplus::Data*);
    Vector<MDSplus::Data*> scalarSequence = Vector<MDSplus::Data*>(4u);

    int32 vectorDims      = 4;
    int32 matrixDims[2]   = {3, 4};
    int32 matrix3DDims[3] = {3, 4, 2};

    try {
        if (typeIn == UnsignedInteger8Bit) {
            uint8 scalarValue = 1;
            uint8 vectorValue[4] = {1, 2, 3, 4};
            uint8 matrixValue[3][4] = { {1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12} };
            uint8 matrix3DValue[3][4][2] = { { { 1, 13}, { 2, 14}, { 3, 15}, { 4, 16} }, { { 5, 17}, { 6, 18}, { 7, 19}, { 8, 20} }, { { 9, 21}, {10, 22}, {11, 23}, {12, 24} } };
            scalarData   = new MDSplus::Uint8(scalarValue);
            vectorData   = new MDSplus::Uint8Array(&vectorValue[0], vectorDims);
            matrixData   = new MDSplus::Uint8Array(&matrixValue[0][0], 2, &matrixDims[0]);
            matrix3DData = new MDSplus::Uint8Array(&matrix3DValue[0][0][0], 3, &matrix3DDims[0]);
            for (uint32 i = 0u; i < scalarSequence.GetNumberOfElements(); i++) {
                scalarSequence[i] = new MDSplus::Uint8(i + 1);
            }
            typeSuffix = "UINT8";
        } else if (typeIn == UnsignedInteger16Bit) {
            uint16 scalarValue = 1;
            uint16 vectorValue[4] = {1, 2, 3, 4};
            uint16 matrixValue[3][4] = { {1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12} };
            uint16 matrix3DValue[3][4][2] = { { { 1, 13}, { 2, 14}, { 3, 15}, { 4, 16} }, { { 5, 17}, { 6, 18}, { 7, 19}, { 8, 20} }, { { 9, 21}, {10, 22}, {11, 23}, {12, 24} } };
            scalarData   = new MDSplus::Uint16(scalarValue);
            vectorData   = new MDSplus::Uint16Array(&vectorValue[0], vectorDims);
            matrixData   = new MDSplus::Uint16Array(&matrixValue[0][0], 2, &matrixDims[0]);
            matrix3DData = new MDSplus::Uint16Array(&matrix3DValue[0][0][0], 3, &matrix3DDims[0]);
            for (uint32 i = 0u; i < scalarSequence.GetNumberOfElements(); i++) {
                scalarSequence[i] = new MDSplus::Uint16(i + 1);
            }
            typeSuffix = "UINT16";
        } else if (typeIn == UnsignedInteger32Bit) {
            uint32 scalarValue = 1;
            uint32 vectorValue[4] = {1, 2, 3, 4};
            uint32 matrixValue[3][4] = { {1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12} };
            uint32 matrix3DValue[3][4][2] = { { { 1, 13}, { 2, 14}, { 3, 15}, { 4, 16} }, { { 5, 17}, { 6, 18}, { 7, 19}, { 8, 20} }, { { 9, 21}, {10, 22}, {11, 23}, {12, 24} } };
            scalarData   = new MDSplus::Uint32(scalarValue);
            vectorData   = new MDSplus::Uint32Array(&vectorValue[0], vectorDims);
            matrixData   = new MDSplus::Uint32Array(&matrixValue[0][0], 2, &matrixDims[0]);
            matrix3DData = new MDSplus::Uint32Array(&matrix3DValue[0][0][0], 3, &matrix3DDims[0]);
            for (uint32 i = 0u; i < scalarSequence.GetNumberOfElements(); i++) {
                scalarSequence[i] = new MDSplus::Uint32(i + 1);
            }
            typeSuffix = "UINT32";
        } else if (typeIn == UnsignedInteger64Bit) {
            uint64 scalarValue = 1;
            uint64 vectorValue[4] = {1, 2, 3, 4};
            uint64 matrixValue[3][4] = { {1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12} };
            uint64 matrix3DValue[3][4][2] = { { { 1, 13}, { 2, 14}, { 3, 15}, { 4, 16} }, { { 5, 17}, { 6, 18}, { 7, 19}, { 8, 20} }, { { 9, 21}, {10, 22}, {11, 23}, {12, 24} } };
            scalarData   = new MDSplus::Uint64(scalarValue);
            vectorData   = new MDSplus::Uint64Array((uint64_t*)&vectorValue[0], vectorDims);
            matrixData   = new MDSplus::Uint64Array((uint64_t*)&matrixValue[0][0], 2, &matrixDims[0]);
            matrix3DData = new MDSplus::Uint64Array((uint64_t*)&matrix3DValue[0][0][0], 3, &matrix3DDims[0]);
            for (uint32 i = 0u; i < scalarSequence.GetNumberOfElements(); i++) {
                scalarSequence[i] = new MDSplus::Uint64(i + 1);
            }
            typeSuffix = "UINT64";
        } else if (typeIn == SignedInteger8Bit) {
            int8 scalarValue = 1;
            int8 vectorValue[4] = {1, -2, 3, -4};
            int8 matrixValue[3][4] = { {1, -2, 3, -4}, {5, -6, 7, -8}, {9, -10, 11, -12} };
            int8 matrix3DValue[3][4][2] = { { { 1, 13}, { -2, -14}, { 3, 15}, { -4, -16} }, { { 5, 17}, { -6, -18}, { 7, 19}, { -8, -20} }, { { 9, 21}, {-10, -22}, {11, 23}, {-12, -24} } };
            scalarData   = new MDSplus::Int8(scalarValue);
            vectorData   = new MDSplus::Int8Array((char*)&vectorValue[0], vectorDims);
            matrixData   = new MDSplus::Int8Array((char*)&matrixValue[0][0], 2, &matrixDims[0]);
            matrix3DData = new MDSplus::Int8Array((char*)&matrix3DValue[0][0][0], 3, &matrix3DDims[0]);
            for (uint32 i = 0u; i < scalarSequence.GetNumberOfElements(); i++) {
                scalarSequence[i] = new MDSplus::Int8(i + 1);
            }
            typeSuffix = "INT8";
        } else if (typeIn == SignedInteger16Bit) {
            int16 scalarValue = 1;
            int16 vectorValue[4] = {1, -2, 3, -4};
            int16 matrixValue[3][4] = { {1, -2, 3, -4}, {5, -6, 7, -8}, {9, -10, 11, -12} };
            int16 matrix3DValue[3][4][2] = { { { 1, 13}, { -2, -14}, { 3, 15}, { -4, -16} }, { { 5, 17}, { -6, -18}, { 7, 19}, { -8, -20} }, { { 9, 21}, {-10, -22}, {11, 23}, {-12, -24} } };
            scalarData   = new MDSplus::Int16(scalarValue);
            vectorData   = new MDSplus::Int16Array(&vectorValue[0], vectorDims);
            matrixData   = new MDSplus::Int16Array(&matrixValue[0][0], 2, &matrixDims[0]);
            matrix3DData = new MDSplus::Int16Array(&matrix3DValue[0][0][0], 3, &matrix3DDims[0]);
            for (uint32 i = 0u; i < scalarSequence.GetNumberOfElements(); i++) {
                scalarSequence[i] = new MDSplus::Int16(i + 1);
            }
            typeSuffix = "INT16";
        } else if (typeIn == SignedInteger32Bit) {
            int32 scalarValue = 1;
            int32 vectorValue[4] = {1, -2, 3, -4};
            int32 matrixValue[3][4] = { {1, -2, 3, -4}, {5, -6, 7, -8}, {9, -10, 11, -12} };
            int32 matrix3DValue[3][4][2] = { { { 1, 13}, { -2, -14}, { 3, 15}, { -4, -16} }, { { 5, 17}, { -6, -18}, { 7, 19}, { -8, -20} }, { { 9, 21}, {-10, -22}, {11, 23}, {-12, -24} } };
            scalarData   = new MDSplus::Int32(scalarValue);
            vectorData   = new MDSplus::Int32Array(&vectorValue[0], vectorDims);
            matrixData   = new MDSplus::Int32Array(&matrixValue[0][0], 2, &matrixDims[0]);
            matrix3DData = new MDSplus::Int32Array(&matrix3DValue[0][0][0], 3, &matrix3DDims[0]);
            for (uint32 i = 0u; i < scalarSequence.GetNumberOfElements(); i++) {
                scalarSequence[i] = new MDSplus::Int32(i + 1);
            }
            typeSuffix = "INT32";
        } else if (typeIn == SignedInteger64Bit) {
            int64 scalarValue = 1;
            int64 vectorValue[4] = {1, -2, 3, -4};
            int64 matrixValue[3][4] = { {1, -2, 3, -4}, {5, -6, 7, -8}, {9, -10, 11, -12} };
            int64 matrix3DValue[3][4][2] = { { { 1, 13}, { -2, -14}, { 3, 15}, { -4, -16} }, { { 5, 17}, { -6, -18}, { 7, 19}, { -8, -20} }, { { 9, 21}, {-10, -22}, {11, 23}, {-12, -24} } };
            scalarData   = new MDSplus::Int64(scalarValue);
            vectorData   = new MDSplus::Int64Array((int64_t*)&vectorValue[0], vectorDims);
            matrixData   = new MDSplus::Int64Array((int64_t*)&matrixValue[0][0], 2, &matrixDims[0]);
            matrix3DData = new MDSplus::Int64Array((int64_t*)&matrix3DValue[0][0][0], 3, &matrix3DDims[0]);
            for (uint32 i = 0u; i < scalarSequence.GetNumberOfElements(); i++) {
                scalarSequence[i] = new MDSplus::Int64(i + 1);
            }
            typeSuffix = "INT64";
        } else if (typeIn == Float32Bit) {
            float32 scalarValue = 1.1;
            float32 vectorValue[4] = {1.1, 2.2, 3.3, 4.4};
            float32 matrixValue[3][4] = { {1.1, 2.2, 3.3, 4.4}, {5.5, 6.6, 7.7, 8.8}, {9.9, 10.1, 11.11, 12.12} };
            float32 matrix3DValue[3][4][2] = { { { 1.1, 13.13}, { 2.2, 14.14}, { 3.3, 15.15}, { 4.4, 16.16} }, { { 5.5, 17.17}, { 6.6, 18.18}, { 7.7, 19.19}, { 8.8, 20.20} }, { { 9.9, 21.21}, {10.10, 22.22}, {11.11, 23.23}, {12.12, 24.24} } };
            scalarData   = new MDSplus::Float32(scalarValue);
            vectorData   = new MDSplus::Float32Array(&vectorValue[0], vectorDims);
            matrixData   = new MDSplus::Float32Array(&matrixValue[0][0], 2, &matrixDims[0]);
            matrix3DData = new MDSplus::Float32Array(&matrix3DValue[0][0][0], 3, &matrix3DDims[0]);
            for (uint32 i = 0u; i < scalarSequence.GetNumberOfElements(); i++) {
                scalarSequence[i] = new MDSplus::Float32(i + 1);
            }
            typeSuffix = "FLOAT32";
        } else if (typeIn == Float64Bit) {
            float64 scalarValue = 1.1;
            float64 vectorValue[4] = {1.1, 2.2, 3.3, 4.4};
            float64 matrixValue[3][4] = { {1.1, 2.2, 3.3, 4.4}, {5.5, 6.6, 7.7, 8.8}, {9.9, 10.10, 11.11, 12.12} };
            float64 matrix3DValue[3][4][2] = { { { 1.1, 13.13}, { 2.2, 14.14}, { 3.3, 15.15}, { 4.4, 16.16} }, { { 5.5, 17.17}, { 6.6, 18.18}, { 7.7, 19.19}, { 8.8, 20.20} }, { { 9.9, 21.21}, {10.10, 22.22}, {11.11, 23.23}, {12.12, 24.24} } };
            scalarData   = new MDSplus::Float64(scalarValue);
            vectorData   = new MDSplus::Float64Array(&vectorValue[0], vectorDims);
            matrixData   = new MDSplus::Float64Array(&matrixValue[0][0], 2, &matrixDims[0]);
            matrix3DData = new MDSplus::Float64Array(&matrix3DValue[0][0][0], 3, &matrix3DDims[0]);
            for (uint32 i = 0u; i < scalarSequence.GetNumberOfElements(); i++) {
                scalarSequence[i] = new MDSplus::Float64(i + 1);
            }
            typeSuffix = "FLOAT64";
        }
        ok = true;
    } catch (const MDSplus::MdsException &exc) {
        REPORT_ERROR_STATIC(ErrorManagement::Exception, "Failed ::Data. MDSplus exception: %s", exc.what());
        ok = false;
    }

    if (ok) {
        scalarNodeName   += typeSuffix;
        vectorNodeName   += typeSuffix;
        matrixNodeName   += typeSuffix;
        matrix3DNodeName += typeSuffix;
        scalarSequenceNodeName += typeSuffix;
        try {
            if (testDict == NULL_PTR(MDSplus::Dictionary*)) {
                MDSplus::TreeNode* scalarNode   = testTree->getNode(scalarNodeName.Buffer());
                MDSplus::TreeNode* vectorNode   = testTree->getNode(vectorNodeName.Buffer());
                MDSplus::TreeNode* matrixNode   = testTree->getNode(matrixNodeName.Buffer());
                MDSplus::TreeNode* matrix3DNode = testTree->getNode(matrix3DNodeName.Buffer());
                scalarNode->putData(scalarData);
                vectorNode->putData(vectorData);
                matrixNode->putData(matrixData);
                matrix3DNode->putData(matrix3DData);
                for (uint32 i = 1u; i <= scalarSequence.GetNumberOfElements(); i++) {
                    StreamString nthName = scalarSequenceNodeName;
                    nthName.Printf("_%u", i);
                    MDSplus::TreeNode* sequenceNode = testTree->getNode(nthName.Buffer());
                    sequenceNode->putData(scalarSequence[i - 1u]);
                }
                ok = true;
            } else {
                MDSplus::String* scalarNodeString   = new MDSplus::String(scalarNodeName.Buffer());
                MDSplus::String* vectorNodeString   = new MDSplus::String(vectorNodeName.Buffer());
                MDSplus::String* matrixNodeString   = new MDSplus::String(matrixNodeName.Buffer());
                MDSplus::String* matrix3DNodeString = new MDSplus::String(matrix3DNodeName.Buffer());
                testDict->setItem(scalarNodeString,   scalarData);
                testDict->setItem(vectorNodeString,   vectorData);
                testDict->setItem(matrixNodeString,   matrixData);
                testDict->setItem(matrix3DNodeString, matrix3DData);
                for (uint32 i = 1u; i <= scalarSequence.GetNumberOfElements(); i++) {
                    StreamString nthName = scalarSequenceNodeName;
                    nthName.Printf("_%u", i);
                    MDSplus::String* scalarSequenceNodeString = new MDSplus::String(nthName.Buffer());
                    testDict->setItem(scalarSequenceNodeString, scalarSequence[i - 1u]);
                }
            }
        } catch (const MDSplus::MdsException &exc) {
            REPORT_ERROR_STATIC(ErrorManagement::Exception, "Failed putData. MDSplus exception: %s", exc.what());
            ok = false;
        }
    }
    return ok;
}

void MDSObjectConnectionTestEnvironment::SetupTestEnvironment() {

    treeName = "mdsoc_ttree";
    StreamString wd       = getenv("PWD");
    StreamString treeEnv  = treeName;
    treeEnv += "_path";
    setenv(treeEnv.Buffer(), wd.Buffer(), true);

    // clean-up
    DeleteTestEnvironment();

    bool ok = false;
    try {
        testTree = new MDSplus::Tree(treeName.Buffer(), -1, "NEW");
        testTree->write();
        ok = true;
    } catch (const MDSplus::MdsException &exc) {
        REPORT_ERROR_STATIC(ErrorManagement::Exception, "Failed creating tree. MDSplus exception: %s", exc.what());
        ok = false;
    }

    if (ok) {
        try {
            testTree->addNode("STRING", "TEXT");
            testTree->addNode("STRUCT", "STRUCTURE");
            testTree->addNode("STRUCT.SUBSTRUCT", "STRUCTURE");
            testTree->addNode("DICT", "ANY");
            testTree->addNode("STRUCTARRAY", "ANY");

            // invalid nodes
            testTree->addNode("INVALIDLIST", "ANY");
            testTree->addNode("MAT4DUINT32", "NUMERIC");
            testTree->addNode("SCALCOMP32", "NUMERIC");
            testTree->addNode("INV_USAGE", "ACTION");
            testTree->addNode("NODATA", "NUMERIC");
            testTree->addNode("INV_STRUCT", "STRUCTURE");
            testTree->addNode("INV_STRUCT.INV_USAGE", "ACTION");
        } catch (const MDSplus::MdsException &exc) {
            REPORT_ERROR_STATIC(ErrorManagement::Debug, "Failed creating structure node. MDSplus exception: %s", exc.what());
            ok = false;
        }
        if (ok) {
            ok &= AddNodes(testTree, "SCAL");
            ok &= AddNodes(testTree, "VEC");
            ok &= AddNodes(testTree, "MAT");
            ok &= AddNodes(testTree, "MAT3D");
            ok &= AddNodes(testTree, "NS_", 4u);
            ok &= AddNodes(testTree, "STRUCT.SCAL");
            ok &= AddNodes(testTree, "STRUCT.VEC");
            ok &= AddNodes(testTree, "STRUCT.MAT");
            ok &= AddNodes(testTree, "STRUCT.MAT3D");
            ok &= AddNodes(testTree, "STRUCT.NS_", 4u);
            ok &= AddNodes(testTree, "STRUCT.SUBSTRUCT.SCAL");
            ok &= AddNodes(testTree, "STRUCT.SUBSTRUCT.VEC");
            ok &= AddNodes(testTree, "STRUCT.SUBSTRUCT.MAT");
            ok &= AddNodes(testTree, "STRUCT.SUBSTRUCT.MAT3D");
            ok &= AddNodes(testTree, "STRUCT.SUBSTRUCT.NS_", 4u);
            if (!ok) {
                REPORT_ERROR_STATIC(ErrorManagement::Debug,"Failed AddNodes");
            }
        }
    }

    if (ok) {
        ok &= AddNodeValues(UnsignedInteger8Bit , "");
        ok &= AddNodeValues(UnsignedInteger16Bit, "");
        ok &= AddNodeValues(UnsignedInteger32Bit, "");
        ok &= AddNodeValues(UnsignedInteger64Bit, "");
        ok &= AddNodeValues(SignedInteger8Bit   , "");
        ok &= AddNodeValues(SignedInteger16Bit  , "");
        ok &= AddNodeValues(SignedInteger32Bit  , "");
        ok &= AddNodeValues(SignedInteger64Bit  , "");
        ok &= AddNodeValues(Float32Bit          , "");
        ok &= AddNodeValues(Float64Bit          , "");
        ok &= AddNodeValues(UnsignedInteger8Bit , "STRUCT.");
        ok &= AddNodeValues(UnsignedInteger16Bit, "STRUCT.");
        ok &= AddNodeValues(UnsignedInteger32Bit, "STRUCT.");
        ok &= AddNodeValues(UnsignedInteger64Bit, "STRUCT.");
        ok &= AddNodeValues(SignedInteger8Bit   , "STRUCT.");
        ok &= AddNodeValues(SignedInteger16Bit  , "STRUCT.");
        ok &= AddNodeValues(SignedInteger32Bit  , "STRUCT.");
        ok &= AddNodeValues(SignedInteger64Bit  , "STRUCT.");
        ok &= AddNodeValues(Float32Bit          , "STRUCT.");
        ok &= AddNodeValues(Float64Bit          , "STRUCT.");
        ok &= AddNodeValues(UnsignedInteger8Bit , "STRUCT.SUBSTRUCT.");
        ok &= AddNodeValues(UnsignedInteger16Bit, "STRUCT.SUBSTRUCT.");
        ok &= AddNodeValues(UnsignedInteger32Bit, "STRUCT.SUBSTRUCT.");
        ok &= AddNodeValues(UnsignedInteger64Bit, "STRUCT.SUBSTRUCT.");
        ok &= AddNodeValues(SignedInteger8Bit   , "STRUCT.SUBSTRUCT.");
        ok &= AddNodeValues(SignedInteger16Bit  , "STRUCT.SUBSTRUCT.");
        ok &= AddNodeValues(SignedInteger32Bit  , "STRUCT.SUBSTRUCT.");
        ok &= AddNodeValues(SignedInteger64Bit  , "STRUCT.SUBSTRUCT.");
        ok &= AddNodeValues(Float32Bit          , "STRUCT.SUBSTRUCT.");
        ok &= AddNodeValues(Float64Bit          , "STRUCT.SUBSTRUCT.");

        MDSplus::String* stringData = new MDSplus::String("Hello World!");
        MDSplus::TreeNode* stringNode = testTree->getNode("STRING");
        stringNode->putData(stringData);

        // invalid numeric node
        uint32 matrix4DValue[2][2][2][2] = { { { {1, 2}, {1, 2} }, { {1, 2}, {1, 2} } }, { { {1, 2}, {1, 2} }, { {1, 2}, {1, 2} } } };
        int32  matrix4DDims[4] = {2, 2, 2, 2};
        MDSplus::Uint32Array* matrix4DData = new MDSplus::Uint32Array(&matrix4DValue[0][0][0][0], 4, &matrix4DDims[0]);
        MDSplus::TreeNode* invalid4DNode = testTree->getNode("MAT4DUINT32");
        invalid4DNode->putData(matrix4DData);

        // invalid node type
        MDSplus::Complex32* invalidScalar = new MDSplus::Complex32(1, -1);
        MDSplus::TreeNode* invalidScalarNode = testTree->getNode("SCALCOMP32");
        invalidScalarNode->putData(invalidScalar);
       if (!ok) {
            REPORT_ERROR_STATIC(ErrorManagement::Debug, "Failed AddNodeValues");
       }
    }

    if (ok) {
        try {
            // struct
            MDSplus::Dictionary* dict = new MDSplus::Dictionary();
            ok &= AddNodeValues(UnsignedInteger8Bit , "", dict);
            ok &= AddNodeValues(UnsignedInteger16Bit, "", dict);
            ok &= AddNodeValues(UnsignedInteger32Bit, "", dict);
            ok &= AddNodeValues(UnsignedInteger64Bit, "", dict);
            ok &= AddNodeValues(SignedInteger8Bit   , "", dict);
            ok &= AddNodeValues(SignedInteger16Bit  , "", dict);
            ok &= AddNodeValues(SignedInteger32Bit  , "", dict);
            ok &= AddNodeValues(SignedInteger64Bit  , "", dict);
            ok &= AddNodeValues(Float32Bit          , "", dict);
            ok &= AddNodeValues(Float64Bit          , "", dict);

            MDSplus::String* subDictName = new MDSplus::String("SUBDICT");
            MDSplus::Dictionary* subDict     = new MDSplus::Dictionary();
            ok &= AddNodeValues(UnsignedInteger8Bit , "", subDict);
            ok &= AddNodeValues(UnsignedInteger16Bit, "", subDict);
            ok &= AddNodeValues(UnsignedInteger32Bit, "", subDict);
            ok &= AddNodeValues(UnsignedInteger64Bit, "", subDict);
            ok &= AddNodeValues(SignedInteger8Bit   , "", subDict);
            ok &= AddNodeValues(SignedInteger16Bit  , "", subDict);
            ok &= AddNodeValues(SignedInteger32Bit  , "", subDict);
            ok &= AddNodeValues(SignedInteger64Bit  , "", subDict);
            ok &= AddNodeValues(Float32Bit          , "", subDict);
            ok &= AddNodeValues(Float64Bit          , "", subDict);
            dict->setItem(subDictName, subDict);

            MDSplus::TreeNode* dictNode = testTree->getNode("DICT");
            dictNode->putData(dict);

            // struct array
            MDSplus::List* structArray = new MDSplus::List();
            structArray->append(dict);
            structArray->append(dict);

            MDSplus::TreeNode* structArrayNode = testTree->getNode("STRUCTARRAY");
            structArrayNode->putData(structArray);

            // invalid list
            MDSplus::List* invalidStructArray = new MDSplus::List();
            MDSplus::Uint8* scalarUint = new MDSplus::Uint8(10);
            MDSplus::Float32* scalarFloat = new MDSplus::Float32(3.14);
            MDSplus::String* stringData = new MDSplus::String("Hello World!");
            invalidStructArray->append(scalarUint);
            invalidStructArray->append(scalarFloat);
            invalidStructArray->append(stringData);

            MDSplus::TreeNode* invalidStructNode = testTree->getNode("INVALIDLIST");
            invalidStructNode->putData(invalidStructArray);
        } catch (const MDSplus::MdsException &exc) {
            /*REPORT_ERROR_STATIC(ErrorManagement::Debug,*/ printf("Failed creating structured node. MDSplus exception: %s", exc.what());
            ok = false;
        }
    }

    if (ok) {
        printf("--started test environment in %s\n", getenv(treeEnv.Buffer()));
    } else {
        printf("--- failed to start test environment\n");
    }
}

void MDSObjectConnectionTestEnvironment::DeleteTestEnvironment() {
    printf("--deleting test environment\n");

    bool ok = false;

    StreamString nameToDelete;
    Directory toDelete;

    nameToDelete = treeName;
    nameToDelete += "_model.tree";
    toDelete.SetByName(nameToDelete.Buffer());
    if (toDelete.Exists()) {
        ok = toDelete.Delete();
        if (!ok) {
            REPORT_ERROR_STATIC(ErrorManagement::Debug, "Failed to delete tree %s", treeName.Buffer());
        }
    }

    if (ok) {
        nameToDelete = treeName;
        nameToDelete += "_model.datafile";
        toDelete.SetByName(nameToDelete.Buffer());
        if (toDelete.Exists()) {
            ok = toDelete.Delete();
            if (!ok) {
                REPORT_ERROR_STATIC(ErrorManagement::Debug, "Failed to delete tree %s", treeName.Buffer());
            }
        }
    }

    if (ok) {
        nameToDelete = treeName;
        nameToDelete += "_model.characteristics";
        toDelete.SetByName(nameToDelete.Buffer());
        if (toDelete.Exists()) {
            ok = toDelete.Delete();
            if (!ok) {
                REPORT_ERROR_STATIC(ErrorManagement::Debug, "Failed to delete tree %s", treeName.Buffer());
            }
        }
    }

    if (testTree != NULL) {
        delete testTree;
    }
}

static MDSObjectConnectionTestEnvironment testEnvironment;

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

MDSObjectConnectionTest::MDSObjectConnectionTest() {

    standardParameters = ""
        "    SCALUINT8 = (uint8) 1"
        "     VECUINT8 = (uint8) {1, 2, 3, 4}"
        "     MATUINT8 = (uint8) { {1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12} }"
        "   MAT3DUINT8 = (uint8) { 1, 13, 2, 14, 3, 15, 4, 16, 5, 17, 6, 18, 7, 19, 8, 20, 9, 21, 10, 22, 11, 23, 12, 24 }"
        "   SCALUINT16 = (uint16) 1"
        "    VECUINT16 = (uint16) {1, 2, 3, 4}"
        "    MATUINT16 = (uint16) { {1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12} }"
        "  MAT3DUINT16 = (uint8)  { 1, 0, 13, 0, 2, 0, 14, 0, 3, 0, 15, 0, 4, 0, 16, 0, 5, 0, 17, 0, 6, 0, 18, 0, 7, 0, 19, 0, 8, 0, 20, 0, 9, 0, 21, 0, 10, 0, 22, 0, 11, 0, 23, 0, 12, 0, 24, 0 } "
        "   SCALUINT32 = (uint32) 1"
        "    VECUINT32 = (uint32) {1, 2, 3, 4}"
        "    MATUINT32 = (uint32) { {1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12} }"
        "  MAT3DUINT32 = (uint8) { 1, 0, 0, 0, 13, 0, 0, 0, 2, 0, 0, 0, 14, 0, 0, 0, 3, 0, 0, 0, 15, 0, 0, 0, 4, 0, 0, 0, 16, 0, 0, 0, 5, 0, 0, 0, 17, 0, 0, 0, 6, 0, 0, 0, 18, 0, 0, 0, 7, 0, 0, 0, 19, 0, 0, 0, 8, 0, 0, 0, 20, 0, 0, 0, 9, 0, 0, 0, 21, 0, 0, 0, 10, 0, 0, 0, 22, 0, 0, 0, 11, 0, 0, 0, 23, 0, 0, 0, 12, 0, 0, 0, 24, 0, 0, 0 }"
        "   SCALUINT64 = (uint64) 1"
        "    VECUINT64 = (uint64) {1, 2, 3, 4}"
        "    MATUINT64 = (uint64) { {1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12} }"
        "  MAT3DUINT64 = (uint8) { 1, 0, 0, 0, 0, 0, 0, 0, 13, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 14, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 15, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 16, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 17, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 18, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 19, 0, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 20, 0, 0, 0, 0, 0, 0, 0, 9, 0, 0, 0, 0, 0, 0, 0, 21, 0, 0, 0, 0, 0, 0, 0, 10, 0, 0, 0, 0, 0, 0, 0, 22, 0, 0, 0, 0, 0, 0, 0, 11, 0, 0, 0, 0, 0, 0, 0, 23, 0, 0, 0, 0, 0, 0, 0, 12, 0, 0, 0, 0, 0, 0, 0, 24, 0, 0, 0, 0, 0, 0, 0 }"
        "     SCALINT8 = (int8) 1"
        "      VECINT8 = (int8) {1, -2, 3, -4}"
        "      MATINT8 = (int8) { {1, -2, 3, -4}, {5, -6, 7, -8}, {9, -10, 11, -12} }"
        "    MAT3DINT8 = (uint8) { 1, 13, 254, 242, 3, 15, 252, 240, 5, 17, 250, 238, 7, 19, 248, 236, 9, 21, 246, 234, 11, 23, 244, 232 }"
        "    SCALINT16 = (int16) 1"
        "     VECINT16 = (int16) {1, -2, 3, -4}"
        "     MATINT16 = (int16) { {1, -2, 3, -4}, {5, -6, 7, -8}, {9, -10, 11, -12} }"
        "   MAT3DINT16 = (uint8) { 1, 0, 13, 0, 254, 255, 242, 255, 3, 0, 15, 0, 252, 255, 240, 255, 5, 0, 17, 0, 250, 255, 238, 255, 7, 0, 19, 0, 248, 255, 236, 255, 9, 0, 21, 0, 246, 255, 234, 255, 11, 0, 23, 0, 244, 255, 232, 255 }"
        "    SCALINT32 = (int32) 1"
        "     VECINT32 = (int32) {1, -2, 3, -4}"
        "     MATINT32 = (int32) { {1, -2, 3, -4}, {5, -6, 7, -8}, {9, -10, 11, -12} }"
        "   MAT3DINT32 = (uint8) { 1, 0, 0, 0, 13, 0, 0, 0, 254, 255, 255, 255, 242, 255, 255, 255, 3, 0, 0, 0, 15, 0, 0, 0, 252, 255, 255, 255, 240, 255, 255, 255, 5, 0, 0, 0, 17, 0, 0, 0, 250, 255, 255, 255, 238, 255, 255, 255, 7, 0, 0, 0, 19, 0, 0, 0, 248, 255, 255, 255, 236, 255, 255, 255, 9, 0, 0, 0, 21, 0, 0, 0, 246, 255, 255, 255, 234, 255, 255, 255, 11, 0, 0, 0, 23, 0, 0, 0, 244, 255, 255, 255, 232, 255, 255, 255 }"
        "    SCALINT64 = (int64) 1"
        "     VECINT64 = (int64) {1, -2, 3, -4}"
        "     MATINT64 = (int64) { {1, -2, 3, -4}, {5, -6, 7, -8}, {9, -10, 11, -12} }"
        "   MAT3DINT64 = (uint8) { 1, 0, 0, 0, 0, 0, 0, 0, 13, 0, 0, 0, 0, 0, 0, 0, 254, 255, 255, 255, 255, 255, 255, 255, 242, 255, 255, 255, 255, 255, 255, 255, 3, 0, 0, 0, 0, 0, 0, 0, 15, 0, 0, 0, 0, 0, 0, 0, 252, 255, 255, 255, 255, 255, 255, 255, 240, 255, 255, 255, 255, 255, 255, 255, 5, 0, 0, 0, 0, 0, 0, 0, 17, 0, 0, 0, 0, 0, 0, 0, 250, 255, 255, 255, 255, 255, 255, 255, 238, 255, 255, 255, 255, 255, 255, 255, 7, 0, 0, 0, 0, 0, 0, 0, 19, 0, 0, 0, 0, 0, 0, 0, 248, 255, 255, 255, 255, 255, 255, 255, 236, 255, 255, 255, 255, 255, 255, 255, 9, 0, 0, 0, 0, 0, 0, 0, 21, 0, 0, 0, 0, 0, 0, 0, 246, 255, 255, 255, 255, 255, 255, 255, 234, 255, 255, 255, 255, 255, 255, 255, 11, 0, 0, 0, 0, 0, 0, 0, 23, 0, 0, 0, 0, 0, 0, 0, 244, 255, 255, 255, 255, 255, 255, 255, 232, 255, 255, 255, 255, 255, 255, 255 }"
        "  SCALFLOAT32 = (float32) 1.1"
        "   VECFLOAT32 = (float32) {1.1, 2.2, 3.3, 4.4}"
        "   MATFLOAT32 = (float32) { {1.1, 2.2, 3.3, 4.4}, {5.5, 6.6, 7.7, 8.8}, {9.9, 10.1, 11.11, 12.12} }"
        " MAT3DFLOAT32 = (uint8) { 205, 204, 140, 63, 123, 20, 82, 65, 205, 204, 12, 64, 113, 61, 98, 65, 51, 51, 83, 64, 102, 102, 114, 65, 205, 204, 140, 64, 174, 71, 129, 65, 0, 0, 176, 64, 41, 92, 137, 65, 51, 51, 211, 64, 164, 112, 145, 65, 102, 102, 246, 64, 31, 133, 153, 65, 205, 204, 12, 65, 154, 153, 161, 65, 102, 102, 30, 65, 20, 174, 169, 65, 154, 153, 33, 65, 143, 194, 177, 65, 143, 194, 49, 65, 10, 215, 185, 65, 133, 235, 65, 65, 133, 235, 193, 65 }"
        "  SCALFLOAT64 = (float64) 1.1 "
        "   VECFLOAT64 = (float64) {1.1, 2.2, 3.3, 4.4}"
        "   MATFLOAT64 = (float64) { {1.1, 2.2, 3.3, 4.4}, {5.5, 6.6, 7.7, 8.8}, {9.9, 10.1, 11.11, 12.12} }"
        " MAT3DFLOAT64 = (uint8) { 154, 153, 153, 153, 153, 153, 241, 63, 195, 245, 40, 92, 143, 66, 42, 64, 154, 153, 153, 153, 153, 153, 1, 64, 72, 225, 122, 20, 174, 71, 44, 64, 102, 102, 102, 102, 102, 102, 10, 64, 205, 204, 204, 204, 204, 76, 46, 64, 154, 153, 153, 153, 153, 153, 17, 64, 41, 92, 143, 194, 245, 40, 48, 64, 0, 0, 0, 0, 0, 0, 22, 64, 236, 81, 184, 30, 133, 43, 49, 64, 102, 102, 102, 102, 102, 102, 26, 64, 174, 71, 225, 122, 20, 46, 50, 64, 205, 204, 204, 204, 204, 204, 30, 64, 113, 61, 10, 215, 163, 48, 51, 64, 154, 153, 153, 153, 153, 153, 33, 64, 51, 51, 51, 51, 51, 51, 52, 64, 205, 204, 204, 204, 204, 204, 35, 64, 246, 40, 92, 143, 194, 53, 53, 64, 51, 51, 51, 51, 51, 51, 36, 64, 184, 30, 133, 235, 81, 56, 54, 64, 184, 30, 133, 235, 81, 56, 38, 64, 123, 20, 174, 71, 225, 58, 55, 64, 61, 10, 215, 163, 112, 61, 40, 64, 61, 10, 215, 163, 112, 61, 56, 64 }"
        "       STRING = \"Hello World!\" "
        ""
        " NS_UINT8_1   = (uint8)   1   NS_UINT8_2   = (uint8)   2   NS_UINT8_3   = (uint8)   3   NS_UINT8_4   = (uint8)   4 "
        " NS_UINT16_1  = (uint16)  1   NS_UINT16_2  = (uint16)  2   NS_UINT16_3  = (uint16)  3   NS_UINT16_4  = (uint16)  4 "
        " NS_UINT32_1  = (uint32)  1   NS_UINT32_2  = (uint32)  2   NS_UINT32_3  = (uint32)  3   NS_UINT32_4  = (uint32)  4 "
        " NS_UINT64_1  = (uint64)  1   NS_UINT64_2  = (uint64)  2   NS_UINT64_3  = (uint64)  3   NS_UINT64_4  = (uint64)  4 "
        " NS_INT8_1    = (int8)    1   NS_INT8_2    = (int8)    2   NS_INT8_3    = (int8)    3   NS_INT8_4    = (int8)    4 "
        " NS_INT16_1   = (int16)   1   NS_INT16_2   = (int16)   2   NS_INT16_3   = (int16)   3   NS_INT16_4   = (int16)   4 "
        " NS_INT32_1   = (int32)   1   NS_INT32_2   = (int32)   2   NS_INT32_3   = (int32)   3   NS_INT32_4   = (int32)   4 "
        " NS_INT64_1   = (int64)   1   NS_INT64_2   = (int64)   2   NS_INT64_3   = (int64)   3   NS_INT64_4   = (int64)   4 "
        " NS_FLOAT32_1 = (float32) 1   NS_FLOAT32_2 = (float32) 2   NS_FLOAT32_3 = (float32) 3   NS_FLOAT32_4 = (float32) 4 "
        " NS_FLOAT64_1 = (float64) 1   NS_FLOAT64_2 = (float64) 2   NS_FLOAT64_3 = (float64) 3   NS_FLOAT64_4 = (float64) 4 "
        ;

    standardParameters.Seek(0u);
    StandardParser parser1(standardParameters, referenceCdbRowMajor);
    parser1.Parse();

    standardParameters = ""
        "    SCALUINT8 = (uint8) 1"
        "     VECUINT8 = (uint8) {1, 2, 3, 4}"
        "     MATUINT8 = (uint8) { { 1, 5, 9 }, { 2, 6, 10 }, { 3, 7, 11 }, { 4, 8, 12 } }"
        "   MAT3DUINT8 = (uint8) { 1,  5,  9,  2,  6, 10,  3,  7, 11,  4,  8, 12,  13, 17, 21, 14, 18, 22, 15, 19, 23, 16, 20, 24 }"
        "   SCALUINT16 = (uint16) 1"
        "    VECUINT16 = (uint16) {1, 2, 3, 4}"
        "    MATUINT16 = (uint16) { { 1, 5, 9 }, { 2, 6, 10 }, { 3, 7, 11 }, { 4, 8, 12 } }"
        "  MAT3DUINT16 = (uint8)  { 1, 0, 5, 0, 9, 0, 2, 0, 6, 0, 10, 0, 3, 0, 7, 0, 11, 0, 4, 0, 8, 0, 12, 0, 13, 0, 17, 0, 21, 0, 14, 0, 18, 0, 22, 0, 15, 0, 19, 0, 23, 0, 16, 0, 20, 0, 24, 0 } "
        "   SCALUINT32 = (uint32) 1"
        "    VECUINT32 = (uint32) {1, 2, 3, 4}"
        "    MATUINT32 = (uint32) { { 1, 5, 9 }, { 2, 6, 10 }, { 3, 7, 11 }, { 4, 8, 12 } }"
        "  MAT3DUINT32 = (uint8) { 1, 0, 0, 0, 5, 0, 0, 0, 9, 0, 0, 0, 2, 0, 0, 0, 6, 0, 0, 0, 10, 0, 0, 0, 3, 0, 0, 0, 7, 0, 0, 0, 11, 0, 0, 0, 4, 0, 0, 0, 8, 0, 0, 0, 12, 0, 0, 0 }"
        "   SCALUINT64 = (uint64) 1"
        "    VECUINT64 = (uint64) {1, 2, 3, 4}"
        "    MATUINT64 = (uint64) { { 1, 5, 9 }, { 2, 6, 10 }, { 3, 7, 11 }, { 4, 8, 12 } }"
        "  MAT3DUINT64 = (uint8) { 1, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 9, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 10, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 11, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 12, 0, 0, 0, 0, 0, 0, 0 }"
        "     SCALINT8 = (int8) 1"
        "      VECINT8 = (int8) {1, -2, 3, -4}"
        "      MATINT8 = (int8) { { 1, 5, 9 }, { -2, -6, -10 }, { 3, 7, 11 }, { -4, -8, -12 } }"
        "    MAT3DINT8 = (uint8) { 1, 5, 9, 254, 250, 246, 3, 7, 11, 252, 248, 244, 13, 17, 21, 242, 238, 234, 15, 19, 23, 240, 236, 232 }"
        "    SCALINT16 = (int16) 1"
        "     VECINT16 = (int16) {1, -2, 3, -4}"
        "     MATINT16 = (int16) { { 1, 5, 9 }, { -2, -6, -10 }, { 3, 7, 11 }, { -4, -8, -12 } }"
        "   MAT3DINT16 = (uint8) { 1, 0, 5, 0, 9, 0, 254, 255, 250, 255, 246, 255, 3, 0, 7, 0, 11, 0, 252, 255, 248, 255, 244, 255, 13, 0, 17, 0, 21, 0, 242, 255, 238, 255, 234, 255, 15, 0, 19, 0, 23, 0, 240, 255, 236, 255, 232, 255 }"
        "    SCALINT32 = (int32) 1"
        "     VECINT32 = (int32) {1, -2, 3, -4}"
        "     MATINT32 = (int32) { { 1, 5, 9 }, { -2, -6, -10 }, { 3, 7, 11 }, { -4, -8, -12 } }"
        "   MAT3DINT32 = (uint8) { 1, 0, 0, 0, 5, 0, 0, 0, 9, 0, 0, 0, 254, 255, 255, 255, 250, 255, 255, 255, 246, 255, 255, 255, 3, 0, 0, 0, 7, 0, 0, 0, 11, 0, 0, 0, 252, 255, 255, 255, 248, 255, 255, 255, 244, 255, 255, 255, 13, 0, 0, 0, 17, 0, 0, 0, 21, 0, 0, 0, 242, 255, 255, 255, 238, 255, 255, 255, 234, 255, 255, 255, 15, 0, 0, 0, 19, 0, 0, 0, 23, 0, 0, 0, 240, 255, 255, 255, 236, 255, 255, 255, 232, 255, 255, 255 }"
        "    SCALINT64 = (int64) 1"
        "     VECINT64 = (int64) {1, -2, 3, -4}"
        "     MATINT64 = (int64) { { 1, 5, 9 }, { -2, -6, -10 }, { 3, 7, 11 }, { -4, -8, -12 } }"
        "   MAT3DINT64 = (uint8) { 1, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 9, 0, 0, 0, 0, 0, 0, 0, 254, 255, 255, 255, 255, 255, 255, 255, 250, 255, 255, 255, 255, 255, 255, 255, 246, 255, 255, 255, 255, 255, 255, 255, 3, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 11, 0, 0, 0, 0, 0, 0, 0, 252, 255, 255, 255, 255, 255, 255, 255, 248, 255, 255, 255, 255, 255, 255, 255, 244, 255, 255, 255, 255, 255, 255, 255, 13, 0, 0, 0, 0, 0, 0, 0, 17, 0, 0, 0, 0, 0, 0, 0, 21, 0, 0, 0, 0, 0, 0, 0, 242, 255, 255, 255, 255, 255, 255, 255, 238, 255, 255, 255, 255, 255, 255, 255, 234, 255, 255, 255, 255, 255, 255, 255, 15, 0, 0, 0, 0, 0, 0, 0, 19, 0, 0, 0, 0, 0, 0, 0, 23, 0, 0, 0, 0, 0, 0, 0, 240, 255, 255, 255, 255, 255, 255, 255, 236, 255, 255, 255, 255, 255, 255, 255, 232, 255, 255, 255, 255, 255, 255, 255 }"
        "  SCALFLOAT32 = (float32) 1.1"
        "   VECFLOAT32 = (float32) {1.1, 2.2, 3.3, 4.4}"
        "   MATFLOAT32 = (float32) { { 1.1, 5.5, 9.9 }, { 2.2, 6.6, 10.10 }, { 3.3, 7.7, 11.11 }, { 4.4, 8.8, 12.12 } }"
        " MAT3DFLOAT32 = (uint8) { 205, 204, 140, 63, 0, 0, 176, 64, 102, 102, 30, 65, 205, 204, 12, 64, 51, 51, 211, 64, 154, 153, 33, 65, 51, 51, 83, 64, 102, 102, 246, 64, 143, 194, 49, 65, 205, 204, 140, 64, 205, 204, 12, 65, 133, 235, 65, 65 }"
        "  SCALFLOAT64 = (float64) 1.1"
        "   VECFLOAT64 = (float64) {1.1, 2.2, 3.3, 4.4}"
        "   MATFLOAT64 = (float64)  { { 1.1, 5.5, 9.9 }, { 2.2, 6.6, 10.10 }, { 3.3, 7.7, 11.11 }, { 4.4, 8.8, 12.12 } }"
        " MAT3DFLOAT64 = (uint8) { 154, 153, 153, 153, 153, 153, 241, 63, 0, 0, 0, 0, 0, 0, 22, 64, 205, 204, 204, 204, 204, 204, 35, 64, 154, 153, 153, 153, 153, 153, 1, 64, 102, 102, 102, 102, 102, 102, 26, 64, 51, 51, 51, 51, 51, 51, 36, 64, 102, 102, 102, 102, 102, 102, 10, 64, 205, 204, 204, 204, 204, 204, 30, 64, 184, 30, 133, 235, 81, 56, 38, 64, 154, 153, 153, 153, 153, 153, 17, 64, 154, 153, 153, 153, 153, 153, 33, 64, 61, 10, 215, 163, 112, 61, 40, 64, 195, 245, 40, 92, 143, 66, 42, 64, 236, 81, 184, 30, 133, 43, 49, 64, 246, 40, 92, 143, 194, 53, 53, 64, 72, 225, 122, 20, 174, 71, 44, 64, 174, 71, 225, 122, 20, 46, 50, 64, 184, 30, 133, 235, 81, 56, 54, 64, 205, 204, 204, 204, 204, 76, 46, 64, 113, 61, 10, 215, 163, 48, 51, 64, 123, 20, 174, 71, 225, 58, 55, 64, 41, 92, 143, 194, 245, 40, 48, 64, 51, 51, 51, 51, 51, 51, 52, 64, 61, 10, 215, 163, 112, 61, 56, 64 }"
        ""
        " NS_UINT8_1   = (uint8)   1   NS_UINT8_2   = (uint8)   2   NS_UINT8_3   = (uint8)   3   NS_UINT8_4   = (uint8)   4 "
        " NS_UINT16_1  = (uint16)  1   NS_UINT16_2  = (uint16)  2   NS_UINT16_3  = (uint16)  3   NS_UINT16_4  = (uint16)  4 "
        " NS_UINT32_1  = (uint32)  1   NS_UINT32_2  = (uint32)  2   NS_UINT32_3  = (uint32)  3   NS_UINT32_4  = (uint32)  4 "
        " NS_UINT64_1  = (uint64)  1   NS_UINT64_2  = (uint64)  2   NS_UINT64_3  = (uint64)  3   NS_UINT64_4  = (uint64)  4 "
        " NS_INT8_1    = (int8)    1   NS_INT8_2    = (int8)    2   NS_INT8_3    = (int8)    3   NS_INT8_4    = (int8)    4 "
        " NS_INT16_1   = (int16)   1   NS_INT16_2   = (int16)   2   NS_INT16_3   = (int16)   3   NS_INT16_4   = (int16)   4 "
        " NS_INT32_1   = (int32)   1   NS_INT32_2   = (int32)   2   NS_INT32_3   = (int32)   3   NS_INT32_4   = (int32)   4 "
        " NS_INT64_1   = (int64)   1   NS_INT64_2   = (int64)   2   NS_INT64_3   = (int64)   3   NS_INT64_4   = (int64)   4 "
        " NS_FLOAT32_1 = (float32) 1   NS_FLOAT32_2 = (float32) 2   NS_FLOAT32_3 = (float32) 3   NS_FLOAT32_4 = (float32) 4 "
        " NS_FLOAT64_1 = (float64) 1   NS_FLOAT64_2 = (float64) 2   NS_FLOAT64_3 = (float64) 3   NS_FLOAT64_4 = (float64) 4 "
        ;

    standardParameters.Seek(0u);
    StandardParser parser2(standardParameters, referenceCdbColMajor);
    parser2.Parse();

    // additional parameters to test extra features
    standardParameters = ""
        " VECUINT8_PAD    = (uint8)   {1, 2, 3, 4, 0, 0} "
        " VECUINT8_TRIM   = (uint8)   {1, 2} "
        " VECUINT16_PAD   = (uint16)  {1, 2, 3, 4, 0, 0} "
        " VECUINT16_TRIM  = (uint16)  {1, 2} "
        " VECUINT32_PAD   = (uint32)  {1, 2, 3, 4, 0, 0} "
        " VECUINT32_TRIM  = (uint32)  {1, 2} "
        " VECUINT64_PAD   = (uint64)  {1, 2, 3, 4, 0, 0} "
        " VECUINT64_TRIM  = (uint64)  {1, 2} "
        " VECINT8_PAD     = (int8)    {1, -2, 3, -4, 0, 0} "
        " VECINT8_TRIM    = (int8)    {1, -2} "
        " VECINT16_PAD    = (int16)   {1, -2, 3, -4, 0, 0} "
        " VECINT16_TRIM   = (int16)   {1, -2} "
        " VECINT32_PAD    = (int32)   {1, -2, 3, -4, 0, 0} "
        " VECINT32_TRIM   = (int32)   {1, -2} "
        " VECINT64_PAD    = (int64)   {1, -2, 3, -4, 0, 0} "
        " VECINT64_TRIM   = (int64)   {1, -2} "
        " VECFLOAT32_PAD  = (float32) {1.1, 2.2, 3.3, 4.4, 0, 0} "
        " VECFLOAT32_TRIM = (float32) {1.1, 2.2} "
        " VECFLOAT64_PAD  = (float64) {1.1, 2.2, 3.3, 4.4, 0, 0} "
        " VECFLOAT64_TRIM = (float64) {1.1, 2.2} "
        " NS_UINT8_SEQ    = (uint8)   { 1, 2, 3, 4 } "
        " NS_UINT16_SEQ   = (uint16)  { 1, 2, 3, 4 } "
        " NS_UINT32_SEQ   = (uint32)  { 1, 2, 3, 4 } "
        " NS_UINT64_SEQ   = (uint64)  { 1, 2, 3, 4 } "
        " NS_INT8_SEQ     = (int8)    { 1, 2, 3, 4 } "
        " NS_INT16_SEQ    = (int16)   { 1, 2, 3, 4 } "
        " NS_INT32_SEQ    = (int32)   { 1, 2, 3, 4 } "
        " NS_INT64_SEQ    = (int64)   { 1, 2, 3, 4 } "
        " NS_FLOAT32_SEQ  = (float32) { 1, 2, 3, 4 } "
        " NS_FLOAT64_SEQ  = (float64) { 1, 2, 3, 4 } "
        ""
        ;

    standardParameters.Seek(0u);
    StandardParser parser3(standardParameters, referenceCdbAdditional);
    parser3.Parse();
}

MDSObjectConnectionTest::~MDSObjectConnectionTest() {
    ObjectRegistryDatabase::Instance()->Purge();
}

bool MDSObjectConnectionTest::TestInitialiseWithConfiguration(StreamString configStreamIn, ErrorManagement::ErrorType& statusOut) {

    ConfigurationDatabase config;
    MDSObjectConnection loader;
    loader.SetName("MDSOC");
    bool ok = TestInitialiseWithConfiguration(configStreamIn, statusOut, config, loader);

    return ok;
}

bool MDSObjectConnectionTest::TestInitialiseWithConfiguration(StreamString configStreamIn, ErrorManagement::ErrorType& statusOut, ConfigurationDatabase& cdbOut, MDSObjectConnection& loader) {

    configStreamIn.Seek(0u);
    StandardParser parser(configStreamIn, cdbOut);
    bool ok = parser.Parse();

    if (ok) {
        ok = loader.Initialise(cdbOut);
        statusOut = loader.GetStatus();
    }

    return ok;
}

bool MDSObjectConnectionTest::TestParameterLoading(MDSObjectConnection& loader, ConfigurationDatabase& referenceCdb) {

    bool ok = true;

    Vector<bool> hasBeenChecked = Vector<bool>(loader.GetSize());
    for (uint32 i = 0u; i < hasBeenChecked.GetNumberOfElements(); i++) {
        hasBeenChecked[i] = false;
    }

    for (uint32 refParIdx = 0u; refParIdx < referenceCdb.GetNumberOfChildren() && ok; refParIdx++) {
        StreamString refParName = referenceCdb.GetChildName(refParIdx);
        for (uint32 loadParIdx = 0u; loadParIdx < loader.GetSize() && ok; loadParIdx++) {
            StreamString loadParName = loader.GetParameterName(loadParIdx);
            if (loadParName.Locate(refParName) >= 0) {
                AnyType refPar = referenceCdb.GetType(refParName.Buffer());
                AnyType* loadPar = loader[loadParIdx];
                ok = (MemoryOperationsHelper::Compare(refPar.GetDataPointer(), loadPar->GetDataPointer(), refPar.GetDataSize()) == 0u );
                if (ok) {
                    hasBeenChecked[loadParIdx] = true;
                } else {
                    REPORT_ERROR_STATIC(ErrorManagement::Debug, "[%u] %s not equal to %s", refParIdx, refParName.Buffer(), loadParName.Buffer());
                }
            }
        }
    }

    // verify that all parameters have been checked
    ok = (hasBeenChecked.GetNumberOfElements() > 0u);
    for (uint32 i = 0u; i < hasBeenChecked.GetNumberOfElements() && ok; i++) {
        ok &= (hasBeenChecked[i] == true);
        if (!ok) {
            REPORT_ERROR_STATIC(ErrorManagement::Debug, "Parameter %s not checked.", loader.GetParameterName(i).Buffer());
        }
    }

    return ok;
}

bool MDSObjectConnectionTest::TestConstructor() {

    MDSObjectConnection loader;

    return loader.GetStatus().ErrorsCleared();
}

bool MDSObjectConnectionTest::TestInitialise() {

    StreamString configStream = ""
        "Class  = MDSObjectConnection                 \n"
        "Tree   = mdsoc_ttree                         \n"
        "Shot   = -1                                  \n"
        "Parameters = {                               \n"
        "    ScalUint8   = { Path = \"SCALUINT8\"   } \n"
        "    ScalUint16  = { Path = \"SCALUINT16\"  } \n"
        "    ScalUint32  = { Path = \"SCALUINT32\"  } \n"
        "    ScalUint64  = { Path = \"SCALUINT64\"  } \n"
        "    ScalInt8    = { Path = \"SCALINT8\"    } \n"
        "    ScalInt16   = { Path = \"SCALINT16\"   } \n"
        "    ScalInt32   = { Path = \"SCALINT32\"   } \n"
        "    ScalInt64   = { Path = \"SCALINT64\"   } \n"
        "    ScalFloat32 = { Path = \"SCALFLOAT32\" } \n"
        "    ScalFloat64 = { Path = \"SCALFLOAT64\" } \n"
        "}                                            \n"
        ""
        ;

    ErrorManagement::ErrorType status = ErrorManagement::FatalError;
    bool ok = TestInitialiseWithConfiguration(configStream, status);

    return (status.ErrorsCleared() && ok);
}

bool MDSObjectConnectionTest::TestInitialise_ThinClient() {

    StreamString configStream = ""
        "Class  = MDSObjectConnection                 \n"
        "ClientType = Thin                            \n"
        "Server = localhost:8002                      \n"
        "Tree   = mdsoc_ttree                         \n"
        "Shot   = -1                                  \n"
        "Parameters = {                               \n"
        "    SCALUINT8   = { Path = \"SCALUINT8\"   } \n"
        "}                                            \n"
        ""
        ;

    ConfigurationDatabase config;
    MDSObjectConnection loader;
    loader.SetName("MDSOC");
    ErrorManagement::ErrorType status = ErrorManagement::FatalError;
    bool ok = TestInitialiseWithConfiguration(configStream, status, config, loader);

    if (ok) {
        ok = TestParameterLoading(loader, referenceCdbRowMajor);
    }

    return (status.ErrorsCleared() && ok);
}

bool MDSObjectConnectionTest::TestInitialise_DistributedClient() {

    StreamString configStream = ""
        "Class  = MDSObjectConnection                 \n"
        "ClientType = Distributed                     \n"
        "Tree   = mdsoc_ttree                         \n"
        "Shot   = -1                                  \n"
        "Parameters = {                               \n"
        "    SCALUINT8   = { Path = \"SCALUINT8\"   } \n"
        "}                                            \n"
        ""
        ;

    ConfigurationDatabase config;
    MDSObjectConnection loader;
    loader.SetName("MDSOC");
    ErrorManagement::ErrorType status = ErrorManagement::FatalError;
    bool ok = TestInitialiseWithConfiguration(configStream, status, config, loader);

    if (ok) {
        ok = TestParameterLoading(loader, referenceCdbRowMajor);
    }

    return (status.ErrorsCleared() && ok);
}

bool MDSObjectConnectionTest::TestInitialise_TargetDim() {

    StreamString configStream = ""
        "Class  = MDSObjectConnection                                     \n"
        "ClientType = Distributed                                         \n"
        "Tree   = mdsoc_ttree                                             \n"
        "Shot   = -1                                                      \n"
        "Parameters = {                                                   \n"
        "    VECUINT8_PAD    = { Path = \"VECUINT8\"    TargetDim = 6  }  \n"
        "    VECUINT8_TRIM   = { Path = \"VECUINT8\"    TargetDim = 2  }  \n"
        "    VECUINT16_PAD   = { Path = \"VECUINT16\"   TargetDim = 6  }  \n"
        "    VECUINT16_TRIM  = { Path = \"VECUINT16\"   TargetDim = 2  }  \n"
        "    VECUINT32_PAD   = { Path = \"VECUINT32\"   TargetDim = 6  }  \n"
        "    VECUINT32_TRIM  = { Path = \"VECUINT32\"   TargetDim = 2  }  \n"
        "    VECUINT64_PAD   = { Path = \"VECUINT64\"   TargetDim = 6  }  \n"
        "    VECUINT64_TRIM  = { Path = \"VECUINT64\"   TargetDim = 2  }  \n"
        "    VECINT8_PAD     = { Path = \"VECINT8\"     TargetDim = 6  }  \n"
        "    VECINT8_TRIM    = { Path = \"VECINT8\"     TargetDim = 2  }  \n"
        "    VECINT16_PAD    = { Path = \"VECINT16\"    TargetDim = 6  }  \n"
        "    VECINT16_TRIM   = { Path = \"VECINT16\"    TargetDim = 2  }  \n"
        "    VECINT32_PAD    = { Path = \"VECINT32\"    TargetDim = 6  }  \n"
        "    VECINT32_TRIM   = { Path = \"VECINT32\"    TargetDim = 2  }  \n"
        "    VECINT64_PAD    = { Path = \"VECINT64\"    TargetDim = 6  }  \n"
        "    VECINT64_TRIM   = { Path = \"VECINT64\"    TargetDim = 2  }  \n"
        "    VECFLOAT32_PAD  = { Path = \"VECFLOAT32\"  TargetDim = 6  }  \n"
        "    VECFLOAT32_TRIM = { Path = \"VECFLOAT32\"  TargetDim = 2  }  \n"
        "    VECFLOAT64_PAD  = { Path = \"VECFLOAT64\"  TargetDim = 6  }  \n"
        "    VECFLOAT64_TRIM = { Path = \"VECFLOAT64\"  TargetDim = 2  }  \n"
        "}                                                                \n"
        ""
        ;

    ConfigurationDatabase config;
    MDSObjectConnection loader;
    loader.SetName("MDSOC");
    ErrorManagement::ErrorType status = ErrorManagement::FatalError;
    bool ok = TestInitialiseWithConfiguration(configStream, status, config, loader);

    if (ok) {
        ok = TestParameterLoading(loader, referenceCdbAdditional);
    }

    return (status.ErrorsCleared() && ok);
}

bool MDSObjectConnectionTest::TestInitialise_StartIdxStopIdx() {

    StreamString configStream = ""
        "Class  = MDSObjectConnection                                                    \n"
        "ClientType = Distributed                                                        \n"
        "Tree   = mdsoc_ttree                                                            \n"
        "Shot   = -1                                                                     \n"
        "Parameters = {                                                                  \n"
        "    NS_UINT8_SEQ    = { Path = \"NS_UINT8_%u\"    StartIdx = 1   StopIdx = 4 }  \n"
        "    NS_UINT16_SEQ   = { Path = \"NS_UINT16_%u\"   StartIdx = 1   StopIdx = 4 }  \n"
        "    NS_UINT32_SEQ   = { Path = \"NS_UINT32_%u\"   StartIdx = 1   StopIdx = 4 }  \n"
        "    NS_UINT64_SEQ   = { Path = \"NS_UINT64_%u\"   StartIdx = 1   StopIdx = 4 }  \n"
        "    NS_INT8_SEQ     = { Path = \"NS_INT8_%u\"     StartIdx = 1   StopIdx = 4 }  \n"
        "    NS_INT16_SEQ    = { Path = \"NS_INT16_%u\"    StartIdx = 1   StopIdx = 4 }  \n"
        "    NS_INT32_SEQ    = { Path = \"NS_INT32_%u\"    StartIdx = 1   StopIdx = 4 }  \n"
        "    NS_INT64_SEQ    = { Path = \"NS_INT64_%u\"    StartIdx = 1   StopIdx = 4 }  \n"
        "    NS_FLOAT32_SEQ  = { Path = \"NS_FLOAT32_%u\"  StartIdx = 1   StopIdx = 4 }  \n"
        "    NS_FLOAT64_SEQ  = { Path = \"NS_FLOAT64_%u\"  StartIdx = 1   StopIdx = 4 }  \n"
        "}                                                                               \n"
        ""
        ;

    ConfigurationDatabase config;
    MDSObjectConnection loader;
    loader.SetName("MDSOC");
    ErrorManagement::ErrorType status = ErrorManagement::FatalError;
    bool ok = TestInitialiseWithConfiguration(configStream, status, config, loader);

    if (ok) {
        ok = TestParameterLoading(loader, referenceCdbAdditional);
    }

    return (status.ErrorsCleared() && ok);
}

bool MDSObjectConnectionTest::TestInitialise_UnlinkedParameter() {

    StreamString configStream = ""
        "Class  = MDSObjectConnection                 \n"
        "ClientType = Distributed                     \n"
        "Tree   = mdsoc_ttree                         \n"
        "Shot   = -1                                  \n"
        "Parameters = {                               \n"
        "    SCALUINT8   = { Path = \"SCALUINT8\"   } \n"
        "    SCALUINT16  = { Path = \"\"   }          \n"
        "    SCALUINT32  = { Path = \"SCALUINT32\"  } \n"
        "}                                            \n"
        ""
        ;

    ConfigurationDatabase config;
    MDSObjectConnection loader;
    loader.SetName("MDSOC");
    ErrorManagement::ErrorType status = ErrorManagement::FatalError;
    bool ok = TestInitialiseWithConfiguration(configStream, status, config, loader);

    return (status.ErrorsCleared() && ok);
}

bool MDSObjectConnectionTest::TestInitialise_NestedParameters() {

    StreamString configStream = ""
        "Class  = MDSObjectConnection                   \n"
        "ClientType = Distributed                       \n"
        "Tree   = mdsoc_ttree                           \n"
        "Shot   = -1                                    \n"
        "Parameters = {                                 \n"
        "    myStruct = {                               \n"
        "        SCALUINT8  = { Path = \"SCALUINT8\" }  \n"
        "        SCALUINT16 = { Path = \"SCALUINT16\" } \n"
        "    }                                          \n"
        "}                                              \n"
        ""
        ;

    ConfigurationDatabase config;
    MDSObjectConnection loader;
    loader.SetName("MDSOC");
    ErrorManagement::ErrorType status = ErrorManagement::FatalError;
    bool ok = TestInitialiseWithConfiguration(configStream, status, config, loader);

    if (ok) {
        ok = TestParameterLoading(loader, referenceCdbRowMajor);
    }

    return (status.ErrorsCleared() && ok);
}

bool MDSObjectConnectionTest::TestInitialise_NestedParameters_WithDash() {

    StreamString configStream = ""
        "Class  = MDSObjectConnection                        \n"
        "ClientType = Distributed                            \n"
        "Tree   = mdsoc_ttree                                \n"
        "Shot   = -1                                         \n"
        "Parameters = {                                      \n"
        "    myStruct-SCALUINT8  = { Path = \"SCALUINT8\"  } \n"
        "    myStruct-SCALUINT16 = { Path = \"SCALUINT16\" } \n"
        "}                                                   \n"
        ""
        ;

    ConfigurationDatabase config;
    MDSObjectConnection loader;
    loader.SetName("MDSOC");
    ErrorManagement::ErrorType status = ErrorManagement::FatalError;
    bool ok = TestInitialiseWithConfiguration(configStream, status, config, loader);

    if (ok) {
        ok = TestParameterLoading(loader, referenceCdbRowMajor);
    }

    return (status.ErrorsCleared() && ok);
}


bool MDSObjectConnectionTest::TestInitialise_RowMajor(StreamString clientType = "Distributed") {

    StreamString configStream = ""
        "Class  = MDSObjectConnection                                                \n"
        "Tree   = mdsoc_ttree                                                        \n"
        "Server = localhost:8002                                                     \n"
        "Shot   = -1                                                                 \n"
        "Parameters = {                                                              \n"
        "    SCALUINT8    = { Path = \"SCALUINT8\"     DataOrientation = RowMajor }  \n"
        "    SCALUINT16   = { Path = \"SCALUINT16\"    DataOrientation = RowMajor }  \n"
        "    SCALUINT32   = { Path = \"SCALUINT32\"    DataOrientation = RowMajor }  \n"
        "    SCALUINT64   = { Path = \"SCALUINT64\"    DataOrientation = RowMajor }  \n"
        "    SCALINT8     = { Path = \"SCALINT8\"      DataOrientation = RowMajor }  \n"
        "    SCALINT16    = { Path = \"SCALINT16\"     DataOrientation = RowMajor }  \n"
        "    SCALINT32    = { Path = \"SCALINT32\"     DataOrientation = RowMajor }  \n"
        "    SCALINT64    = { Path = \"SCALINT64\"     DataOrientation = RowMajor }  \n"
        "    SCALFLOAT32  = { Path = \"SCALFLOAT32\"   DataOrientation = RowMajor }  \n"
        "    SCALFLOAT64  = { Path = \"SCALFLOAT64\"   DataOrientation = RowMajor }  \n"
        "    VECUINT8     = { Path = \"VECUINT8\"      DataOrientation = RowMajor }  \n"
        "    VECUINT16    = { Path = \"VECUINT16\"     DataOrientation = RowMajor }  \n"
        "    VECUINT32    = { Path = \"VECUINT32\"     DataOrientation = RowMajor }  \n"
        "    VECUINT64    = { Path = \"VECUINT64\"     DataOrientation = RowMajor }  \n"
        "    VECINT8      = { Path = \"VECINT8\"       DataOrientation = RowMajor }  \n"
        "    VECINT16     = { Path = \"VECINT16\"      DataOrientation = RowMajor }  \n"
        "    VECINT32     = { Path = \"VECINT32\"      DataOrientation = RowMajor }  \n"
        "    VECINT64     = { Path = \"VECINT64\"      DataOrientation = RowMajor }  \n"
        "    VECFLOAT32   = { Path = \"VECFLOAT32\"    DataOrientation = RowMajor }  \n"
        "    VECFLOAT64   = { Path = \"VECFLOAT64\"    DataOrientation = RowMajor }  \n"
        "    MATUINT8     = { Path = \"MATUINT8\"      DataOrientation = RowMajor }  \n"
        "    MATUINT16    = { Path = \"MATUINT16\"     DataOrientation = RowMajor }  \n"
        "    MATUINT32    = { Path = \"MATUINT32\"     DataOrientation = RowMajor }  \n"
        "    MATUINT64    = { Path = \"MATUINT64\"     DataOrientation = RowMajor }  \n"
        "    MATINT8      = { Path = \"MATINT8\"       DataOrientation = RowMajor }  \n"
        "    MATINT16     = { Path = \"MATINT16\"      DataOrientation = RowMajor }  \n"
        "    MATINT32     = { Path = \"MATINT32\"      DataOrientation = RowMajor }  \n"
        "    MATINT64     = { Path = \"MATINT64\"      DataOrientation = RowMajor }  \n"
        "    MATFLOAT32   = { Path = \"MATFLOAT32\"    DataOrientation = RowMajor }  \n"
        "    MATFLOAT64   = { Path = \"MATFLOAT64\"    DataOrientation = RowMajor }  \n"
        "    MAT3DUINT8   = { Path = \"MAT3DUINT8\"    DataOrientation = RowMajor }  \n"
        "    MAT3DUINT16  = { Path = \"MAT3DUINT16\"   DataOrientation = RowMajor }  \n"
        "    MAT3DUINT32  = { Path = \"MAT3DUINT32\"   DataOrientation = RowMajor }  \n"
        "    MAT3DUINT64  = { Path = \"MAT3DUINT64\"   DataOrientation = RowMajor }  \n"
        "    MAT3DINT8    = { Path = \"MAT3DINT8\"     DataOrientation = RowMajor }  \n"
        "    MAT3DINT16   = { Path = \"MAT3DINT16\"    DataOrientation = RowMajor }  \n"
        "    MAT3DINT32   = { Path = \"MAT3DINT32\"    DataOrientation = RowMajor }  \n"
        "    MAT3DINT64   = { Path = \"MAT3DINT64\"    DataOrientation = RowMajor }  \n"
        "    MAT3DFLOAT32 = { Path = \"MAT3DFLOAT32\"  DataOrientation = RowMajor }  \n"
        "    MAT3DFLOAT64 = { Path = \"MAT3DFLOAT64\"  DataOrientation = RowMajor }  \n"
        "}                                                                           \n"
        ""
        ;

    configStream += "ClientType = ";
    configStream += clientType;

    ConfigurationDatabase config;
    MDSObjectConnection loader;
    loader.SetName("MDSOC");
    ErrorManagement::ErrorType status = ErrorManagement::FatalError;
    bool ok = TestInitialiseWithConfiguration(configStream, status, config, loader);

    if (ok) {
        ok = TestParameterLoading(loader, referenceCdbRowMajor);
    }

    return (status.ErrorsCleared() && ok);
}

bool MDSObjectConnectionTest::TestInitialise_ColMajor(StreamString clientType = "Distributed") {

    StreamString configStream = ""
        "Class  = MDSObjectConnection                                                \n"
        "Tree   = mdsoc_ttree                                                        \n"
        "Server = localhost:8002                                                     \n"
        "Shot   = -1                                                                 \n"
        "Parameters = {                                                              \n"
        "    SCALUINT8    = { Path = \"SCALUINT8\"     DataOrientation = ColumnMajor }  \n"
        "    SCALUINT16   = { Path = \"SCALUINT16\"    DataOrientation = ColumnMajor }  \n"
        "    SCALUINT32   = { Path = \"SCALUINT32\"    DataOrientation = ColumnMajor }  \n"
        "    SCALUINT64   = { Path = \"SCALUINT64\"    DataOrientation = ColumnMajor }  \n"
        "    SCALINT8     = { Path = \"SCALINT8\"      DataOrientation = ColumnMajor }  \n"
        "    SCALINT16    = { Path = \"SCALINT16\"     DataOrientation = ColumnMajor }  \n"
        "    SCALINT32    = { Path = \"SCALINT32\"     DataOrientation = ColumnMajor }  \n"
        "    SCALINT64    = { Path = \"SCALINT64\"     DataOrientation = ColumnMajor }  \n"
        "    SCALFLOAT32  = { Path = \"SCALFLOAT32\"   DataOrientation = ColumnMajor }  \n"
        "    SCALFLOAT64  = { Path = \"SCALFLOAT64\"   DataOrientation = ColumnMajor }  \n"
        "    VECUINT8     = { Path = \"VECUINT8\"      DataOrientation = ColumnMajor }  \n"
        "    VECUINT16    = { Path = \"VECUINT16\"     DataOrientation = ColumnMajor }  \n"
        "    VECUINT32    = { Path = \"VECUINT32\"     DataOrientation = ColumnMajor }  \n"
        "    VECUINT64    = { Path = \"VECUINT64\"     DataOrientation = ColumnMajor }  \n"
        "    VECINT8      = { Path = \"VECINT8\"       DataOrientation = ColumnMajor }  \n"
        "    VECINT16     = { Path = \"VECINT16\"      DataOrientation = ColumnMajor }  \n"
        "    VECINT32     = { Path = \"VECINT32\"      DataOrientation = ColumnMajor }  \n"
        "    VECINT64     = { Path = \"VECINT64\"      DataOrientation = ColumnMajor }  \n"
        "    VECFLOAT32   = { Path = \"VECFLOAT32\"    DataOrientation = ColumnMajor }  \n"
        "    VECFLOAT64   = { Path = \"VECFLOAT64\"    DataOrientation = ColumnMajor }  \n"
        "    MATUINT8     = { Path = \"MATUINT8\"      DataOrientation = ColumnMajor }  \n"
        "    MATUINT16    = { Path = \"MATUINT16\"     DataOrientation = ColumnMajor }  \n"
        "    MATUINT32    = { Path = \"MATUINT32\"     DataOrientation = ColumnMajor }  \n"
        "    MATUINT64    = { Path = \"MATUINT64\"     DataOrientation = ColumnMajor }  \n"
        "    MATINT8      = { Path = \"MATINT8\"       DataOrientation = ColumnMajor }  \n"
        "    MATINT16     = { Path = \"MATINT16\"      DataOrientation = ColumnMajor }  \n"
        "    MATINT32     = { Path = \"MATINT32\"      DataOrientation = ColumnMajor }  \n"
        "    MATINT64     = { Path = \"MATINT64\"      DataOrientation = ColumnMajor }  \n"
        "    MATFLOAT32   = { Path = \"MATFLOAT32\"    DataOrientation = ColumnMajor }  \n"
        "    MATFLOAT64   = { Path = \"MATFLOAT64\"    DataOrientation = ColumnMajor }  \n"
        "    MAT3DUINT8   = { Path = \"MAT3DUINT8\"    DataOrientation = ColumnMajor }  \n"
        "    MAT3DUINT16  = { Path = \"MAT3DUINT16\"   DataOrientation = ColumnMajor }  \n"
        "    MAT3DUINT32  = { Path = \"MAT3DUINT32\"   DataOrientation = ColumnMajor }  \n"
        "    MAT3DUINT64  = { Path = \"MAT3DUINT64\"   DataOrientation = ColumnMajor }  \n"
        "    MAT3DINT8    = { Path = \"MAT3DINT8\"     DataOrientation = ColumnMajor }  \n"
        "    MAT3DINT16   = { Path = \"MAT3DINT16\"    DataOrientation = ColumnMajor }  \n"
        "    MAT3DINT32   = { Path = \"MAT3DINT32\"    DataOrientation = ColumnMajor }  \n"
        "    MAT3DINT64   = { Path = \"MAT3DINT64\"    DataOrientation = ColumnMajor }  \n"
        "    MAT3DFLOAT32 = { Path = \"MAT3DFLOAT32\"  DataOrientation = ColumnMajor }  \n"
        "    MAT3DFLOAT64 = { Path = \"MAT3DFLOAT64\"  DataOrientation = ColumnMajor }  \n"
        "}                                                                           \n"
        ""
        ;

    configStream += "ClientType = ";
    configStream += clientType;

    ConfigurationDatabase config;
    MDSObjectConnection loader;
    loader.SetName("MDSOC");
    ErrorManagement::ErrorType status = ErrorManagement::FatalError;
    bool ok = TestInitialiseWithConfiguration(configStream, status, config, loader);

    if (ok) {
        ok = TestParameterLoading(loader, referenceCdbColMajor);
    }

    return (status.ErrorsCleared() && ok);
}


bool MDSObjectConnectionTest::TestInitialise_String(StreamString clientType = "Distributed") {

    StreamString configStream = ""
        "Class  = MDSObjectConnection              \n"
        "Tree   = mdsoc_ttree                      \n"
        "Server = localhost:8002                   \n"
        "Shot   = -1                               \n"
        "Parameters = {                            \n"
        "    STRING = { Path = \"STRING\" DataOrientation = RowMajor } \n"
        "}                                         \n"
        ""
        ;

    configStream += "ClientType = ";
    configStream += clientType;

    ConfigurationDatabase config;
    MDSObjectConnection loader;
    loader.SetName("MDSOC");
    ErrorManagement::ErrorType status = ErrorManagement::FatalError;
    bool ok = TestInitialiseWithConfiguration(configStream, status, config, loader);

    if (ok) {
        ok = TestParameterLoading(loader, referenceCdbRowMajor);
    }

    return (status.ErrorsCleared() && ok);
}

bool MDSObjectConnectionTest::TestInitialise_Struct_RowMajor(StreamString clientType = "Distributed") {

    StreamString configStream = ""
        "Class  = MDSObjectConnection              \n"
        "Tree   = mdsoc_ttree                      \n"
        "Server = localhost:8002                   \n"
        "Shot   = -1                               \n"
        "Parameters = {                            \n"
        "    StructParameter = { Path = \"STRUCT\" DataOrientation = RowMajor } \n"
        "}                                         \n"
        ""
        ;

    configStream += "ClientType = ";
    configStream += clientType;

    ConfigurationDatabase config;
    MDSObjectConnection loader;
    loader.SetName("MDSOC");
    ErrorManagement::ErrorType status = ErrorManagement::FatalError;
    bool ok = TestInitialiseWithConfiguration(configStream, status, config, loader);

    if (ok) {
        ok = TestParameterLoading(loader, referenceCdbRowMajor);
    }

    return (status.ErrorsCleared() && ok);
}

bool MDSObjectConnectionTest::TestInitialise_DictAsStruct_RowMajor(StreamString clientType = "Distributed") {

    StreamString configStream = ""
        "Class  = MDSObjectConnection              \n"
        "Tree   = mdsoc_ttree                      \n"
        "Server = localhost:8002                   \n"
        "Shot   = -1                               \n"
        "Parameters = {                            \n"
        "    StructParameter = { Path = \"DICT\" DataOrientation = RowMajor } \n"
        "}                                         \n"
        ""
        ;

    configStream += "ClientType = ";
    configStream += clientType;

    ConfigurationDatabase config;
    MDSObjectConnection loader;
    loader.SetName("MDSOC");
    ErrorManagement::ErrorType status = ErrorManagement::FatalError;
    bool ok = TestInitialiseWithConfiguration(configStream, status, config, loader);

    if (ok) {
        ok = TestParameterLoading(loader, referenceCdbRowMajor);
    }

    return (status.ErrorsCleared() && ok);
}

bool MDSObjectConnectionTest::TestInitialise_DictAsStruct_ColMajor(StreamString clientType = "Distributed") {

    StreamString configStream = ""
        "Class  = MDSObjectConnection              \n"
        "Tree   = mdsoc_ttree                      \n"
        "Server = localhost:8002                   \n"
        "Shot   = -1                               \n"
        "Parameters = {                            \n"
        "    StructParameter = { Path = \"DICT\" DataOrientation = ColumnMajor } \n"
        "}                                         \n"
        ""
        ;

    configStream += "ClientType = ";
    configStream += clientType;

    ConfigurationDatabase config;
    MDSObjectConnection loader;
    loader.SetName("MDSOC");
    ErrorManagement::ErrorType status = ErrorManagement::FatalError;
    bool ok = TestInitialiseWithConfiguration(configStream, status, config, loader);

    if (ok) {
        ok = TestParameterLoading(loader, referenceCdbColMajor);
    }

    return (status.ErrorsCleared() && ok);
}

bool MDSObjectConnectionTest::TestInitialise_StructArray_RowMajor(StreamString clientType = "Distributed") {

    StreamString configStream = ""
        "Class  = MDSObjectConnection              \n"
        "ClientType = Thin                         \n"
        "Server = localhost:8002                   \n"
        "Tree   = mdsoc_ttree                      \n"
        "Shot   = -1                               \n"
        "Parameters = {                            \n"
        "    StructParameter = { Path = \"STRUCTARRAY\" DataOrientation = RowMajor } \n"
        "}                                         \n"
        ""
        ;

    configStream += "ClientType = ";
    configStream += clientType;

    ConfigurationDatabase config;
    MDSObjectConnection loader;
    loader.SetName("MDSOC");
    ErrorManagement::ErrorType status = ErrorManagement::FatalError;
    bool ok = TestInitialiseWithConfiguration(configStream, status, config, loader);

    if (ok) {
        ok = TestParameterLoading(loader, referenceCdbRowMajor);
    }

    return (status.ErrorsCleared() && ok);
}

bool MDSObjectConnectionTest::TestInitialise_StructArray_ColMajor(StreamString clientType = "Distributed") {

    StreamString configStream = ""
        "Class  = MDSObjectConnection              \n"
        "Tree   = mdsoc_ttree                      \n"
        "Shot   = -1                               \n"
        "Server = localhost:8002                   \n"
        "Parameters = {                            \n"
        "    StructParameter = { Path = \"STRUCTARRAY\" DataOrientation = ColumnMajor } \n"
        "}                                         \n"
        ""
        ;

    configStream += "ClientType = ";
    configStream += clientType;

    ConfigurationDatabase config;
    MDSObjectConnection loader;
    loader.SetName("MDSOC");
    ErrorManagement::ErrorType status = ErrorManagement::FatalError;
    bool ok = TestInitialiseWithConfiguration(configStream, status, config, loader);

    if (ok) {
        ok = TestParameterLoading(loader, referenceCdbColMajor);
    }

    return (status.ErrorsCleared() && ok);
}

bool MDSObjectConnectionTest::TestInitialise_NoTree_Failed() {

    StreamString configStream = ""
        "Class  = MDSObjectConnection                 \n"
        "Shot   = -1                                  \n"
        "Parameters = {                               \n"
        "    ScalUint8   = { Path = \"SCALUINT8\"   } \n"
        "}                                            \n"
        ""
        ;

    ErrorManagement::ErrorType status = ErrorManagement::FatalError;
    bool ok = TestInitialiseWithConfiguration(configStream, status);

    return (status.parametersError && !ok);
}

bool MDSObjectConnectionTest::TestInitialise_NoShot_Failed() {

    StreamString configStream = ""
        "Class  = MDSObjectConnection                 \n"
        "Tree   = mdsoc_ttree                         \n"
        "Parameters = {                               \n"
        "    ScalUint8   = { Path = \"SCALUINT8\"   } \n"
        "}                                            \n"
        ""
        ;

    ErrorManagement::ErrorType status = ErrorManagement::FatalError;
    bool ok = TestInitialiseWithConfiguration(configStream, status);

    return (status.parametersError && !ok);
}

bool MDSObjectConnectionTest::TestInitialise_ThinClient_NoServer_Failed() {

    StreamString configStream = ""
        "Class  = MDSObjectConnection                 \n"
        "ClientType = Thin                            \n"
        "Tree   = mdsoc_ttree                         \n"
        "Shot   = -1                                  \n"
        "Parameters = {                               \n"
        "    ScalUint8   = { Path = \"SCALUINT8\"   } \n"
        "}                                            \n"
        ""
        ;

    ErrorManagement::ErrorType status = ErrorManagement::FatalError;
    bool ok = TestInitialiseWithConfiguration(configStream, status);

    return (status.internalSetupError && !ok);
}

bool MDSObjectConnectionTest::TestInitialise_WrongClient_Failed() {

    StreamString configStream = ""
        "Class  = MDSObjectConnection                 \n"
        "ClientType = Thick                           \n"
        "Server = localhost:8002                      \n"
        "Tree   = mdsoc_ttree                         \n"
        "Shot   = -1                                  \n"
        "Parameters = {                               \n"
        "    ScalUint8   = { Path = \"SCALUINT8\"   } \n"
        "}                                            \n"
        ""
        ;

    ErrorManagement::ErrorType status = ErrorManagement::FatalError;
    bool ok = TestInitialiseWithConfiguration(configStream, status);

    return (status.unsupportedFeature && !ok);
}

bool MDSObjectConnectionTest::TestInitialise_NoParameters_Failed() {

    StreamString configStream = ""
        "Class  = MDSObjectConnection                 \n"
        "ClientType = Thin                            \n"
        "Server = localhost:8002                      \n"
        "Tree   = mdsoc_ttree                         \n"
        "Shot   = -1                                  \n"
        ""
        ;

    ErrorManagement::ErrorType status = ErrorManagement::FatalError;
    bool ok = TestInitialiseWithConfiguration(configStream, status);

    return (status.parametersError && !ok);
}

bool MDSObjectConnectionTest::TestInitialise_NoPath_Failed() {

    StreamString configStream = ""
        "Class  = MDSObjectConnection                 \n"
        "ClientType = Thin                            \n"
        "Server = localhost:8002                      \n"
        "Tree   = mdsoc_ttree                         \n"
        "Shot   = -1                                  \n"
        "Parameters = {                               \n"
        "    SCALUINT8   = { NotAPath = \"BYE!\"   }  \n"
        "}                                            \n"
        ""
        ;

    ErrorManagement::ErrorType status = ErrorManagement::FatalError;
    bool ok = TestInitialiseWithConfiguration(configStream, status);

    return (status.parametersError && !ok);
}

bool MDSObjectConnectionTest::TestInitialise_TargetDimAndStartIdx_Failed() {

    StreamString configStream = ""
        "Class  = MDSObjectConnection                                           \n"
        "ClientType = Thin                                                      \n"
        "Server = localhost:8002                                                \n"
        "Tree   = mdsoc_ttree                                                   \n"
        "Shot   = -1                                                            \n"
        "Parameters = {                                                         \n"
        "    SCALUINT8   = { Path = \"NS_UINT8_%u\" TargetDim = 5 StartIdx = 1 StopIdx = 4 } \n"
        "}                                                                      \n"
        ""
        ;

    ErrorManagement::ErrorType status = ErrorManagement::FatalError;
    bool ok = TestInitialiseWithConfiguration(configStream, status);

    return (status.unsupportedFeature && !ok);
}

bool MDSObjectConnectionTest::TestInitialise_WrongOrientation_Failed() {

    StreamString configStream = ""
        "Class  = MDSObjectConnection                                           \n"
        "ClientType = Thin                                                      \n"
        "Server = localhost:8002                                                \n"
        "Tree   = mdsoc_ttree                                                   \n"
        "Shot   = -1                                                            \n"
        "Parameters = {                                                         \n"
        "    SCALUINT8   = { Path = \"SCALUINT8\" DataOrientation = PageMajor } \n"
        "}                                                                      \n"
        ""
        ;

    ErrorManagement::ErrorType status = ErrorManagement::FatalError;
    bool ok = TestInitialiseWithConfiguration(configStream, status);

    return (status.unsupportedFeature && !ok);
}

bool MDSObjectConnectionTest::TestInitialise_WrongPath_Failed() {

    StreamString configStream = ""
        "Class  = MDSObjectConnection              \n"
        "Tree   = mdsoc_ttree                      \n"
        "Shot   = -1                               \n"
        "Parameters = {                            \n"
        "    SCALUINT8   = { Path = \"NOTAPATH\" } \n"
        "}                                         \n"
        ""
        ;

    ErrorManagement::ErrorType status = ErrorManagement::FatalError;
    bool ok = TestInitialiseWithConfiguration(configStream, status);

    return (status.exception && !ok);
}

bool MDSObjectConnectionTest::TestInitialise_InvalidListItems_Failed() {

    StreamString configStream = ""
        "Class  = MDSObjectConnection              \n"
        "Tree   = mdsoc_ttree                      \n"
        "Shot   = -1                               \n"
        "Parameters = {                            \n"
        "    INVALIDLIST   = { Path = \"INVALIDLIST\" } \n"
        "}                                         \n"
        ""
        ;

    ErrorManagement::ErrorType status = ErrorManagement::FatalError;
    bool ok = TestInitialiseWithConfiguration(configStream, status);

    return (status.unsupportedFeature && !ok);
}

bool MDSObjectConnectionTest::TestInitialise_Invalid4DMatrix_Failed() {

    StreamString configStream = ""
        "Class  = MDSObjectConnection              \n"
        "Tree   = mdsoc_ttree                      \n"
        "Shot   = -1                               \n"
        "Parameters = {                            \n"
        "    MAT4DUINT32   = { Path = \"MAT4DUINT32\" } \n"
        "}                                         \n"
        ""
        ;

    ErrorManagement::ErrorType status = ErrorManagement::FatalError;
    bool ok = TestInitialiseWithConfiguration(configStream, status);

    return (status.unsupportedFeature && !ok);
}

bool MDSObjectConnectionTest::TestInitialise_UnsupportedDataType_Failed() {

    StreamString configStream = ""
        "Class  = MDSObjectConnection              \n"
        "Tree   = mdsoc_ttree                      \n"
        "Shot   = -1                               \n"
        "Parameters = {                            \n"
        "    SCALCOMP32   = { Path = \"SCALCOMP32\" } \n"
        "}                                         \n"
        ""
        ;

    ErrorManagement::ErrorType status = ErrorManagement::FatalError;
    bool ok = TestInitialiseWithConfiguration(configStream, status);

    return (status.unsupportedFeature && !ok);
}

bool MDSObjectConnectionTest::TestInitialise_StringColMajor_Failed() {

    StreamString configStream = ""
        "Class  = MDSObjectConnection              \n"
        "Tree   = mdsoc_ttree                      \n"
        "Shot   = -1                               \n"
        "Parameters = {                            \n"
        "    STRING   = { Path = \"STRING\"   DataOrientation = ColumnMajor } \n"
        "}                                         \n"
        ""
        ;

    ErrorManagement::ErrorType status = ErrorManagement::FatalError;
    bool ok = TestInitialiseWithConfiguration(configStream, status);

    return (status.illegalOperation && !ok);
}

bool MDSObjectConnectionTest::TestInitialise_InvalidNodeUsage_Failed() {

    StreamString configStream = ""
        "Class  = MDSObjectConnection              \n"
        "Tree   = mdsoc_ttree                      \n"
        "Shot   = -1                               \n"
        "Parameters = {                            \n"
        "    STRING   = { Path = \"INV_USAGE\" }   \n"
        "}                                         \n"
        ""
        ;

    ErrorManagement::ErrorType status = ErrorManagement::FatalError;
    bool ok = TestInitialiseWithConfiguration(configStream, status);

    return (status.unsupportedFeature && !ok);
}

bool MDSObjectConnectionTest::TestInitialise_NoDataNode_Failed() {

    StreamString configStream = ""
        "Class  = MDSObjectConnection              \n"
        "Tree   = mdsoc_ttree                      \n"
        "Shot   = -1                               \n"
        "Parameters = {                            \n"
        "    STRING   = { Path = \"NODATA\" }      \n"
        "}                                         \n"
        ""
        ;

    ErrorManagement::ErrorType status = ErrorManagement::FatalError;
    bool ok = TestInitialiseWithConfiguration(configStream, status);

    return (status.communicationError && !ok);
}

bool MDSObjectConnectionTest::TestInitialise_InvalidStructureSubnode_Failed() {

    StreamString configStream = ""
        "Class  = MDSObjectConnection              \n"
        "Tree   = mdsoc_ttree                      \n"
        "Shot   = -1                               \n"
        "Parameters = {                            \n"
        "    STRING   = { Path = \"INV_STRUCT\" }  \n"
        "}                                         \n"
        ""
        ;

    ErrorManagement::ErrorType status = ErrorManagement::FatalError;
    bool ok = TestInitialiseWithConfiguration(configStream, status);

    return (status.communicationError && !ok);
}
