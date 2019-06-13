/**
 * @file OPCUANodeTest.h
 * @brief Header file for class OPCUANodeTest
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

 * @details This header file contains the declaration of the class OPCUANodeTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef TEST_COMPONENTS_INTERFACES_OPCUA_OPCUANODETEST_H_
#define TEST_COMPONENTS_INTERFACES_OPCUA_OPCUANODETEST_H_

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
 * @brief Tests the OPCUANode public methods.
 */
class OPCUANodeTest {
public:

    /**
     * @brief Tests the constructor.
     */
    bool TestConstructor();

    /**
     * @brief Tests the IsNode method.
     */
    bool Test_IsNode();

    /**
     * @brief Tests the InitArray method.
     */
    bool Test_InitArray();

    /**
     * @brief Tests the GetOPCVariable with a uint8 variable.
     */
    bool TestGetOPCVariable_uint8();

    /**
     * @brief Tests the GetOPCVariable with a uint16 variable.
     */
    bool TestGetOPCVariable_uint16();

    /**
     * @brief Tests the GetOPCVariable with a uint32 variable.
     */
    bool TestGetOPCVariable_uint32();

    /**
     * @brief Tests the GetOPCVariable with a uint64 variable.
     */
    bool TestGetOPCVariable_uint64();

    /**
     * @brief Tests the GetOPCVariable with a float32 variable.
     */
    bool TestGetOPCVariable_float32();

    /**
     * @brief Tests the GetOPCVariable with a float64 variable.
     */
    bool TestGetOPCVariable_float64();

    /**
     * @brief Tests the GetOPCVariable with a int8 variable.
     */
    bool TestGetOPCVariable_int8();

    /**
     * @brief Tests the GetOPCVariable with a int16 variable.
     */
    bool TestGetOPCVariable_int16();

    /**
     * @brief Tests the GetOPCVariable with a int32 variable.
     */
    bool TestGetOPCVariable_int32();

    /**
     * @brief Tests the GetOPCVariable with a int64 variable.
     */
    bool TestGetOPCVariable_int64();

    /**
     * @brief Tests the GetOPCVariable with a uint8 array variable.
     */
    bool TestGetOPCVariable_array_uint8();

    /**
     * @brief Tests the GetOPCVariable with a uint16 array variable.
     */
    bool TestGetOPCVariable_array_uint16();

    /**
     * @brief Tests the GetOPCVariable with a uint32 array variable.
     */
    bool TestGetOPCVariable_array_uint32();

    /**
     * @brief Tests the GetOPCVariable with a uint64 array variable.
     */
    bool TestGetOPCVariable_array_uint64();

    /**
     * @brief Tests the GetOPCVariable with a float32 array variable.
     */
    bool TestGetOPCVariable_array_float32();

    /**
     * @brief Tests the GetOPCVariable with a float64 array variable.
     */
    bool TestGetOPCVariable_array_float64();

    /**
     * @brief Tests the GetOPCVariable with a int8 array variable.
     */
    bool TestGetOPCVariable_array_int8();

    /**
     * @brief Tests the GetOPCVariable with a int16 array variable.
     */
    bool TestGetOPCVariable_array_int16();

    /**
     * @brief Tests the GetOPCVariable with a int32 array variable.
     */
    bool TestGetOPCVariable_array_int32();

    /**
     * @brief Tests the GetOPCVariable with a int64 array variable.
     */
    bool TestGetOPCVariable_array_int64();


};



/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* TEST_COMPONENTS_INTERFACES_OPCUA_OPCUANODETEST_H_ */
	
