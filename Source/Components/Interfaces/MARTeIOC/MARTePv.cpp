/**
 * @file MARTePv.cpp
 * @brief Source file for class MARTePv
 * @date 01/07/2019
 * @author Giuseppe Ferro
 *
 * @copyright Copyright 2015 F4E | European Joint Undertaking for ITER and
 * the Development of Fusion Energy ('Fusion for Energy').
 * Licensed under the EUPL, Version 1.1 or - as soon they will be approved
 * by the European Commission - subsequent versions of the EUPL (the "Licence")
 * You may not use this work except in compliance with the Licence.
 * You may obtain a copy of the Licence at: http://ec.europa.eu/idabc/eupl
 *`
 * @warning Unless required by applicable law or agreed to in writing, 
 * software distributed under the Licence is distributed on an "AS IS"
 * basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
 * or implied. See the Licence permissions and limitations under the Licence.

 * @details This source file contains the definition of all the methods for
 * the class MARTePv (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

#include "dbMapper.h"

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "MARTePv.h"
#include "MARTeChannel.h"
#include "AdvancedErrorManagement.h"
#include "MARTeIocServer.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {
//#define NULL_PTR(x) NULL

const uint32 DEFAULT_EPICS_LEN = 1u;
const float32 DEFAULT_EPICS_SCAN = 0.F;

static const char8 * aitEnum_strings[] = { "aitEnumInvalid", "aitEnumInt8", "aitEnumUint8", "aitEnumInt16", "aitEnumUint16", "aitEnumEnum16", "aitEnumInt32",
        "aitEnumUint32", "aitEnumFloat32", "aitEnumFloat64", "aitEnumFixedString", "aitEnumString", "aitEnumContainer", NULL_PTR(const char8 *) };

static aitEnum aitTypes[] = { aitEnumInvalid, aitEnumInt8, aitEnumUint8, aitEnumInt16, aitEnumUint16, aitEnumEnum16, aitEnumInt32, aitEnumUint32,
        aitEnumFloat32, aitEnumFloat64, aitEnumFixedString, aitEnumString, aitEnumContainer, aitEnumInvalid };

static TypeDescriptor typeDescriptors[] = { InvalidType, SignedInteger8Bit, UnsignedInteger8Bit, SignedInteger16Bit, UnsignedInteger16Bit, Character8Bit,
        SignedInteger32Bit, UnsignedInteger32Bit, Float32Bit, Float64Bit, Character8Bit, Character8Bit, StructuredDataInterfaceType, InvalidType };

class exStringDestructor: public gddDestructor {
public:
    exStringDestructor();

    virtual ~exStringDestructor();
    /*lint -e{952} not declared as const to match the external lib prototype*/
    virtual void run(void * thing_to_remove);
};

exStringDestructor::exStringDestructor() :
        gddDestructor() {

}

exStringDestructor::~exStringDestructor() {

}
/*lint -e{952} not declared as const to match the external lib prototype*/
void exStringDestructor::run(void * thing_to_remove) {
    aitString *ps = reinterpret_cast<aitString *>(thing_to_remove);
    delete[] ps;
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

MARTePv::MARTePv() :
        /*lint -e{1069} casPV is a base of MARTePV*/
        casPV(),
        tsSLNode<MARTePv>(),
        ReferenceContainer()
/*lint -e{1928} casPV appears in the constructor initialiser list*/
{
    interest = false;
    pvScanPeriod = 0.;

    myType = aitEnumInvalid;

    elementCount = 1u;
    cas = NULL_PTR(MARTeIocServer *);
    timer = NULL_PTR(epicsTimer *);

    useLocalTs = true;

    currentTimestamp = epicsTime::getCurrent();
}

/*lint -e{1551} -e{1510} no exception is thrown*/
MARTePv::~MARTePv() {
    if (timer != NULL_PTR(epicsTimer *)) {
        timer->destroy();
        timer = NULL_PTR(epicsTimer *);
    }
    void *ptr = pValue.GetDataPointer();
    if (ptr != NULL) {
        (void) HeapManager::Free(ptr);
        pValue = voidAnyType;
    }
    cas = NULL_PTR(MARTeIocServer *);
}

void MARTePv::destroy() {

}

bool MARTePv::Initialise(StructuredDataI &data) {
    bool ret = ReferenceContainer::Initialise(data);
    if (ret) {
        StreamString pvType;
        ret = (data.Read("TYPE", pvType));
        if (ret) {
            myType = ConvertToaitEnum(pvType.Buffer());
        }
        else {
            REPORT_ERROR(ErrorManagement::InitialisationError, "PV Type not specified!");
        }

        if (ret) {
            uint8 useLocalTsT = 0u;
            if (!data.Read("UseLocalTimeStamp", useLocalTsT)) {
                useLocalTsT = 1u;
            }
            useLocalTs = (useLocalTsT > 0u) ? (true) : (false);
            if (!data.Read("NELM", elementCount)) {
                elementCount = DEFAULT_EPICS_LEN;
            }
            if (!data.Read("SCAN", pvScanPeriod)) {
                pvScanPeriod = DEFAULT_EPICS_SCAN;
            }
        }
        if (ret) {
            InitPv();
        }
    }
    return ret;
}

void MARTePv::SetIocServer(MARTeIocServer &casIn) {
    cas = &casIn;
    uint32 size = Size();
    for (uint32 i = 0u; i < size; i++) {
        ReferenceT<MARTePv> ref = Get(i);
        if (ref.IsValid()) {
            ref->SetIocServer(casIn);
        }
    }
}

caStatus MARTePv::interestRegister() {

    /*lint -e{9130} depends on external library*/
    caStatus status = static_cast<caStatus>(S_casApp_undefined);
    if (cas != NULL_PTR(MARTeIocServer *)) {
        interest = true;
        if (pvScanPeriod > 0.F) {
            if (timer == NULL) {
                timer = &cas->createTimer();
            }
            timer->start(*this, pvScanPeriod);
        }
        status = static_cast<caStatus>(S_casApp_success);
    }
    return status;
}

void MARTePv::interestDelete() {
    interest = false;
    if (timer != NULL_PTR(epicsTimer *)) {
        timer->cancel();
    }
}

aitEnum MARTePv::bestExternalType() const {
    return myType;
}

const char8 * MARTePv::getName() const {
    return Object::GetName();
}

/*lint -e{715} pHostName and pUserName are not needed*/
casChannel *MARTePv::createChannel(const casCtx &ctx,
                                   const char8 * const pUserName,
                                   const char8 * const pHostName) {
    return new MARTeChannel(ctx);
}

/*lint -e{715} currentTimeIn is not nedded*/
epicsTimerNotify::expireStatus MARTePv::expire(const epicsTime &) {
    epicsTimerNotify::expireStatus ret = noRestart;
    scan();
    if (pvScanPeriod > 0.) {
        ret = expireStatus(restart, pvScanPeriod);
    }
    return ret;
}

aitEnum MARTePv::ConvertToaitEnum(const char8 * const typeIn) {
    aitEnum ret = aitEnumInvalid;
    uint32 i = 0u;
    while (aitEnum_strings[i] != NULL) {
        if (StringHelper::Compare(aitEnum_strings[i], typeIn) == 0) {
            ret = aitTypes[i];
            break;
        }
        i++;
    }
    return ret;
}

void MARTePv::SetUseLocalTimeStamp(const bool useLocalTsIn) {
    useLocalTs = useLocalTsIn;
    uint32 size = Size();
    for (uint32 i = 0u; i < size; i++) {
        ReferenceT<MARTePv> ref = Get(i);
        if (ref.IsValid()) {
            ref->SetUseLocalTimeStamp(useLocalTsIn);
        }
    }
}

void MARTePv::SetTimeStamp(const uint32 secs,
                           const uint32 nanosecs) {

    epicsTimeStamp currTime;
    currTime.secPastEpoch = secs;
    currTime.nsec = nanosecs;

    if (!useLocalTs) {
        currentTimestamp = epicsTime(currTime);
    }
}

bool MARTePv::GddToAnyTypeConverter(AnyType &output,
                                    const gdd &input,
                                    const bool newPointer) {
    const void *ptr = input.dataVoid();
    //input.dataAddress();
    uint32 dimensions = input.dimension();
    bool ret = (dimensions < 2u);
    if (ret) {

        void* oldPtr = output.GetDataPointer();
        if (oldPtr != NULL_PTR(void*)) {
            (void) HeapManager::Free(oldPtr);
        }

        const gddBounds *pB = input.getBounds();

        uint32 totalNumberOfElements = 1u;
        for (uint32 i = 0u; (i < dimensions); i++) {
            uint32 nElems = pB[i].size();
            totalNumberOfElements *= nElems;
        }

        TypeDescriptor td = typeDescriptors[input.primitiveType()];

//case string
        bool isString = (td.operator==(Character8Bit));

        uint32 bitSize = static_cast<uint32>(td.numberOfBits);
        if (isString) {
            bitSize *= static_cast<uint32>(MAX_STRING_SIZE);
        }

        uint32 totalSize = totalNumberOfElements * (bitSize / 8u);
        void* newPtr = NULL_PTR(void*);
        if (newPointer) {
            newPtr = HeapManager::Malloc(totalSize);
            (void) MemoryOperationsHelper::Set(newPtr, '\0', totalSize);
            if (isString) {
                aitString *tempStr = reinterpret_cast<aitString *>(const_cast<void*>(ptr));
                char8 *newStr = reinterpret_cast<char8 *>(newPtr);
                for (uint32 i = 0u; i < totalNumberOfElements; i++) {
                    /*lint -e{923} use the const char* operator of aitString*/
                    const char8 *str = static_cast<const char8 *>(tempStr[i]);
                    (void) StringHelper::Copy(newStr, str);
                    newStr = &newStr[MAX_STRING_SIZE];
                }
            }
            else {
                ret = MemoryOperationsHelper::Copy(newPtr, ptr, totalSize);
            }
        }
        else {
            if (isString) {
                void *temp = const_cast<void*>(ptr);
                aitString *temp1 = reinterpret_cast<aitString *>(temp);
                const char8 *temp2 = temp1->operator const char8*();
                char8 *temp3 = const_cast<char8 *>(temp2);
                newPtr = reinterpret_cast<void *>(temp3);
            }
            else {
                newPtr = const_cast<void*>(ptr);
            }
        }

        if (ret) {
            output = AnyType(td, 0u, newPtr);

            if (dimensions == 0u) {
                if (isString) {
                    output.SetNumberOfDimensions(1u);
                    output.SetNumberOfElements(0u, static_cast<uint32>(MAX_STRING_SIZE));
                }
            }
            else {
                if (isString) {
                    output.SetNumberOfDimensions(2u);
                    output.SetNumberOfElements(0u, static_cast<uint32>(MAX_STRING_SIZE));
                    output.SetNumberOfElements(1u, pB[0u].size());
                }
                else {
                    output.SetNumberOfDimensions(1u);
                    output.SetNumberOfElements(0u, pB[0u].size());
                }
            }
        }
    }
    return ret;
}

void MARTePv::AnyTypeToGddConverter(gdd &output,
                                    const AnyType &input,
                                    const aitEnum typeIn) {
    TypeDescriptor td = input.GetTypeDescriptor();
    bool isString = (td.operator==(Character8Bit));
    uint32 numberOfElements = input.GetNumberOfElements(0u);
    if (isString) {
        numberOfElements = input.GetNumberOfElements(1u);
    }
    if (numberOfElements > 1u) {
        output.setDimension(1);
        if (output.setBound(0u, 0u, numberOfElements) == 0) {
            if (isString) {
                aitString *temp = new aitString[numberOfElements];
                const char8 *ptr = reinterpret_cast<const char8*>(input.GetDataPointer());
                for (uint32 i = 0u; i < numberOfElements; i++) {
                    temp[i] = ptr;
                    ptr = &ptr[MAX_STRING_SIZE];
                }

                output.adjust(new exStringDestructor, temp, typeIn, aitLocalDataFormat);
                /*lint -e{429} the pointer will be freed by the destructor*/
            }
            else {
                uint32 totalSize = (input.GetByteSize() * input.GetNumberOfElements(0u));
                void *temp = HeapManager::Malloc(totalSize);
                (void) MemoryOperationsHelper::Copy(temp, input.GetDataPointer(), totalSize);
                output.adjust(new gddDestructor, temp, typeIn, aitLocalDataFormat);
                /*lint -e{429} the pointer will be freed by the destructor*/
            }
        }
    }
    else {
        if (isString) {
            aitString temp = aitString(reinterpret_cast<const char8*>(input.GetDataPointer()));
            output.set(typeIn, &temp, aitLocalDataFormat);
        }
        else {
            output.set(typeIn, input.GetDataPointer(), aitLocalDataFormat);
        }
    }

}

TypeDescriptor MARTePv::ConvertToTypeDescriptor(const aitEnum typeIn) {
    return typeDescriptors[typeIn];
}

uint32 MARTePv::maxDimension() const {
    return (elementCount > 1u) ? (1u) : (0u);
}

/*lint -e{715} dimension is not needed*/
aitIndex MARTePv::maxBound(const uint32 dimension) const {
    return (dimension == 0u) ? (elementCount) : (1u);
}

/*lint -e{1960} */
void MARTePv::InitPv(){

}

}
