/**
 * @file OPCUADSOutputTest.h
 * @brief Header file for class OPCUADSOutputTest
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
 * @warning Unless?
 *  required by applicable law or agreed to in writing,
 * software distributed under the Licence is distributed on an "AS IS"
 * basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
 * or implied. See the Licence permissions and limitations under the Licence.

 * @details This header file contains the declaration of the class OPCUADSOutputTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef TEST_COMPONENTS_DATASOURCES_OPCUADATASOURCE_OPCUADSOUTPUTTEST_H_
#define TEST_COMPONENTS_DATASOURCES_OPCUADATASOURCE_OPCUADSOUTPUTTEST_H_

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
 * @brief Tests the OPCUADSOutput public methods.
 */
class OPCUADSOutputTest {
public:
    /**
     * @brief Tests the constructor.
     */
    bool TestConstructor();

    /**
     * @brief Tests the Initialise method.
     */
    bool TestInitialise();

    /**
     * @brief Tests the Initialise method when the signals are Introspection Structures.
     */
    bool TestInitialise_Introspection();

    /**
     * @brief Tests the Initialise method with Introspections Structures read as ExtensionObject.
     */
    bool TestInitialise_ExtensionObject();

    /**
     * @brief Tests the Initialise method with more than one ExtensionObject which is not allowed.
     */
    bool TestInitialise_MoreThanOneExtensionObject();

    /**
     * @brief Tests the Initialise method with more than one structured type, which is not allowed.
     */
    bool TestInitialise_MoreThanOneStruct();

    /**
     * @brief Tests the Initialise method without specifyng the Path in the ExtensionObject.
     */
    bool TestInitialise_ExtensionObject_NoPath();

    /**
     * @brief Tests the Initialise method without specifyng the NamespaceIndex in the ExtensionObject.
     */
    bool TestInitialise_ExtensionObject_NoNamespaceIndex();

    /**
     * @brief Tests the Initialise method without specifyng the Path in the structured type.
     */
    bool TestInitialise_Struct_NoPath();

    /**
     * @brief Tests the Initialise method without specifyng the NamespaceIndex in the structured type.
     */
    bool TestInitialise_Struct_NoNamespaceIndex();

    /**
     * @brief Tests the Initialise method without specifying the address parameter.
     */
    bool TestInitialise_NoAddress();

    /**
     * @brief Tests the Initialise method without specifying any signal.
     */
    bool TestInitialise_NoSignals();

    /**
     * @brief Tests the Initialise method without specifying the path parameter.
     */
    bool TestInitialise_NoPath();

    /**
     * @brief Tests the Initialise method without specifying the NamespaceIndex parameter.
     */
    bool TestInitialise_NoNamespaceIndex();

    /**
     * @brief Tests the Initialise method without specifying an invalid Authentication method.
     */
    bool TestInitialise_Authentication_InvalidParameter();

    /**
     * @brief Tests the Initialise method with Authentication pointing at an invalid file.
     */
    bool TestInitialise_Authentication_InvalidFile();

    /**
     * @brief Tests the Initialise method specifying a signal number of dimensions greater than 1.
     */
    bool Test_NumberOfDimensionsGreater1();

    /**
     * @brief Tests the SetTargetNodes method with a wrong path that doesn't match any variable path on the server.
     */
    bool Test_SetConfiguredDatabase_FailSetServiceRequest();

    /**
     * @brief Tests that the ExtensionObject is not allowed to be mixed with normal nodes.
     */
    bool Test_SetConfiguredDatabase_ExtensionObject_MixedSignal();

    /**
     * @brief Tests that the ExtensionObject is not allowed to be mixed with normal nodes and timing signals.
     */
    bool Test_SetConfiguredDatabase_ExtensionObject_MixedSignals();

    /**
     * @brief Tests that the structured type is not allowed to be mixed with normal nodes.
     */
    bool Test_SetConfiguredDatabase_Struct_MixedSignal();

    /**
     * @brief Tests that the structured type is not allowed to be mixed with normal nodes and timing signals.
     */
    bool Test_SetConfiguredDatabase_Struct_MixedSignals();

    /**
     * @brief Tests that only one DefaultTimestampSignal is allowed.
     */
    bool Test_SetConfiguredDatabase_MoreThanOneDefaultTimestamp();

    /**
     * @brief Tests that only uint64 is allowed for the timestamp signal.
     */
    bool Test_SetConfiguredDatabase_TimestampNotUint64();

    /**
     * @brief Tests that the timestamp signal is scalar.
     */
    bool Test_SetConfiguredDatabase_TimestampNotScalar();

    /**
     * @brief Tests that a trigger shall be set.
     */
    bool Test_SetConfiguredDatabase_NoTrigger();

    /**
     * @brief Tests that with ExtensionObject the only timestamp signal allowed is with DefaultTimestampSignal.
     */
    bool Test_SetConfiguredDatabase_ExtensionObject_Timestamp_NotDefault();

    /**
     * @brief Tests that with ExtensionObject only one timestamp signal is allowed.
     */
    bool Test_SetConfiguredDatabase_ExtensionObject_MoreThanOneTimestamp();

    /**
     * @brief Tests that with structured types the only timestamp signal allowed is with DefaultTimestampSignal.
     */
    bool Test_SetConfiguredDatabase_Struct_Timestamp_NotDefault();

    /**
     * @brief Tests that with structured types only one timestamp signal is allowed.
     */
    bool Test_SetConfiguredDatabase_Struct_MoreThanOneTimestamp();

    /**
     * @brief Tests that the DataSource only accepts one trigger signal
     */
    bool Test_SetConfiguredDatabase_MoreThanOneTrigger();

    /**
     * @brief Tests that the DataSource does not accept input signals. 
     */
    bool Test_SetConfiguredDatabase_InputSignals();

    /**
     * @brief Tests that the DataSource detects an invalid timestamping signal.
     */
    bool Test_SetConfiguredDatabase_TimestampNonExisting();

    /**
     * @brief Test the user/password Authentication capability of the OPC UA Server.
     */
    bool Test_Authentication();

    /**
     * @brief Test the user/password Authentication capability of the OPC UA Server with no creds.
     */
    bool Test_Authentication_NoCreds();

    /**
     * @brief Test the user/password Authentication capability of the OPC UA Server with bad creds.
     */
    bool Test_Authentication_BadCreds();

    /**
     * @brief Test the Synchronise method with no timing signals
     */
    bool Test_Synchronise();

    /**
     * @brief Test the Synchronise method with ExtensionObject
     */
    bool Test_Synchronise_ExtensionObject();

    /**
     * @brief Test the Synchronise method with a structured type that is not an ExtensionObject 
     */
    bool Test_Synchronise_Struct();

    /**
     * @brief Test the Synchronise method with timing signals
     */
    bool Test_Synchronise_Timestamp();

    /**
     * @brief Test the Synchronise method with only the DefaultTimestampSignal
     */
    bool Test_Synchronise_Timestamp_Default_Only();

    /**
     * @brief Test the Synchronise method with only the DefaultTimestampSignal and non DefaultTimestampSignal
     */
    bool Test_Synchronise_Timestamp_Default_Mix();

    /**
     * @brief Test the Synchronise method with ExtensionObject and time stamping at idx 0
     */
    bool Test_Synchronise_ExtensionObject_Timestamp_Idx_0();

    /**
     * @brief Test the Synchronise method with ExtensionObject and time stamping at idx last
     */
    bool Test_Synchronise_ExtensionObject_Timestamp_Idx_Last();

    /**
     * @brief Test the Synchronise method with a structured type and time stamping at idx 0  
     */
    bool Test_Synchronise_Struct_Timestamp_Idx_0();

    /**
     * @brief Test the Synchronise method with a structured type and time stamping at idx 0  
     */
    bool Test_Synchronise_Struct_Timestamp_Idx_Last();

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* TEST_COMPONENTS_DATASOURCES_OPCUADATASOURCE_OPCUADSOUTPUTTEST_H_ */

