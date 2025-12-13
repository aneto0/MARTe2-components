/**
 * @file DANStreamByReferenceTest.h
 * @brief Header file for class DANStreamByReferenceTest
 * @date 18/11/2025
 * @author Kevin Meyer
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

#ifndef DAN_DANBYREFERENCESTREAMTEST_H_
#define DAN_DANBYREFERENCESTREAMTEST_H_

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
 * @brief Tests the DANStreamByReferenceTest public methods.
 * @details Note that these tests require:
 * - The DANArchiver to be running: dan_archiver_writer start
 * - The DAN api has been loaded: /opt/codac/bin/danApiTool api init $MARTe2_Components_DIR/Test/Components/DataSources/DAN/dan_test_config.xml
 */

class DANStreamByReferenceTest {
public:
    /**
     * @brief Tests the constructor.
     */
    bool TestConstructor();
    /**
     * @brief Tests the InitializePublishSource method.
     */

    bool TestInitializePublishSource();
    /**
     * @brief Tests the OpenStream method.
     */
    bool TestOpenStream();

    /**
     * @brief Tests the CloseStream method.
     */
    bool TestCloseStream();

    /**
     * @brief Tests the PutData method with float32.
     */
    bool TestPutData_Float32();

    /**
     * @brief Tests the PutData method with float32, with flush.
     */
    bool TestPutData_Float32Flush();

    /**
     * @brief Tests the SetAbsoluteTimeSignal method.
     */
    bool TestSetAbsoluteTimeSignal();

    /**
     * @brief Tests the SetRelativeTimeSignal method.
     */
    bool TestSetRelativeTimeSignal();

    /**
     * @brief Tests PutData with no open stream.
     */
    bool TestPutWithNoOpenOK();

    /**
     * @brief Tests CloseStream on closed stream.
     */
    bool TestDoubleCloseOK();
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */

/*---------------------------------------------------------------------------*/

#endif /* DAN_DANBYREFERENCESTREAMTEST_H_ */

