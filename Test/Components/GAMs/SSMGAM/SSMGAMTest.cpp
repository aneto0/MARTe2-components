/**
 * @file SSMGAMTest.cpp
 * @brief Source file for class SSMGAMTest
 * @date 01/09/2017
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
 * the class SSMGAMTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include "stdio.h"
/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "SSMGAMTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace {

}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {
class SSMGAMTestHelper: public SSMGAM {
public:
    CLASS_REGISTER_DECLARATION()

SSMGAMTestHelper    () {

    }
    virtual ~SSMGAMTestHelper() {

    }
    bool InitialiseConfig1() {
        bool ok = true;
        float64 stateMatrix[2][2];
        stateMatrix[0][0] = 11;
        stateMatrix[0][1] = 12;
        stateMatrix[1][0] = 21;
        stateMatrix[1][1] = 22;
        Matrix<float64> matrix(&stateMatrix[0][0], 2u, 2u);
        ok &= config.Write("StateMatrix", matrix);

        float64 inputMatrix[2][1];
        inputMatrix[0][0] = 11;
        inputMatrix[1][0] = 21;
        Matrix<float64> matrix2(&inputMatrix[0][0], 2u, 1u);
        ok &= config.Write("InputMatrix", matrix2);

        float64 outputMatrix[1][2];
        outputMatrix[0][0] = 311;
        outputMatrix[0][1] = 312;
        Matrix<float64> matrix3(&outputMatrix[0][0], 1u, 2u);
        ok &= config.Write("OutputMatrix", matrix3);

        float64 feedthoughMatrix[1][1];
        feedthoughMatrix[0][0] = 411;
        Matrix<float64> matrix4(&feedthoughMatrix[0][0], 1u, 1u);
        ok &= config.Write("FeedthroughMatrix", matrix4);
        ok &= config.Write("ResetInEachState", 1);
        return ok;

    }
    bool InitialiseConfigSpring(uint32 resetEachStateConfig = 1u) {
        bool ok = true;
        float64 stateMatrix[2][2];
        stateMatrix[0][0] = 0.659700153391702;
        stateMatrix[0][1] = 0.533507195114693;
        stateMatrix[1][0] = -0.533507195114693;
        stateMatrix[1][1] = 0.126192958277009;
        Matrix<float64> matrix(&stateMatrix[0][0], 2u, 2u);
        ok &= config.Write("StateMatrix", matrix);

        float64 inputMatrix[2][1];
        inputMatrix[0][0] = 0.340299846608298;
        inputMatrix[1][0] = 0.533507195114693;
        Matrix<float64> matrix2(&inputMatrix[0][0], 2u, 1u);
        ok &= config.Write("InputMatrix", matrix2);

        float64 outputMatrix[1][2];
        outputMatrix[0][0] = 1;
        outputMatrix[0][1] = 0;
        Matrix<float64> matrix3(&outputMatrix[0][0], 1u, 2u);
        ok &= config.Write("OutputMatrix", matrix3);

        float64 feedthoughMatrix[1][1];
        feedthoughMatrix[0][0] = 0;
        Matrix<float64> matrix4(&feedthoughMatrix[0][0], 1u, 1u);
        ok &= config.Write("FeedthroughMatrix", matrix4);
        ok &= config.Write("ResetInEachState", resetEachStateConfig);
        return ok;
    }

    void *GetInputSignalsMemory() {
        return MARTe::GAM::GetInputSignalsMemory();
    }
    void *GetOutputSignalsMemory() {
        return MARTe::GAM::GetOutputSignalsMemory();
    }
    void *GetInputSignalsMemory(MARTe::uint32 idx) {
        return MARTe::GAM::GetInputSignalMemory(idx);
    }

    void *GetOutputSignalsMemory(MARTe::uint32 idx) {
        return MARTe::GAM::GetOutputSignalMemory(idx);
    }
    bool InitialiseConfig2() {
        bool ok = true;
        float64 stateMatrix[4][4];
        stateMatrix[0][0] = 11;
        stateMatrix[0][2] = 12;
        stateMatrix[0][3] = 13;
        stateMatrix[0][1] = 14;
        stateMatrix[1][0] = 21;
        stateMatrix[1][1] = 22;
        stateMatrix[1][2] = 23;
        stateMatrix[1][3] = 24;
        stateMatrix[2][0] = 31;
        stateMatrix[2][1] = 32;
        stateMatrix[2][2] = 33;
        stateMatrix[2][3] = 34;
        stateMatrix[3][0] = 41;
        stateMatrix[3][1] = 42;
        stateMatrix[3][2] = 43;
        stateMatrix[3][3] = 44;
        Matrix<float64> matrix(&stateMatrix[0][0], 4u, 4u);
        ok &= config.Write("StateMatrix", matrix);

        float64 inputMatrix[4][2];
        inputMatrix[0][0] = 211;
        inputMatrix[0][1] = 212;
        inputMatrix[1][0] = 221;
        inputMatrix[1][1] = 222;
        inputMatrix[2][0] = 231;
        inputMatrix[2][1] = 232;
        inputMatrix[3][0] = 241;
        inputMatrix[3][1] = 242;
        Matrix<float64> matrix2(&inputMatrix[0][0], 4u, 2u);
        ok &= config.Write("InputMatrix", matrix2);

        float64 outputMatrix[1][4];
        outputMatrix[0][0] = 311;
        outputMatrix[0][1] = 312;
        outputMatrix[0][2] = 313;
        outputMatrix[0][3] = 314;

        Matrix<float64> matrix3(&outputMatrix[0][0], 1u, 4u);
        ok &= config.Write("OutputMatrix", matrix3);

        float64 feedthoughMatrix[4][2];
        feedthoughMatrix[0][0] = 411;
        feedthoughMatrix[0][1] = 412;
        feedthoughMatrix[1][0] = 421;
        feedthoughMatrix[1][1] = 422;
        feedthoughMatrix[2][0] = 431;
        feedthoughMatrix[2][1] = 432;
        feedthoughMatrix[3][0] = 441;
        feedthoughMatrix[3][1] = 442;
        Matrix<float64> matrix4(&feedthoughMatrix[0][0], 4u, 2u);
        ok &= config.Write("FeedthroughMatrix", matrix4);
        ok &= config.Write("ResetInEachState", 1);

        return ok;

    }

    bool InitialiseConfigSpringNoD() {
        bool ok = true;
        float64 stateMatrix[2][2];
        stateMatrix[0][0] = 0.659700153391702;
        stateMatrix[0][1] = 0.533507195114693;
        stateMatrix[1][0] = -0.533507195114693;
        stateMatrix[1][1] = 0.126192958277009;
        Matrix<float64> matrix(&stateMatrix[0][0], 2u, 2u);
        ok &= config.Write("StateMatrix", matrix);

        float64 inputMatrix[2][1];
        inputMatrix[0][0] = 0.340299846608298;
        inputMatrix[1][0] = 0.533507195114693;
        Matrix<float64> matrix2(&inputMatrix[0][0], 2u, 1u);
        ok &= config.Write("InputMatrix", matrix2);

        float64 outputMatrix[1][2];
        outputMatrix[0][0] = 1;
        outputMatrix[0][1] = 0;
        Matrix<float64> matrix3(&outputMatrix[0][0], 1u, 2u);
        ok &= config.Write("OutputMatrix", matrix3);
        ok &= config.Write("ResetInEachState", 0);
        return ok;

    }
    bool InitialiseConfigSignals1() {
        bool ok = true;
        ok &= configSignals.CreateAbsolute("Signals.InputSignals");
        ok &= configSignals.CreateRelative("0");
        ok &= configSignals.Write("Type", "float64");
        ok &= configSignals.Write("NumberOfElements", 1);
        ok &= configSignals.Write("NumberOfDimensions", 1);
        ok &= configSignals.Write("DataSource", "TestDataSource");
        ok &= configSignals.Write("ByteSize", 8);
        ok &= configSignals.MoveToAncestor(1u);
        ok &= configSignals.Write("ByteSize", 8);

        ok &= configSignals.CreateAbsolute("Signals.OutputSignals");
        ok &= configSignals.CreateRelative("0");
        ok &= configSignals.Write("QualifiedName", "outputVector1");
        ok &= configSignals.Write("Type", "float64");
        ok &= configSignals.Write("NumberOfElements", 1);
        ok &= configSignals.Write("NumberOfDimensions", 0);
        ok &= configSignals.Write("ByteSize", 8);
        ok &= configSignals.Write("DataSource", "DataSourceOutputVector");
        ok &= configSignals.MoveToAncestor(1u);

        ok &= configSignals.CreateRelative("1");
        ok &= configSignals.Write("QualifiedName", "stateVector1");
        ok &= configSignals.Write("Type", "float64");
        ok &= configSignals.Write("NumberOfElements", 1);
        ok &= configSignals.Write("NumberOfDimensions", 0);
        ok &= configSignals.Write("DataSource", "DataSourceStateVector");
        ok &= configSignals.Write("ByteSize", 8);
        ok &= configSignals.MoveToAncestor(1u);

        ok &= configSignals.CreateRelative("2");
        ok &= configSignals.Write("QualifiedName", "stateVector2");
        ok &= configSignals.Write("Type", "float64");
        ok &= configSignals.Write("NumberOfElements", 1);
        ok &= configSignals.Write("NumberOfDimensions", 1);
        ok &= configSignals.Write("DataSource", "DataSourceStateVector");
        ok &= configSignals.Write("ByteSize", 8);
        ok &= configSignals.MoveToAncestor(1u);

        ok &= configSignals.Write("ByteSize", 24);

        ok &= configSignals.CreateAbsolute("Memory.InputSignals");
        ok &= configSignals.CreateRelative("0");
        ok &= configSignals.Write("DataSource", "TestDataSource");
        ok &= configSignals.CreateRelative("Signals");
        ok &= configSignals.CreateRelative("0");
        ok &= configSignals.Write("Samples", 1);

        ok &= configSignals.CreateAbsolute("Memory.OutputSignals");
        ok &= configSignals.CreateRelative("0");
        ok &= configSignals.Write("DataSource", "DataSourceOutputVector");
        ok &= configSignals.CreateRelative("Signals");
        ok &= configSignals.CreateRelative("0");
        ok &= configSignals.Write("Samples", 1);

        ok &= configSignals.MoveToAncestor(3u);
        ok &= configSignals.CreateRelative("1");
        ok &= configSignals.Write("DataSource", "DataSourceStateVector");
        ok &= configSignals.CreateRelative("Signals");
        ok &= configSignals.CreateRelative("1");
        ok &= configSignals.Write("Samples", 1);
        ok &= configSignals.MoveToAncestor(1u);
        ok &= configSignals.CreateRelative("2");
        ok &= configSignals.Write("Samples", 1);

        ok &= configSignals.MoveToRoot();
        return ok;
    }
    bool InitialiseConfigSignalsSpring() {
        bool ok = true;
        ok &= configSignals.CreateAbsolute("Signals.InputSignals");
        ok &= configSignals.CreateRelative("0");
        ok &= configSignals.Write("Type", "float64");
        ok &= configSignals.Write("NumberOfElements", 1);
        ok &= configSignals.Write("NumberOfDimensions", 1);
        ok &= configSignals.Write("DataSource", "TestDataSource");
        ok &= configSignals.Write("ByteSize", 8);
        ok &= configSignals.MoveToAncestor(1u);
        ok &= configSignals.Write("ByteSize", 8);

        ok &= configSignals.CreateAbsolute("Signals.OutputSignals");
        ok &= configSignals.CreateRelative("0");
        ok &= configSignals.Write("QualifiedName", "outputVector1");
        ok &= configSignals.Write("Type", "float64");
        ok &= configSignals.Write("NumberOfElements", 1);
        ok &= configSignals.Write("NumberOfDimensions", 1);
        ok &= configSignals.Write("ByteSize", 8);
        ok &= configSignals.Write("DataSource", "DataSourceOutputVector");

        ok &= configSignals.MoveToAncestor(1u);
        ok &= configSignals.CreateRelative("1");
        ok &= configSignals.Write("QualifiedName", "stateVector1");
        ok &= configSignals.Write("Type", "float64");
        ok &= configSignals.Write("NumberOfElements", 1);
        ok &= configSignals.Write("NumberOfDimensions", 1);
        ok &= configSignals.Write("DataSource", "DataSourceStateVector");
        ok &= configSignals.Write("ByteSize", 8);

        ok &= configSignals.MoveToAncestor(1u);
        ok &= configSignals.CreateRelative("2");
        ok &= configSignals.Write("QualifiedName", "stateVector2");
        ok &= configSignals.Write("Type", "float64");
        ok &= configSignals.Write("NumberOfElements", 1);
        ok &= configSignals.Write("NumberOfDimensions", 1);
        ok &= configSignals.Write("DataSource", "DataSourceStateVector");
        ok &= configSignals.Write("ByteSize", 8);

        ok &= configSignals.MoveToAncestor(1u);
        ok &= configSignals.Write("ByteSize", 24);

        ok &= configSignals.CreateAbsolute("Memory.InputSignals");
        ok &= configSignals.CreateRelative("0");
        ok &= configSignals.Write("DataSource", "TestDataSource");
        ok &= configSignals.CreateRelative("Signals");
        ok &= configSignals.CreateRelative("0");
        ok &= configSignals.Write("Samples", 1);

        ok &= configSignals.CreateAbsolute("Memory.OutputSignals");
        ok &= configSignals.CreateRelative("0");
        ok &= configSignals.Write("DataSource", "DataSourceOutputVector");
        ok &= configSignals.CreateRelative("Signals");
        ok &= configSignals.CreateRelative("0");
        ok &= configSignals.Write("Samples", 1);

        ok &= configSignals.MoveToAncestor(3u);
        ok &= configSignals.CreateRelative("1");
        ok &= configSignals.Write("DataSource", "DataSourceStateVector");
        ok &= configSignals.CreateRelative("Signals");
        ok &= configSignals.CreateRelative("1");
        ok &= configSignals.Write("Samples", 1);
        ok &= configSignals.MoveToAncestor(1u);
        ok &= configSignals.CreateRelative("2");
        ok &= configSignals.Write("Samples", 1);

        ok &= configSignals.MoveToRoot();
        return ok;
    }
    bool IsEqualLargerMargins(const float64 f1, const float64 f2) {
        float64 *min=reinterpret_cast<float64*>(const_cast<uint64*>(&EPSILON_FLOAT64));
        float64 minLarger = 2 * *min;
        return ((f1-f2)<(minLarger)) && ((f1-f2)>-(minLarger));
    }
    ConfigurationDatabase config;
    ConfigurationDatabase configSignals;
};

CLASS_REGISTER(SSMGAMTestHelper, "1.0")

SSMGAMTest::SSMGAMTest() {
//Auto-generated constructor stub for SSMGAMTest

//TODO Verify if manual additions are needed here
}

SSMGAMTest::~SSMGAMTest() {
//Auto-generated destructor stub for SSMGAMTest

//TODO Verify if manual additions are needed here
}

bool SSMGAMTest::TestInitialiseNoStateMatrix() {
    bool ok = true;
    SSMGAM gam;
    ConfigurationDatabase config;
    ok &= !gam.Initialise(config);
    return ok;
}

bool SSMGAMTest::TestInitialiseStateMatrix1x2() {
    bool ok;
    SSMGAM gam;
    ConfigurationDatabase config;
    float64 stateMatrix[1][2];
    stateMatrix[0][0] = 11;
    stateMatrix[0][1] = 12;
    Matrix<float64> matrix(&stateMatrix[0][0], 1u, 2u);
    ok = config.Write("StateMatrix", matrix);
    /*
     //example
     StreamString a;
     printf("%d\n", a.Printf("%!", config));
     printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
     printf("%s\n", a.Buffer());
     printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
     */
    if (ok) {
        ok &= !gam.Initialise(config);
    }
    return ok;
}

bool SSMGAMTest::TestInitialiseNoInputMatrix() {
    bool ok;
    SSMGAM gam;
    ConfigurationDatabase config;
    float64 stateMatrix[2][2];
    stateMatrix[0][0] = 11;
    stateMatrix[0][1] = 12;
    stateMatrix[1][0] = 21;
    stateMatrix[1][1] = 22;
    Matrix<float64> matrix(&stateMatrix[0][0], 2u, 2u);
    ok = config.Write("StateMatrix", matrix);
    if (ok) {
        ok &= !gam.Initialise(config);
    }
    return ok;
}

bool SSMGAMTest::TestInitialiseWrongInputMatrixDimension() {
    bool ok;
    SSMGAM gam;
    ConfigurationDatabase config;
    float64 stateMatrix[2][2];
    stateMatrix[0][0] = 11;
    stateMatrix[0][1] = 12;
    stateMatrix[1][0] = 21;
    stateMatrix[1][1] = 22;
    Matrix<float64> matrix(&stateMatrix[0][0], 2u, 2u);
    ok = config.Write("StateMatrix", matrix);

    float64 inputMatrix[1][2];
    inputMatrix[0][0] = 11;
    inputMatrix[0][1] = 12;
    Matrix<float64> matrix2(&inputMatrix[0][0], 1u, 2u);
    ok &= config.Write("InputMatrix", matrix2);
    if (ok) {
        ok &= !gam.Initialise(config);
    }
    return ok;
}

bool SSMGAMTest::TestInitialiseNoOutputMatrix() {
    bool ok;
    SSMGAM gam;
    ConfigurationDatabase config;
    float64 stateMatrix[2][2];
    stateMatrix[0][0] = 11;
    stateMatrix[0][1] = 12;
    stateMatrix[1][0] = 21;
    stateMatrix[1][1] = 22;
    Matrix<float64> matrix(&stateMatrix[0][0], 2u, 2u);
    ok = config.Write("StateMatrix", matrix);

    float64 inputMatrix[2][1];
    inputMatrix[0][0] = 11;
    inputMatrix[1][0] = 21;
    Matrix<float64> matrix2(&inputMatrix[0][0], 2u, 1u);
    ok &= config.Write("InputMatrix", matrix2);
    if (ok) {
        ok &= !gam.Initialise(config);
    }
    return ok;
}

bool SSMGAMTest::TestInitialiseWrongOutputMatrixDimension() {
    bool ok;
    SSMGAM gam;
    ConfigurationDatabase config;
    float64 stateMatrix[2][2];
    stateMatrix[0][0] = 11;
    stateMatrix[0][1] = 12;
    stateMatrix[1][0] = 21;
    stateMatrix[1][1] = 22;
    Matrix<float64> matrix(&stateMatrix[0][0], 2u, 2u);
    ok = config.Write("StateMatrix", matrix);

    float64 inputMatrix[2][1];
    inputMatrix[0][0] = 11;
    inputMatrix[1][0] = 21;
    Matrix<float64> matrix2(&inputMatrix[0][0], 2u, 1u);
    ok &= config.Write("InputMatrix", matrix2);

    float64 outputMatrix[2][1];
    outputMatrix[0][0] = 11;
    outputMatrix[1][0] = 21;
    Matrix<float64> matrix3(&outputMatrix[0][0], 2u, 1u);
    ok &= config.Write("OutputMatrix", matrix3);
    if (ok) {
        ok &= !gam.Initialise(config);
    }
    return ok;
}

bool SSMGAMTest::TestInitialiseNoFeedthoughMatrix() {
    bool ok;
    SSMGAM gam;
    ConfigurationDatabase config;
    float64 stateMatrix[2][2];
    stateMatrix[0][0] = 11;
    stateMatrix[0][1] = 12;
    stateMatrix[1][0] = 21;
    stateMatrix[1][1] = 22;
    Matrix<float64> matrix(&stateMatrix[0][0], 2u, 2u);
    ok = config.Write("StateMatrix", matrix);

    float64 inputMatrix[2][1];
    inputMatrix[0][0] = 11;
    inputMatrix[1][0] = 21;
    Matrix<float64> matrix2(&inputMatrix[0][0], 2u, 1u);
    ok &= config.Write("InputMatrix", matrix2);

    float64 outputMatrix[1][2];
    outputMatrix[0][0] = 311;
    outputMatrix[0][1] = 312;
    Matrix<float64> matrix3(&outputMatrix[0][0], 1u, 2u);
    ok &= config.Write("OutputMatrix", matrix3);

    ok &= config.Write("ResetInEachState", 1);
    if (ok) {
        ok &= gam.Initialise(config);
    }
    return ok;
}

bool SSMGAMTest::TestInitialiseWrongFeedthroughMatrixDimension() {
    bool ok;
    SSMGAM gam;
    ConfigurationDatabase config;
    float64 stateMatrix[2][2];
    stateMatrix[0][0] = 11;
    stateMatrix[0][1] = 12;
    stateMatrix[1][0] = 21;
    stateMatrix[1][1] = 22;
    Matrix<float64> matrix(&stateMatrix[0][0], 2u, 2u);
    ok = config.Write("StateMatrix", matrix);

    float64 inputMatrix[2][1];
    inputMatrix[0][0] = 11;
    inputMatrix[1][0] = 21;
    Matrix<float64> matrix2(&inputMatrix[0][0], 2u, 1u);
    ok &= config.Write("InputMatrix", matrix2);

    float64 outputMatrix[1][2];
    outputMatrix[0][0] = 311;
    outputMatrix[0][1] = 312;
    Matrix<float64> matrix3(&outputMatrix[0][0], 1u, 2u);
    ok &= config.Write("OutputMatrix", matrix3);

    float64 feedthoughMatrix[2][1];
    feedthoughMatrix[0][0] = 411;
    feedthoughMatrix[1][0] = 421;
    Matrix<float64> matrix4(&feedthoughMatrix[0][0], 2u, 1u);
    ok &= config.Write("FeedthroughMatrix", matrix4);

    if (ok) {
        ok &= !gam.Initialise(config);
    }
    return ok;
}

bool SSMGAMTest::TestInitialiseWrongFeedthroughMatrixDimension_2() {
    bool ok;
    SSMGAM gam;
    ConfigurationDatabase config;
    float64 stateMatrix[2][2];
    stateMatrix[0][0] = 11;
    stateMatrix[0][1] = 12;
    stateMatrix[1][0] = 21;
    stateMatrix[1][1] = 22;
    Matrix<float64> matrix(&stateMatrix[0][0], 2u, 2u);
    ok = config.Write("StateMatrix", matrix);

    float64 inputMatrix[2][1];
    inputMatrix[0][0] = 11;
    inputMatrix[1][0] = 21;
    Matrix<float64> matrix2(&inputMatrix[0][0], 2u, 1u);
    ok &= config.Write("InputMatrix", matrix2);

    float64 outputMatrix[1][2];
    outputMatrix[0][0] = 311;
    outputMatrix[0][1] = 312;
    Matrix<float64> matrix3(&outputMatrix[0][0], 1u, 2u);
    ok &= config.Write("OutputMatrix", matrix3);

    float64 feedthoughMatrix[1][2];
    feedthoughMatrix[0][0] = 411;
    feedthoughMatrix[0][1] = 412;
    Matrix<float64> matrix4(&feedthoughMatrix[0][0], 1u, 2u);
    ok &= config.Write("FeedthroughMatrix", matrix4);

    if (ok) {
        ok &= !gam.Initialise(config);
    }
    return ok;
}

bool SSMGAMTest::TestInitialiseNoResetInEachState() {
    bool ok;
    SSMGAM gam;
    ConfigurationDatabase config;
    float64 stateMatrix[2][2];
    stateMatrix[0][0] = 11;
    stateMatrix[0][1] = 12;
    stateMatrix[1][0] = 21;
    stateMatrix[1][1] = 22;
    Matrix<float64> matrix(&stateMatrix[0][0], 2u, 2u);
    ok = config.Write("StateMatrix", matrix);

    float64 inputMatrix[2][1];
    inputMatrix[0][0] = 11;
    inputMatrix[1][0] = 21;
    Matrix<float64> matrix2(&inputMatrix[0][0], 2u, 1u);
    ok &= config.Write("InputMatrix", matrix2);

    float64 outputMatrix[1][2];
    outputMatrix[0][0] = 311;
    outputMatrix[0][1] = 312;
    Matrix<float64> matrix3(&outputMatrix[0][0], 1u, 2u);
    ok &= config.Write("OutputMatrix", matrix3);

    float64 feedthoughMatrix[1][2];
    feedthoughMatrix[0][0] = 411;
    feedthoughMatrix[0][1] = 412;
    Matrix<float64> matrix4(&feedthoughMatrix[0][0], 1u, 1u);
    ok &= config.Write("FeedthroughMatrix", matrix4);

    if (ok) {
        ok &= !gam.Initialise(config);
    }
    return ok;
}

bool SSMGAMTest::TestInitialiseWrongResetInEachState() {
    bool ok;
    SSMGAM gam;
    ConfigurationDatabase config;
    float64 stateMatrix[2][2];
    stateMatrix[0][0] = 11;
    stateMatrix[0][1] = 12;
    stateMatrix[1][0] = 21;
    stateMatrix[1][1] = 22;
    Matrix<float64> matrix(&stateMatrix[0][0], 2u, 2u);
    ok = config.Write("StateMatrix", matrix);

    float64 inputMatrix[2][1];
    inputMatrix[0][0] = 11;
    inputMatrix[1][0] = 21;
    Matrix<float64> matrix2(&inputMatrix[0][0], 2u, 1u);
    ok &= config.Write("InputMatrix", matrix2);

    float64 outputMatrix[1][2];
    outputMatrix[0][0] = 311;
    outputMatrix[0][1] = 312;
    Matrix<float64> matrix3(&outputMatrix[0][0], 1u, 2u);
    ok &= config.Write("OutputMatrix", matrix3);

    float64 feedthoughMatrix[1][2];
    feedthoughMatrix[0][0] = 411;
    feedthoughMatrix[0][1] = 412;
    Matrix<float64> matrix4(&feedthoughMatrix[0][0], 1u, 1u);
    ok &= config.Write("FeedthroughMatrix", matrix4);
    ok &= config.Write("ResetInEachState", 2);

    if (ok) {
        ok &= !gam.Initialise(config);
    }
    return ok;
}

bool SSMGAMTest::TestInitialise() {
    bool ok = true;
    SSMGAMTestHelper gam;
    ok &= gam.InitialiseConfig1();
    if (ok) {
        ok &= gam.Initialise(gam.config);
    }
    return ok;
}

bool SSMGAMTest::TestInitialise_2() {
    bool ok = true;
    SSMGAMTestHelper gam;
    ok &= gam.InitialiseConfig2();
    if (ok) {
        ok &= gam.Initialise(gam.config);
    }
    return ok;
}

bool SSMGAMTest::TestSetupWrongInputSignals() {
    bool ok = true;
    SSMGAMTestHelper gam;
    //n = 2, p = 1 and q = 1
    ok &= gam.InitialiseConfig1();
    if (ok) {
        ok &= gam.Initialise(gam.config);
    }
    ConfigurationDatabase configSignals;
    ok &= configSignals.CreateAbsolute("Signals.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 16);
    ok &= configSignals.MoveToRoot();
    ok &= gam.SetConfiguredDatabase(configSignals);
    if (ok) {
        ok &= !gam.Setup();
    }
    return ok;
}

bool SSMGAMTest::TestSetupWrongOutputsSignals() {
    bool ok = true;
    SSMGAMTestHelper gam;
    //n = 2, p = 1 and q = 1
    ok &= gam.InitialiseConfig1();
    if (ok) {
        ok &= gam.Initialise(gam.config);
    }
    ConfigurationDatabase configSignals;
    ok &= configSignals.CreateAbsolute("Signals.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 16);
    ok &= configSignals.CreateAbsolute("Signals.OutputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 16);
    ok &= configSignals.MoveToRoot();
    ok &= gam.SetConfiguredDatabase(configSignals);
    if (ok) {
        ok &= !gam.Setup();
    }
    return ok;
}

bool SSMGAMTest::TestSetupWrongInputType() {
    bool ok = true;
    SSMGAMTestHelper gam;
    //n = 2, p = 1 and q = 1
    ok &= gam.InitialiseConfig1();
    if (ok) {
        ok &= gam.Initialise(gam.config);
    }
    ConfigurationDatabase configSignals;
    ok &= configSignals.CreateAbsolute("Signals.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 16);
    ok &= configSignals.CreateAbsolute("Signals.OutputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("QualifiedName", "outputVector1");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("QualifiedName", "stateVector1");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("2");
    ok &= configSignals.Write("QualifiedName", "stateVector2");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 16);
    ok &= configSignals.MoveToRoot();
    ok &= gam.SetConfiguredDatabase(configSignals);
    if (ok) {
        ok &= !gam.Setup();
    }
    return ok;
}

bool SSMGAMTest::TestSetupWrongOutputType() {
    bool ok = true;
    SSMGAMTestHelper gam;
    //n = 2, p = 1 and q = 1
    ok &= gam.InitialiseConfig1();
    if (ok) {
        ok &= gam.Initialise(gam.config);
    }
    ConfigurationDatabase configSignals;
    ok &= configSignals.CreateAbsolute("Signals.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Type", "float64");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 16);
    ok &= configSignals.CreateAbsolute("Signals.OutputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("QualifiedName", "outputVector1");
    ok &= configSignals.Write("Type", "float64");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("QualifiedName", "stateVector1");
    ok &= configSignals.Write("Type", "float32");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("2");
    ok &= configSignals.Write("QualifiedName", "stateVector2");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 16);
    ok &= configSignals.MoveToRoot();
    ok &= gam.SetConfiguredDatabase(configSignals);
    if (ok) {
        ok &= !gam.Setup();
    }
    return ok;
}

bool SSMGAMTest::TestSetupNoInputElements() {
    bool ok = true;
    SSMGAMTestHelper gam;
    //n = 2, p = 1 and q = 1
    ok &= gam.InitialiseConfig1();
    if (ok) {
        ok &= gam.Initialise(gam.config);
    }
    ConfigurationDatabase configSignals;
    ok &= configSignals.CreateAbsolute("Signals.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Type", "float64");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 16);
    ok &= configSignals.CreateAbsolute("Signals.OutputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("QualifiedName", "outputVector1");
    ok &= configSignals.Write("Type", "float64");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("QualifiedName", "stateVector1");
    ok &= configSignals.Write("Type", "float64");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("2");
    ok &= configSignals.Write("QualifiedName", "stateVector2");
    ok &= configSignals.Write("Type", "float64");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 16);
    ok &= configSignals.MoveToRoot();
    ok &= gam.SetConfiguredDatabase(configSignals);
    if (ok) {
        ok &= !gam.Setup();
    }
    return ok;
}

bool SSMGAMTest::TestSetupWrongInputElements() {
    bool ok = true;
    SSMGAMTestHelper gam;
    //n = 2, p = 1 and q = 1
    ok &= gam.InitialiseConfig1();
    if (ok) {
        ok &= gam.Initialise(gam.config);
    }
    ConfigurationDatabase configSignals;
    ok &= configSignals.CreateAbsolute("Signals.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("NumberOfElements", 2);
    ok &= configSignals.Write("Type", "float64");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 16);
    ok &= configSignals.CreateAbsolute("Signals.OutputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("QualifiedName", "outputVector1");
    ok &= configSignals.Write("Type", "float64");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("QualifiedName", "stateVector1");
    ok &= configSignals.Write("Type", "float64");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("2");
    ok &= configSignals.Write("QualifiedName", "stateVector2");
    ok &= configSignals.Write("Type", "float64");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 16);
    ok &= configSignals.MoveToRoot();
    ok &= gam.SetConfiguredDatabase(configSignals);
    if (ok) {
        ok &= !gam.Setup();
    }
    return ok;
}

bool SSMGAMTest::TestSetupNoOutputElements() {
    bool ok = true;
    SSMGAMTestHelper gam;
    //n = 2, p = 1 and q = 1
    ok &= gam.InitialiseConfig1();
    if (ok) {
        ok &= gam.Initialise(gam.config);
    }
    ConfigurationDatabase configSignals;
    ok &= configSignals.CreateAbsolute("Signals.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("NumberOfElements", 1);
    ok &= configSignals.Write("Type", "float64");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 16);
    ok &= configSignals.CreateAbsolute("Signals.OutputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("QualifiedName", "outputVector1");
    ok &= configSignals.Write("Type", "float64");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("QualifiedName", "stateVector1");
    ok &= configSignals.Write("Type", "float64");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("2");
    ok &= configSignals.Write("QualifiedName", "stateVector2");
    ok &= configSignals.Write("Type", "float64");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 16);
    ok &= configSignals.MoveToRoot();
    ok &= gam.SetConfiguredDatabase(configSignals);
    if (ok) {
        ok &= !gam.Setup();
    }
    return ok;
}

bool SSMGAMTest::TestSetupWrongOutputElements() {
    bool ok = true;
    SSMGAMTestHelper gam;
    //n = 2, p = 1 and q = 1
    ok &= gam.InitialiseConfig1();
    if (ok) {
        ok &= gam.Initialise(gam.config);
    }
    ConfigurationDatabase configSignals;
    ok &= configSignals.CreateAbsolute("Signals.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("NumberOfElements", 1);
    ok &= configSignals.Write("Type", "float64");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 16);
    ok &= configSignals.CreateAbsolute("Signals.OutputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("QualifiedName", "outputVector1");
    ok &= configSignals.Write("Type", "float64");
    ok &= configSignals.Write("NumberOfElements", 1);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("QualifiedName", "stateVector1");
    ok &= configSignals.Write("Type", "float64");
    ok &= configSignals.Write("NumberOfElements", 1);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("2");
    ok &= configSignals.Write("QualifiedName", "stateVector2");
    ok &= configSignals.Write("Type", "float64");
    ok &= configSignals.Write("NumberOfElements", 3);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 16);
    ok &= configSignals.MoveToRoot();
    ok &= gam.SetConfiguredDatabase(configSignals);
    if (ok) {
        ok &= !gam.Setup();
    }
    return ok;
}

bool SSMGAMTest::TestSetupNoInputDimensions() {
    bool ok = true;
    SSMGAMTestHelper gam;
    //n = 2, p = 1 and q = 1
    ok &= gam.InitialiseConfig1();
    if (ok) {
        ok &= gam.Initialise(gam.config);
    }
    ConfigurationDatabase configSignals;
    ok &= configSignals.CreateAbsolute("Signals.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("NumberOfElements", 1);
    ok &= configSignals.Write("Type", "float64");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 8);
    ok &= configSignals.CreateAbsolute("Signals.OutputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("QualifiedName", "outputVector1");
    ok &= configSignals.Write("Type", "float64");
    ok &= configSignals.Write("NumberOfElements", 1);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("QualifiedName", "stateVector1");
    ok &= configSignals.Write("Type", "float64");
    ok &= configSignals.Write("NumberOfElements", 1);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("2");
    ok &= configSignals.Write("QualifiedName", "stateVector2");
    ok &= configSignals.Write("Type", "float64");
    ok &= configSignals.Write("NumberOfElements", 1);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 16);
    ok &= configSignals.MoveToRoot();
    ok &= gam.SetConfiguredDatabase(configSignals);
    if (ok) {
        ok &= !gam.Setup();
    }
    return ok;
}

bool SSMGAMTest::TestSetupWrongInputDimensions() {
    bool ok = true;
    SSMGAMTestHelper gam;
    //n = 2, p = 1 and q = 1
    ok &= gam.InitialiseConfig1();
    if (ok) {
        ok &= gam.Initialise(gam.config);
    }
    ConfigurationDatabase configSignals;
    ok &= configSignals.CreateAbsolute("Signals.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Type", "float64");
    ok &= configSignals.Write("NumberOfElements", 1);
    ok &= configSignals.Write("NumberOfDimensions", 2);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 16);
    ok &= configSignals.CreateAbsolute("Signals.OutputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("QualifiedName", "outputVector1");
    ok &= configSignals.Write("Type", "float64");
    ok &= configSignals.Write("NumberOfElements", 1);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("QualifiedName", "stateVector1");
    ok &= configSignals.Write("Type", "float64");
    ok &= configSignals.Write("NumberOfElements", 1);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("2");
    ok &= configSignals.Write("QualifiedName", "stateVector2");
    ok &= configSignals.Write("Type", "float64");
    ok &= configSignals.Write("NumberOfElements", 1);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 16);
    ok &= configSignals.MoveToRoot();
    ok &= gam.SetConfiguredDatabase(configSignals);
    if (ok) {
        ok &= !gam.Setup();
    }
    return ok;
}

bool SSMGAMTest::TestSetupNoOutputDimensions() {
    bool ok = true;
    SSMGAMTestHelper gam;
    //n = 2, p = 1 and q = 1
    ok &= gam.InitialiseConfig1();
    if (ok) {
        ok &= gam.Initialise(gam.config);
    }
    ConfigurationDatabase configSignals;
    ok &= configSignals.CreateAbsolute("Signals.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Type", "float64");
    ok &= configSignals.Write("NumberOfElements", 1);
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 16);
    ok &= configSignals.CreateAbsolute("Signals.OutputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("QualifiedName", "outputVector1");
    ok &= configSignals.Write("Type", "float64");
    ok &= configSignals.Write("NumberOfElements", 1);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("QualifiedName", "stateVector1");
    ok &= configSignals.Write("Type", "float64");
    ok &= configSignals.Write("NumberOfElements", 1);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("2");
    ok &= configSignals.Write("QualifiedName", "stateVector2");
    ok &= configSignals.Write("Type", "float64");
    ok &= configSignals.Write("NumberOfElements", 1);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 16);
    ok &= configSignals.MoveToRoot();
    ok &= gam.SetConfiguredDatabase(configSignals);
    if (ok) {
        ok &= !gam.Setup();
    }
    return ok;
}

bool SSMGAMTest::TestSetupWrongOutputDimensions() {
    bool ok = true;
    SSMGAMTestHelper gam;
    //n = 2, p = 1 and q = 1
    ok &= gam.InitialiseConfig1();
    if (ok) {
        ok &= gam.Initialise(gam.config);
    }
    ConfigurationDatabase configSignals;
    ok &= configSignals.CreateAbsolute("Signals.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Type", "float64");
    ok &= configSignals.Write("NumberOfElements", 1);
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 16);
    ok &= configSignals.CreateAbsolute("Signals.OutputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("QualifiedName", "outputVector1");
    ok &= configSignals.Write("Type", "float64");
    ok &= configSignals.Write("NumberOfElements", 1);
    ok &= configSignals.Write("NumberOfDimensions", 0);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("QualifiedName", "stateVector1");
    ok &= configSignals.Write("Type", "float64");
    ok &= configSignals.Write("NumberOfElements", 1);
    ok &= configSignals.Write("NumberOfDimensions", 0);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("2");
    ok &= configSignals.Write("QualifiedName", "stateVector2");
    ok &= configSignals.Write("Type", "float64");
    ok &= configSignals.Write("NumberOfElements", 1);
    ok &= configSignals.Write("NumberOfDimensions", 3);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 16);
    configSignals.MoveToRoot();
    ok &= gam.SetConfiguredDatabase(configSignals);
    if (ok) {
        ok &= !gam.Setup();
    }
    return ok;
}

bool SSMGAMTest::TestSetupNoInputSamples() {
    bool ok = true;
    SSMGAMTestHelper gam;
    //n = 2, p = 1 and q = 1
    ok &= gam.InitialiseConfig1();
    if (ok) {
        ok &= gam.Initialise(gam.config);
    }
    ConfigurationDatabase configSignals;
    ok &= configSignals.CreateAbsolute("Signals.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Type", "float64");
    ok &= configSignals.Write("NumberOfElements", 1);
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.Write("DataSource", "TestDataSource");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 16);
    ok &= configSignals.CreateAbsolute("Signals.OutputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("QualifiedName", "outputVector1");
    ok &= configSignals.Write("Type", "float64");
    ok &= configSignals.Write("NumberOfElements", 1);
    ok &= configSignals.Write("NumberOfDimensions", 0);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("QualifiedName", "stateVector1");
    ok &= configSignals.Write("Type", "float64");
    ok &= configSignals.Write("NumberOfElements", 1);
    ok &= configSignals.Write("NumberOfDimensions", 0);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("2");
    ok &= configSignals.Write("QualifiedName", "stateVector2");
    ok &= configSignals.Write("Type", "float64");
    ok &= configSignals.Write("NumberOfElements", 1);
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 16);

    ok &= configSignals.CreateAbsolute("Memory.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("DataSource", "TestDataSource");
    ok &= configSignals.CreateRelative("Signals");
    ok &= configSignals.CreateRelative("0");
    //configSignals.Write("Samples", numberOfSamples);

    configSignals.MoveToRoot();
    ok &= gam.SetConfiguredDatabase(configSignals);
    if (ok) {
        ok &= !gam.Setup();
    }
    return ok;
}

bool SSMGAMTest::TestSetupWrongInputSamples() {
    bool ok = true;
    SSMGAMTestHelper gam;
    //n = 2, p = 1 and q = 1
    ok &= gam.InitialiseConfig1();
    if (ok) {
        ok &= gam.Initialise(gam.config);
    }
    ConfigurationDatabase configSignals;
    ok &= configSignals.CreateAbsolute("Signals.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Type", "float64");
    ok &= configSignals.Write("NumberOfElements", 1);
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.Write("DataSource", "TestDataSource");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 16);
    ok &= configSignals.CreateAbsolute("Signals.OutputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("QualifiedName", "outputVector1");
    ok &= configSignals.Write("Type", "float64");
    ok &= configSignals.Write("NumberOfElements", 1);
    ok &= configSignals.Write("NumberOfDimensions", 0);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("QualifiedName", "stateVector1");
    ok &= configSignals.Write("Type", "float64");
    ok &= configSignals.Write("NumberOfElements", 1);
    ok &= configSignals.Write("NumberOfDimensions", 0);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("2");
    ok &= configSignals.Write("QualifiedName", "stateVector2");
    ok &= configSignals.Write("Type", "float64");
    ok &= configSignals.Write("NumberOfElements", 1);
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 16);

    ok &= configSignals.CreateAbsolute("Memory.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("DataSource", "TestDataSource");
    ok &= configSignals.CreateRelative("Signals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Samples", 2);

    configSignals.MoveToRoot();
    ok &= gam.SetConfiguredDatabase(configSignals);
    if (ok) {
        ok &= !gam.Setup();
    }
    return ok;
}

bool SSMGAMTest::TestSetupNoOutputSamples() {
    bool ok = true;
    SSMGAMTestHelper gam;
    //n = 2, p = 1 and q = 1
    ok &= gam.InitialiseConfig1();
    if (ok) {
        ok &= gam.Initialise(gam.config);
    }
    ConfigurationDatabase configSignals;
    ok &= configSignals.CreateAbsolute("Signals.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Type", "float64");
    ok &= configSignals.Write("NumberOfElements", 1);
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.Write("DataSource", "TestDataSource");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 16);
    ok &= configSignals.CreateAbsolute("Signals.OutputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("QualifiedName", "outputVector1");
    ok &= configSignals.Write("Type", "float64");
    ok &= configSignals.Write("NumberOfElements", 1);
    ok &= configSignals.Write("NumberOfDimensions", 0);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("QualifiedName", "stateVector1");
    ok &= configSignals.Write("Type", "float64");
    ok &= configSignals.Write("NumberOfElements", 1);
    ok &= configSignals.Write("NumberOfDimensions", 0);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("2");
    ok &= configSignals.Write("QualifiedName", "stateVector2");
    ok &= configSignals.Write("Type", "float64");
    ok &= configSignals.Write("NumberOfElements", 1);
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 16);

    ok &= configSignals.CreateAbsolute("Memory.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("DataSource", "TestDataSource");
    ok &= configSignals.CreateRelative("Signals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Samples", 1);

    configSignals.MoveToRoot();
    ok &= gam.SetConfiguredDatabase(configSignals);
    if (ok) {
        ok &= !gam.Setup();
    }
    return ok;
}

bool SSMGAMTest::TestSetupWrongOutputSamples() {
    bool ok = true;
    SSMGAMTestHelper gam;
    //n = 2, p = 1 and q = 1
    ok &= gam.InitialiseConfig1();
    if (ok) {
        ok &= gam.Initialise(gam.config);
    }
    ConfigurationDatabase configSignals;
    ok &= configSignals.CreateAbsolute("Signals.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Type", "float64");
    ok &= configSignals.Write("NumberOfElements", 1);
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.Write("DataSource", "TestDataSource");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 16);
    ok &= configSignals.CreateAbsolute("Signals.OutputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("QualifiedName", "outputVector1");
    ok &= configSignals.Write("Type", "float64");
    ok &= configSignals.Write("NumberOfElements", 1);
    ok &= configSignals.Write("NumberOfDimensions", 0);
    ok &= configSignals.Write("DataSource", "DataSourceOutputVector");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("QualifiedName", "stateVector1");
    ok &= configSignals.Write("Type", "float64");
    ok &= configSignals.Write("NumberOfElements", 1);
    ok &= configSignals.Write("NumberOfDimensions", 0);
    ok &= configSignals.Write("DataSource", "DataSourceStateVector");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("2");
    ok &= configSignals.Write("QualifiedName", "stateVector2");
    ok &= configSignals.Write("Type", "float64");
    ok &= configSignals.Write("NumberOfElements", 1);
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.Write("DataSource", "DataSourceStateVector");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 16);

    ok &= configSignals.CreateAbsolute("Memory.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("DataSource", "TestDataSource");
    ok &= configSignals.CreateRelative("Signals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Samples", 1);

    ok &= configSignals.CreateAbsolute("Memory.OutputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("DataSource", "outputVector1");
    ok &= configSignals.CreateRelative("Signals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Samples", 1);
    ok &= configSignals.MoveToAncestor(3u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("DataSource", "DataSourceOutputVector");
    ok &= configSignals.CreateRelative("Signals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Samples", 2);

    configSignals.MoveToRoot();
    ok &= gam.SetConfiguredDatabase(configSignals);
    if (ok) {
        ok &= !gam.Setup();
    }
    return ok;
}

bool SSMGAMTest::TestSetupWrongOutputSamples_2() {
    bool ok = true;
    SSMGAMTestHelper gam;
    //n = 2, p = 1 and q = 1
    ok &= gam.InitialiseConfig1();
    if (ok) {
        ok &= gam.Initialise(gam.config);
    }
    ConfigurationDatabase configSignals;
    ok &= configSignals.CreateAbsolute("Signals.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Type", "float64");
    ok &= configSignals.Write("NumberOfElements", 1);
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.Write("DataSource", "TestDataSource");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 16);
    ok &= configSignals.CreateAbsolute("Signals.OutputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("QualifiedName", "outputVector1");
    ok &= configSignals.Write("Type", "float64");
    ok &= configSignals.Write("NumberOfElements", 1);
    ok &= configSignals.Write("NumberOfDimensions", 0);
    ok &= configSignals.Write("DataSource", "DataSourceOutputVector");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("QualifiedName", "stateVector1");
    ok &= configSignals.Write("Type", "float64");
    ok &= configSignals.Write("NumberOfElements", 1);
    ok &= configSignals.Write("NumberOfDimensions", 0);
    ok &= configSignals.Write("DataSource", "DataSourceStateVector");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("2");
    ok &= configSignals.Write("QualifiedName", "stateVector2");
    ok &= configSignals.Write("Type", "float64");
    ok &= configSignals.Write("NumberOfElements", 1);
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.Write("DataSource", "DataSourceStateVector");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 16);

    ok &= configSignals.CreateAbsolute("Memory.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("DataSource", "TestDataSource");
    ok &= configSignals.CreateRelative("Signals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Samples", 1);

    ok &= configSignals.CreateAbsolute("Memory.OutputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("DataSource", "DataSourceOutputVector");
    ok &= configSignals.CreateRelative("Signals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Samples", 1);
    ok &= configSignals.MoveToAncestor(3u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("DataSource", "DataSourceStateVector");
    ok &= configSignals.CreateRelative("Signals");
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("Samples", 1);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("2");
    ok &= configSignals.Write("Samples", 2);

    ok &= configSignals.MoveToRoot();
    /*
     //example
     StreamString a;
     printf("%d\n", a.Printf("%!", configSignals));
     printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
     printf("%s\n", a.Buffer());
     printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
     */
    ok &= gam.SetConfiguredDatabase(configSignals);
    if (!ok) {
        printf("SetConfiguredDatabase failed\n");
    }
    if (ok) {
        ok &= !gam.Setup();
    }
    return ok;
}

bool SSMGAMTest::TestSetupWrongOutputSamples_3() {
    bool ok = true;
    SSMGAMTestHelper gam;
    //n = 2, p = 1 and q = 1
    ok &= gam.InitialiseConfig1();
    if (ok) {
        ok &= gam.Initialise(gam.config);
    }
    ConfigurationDatabase configSignals;
    ok &= configSignals.CreateAbsolute("Signals.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Type", "float64");
    ok &= configSignals.Write("NumberOfElements", 1);
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.Write("DataSource", "TestDataSource");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 16);
    ok &= configSignals.CreateAbsolute("Signals.OutputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("QualifiedName", "outputVector1");
    ok &= configSignals.Write("Type", "float64");
    ok &= configSignals.Write("NumberOfElements", 1);
    ok &= configSignals.Write("NumberOfDimensions", 0);
    ok &= configSignals.Write("DataSource", "DataSourceOutputVector");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("QualifiedName", "stateVector1");
    ok &= configSignals.Write("Type", "float64");
    ok &= configSignals.Write("NumberOfElements", 1);
    ok &= configSignals.Write("NumberOfDimensions", 0);
    ok &= configSignals.Write("DataSource", "DataSourceStateVector");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("2");
    ok &= configSignals.Write("QualifiedName", "stateVector2");
    ok &= configSignals.Write("Type", "float64");
    ok &= configSignals.Write("NumberOfElements", 1);
    ok &= configSignals.Write("NumberOfDimensions", 1);
    ok &= configSignals.Write("DataSource", "DataSourceStateVector");
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.Write("ByteSize", 16);

    ok &= configSignals.CreateAbsolute("Memory.InputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("DataSource", "TestDataSource");
    ok &= configSignals.CreateRelative("Signals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Samples", 1);

    ok &= configSignals.CreateAbsolute("Memory.OutputSignals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("DataSource", "DataSourceOutputVector");
    ok &= configSignals.CreateRelative("Signals");
    ok &= configSignals.CreateRelative("0");
    ok &= configSignals.Write("Samples", 1);
    ok &= configSignals.MoveToAncestor(3u);
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("DataSource", "DataSourceStateVector");
    ok &= configSignals.CreateRelative("Signals");
    ok &= configSignals.CreateRelative("1");
    ok &= configSignals.Write("Samples", 2);
    ok &= configSignals.MoveToAncestor(1u);
    ok &= configSignals.CreateRelative("2");
    ok &= configSignals.Write("Samples", 1);

    ok &= configSignals.MoveToRoot();
    /*
     //example
     StreamString a;
     printf("%d\n", a.Printf("%!", configSignals));
     printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
     printf("%s\n", a.Buffer());
     printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
     */
    ok &= gam.SetConfiguredDatabase(configSignals);
    if (!ok) {
        printf("SetConfiguredDatabase failed\n");
    }
    if (ok) {
        ok &= !gam.Setup();
    }
    return ok;
}

bool SSMGAMTest::TestSetupNoMemoryInitialised() {
    bool ok = true;
    SSMGAMTestHelper gam;
    //n = 2, p = 1 and q = 1
    ok &= gam.InitialiseConfig1();
    if (ok) {
        ok &= gam.Initialise(gam.config);
    }
    /*
     //example
     StreamString a;
     printf("%d\n", a.Printf("%!", configSignals));
     printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
     printf("%s\n", a.Buffer());
     printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
     */

    ok &= gam.InitialiseConfigSignals1();
    ok &= gam.SetConfiguredDatabase(gam.configSignals);

    if (ok) {
        ok &= !gam.Setup();
    }
    return ok;
}

bool SSMGAMTest::TestSetup() {
    bool ok = true;
    SSMGAMTestHelper gam;
    //n = 2, p = 1 and q = 1
    ok &= gam.InitialiseConfig1();
    if (ok) {
        ok &= gam.Initialise(gam.config);
    }
    /*
     //example
     StreamString a;
     printf("%d\n", a.Printf("%!", configSignals));
     printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
     printf("%s\n", a.Buffer());
     printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
     */

    ok &= gam.InitialiseConfigSignals1();
    ok &= gam.SetConfiguredDatabase(gam.configSignals);
    if (ok) {
        ok &= gam.AllocateInputSignalsMemory();
        ok &= gam.AllocateOutputSignalsMemory();
    }
    if (ok) {
        ok &= gam.Setup();
    }
    return ok;
}

bool SSMGAMTest::TestExecute() {
    bool ok = true;
    SSMGAMTestHelper gam;
    //n = 2, p = 1 and q = 1
    ok &= gam.InitialiseConfig1();
    if (ok) {
        ok &= gam.Initialise(gam.config);
    }
    if (ok) {
        ok &= gam.InitialiseConfigSignals1();
    }
    if (ok) {
        ok &= gam.SetConfiguredDatabase(gam.configSignals);
    }
    if (ok) {
        ok &= gam.AllocateInputSignalsMemory();
        ok &= gam.AllocateOutputSignalsMemory();
    }
    if (ok) {
        ok &= gam.Setup();
    }
    float64 *gamMemoryIn = NULL_PTR(float64 *);
    float64 *gamMemoryOutVector = NULL_PTR(float64 *);
    float64 *gamMemoryOutState1 = NULL_PTR(float64 *);
    float64 *gamMemoryOutState2 = NULL_PTR(float64 *);

    if (ok) {
        gamMemoryIn = static_cast<float64 *>(gam.GetInputSignalsMemory());
        gamMemoryOutVector = static_cast<float64 *>(gam.GetOutputSignalsMemory());
        gamMemoryOutState1 = static_cast<float64 *>(gam.GetOutputSignalsMemory(1u));
        gamMemoryOutState2 = static_cast<float64 *>(gam.GetOutputSignalsMemory(2u));

        *gamMemoryOutVector = 0.0;
        *gamMemoryOutState1 = 0.0;
        *gamMemoryOutState2 = 0.0;
        gamMemoryIn[0] = 1.0;
    }
    if (ok) {
        ok &= gam.Execute();
    }
    if (ok) {
        ok &= gam.IsEqualLargerMargins(*gamMemoryOutState1, 0.0);
        if (!ok) {
            printf("*gamMemoryOutState1 = %.16lf\n", *gamMemoryOutState1);
        }
        ok &= gam.IsEqualLargerMargins(*gamMemoryOutState2, 0.0);
        if (!ok) {
            printf("*gamMemoryOutState2 = %.16lf\n", *gamMemoryOutState2);
        }
        ok &= gam.IsEqualLargerMargins(*gamMemoryOutVector, 411.0);
        if (!ok) {
            printf("*gamMemoryOutVector = %.16lf\n", *gamMemoryOutVector);
        }
    }
    if (ok) {
        ok &= gam.Execute();
    }
    if (ok) {
        ok &= gam.IsEqualLargerMargins(*gamMemoryOutState1, 11.0);
        if (!ok) {
            printf("*gamMemoryOutState1 = %.16lf\n", *gamMemoryOutState1);
        }
        ok &= gam.IsEqualLargerMargins(*gamMemoryOutState2, 21.0);
        if (!ok) {
            printf("*gamMemoryOutState2 = %.16lf\n", *gamMemoryOutState2);
        }
        ok &= gam.IsEqualLargerMargins(*gamMemoryOutVector, 10384.0);
        if (!ok) {
            printf("*gamMemoryOutVector = %.16lf\n", *gamMemoryOutVector);
        }
    }
    return ok;
}

bool SSMGAMTest::TestExecuteSpring() {
    bool ok = true;
    SSMGAMTestHelper gam;
    //n = 2, p = 1 and q = 1
    ok &= gam.InitialiseConfigSpring();
    if (ok) {
        ok &= gam.Initialise(gam.config);
    }
    ok &= gam.InitialiseConfigSignalsSpring();
    ok &= gam.SetConfiguredDatabase(gam.configSignals);
    /*
     StreamString a;
     printf("%d\n", a.Printf("%!", gam.configSignals));
     printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
     printf("%s\n", a.Buffer());
     printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
     */
    if (ok) {
        ok &= gam.AllocateInputSignalsMemory();
        ok &= gam.AllocateOutputSignalsMemory();
    }
    if (ok) {
        ok &= gam.Setup();
    }
    float64 *gamMemoryIn = NULL_PTR(float64 *);
    float64 *gamMemoryOutVector = NULL_PTR(float64 *);
    float64 *gamMemoryOutState1 = NULL_PTR(float64 *);
    float64 *gamMemoryOutState2 = NULL_PTR(float64 *);

    if (ok) {
        gamMemoryIn = static_cast<float64 *>(gam.GetInputSignalsMemory());
        gamMemoryOutVector = static_cast<float64 *>(gam.GetOutputSignalsMemory());
        gamMemoryOutState1 = static_cast<float64 *>(gam.GetOutputSignalsMemory(1u));
        gamMemoryOutState2 = static_cast<float64 *>(gam.GetOutputSignalsMemory(2u));

        *gamMemoryOutVector = 0.0;
        *gamMemoryOutState1 = 0.0;
        *gamMemoryOutState2 = 0.0;

        gamMemoryIn[0] = 1.0;
    }
    if (ok) {
        ok &= gam.Execute();
    }

    if (ok) {
        ok &= gam.IsEqualLargerMargins(*gamMemoryOutVector, 0.0);
        if (!ok) {
            printf("*gamMemoryOutVector = %.16lf\n", *gamMemoryOutVector);
        }
    }

    if (ok) {
        gamMemoryIn[0] = 0.0;
        ok &= gam.Execute();
    }
    if (ok) {
        ok &= gam.IsEqualLargerMargins(*gamMemoryOutVector, 0.340299846608298);
        if (!ok) {
            printf("*gamMemoryOutVector = %.16lf\n", *gamMemoryOutVector);
        }
    }

    if (ok) {
        ok &= gam.Execute();
    }
    if (ok) {
        ok &= gam.IsEqualLargerMargins(*gamMemoryOutVector, 0.509125788245814);
        if (!ok) {
            printf("*gamMemoryOutVector = %.16lf\n", *gamMemoryOutVector);
        }
    }
    return ok;
}

bool SSMGAMTest::TestExecuteSpringNoFeedthroughMatrix() {
    bool ok = true;
    SSMGAMTestHelper gam;
    //n = 2, p = 1 and q = 1
    ok &= gam.InitialiseConfigSpringNoD();
    if (ok) {
        ok &= gam.Initialise(gam.config);
    }
    ok &= gam.InitialiseConfigSignalsSpring();
    ok &= gam.SetConfiguredDatabase(gam.configSignals);
    /*
     StreamString a;
     printf("%d\n", a.Printf("%!", gam.configSignals));
     printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
     printf("%s\n", a.Buffer());
     printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
     */
    if (ok) {
        ok &= gam.AllocateInputSignalsMemory();
        ok &= gam.AllocateOutputSignalsMemory();
    }
    if (ok) {
        ok &= gam.Setup();
    }
    float64 *gamMemoryIn = NULL_PTR(float64 *);
    float64 *gamMemoryOutVector = NULL_PTR(float64 *);
    float64 *gamMemoryOutState1 = NULL_PTR(float64 *);
    float64 *gamMemoryOutState2 = NULL_PTR(float64 *);

    if (ok) {
        gamMemoryIn = static_cast<float64 *>(gam.GetInputSignalsMemory());
        gamMemoryOutVector = static_cast<float64 *>(gam.GetOutputSignalsMemory());
        gamMemoryOutState1 = static_cast<float64 *>(gam.GetOutputSignalsMemory(1u));
        gamMemoryOutState2 = static_cast<float64 *>(gam.GetOutputSignalsMemory(2u));

        *gamMemoryOutVector = 0.0;
        *gamMemoryOutState1 = 0.0;
        *gamMemoryOutState2 = 0.0;

        gamMemoryIn[0] = 1.0;
    }
    if (ok) {
        ok &= gam.Execute();
    }

    if (ok) {
        ok &= gam.IsEqualLargerMargins(*gamMemoryOutVector, 0.0);
        if (!ok) {
            printf("*gamMemoryOutVector = %.16lf\n", *gamMemoryOutVector);
        }
    }

    if (ok) {
        gamMemoryIn[0] = 0.0;
        ok &= gam.Execute();
    }
    if (ok) {
        ok &= gam.IsEqualLargerMargins(*gamMemoryOutVector, 0.340299846608298);
        if (!ok) {
            printf("*gamMemoryOutVector = %.16lf\n", *gamMemoryOutVector);
        }
    }

    if (ok) {
        ok &= gam.Execute();
    }
    if (ok) {
        ok &= gam.IsEqualLargerMargins(*gamMemoryOutVector, 0.509125788245814);
        if (!ok) {
            printf("*gamMemoryOutVector = %.16lf\n", *gamMemoryOutVector);
        }
    }
    return ok;
}

bool SSMGAMTest::TestPrepareNextStateReset(uint32 resetEachStateConfig = 1u) {
    bool ok = true;
    SSMGAMTestHelper gam;
    //n = 2, p = 1 and q = 1
    ok &= gam.InitialiseConfigSpring(resetEachStateConfig);
    if (ok) {
        ok &= gam.Initialise(gam.config);
    }
    ok &= gam.InitialiseConfigSignalsSpring();
    ok &= gam.SetConfiguredDatabase(gam.configSignals);
    /*
     StreamString a;
     printf("%d\n", a.Printf("%!", gam.configSignals));
     printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
     printf("%s\n", a.Buffer());
     printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
     */
    if (ok) {
        ok &= gam.AllocateInputSignalsMemory();
        ok &= gam.AllocateOutputSignalsMemory();
    }
    if (ok) {
        ok &= gam.Setup();
    }
    float64 *gamMemoryIn = NULL_PTR(float64 *);
    float64 *gamMemoryOutVector = NULL_PTR(float64 *);
    float64 *gamMemoryOutState1 = NULL_PTR(float64 *);
    float64 *gamMemoryOutState2 = NULL_PTR(float64 *);

    if (ok) {
        gamMemoryIn = static_cast<float64 *>(gam.GetInputSignalsMemory());
        if (gamMemoryIn == NULL) {
            printf("gamMemoryIn is NULL\n");
        }
        gamMemoryOutVector = static_cast<float64 *>(gam.GetOutputSignalsMemory());
        if (gamMemoryOutVector == NULL) {
            printf("gamMemoryOutVector is NULL\n");
        }
        gamMemoryOutState1 = static_cast<float64 *>(gam.GetOutputSignalsMemory(1u));
        if (gamMemoryOutState1 == NULL) {
            printf("gamMemoryOutState1 is NULL\n");
        }
        gamMemoryOutState2 = static_cast<float64 *>(gam.GetOutputSignalsMemory(2u));
        if (gamMemoryOutState2 == NULL) {
            printf("gamMemoryOutState2 is NULL\n");
        }

        *gamMemoryOutVector = 0.0;
        *gamMemoryOutState1 = 0.0;
        *gamMemoryOutState2 = 0.0;

        gamMemoryIn[0] = 1.0;
    }
    if (ok) {
        ok &= gam.Execute();
    }

    if (ok) {
        ok &= gam.IsEqualLargerMargins(*gamMemoryOutVector, 0.0);
        if (!ok) {
            printf("*gamMemoryOutVector = %.16lf\n", *gamMemoryOutVector);
        }
    }

    if (ok) {
        gamMemoryIn[0] = 0.0;
        ok &= gam.Execute();
    }
    if (ok) {
        ok &= gam.IsEqualLargerMargins(*gamMemoryOutVector, 0.340299846608298);
        if (!ok) {
            printf("*gamMemoryOutVector = %.16lf\n", *gamMemoryOutVector);
        }
    }
    if (ok) {
        ok &= gam.PrepareNextState("A", "B");
    }

    if (ok) {
        gamMemoryIn[0] = 1.0;
        ok &= gam.Execute();
    }
    if (ok) {
        ok &= gam.IsEqualLargerMargins(*gamMemoryOutVector, 0.0);
        if (!ok) {
            printf("*gamMemoryOutVector = %.16lf\n", *gamMemoryOutVector);
        }
    }
    if (ok) {
        gamMemoryIn[0] = 0.0;
        ok &= gam.Execute();
    }
    if (ok) {
        ok &= gam.IsEqualLargerMargins(*gamMemoryOutVector, 0.340299846608298);
        if (!ok) {
            printf("*gamMemoryOutVector = %.16lf\n", *gamMemoryOutVector);
        }
    }
    return ok;
}

bool SSMGAMTest::TestPrepareNextStateNoReset(uint32 resetEachStateConfig = 1u) {
    bool ok = true;
    SSMGAMTestHelper gam;
    //n = 2, p = 1 and q = 1
    ok &= gam.InitialiseConfigSpring(resetEachStateConfig);
    if (ok) {
        ok &= gam.Initialise(gam.config);
    }
    ok &= gam.InitialiseConfigSignalsSpring();
    ok &= gam.SetConfiguredDatabase(gam.configSignals);
    /*
     StreamString a;
     printf("%d\n", a.Printf("%!", gam.configSignals));
     printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
     printf("%s\n", a.Buffer());
     printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
     */
    if (ok) {
        ok &= gam.AllocateInputSignalsMemory();
        ok &= gam.AllocateOutputSignalsMemory();
    }
    if (ok) {
        ok &= gam.Setup();
    }
    float64 *gamMemoryIn = NULL_PTR(float64 *);
    float64 *gamMemoryOutVector = NULL_PTR(float64 *);
    float64 *gamMemoryOutState1 = NULL_PTR(float64 *);
    float64 *gamMemoryOutState2 = NULL_PTR(float64 *);

    if (ok) {
        gamMemoryIn = static_cast<float64 *>(gam.GetInputSignalsMemory());
        if (gamMemoryIn == NULL) {
            printf("gamMemoryIn is NULL\n");
        }
        gamMemoryOutVector = static_cast<float64 *>(gam.GetOutputSignalsMemory());
        if (gamMemoryOutVector == NULL) {
            printf("gamMemoryOutVector is NULL\n");
        }
        gamMemoryOutState1 = static_cast<float64 *>(gam.GetOutputSignalsMemory(1u));
        if (gamMemoryOutState1 == NULL) {
            printf("gamMemoryOutState1 is NULL\n");
        }
        gamMemoryOutState2 = static_cast<float64 *>(gam.GetOutputSignalsMemory(2u));
        if (gamMemoryOutState2 == NULL) {
            printf("gamMemoryOutState2 is NULL\n");
        }

        *gamMemoryOutVector = 0.0;
        *gamMemoryOutState1 = 0.0;
        *gamMemoryOutState2 = 0.0;

        gamMemoryIn[0] = 1.0;
    }
    if (ok) {
        ok &= gam.PrepareNextState("", "A");
    }
    if (ok) {

        ok &= gam.Execute();
    }

    if (ok) {
        ok &= gam.IsEqualLargerMargins(*gamMemoryOutVector, 0.0);
        if (!ok) {
            printf("*gamMemoryOutVector = %.16lf\n", *gamMemoryOutVector);
        }
    }

    if (ok) {
        gamMemoryIn[0] = 0.0;
        ok &= gam.Execute();
    }
    if (ok) {
        ok &= gam.IsEqualLargerMargins(*gamMemoryOutVector, 0.340299846608298);
        if (!ok) {
            printf("*gamMemoryOutVector = %.16lf\n", *gamMemoryOutVector);
        }
    }
    if (ok) {
        ok &= gam.PrepareNextState("A", "B");
    }

    if (ok) {
        gamMemoryIn[0] = 0.0;
        ok &= gam.Execute();
    }
    if (ok) {
        ok &= gam.IsEqualLargerMargins(*gamMemoryOutVector, 0.509125788245814);
        if (!ok) {
            printf("*gamMemoryOutVector = %.16lf\n", *gamMemoryOutVector);
        }
    }
    return ok;
}

bool SSMGAMTest::TestPrepareNextStateNoMemoryAllocated(uint32 resetEachStateConfig = 1u) {
    bool ok = true;
    SSMGAMTestHelper gam;
    //n = 2, p = 1 and q = 1
    ok &= gam.InitialiseConfigSpring(resetEachStateConfig);
    if (ok) {
        ok &= gam.Initialise(gam.config);
    }
    ok &= gam.InitialiseConfigSignalsSpring();
    ok &= gam.SetConfiguredDatabase(gam.configSignals);
    ok &= !gam.PrepareNextState("A", "B");
    return ok;
}

}

