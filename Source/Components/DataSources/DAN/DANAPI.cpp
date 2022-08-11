/**
 * @file DANAPI.cpp
 * @brief Source file for class DANAPI
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

 * @details This source file contains the definition of all the methods for
 * the class DANAPI (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <dan/dan_DataCore.h>
#include <dan/dan_Source.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "CompilerTypes.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {
namespace DANAPI {
/**
 * dan_DataCore singleton interface.
 */
dan_DataCore danDataCore = NULL_PTR(dan_DataCore);
}
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

namespace DANAPI {
bool InitLibrary() {
    bool ok = true;
    if (danDataCore == NULL_PTR(dan_DataCore)) {
        danDataCore = dan_initLibrary();
        ok = (danDataCore != NULL_PTR(dan_DataCore));
    }
    return ok;
}

bool InitLibraryICProg(const char8 * const progName) {
    bool ok = true;
    if (danDataCore == NULL_PTR(dan_DataCore)) {
#ifdef CCS_LT_60
        ok = (danDataCore != NULL_PTR(dan_DataCore));
#else
        danDataCore = dan_initLibrary_icprog(progName); // CCSv6.0 and above
#endif
        ok = (danDataCore != NULL_PTR(dan_DataCore));
    }
    return ok;
}

void CloseLibrary() {
    if (danDataCore != NULL_PTR(dan_DataCore)) {
        dan_closeLibrary(danDataCore);
        danDataCore = NULL_PTR(dan_DataCore);
    }
}

bool PutDataBlock(void * danSource, uint64 timeStamp, char8 *blockInterleavedMemory, uint32 blockSize) {
    return (dan_publisher_putDataBlock(reinterpret_cast<dan_Source>(danSource), timeStamp, blockInterleavedMemory, static_cast<ssize_t>(blockSize), NULL_PTR(char8 *)) >= 0);
}

bool OpenStream(void * danSource, float64 samplingFrequency) {
    return (dan_publisher_openStream(reinterpret_cast<dan_Source>(danSource), samplingFrequency, static_cast<ssize_t>(0)) == 0);
}

bool CloseStream(void * danSource) {
    return (dan_publisher_closeStream(reinterpret_cast<dan_Source>(danSource)) == 0);
}

void * PublishSource(const char8 * const sourceName, uint64 bufferSize) {
    return dan_publisher_publishSource_withDAQBuffer(danDataCore, sourceName, bufferSize);
}

void UnpublishSource(void *danSource) {
    if ((danDataCore != NULL_PTR(dan_DataCore)) && (danSource != NULL_PTR(void *))) {
        // DAN v3.1 has replaced dan_publisher_unpublishSource with a macro which follows
        // an expression such as s = new_method (d,s) which imposes the second parameter
        // passed to dan_publisher_unpublishSource to be a valid lvalue argument.
        dan_Source lvSource = reinterpret_cast<dan_Source>(danSource);
        dan_publisher_unpublishSource(danDataCore, lvSource);
    }
}
}
}

