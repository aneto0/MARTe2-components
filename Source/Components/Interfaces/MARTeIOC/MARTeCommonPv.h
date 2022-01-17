/**
 * @file MARTeCommonPv.h
 * @brief Header file for class MARTeCommonPv
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

 * @details This header file contains the declaration of the class MARTeCommonPv
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SOURCE_COMPONENTS_INTERFACES_MARTEIOC_MARTEAIPV_H_
#define SOURCE_COMPONENTS_INTERFACES_MARTEIOC_MARTEAIPV_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "MARTePv.h"
#include "FastPollingMutexSem.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/



namespace MARTe {

/**
 * The number of application id already registered in EPICS lib
 */
static const uint32 MAX_APP_TYPE = 35u;
/**
 * The number of needed application id
 */
static const uint32 MAX_N_FUNC = (MAX_APP_TYPE+10u);
/**
 * HYST identifier
 */
static const uint32 gddAppType_hyst= (MAX_APP_TYPE + 1u);
/**
 * MDEL identifier
 */
static const uint32 gddAppType_mdel= (MAX_APP_TYPE + 2u);
/**
 * ADEL identifier
 */
static const uint32 gddAppType_adel= (MAX_APP_TYPE + 3u);
/**
 * HHSV identifier
 */
static const uint32 gddAppType_hhsv= (MAX_APP_TYPE + 4u);
/**
 * HSV identifier
 */
static const uint32 gddAppType_hsv= (MAX_APP_TYPE + 5u);
/**
 * LLSV identifier
 */
static const uint32 gddAppType_lsv= (MAX_APP_TYPE + 6u);
/**
 * LLSV identifier
 */
static const uint32 gddAppType_llsv= (MAX_APP_TYPE + 7u);
/**
 * STAT identifier
 */
static const uint32 gddAppType_stat= (MAX_APP_TYPE + 8u);
/**
 * SEVR identifier
 */
static const uint32 gddAppType_sevr= (MAX_APP_TYPE + 9u);

/**
 * @brief MARTe implementation of a Common PV.
 *
 * @details This component implements one of the most common PVs types. This PV exports the following fields:
 *   VAL: (string) The initial value of the PV
 *   EGU: (String) The measurement unit
 *   HOPR: (float64) The upper limit value
 *   LOPR: (float64) The lower limit value
 *   HIHI: (float64) The upper alarm value //set to HOPR if not specified
 *   HIGH: (float64) The upper warning value //set to HOPR if not specified
 *   LOW: (float64) The lower warning value //set to LOPR if not specified
 *   LOLO: (float64) The lower alarm value //set to LOPR if not specified
 *   HHSV: (string) (NO_ALARAM, MINOR, MAJOR, INVALID) The upper alarm severity
 *   HSV: (string) (NO_ALARAM, MINOR, MAJOR, INVALID) The upper warning severity
 *   LSV: (string) (NO_ALARAM, MINOR, MAJOR, INVALID) The lower warning severity
 *   LLSV: (String) (NO_ALARAM, MINOR, MAJOR, INVALID) The lower alarm severity
 *   HYST: (float64) The PV hysteresis
 *   MDEL: (float64) The monitor delta
 *   ADEL: (float64) The archiving delta
 *
 * @details The following behavior is true if the PV is scalar and not string:\n
 *   - The PV value is always in the range [HOPR, LOPR]. If a write operation attempts to set the PV value outside of that range,
 *   the value is saturated to its limit.\n
 *   - The PV can have four threshold levels: HIHI (alarm high threshold), HIGH (warning high threshold), LOW (warning low threshold), LOLO (alarm
 *   low threshold.\n
 *   - To each of this threshold, the user can set one of the following severities: NO_ALARM, MINOR, MAJOR, INVALID. HHSEV is the severity for the
 *   HIHI threshold, HSEV for the HIGH threshold, LSEV for the LOW threshold, LLSEV for the LOLO threshold. Using the HYST parameter, the user can
 *   set the hysteresis around a threshold.\n
 *   - If the variation between the current PV value and the last monitored value is greater than MDEL, then refresh the last monitored value with
 *   the current one and set the monitor event.\n
 *   - If the variation between the current PV value and the last archived value is greater than ADEL, then refresh the last archived value with
 *   the current one and set the log event.\n
 *   - If the variable is marked as \a interest the event is posted (alarm event if the PV value passes a threshold, value event if mdel, log event if adel).
 *
 * @details It is possible to read and write most of the fields associated to this PV. The field is identified by the application id that is set within the
 * EPICS basic variable gdd, or it can be explicitly passed in input in the ReadDirect and WriteDirect operations. Follows a list with all the field
 * application identifiers:\n
 *   - gddAppType_maxElements -- read NELM\n
 *   - gddAppType_units -- read/write EGU\n
 *   - gddAppType_controlHigh -- read/write HOPR\n
 *   - gddAppType_controlLow -- read/write LOPR\n
 *   - gddAppType_alarmHigh -- read/write HIHI\n
 *   - gddAppType_alarmLow -- read/write LOLO\n
 *   - gddAppType_alarmHighWarning -- read/write HIGH\n
 *   - gddAppType_alarmLowWarning -- read/write LOW\n
 *   - gddAppType_hhsv -- read/write HHSV\n
 *   - gddAppType_hsv -- read/writ HSV\n
 *   - gddAppType_lsv -- read/write LSV\n
 *   - gddAppType_llsv -- read/write LLSV\n
 *   - gddAppType_hyst -- read/write HYST\n
 *   - gddAppType_adel -- read/write ADEL\n
 *   - gddAppType_mdel -- read/write MDEL\n
 *   - gddAppType_precision -- read/write PREC\n
 *   - gddAppType_stat -- read/write the PV status (STAT can be one of the following: [NO_ALARM, HIHI_ALARM, HIGH_ALARM, LOLO_ALARM, LOW_ALARM])\n
 *   - gddAppType_sevr -- read/write the PV severity (SEVR can be one of the following: [NO_ALARM, MINOR, MAJOR, INVALID]\n
 *
 * @details During the initialisation phase, a \a MARTePvField reference is created for each field and inserted in this ReferenceContainer. This allows
 * the \a MARTeIocServer to find the PV fields as PVs and to pass them to the channel access to be monitored and read/written in EPICS.
 *
 * @details Follows an example of configuration:\n
 * <pre>
 * +PS_VOLTAGE = {
 *    Class = MARTeCommonPv
 *    TYPE = aitEnumFloat64
 *    VAL = 2.0
 *    NELM = 1
 *    PREC = 4
 *    EGU = volt
 *    HOPR = 10.0"
 *    LOPR = -10.0"
 *    HIHI = 9.0"
 *    HIGH = 8.0"
 *    LOW = 0.0"
 *    LOLO = -5.0"
 *    HHSV =  MAJOR
 *    HSV = MINOR"
 *    LSV = MINOR
 *    LLSV = MAJOR
 *    HYST = 0.0001"
 *    ADEL = 0.0001"
 *    MDEL = 0.01";
 *}
 * </pre>
 */
class MARTeCommonPv: public MARTePv {
public:

    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor
     */
    MARTeCommonPv();

    /**
     * @brief Destructor
     */
    virtual ~MARTeCommonPv();

    /**
     * @brief Initialises the PV.
     * @see MARTePv::Initialise.
     * @details The user can define the following fields:\n
     *   VAL: (string) The initial value of the PV\n
     *   EGU: (String) The measurement unit\n
     *   HOPR: (float64) The upper limit value\n
     *   LOPR: (float64) The lower limit value\n
     *   HIHI: (float64) The upper alarm value //set to HOPR if not specified\n
     *   HIGH: (float64) The upper warning value //set to HOPR if not specified\n
     *   LOW: (float64) The lower warning value //set to LOPR if not specified\n
     *   LOLO: (float64) The lower alarm value //set to LOPR if not specified\n
     *   HHSV: (string) (NO_ALARAM, MINOR, MAJOR, INVALID) The upper alarm severity\n
     *   HSV: (string) (NO_ALARAM, MINOR, MAJOR, INVALID) The upper warning severity\n
     *   LSV: (string) (NO_ALARAM, MINOR, MAJOR, INVALID) The lower warning severityv
     *   LLSV: (String) (NO_ALARAM, MINOR, MAJOR, INVALID) The lower alarm severityv
     *   HYST: (float64) The PV hysteresis\n
     *   MDEL: (float64) The monitor delta\n
     *   ADEL: (float64) The archiving delta\n
     */
    virtual bool Initialise(StructuredDataI &data);

    /**
     * @brief Reads the PV value or the value or one of its fields.
     * @see MARTePv::read
     * @details Depending on the application id of \a prototype, it can read the
     * PV value (application id = 16) or one of the PV fields. The following fields can be
     * read:\n
     *   - NELM (id=gddAppType_maxElements)\n
     *   - EGU (id=gddAppType_units)\n
     *   - HOPR (id=gddAppType_controlHigh)\n
     *   - LOPR (id=gddAppType_controlLow)\n
     *   - HIHI (id=gddAppType_alarmHigh)\n
     *   - LOLO (id=gddAppType_alarmLow)\n
     *   - HIGH (id=gddAppType_alarmHighWarning)\n
     *   - LOW (id=gddAppType_alarmLowWarning)\n
     *   - HHSV (id=gddAppType_hhsv)\n
     *   - HSV (id=gddAppType_hsv)\n
     *   - LSV (id=gddAppType_lsv)\n
     *   - LLSV (id=gddAppType_llsv)\n
     *   - HYST (id=gddAppType_hyst)\n
     *   - ADEL (id=gddAppType_adel)\n
     *   - MDEL (id=gddAppType_mdel)\n
     *   - PREC (id=gddAppType_precision)\n
     *   - STAT (id=gddAppType_stat)\n
     *   - SEVR (id=gddAppType_sevr)\n
     * @param[in] ctx the context
     * @param[out] prototype the read value.
     * @return S_cas_success in case of operation success, S_cas_internal in case of error.
     */
    virtual caStatus read(const casCtx &ctx,
                          gdd & prototype);

    /**
     * @brief Writes the PV value or the value or one of its fields.
     * @see MARTePv::write
     * @details Depending on the application id of \a prototype, it can write the
     * PV value (application id = 16) or one of the PV fields. The following fields can be
     * written:\n
     *   - EGU (id=gddAppType_units)\n
     *   - HOPR (id=gddAppType_controlHigh)\n
     *   - LOPR (id=gddAppType_controlLow)\n
     *   - HIHI (id=gddAppType_alarmHigh)\n
     *   - LOLO (id=gddAppType_alarmLow)\n
     *   - HIGH (id=gddAppType_alarmHighWarning)\n
     *   - LOW (id=gddAppType_alarmLowWarning)\n
     *   - HHSV (id=gddAppType_hhsv)\n
     *   - HSV (id=gddAppType_hsv)\n
     *   - LSV (id=gddAppType_lsv)\n
     *   - LLSV (id=gddAppType_llsv)\n
     *   - HYST (id=gddAppType_hyst)\n
     *   - ADEL (id=gddAppType_adel)\n
     *   - MDEL (id=gddAppType_mdel)\n
     *   - PREC (id=gddAppType_precision)\n
     * @param[in] ctx the context
     * @param[in] value the value to be written.
     * @return S_cas_success in case of operation success, S_cas_internal in case of error.
     */
    virtual caStatus write(const casCtx &ctx,
                           const gdd & value);


    /**
     * @brief Writes the PV value or the value or one of its fields from inside the IOC.
     * @see MARTePv::WriteDirect
     * @details Depending on \a applType, it can write the PV value (application id = 16) or one of the PV fields.
     * The following fields can be written:\n
     *   - EGU (id=gddAppType_units)\n
     *   - HOPR (id=gddAppType_controlHigh)\n
     *   - LOPR (id=gddAppType_controlLow)\n
     *   - HIHI (id=gddAppType_alarmHigh)\n
     *   - LOLO (id=gddAppType_alarmLow)\n
     *   - HIGH (id=gddAppType_alarmHighWarning)\n
     *   - LOW (id=gddAppType_alarmLowWarning)\n
     *   - HHSV (id=gddAppType_hhsv)\n
     *   - HSV (id=gddAppType_hsv)\n
     *   - LSV (id=gddAppType_lsv)\n
     *   - LLSV (id=gddAppType_llsv)\n
     *   - HYST (id=gddAppType_hyst)\n
     *   - ADEL (id=gddAppType_adel)\n
     *   - MDEL (id=gddAppType_mdel)\n
     *   - PREC (id=gddAppType_precision)\n
     * @param[in] valueIn
     * @param[in] applType the field id.
     * @return S_cas_success in case of operation success, S_cas_internal in case of error.
     */
    /*lint -e{1735} default parameter*/
    virtual caStatus WriteDirect(const AnyType& valueIn, const uint32 applType=16u);

    /**
     * @brief Reads the PV value or the value or one of its fields from inside the IOC.
     * @see MARTePv::ReadDirect
     * @details Depending on \a applType, it can read the PV value (application id = 16) or one of the PV fields.
     * The following fields can be read:\n
     *   - NELM (id=gddAppType_maxElements)\n
     *   - EGU (id=gddAppType_units)\n
     *   - HOPR (id=gddAppType_controlHigh)\n
     *   - LOPR (id=gddAppType_controlLow)\n
     *   - HIHI (id=gddAppType_alarmHigh)\n
     *   - LOLO (id=gddAppType_alarmLow)\n
     *   - HIGH (id=gddAppType_alarmHighWarning)\n
     *   - LOW (id=gddAppType_alarmLowWarning)\n
     *   - HHSV (id=gddAppType_hhsv)\n
     *   - HSV (id=gddAppType_hsv)\n
     *   - LSV (id=gddAppType_lsv)\n
     *   - LLSV (id=gddAppType_llsv)\n
     *   - HYST (id=gddAppType_hyst)\n
     *   - ADEL (id=gddAppType_adel)\n
     *   - MDEL (id=gddAppType_mdel)\n
     *   - PREC (id=gddAppType_precision)\n
     *   - STAT (id=gddAppType_stat)\n
     *   - SEVR (id=gddAppType_sevr)\n
     * @param[out] valueOut the value to be read.
     * @param[out] in the field id.
     * @return S_cas_success in case of operation success, S_cas_internal in case of error.
     */
    /*lint -e{1735} default parameter*/
    virtual caStatus ReadDirect(const AnyType& valueOut, const uint32 applType=16u);

    /**
     * @see MARTePv::scan
     * @details Refreshes the timestamp and, if set as \a interest, posts an event.
     */
    virtual void scan();


    /**
     * @see write.
     */
    virtual caStatus writeNotify(const casCtx & ctx,
                                 const gdd & value);

protected:

    /**
     * @brief Called when the PV value changes to change the PV status and severity, trigger alarms, etc.
     * @param[in] value is the new PV value to be updated.
     * @return S_cas_success in case of operation success, S_cas_internal in case of error.
     */
    virtual caStatus Update(const gdd &value);

    /**
     * @brief Initialises the PV allocating the memory to hold the value and setting the initialisation value.
     */
    virtual void InitPv();

    /**
     * @brief Creates and inserts in the ReferenceContainer a ReferenceT<MARTePvField> for each field. It allows
     * the MARTeIocServer to find the PV fields and to attach them to the channel access as a standard PVs (in order
     * to be read, written and monitored in EPICS).
     */
    void InsertRefs();

    /**
     * @brief Reads the PV PREC field
     */
    caStatus GetPrecision(gdd &value) const;
    /**
     * @brief Reads the PV HOPR field
     */
    caStatus GetHighLimit(gdd &value) const;
    /**
     * @brief Reads the PV LOPR field
     */
    caStatus GetLowLimit(gdd &value) const;
    /**
     * @brief Reads the PV HIHI field
     */
    caStatus GetHighAlarm(gdd & value) const;
    /**
     * @brief Reads the PV LOLO field
     */
    caStatus GetLowAlarm(gdd & value) const;
    /**
     * @brief Reads the PV HIGH field
     */
    caStatus GetHighWarning(gdd & value) const;
    /**
     * @brief Reads the PV LOW field
     */
    caStatus GetLowWarning(gdd & value) const;
    /**
     * @brief Reads the PV EGU field
     */
    caStatus GetUnits(gdd &value) const;
    /**
     * @brief Reads the PV value
     */
    caStatus GetValue(gdd &value) const;
    /**
     * @brief NOOP
     */
    caStatus GetEnums(gdd &) const;
    /**
     * @brief Reads the PV ACKT field
     */
    caStatus GetAckt(gdd & value) const;
    /**
     * @brief Reads the PV ACKS field
     */
    caStatus GetAcks(gdd & value) const;
    /**
     * @brief Reads the PV NELM field
     */
    caStatus GetElementCount(gdd & value) const;
    /**
     * @brief Sets the PV timestamp in \a value
     */
    caStatus GetTimestamp(gdd & value) const;
    /**
     * @brief Reads the PV HYST field
     */
    caStatus GetHyst(gdd & value) const;
    /**
     * @brief Reads the PV MDEL field
     */
    caStatus GetMdel(gdd & value) const;
    /**
     * @brief Reads the PV ADEL field
     */
    caStatus GetAdel(gdd & value) const;
    /**
     * @brief Reads the PV HHSV field
     */
    caStatus GetHhsv(gdd & value) const;
    /**
     * @brief Reads the PV HSV field
     */
    caStatus GetHsv(gdd & value) const;
    /**
     * @brief Reads the PV LSV field
     */
    caStatus GetLsv(gdd & value) const;
    /**
     * @brief Reads the PV LLSV field
     */
    caStatus GetLlsv(gdd & value) const;
    /**
     * @brief Reads the PV STAT field
     */
    caStatus GetStat(gdd & value) const;
    /**
     * @brief Reads the PV SEVR field
     */
    caStatus GetSevr(gdd & value) const;


    /**
     * @brief Reads the PV PREC field from inside the IOC
     */
    bool GetDirectPrecision(const AnyType &value) const;

    /**
     * @brief Reads the PV HOPR field from inside the IOC
     */
    bool GetDirectHighLimit(const AnyType &value) const;
    /**
     * @brief Reads the PV LOPR field from inside the IOC
     */
    bool GetDirectLowLimit(const AnyType &value) const;
    /**
     * @brief Reads the PV HIHI field from inside the IOC
     */
    bool GetDirectHighAlarm(const AnyType &value) const;
    /**
     * @brief Reads the PV LOLO field from inside the IOC
     */
    bool GetDirectLowAlarm(const AnyType &value) const;
    /**
     * @brief Reads the PV HIGH field from inside the IOC
     */
    bool GetDirectHighWarning(const AnyType &value) const;
    /**
     * @brief Reads the PV LOW field from inside the IOC
     */
    bool GetDirectLowWarning(const AnyType &value) const;
    /**
     * @brief Reads the PV EGU field from inside the IOC
     */
    bool GetDirectUnits(const AnyType &value) const;
    /**
     * @brief Reads the PV value from inside the IOC
     */
    bool GetDirectValue(const AnyType &value) const;
    /**
     * @brief NOOP
     */
    bool GetDirectEnums(const AnyType &) const;
    /**
     * @brief Reads the PV ACKT field from inside the IOC
     */
    bool GetDirectAckt(const AnyType &value) const;
    /**
     * @brief Reads the PV ACKS field from inside the IOC
     */
    bool GetDirectAcks(const AnyType &value) const;
    /**
     * @brief Reads the PV NELM field from inside the IOC
     */
    bool GetDirectElementCount(const AnyType &value) const;
    /**
     * @brief Reads the PV Timestamp field from inside the IOC.
     * @details It is necessary to cast \a value (that has to be uint64) to epicsTimeStamp
     * to extract the \a secPastEpoch (uint32) and nsec (uint32) timestamp fields.
     */
    bool GetDirectTimeStamp(const AnyType &value) const;

    /**
     * @brief Reads the PV HYST field from inside the IOC
     */
    bool GetDirectHyst(const AnyType &value) const;
    /**
     * @brief Reads the PV MDEL field from inside the IOC
     */
    bool GetDirectMdel(const AnyType &value) const;
    /**
     * @brief Reads the PV ADEL field from inside the IOC
     */
    bool GetDirectAdel(const AnyType &value) const;
    /**
     * @brief Reads the PV HHSV field from inside the IOC
     */
    bool GetDirectHhsv(const AnyType &value) const;
    /**
     * @brief Reads the PV HSV field from inside the IOC
     */
    bool GetDirectHsv(const AnyType &value) const;
    /**
     * @brief Reads the PV LSV field from inside the IOC
     */
    bool GetDirectLsv(const AnyType &value) const;
    /**
     * @brief Reads the PV LSV field from inside the IOC
     */
    bool GetDirectLlsv(const AnyType &value) const;
    /**
     * @brief Reads the PV STAT field from inside the IOC
     */
    bool GetDirectStat(const AnyType &value) const;
    /**
     * @brief Reads the PV SEVR field from inside the IOC
     */
    bool GetDirectSevr(const AnyType &value) const;

    /**
     * @brief Writes the PV PREC field
     */
    caStatus PutPrecision(const gdd &value);
    /**
     * @brief Writes the PV HOPR field
     */
    caStatus PutHighLimit(const gdd &value);
    /**
     * @brief Writes the PV LOPR field
     */
    caStatus PutLowLimit(const gdd &value);
    /**
     * @brief Writes the PV HIHI field
     */
    caStatus PutHighAlarm(const gdd & value);
    /**
     * @brief Writes the PV LOLO field
     */
    caStatus PutLowAlarm(const gdd & value);
    /**
     * @brief Writes the PV HIGH field
     */
    caStatus PutHighWarning(const gdd & value);
    /**
     * @brief Writes the PV LOW field
     */
    caStatus PutLowWarning(const gdd & value);
    /**
     * @brief Writes the PV EGU field
     */
    caStatus PutUnits(const gdd &value);
    /**
     * @brief Writes the PV value
     */
    caStatus PutValue(const gdd &value);
    /**
     * @brief NOOP
     */
    caStatus PutEnums(const gdd &);
    /**
     * @brief Writes the PV ACKT field
     * @details If the current acks is different than \a value an alarm event is posted.
     */
    caStatus PutAckt(const gdd & value);
    /**
     * @brief Writes the PV ACKS field.
     * @details If the current acks is different than \a value an alarm event is posted.
     */
    caStatus PutAcks(const gdd & value);
    /**
     * @brief Sets to this PV the same timestamp of \a value
     */
    caStatus PutTimeStamp(const gdd & value);
    /**
     * @brief Writes the PV HYST field
     */
    caStatus PutHyst(const gdd & value);
    /**
     * @brief Writes the PV MDEL field
     */
    caStatus PutMdel(const gdd & value);
    /**
     * @brief Writes the PV ADEL field
     */
    caStatus PutAdel(const gdd & value);
    /**
     * @brief Writes the PV HHSV field
     */
    caStatus PutHhsv(const gdd & value);
    /**
     * @brief Writes the PV HSV field
     */
    caStatus PutHsv(const gdd & value);
    /**
     * @brief Writes the PV LSV field
     */
    caStatus PutLsv(const gdd & value);
    /**
     * @brief Writes the PV LLSV field
     */
    caStatus PutLlsv(const gdd & value);

    /**
     * @brief Writes the PV PREC field from inside the IOC
     */
    bool PutDirectPrecision(const AnyType &value);
    /**
     * @brief Writes the PV HOPR field from inside the IOC
     */
    bool PutDirectHighLimit(const AnyType &value);
    /**
     * @brief Writes the PV LOPR field from inside the IOC
     */
    bool PutDirectLowLimit(const AnyType &value);
    /**
     * @brief Writes the PV HIHI field from inside the IOC
     */
    bool PutDirectHighAlarm(const AnyType &value);
    /**
     * @brief Writes the PV LOLO field from inside the IOC
     */
    bool PutDirectLowAlarm(const AnyType &value);
    /**
     * @brief Writes the PV HIGH field from inside the IOC
     */
    bool PutDirectHighWarning(const AnyType &value);
    /**
     * @brief Writes the PV LOW field from inside the IOC
     */
    bool PutDirectLowWarning(const AnyType &value);
    /**
     * @brief Writes the PV EGU field from inside the IOC
     */
    bool PutDirectUnits(const AnyType &value);
    /**
     * @brief Writes the PV value from inside the IOC
     */
    bool PutDirectValue(const AnyType &value);
    /**
     * @brief NOOP
     */
    bool PutDirectEnums(const AnyType &);
    /**
     * @brief Writes the PV ACKT field from inside the IOC
     */
    bool PutDirectAckt(const AnyType &value);
    /**
     * @brief Writes the PV ACKS field from inside the IOC
     */
    bool PutDirectAcks(const AnyType &value);
    /**
     * @brief Writes the PV timestamp from inside the IOC
     * @details The \a value in input has to be uint64 and it is casted
     * to epicsTimeStamp to read the timestamp to set to this PV.
     */
    bool PutDirectTimeStamp(const AnyType & value);
    /**
     * @brief Writes the PV HYST field from inside the IOC
     */
    bool PutDirectHyst(const AnyType & value);
    /**
     * @brief Writes the PV MDEL field from inside the IOC
     */
    bool PutDirectMdel(const AnyType & value);
    /**
     * @brief Writes the PV ADEL field from inside the IOC
     */
    bool PutDirectAdel(const AnyType & value);
    /**
     * @brief Writes the PV HHSV field from inside the IOC
     */
    bool PutDirectHhsv(const AnyType &value);
    /**
     * @brief Writes the PV HSV field from inside the IOC
     */
    bool PutDirectHsv(const AnyType &value);
    /**
     * @brief Writes the PV LSV field from inside the IOC
     */
    bool PutDirectLsv(const AnyType &value);
    /**
     * @brief Writes the PV LLSV field from inside the IOC
     */
    bool PutDirectLlsv(const AnyType &value);

    /**
     * The internal mutex
     */
    FastPollingMutexSem fmutex;
    /**
     * Holds the unit string
     */
    char8 pvUnits[MAX_STRING_SIZE];
    /**
     * Holds the initial value
     */
    StreamString pvInitialVal;
    /**
     * Holds the high limit value
     */
    float64 pvHopr;
    /**
     * Holds the low limit value
     */
    float64 pvLopr;
    /**
     * Holds the high alarm value
     */
    float64 pvHihi;
    /**
     * Holds the low alarm value
     */
    float64 pvLolo;
    /**
     * Holds the high warning value
     */
    float64 pvHigh;
    /**
     * Holds the low warning value
     */
    float64 pvLow;
    /**
     * Holds the high alarm severity
     */
    aitInt16 pvHhsv;
    /**
     * Holds the high warning severity
     */
    aitInt16 pvHsv;
    /**
     * Holds the low warning severity
     */
    aitInt16 pvLsv;
    /**
     * Holds the low alarm severity
     */
    aitInt16 pvLlsv;
    /**
     * Holds the hysteresis value
     */
    float64 pvHyst;
    /**
     * Holds the archiving delta value
     */
    float64 pvAdel;
    /**
     * Holds the monitoring delta value
     */
    float64 pvMdel;
    /**
     * Holds the precision value.
     */
    uint32 pvPrec;
    /**
     * Holds the last alarm value
     */
    float64 lalm;
    /**
     * Holds the last archived value
     */
    float64 alst;
    /**
     * Holds the last monitored value
     */
    float64 mlst;
    /**
     * Holds the acks value
     */
    aitEnum acks;
    /**
     * Holds the ackt value
     */
    aitEnum ackt;

    /**
     * Holds the current severity
     */
    aitInt16 pvSev;
    /**
     * Holds the current status
     */
    aitInt16 pvSta;

    /**
     * Read functions table
     */
    caStatus (MARTeCommonPv::*functionReadTable[MAX_N_FUNC])(gdd &) const;
    /**
     * Write functions table
     */
    caStatus (MARTeCommonPv::*functionWriteTable[MAX_N_FUNC])(const gdd &);

    /**
     * Direct read functions table
     */
    bool (MARTeCommonPv::*functionDirectReadTable[MAX_N_FUNC])(const AnyType &) const;
    /**
     * Direct write functions table
     */
    bool (MARTeCommonPv::*functionDirectWriteTable[MAX_N_FUNC])(const AnyType &);


};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/


}


#endif /* SOURCE_COMPONENTS_INTERFACES_MARTEIOC_MARTEAIPV_H_ */

