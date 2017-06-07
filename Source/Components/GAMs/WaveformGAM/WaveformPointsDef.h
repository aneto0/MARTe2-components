/**
 * @file WaveformPointsDef.h
 * @brief Header file for class WaveformPointsDef
 * @date 29/05/2017
 * @author Llorenc
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

 * @details This header file contains the declaration of the class WaveformPointsDef
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef WAVEFORMPOINTSDEFGAM_H_
#define WAVEFORMPOINTSDEFGAM_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "Waveform.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

class WaveformPointsDef: public Waveform {
//TODO Add the macro DLL_API to the class declaration (i.e. class DLL_API WaveformPointsDef)
public:
    CLASS_REGISTER_DECLARATION()

WaveformPointsDef    ();
    virtual ~WaveformPointsDef();

    /**
     * @brief Initialise the GAM from a configuration file.
     * @brief Load the parameters of for the points implementation and verify its correctness and consistency
     * @return true if all parameters are valid
     */
    virtual bool Initialise(StructuredDataI & data);

    /**
     * @brief computes the points signal implementation in uint8.
     * @brief this function calls the template implementation of the points implementation.
     */
    virtual bool GetUInt8Value();

    /**
     * @brief computes the points signal implementation in int8.
     * @brief this function calls the template implementation.
     */
    virtual bool GetInt8Value();

    /**
     * @brief computes the points signal implementation in uint16.
     * @brief this function calls the template implementation.
     */
    virtual bool GetUInt16Value();

    /**
     * @brief computes the points signal implementation in in16.
     * @brief this function calls the template implementation.
     */
    virtual bool GetInt16Value();

    /**
     * @brief computes the points signal implementation in uin32.
     * @brief this function calls the template implementation.
     */
    virtual bool GetUInt32Value();

    /**
     * @brief computes the points signal implementation in int32.
     * @brief this function calls the template implementation.
     */
    virtual bool GetInt32Value();

    /**
     * @brief computes the points signal implementation in uin64.
     * @brief this function calls the template implementation.
     */
    virtual bool GetUInt64Value();

    /**
     * @brief computes the points signal implementation in int64.
     * @brief this function calls the template implementation.
     */
    virtual bool GetInt64Value();

    /**
     * @brief computes the points signal implementation in float32.
     * @brief this function calls the template implementation.
     */
    virtual bool GetFloat32Value();

    /**
     * @brief computes the points signal implementation in float64.
     * @brief this function calls the template implementation.
     */
    virtual bool GetFloat64Value();

    /**
     * @brief computes the points signal implementation in the specified type.
     */
    template<typename T>
    bool GetValue();

    virtual bool GetFloat64OutputValues();

private:
    float64 *points;
    /**
     * times array to indicate at which time there is a new point. The first time must be 0
     */
    float64 *times;
    float64 *slopes;
    uint32 indexSlopes;
    uint32 numberOfPointsElements;
    uint32 numberOfTimesElements;
    uint32 numberOfSlopeElements;
    float64 lastOutputValue;
    bool beginningSequence;

    /**
     * Used to move the pattern for the next iteration
     */
    float64 lastTimeValue;

    /**
     * Is the reference value from which the signal is interpolated. This values is changed for each point.
     */
    float64 refVal;

    float64 timeRefVal;

    bool VerifyTimes () const {
        bool ret = (IsEqual(times[0],0.0));
        for(uint32 i = 0u; (i < (numberOfTimesElements - 1u)) && ret; i ++) {
            //Added due to MISRA:Suspicious Truncation in arithmetic expression combining with pointer
            uint32 aux = i+1u;
            ret = (times[i] < times[aux]);
        }
        return ret;
    }

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* WAVEFORMPOINTSDEFGAM_H_ */

