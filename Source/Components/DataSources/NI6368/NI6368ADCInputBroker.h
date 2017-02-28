/**
 * @file NI6368ADCInputBroker.h
 * @brief Header file for class NI6368ADCInputBroker
 * @date 13/01/2017
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

 * @details This header file contains the declaration of the class NI6368ADCInputBroker
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef NI6368_NI6368ADCINPUTBROKER_H_
#define NI6368_NI6368ADCINPUTBROKER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "MemoryMapStatefulBroker.h"
#include "NI6368ADC.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {

/**
 * @brief An input broker for NI6368 which copies from the latest buffer available.
 * @details The NI6368 DataSourceI copies the data using DMA to one of two buffers.
 * This NI6368ADCInputBroker copies the data back to the GAM memory from the latest
 * complete buffer written by the NI6368ADC.
 */
class DLL_API NI6368ADCInputBroker: public MemoryMapStatefulBroker {
public:
    CLASS_REGISTER_DECLARATION()
    /**
     * @brief Default constructor. NOOP.
     */
    NI6368ADCInputBroker();

    /**
     * @brief Default constructor. Sets a pointer to the NI6368ADC board.
     * @param adcBoard a reference to the NI6368ADC board.
     */
    NI6368ADCInputBroker(NI6368ADC *adcBoardIn);

    /**
     * @brief Destructor. NOOP.
     */
    virtual ~NI6368ADCInputBroker();

    /**
     * @brief Sequentially copies all the signals to the GAM memory from the DataSourceI memory buffer[NI6368::GetLastBufferIdx()].
     * @return true if all copies are successfully performed.
     */
    virtual bool Execute();

private:
    /**
     * Reference to the NI6368ADC board
     */
    NI6368ADC *adcBoard;
};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* NI6368_NI6368ADCINPUTBROKER_H_ */

