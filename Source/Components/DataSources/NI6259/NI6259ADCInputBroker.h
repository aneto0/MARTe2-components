/**
 * @file NI6259ADCInputBroker.h
 * @brief Header file for class NI6259ADCInputBroker
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

 * @details This header file contains the declaration of the class NI6259ADCInputBroker
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef NI6259_NI6259ADCINPUTBROKER_H_
#define NI6259_NI6259ADCINPUTBROKER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "MemoryMapStatefulBroker.h"
#include "NI6259ADC.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {
/**
 * @brief Helper structure which holds the memory pointers of the GAM and DataSource elements
 * that are to be copied by this MemoryMapStatefulBroker. Note that the dataSourcePointer will
 * hold the address of variable which has the pointer to the memory (and not the memory address itself).
 */
struct NI6259CopyTableEntry {
    /**
     * The pointer to the base address of the GAM.
     */
    void *gamPointer;
    /**
     * The array with the two datasource indirections.
     */
    void *dataSourcePointer[NUMBER_OF_BUFFERS];
    /**
     * The data source offset.
     */
    uint32 dataSourceOffset;
    /**
     * The size of the copy
     */
    uint32 copySize;
};
/**
 * @brief An input broker for NI6259 which copies from the latest buffer available.
 * @details The NI6259 DataSourceI copies the data using DMA to one of two buffers.
 * This NI6259ADCInputBroker copies the data back to the GAM memory from the latest
 * complete buffer written by the NI6259ADC.
 */
class DLL_API NI6259ADCInputBroker: public BrokerI {
public:
    CLASS_REGISTER_DECLARATION()
    /**
     * @brief Default constructor. NOOP.
     */
NI6259ADCInputBroker    ();

    /**
     * @brief Default constructor. Sets a pointer to the NI6259ADC board.
     * @param adcBoard a reference to the NI6259ADC board.
     */
    NI6259ADCInputBroker(NI6259ADC *adcBoardIn);

    /**
     * @brief Destructor. NOOP.
     */
    virtual ~NI6259ADCInputBroker();

    /**
     * @brief TODO.
     */
    virtual bool Init(const SignalDirection direction,
            DataSourceI &dataSourceIn,
            const char8 * const functionName,
            void * const gamMemoryAddress);

    /**
     * @brief Sequentially copies all the signals to the GAM memory from the DataSourceI memory buffer[NI6259::GetLastBufferIdx()].
     * @return true if all copies are successfully performed.
     */
    virtual bool Execute();

private:
    /**
     * Reference to the NI6259ADC board
     */
    NI6259ADC *adcBoard;


    /**
     * A table with all the elements to be copied
     */
    NI6259CopyTableEntry *copyTable;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* NI6259_NI6259ADCINPUTBROKER_H_ */

