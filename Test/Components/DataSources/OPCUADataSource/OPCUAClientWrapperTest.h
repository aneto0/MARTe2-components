/**
 * @file OPCUAClientWrapperTest.h
 * @brief Header file for class OPCUAClientWrapperTest
 * @date 15/03/2019
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

 * @details This header file contains the declaration of the class OPCUAClientWrapperTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef TEST_COMPONENTS_DATASOURCES_OPCUADATASOURCE_OPCUACLIENTWRAPPERTEST_H_
#define TEST_COMPONENTS_DATASOURCES_OPCUADATASOURCE_OPCUACLIENTWRAPPERTEST_H_

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
 * @brief Tests the OPCUAClientWrapper public methods.
 */
class OPCUAClientWrapperTest {
public:

    /**
     * @brief Tests the Write method with a uint8 single variable
     */
    bool Test_Write_uint8();

    /**
     * @brief Tests the Write method with a uint16 single variable
     */
    bool Test_Write_uint16();

    /**
     * @brief Tests the Write method with a uint32 single variable
     */
    bool Test_Write_uint32();

    /**
     * @brief Tests the Write method with a uint64 single variable
     */
    bool Test_Write_uint64();

    /**
     * @brief Tests the Write method with a int8 single variable
     */
    bool Test_Write_int8();

    /**
     * @brief Tests the Write method with a int16 single variable
     */
    bool Test_Write_int16();

    /**
     * @brief Tests the Write method with a int32 single variable
     */
    bool Test_Write_int32();

    /**
     * @brief Tests the Write method with a int64 single variable
     */
    bool Test_Write_int64();

    /**
     * @brief Tests the Write method with a float32 single variable
     */
    bool Test_Write_float32();

    /**
     * @brief Tests the Write method with a float64 single variable
     */
    bool Test_Write_float64();

    /**
     * @brief Tests the SetTargetNodes method with a wrong string identifier that doesn't match any variable on the server.
     */
    bool Test_WrongNodeId();

    /**
     * @brief Tests the Write method with a uint8 array variable
     */
    bool Test_Write_uint8_array();

    /**
     * @brief Tests the Write method with a uint16 array variable
     */
    bool Test_Write_uint16_array();

    /**
     * @brief Tests the Write method with a uint32 array variable
     */
    bool Test_Write_uint32_array();

    /**
     * @brief Tests the Write method with a uint64 array variable
     */
    bool Test_Write_uint64_array();

    /**
     * @brief Tests the Write method with a int8 array variable
     */
    bool Test_Write_int8_array();

    /**
     * @brief Tests the Write method with a int16 array variable
     */
    bool Test_Write_int16_array();

    /**
     * @brief Tests the Write method with a int32 array variable
     */
    bool Test_Write_int32_array();

    /**
     * @brief Tests the Write method with a int64 array variable
     */
    bool Test_Write_int64_array();

    /**
     * @brief Tests the Write method with a float32 array variable
     */
    bool Test_Write_float32_array();

    /**
     * @brief Tests the Write method with a float64 array variable
     */
    bool Test_Write_float64_array();

    /**
     * @brief Tests the Read method.
     */
    bool Test_Read();
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* TEST_COMPONENTS_DATASOURCES_OPCUADATASOURCE_OPCUACLIENTWRAPPERTEST_H_ */

