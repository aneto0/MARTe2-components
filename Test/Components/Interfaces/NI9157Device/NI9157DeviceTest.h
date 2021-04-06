/**
 * @file NI9157DeviceTest.h
 * @brief Header file for class NI9157DeviceTest
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
 * NI9157DeviceTest with all of its public, protected and private members. It
 * may also include definitions for inline methods which need to be visible to
 * the compiler.
 */

#ifndef NI9157DEVICETEST_H_
#define NI9157DEVICETEST_H_

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
#include "MemoryMapMultiBufferInputBroker.h"
#include "MemoryMapMultiBufferOutputBroker.h"
#include "MemoryMapSynchronisedMultiBufferInputBroker.h"
#include "MemoryMapSynchronisedMultiBufferOutputBroker.h"
#include "NI9157Device.h"
#include "NI9157DeviceTestsAndFwSettings.h"
#include "ObjectRegistryDatabase.h"
#include "RealTimeApplication.h"
#include "StandardParser.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
using namespace MARTe;

/**
 * @brief Tests the NI9157Device methods.
 */
class NI9157DeviceTest {
public:

    /**
     * @brief Constructor.
     */
    NI9157DeviceTest();

    /**
     * @brief Destructor.
     */
    virtual ~NI9157DeviceTest();

    /**
     * @brief Tests the constructor.
     */
    bool TestConstructor();

    /**
     * @brief Tests the NI9157Device::Initialise method.
     * @param model the NI crate model being used.
     */
    bool TestInitialise(uint32 model);

    /**
     * @brief Tests the NI9157Device::Initialise method with parameter Open=1.
     * @param model the NI crate model being used.
     */
    bool TestInitialiseIsOpened(uint32 model);

    /**
     * @brief Tests the NI9157Device::Initialise mixing the variable types.
     * @param model the NI crate model being used.
     */
    bool TestInitialiseRandomConfig(uint32 model);

    /**
     * @brief Tests the NI9157Device::Initialise method that fails if 
	 * NiRioDeviceName is not specified.
     * @param model the NI crate model being used.
     */
    bool TestInitialise_FalseNoDeviceName(uint32 model);

    /**
     * @brief Tests the NI9157Device::Initialise method that fails if
	 * NiRioGenFile is not specified.
     * @param model the NI crate model being used.
     */
    bool TestInitialise_FalseNoGenFile(uint32 model);

    /**
     * @brief Tests the NI9157Device::Initialise method that fails if 
	 * NiRioGenSignature is not specified.
     * @param model the NI crate model being used.
     */
    bool TestInitialise_FalseNoGenSignature(uint32 model);

    /**
     * @brief Tests the NI9157Device::Initialise method that fails if
	 * the type id is not found in the variable to be initialised.
     * @param model the NI crate model being used.
     */
    bool TestInitialise_FalseNoType(uint32 model);

    /**
     * @brief Tests the NI9157Device::Initialise method that fails if the type
	 * id is not found in the variable is not found in the Labview project.
     * @param model the NI crate model being used.
     */
    bool TestInitialise_FalseVariableNotFound(uint32 model);

    /**
     * @brief Tests the NI9157Device::Initialise method that fails with a device
	 * that is not found when Open in set.
     * @param model the NI crate model being used.
     */
    bool TestInitialise_FalseOpenFakeDevice(uint32 model);

    /**
     * @brief Tests the NI9157Device::Initialise method does not fail with Open,
     * Reset and Run parameters set.
     * @param model the NI crate model being used.
     */
    bool TestInitialise_OpenResetRun(uint32 model);

    /**
     * @brief Tests the NI9157Device::Initialise method does not fail without
	 * Open, Reset and Run parameters set.
     * @param model the NI crate model being used.
     */
    bool TestInitialise_NoOpenResetRun(uint32 model);

    /**
     * @brief Tests the NI9157Device::Initialise method that does not fail without
	 * Configuration.
     * @param model the NI crate model being used.
     */
    bool TestInitialise_FalseNoConfig(uint32 model);

    /**
     * @brief Tests the NI9157Device::Open method.
     * @param model the NI crate model being used.
     */
    bool TestOpen(uint32 model);

    /**
     * @brief Tests the NI9157Device::Open method that fails for a device that
     * can not be found.
     * @param model the NI crate model being used.
     */
    bool TestOpen_FalseParams(uint32 model);

	/**
     * @brief Tests the NI9157Device::IsOpened method.
     * @param model the NI crate model being used.
     */
    bool TestIsOpened(uint32 model);

    /**
     * @brief Tests the NI9157Device::Run method.
     * @param model the NI crate model being used.
     */
    bool TestRun(uint32 model);

    /**
     * @brief Tests the NI9157Device::Run method thats fails for a device that
     * can not be found.
     * @param model the NI crate model being used.
     */
    bool TestRun_FalseParams(uint32 model);

    /**
     * @brief Tests the NI9157Device::IsRunning method.
     * @param model the NI crate model being used.
     */
    bool TestIsRunning(uint32 model);

    /**
     * @brief Tests the NI9157Device::GetSession method.
     * @param model the NI crate model being used.
     */
    bool TestGetSession(uint32 model);

    /**
     * @brief Tests the NI9157Device::Reset method.
     * @param model the NI crate model being used.
     */
    bool TestReset(uint32 model);

    /**
     * @brief Tests the NI9157Device::Close methods.
     * @param model the NI crate model being used.
     */
    bool TestClose(uint32 model);

    /**
     * @brief Tests the NI9157Device::CrioStart method.
     * @param model the NI crate model being used.
     */
    bool TestCrioStart(uint32 model);

    /**
     * @brief Tests the NI9157Device::CrioStop method.
     * @param model the NI crate model being used.
     */
    bool TestCrioStop(uint32 model);

    /**
     * @brief Tests the NI9157Device::FindResource method.
     * @param model the NI crate model being used.
     */
    bool TestFindResource(uint32 model);

    /**
     * @brief Tests the NI9157Device::NiWrite method.
     * @param model the NI crate model being used.
     */
    bool TestNiWrite(uint32 model);

    /**
     * @brief Tests the NI9157Device::NiRead method.
     * @param model the NI crate model being used.
     */
    bool TestNiRead(uint32 model);

    /**
     * @brief Tests the NI9157Device::NiConfigureFifo method.
     * @param model the NI crate model being used.
     */
    bool TestNiConfigureFifo(uint32 model);

    /**
     * @brief Tests the NI9157Device::NiStartFifo method.
     * @param model the NI crate model being used.
     */
    bool TestNiStartFifo(uint32 model);

    /**
     * @brief Tests the NI9157Device::NiStopFifo method.
     * @param model the NI crate model being used.
     */
    bool TestNiStopFifo(uint32 model);

    /**
     * @brief Tests the NI9157Device::WriteParam method.
     * @param model the NI crate model being used.
     */
    bool TestWriteParam(uint32 model);

    /**
     * @brief Tests the NI9157Device::ReadParam method.
     * @param model the NI crate model being used.
     */
    bool TestReadParam(uint32 model);

    /**
     * @brief Tests the NI9157Device::NiWriteFifo method with bool input type.
     * @param model the NI crate model being used.
     */
    bool TestNiWriteFifo_Bool(uint32 model);

    /**
     * @brief Tests the NI9157Device::NiReadFifo method with bool input type.
     * @param model the NI crate model being used.
     */
    bool TestNiReadFifo_Bool(uint32 model);

    /**
     * @brief Tests the NI9157Device::NiWriteFifo method with uint8 input type.
     * @param model the NI crate model being used.
     */
    bool TestNiWriteFifo_U8(uint32 model);

    /**
     * @brief Tests the NI9157Device::NiReadFifo method with uint8 input type.
     * @param model the NI crate model being used.
     */
    bool TestNiReadFifo_U8(uint32 model);

    /**
     * @brief Tests the NI9157Device::NiWriteFifo method with uint16 input type.
     * @param model the NI crate model being used.
     */
    bool TestNiWriteFifo_U16(uint32 model);

    /**
     * @brief Tests the NI9157Device::NiReadFifo method with uint16 input type.
     * @param model the NI crate model being used.
     */
    bool TestNiReadFifo_U16(uint32 model);

    /**
     * @brief Tests the NI9157Device::NiWriteFifo method with uint32 input type.
     * @param model the NI crate model being used.
     */
    bool TestNiWriteFifo_U32(uint32 model);

    /**
     * @brief Tests the NI9157Device::NiReadFifo method with uint32 input type.
     * @param model the NI crate model being used.
     */
    bool TestNiReadFifo_U32(uint32 model);

    /**
     * @brief Tests the NI9157Device::NiWriteFifo method with uint64 input type.
     * @param model the NI crate model being used.
     */
    bool TestNiWriteFifo_U64(uint32 model);

    /**
     * @brief Tests the NI9157Device::NiReadFifo method with uint64 input type.
     * @param model the NI crate model being used.
     */
    bool TestNiReadFifo_U64(uint32 model);

    /**
     * @brief Tests the NI9157Device::NiWriteFifo method with int8 input type.
     * @param model the NI crate model being used.
     */
    bool TestNiWriteFifo_I8(uint32 model);

    /**
     * @brief Tests the NI9157Device::NiReadFifo method with int8 input type.
     * @param model the NI crate model being used.
     */
    bool TestNiReadFifo_I8(uint32 model);

    /**
     * @brief Tests the NI9157Device::NiWriteFifo method with int16 input type.
     * @param model the NI crate model being used.
     */
    bool TestNiWriteFifo_I16(uint32 model);

    /**
     * @brief Tests the NI9157Device::NiReadFifo method with int16 input type.
     * @param model the NI crate model being used.
     */
    bool TestNiReadFifo_I16(uint32 model);

    /**
     * @brief Tests the NI9157Device::NiWriteFifo method with int32 input type.
     * @param model the NI crate model being used.
     */
    bool TestNiWriteFifo_I32(uint32 model);

    /**
     * @brief Tests the NI9157Device::NiReadFifo method with int32 input type.
     * @param model the NI crate model being used.
     */
    bool TestNiReadFifo_I32(uint32 model);

    /**
     * @brief Tests the NI9157Device::NiWriteFifo method with int64 input type.
     * @param model the NI crate model being used.
     */
    bool TestNiWriteFifo_I64(uint32 model);

    /**
     * @brief Tests the NI9157Device::NiReadFifo method with int64 input type.
     * @param model the NI crate model being used.
     */
    bool TestNiReadFifo_I64(uint32 model);

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* NI9157DEVICETEST_H_ */
