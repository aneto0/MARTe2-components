/**
 * @file ObjectConnectionITest.h
 * @brief Header file for class ObjectConnectionITest
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

 * @details This header file contains the declaration of the class ObjectConnectionITest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef OBJECTCONNECTIONITEST_H_
#define OBJECTCONNECTIONITEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "AdvancedErrorManagement.h"
#include "ConfigurationDatabase.h"
#include "ObjectConnectionI.h"
#include "DataSourceI.h"
#include "Directory.h"
#include "ErrorInformation.h"
#include "ErrorManagement.h"
#include "FastMath.h"
#include "GAM.h"
#include "GAMScheduler.h"
#include "GlobalObjectsDatabase.h"
#include "MemoryMapInputBroker.h"
#include "MemoryMapOutputBroker.h"
#include "MemoryOperationsHelper.h"
#include "ObjectConnectionI.h"
#include "ObjectRegistryDatabase.h"
#include "RealTimeApplication.h"
#include "SafeMath.h"
#include "StandardParser.h"
#include "StaticList.h"
#include "StreamString.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

class TestObjectConnectionI : public MARTe::ObjectConnectionI {
public:
    CLASS_REGISTER_DECLARATION()

    TestObjectConnectionI();

    ~TestObjectConnectionI();

    ErrorManagement::ErrorType TestTransposeAndCopy(void *const destination, const void *const source, const TypeDescriptor typeDesc,
        const uint32 numberOfRows, const uint32 numberOfColumns, const uint32 numberOfPages);

};


/**
 * @brief Test all the ObjectConnectionITest methods
 */
class ObjectConnectionITest {
public:

    /**
     * @brief Constructor
     */
    ObjectConnectionITest();

    /**
     * @brief Destructor
     */
    virtual ~ObjectConnectionITest();

    /**
     * @brief Tests the constructor
     */
    bool TestConstructor();

    /**
     * @brief Tests the ObjectConnectionITest::Initialise method
     */
    bool TestInitialise();

    /**
     * @brief Tests the ObjectConnectionITest::Initialise method
     */
    template<typename T>
    bool TestTransposeAndCopy(const TypeDescriptor typeDesc);

    /**
     * @brief Tests the ObjectConnectionITest::Initialise method
     */
    bool TestTransposeAndCopy_Failed_InvalidType();



private:

    /**
     * @brief Template to test the Initialise() method.
     * @param[in]  configStreamIn The configuration stream to test the Initialise method.
     * @param[out] statusOut      The returned status of the interface after initialisation.
     * @param[out] cdbOut         The returned configuration database after initalisation.
     */
    bool TestInitialiseWithConfiguration(StreamString configStreamIn, ErrorManagement::ErrorType& statusOut, ConfigurationDatabase& cdbOut, ObjectConnectionI& loader);

    /**
     * @brief Same, but does not return the configuration database.
     */
    bool TestInitialiseWithConfiguration(StreamString configStreamIn, ErrorManagement::ErrorType& statusOut);


};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

template<typename T>
bool ObjectConnectionITest::TestTransposeAndCopy(const TypeDescriptor typeDesc) {

    TestObjectConnectionI obConnection;
    ErrorManagement::ErrorType status = ErrorManagement::FatalError;
    bool ok = false;

    T rowMajorArray[2][3] = { {1, 2, 3}, {4, 5, 6} };
    T colMajorArray[3][2] = { {1, 4}, {2, 5}, {3, 6} };
    T outputArray[3][2]   = { {0, 0}, {0, 0}, {0, 0} };

    status = obConnection.TestTransposeAndCopy(outputArray, rowMajorArray, typeDesc, 2, 3, 1);
    ok = (MemoryOperationsHelper::Compare(&outputArray[0][0], &colMajorArray[0][0], 6u*sizeof(T)) == 0u);

    if (status && ok) {
        // input array
//         T rowMajor3DArray[2][3][4] = {
//             { { 1,  2,  3,  4}, { 5,  6,  7,  8}, { 9, 10, 11, 12} },
//             { {13, 14, 15, 16}, {17, 18, 19, 20}, {21, 22, 23, 24} }
//         };
        T rowMajor3DArray[3][4][2] = {
            { { 1, 13}, { 2, 14}, { 3, 15}, { 4, 16} },
            { { 5, 17}, { 6, 18}, { 7, 19}, { 8, 20} },
            { { 9, 21}, {10, 22}, {11, 23}, {12, 24} }
        };

        // expected array
        T colMajor3DArray[2][4][3] = {
            { { 1,  5,  9}, { 2,  6, 10}, { 3,  7, 11}, { 4,  8, 12} },
            { {13, 17, 21}, {14, 18, 22}, {15, 19, 23}, {16, 20, 24} }
        };

        // output array
        T output3DArray[2][4][3] = {
            { {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0} },
            { {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0} }
        };

        status = obConnection.TestTransposeAndCopy(output3DArray, rowMajor3DArray, typeDesc, 3, 4, 2);
        ok = (MemoryOperationsHelper::Compare(&output3DArray[0][0][0], &colMajor3DArray[0][0][0], 24u*sizeof(T)) == 0u);
    }

    return ( status.ErrorsCleared() && ok);
}

#endif /* OBJECTCONNECTIONITEST_H_ */

