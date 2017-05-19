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

    virtual bool Initialise(StructuredDataI & data);

    virtual bool Setup();

    virtual bool Execute();

    virtual bool GetUInt8Value(uint8 *outputValue) = 0;

    virtual bool GetInt8Value(int8 *outputValue) = 0;

    virtual bool GetUInt16Value(uint16 *outputValue) = 0;

    virtual bool GetInt16Value(int16 *outputValue) = 0;

    uint32 GetNumberOfOutputValues();
private:

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* WAVEFORM_H_ */

