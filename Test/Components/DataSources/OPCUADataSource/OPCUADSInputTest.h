/**
 * @file OPCUADSInputTest.h
 * @brief Header file for class OPCUADSInputTest
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

 * @details This header file contains the declaration of the class OPCUADSInputTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef TEST_COMPONENTS_DATASOURCES_OPCUADATASOURCE_OPCUADSINPUTTEST_H_
#define TEST_COMPONENTS_DATASOURCES_OPCUADATASOURCE_OPCUADSINPUTTEST_H_

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
 * @brief Tests the OPCUADSInput public methods.
 */
class OPCUADSInputTest {
public:
    /**
     * @brief Tests the constructor.
     */
    bool TestConstructor();

    /**
     * @brief Tests the Initialise method with default parameters.
     */
    bool TestInitialise_Default();

    /**
     * @brief Tests the Initialise method with Introspections Structures.
     */
    bool TestInitialise_Introspection();

    /**
     * @brief Tests the Initialise method with Sync option enabled.
     */
    bool TestInitialise_Sync();

    /**
     * @brief Tests the Initialise method with Sync option and MonitoredItem service enabled.
     */
    bool TestInitialise_SyncMonitor();

    /**
     * @brief Tests the Initialise method with Sync option enabled and specifying a not supported read service.
     */
    bool TestInitialise_SyncWrongMode();

    /**
     * @brief Tests the Initialise method with Thread mode enabled and default parameters.
     */
    bool TestInitialise_Thread_Default();

    /**
     * @brief Tests the Initialise method with Thread mode enabled and CpuMask and StackSize defined.
     */
    bool TestInitialise_Thread_NoDefault();

    /**
     * @brief Tests the Initialise method with Thread mode and MonitoredItem enabled.
     */
    bool TestInitialise_ThreadMonitor();

    /**
     * @brief Tests the Initialise method with Thread mode enabled and specifying a not supported read service.
     */
    bool TestInitialise_ThreadWrongMode();

    /**
     * @brief Tests the Initialise method without specifying Address parameter.
     */
    bool TestInitialise_NoAddress();

    /**
     * @brief Tests the Initialise method without specifying any signal.
     */
    bool TestInitialise_NoSignals();

    /**
     * @brief Tests the Initialise method without specifying Path parameter.
     */
    bool TestInitialise_NoPath();

    /**
     * @brief Tests the Initialise method without specifying NAmespaceIndex parameter.
     */
    bool TestInitialise_NoNamespaceIndex();

    /**
     * @brief Tests the Initialise method specifying a signal number of dimensions greater than 1.
     */
    bool Test_NumberOfDimensionsGreater1();

    /**
     * @brief Tests the SetTargetNodes method with a wrong path that doesn't match any variable path on the server.
     */
    bool Test_FailSetTargetNodes();

};


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* TEST_COMPONENTS_DATASOURCES_OPCUADATASOURCE_OPCUADSINPUTTEST_H_ */
	
