/**
 * @file OPCUAReferenceContainerTest.h
 * @brief Header file for class OPCUAReferenceContainerTest
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

 * @details This header file contains the declaration of the class OPCUAReferenceContainerTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef TEST_COMPONENTS_INTERFACES_OPCUA_OPCUAREFERENCECONTAINERTEST_H_
#define TEST_COMPONENTS_INTERFACES_OPCUA_OPCUAREFERENCECONTAINERTEST_H_

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
 * @brief Tests the OPCUAReferenceContainer public methods.
 */
class OPCUAReferenceContainerTest {
public:
    /**
     * @brief Tests the constructor.
     */
    bool TestConstructor();

    /**
     * @brief Tests the IsObject method.
     */
    bool Test_IsObject();

    /**
     * @brief Tests the IsNode method.
     */
    bool Test_IsNode();

    /**
     * @brief Tests the GetOPCVariable method.
     */
    bool Test_GetOPCVariable();

    /**
     * @brief Tests the GetOPCObject method.
     */
    bool Test_GetOPCObject();

    /**
     * @brief Tests the SetParent method.
     */
    bool Test_SetParent();

    /**
     * @brief Tests the GetParent method.
     */
    bool Test_GetParent();

    /**
     * @brief Tests the SetNodeId method.
     */
    bool Test_SetNodeId();

    /**
     * @brief Tests the GetNodeId method.
     */
    bool Test_GetNodeId();

    /**
     * @brief Tests the SetNodeType method.
     */
    bool Test_SetNodeType();

    /**
     * @brief Tests the GetNodeType method.
     */
    bool Test_GetNodeType();

    /**
     * @brief Tests the IsFirst method.
     */
    bool Test_IsFirst();

    /**
     * @brief Tests the SetFirst method.
     */
    bool Test_SetFirst();

    /**
     * @brief Tests the SetNumberOfElements method.
     */
    bool Test_SetNumberOfElements();

    /**
     * @brief Tests the GetNumberOfElements method.
     */
    bool Test_GetNumberOfElements();

    /**
     * @brief Tests the SetNumberOfDimensions method.
     */
    bool Test_SetNumberOfDimensions();

    /**
     * @brief Tests the GetNumberOfDimensions method.
     */
    bool Test_GetNumberOfDimensions();

};


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* TEST_COMPONENTS_INTERFACES_OPCUA_OPCUAREFERENCECONTAINERTEST_H_ */
	
