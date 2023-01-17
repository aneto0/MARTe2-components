/**
 * @file MDSReaderTestHelper.h
 * @brief Header file for class MDSReaderTestHelper
 * @date 16/01/2023
 * @author Llorenc Capella
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

 * @details This header file contains the declaration of the class MDSReaderTestHelper
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef MDSREADERTESTHELPER_H_
#define MDSREADERTESTHELPER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "MDSReader.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {
class MDSReaderTestHelper: public MDSReader {
public:
    MDSReaderTestHelper(StreamString name);

    virtual ~MDSReaderTestHelper();

    bool CreateConfigurationFile(float64 samplingTime = 0.1,
                                 uint8 dataManagement = 0u,
                                 uint8 holeManagement = 0u,
                                 int32 shot = 1,
                                 StreamString typeTime = "uint32");

    bool CompareS_uint8Hold(uint8 *ptr,
                            uint32 iteration,
                            float64 increment = +1,
                            uint32 every = 32,
                            uint32 elementsToCompare = 10u);

    bool CompareS_int8Hold(int8 *ptr,
                           uint32 iteration,
                           float64 increment = +1,
                           uint32 every = 32,
                           uint32 elementsToCompare = 10u);

    bool CompareS_uint16Hold(uint16 *ptr,
                             uint32 iteration,
                             float64 increment = +1,
                             uint32 every = 32,
                             uint32 elementsToCompare = 10u);

    bool CompareS_int16Hold(int16 *ptr,
                            uint32 iteration,
                            float64 increment = +1,
                            uint32 every = 32,
                            uint32 elementsToCompare = 10u);

    bool CompareS_uint32Hold(uint32 *ptr,
                             uint32 iteration,
                             float64 increment = +1,
                             uint32 every = 32,
                             uint32 elementsToCompare = 10u);

    bool CompareS_int32Hold(int32 *ptr,
                            uint32 iteration,
                            float64 increment = +1,
                            uint32 every = 32,
                            uint32 elementsToCompare = 10u);

    bool CompareS_uint64Hold(uint64 *ptr,
                             uint32 iteration,
                             float64 increment = +1,
                             uint32 every = 32,
                             uint32 elementsToCompare = 10u);

    bool CompareS_int64Hold(int64 *ptr,
                            uint32 iteration,
                            float64 increment = +1,
                            uint32 every = 32,
                            uint32 elementsToCompare = 10u);

    bool CompareS_float32Hold(float32 *ptr,
                              uint32 iteration,
                              float64 increment = +1,
                              uint32 every = 32,
                              uint32 elementsToCompare = 10u);

    bool CompareS_float64Hold(float64 *ptr,
                              uint32 iteration,
                              float64 increment = +1,
                              uint32 every = 32,
                              uint32 elementsToCompare = 10u);

    bool CompareS_uint8(uint8 *ptr,
                        uint32 iteration,
                        float64 increment = +1,
                        uint32 elementsToCompare = 10u);

    bool CompareS_int8(int8 *ptr,
                       uint32 iteration,
                       float64 increment = +1,
                       uint32 elementsToCompare = 0u);

    bool CompareS_uint16(uint16 * ptr,
                         uint32 iteration,
                         float64 increment = +2,
                         uint32 elementsToCompare = 0u);

    bool CompareS_int16(int16 * ptr,
                        uint32 iteration,
                        float64 increment = +2,
                        uint32 elementsToCompare = 0u);

    bool CompareS_uint32(uint32 * ptr,
                         uint32 iteration,
                         float64 increment = +4,
                         uint32 elementsToCompare = 0u);

    bool CompareS_int32(int32 * ptr,
                        uint32 iteration,
                        float64 increment = +4,
                        uint32 elementsToCompare = 0u);

    bool CompareS_uint64(uint64 * ptr,
                         uint32 iteration,
                         float64 increment = +8,
                         uint32 elementsToCompare = 0u);

    bool CompareS_int64(int64 * ptr,
                        uint32 iteration,
                        float64 increment = +8,
                        uint32 elementsToCompare = 0u);

    bool CompareS_float32(float32 * ptr,
                          uint32 iteration,
                          float64 increment = 0.1,
                          uint32 elementsToCompare = 0u);

    bool CompareS_float64(float64 * ptr,
                          uint32 iteration,
                          float64 increment = 0.1,
                          uint32 elementsToCompare = 0u);

    template<typename T>
    bool CompareIntegerValue(T *ptr,
                             uint32 iteration,
                             T value,
                             uint32 elementsToCompare);

    template<typename T>
    bool CompareFloatPointValue(T *ptr,
                                uint32 iteration,
                                T value,
                                uint32 elementsToCompare);

    bool IsEqualLargeMargins(float32 f1,
                             float32 f2);

    bool IsEqualLargeMargins(float64 f1,
                             float64 f2);
    ConfigurationDatabase config;
    uint32 elementsRead;

private:
    StreamString treeName;
    float64 sum_uint8;
    float64 sum_int8;
    float64 sum_uint16;
    float64 sum_int16;
    float64 sum_uint32;
    float64 sum_int32;
    float64 sum_uint64;
    float64 sum_int64;
    float64 sum_float32;
    float64 sum_float64;
}
;

template<typename T>
bool MDSReaderTestHelper::CompareIntegerValue(T *ptr,
                                              uint32 iteration,
                                              T value,
                                              uint32 elementsToCompare) {
    bool ok = true;
    for (uint32 i = 0u; (i < elementsToCompare) && ok; i++) {
        ok = (ptr[i] == value);
        if (!ok) {
            printf("iteration = %u, position in the iteration = %u\n", iteration, i);
        }
    }
    return ok;
}

template<typename T>
bool MDSReaderTestHelper::CompareFloatPointValue(T *ptr,
                                                 uint32 iteration,
                                                 T value,
                                                 uint32 elementsToCompare) {
    bool ok = true;
    for (uint32 i = 0u; (i < elementsToCompare) && ok; i++) {
        ok = IsEqualLargeMargins(ptr[i], value);

        if (!ok) {
            printf("iteration = %u, position in the iteration = %u\n", iteration, i);
        }
    }
    return ok;
}

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* MDSREADERTESTHELPER_H_ */

