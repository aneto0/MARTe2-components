/**
 * @file WaveformSin.h
 * @brief Header file for class WaveformSin
 * @date May 19, 2017
 * @author aneto
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

 * @details This header file contains the declaration of the class WaveformSin
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef WAVEFORMSIN_H_
#define WAVEFORMSIN_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "../WaveformGAM/Waveform.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

//template<class T>
class WaveformSin: public Waveform {
public:
    CLASS_REGISTER_DECLARATION();

    WaveformSin();

    virtual ~WaveformSin();

    virtual bool Initialise(StructuredDataI & data);

//    virtual bool GetValue(T *outputValue);
    virtual bool GetUInt8Value();

    virtual bool GetInt8Value();

    virtual bool GetUInt16Value();

    virtual bool GetInt16Value();

    virtual bool GetUInt32Value();

    virtual bool GetInt32Value();

    virtual bool GetUInt64Value();

    virtual bool GetInt64Value();

    virtual bool GetFloat32Value();

    virtual bool GetFloat64Value();

    /*    virtual bool GetInt8Value(int8 *outputValue);

     virtual bool GetUInt16Value(uint16 *outputValue);

     virtual bool GetInt16Value(int16 *outputValue);

     virtual bool GetUInt32Value(uint32 *outputValue);

     virtual bool GetInt32Value(int32 *outputValue);

     virtual bool GetUInt64Value(uint64 *outputValue);

     virtual bool GetInt64Value(int64 *outputValue);

     virtual bool GetFlaot32Value(float32 *outputValue);

     virtual bool GetFloat64Value(float64 *outputValue);
     */
private:
    float64 amplitude;
    float64 frequency;
    float64 phase;
    float64 offset;
    /*first time received in us */
    uint32 time0;
    /*first time received in us */
    uint32 time1;
    /* diffTime = time1 - time0. It is used to know the sample frequency */
    double timeIncrement;
    /* it is used to determine when to save time0, time1 or when generate the signal */
    uint8 timeState;


};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* WAVEFORMSIN_H_ */

