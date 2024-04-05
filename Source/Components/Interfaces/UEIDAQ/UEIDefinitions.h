/**
 * @file UEIDefinitions.h
 * @brief Header file for class UEIDefinitions
 * @date 22/03/2023
 * @author Xavier Ruche
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

 * @details This header file contains the declaration of the class UEIDefinitions
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef UEIDefinitions_H_
#define UEIDefinitions_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

//Chassis definitions
#define MAX_IO_SLOTS    12   //Maximum number of layers across the UEIDAQ chassis catalogue

//Map definitions
enum MapType{
    NO_MAP,
    RTDMAP,
    RTVMAP,
    ADMAP,
    AVMAP
};

//Signal Types definitions
enum SignalType{
    NO_SIGNAL,
    ANALOG_SIGNAL,
    DIGITAL_SIGNAL
};

//Hardware layer type definitions
enum IOLayerType {
    NO_LAYER,
    HARDWARE_LAYER_ANALOG_I,
    HARDWARE_LAYER_ANALOG_O,
    HARDWARE_LAYER_ANALOG_IO,
    HARDWARE_LAYER_DIGITAL_IO,
    HARDWARE_LAYER_MESSAGING
};

//MapReturnCodes
enum MapReturnCode {
    ERROR,
    NEW_DATA_AVAILABLE,
    NO_NEW_DATA_AVAILABLE,
    FIFO_OVERFLOW
};
#endif /*UEIDefinitions_H_*/