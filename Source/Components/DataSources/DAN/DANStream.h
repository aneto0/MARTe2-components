/**
 * @file DANStream.h
 * @brief Header file for class DANStream
 * @date 04/04/2017
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

 * @details This header file contains the declaration of the class DANStream
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef DAN_DANSTREAM_H_
#define DAN_DANSTREAM_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {
/**
 * TODO
 */
class DANStream {
public:
    /**
     * TODO
     */
    DANStream(TypeDescriptor tdIn, StreamString baseName, uint32 danBufferMultiplierIn);

    /**
     * TODO
     */
    ~DANStream();

    /**
     * TODO
     */
    TypeDescriptor GetType();

    /**
     * TODO
     */
    void AddSignal(void * const signalIn);

    /**
     * TODO
     */
    void Finalise();

    /**
     * TODO
     */
    bool PutData(uint64 timeStamp);

    /**
     * TODO
     */
    bool OpenStream(float64 samplingRate);

    /**
     * TODO
     */
    bool CloseStream();

private:
    TypeDescriptor td;

    char8 **signalList;

    char8 *blockMemory;

    uint32 nOfSignals;

    uint32 typeSize;

    /**
     * TODO
     */
    dan_Source danSource;

    /**
     * TODO
     */
    StreamString baseName;

    /**
     *
     */
    uint32 danBufferMultiplier;
};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SOURCE_COMPONENTS_DATASOURCES_DAN_DANDATABLOCK_H_ */

