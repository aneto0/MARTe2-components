/**
 * @file MARTePv.h
 * @brief Header file for class MARTePv
 * @date 01/07/2019
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

 * @details This header file contains the declaration of the class MARTePv
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SOURCE_COMPONENTS_INTERFACES_MARTEIOC_MARTEPV_H_
#define SOURCE_COMPONENTS_INTERFACES_MARTEIOC_MARTEPV_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
#include "tsSLList.h"
#include "smartGDDPointer.h"
#include "epicsTimer.h"
#include "gddAppFuncTable.h"

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "ConfigurationDatabase.h"
#include "MARTeIocServer.h"
#include "ReferenceContainer.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Interface of MARTe PV.
 *
 * @details This class inherits from casPV and implements the basic MARTe PV methods. It contains an AnyType holding the current value of the PV
 * and implements the static method GddToAnyTypeConverter to convert from the EPICS basic type (gdd) to AnyType.
 *
 * @details The methods \a WriteDirect and \a ReadDirect shall be used to read and write the PVs within the IOC scope. The inherited \a read and \a write
 * methods can be used to read and write the PVs passing through the channel access.
 *
 * @details The PV can be marked as \a interest. This means that an event is triggered every time something changes to allow the PV be monitored.
 *
 * @details The timestamp of the PV, namely the last time the PV has been written, can be set by the user before a write operation or can be set
 * automatically using the local time.
 */
/*lint -e{1790} destructor of tsSLNode is not trivial but it depends by external library*/
class MARTePv: public casPV, public epicsTimerNotify, public tsSLNode<MARTePv>, public ReferenceContainer {
public:

    /**
     * @brief Constructor
     */
    MARTePv();

    /**
     * @brief Destructor
     */
    /*lint -e{1510} trivial destructor depends on external library*/
    virtual ~MARTePv();

    /**
     * @brief Initialises the component
     * @details The user must specify the following parameter:\n
     *   TYPE = the PV type. This can be one of the following: {aitEnumInvalid, aitEnumInt8, aitEnumUint8, aitEnumInt16, aitEnumUint16, aitEnumEnum16, aitEnumInt32,
     *   aitEnumUint32, aitEnumFloat32, aitEnumFloat64, aitEnumFixedString, aitEnumString, aitEnumContainer}\n
     * The user can specify one of the following parameters:\n
     *   LEN = the PV number of elements (default is 1 (scalar))
     *   SCAN = the PV scan period in seconds (default is 0 (no-scan))
     */
    virtual bool Initialise(StructuredDataI &data);

    /**
     *  @brief Called by the server library each time that it wishes to subscribe for PV the server tool via postEvent() below.
     *  @details Marking a PV as \a interest means that the scan is activated (if SCAN period is > 0) and that events are posted in
     *  case of alarm risings (overflow or underflow with respect the PV value limits)
     */
    virtual caStatus interestRegister();

    /**
     * @brief Called by the server library each time that it wishes to remove its subscription for PV value change events
     * from the server tool via caServerPostEvents()
     * @details The scan timer is deactivated and the events are not posted in case of alarms.
     */
    virtual void interestDelete();

    /**
     * @brief Retrieves the PV internal type.
     */
    virtual aitEnum bestExternalType() const;

    /**
     * @brief Updates the time-stamp and post the event
     */
    virtual void scan()=0;

    /**
     * @brief Retrieves the scan period.
     */
    float64 GetScanPeriod() const;

    /**
     * @brief Destroys the PV (NOOP)
     */
    virtual void destroy();

    /**
     * @brief Retrieves the PV name.
     */
    virtual const char8 *getName() const;

    /**
     * @brief Creates a \a MARTeChannel.
     * @param[in] ctx the ca context.
     * @param[in] pUserName the user name
     * @param[in] pHostName the host name
     * @return a new MARTeChannel.
     */
    virtual casChannel *createChannel(const casCtx &ctx,
                                      const char8 * const pUserName,
                                      const char8 * const pHostName);

    /**
     * @brief Sets the IOC server this PV belongs to.
     * @param[in] casIn a reference to the MARTeIocServer
     */
    void SetIocServer(MARTeIocServer &casIn);

    /**
     * @brief Retrieves the number of elements of this PV
     */
    uint32 GetNumberOfElements() const;

    /**
     * @brief Choose to use the local time-stamp or the last time-stamp set.
     * @param[in] useLocalTsIn if true the local time is set at the PV update, if false the last
     * time-stamp set using \a SetTimeStamp is used.
     */
    void SetUseLocalTimeStamp(const bool useLocalTsIn);

    /**
     * @brief Retrieves if the local or the external time-stamp is used.
     * @return true if the local time-stamp is being used, false if the external time-stamp is used.
     */
    bool UseLocalTimeStamp() const;

    /**
     * @brief Retrieves the time-stamp.
     * @param[out] secs the time-stamp seconds.
     * @param[out] nanosecs the time-stamp nanoseconds.
     */
    void GetTimeStamp(uint32 &secs,
                      uint32 &nanosecs) const;

    /**
     * @brief Sets the external time-stamp.
     * @param[in] the time-stamp seconds.
     * @param[in] the time-stamp nanoseconds.
     */
    void SetTimeStamp(const uint32 secs,
                      const uint32 nanosecs);

    /**
     * @brief Converts the EPICS base type gdd to AnyType.
     * @param[out] output the converted AnyType.
     * @param[in] input the gdd to be converted.
     * @param[in] newPointer if to allocate new memory for the AnyType or to set directly
     * the pointer of the gdd in input.
     */
    static bool GddToAnyTypeConverter(AnyType &output,
                                      const gdd &input,
                                      const bool newPointer = true);

    /**
     * @brief Converts the AnyType to the EPICS base type gdd.
     * @param[out] output the converted gdd in output.
     * @param[in] input the AnyType to be converted.
     * @param[in] typeIn the type of the gdd.
     */
    static void AnyTypeToGddConverter(gdd & output,
                                      const AnyType &input,
                                      const aitEnum typeIn);

    /**
     * @brief Converts a aitEnum type to the relative TypeDescriptor
     * @param[in] typeIn the aitEnumType to be converted/
     * @return the converted TypeDescriptor
     */
    static TypeDescriptor ConvertToTypeDescriptor(const aitEnum typeIn);

    /**
     * @brief Writes the PV without passing through the channel access
     * @param[in] valueIn is the value of the PV to be set
     */
    /*lint -e{1735} default parameter*/
    virtual caStatus WriteDirect(const AnyType& valueIn,
                                 const uint32 applType = 16u)=0;

    /**
     * @brief Reads the PV without passing through the channel access
     * @param[in] valueOut is the value of the PV to be set
     */
    /*lint -e{1735} default parameter*/
    virtual caStatus ReadDirect(const AnyType& valueOut,
                                const uint32 applType = 16u)=0;

    /**
     * @brief Retrieves the AnyType
     */
    AnyType GetAnyType() const;

    /**
     * @brief Retrieves the PV dimensions
     */
    virtual uint32 maxDimension() const; // return zero if scalar

    /**
     * @brief Retrieves the PV number of elements for each dimension
     * @param[in] dimension the dimension
     */
    virtual aitIndex maxBound(const uint32 dimension) const;

    /**
     * @brief The timer expire callback
     * @details Calls scan() when the scan period time expires.
     */
    virtual expireStatus expire(const epicsTime &);


protected:

    /**
     * @brief Initialises the PV
     */
    virtual void InitPv();

    /**
     * @brief Retrieves the type from its name in string format.
     * @param[in] typeIn the type name
     */
    static aitEnum ConvertToaitEnum(const char8 * const typeIn);

    /**
     * The PV type
     */
    aitEnum myType;

    /**
     * The PV number of elements
     */
    uint32 elementCount;

    /**
     * The PV scan period
     */
    float64 pvScanPeriod;

    /**
     * If the PV is marked as interest
     */
    bool interest;

    /**
     * The PV in AnyType format
     */
    AnyType pValue;

    /**
     * The IOC server that holds this PV
     */
    MARTeIocServer *cas;

    /**
     * A pointer to the scan timer
     */
    epicsTimer *timer;

    /**
     * Specifies if to use the local or the external time-stamp
     */
    bool useLocalTs;

    /**
     * Holds the last time-stamp set
     */
    epicsTime currentTimestamp;

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

inline uint32 MARTePv::GetNumberOfElements() const {
    return elementCount;

}

inline float64 MARTePv::GetScanPeriod() const {
    return pvScanPeriod;
}

inline void MARTePv::GetTimeStamp(uint32 &secs,
                                  uint32 &nanosecs) const {
    epicsTimeStamp currTime = currentTimestamp;
    secs = currTime.secPastEpoch;
    nanosecs = currTime.nsec;
}

inline bool MARTePv::UseLocalTimeStamp() const {
    return useLocalTs;
}

inline AnyType MARTePv::GetAnyType() const {
    return pValue;
}

}
#endif /* SOURCE_COMPONENTS_INTERFACES_MARTEIOC_MARTEPV_H_ */

