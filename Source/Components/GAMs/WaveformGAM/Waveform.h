/**
 * @file Waveform.h
 * @brief Header file for class Waveform
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

 * @details This header file contains the declaration of the class Waveform
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef WAVEFORMGENERATOR_WAVEFORM_H_
#define WAVEFORMGENERATOR_WAVEFORM_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "GAM.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {

class Waveform: public GAM {
public:

    Waveform();

    virtual ~Waveform();

//    virtual bool Initialise(StructuredDataI & data);

    virtual bool Setup();

    virtual bool Execute();

    virtual bool GetUInt8Value() = 0;

    virtual bool GetInt8Value() = 0;

    virtual bool GetUInt16Value() = 0;

    virtual bool GetInt16Value() = 0;

    virtual bool GetUInt32Value() = 0;

    virtual bool GetInt32Value() = 0;

    virtual bool GetUInt64Value() = 0;

    virtual bool GetInt64Value() = 0;

    virtual bool GetFloat32Value() = 0;

    virtual bool GetFloat64Value() = 0;

/*    virtual bool GetInt8Value() = 0;

    virtual bool GetUInt16Value() = 0;

    uint32 GetNumberOfOutputValues();
*/
protected:
    uint32 *inputTime;
    void **outputValue;
    uint32 numberOfOutputElements;
    uint32 numberOfOutputSignals;

private:
    TypeDescriptor typeVariableIn;
    TypeDescriptor typeVariableOut;
    uint32 numberOfInputSignals;
    uint32 numberOfInputElements;
    uint32 numberOfInputSamples;
    uint32 numberOfOutputSamples;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* WAVEFORM_H_ */

