/**
 * @file DANStreamTest.h
 * @brief Header file for class DANStreamTest
 * @date 11/04/2017
 * @author Andre Neto
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

 * @details This header file contains the declaration of the class DANStreamTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef DAN_DANSTREAMTEST_H_
#define DAN_DANSTREAMTEST_H_

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
 * @brief Tests the DANStream public methods.
 * @details Note that these tests require:
 * - The DANArchiver to be running: dan_archiver_writer start
 * - The DAN api has been loaded: /opt/codac/bin/danApiTool api init $MARTe2_Components_DIR/Test/Components/DataSources/DAN/dan_test_config.xml
 */
class DANStreamTest {
public:
    /**
     * @brief Tests the constructor.
     */
    bool TestConstructor();

    /**
     * @brief Tests the GetType method.
     */
    bool TestGetType();

    /**
     * @brief Tests the GetSamplingFrequency method.
     */
    bool TestGetSamplingFrequency();

    /**
     * @brief Tests the GetDANBufferMultiplier method.
     */
    bool TestGetDANBufferMultiplier();

    /**
     * @brief Tests the GetNumberOfSamples method.
     */
    bool TestGetNumberOfSamples();

    /**
     * @brief Tests the AddSignal method.
     */
    bool TestAddSignal();

    /**
     * @brief Tests the Finalise method.
     */
    bool TestFinalise();

    /**
     * @brief Tests the PutData method with uint16.
     */
    bool TestPutData_UInt16();

    /**
     * @brief Tests the PutData method with int16.
     */
    bool TestPutData_Int16();

    /**
     * @brief Tests the PutData method with uint32.
     */
    bool TestPutData_UInt32();

    /**
     * @brief Tests the PutData method with int32.
     */
    bool TestPutData_Int32();

    /**
     * @brief Tests the PutData method with uint64.
     */
    bool TestPutData_UInt64();

    /**
     * @brief Tests the PutData method with int64.
     */
    bool TestPutData_Int64();

    /**
     * @brief Tests the PutData method with float32.
     */
    bool TestPutData_Float32();

    /**
     * @brief Tests the PutData method with float64.
     */
    bool TestPutData_Float64();

    /**
     * @brief Tests the OpenStream method.
     */
    bool TestOpenStream();

    /**
     * @brief Tests that the OpenStream method fails if no danSource is specified.
     */
    bool TestOpenStream_NoFinalise();

    /**
     * @brief Tests the CloseStream method.
     */
    bool TestCloseStream();

    /**
     * @brief Tests the GetSignalMemoryBuffer method.
     */
    bool TestGetSignalMemoryBuffer();

    /**
     * @brief Tests the Reset method.
     */
    bool TestReset();

    /**
     * @brief Tests the SetAbsoluteTimeSignal method.
     */
    bool TestSetAbsoluteTimeSignal();

    /**
     * @brief Tests the SetRelativeTimeSignal method.
     */
    bool TestSetRelativeTimeSignal();

    /**
     * @brief Tests the SetAbsoluteStartTime method.
     */
    bool TestSetAbsoluteStartTime();

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */

/*---------------------------------------------------------------------------*/

#endif /* DAN_DANSTREAMTEST_H_ */

