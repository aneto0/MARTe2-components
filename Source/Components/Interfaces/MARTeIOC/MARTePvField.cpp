/**
 * @file MARTePvField.cpp
 * @brief Source file for class MARTePvField
 * @date 04/09/2019
 * @author Giuseppe Ferro
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
 * the class MARTePvField (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include "dbMapper.h"
#include "gddApps.h"
#include "casCtx.h"

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "MARTePvField.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
//#define NULL_PTR(x) NULL
/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

MARTePvField::MARTePvField() :
        MARTePv() {
    // Auto-generated constructor stub for MARTePvField
    // TODO Verify if manual additions are needed
    fatherPv = NULL_PTR(MARTePv *);
    applicationId = 0u;
}

MARTePvField::~MARTePvField() {
    fatherPv = NULL_PTR(MARTePv *);
// Auto-generated destructor stub for MARTePvField
// TODO Verify if manual additions are needed
}

/*lint -e{715} ctx not referenced*/
caStatus MARTePvField::read(const casCtx &ctx,
                            gdd & prototype) {
    caStatus ret = static_cast<caStatus>(S_casApp_success);
    uint32 original_app = prototype.applicationType();
    /*lint -e{9117} -e{713} needed conversion from uint32 to int32*/
    prototype.setApplType(static_cast<int32>(applicationId));
    if (fatherPv != NULL_PTR(MARTePv*)) {
        ret = fatherPv->read(ctx, prototype);
    }
    /*lint -e{9117} -e{713} needed conversion from uint32 to int32*/
    prototype.setApplType(original_app);
    return ret;
}

/*lint -e{715} ctx not referenced*/
caStatus MARTePvField::write(const casCtx &ctx,
                             const gdd & value) {
    caStatus ret = static_cast<caStatus>(S_casApp_success);
    if (useLocalTs) {
        currentTimestamp = epicsTime::getCurrent();
    }
    if (fatherPv != NULL_PTR(MARTePv*)) {
        //convert to anytype and do WriteDirect
        bool isString = (myType == (aitEnumString));

        AnyType temp;
        if (MARTePv::GddToAnyTypeConverter(temp, value, isString)) {
            ret = fatherPv->WriteDirect(temp, applicationId);
        }
        else {
            /*lint -e{9130} depends in external library*/
            ret = static_cast<caStatus>(S_cas_noConvert);
        }
        if (isString) {
            if (!temp.IsVoid()) {
                void* ptr = temp.GetDataPointer();
                (void) HeapManager::Free(ptr);
            }
        }
        if (ret == static_cast<caStatus>(S_casApp_success)) {
            if (interest) {
                smartGDDPointer gddVal = new gdd(value);
                aitTimeStamp gddts(currentTimestamp);
                gddVal->setTimeStamp(&gddts);
                if (cas != NULL_PTR(MARTeIocServer *)) {
                    casEventMask sel = (cas->valueEventMask() | cas->logEventMask());
                    postEvent(sel, *gddVal);
                }
                if (gddVal->unreference() != 0) {
                    /*lint -e{9130} depends in external library*/
                    ret = static_cast<caStatus>(S_cas_internal);
                }
            }
        }
    }
    return ret;
}

/*lint -e{715} applType not needed here because this is already a field*/
caStatus MARTePvField::WriteDirect(const AnyType& valueIn,
                                   const uint32 applType) {
    caStatus ret = static_cast<caStatus>(S_casApp_success);
    if (useLocalTs) {
        currentTimestamp = epicsTime::getCurrent();
    }
    if (fatherPv != NULL_PTR(MARTePv*)) {
        ret = fatherPv->WriteDirect(valueIn, applicationId);
        if (ret == static_cast<caStatus>(S_casApp_success)) {
            if (cas != NULL_PTR(MARTeIocServer *)) {
                if (interest) {
                    smartGDDPointer gddVal = new gdd(gddAppType_value);
                    MARTePv::AnyTypeToGddConverter(*gddVal, valueIn, myType);
                    aitTimeStamp gddts(currentTimestamp);
                    gddVal->setTimeStamp(&gddts);

                    casEventMask sel = (cas->valueEventMask() | cas->logEventMask());
                    postEvent(sel, *gddVal);
                    if (gddVal->unreference() != 0) {
                        /*lint -e{9130} depends in external library*/
                        ret = static_cast<caStatus>(S_cas_internal);
                    }
                }
            }
        }
    }
    return ret;
}

/*lint -e{715} applType not needed here because this is already a field*/
caStatus MARTePvField::ReadDirect(const AnyType& valueOut,
                                  const uint32 applType) {
    caStatus ret = static_cast<caStatus>(S_casApp_success);
    if (fatherPv != NULL_PTR(MARTePv*)) {
        ret = fatherPv->ReadDirect(valueOut, applicationId);
    }
    return ret;
}

void MARTePvField::scan() {
    if (useLocalTs) {
        currentTimestamp = epicsTime::getCurrent();
    }
    if (cas != NULL_PTR(MARTeIocServer*)) {
        smartGDDPointer gddVal = new gdd(static_cast<int32>(applicationId));
        casCtx ctx;
        if (fatherPv != NULL_PTR(MARTePv *)) {
            if (fatherPv->read(ctx, *gddVal) == static_cast<caStatus>(S_casApp_success)) {
                aitTimeStamp gddts(currentTimestamp);
                gddVal->setTimeStamp(&gddts);
                casEventMask sel = (cas->valueEventMask() | cas->logEventMask());
                postEvent(sel, *gddVal);
                (void) gddVal->unreference();
            }
        }
    }
}

caStatus MARTePvField::writeNotify(const casCtx & ctx,
                                   const gdd & value) {
    return write(ctx, value);
}

void MARTePvField::SetPvFather(MARTePv &fatherIn,
                               const uint32 applType,
                               const aitEnum pvType,
                               const bool useLocalTsIn) {
    fatherPv = &fatherIn;
    applicationId = applType;
    myType = pvType;
    useLocalTs = useLocalTsIn;
}

CLASS_REGISTER(MARTePvField, "1.0")

}
