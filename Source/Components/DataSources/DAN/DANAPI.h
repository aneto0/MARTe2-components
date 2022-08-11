/**
 * @file DANAPI.h
 * @brief Header file for class DANAPI
 * @date 20/04/2017
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

 * @details This header file contains the declaration of the class DANAPI
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef DAN_DANAPI_H_
#define DAN_DANAPI_H_

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
 * @brief Wrapper to the DAN library in order to able to lint the DANSource and DANStream classes. The inclusion
 *  of the dan/dan_Source.h and dan/dan_DataCore.h has many hidden dependencies that are putting lint into trouble.
 */
/*lint -estring(526, "*DANAPI::*") cannot include DANAPI.cpp on the files to be linted as otherwise it would go back to the original issue.*/
namespace DANAPI {

/**
 * @brief see dan_initLibrary
 */
bool InitLibrary();

/**
 * @brief see dan_initLibrary_icprog
 */
bool InitLibraryICProg(const char8 * const progName);

/**
 * @brief see dan_closeLibrary
 */
void CloseLibrary();

/**
 * @brief see dan_publisher_putDataBlock
 */
bool PutDataBlock(void * danSource, uint64 timeStamp, char8 *blockInterleavedMemory, uint32 blockSize);

/**
 * @brief see dan_publisher_openStream
 */
bool OpenStream(void * danSource, float64 samplingFrequency);

/**
 * @brief see dan_publisher_closeStream
 */
bool CloseStream(void * danSource);

/**
 * @brief see dan_publisher_publishSource_withDAQBuffer
 */
void *PublishSource(const char8 * const sourceName, uint64 bufferSize);

/**
 * @brief see dan_publisher_unpublishSource
 */
void UnpublishSource(void * danSource);

}
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* DAN_DANAPI_H_ */

