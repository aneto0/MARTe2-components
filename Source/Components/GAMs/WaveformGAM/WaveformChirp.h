/**
 * @file WaveformChirp.h
 * @brief Header file for class WaveformChirp
 * @date 02/06/2017
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

 * @details This header file contains the declaration of the class WaveformChirp
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef WAVEFORMCHIRP_H_
#define WAVEFORMCHIRP_H_

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

class WaveformChirp:public Waveform {
//TODO Add the macro DLL_API to the class declaration (i.e. class DLL_API WaveformChirp)
public:
    CLASS_REGISTER_DECLARATION()
    WaveformChirp();
    virtual ~WaveformChirp();

    virtual bool Initialise(StructuredDataI &data);

    /**
      * @brief computes the Sin in uint8.
      * @brief this function calls the template implementation of the sin
      */
     virtual bool GetUInt8Value();

     /**
      * @brief computes the Sin in int8.
      * @brief this function calls the template implementation of the sin
      */
     virtual bool GetInt8Value();

     /**
      * @brief computes the Sin in uint16.
      * @brief this function calls the template implementation of the sin
      */
     virtual bool GetUInt16Value();

     /**
      * @brief computes the Sin in in16.
      * @brief this function calls the template implementation of the sin
      */
     virtual bool GetInt16Value();

     /**
      * @brief computes the Sin in uin32.
      * @brief this function calls the template implementation of the sin
      */
     virtual bool GetUInt32Value();

     /**
      * @brief computes the Sin in int32.
      * @brief this function calls the template implementation of the sin
      */
     virtual bool GetInt32Value();

     /**
      * @brief computes the Sin in uin64.
      * @brief this function calls the template implementation of the sin
      */
     virtual bool GetUInt64Value();

     /**
      * @brief computes the Sin in int64.
      * @brief this function calls the template implementation of the sin
      */
     virtual bool GetInt64Value();

     /**
      * @brief computes the Sin in float32.
      * @brief this function calls the template implementation of the sin
      */
     virtual bool GetFloat32Value();

     /**
      * @brief computes the Sin in float64.
      * @brief this function calls the template implementation of the sin
      */
     virtual bool GetFloat64Value();

     /**
      * @brief computes the Sin in the specified type.
      */
     template<typename T>
     bool GetValue();

     virtual bool GetFloat64OutputValues();

private:
     float64 amplitude;
     float64 phase;
     float64 offset;
     float64 frequency1;
     float64 frequency2;
     float64 w1;
     float64 w2;
     //w2-w1
     float64 w12;
     float64 chirpDuration;
     //2*chirpDuration
     float64 cD2;
};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* WAVEFORMCHIRP_H_ */

