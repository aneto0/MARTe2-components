/**
 * @file OPCUAServerTest.h
 * @brief Header file for class OPCUAServerTest
 * @date 12/03/2019
 * @author Luca Porzio
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

 * @details This header file contains the declaration of the class OPCUAServerTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef TEST_COMPONENTS_INTERFACES_OPCUA_OPCUASERVERTEST_H_
#define TEST_COMPONENTS_INTERFACES_OPCUA_OPCUASERVERTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
/**
 * @brief Tests the OPCUAServer public methods.
 */
class OPCUAServerTest {
public:

    /**
     * @brief Tests the constructor
     */
    bool TestConstructor();

    /**
     * @brief Tests the SetRunning method
     */
    bool Test_SetRunning();

    /**
     * @brief Tests the GetRunning method
     */
    bool Test_GetRunning();

    /**
     * @brief Tests the GetCPUMask method
     */
    bool Test_GetCPUMask();

    /**
     * @brief Tests the GetStackSize method
     */
    bool Test_GetStackSize();

    /**
     * @brief Tests the GetPort method
     */
    bool Test_GetPort();

    /**
     * @brief Tests the InitAddressSpace method
     */
    bool Test_InitAddressSpace();

    /**
     * @brief Tests the Initialise method with default parameters.
     */
    bool TestInitialise_Default();

    /**
     * @brief Tests the Initialise method specifying all the optional parameters.
     */
    bool TestInitialise_NoDefault();

    /**
     * @brief Tests the Initialise method without specifying any Address Space.
     */
    bool TestInitialise_NoAddressSpace();

    /**
     * @brief Tests the Execute method.
     */
    bool TestExecute();

    /**
     * @brief Tests the Execute method when the Address Space has to be constructed from an Introspection Structure.
     */
    bool TestExecute_Introspection();

    /**
     * @brief Tests the Execute method.
     */
    bool TestExecute_NoType();

    /**
     * @brief Tests the Execute method when the Address Space has to be constructed from an Introspection Structure with array variables.
     */
    bool TestExecute_IntrospectionArray();

    /**
     * @brief Tests the GetStructure method when a variable number of dimensions is greater than 1.
     */
    bool TestGetStructure_WrongNDimensions();

};


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* TEST_COMPONENTS_INTERFACES_OPCUA_OPCUASERVERTEST_H_ */
	
