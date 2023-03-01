/**
 * @file UEIDefinitions.h
 * @brief Header file for class UEIDefinitions
 * @date 09/02/2023
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
#define MAX_IO_SLOTS    12u   //Maximum number of layers across the UEIDAQ chassis catalogue

//Channel direction definition
#define OUTPUT_CHANNEL  0u
#define INPUT_CHANNEL   1u

//Map definitions
#define NOMAP           0u
#define RTDMAP          1u
#define RTVMAP          2u
#define ADMAP           3u
#define AVMAP           4u

//Signal Types definitions
#define  ANALOG_SIGNAL  1u //The output of this signal is either 
#define DIGITAL_SIGNAL  2u

//Hardware layer type definitions
#define HARDWARE_LAYER_UNDEFINED    0u
#define HARDWARE_LAYER_ANALOG_I     1u
#define HARDWARE_LAYER_ANALOG_O     2u
#define HARDWARE_LAYER_ANALOG_IO    3u
#define HARDWARE_LAYER_DIGITAL_IO   4u
#define HARDWARE_LAYER_MESSAGING    5u

#endif /*UEIDefinitions_H_*/