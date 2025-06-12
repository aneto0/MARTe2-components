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

// TestMDSObjectConnection::TestMDSObjectConnection() {}
//
// TestMDSObjectConnection::~TestMDSObjectConnection() {}
//
// ErrorManagement::ErrorType TestMDSObjectConnection::TestTransposeAndCopy(void *const destination, const void *const source, const TypeDescriptor typeDesc,
//     const uint32 numberOfRows, const uint32 numberOfColumns, const uint32 numberOfPages) {
//
//     return TransposeAndCopy(destination, source, typeDesc, numberOfRows, numberOfColumns, numberOfPages);
// }
//
// CLASS_REGISTER(TestMDSObjectConnection, "1.0");

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

    bool AddNodeValues(TypeDescriptor typeIn);

    MDSplus::Tree* testTree;

    StreamString treeName;
};

bool MDSObjectConnectionTestEnvironment::AddNodeValues(TypeDescriptor typeIn) {

    bool ok = false;
    StreamString scalarNodeName   = "SCAL";
    StreamString vectorNodeName   = "VEC";
    StreamString matrixNodeName   = "MAT";
    StreamString matrix3DNodeName = "MAT3D";
    StreamString typeSuffix = "";
    MDSplus::Data* scalarData   = NULL_PTR(MDSplus::Data*);
    MDSplus::Data* vectorData   = NULL_PTR(MDSplus::Data*);
    MDSplus::Data* matrixData   = NULL_PTR(MDSplus::Data*);
    MDSplus::Data* matrix3DData = NULL_PTR(MDSplus::Data*);

    int32 vectorDims      = 4;
    int32 matrixDims[2]   = {3, 4};
    int32 matrix3DDims[3] = {2, 3, 4};

    try {
        if (typeIn == UnsignedInteger8Bit) {
            uint8 scalarValue = 1;
            uint8 vectorValue[4] = {1, 2, 3, 4};
            uint8 matrixValue[3][4] = { {1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12} };
            uint8 matrix3DValue[2][3][4] = { { {1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12} }, { {13, 14, 15, 16}, {17, 18, 19, 20}, {21, 22, 23, 24} } };
            scalarData   = new MDSplus::Uint8(scalarValue);
            vectorData   = new MDSplus::Uint8Array(&vectorValue[0], vectorDims);
            matrixData   = new MDSplus::Uint8Array(&matrixValue[0][0], 2, &matrixDims[0]);
            matrix3DData = new MDSplus::Uint8Array(&matrix3DValue[0][0][0], 3, &matrix3DDims[0]);
            typeSuffix = "UINT8";
        } else if (typeIn == UnsignedInteger16Bit) {
            uint16 scalarValue = 1;
            uint16 vectorValue[4] = {1, 2, 3, 4};
            uint16 matrixValue[3][4] = { {1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12} };
            uint16 matrix3DValue[2][3][4] = { { {1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12} }, { {13, 14, 15, 16}, {17, 18, 19, 20}, {21, 22, 23, 24} } };
            scalarData   = new MDSplus::Uint16(scalarValue);
            vectorData   = new MDSplus::Uint16Array(&vectorValue[0], vectorDims);
            matrixData   = new MDSplus::Uint16Array(&matrixValue[0][0], 2, &matrixDims[0]);
            matrix3DData = new MDSplus::Uint16Array(&matrix3DValue[0][0][0], 3, &matrix3DDims[0]);
            typeSuffix = "UINT16";
        } else if (typeIn == UnsignedInteger32Bit) {
            uint32 scalarValue = 1;
            uint32 vectorValue[4] = {1, 2, 3, 4};
            uint32 matrixValue[3][4] = { {1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12} };
            uint32 matrix3DValue[2][3][4] = { { {1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12} }, { {13, 14, 15, 16}, {17, 18, 19, 20}, {21, 22, 23, 24} } };
            scalarData   = new MDSplus::Uint32(scalarValue);
            vectorData   = new MDSplus::Uint32Array(&vectorValue[0], vectorDims);
            matrixData   = new MDSplus::Uint32Array(&matrixValue[0][0], 2, &matrixDims[0]);
            matrix3DData = new MDSplus::Uint32Array(&matrix3DValue[0][0][0], 3, &matrix3DDims[0]);
            typeSuffix = "UINT32";
        } else if (typeIn == UnsignedInteger64Bit) {
            uint64 scalarValue = 1;
            uint64 vectorValue[4] = {1, 2, 3, 4};
            uint64 matrixValue[3][4] = { {1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12} };
            uint64 matrix3DValue[2][3][4] = { { {1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12} }, { {13, 14, 15, 16}, {17, 18, 19, 20}, {21, 22, 23, 24} } };
            scalarData   = new MDSplus::Uint64(scalarValue);
            vectorData   = new MDSplus::Uint64Array((uint64_t*)&vectorValue[0], vectorDims);
            matrixData   = new MDSplus::Uint64Array((uint64_t*)&matrixValue[0][0], 2, &matrixDims[0]);
            matrix3DData = new MDSplus::Uint64Array((uint64_t*)&matrix3DValue[0][0][0], 3, &matrix3DDims[0]);
            typeSuffix = "UINT64";
        } else if (typeIn == SignedInteger8Bit) {
            int8 scalarValue = 1;
            int8 vectorValue[4] = {1, 2, 3, 4};
            int8 matrixValue[3][4] = { {1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12} };
            int8 matrix3DValue[2][3][4] = { { {1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12} }, { {13, 14, 15, 16}, {17, 18, 19, 20}, {21, 22, 23, 24} } };
            scalarData   = new MDSplus::Int8(scalarValue);
            vectorData   = new MDSplus::Int8Array((char*)&vectorValue[0], vectorDims);
            matrixData   = new MDSplus::Int8Array((char*)&matrixValue[0][0], 2, &matrixDims[0]);
            matrix3DData = new MDSplus::Int8Array((char*)&matrix3DValue[0][0][0], 3, &matrix3DDims[0]);
            typeSuffix = "INT8";
        } else if (typeIn == SignedInteger16Bit) {
            int16 scalarValue = 1;
            int16 vectorValue[4] = {1, 2, 3, 4};
            int16 matrixValue[3][4] = { {1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12} };
            int16 matrix3DValue[2][3][4] = { { {1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12} }, { {13, 14, 15, 16}, {17, 18, 19, 20}, {21, 22, 23, 24} } };
            scalarData   = new MDSplus::Int16(scalarValue);
            vectorData   = new MDSplus::Int16Array(&vectorValue[0], vectorDims);
            matrixData   = new MDSplus::Int16Array(&matrixValue[0][0], 2, &matrixDims[0]);
            matrix3DData = new MDSplus::Int16Array(&matrix3DValue[0][0][0], 3, &matrix3DDims[0]);
            typeSuffix = "INT16";
        } else if (typeIn == SignedInteger32Bit) {
            int32 scalarValue = 1;
            int32 vectorValue[4] = {1, 2, 3, 4};
            int32 matrixValue[3][4] = { {1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12} };
            int32 matrix3DValue[2][3][4] = { { {1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12} }, { {13, 14, 15, 16}, {17, 18, 19, 20}, {21, 22, 23, 24} } };
            scalarData   = new MDSplus::Int32(scalarValue);
            vectorData   = new MDSplus::Int32Array(&vectorValue[0], vectorDims);
            matrixData   = new MDSplus::Int32Array(&matrixValue[0][0], 2, &matrixDims[0]);
            matrix3DData = new MDSplus::Int32Array(&matrix3DValue[0][0][0], 3, &matrix3DDims[0]);
            typeSuffix = "INT32";
        } else if (typeIn == SignedInteger64Bit) {
            int64 scalarValue = 1;
            int64 vectorValue[4] = {1, 2, 3, 4};
            int64 matrixValue[3][4] = { {1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12} };
            int64 matrix3DValue[2][3][4] = { { {1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12} }, { {13, 14, 15, 16}, {17, 18, 19, 20}, {21, 22, 23, 24} } };
            scalarData   = new MDSplus::Int64(scalarValue);
            vectorData   = new MDSplus::Int64Array((int64_t*)&vectorValue[0], vectorDims);
            matrixData   = new MDSplus::Int64Array((int64_t*)&matrixValue[0][0], 2, &matrixDims[0]);
            matrix3DData = new MDSplus::Int64Array((int64_t*)&matrix3DValue[0][0][0], 3, &matrix3DDims[0]);
            typeSuffix = "INT64";
        } else if (typeIn == Float32Bit) {
            float32 scalarValue = 1;
            float32 vectorValue[4] = {1, 2, 3, 4};
            float32 matrixValue[3][4] = { {1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12} };
            float32 matrix3DValue[2][3][4] = { { {1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12} }, { {13, 14, 15, 16}, {17, 18, 19, 20}, {21, 22, 23, 24} } };
            scalarData   = new MDSplus::Float32(scalarValue);
            vectorData   = new MDSplus::Float32Array(&vectorValue[0], vectorDims);
            matrixData   = new MDSplus::Float32Array(&matrixValue[0][0], 2, &matrixDims[0]);
            matrix3DData = new MDSplus::Float32Array(&matrix3DValue[0][0][0], 3, &matrix3DDims[0]);
            typeSuffix = "FLOAT32";
        } else if (typeIn == Float64Bit) {
            float64 scalarValue = 1;
            float64 vectorValue[4] = {1, 2, 3, 4};
            float64 matrixValue[3][4] = { {1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12} };
            float64 matrix3DValue[2][3][4] = { { {1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12} }, { {13, 14, 15, 16}, {17, 18, 19, 20}, {21, 22, 23, 24} } };
            scalarData   = new MDSplus::Float64(scalarValue);
            vectorData   = new MDSplus::Float64Array(&vectorValue[0], vectorDims);
            matrixData   = new MDSplus::Float64Array(&matrixValue[0][0], 2, &matrixDims[0]);
            matrix3DData = new MDSplus::Float64Array(&matrix3DValue[0][0][0], 3, &matrix3DDims[0]);
            typeSuffix = "FLOAT64";
        }
        ok = true;
    } catch (const MDSplus::MdsException &exc) {
        REPORT_ERROR_STATIC(ErrorManagement::Exception, "Failed ::Data.");
        REPORT_ERROR_STATIC(ErrorManagement::Exception, "MDSplus exception: %s", exc.what());
        ok = false;
    }

    if (ok) {
        try {
            scalarNodeName += typeSuffix;
            vectorNodeName   += typeSuffix;
            matrixNodeName   += typeSuffix;
            matrix3DNodeName += typeSuffix;
            MDSplus::TreeNode* scalarNode   = testTree->getNode(scalarNodeName.Buffer());
            MDSplus::TreeNode* vectorNode   = testTree->getNode(vectorNodeName.Buffer());
            MDSplus::TreeNode* matrixNode   = testTree->getNode(matrixNodeName.Buffer());
            MDSplus::TreeNode* matrix3DNode = testTree->getNode(matrix3DNodeName.Buffer());
            scalarNode->putData(scalarData);
            vectorNode->putData(vectorData);
            matrixNode->putData(matrixData);
            matrix3DNode->putData(matrix3DData);
            ok = true;
        } catch (const MDSplus::MdsException &exc) {
            REPORT_ERROR_STATIC(ErrorManagement::Exception, "Failed putData.");
            REPORT_ERROR_STATIC(ErrorManagement::Exception, "MDSplus exception: %s", exc.what());
            ok = false;
        }
    }

    return ok;
}

void MDSObjectConnectionTestEnvironment::SetupTestEnvironment() {

    bool ok = false;

    treeName = "mdsoc_ttree";
    StreamString wd       = getenv("PWD");
    StreamString treeEnv  = treeName;
    treeEnv += "_path";
    setenv(treeEnv.Buffer(), wd.Buffer(), true);

    // clean-up
    DeleteTestEnvironment();

    try {
        testTree = new MDSplus::Tree(treeName.Buffer(), -1, "NEW");
        testTree->write();
        ok = true;
    } catch (const MDSplus::MdsException &exc) {
        REPORT_ERROR_STATIC(ErrorManagement::Exception, "Failed creating tree.");
        REPORT_ERROR_STATIC(ErrorManagement::Exception, "MDSplus exception: %s", exc.what());
        ok = false;
    }

    if (ok) {
        try {
            testTree->addNode("SCALUINT8",    "NUMERIC");
            testTree->addNode("SCALUINT16",   "NUMERIC");
            testTree->addNode("SCALUINT32",   "NUMERIC");
            testTree->addNode("SCALUINT64",   "NUMERIC");
            testTree->addNode("SCALINT8",     "NUMERIC");
            testTree->addNode("SCALINT16",    "NUMERIC");
            testTree->addNode("SCALINT32",    "NUMERIC");
            testTree->addNode("SCALINT64",    "NUMERIC");
            testTree->addNode("SCALFLOAT32",  "NUMERIC");
            testTree->addNode("SCALFLOAT64",  "NUMERIC");

            testTree->addNode("VECUINT8",     "NUMERIC");
            testTree->addNode("VECUINT16",    "NUMERIC");
            testTree->addNode("VECUINT32",    "NUMERIC");
            testTree->addNode("VECUINT64",    "NUMERIC");
            testTree->addNode("VECINT8",      "NUMERIC");
            testTree->addNode("VECINT16",     "NUMERIC");
            testTree->addNode("VECINT32",     "NUMERIC");
            testTree->addNode("VECINT64",     "NUMERIC");
            testTree->addNode("VECFLOAT32",   "NUMERIC");
            testTree->addNode("VECFLOAT64",   "NUMERIC");

            testTree->addNode("MATUINT8",     "NUMERIC");
            testTree->addNode("MATUINT16",    "NUMERIC");
            testTree->addNode("MATUINT32",    "NUMERIC");
            testTree->addNode("MATUINT64",    "NUMERIC");
            testTree->addNode("MATINT8",      "NUMERIC");
            testTree->addNode("MATINT16",     "NUMERIC");
            testTree->addNode("MATINT32",     "NUMERIC");
            testTree->addNode("MATINT64",     "NUMERIC");
            testTree->addNode("MATFLOAT32",   "NUMERIC");
            testTree->addNode("MATFLOAT64",   "NUMERIC");

            testTree->addNode("MAT3DUINT8",   "NUMERIC");
            testTree->addNode("MAT3DUINT16",  "NUMERIC");
            testTree->addNode("MAT3DUINT32",  "NUMERIC");
            testTree->addNode("MAT3DUINT64",  "NUMERIC");
            testTree->addNode("MAT3DINT8",    "NUMERIC");
            testTree->addNode("MAT3DINT16",   "NUMERIC");
            testTree->addNode("MAT3DINT32",   "NUMERIC");
            testTree->addNode("MAT3DINT64",   "NUMERIC");
            testTree->addNode("MAT3DFLOAT32", "NUMERIC");
            testTree->addNode("MAT3DFLOAT64", "NUMERIC");

            testTree->write();
            ok = true;
        } catch (const MDSplus::MdsException &exc) {
            ok = false;
            REPORT_ERROR_STATIC(ErrorManagement::Exception, "Failed adding nodes.");
            REPORT_ERROR_STATIC(ErrorManagement::Exception, "MDSplus exception: %s", exc.what());
        }
    }

    if (ok) {
        ok &= AddNodeValues(UnsignedInteger8Bit);
        ok &= AddNodeValues(UnsignedInteger16Bit);
        ok &= AddNodeValues(UnsignedInteger32Bit);
        ok &= AddNodeValues(UnsignedInteger64Bit);
        ok &= AddNodeValues(SignedInteger8Bit);
        ok &= AddNodeValues(SignedInteger16Bit);
        ok &= AddNodeValues(SignedInteger32Bit);
        ok &= AddNodeValues(SignedInteger64Bit);
        ok &= AddNodeValues(Float32Bit);
        ok &= AddNodeValues(Float64Bit);
        if (!ok) {
            REPORT_ERROR_STATIC(ErrorManagement::Debug, "Failed AddNodeValues");
        }
    }

    printf("--started test environment in %s\n", getenv(treeEnv.Buffer()));
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

bool MDSObjectConnectionTest::TestConstructor() {

    MDSObjectConnection loader;

    return loader.GetStatus().ErrorsCleared();
}

bool MDSObjectConnectionTest::TestInitialise() {

    StreamString configStream = ""
        "Class  = MDSObjectConnection         \n"
        "Tree   = mdsoc_ttree                 \n"
        "Server = localhost:8002              \n"
        "Shot   = -1                          \n"
        "Parameters = {                       \n"
        "    par1 = { Path = \"SCALUINT32\" } \n"
        "}                                    \n"
        ""
        ;

    ErrorManagement::ErrorType status = ErrorManagement::FatalError;
    bool ok = TestInitialiseWithConfiguration(configStream, status);

    return (status.ErrorsCleared() && ok);
}

