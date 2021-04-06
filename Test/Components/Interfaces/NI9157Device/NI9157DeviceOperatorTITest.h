/**
 * @file NI9157DeviceOperatorTITest.h
 * @brief Header file for class NI9157DeviceOperatorTITest.
 * @date 22/03/2021
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
 * NI9157DeviceOperatorTITest with all of its public, protected and private
 * members. It may also include definitions for inline methods which need to
 * be visible to the compiler.
 */

#ifndef NI9157DEVICEOPERATORTITEST_H_
#define NI9157DEVICEOPERATORTITEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "ConfigurationDatabase.h"
#include "CreateNI9157DeviceOperatorI.h"
#include "DataSourceI.h"
#include "GAMSchedulerI.h"
#include "GlobalObjectsDatabase.h"
#include "MemoryMapMultiBufferInputBroker.h"
#include "MemoryMapMultiBufferOutputBroker.h"
#include "MemoryMapSynchronisedMultiBufferInputBroker.h"
#include "MemoryMapSynchronisedMultiBufferOutputBroker.h"
#include "NI9157DeviceOperatorDatabase.h"
#include "NI9157DeviceOperatorTI.h"
#include "NI9157DeviceTest.h"
#include "NI9157DeviceTestsAndFwSettings.h"
#include "ObjectRegistryDatabase.h"
#include "RealTimeApplication.h"
#include "StandardParser.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
using namespace MARTe;

/**
 * @brief Tests the NI9157DeviceOperatorTI methods.
 */
class NI9157DeviceOperatorTITest {
public:

    /**
     * @brief Constructor.
     */
    NI9157DeviceOperatorTITest();

    /**
     * @brief Destructor.
     */
    virtual ~NI9157DeviceOperatorTITest();

    /**
     * @brief Tests the constructor.
     */
    bool TestConstructor();

    /**
     * @brief Tests the IsValid method.
     */
    bool TestIsValid();

    /**
     * @brief Tests the GetByteSize method.
     */
    bool TestGetByteSize();

    /**
     * @brief Tests the GetNI9157Device method.
     */
    bool TestGetNI9157Device();

    /**
     * @brief Tests the Compare method.
     */
    bool TestCompare();

    /**
     * @brief Tests the Copy method.
     */
    bool TestCopy();

    /**
     * @brief Tests the FindResource method.
     * @param model the NI crate model being used.
     */
    bool TestFindResource(uint32 model);

    /**
     * @brief Tests the NiWrite method.
     * @param model the NI crate model being used.
     */
    bool TestNiWrite(uint32 model);

    /**
     * @brief Tests the NiRead method.
     * @param model the NI crate model being used.
     */
    bool TestNiRead(uint32 model);

    /**
     * @brief Tests the NiWriteFifo method.
     * @param model the NI crate model being used.
     */
    bool TestNiWriteFifo(uint32 model);

    /**
     * @brief Tests the NiReadFifo method.
     * @param model the NI crate model being used.
     */
    bool TestNiReadFifo(uint32 model);

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* NI9157DEVICEOPERATORTITEST_H_ */
