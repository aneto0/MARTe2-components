/**
 * @file MARTeCommonPv.cpp
 * @brief Source file for class MARTeCommonPv
 * @date 11/07/2019
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
 * the class MARTeCommonPv (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <math.h>
#include <stdlib.h>
#include <limits.h>
#include "alarm.h"
#include "dbMapper.h"
#include "gddApps.h"
#include "casStrmClient.h"
/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "MARTeCommonPv.h"
#include "MARTeIocServer.h"
#include "MARTePvField.h"
#include "StreamMemoryReference.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

//#define NULL_PTR(x) NULL

static const float64 DEFAULT_EPICS_HOPR = 10.0;
static const float64 DEFAULT_EPICS_LOPR = -10.0;
static const uint32 DEFAULT_EPICS_PREC = 4u;
static const char8 * DEFAULT_EPICS_EGU = "";
static const float64 DEFAULT_EPICS_HYST = 0.0;
static const float64 DEFAULT_EPICS_ADEL = 0.0;
static const float64 DEFAULT_EPICS_MDEL = 0.0;

#define recGblSetSevr(NSTA, NSEV) ((nsev<(NSEV)) ? (nsta=(NSTA),nsev=(NSEV),true) : false)
static const char8 * const menuAlarmSevr_strings[] = {"NO_ALARM", "MINOR", "MAJOR", "INVALID", NULL_PTR(const char8 * const)}
;

static const char8 * const menuAlarmStat_strings[] = {"NO_ALARM", "READ_ALARM", "WRITE_ALARM",
    "HIHI_ALARM", "HIGH_ALARM", "LOLO_ALARM", "LOW_ALARM", "STATE_ALARM", "COS_ALARM", "COMM_ALARM", "TIMEOUT_ALARM", "HW_LIMIT_ALARM", "CALC_ALARM",
    "SCAN_ALARM", "LINK_ALARM", "SOFT_ALARM", "BAD_SUB_ALARM", "UDF_ALARM", "DISABLE_ALARM", "SIMM_ALARM", "READ_ACCESS_ALARM", "WRITE_ACCESS_ALARM",
    "ALARM_NSTATUS", NULL_PTR(const char8 * const)};

static const char8 *fieldNames[] = { "NELM", "EGU", "HOPR", "LOPR", "HIHI", "LOLO", "HIGH", "LOW", "HHSV", "HSV", "LSV", "LLSV", "HYST", "ADEL", "MDEL", "PREC", "STAT", "SEVR", NULL_PTR(const char8 * const) };
static const aitEnum fieldTypes[] = { aitEnumFloat64, aitEnumString, aitEnumFloat64, aitEnumFloat64, aitEnumFloat64, aitEnumFloat64, aitEnumFloat64,
        aitEnumUint32, aitEnumString, aitEnumString, aitEnumString, aitEnumString, aitEnumFloat64, aitEnumFloat64, aitEnumFloat64, aitEnumUint32, aitEnumString,
        aitEnumString };

static const uint32 fieldApps[] = { static_cast<uint32>(gddAppType_maxElements), static_cast<uint32>(gddAppType_units),
        static_cast<uint32>(gddAppType_controlHigh), static_cast<uint32>(gddAppType_controlLow), static_cast<uint32>(gddAppType_alarmHigh),
        static_cast<uint32>(gddAppType_alarmLow), static_cast<uint32>(gddAppType_alarmHighWarning), static_cast<uint32>(gddAppType_alarmLowWarning),
        gddAppType_hhsv, gddAppType_hsv, gddAppType_lsv, gddAppType_llsv, gddAppType_hyst, gddAppType_adel, gddAppType_mdel,
        static_cast<uint32>(gddAppType_precision), gddAppType_stat, gddAppType_sevr };

void MARTeCommonPv::InsertRefs() {
    uint32 i = 0u;
    while (fieldNames[i] != NULL_PTR(const char8 *)) {
        ReferenceT < MARTePvField > ref(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        if (ref.IsValid()) {
            ref->SetPvFather(*this, fieldApps[i], fieldTypes[i], useLocalTs);
            ref->SetName(fieldNames[i]);
            (void) Insert(ref);
        }
        i++;
    }
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
MARTeCommonPv::MARTeCommonPv() :
        MARTePv() {
    // Auto-generated constructor stub for MARTeCommonPv
    // TODO Verify if manual additions are needed
    pvHopr = 0.;
    pvLopr = 0.;
    pvHihi = 0.;
    pvLolo = 0.;
    pvHigh = 0.;
    pvLow = 0.;
    pvHhsv = 0;
    pvHsv = 0;
    pvLsv = 0;
    pvLlsv = 0;
    pvHyst = 0.;
    pvAdel = 0.;
    pvMdel = 0.;
    pvPrec = 0u;
    lalm = 0.;
    alst = 0.;
    mlst = 0.;
    acks = aitEnumInvalid;
    ackt = aitEnumInvalid;
    pvSev = 0;
    pvSta = 0;

    (void) MemoryOperationsHelper::Set(&pvUnits[0], '\0', static_cast<uint32>(MAX_STRING_SIZE));

    functionReadTable[gddAppType_value] = &MARTeCommonPv::GetValue;
    functionReadTable[gddAppType_precision] = &MARTeCommonPv::GetPrecision;
    functionReadTable[gddAppType_controlHigh] = &MARTeCommonPv::GetHighLimit;
    functionReadTable[gddAppType_controlLow] = &MARTeCommonPv::GetLowLimit;
    functionReadTable[gddAppType_alarmHigh] = &MARTeCommonPv::GetHighAlarm;
    functionReadTable[gddAppType_alarmLow] = &MARTeCommonPv::GetLowAlarm;
    functionReadTable[gddAppType_alarmHighWarning] = &MARTeCommonPv::GetHighWarning;
    functionReadTable[gddAppType_alarmLowWarning] = &MARTeCommonPv::GetLowWarning;
    functionReadTable[gddAppType_units] = &MARTeCommonPv::GetUnits;
    functionReadTable[gddAppType_enums] = &MARTeCommonPv::GetEnums;
    functionReadTable[gddAppType_acks] = &MARTeCommonPv::GetAckt;
    functionReadTable[gddAppType_ackt] = &MARTeCommonPv::GetAcks;
    functionReadTable[gddAppType_maxElements] = &MARTeCommonPv::GetElementCount;
    functionReadTable[gddAppType_timeStamp] = &MARTeCommonPv::GetTimestamp;
    functionReadTable[gddAppType_hyst] = &MARTeCommonPv::GetHyst;
    functionReadTable[gddAppType_mdel] = &MARTeCommonPv::GetMdel;
    functionReadTable[gddAppType_adel] = &MARTeCommonPv::GetAdel;
    functionReadTable[gddAppType_hhsv] = &MARTeCommonPv::GetHhsv;
    functionReadTable[gddAppType_hsv] = &MARTeCommonPv::GetHsv;
    functionReadTable[gddAppType_lsv] = &MARTeCommonPv::GetLsv;
    functionReadTable[gddAppType_llsv] = &MARTeCommonPv::GetLlsv;
    functionReadTable[gddAppType_stat] = &MARTeCommonPv::GetStat;
    functionReadTable[gddAppType_sevr] = &MARTeCommonPv::GetSevr;

    functionDirectReadTable[gddAppType_value] = &MARTeCommonPv::GetDirectValue;
    functionDirectReadTable[gddAppType_precision] = &MARTeCommonPv::GetDirectPrecision;
    functionDirectReadTable[gddAppType_controlHigh] = &MARTeCommonPv::GetDirectHighLimit;
    functionDirectReadTable[gddAppType_controlLow] = &MARTeCommonPv::GetDirectLowLimit;
    functionDirectReadTable[gddAppType_alarmHigh] = &MARTeCommonPv::GetDirectHighAlarm;
    functionDirectReadTable[gddAppType_alarmLow] = &MARTeCommonPv::GetDirectLowAlarm;
    functionDirectReadTable[gddAppType_alarmHighWarning] = &MARTeCommonPv::GetDirectHighWarning;
    functionDirectReadTable[gddAppType_alarmLowWarning] = &MARTeCommonPv::GetDirectLowWarning;
    functionDirectReadTable[gddAppType_units] = &MARTeCommonPv::GetDirectUnits;
    functionDirectReadTable[gddAppType_enums] = &MARTeCommonPv::GetDirectEnums;
    functionDirectReadTable[gddAppType_acks] = &MARTeCommonPv::GetDirectAckt;
    functionDirectReadTable[gddAppType_ackt] = &MARTeCommonPv::GetDirectAcks;
    functionDirectReadTable[gddAppType_maxElements] = &MARTeCommonPv::GetDirectElementCount;
    functionDirectReadTable[gddAppType_timeStamp] = &MARTeCommonPv::GetDirectTimeStamp;
    functionDirectReadTable[gddAppType_hyst] = &MARTeCommonPv::GetDirectHyst;
    functionDirectReadTable[gddAppType_mdel] = &MARTeCommonPv::GetDirectMdel;
    functionDirectReadTable[gddAppType_adel] = &MARTeCommonPv::GetDirectAdel;
    functionDirectReadTable[gddAppType_hhsv] = &MARTeCommonPv::GetDirectHhsv;
    functionDirectReadTable[gddAppType_hsv] = &MARTeCommonPv::GetDirectHsv;
    functionDirectReadTable[gddAppType_lsv] = &MARTeCommonPv::GetDirectLsv;
    functionDirectReadTable[gddAppType_llsv] = &MARTeCommonPv::GetDirectLlsv;
    functionDirectReadTable[gddAppType_stat] = &MARTeCommonPv::GetDirectStat;
    functionDirectReadTable[gddAppType_sevr] = &MARTeCommonPv::GetDirectSevr;

    functionWriteTable[gddAppType_value] = &MARTeCommonPv::PutValue;
    functionWriteTable[gddAppType_precision] = &MARTeCommonPv::PutPrecision;
    functionWriteTable[gddAppType_controlHigh] = &MARTeCommonPv::PutHighLimit;
    functionWriteTable[gddAppType_controlLow] = &MARTeCommonPv::PutLowLimit;
    functionWriteTable[gddAppType_alarmHigh] = &MARTeCommonPv::PutHighAlarm;
    functionWriteTable[gddAppType_alarmLow] = &MARTeCommonPv::PutLowAlarm;
    functionWriteTable[gddAppType_alarmHighWarning] = &MARTeCommonPv::PutHighWarning;
    functionWriteTable[gddAppType_alarmLowWarning] = &MARTeCommonPv::PutLowWarning;
    functionWriteTable[gddAppType_units] = &MARTeCommonPv::PutUnits;
    functionWriteTable[gddAppType_enums] = &MARTeCommonPv::PutEnums;
    functionWriteTable[gddAppType_acks] = &MARTeCommonPv::PutAckt;
    functionWriteTable[gddAppType_ackt] = &MARTeCommonPv::PutAcks;
    functionWriteTable[gddAppType_timeStamp] = &MARTeCommonPv::PutTimeStamp;
    functionWriteTable[gddAppType_hyst] = &MARTeCommonPv::PutHyst;
    functionWriteTable[gddAppType_mdel] = &MARTeCommonPv::PutMdel;
    functionWriteTable[gddAppType_adel] = &MARTeCommonPv::PutAdel;
    functionWriteTable[gddAppType_hhsv] = &MARTeCommonPv::PutHhsv;

    functionWriteTable[gddAppType_hsv] = &MARTeCommonPv::PutHsv;
    functionWriteTable[gddAppType_lsv] = &MARTeCommonPv::PutLsv;
    functionWriteTable[gddAppType_llsv] = &MARTeCommonPv::PutLlsv;

    functionDirectWriteTable[gddAppType_value] = &MARTeCommonPv::PutDirectValue;
    functionDirectWriteTable[gddAppType_precision] = &MARTeCommonPv::PutDirectPrecision;
    functionDirectWriteTable[gddAppType_controlHigh] = &MARTeCommonPv::PutDirectHighLimit;
    functionDirectWriteTable[gddAppType_controlLow] = &MARTeCommonPv::PutDirectLowLimit;
    functionDirectWriteTable[gddAppType_alarmHigh] = &MARTeCommonPv::PutDirectHighAlarm;
    functionDirectWriteTable[gddAppType_alarmLow] = &MARTeCommonPv::PutDirectLowAlarm;
    functionDirectWriteTable[gddAppType_alarmHighWarning] = &MARTeCommonPv::PutDirectHighWarning;
    functionDirectWriteTable[gddAppType_alarmLowWarning] = &MARTeCommonPv::PutDirectLowWarning;
    functionDirectWriteTable[gddAppType_units] = &MARTeCommonPv::PutDirectUnits;
    functionDirectWriteTable[gddAppType_enums] = &MARTeCommonPv::PutDirectEnums;
    functionDirectWriteTable[gddAppType_acks] = &MARTeCommonPv::PutDirectAckt;
    functionDirectWriteTable[gddAppType_ackt] = &MARTeCommonPv::PutDirectAcks;
    functionDirectWriteTable[gddAppType_timeStamp] = &MARTeCommonPv::PutDirectTimeStamp;
    functionDirectWriteTable[gddAppType_hyst] = &MARTeCommonPv::PutDirectHyst;
    functionDirectWriteTable[gddAppType_mdel] = &MARTeCommonPv::PutDirectMdel;
    functionDirectWriteTable[gddAppType_adel] = &MARTeCommonPv::PutDirectAdel;
    functionDirectWriteTable[gddAppType_hhsv] = &MARTeCommonPv::PutDirectHhsv;
    functionDirectWriteTable[gddAppType_hsv] = &MARTeCommonPv::PutDirectHsv;
    functionDirectWriteTable[gddAppType_lsv] = &MARTeCommonPv::PutDirectLsv;
    functionDirectWriteTable[gddAppType_llsv] = &MARTeCommonPv::PutDirectLlsv;
}

MARTeCommonPv::~MARTeCommonPv() {
    // Auto-generated destructor stub for MARTeCommonPv
    // TODO Verify if manual additions are needed
}

bool MARTeCommonPv::Initialise(StructuredDataI &data) {

    if (!data.Read("VAL", pvInitialVal)) {
        pvInitialVal = "";
    }

    StreamString pvUnitsStr;
    if (!data.Read("EGU", pvUnitsStr)) {
        pvUnitsStr = DEFAULT_EPICS_EGU;
    }
    (void) StringHelper::Copy(&pvUnits[0], pvUnitsStr.Buffer());

    if (!data.Read("HOPR", pvHopr)) {
        pvHopr = DEFAULT_EPICS_HOPR;
    }

    if (!data.Read("LOPR", pvLopr)) {
        pvLopr = DEFAULT_EPICS_LOPR;
    }

    if (!data.Read("HIHI", pvHihi)) {
        pvHihi = pvHopr;
    }

    if (!data.Read("LOLO", pvLolo)) {
        pvLolo = pvLopr;
    }

    if (!data.Read("HIGH", pvHigh)) {
        pvHigh = pvHopr;
    }

    if (!data.Read("LOW", pvLow)) {
        pvLow = pvLopr;
    }

    pvHhsv = 3;
    StreamString hihisvStr;
    if (data.Read("HHSV", hihisvStr)) {
        int16 n = 0;
        while (menuAlarmSevr_strings[n] != NULL_PTR(const char8*)) {
            if (hihisvStr.operator==(menuAlarmSevr_strings[n])) {
                pvHhsv = n;
                break;
            }
            n++;
        }
    }

    pvHsv = 3;
    StreamString hisvStr;
    if (data.Read("HSV", hisvStr)) {
        int16 n = 0;
        while (menuAlarmSevr_strings[n] != NULL_PTR(const char8*)) {
            if (hisvStr.operator==(menuAlarmSevr_strings[n])) {
                pvHsv = n;
                break;
            }
            n++;
        }
    }

    pvLsv = 3;
    StreamString losvStr;
    if (data.Read("LSV", losvStr)) {
        int16 n = 0;
        while (menuAlarmSevr_strings[n] != NULL_PTR(const char8*)) {
            if (losvStr.operator==(menuAlarmSevr_strings[n])) {
                pvLsv = n;
                break;
            }
            n++;

        }
    }

    pvLlsv = 3;
    StreamString lolosvStr;
    if (data.Read("LLSV", lolosvStr)) {
        int16 n = 0;
        while (menuAlarmSevr_strings[n] != NULL_PTR(const char8*)) {
            if (lolosvStr.operator==(menuAlarmSevr_strings[n])) {
                pvLlsv = n;
                break;
            }
            n++;
        }
    }

    if (!data.Read("HYST", pvHyst)) {
        pvHyst = DEFAULT_EPICS_HYST;
    }

    if (!data.Read("ADEL", pvAdel)) {
        pvAdel = DEFAULT_EPICS_ADEL;
    }

    if (!data.Read("MDEL", pvMdel)) {
        pvMdel = DEFAULT_EPICS_MDEL;
    }

    if (!data.Read("PREC", pvPrec)) {
        pvPrec = DEFAULT_EPICS_PREC;
    }

    InsertRefs();

    bool ret = MARTePv::Initialise(data);
    return ret;
}
caStatus MARTeCommonPv::GetAckt(gdd & value) const {
    /*lint -e{641} required conversion from aitEnum to int*/
    return static_cast<caStatus>(value.put(ackt));
}
caStatus MARTeCommonPv::GetAcks(gdd & value) const {
    /*lint -e{641} required conversion from aitEnum to int*/
    return static_cast<caStatus>(value.put(acks));
}

caStatus MARTeCommonPv::GetPrecision(gdd & value) const {
    return static_cast<caStatus>(value.put(pvPrec));
}

caStatus MARTeCommonPv::GetHighLimit(gdd & value) const {
    return static_cast<caStatus>(value.put(pvHopr));
}

caStatus MARTeCommonPv::GetLowLimit(gdd & value) const {
    return static_cast<caStatus>(value.put(pvLopr));
}
caStatus MARTeCommonPv::GetHighAlarm(gdd & value) const {
    return static_cast<caStatus>(value.put(pvHihi));
}
caStatus MARTeCommonPv::GetLowAlarm(gdd & value) const {
    return static_cast<caStatus>(value.put(pvLolo));
}

caStatus MARTeCommonPv::GetHighWarning(gdd & value) const {
    return static_cast<caStatus>(value.put(pvHigh));
}

caStatus MARTeCommonPv::GetLowWarning(gdd & value) const {
    return static_cast<caStatus>(value.put(pvLow));
}

caStatus MARTeCommonPv::GetUnits(gdd & value) const {
    return static_cast<caStatus>(value.put(reinterpret_cast<const char8 *>(pvUnits)));
}

caStatus MARTeCommonPv::GetEnums(gdd &) const {
    return static_cast<caStatus>(S_cas_success);
}

caStatus MARTeCommonPv::GetValue(gdd & value) const {

    caStatus status = static_cast<caStatus>(S_cas_success);
    MARTePv::AnyTypeToGddConverter(value, pValue, myType);
    aitTimeStamp gddts(currentTimestamp);
    value.setTimeStamp(&gddts);
    return status;
}

gddAppFuncTableStatus MARTeCommonPv::GetElementCount(gdd & value) const {
    return static_cast<caStatus>(value.put(elementCount));
}

caStatus MARTeCommonPv::GetTimestamp(gdd & value) const {
    aitTimeStamp gddts(currentTimestamp);
    value.setTimeStamp(&gddts);
    return static_cast<caStatus>(S_cas_success);
}
caStatus MARTeCommonPv::GetHyst(gdd & value) const {
    return static_cast<caStatus>(value.put(pvHyst));
}
caStatus MARTeCommonPv::GetMdel(gdd & value) const {
    return static_cast<caStatus>(value.put(pvMdel));
}
caStatus MARTeCommonPv::GetAdel(gdd & value) const {
    return static_cast<caStatus>(value.put(pvAdel));
}

caStatus MARTeCommonPv::GetHhsv(gdd & value) const {
    return static_cast<caStatus>(value.put(menuAlarmSevr_strings[pvHhsv]));
}

caStatus MARTeCommonPv::GetHsv(gdd & value) const {
    return static_cast<caStatus>(value.put(menuAlarmSevr_strings[pvHsv]));
}

caStatus MARTeCommonPv::GetLsv(gdd & value) const {
    return static_cast<caStatus>(value.put(menuAlarmSevr_strings[pvLsv]));
}

caStatus MARTeCommonPv::GetLlsv(gdd & value) const {
    return static_cast<caStatus>(value.put(menuAlarmSevr_strings[pvLlsv]));
}
caStatus MARTeCommonPv::GetStat(gdd & value) const {
    return static_cast<caStatus>(value.put(menuAlarmStat_strings[pvSta]));
}

caStatus MARTeCommonPv::GetSevr(gdd & value) const {
    return static_cast<caStatus>(value.put(menuAlarmSevr_strings[pvSev]));
}

caStatus MARTeCommonPv::PutPrecision(const gdd &value) {
    pvPrec = static_cast<uint32>(value);
    return static_cast<caStatus>(S_cas_success);
}
caStatus MARTeCommonPv::PutHighLimit(const gdd &value) {
    pvHopr = static_cast<float64>(value);
    return static_cast<caStatus>(S_cas_success);
}
caStatus MARTeCommonPv::PutLowLimit(const gdd &value) {
    pvLopr = static_cast<float64>(value);
    return static_cast<caStatus>(S_cas_success);
}
caStatus MARTeCommonPv::PutHighAlarm(const gdd & value) {
    pvHihi = static_cast<float64>(value);
    return static_cast<caStatus>(S_cas_success);
}
caStatus MARTeCommonPv::PutLowAlarm(const gdd & value) {
    pvLolo = static_cast<float64>(value);
    return static_cast<caStatus>(S_cas_success);
}
caStatus MARTeCommonPv::PutHighWarning(const gdd & value) {
    pvHigh = static_cast<float64>(value);
    return static_cast<caStatus>(S_cas_success);
}
caStatus MARTeCommonPv::PutLowWarning(const gdd & value) {
    pvLow = static_cast<float64>(value);
    return static_cast<caStatus>(S_cas_success);
}
caStatus MARTeCommonPv::PutUnits(const gdd &value) {
    const aitString str(reinterpret_cast<const aitString*>(value.dataVoid()));
    const char8 *temp = str.operator const char8*();
    (void) StringHelper::Copy(&pvUnits[0], temp);
    return static_cast<caStatus>(S_cas_success);
}
caStatus MARTeCommonPv::PutValue(const gdd &value) {
    caStatus status = static_cast<caStatus>(S_casApp_success);
    TypeDescriptor td = pValue.GetTypeDescriptor();
    bool isString = (td.operator==(Character8Bit));
    AnyType temp;
    if (MARTePv::GddToAnyTypeConverter(temp, value, isString)) {
        if (TypeConvert(pValue, temp)) {
            status = Update(value);
        }
        else {
            /*lint -e{9130} depends on external library*/
            status = static_cast<caStatus>(S_cas_noConvert);
        }
    }
    else {
        /*lint -e{9130} depends on external library*/
        status = static_cast<caStatus>(S_cas_noConvert);
    }
    if (isString) {
        if (!temp.IsVoid()) {
            void* ptr = temp.GetDataPointer();
            (void) HeapManager::Free(ptr);
        }
    }
    return status;
}

/*lint -e{1762} could be made const but has to match a prototype*/
caStatus MARTeCommonPv::PutEnums(const gdd &) {
    return static_cast<caStatus>(S_cas_success);
}
caStatus MARTeCommonPv::PutTimeStamp(const gdd & value) {

    epicsTimeStamp ts;
    value.getTimeStamp(&ts);
    currentTimestamp = ts;
    return static_cast<caStatus>(S_cas_success);
}
caStatus MARTeCommonPv::PutHyst(const gdd & value) {
    pvHyst = static_cast<float64>(value);
    return static_cast<caStatus>(S_cas_success);
}
caStatus MARTeCommonPv::PutMdel(const gdd & value) {
    pvMdel = static_cast<float64>(value);
    return static_cast<caStatus>(S_cas_success);
}
caStatus MARTeCommonPv::PutAdel(const gdd & value) {
    pvAdel = static_cast<float64>(value);
    return static_cast<caStatus>(S_cas_success);
}

caStatus MARTeCommonPv::PutHhsv(const gdd & value) {
    const aitString str(reinterpret_cast<const aitString*>(value.dataVoid()));
    pvHhsv = 3;
    StreamString hihisvStr = str.operator const char8*();
    int16 n = 0;
    while (menuAlarmSevr_strings[n] != NULL_PTR(const char8*)) {
        if (hihisvStr.operator==(menuAlarmSevr_strings[n])) {
            pvHhsv = n;
            break;
        }
        n++;
    }
    return static_cast<caStatus>(S_cas_success);
}
caStatus MARTeCommonPv::PutHsv(const gdd & value) {
    const aitString str(reinterpret_cast<const aitString*>(value.dataVoid()));
    pvHsv = 3;
    StreamString hsvStr = str.operator const char8*();
    int16 n = 0;
    while (menuAlarmSevr_strings[n] != NULL_PTR(const char8*)) {
        if (hsvStr.operator==(menuAlarmSevr_strings[n])) {
            pvHsv = n;
            break;
        }
        n++;
    }
    return static_cast<caStatus>(S_cas_success);
}
caStatus MARTeCommonPv::PutLsv(const gdd & value) {
    const aitString str(reinterpret_cast<const aitString*>(value.dataVoid()));
    pvLsv = 3;
    StreamString lsvStr = str.operator const char8*();
    int16 n = 0;
    while (menuAlarmSevr_strings[n] != NULL_PTR(const char8*)) {
        if (lsvStr.operator==(menuAlarmSevr_strings[n])) {
            pvLsv = n;
            break;
        }
        n++;
    }
    return static_cast<caStatus>(S_cas_success);
}
caStatus MARTeCommonPv::PutLlsv(const gdd & value) {
    const aitString str(reinterpret_cast<const aitString*>(value.dataVoid()));
    pvLlsv = 3;
    StreamString loloStr = str.operator const char8*();
    int16 n = 0;
    while (menuAlarmSevr_strings[n] != NULL_PTR(const char8*)) {
        if (loloStr.operator==(menuAlarmSevr_strings[n])) {
            pvLlsv = n;
            break;
        }
        n++;
    }
    return static_cast<caStatus>(S_cas_success);
}

bool MARTeCommonPv::GetDirectPrecision(const AnyType &value) const {
    return TypeConvert(value, pvPrec);
}
bool MARTeCommonPv::GetDirectHighLimit(const AnyType &value) const {
    return TypeConvert(value, pvHopr);
}
bool MARTeCommonPv::GetDirectLowLimit(const AnyType &value) const {
    return TypeConvert(value, pvLopr);
}
bool MARTeCommonPv::GetDirectHighAlarm(const AnyType &value) const {
    return TypeConvert(value, pvHihi);
}
bool MARTeCommonPv::GetDirectLowAlarm(const AnyType &value) const {
    return TypeConvert(value, pvLolo);
}
bool MARTeCommonPv::GetDirectHighWarning(const AnyType &value) const {
    return TypeConvert(value, pvHigh);
}
bool MARTeCommonPv::GetDirectLowWarning(const AnyType &value) const {
    return TypeConvert(value, pvLow);
}
bool MARTeCommonPv::GetDirectUnits(const AnyType &value) const {
    return TypeConvert(value, pvUnits);
}
bool MARTeCommonPv::GetDirectValue(const AnyType &value) const {
    return TypeConvert(value, pValue);
}
bool MARTeCommonPv::GetDirectEnums(const AnyType &) const {
    return true;
}
bool MARTeCommonPv::GetDirectAckt(const AnyType &value) const {
    /*lint -e{923} -e{930} required cast enum to int*/
    return TypeConvert(value, static_cast<int32>(ackt));
}
bool MARTeCommonPv::GetDirectAcks(const AnyType &value) const {
    /*lint -e{923} -e{930} required cast enum to int*/
    return TypeConvert(value, static_cast<int32>(acks));
}
bool MARTeCommonPv::GetDirectElementCount(const AnyType &value) const {
    return TypeConvert(value, elementCount);
}
bool MARTeCommonPv::GetDirectTimeStamp(const AnyType& value) const {
    epicsTimeStamp ts = currentTimestamp;
    uint64 tsll = 0ull;
    bool ret = MemoryOperationsHelper::Copy(&tsll, &ts, static_cast<uint32>(sizeof(uint64)));
    if (ret) {
        ret = TypeConvert(value, tsll);
    }
    return ret;
}
bool MARTeCommonPv::GetDirectHyst(const AnyType &value) const {
    return TypeConvert(value, pvHyst);
}
bool MARTeCommonPv::GetDirectMdel(const AnyType &value) const {
    return TypeConvert(value, pvMdel);
}
bool MARTeCommonPv::GetDirectAdel(const AnyType &value) const {
    return TypeConvert(value, pvAdel);
}
bool MARTeCommonPv::GetDirectHhsv(const AnyType &value) const {
    StreamString alarmStr = menuAlarmSevr_strings[pvHhsv];
    return TypeConvert(value, alarmStr.Buffer());
}
bool MARTeCommonPv::GetDirectHsv(const AnyType &value) const {
    StreamString alarmStr = menuAlarmSevr_strings[pvHsv];
    return TypeConvert(value, alarmStr.Buffer());
}
bool MARTeCommonPv::GetDirectLsv(const AnyType &value) const {
    StreamString alarmStr = menuAlarmSevr_strings[pvLsv];
    return TypeConvert(value, alarmStr.Buffer());
}
bool MARTeCommonPv::GetDirectLlsv(const AnyType &value) const {
    StreamString alarmStr = menuAlarmSevr_strings[pvLlsv];
    return TypeConvert(value, alarmStr.Buffer());
}
bool MARTeCommonPv::GetDirectStat(const AnyType &value) const {
    StreamString alarmStr = menuAlarmStat_strings[pvSta];
    return TypeConvert(value, alarmStr.Buffer());
}
bool MARTeCommonPv::GetDirectSevr(const AnyType &value) const {
    StreamString alarmStr = menuAlarmSevr_strings[pvSev];
    return TypeConvert(value, alarmStr.Buffer());
}

bool MARTeCommonPv::PutDirectPrecision(const AnyType &value) {
    return TypeConvert(pvPrec, value);
}
bool MARTeCommonPv::PutDirectHighLimit(const AnyType &value) {
    return TypeConvert(pvHopr, value);
}
bool MARTeCommonPv::PutDirectLowLimit(const AnyType &value) {
    return TypeConvert(pvLopr, value);
}
bool MARTeCommonPv::PutDirectHighAlarm(const AnyType &value) {
    return TypeConvert(pvHihi, value);
}
bool MARTeCommonPv::PutDirectLowAlarm(const AnyType &value) {
    return TypeConvert(pvLolo, value);
}
bool MARTeCommonPv::PutDirectHighWarning(const AnyType &value) {
    return TypeConvert(pvHigh, value);
}
bool MARTeCommonPv::PutDirectLowWarning(const AnyType &value) {
    return TypeConvert(pvLow, value);
}
bool MARTeCommonPv::PutDirectUnits(const AnyType &value) {
    return TypeConvert(pvUnits, value);
}
bool MARTeCommonPv::PutDirectValue(const AnyType &value) {

    bool ret = true;

    if (useLocalTs) {
        currentTimestamp = epicsTime::getCurrent();
    }
    TypeDescriptor td = pValue.GetTypeDescriptor();
    bool isString = (td.operator==(Character8Bit));
    if ((elementCount == 1u) && (!isString)) {
        float64 currentValue;
        ret = TypeConvert(currentValue, value);
        if (ret) {
            if (currentValue > pvHopr) {
                currentValue = pvHopr;
            }
            if (currentValue < pvLopr) {
                currentValue = pvLopr;
            }
            ret = TypeConvert(pValue, currentValue);
        }

    }
    else {
        ret = TypeConvert(pValue, value);
    }
    if (ret) {
        if (cas != NULL_PTR(MARTeIocServer *)) {
            if (interest) {
                smartGDDPointer gddVal = new gdd(gddAppType_value);
                MARTePv::AnyTypeToGddConverter(*gddVal, pValue, myType);
                aitTimeStamp gddts(currentTimestamp);
                gddVal->setTimeStamp(&gddts);
                casEventMask sel = (cas->valueEventMask() | cas->logEventMask());
                postEvent(sel, *gddVal);
                ret = (gddVal->unreference() == 0);
            }
        }
    }
    fmutex.FastUnLock();

    return ret;
}

/*lint -e{1762} could be made const but has to match a prototype*/
bool MARTeCommonPv::PutDirectEnums(const AnyType &) {
    return true;
}
bool MARTeCommonPv::PutDirectAckt(const AnyType &value) {
    /*lint -e{923} -e{930} required cast enum to int*/
    int32 acktInt = static_cast<int32>(ackt);
    bool ret = TypeConvert(acktInt, value);
    if (ret) {
        /*lint -e{923} -e{930} required cast int to enum*/
        ackt = static_cast<aitEnum>(acktInt);
    }
    return ret;
}
bool MARTeCommonPv::PutDirectAcks(const AnyType &value) {
    /*lint -e{923} -e{930} required cast enum to int*/
    int32 acksInt = static_cast<int32>(acks);
    bool ret = TypeConvert(acksInt, value);
    if (ret) {
        /*lint -e{923} -e{930} required cast int to enum*/
        acks = static_cast<aitEnum>(acksInt);
    }
    return ret;
}

bool MARTeCommonPv::PutDirectTimeStamp(const AnyType & value) {
    epicsTimeStamp ts;
    uint64 tsll = 0ull;
    bool ret = TypeConvert(tsll, value);
    if (ret) {
        (void) MemoryOperationsHelper::Copy(&ts, &tsll, static_cast<uint32>(sizeof(uint64)));
        currentTimestamp = ts;
    }
    return ret;
}
bool MARTeCommonPv::PutDirectHyst(const AnyType & value) {
    return TypeConvert(pvHyst, value);
}
bool MARTeCommonPv::PutDirectMdel(const AnyType & value) {
    return TypeConvert(pvMdel, value);
}
bool MARTeCommonPv::PutDirectAdel(const AnyType & value) {
    return TypeConvert(pvAdel, value);
}
bool MARTeCommonPv::PutDirectHhsv(const AnyType &value) {
    pvHhsv = 3;
    StreamString hihiStr;
    bool ret = TypeConvert(hihiStr, value);
    if (ret) {
        int16 n = 0;
        while (menuAlarmSevr_strings[n] != NULL_PTR(const char8*)) {
            if (hihiStr.operator==(menuAlarmSevr_strings[n])) {
                pvHhsv = n;
                break;
            }
            n++;
        }
    }
    return ret;
}
bool MARTeCommonPv::PutDirectHsv(const AnyType &value) {
    pvHsv = 3;
    StreamString hStr;
    bool ret = TypeConvert(hStr, value);
    if (ret) {
        int16 n = 0;
        while (menuAlarmSevr_strings[n] != NULL_PTR(const char8*)) {
            if (hStr.operator==(menuAlarmSevr_strings[n])) {
                pvHsv = n;
                break;
            }
            n++;
        }
    }
    return ret;
}
bool MARTeCommonPv::PutDirectLsv(const AnyType &value) {
    pvLsv = 3;
    StreamString lStr;
    bool ret = TypeConvert(lStr, value);
    if (ret) {
        int16 n = 0;
        while (menuAlarmSevr_strings[n] != NULL_PTR(const char8*)) {
            if (lStr.operator==(menuAlarmSevr_strings[n])) {
                pvLsv = n;
                break;
            }
            n++;
        }
    }
    return ret;
}
bool MARTeCommonPv::PutDirectLlsv(const AnyType &value) {
    pvLlsv = 3;
    StreamString loloStr;
    bool ret = TypeConvert(loloStr, value);
    if (ret) {
        int16 n = 0;
        while (menuAlarmSevr_strings[n] != NULL_PTR(const char8*)) {
            if (loloStr.operator==(menuAlarmSevr_strings[n])) {
                pvLlsv = n;
                break;
            }
            n++;
        }
    }
    return ret;
}

/*lint -e{715} ctx not referenced*/
caStatus MARTeCommonPv::read(const casCtx & ctx,
                             gdd & prototype) {

    caStatus ret = static_cast<caStatus>(S_casApp_success);
    if (static_cast<bool>(fmutex.FastLock())) {
        uint32 applType = prototype.applicationType();
        if (applType < MAX_N_FUNC) {
            if (this->functionReadTable[applType] != NULL) {
                ret = (this->*functionReadTable[applType])(prototype);
            }
            else {
                /*lint -e{9130} depends on external library*/
                ret = static_cast<caStatus>(S_cas_internal);
            }
            prototype.setSevr(static_cast<aitUint16>(pvSev));
            prototype.setStat(static_cast<aitUint16>(pvSta));
        }
        fmutex.FastUnLock();
    }
    return ret;
}

caStatus MARTeCommonPv::WriteDirect(const AnyType& valueIn,
                                    const uint32 applType) {
    caStatus status = static_cast<caStatus>(S_casApp_success);

    if (static_cast<bool>(fmutex.FastLock())) {
        if (applType < MAX_N_FUNC) {
            if (this->functionDirectWriteTable[applType] != NULL) {
                if ((this->*functionDirectWriteTable[applType])(valueIn)) {
                    status = static_cast<caStatus>(S_casApp_success);
                }
                else {
                    /*lint -e{9130} depends on external library*/
                    status = static_cast<caStatus>(S_cas_internal);
                }
            }
        }
        fmutex.FastUnLock();
    }
    return status;
}

caStatus MARTeCommonPv::ReadDirect(const AnyType& valueOut,
                                   const uint32 applType) {
    caStatus status = static_cast<caStatus>(S_casApp_success);

    if (static_cast<bool>(fmutex.FastLock())) {
        if (applType < MAX_N_FUNC) {
            if (this->functionDirectReadTable[applType] != NULL) {
                if ((this->*functionDirectReadTable[applType])(valueOut)) {
                    status = static_cast<caStatus>(S_casApp_success);
                }
                else {
                    /*lint -e{9130} depends on external library*/
                    status = static_cast<caStatus>(S_cas_internal);
                }
            }
        }
        fmutex.FastUnLock();
    }
    return status;
}

/*lint -e{715} ctx not referenced*/
caStatus MARTeCommonPv::write(const casCtx & ctx,
                              const gdd & value) {
    caStatus status = static_cast<caStatus>(S_casApp_success);
    if (static_cast<bool>(fmutex.FastLock())) {
        uint32 applType = value.applicationType();
        if (applType < MAX_N_FUNC) {
            if (this->functionWriteTable[applType] != NULL) {
                status = (this->*functionWriteTable[applType])(value);
            }
            else {
                /*lint -e{9130} depends on external library*/
                status = static_cast<caStatus>(S_cas_internal);
            }
        }
        fmutex.FastUnLock();
    }
    return status;
}

caStatus MARTeCommonPv::Update(const gdd & value) {
    caStatus status = static_cast<caStatus>(S_casApp_success);

    aitInt16 sta = 0;
    aitInt16 sev = static_cast<aitInt16>(NO_ALARM);
// fetch previous values
    value.getStatSevr(sta, sev);

    if (useLocalTs) {
        currentTimestamp = epicsTime::getCurrent();
    }

    TypeDescriptor td = pValue.GetTypeDescriptor();

    if (td.operator!=(Character8Bit)) {
        if (elementCount == 1u) {
            //check the limits
            float64 currentValue;
            bool changed = false;
            if (TypeConvert(currentValue, pValue)) {
                if (currentValue > pvHopr) {
                    currentValue = pvHopr;
                    changed = true;
                }
                if (currentValue < pvLopr) {
                    currentValue = pvLopr;
                    changed = true;
                }
                if (changed) {
                    if (!TypeConvert(pValue, currentValue)) {
                        /*lint -e{9130} depends on external library*/
                        status = static_cast<caStatus>(S_cas_noConvert);
                    }
                }
                if (status == static_cast<caStatus>(S_casApp_success)) {
                    aitInt16 nsev = 0;
                    aitInt16 nsta = 0;

                    bool lalmSet = false;
                    if (pvHhsv > 0) {
                        bool cond1 = (currentValue >= pvHihi);
                        bool cond2 = (IsEqual(lalm, pvHihi) && (currentValue >= (pvHihi - pvHyst)));
                        if (cond1 || cond2) {
                            if (nsev < (pvHhsv)) {
                                /*lint -e{930} required cast from enum to short*/
                                nsta = static_cast<aitInt16>(HIHI_ALARM);
                                nsev = pvHhsv;
                                lalm = pvHihi;
                                lalmSet = true;
                            }
                        }
                    }

                    // alarm condition lolo
                    if (pvLlsv > 0) {
                        bool cond1 = (currentValue <= pvLolo);
                        bool cond2 = (IsEqual(lalm, pvLolo) && (currentValue <= (pvLolo + pvHyst)));
                        if (cond1 || cond2) {
                            if (nsev < (pvLlsv)) {
                                /*lint -e{930} required cast from enum to short*/
                                nsta = static_cast<aitInt16>(LOLO_ALARM);
                                nsev = pvLlsv;
                                lalm = pvLolo;
                                lalmSet = true;
                            }
                        }
                    }

                    // alarm condition high
                    if (pvHsv > 0) {
                        bool cond1 = (currentValue >= pvHigh);
                        bool cond2 = (IsEqual(lalm, pvHigh) && (currentValue >= (pvHigh - pvHyst)));
                        if (cond1 || cond2) {
                            if (nsev < (pvHsv)) {
                                /*lint -e{930} required cast from enum to short*/
                                nsta = static_cast<aitInt16>(HIGH_ALARM);
                                nsev = pvHsv;
                                lalm = pvHigh;
                                lalmSet = true;
                            }
                        }
                    }

                    // alarm condition low
                    if (pvLsv > 0) {
                        bool cond1 = (currentValue <= pvLow);
                        bool cond2 = (IsEqual(lalm, pvLow) && (currentValue <= (pvLow + pvHyst)));
                        if (cond1 || cond2) {
                            if (nsev < (pvLsv)) {
                                /*lint -e{930} required cast from enum to short*/
                                nsta = static_cast<aitInt16>(LOW_ALARM);
                                nsev = pvLsv;
                                lalm = (pvLow);
                                lalmSet = true;
                            }
                        }
                    }

                    if (!lalmSet) {
                        lalm = currentValue;
                    }

                    epicsEnum16 stat_mask = 0u;
                    if (pvSev != nsev) {
                        pvSev = nsev;
                        /*lint -e{9130} depends on external library*/
                        stat_mask = static_cast<epicsEnum16>(DBE_ALARM);
                    }
                    if (pvSta != nsta) {
                        pvSta = nsta;
                        /*lint -e{9130} -e{835} depends on external library*/
                        stat_mask |= static_cast<epicsEnum16>(DBE_VALUE);
                    }
                    if ((interest) && (cas != NULL_PTR(MARTeIocServer*))) {
                        casEventMask monitor_mask;

                        //fetch previous values
                        if (stat_mask != 0u) {
                            monitor_mask |= cas->alarmEventMask();
                        }

                        float64 delta = (mlst - currentValue);
                        if (delta < 0.) {
                            delta = -delta;
                        }
                        if (delta > pvMdel) {
                            monitor_mask |= cas->valueEventMask();
                            mlst = currentValue;
                        }

                        //archiving
                        delta = (alst - currentValue);
                        if (delta < 0.) {
                            delta = -delta;
                        }

                        if (delta > pvAdel) {
                            monitor_mask |= cas->logEventMask();
                            alst = currentValue;
                        }

                        if (monitor_mask.eventsSelected()) {
                            postEvent(monitor_mask, value);
                        }
                    }
                }
            }
        }
    }
    return status;
}

caStatus MARTeCommonPv::PutAckt(const gdd & value) {
    caStatus status = static_cast<caStatus>(S_casApp_success);

    aitUint16 ack_value;
    value.get(ack_value);
    /*lint -e{930} conversion to enum required*/
    aitEnum ackValueEnum = static_cast<aitEnum>(ack_value);
    if (ackt != ackValueEnum) {
        ackt = ackValueEnum;

//do a post event to people monitoring alarm changes...
//db_post_events(precord, NULL, DBE_ALARM);
        if (cas != NULL_PTR(MARTeIocServer*)) {
            smartGDDPointer gddVal = new gdd(gddAppType_value);
            MARTePv::AnyTypeToGddConverter(*gddVal, pValue, myType);
            casEventMask sel(cas->alarmEventMask());
            postEvent(sel, *gddVal);
            if (gddVal->unreference() != 0) {
                /*lint -e{9130} depends on external library*/
                status = static_cast<caStatus>(S_cas_internal);
            }
        }
    }
    return status;
}

caStatus MARTeCommonPv::PutAcks(const gdd & value) {
    aitUint16 ack_value;
    value.get(ack_value);
    caStatus status = static_cast<caStatus>(S_casApp_success);
    /*lint -e{930} conversion to enum required*/
    aitEnum ackValueEnum = static_cast<aitEnum>(ack_value);

    if (ackValueEnum != acks) {
        acks = ackValueEnum;
        if (cas != NULL_PTR(MARTeIocServer*)) {
            smartGDDPointer gddVal = new gdd(gddAppType_value);
            MARTePv::AnyTypeToGddConverter(*gddVal, pValue, myType);
            casEventMask sel(cas->alarmEventMask());
            postEvent(sel, *gddVal);
            if (gddVal->unreference() != 0) {
                /*lint -e{9130} depends on external library*/
                status = static_cast<caStatus>(S_cas_internal);
            }
        }
    }
    return status;
}

void MARTeCommonPv::scan() {

    if (useLocalTs) {
        currentTimestamp = epicsTime::getCurrent();
    }
    if (cas != NULL_PTR(MARTeIocServer*)) {
        smartGDDPointer gddVal = new gdd(gddAppType_value);
        MARTePv::AnyTypeToGddConverter(*gddVal, pValue, myType);
        aitTimeStamp gddts(currentTimestamp);
        gddVal->setTimeStamp(&gddts);
        casEventMask sel = (cas->valueEventMask() | cas->logEventMask());
        postEvent(sel, *gddVal);
        (void) gddVal->unreference();
    }
}

void MARTeCommonPv::InitPv() {

    TypeDescriptor td = ConvertToTypeDescriptor(myType);
    bool isString = (td.operator==(Character8Bit));

    uint32 size = static_cast<uint32>(td.numberOfBits) / 8u;
    if (isString) {
        size *= static_cast<uint32>(MAX_STRING_SIZE);
    }
    uint32 totalSize = (elementCount * size);
    void *newPointer = HeapManager::Malloc(totalSize);
    (void) MemoryOperationsHelper::Set(newPointer, '\0', totalSize);
    pValue = AnyType(td, 0u, newPointer);
    if (elementCount > 1u) {
        pValue.SetNumberOfDimensions((isString) ? (2u) : (1u));
        pValue.SetNumberOfElements(0u, elementCount);
        if (isString) {
            pValue.SetNumberOfElements(0u, static_cast<uint32>(MAX_STRING_SIZE));
            pValue.SetNumberOfElements(1u, elementCount);
        }
    }
    else {
        if (isString) {
            pValue.SetNumberOfDimensions(1u);
            pValue.SetNumberOfElements(0u, static_cast<uint32>(MAX_STRING_SIZE));
        }
    }
    (void) TypeConvert(pValue, pvInitialVal.Buffer());

}

caStatus MARTeCommonPv::writeNotify(const casCtx & ctx,
                                    const gdd & value) {

    return write(ctx, value);
}

CLASS_REGISTER(MARTeCommonPv, "1.0")

}

