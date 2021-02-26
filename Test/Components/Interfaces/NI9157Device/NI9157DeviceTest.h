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
 * @details This header file contains the declaration of the class NI9157DeviceTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef NI9157DEVICETEST_H_
#define NI9157DEVICETEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "NI9157Device.h"

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
     * @brief Tests the NI9157Device::Initialise method that fails if NiRioDeviceName is not specified.
     * @param model the NI crate model being used.
     */
    bool TestInitialise_FalseNoDeviceName(uint32 model);

    /**
     * @brief Tests the NI9157Device::Initialise method that fails if NiRioGenFile is not specified.
     * @param model the NI crate model being used.
     */
    bool TestInitialise_FalseNoGenFile(uint32 model);

    /**
     * @brief Tests the NI9157Device::Initialise method that fails if NiRioGenSignature is not specified.
     * @param model the NI crate model being used.
     */
    bool TestInitialise_FalseNoGenSignature(uint32 model);

    /**
     * @brief Tests the NI9157Device::Initialise method that fails if the type id is not found in the variable
     * to be initialised.
     * @param model the NI crate model being used.
     */
    bool TestInitialise_FalseNoType(uint32 model);

    /**
     * @brief Tests the NI9157Device::Initialise method that fails if the type id is not found in the variable
     * is not found in the Labview project.
     * @param model the NI crate model being used.
     */
    bool TestInitialise_FalseVariableNotFound(uint32 model);

    /**
     * @brief Tests the NI9157Device::Open and NI9157Device::IsOpened
     * methods.
     * @param model the NI crate model being used.
     */
    bool TestOpenIsOpened(uint32 model);

    /**
     * @brief Tests the NI9157Device::Run  and NI9157Device::IsRunning
     * methods.
     * @param model the NI crate model being used.
     */
    bool TestRunIsRunning(uint32 model);

    /**
     * @brief Tests the NI9157Device::GetSession, NI9157Device::Reset
     * and NI9157Device::Close methods.
     * @param model the NI crate model being used.
     */
    bool TestGetSessionResetClose(uint32 model);

    /**
     * @brief Tests the NI9157Device::CrioStart and NI9157Device::CrioStop methods.
     * @param model the NI crate model being used.
     */
    bool TestCrioStartStop(uint32 model);

    /**
     * @brief Tests the NI9157Device::FindResource method.
     * @param model the NI crate model being used.
     */
    bool TestFindResource(uint32 model);

    /**
     * @brief Tests the NI9157Device::NiWrite and NI9157Device::NiRead methods.
     * @param model the NI crate model being used.
     */
    bool TestNiWriteRead(uint32 model);

    /**
     * @brief Tests the NI9157Device::NiConfigureFifo and NI9157Device::NiReleaseFifo
     * methods.
     * @param model the NI crate model being used.
     */
    bool TestNiConfigureReleaseFifo(uint32 model);

    /**
     * @brief Tests the NI9157Device::NiStartFifo and NI9157Device::NiStopFifo
     * methods.
     * @param model the NI crate model being used.
     */
    bool TestNiStartStopFifo(uint32 model);

    /**
     * @brief Tests the NI9157Device::WriteParam and NI9157Device::ReadParam
     * methods.
     * @param model the NI crate model being used.
     */
    bool TestWriteReadParam(uint32 model);

    /**
     * @brief Tests the NI9157Device::NiWriteFifo and NI9157Device::NiReadFifo
     * methods with bool input type.
     * @param model the NI crate model being used.
     */
    bool TestNiWriteReadFifo_Bool(uint32 model);

    /**
     * @brief Tests the NI9157Device::NiWriteFifo and NI9157Device::NiReadFifo
     * methods with uint8 input type.
     * @param model the NI crate model being used.
     */
    bool TestNiWriteReadFifo_U8(uint32 model);

    /**
     * @brief Tests the NI9157Device::NiWriteFifo and NI9157Device::NiReadFifo
     * methods with uint16 input type.
     * @param model the NI crate model being used.
     */
    bool TestNiWriteReadFifo_U16(uint32 model);

    /**
     * @brief Tests the NI9157Device::NiWriteFifo and NI9157Device::NiReadFifo
     * methods with uint32 input type.
     * @param model the NI crate model being used.
     */
    bool TestNiWriteReadFifo_U32(uint32 model);

    /**
     * @brief Tests the NI9157Device::NiWriteFifo and NI9157Device::NiReadFifo
     * methods with uint64 input type.
     * @param model the NI crate model being used.
     */
    bool TestNiWriteReadFifo_U64(uint32 model);

    /**
     * @brief Tests the NI9157Device::NiWriteFifo and NI9157Device::NiReadFifo
     * methods with int8 input type.
     * @param model the NI crate model being used.
     */
    bool TestNiWriteReadFifo_I8(uint32 model);

    /**
     * @brief Tests the NI9157Device::NiWriteFifo and NI9157Device::NiReadFifo
     * methods with int16 input type.
     * @param model the NI crate model being used.
     */
    bool TestNiWriteReadFifo_I16(uint32 model);

    /**
     * @brief Tests the NI9157Device::NiWriteFifo and NI9157Device::NiReadFifo
     * methods with int32 input type.
     * @param model the NI crate model being used.
     */
    bool TestNiWriteReadFifo_I32(uint32 model);

    /**
     * @brief Tests the NI9157Device::NiWriteFifo and NI9157Device::NiReadFifo
     * methods with int64 input type.
     * @param model the NI crate model being used.
     */
    bool TestNiWriteReadFifo_I64(uint32 model);

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* NI9157DEVICETEST_H_ */
