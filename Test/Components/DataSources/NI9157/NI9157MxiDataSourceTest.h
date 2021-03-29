/**
 * @file NI9157MxiDataSourceTest.h
 * @brief Header file for class NI9157MxiDataSourceTest.
 * @date 11/02/2021
 * @author Giuseppe Ferro
 * @author Pedro Lourenco
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
 *
 * @details This header file contains the declaration of the class
 * NI9157MxiDataSourceTest with all of its public, protected and private
 * members. It may also include definitions for inline methods which need to
 * be visible to the compiler.
 */

#ifndef NI9157MXIDATASOURCETEST_H_
#define NI9157MXIDATASOURCETEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "ConfigurationDatabase.h"
#include "DataSourceI.h"
#include "GAMSchedulerI.h"
#include "IOGAM.h"
#include "Interleaved2FlatGAM.h"
#include "MemoryMapInputBroker.h"
#include "MemoryMapMultiBufferInputBroker.h"
#include "MemoryMapMultiBufferOutputBroker.h"
#include "MemoryMapOutputBroker.h"
#include "MemoryMapSynchronisedInputBroker.h"
#include "MemoryMapSynchronisedMultiBufferInputBroker.h"
#include "MemoryMapSynchronisedMultiBufferOutputBroker.h"
#include "MemoryMapSynchronisedOutputBroker.h"
#include "NI9157MxiDataSource.h"
#include "NI9157TestsAndFwSettings.h"
#include "ObjectRegistryDatabase.h"
#include "RealTimeApplication.h"
#include "StandardParser.h"
#include "Threads.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
using namespace MARTe;

/**
 * @brief Tests the NI9157MxiDataSource methods.
 */
class NI9157MxiDataSourceTest {
public:

    /**
     * @brief NI9157MxiDataSourceTest Constructor.
     */
    NI9157MxiDataSourceTest();

    /**
     * @brief NI9157MxiDataSourceTest Destructor.
     */
    virtual ~NI9157MxiDataSourceTest();

    /**
     * @brief Tests the NI9157MxiDataSource Constructor.
     */
    bool TestConstructor();

    /**
     * @brief Tests the NI9157MxiDataSource::Initialise method.
     */
    bool TestInitialise(uint32 model);

    /**
     * @brief Tests the NI9157MxiDataSource::Initialise method with the
	 * default Run parameter.
     */
    bool TestInitialise_DefaultRunNi(uint32 model);

    /**
     * @brief Tests the NI9157MxiDataSource::Initialise method that returns
	 * false if the path is not specified.
     */
    bool TestInitialise_False_NoNiDev(uint32 model);

    /**
     * @brief Tests the NI9157MxiDataSource::Initialise method that returns
	 * false if the path is invalid.
     */
    bool TestInitialise_False_InvalidNiDevPath(uint32 model);

    /**
     * @brief Tests the NI9157MxiDataSource::GetBrokerName method.
     */
    bool TestGetBrokerName();

    /**
     * @brief Tests the PrepareNextState method
     */
    bool TestPrepareNextState(uint32 model);

    /**
     * @brief Tests the NI9157MxiDataSource::SetConfiguredDatabase method.
     */
    bool TestSetConfiguredDatabase(uint32 model);

    /**
     * @brief Tests the NI9157MxiDataSource::SetConfiguredDatabase method with
	 * an invalid variable.
     */
    bool TestSetConfiguredDatabase_False_InvalidLabviewVar(uint32 model);

    /**
     * @brief Tests the NI9157MxiDataSource::SetConfiguredDatabase method with
	 * an invalid type.
     */
    bool TestSetConfiguredDatabase_False_InvalidType(uint32 model);

    /**
     * @brief Tests the NI9157MxiDataSource::Synchronise method.
     */
    bool TestSynchronise(uint32 model);

    /**
     * @brief Tests the NI9157MxiDataSource::Synchronise method for IO vars.
     */
    bool TestSynchronise_Variables(uint32 model);

    /**
     * @brief Tests the NI9157MxiDataSource::Synchronise method for IO FIFOs.
     */
    bool TestSynchronise_FIFOs(uint32 model);

    /**
     * @brief Tests the NI9157MxiDataSource::Synchronise using initial patterns.
     */
    bool TestSynchronise_InitialPatterns(uint32 model);

    /**
     * @brief Tests the NI9157MxiDataSource::Synchronise using BlockIfNotRunning.
     */
    bool TestSynchronise_BlockIfNotRunning(uint32 model);

    /**
     * @brief Tests the NI9157MxiDataSource::AsyncRead method.
     */
    bool TestAsyncRead(uint32 model);

    /**
     * @brief Tests the NI9157MxiDataSource::AsyncWrite method.
     */
    bool TestAsyncWrite(uint32 model);

    /**
     * @brief Test the NI9157MxiDataSource::Reset method.
     */
    bool TestReset(uint32 model);

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* NI9157MXIDATASOURCETEST_H_ */
