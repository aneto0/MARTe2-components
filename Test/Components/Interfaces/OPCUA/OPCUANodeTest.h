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

class OPCUANodeTest {
public:

    bool TestGetOPCVariable_uint8();

    bool TestGetOPCVariable_uint16();

    bool TestGetOPCVariable_uint32();

    bool TestGetOPCVariable_uint64();

    bool TestGetOPCVariable_float32();

    bool TestGetOPCVariable_float64();

    bool TestGetOPCVariable_int8();

    bool TestGetOPCVariable_int16();

    bool TestGetOPCVariable_int32();

    bool TestGetOPCVariable_int64();

    bool TestGetOPCVariable_array_uint8();

    bool TestGetOPCVariable_array_uint16();

    bool TestGetOPCVariable_array_uint32();

    bool TestGetOPCVariable_array_uint64();

    bool TestGetOPCVariable_array_float32();

    bool TestGetOPCVariable_array_float64();

    bool TestGetOPCVariable_array_int8();

    bool TestGetOPCVariable_array_int16();

    bool TestGetOPCVariable_array_int32();

    bool TestGetOPCVariable_array_int64();


};



/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* TEST_COMPONENTS_INTERFACES_OPCUA_OPCUANODETEST_H_ */
	
