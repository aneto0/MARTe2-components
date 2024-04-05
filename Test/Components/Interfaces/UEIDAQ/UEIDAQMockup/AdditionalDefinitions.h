#ifndef _ADDITIONAL_DEFINITIONS_H_
#define _ADDITIONAL_DEFINITIONS_H_

#define DQ_NOERROR             0       // no error encountered
#define DQ_SUCCESS             1       // success
#define DQ_WAIT_ENDED          2       // asynchronous event or VMap/DMap timed out
#define DQ_DATA_NOTREADY       3       // success but new data has not yet settled on the IOM side
#define DQ_DATA_NOTEXIST       4       // success but new data does not exist
#define DQ_DEV_STARTED         5       // DQERR_STARTED (0x80020000) /* Start trigger is received */
#define DQ_DEV_STOPPED         6       // DQERR_STOPPED (0x80040000) /* Stop trigger is received */

/*
These are error codes specific for host library
They shall be in [0xffffffff...0xffffff00] range
*/
#define DQ_NOERROR             0       // no error encountered
#define DQ_SUCCESS             1       // success
#define DQ_WAIT_ENDED          2       // asynchronous event or VMap/DMap timed out
#define DQ_DATA_NOTREADY       3       // success but new data has not yet settled on the IOM side
#define DQ_DATA_NOTEXIST       4       // success but new data does not exist
#define DQ_DEV_STARTED         5       // DQERR_STARTED (0x80020000) Start trigger is received 
#define DQ_DEV_STOPPED         6       // DQERR_STOPPED (0x80040000) Stop trigger is received 

#define DQ_ILLEGAL_ENTRY      (-1)     // illegal entry in parameters
#define DQ_ILLEGAL_HANDLE     (-2)     // illegal IOM handle (index)
#define DQ_SOCK_LIB_ERROR     (-3)     // socket error
#define DQ_TIMEOUT_ERROR      (-4)     // command returns upon timeout
#define DQ_SEND_ERROR         (-5)     // packet sending error
#define DQ_RECV_ERROR         (-6)     // packet receiving error
#define DQ_IOM_ERROR          (-7)     // IOM reports an unrecoverable error (DQERR_GENFAIL)
#define DQ_PKT_TOOLONG        (-8)     // too much data to fit a packet
#define DQ_ILLEGAL_PKTSIZE    (-9)     // packet size too small or too large
#define DQ_INIT_ERROR         (-10)    // IOM initialization error
#define DQ_BAD_PARAMETER      (-11)    // Invalid parameter passed
#define DQ_BAD_DEVN           (-12)    // incorrect DEVN, function does not support this layer type
#define DQ_NOT_IMPLEMENTED    (-13)    // call tech support - not implemented yet, firmware + lib version mismatch
#define DQ_NO_MEMORY          (-14)    // not enough memory
#define DQ_NOT_ENOUGH_ROOM    (-15)    // not enough room in the packet/structure
#define DQ_DEVICE_BUSY        (-16)    // somebody else uses this device
#define DQ_EVENT_ERROR        (-17)    // event handling error
#define DQ_BAD_CONFIG         (-18)    // bad configuration reported by DQCMD_RDSTS
#define DQ_DATA_ERROR         (-19)    // layer returned invalid data
#define DQ_DEVICE_NOTREADY    (-20)    // device is not ready
#define DQ_CALIBRATION_ERROR  (-21)    // error while performing calibration
#define DQ_WRONG_DMAP         (-22)    // requested and received dmapid's do not match
#define DQ_DATA_NOT_AVAILABLE (-23)    // requested data is not available
#define DQ_FIFO_OVERFLOW      (-24)    // device FIFO overflowed (DQERR_OVRFLW)
#define DQ_ILLEGAL_INDEX      (-25)    // illegal index supplied
#define DQ_DIO_LINE_NOT_EXIST (-26)    // the specified dio line doesn't exist
#define DQ_WRONG_PKT_COUNTER  (-27)    // incorrect packet counter was received
#define DQ_ASYNC_OUT_REREQST  (-28)    // last asynchronous output packet is re-requested
#define DQ_PROTOCOL_MISMATCH  (-29)    // protocol version mismatch (DAQLib / IOM CPU)
#define DQ_CMD_NOTALLOWED     (-30)    // command is not allowed either because the device is in op or shutdown mode
#define DQ_CMD_ACCESSDENIED   (-31)    // access denied due to invalid authentication or hardware prevention mechanism
#define DQ_DEVLOCKED          (-32)    // access denied due to IOM locked to another host
#define DQ_CRC_CHECK_FAILED   (-33)    // received/transmitted parameters failed CRC secure shell check
#define DQ_NOT_XMAP_PKT       (-34)    // the packet is not DMap or VMap

#define DQ_BAD_PARAMETER_0    (-200)    // first parameter on command line was invalid
#define DQ_BAD_PARAMETER_1    (-201)    // second parameter on command line was invalid
#define DQ_BAD_PARAMETER_2    (-202)    // third parameter on command line was invalid
#define DQ_BAD_PARAMETER_3    (-203)    // fourth parameter on command line was invalid
#define DQ_BAD_PARAMETER_4    (-204)    // fifth parameter on command line was invalid
#define DQ_BAD_PARAMETER_5    (-205)    // sixth parameter on command line was invalid
#define DQ_BAD_PARAMETER_6    (-206)    // seventh parameter on command line was invalid
#define DQ_BAD_PARAMETER_7    (-207)    // eighth parameter on command line was invalid
#define DQ_BAD_PARAMETER_8    (-208)    // ninth parameter on command line was invalid
#define DQ_BAD_PARAMETER_9    (-209)    // tenth parameter on command line was invalid
#define DQ_BAD_PARAMETER_10   (-210)    // parameter on command line was invalid
#define DQ_BAD_PARAMETER_11   (-211)    // parameter on command line was invalid
#define DQ_BAD_PARAMETER_12   (-212)    // parameter on command line was invalid
#define DQ_BAD_PARAMETER_13   (-213)    // parameter on command line was invalid
#define DQ_BAD_PARAMETER_14   (-214)    // parameter on command line was invalid
#define DQ_BAD_PARAMETER_15   (-215)    // parameter on command line was invalid
#define DQ_BAD_PARAMETER_16   (-216)    // parameter on command line was invalid
#define DQ_BAD_PARAMETER_17   (-217)    // parameter on command line was invalid
#define DQ_BAD_PARAMETER_18   (-218)    // parameter on command line was invalid
#define DQ_BAD_PARAMETER_19   (-219)    // parameter on command line was invalid
#define DQ_BAD_PARAMETER_20   (-220)    // parameter on command line was invalid
#define DQ_BAD_PARAMETER_21   (-221)    // parameter on command line was invalid
#define DQ_BAD_PARAMETER_22   (-222)    // parameter on command line was invalid
#define DQ_BAD_PARAMETER_23   (-223)    // parameter on command line was invalid
#define DQ_BAD_PARAMETER_24   (-224)    // parameter on command line was invalid
#define DQ_BAD_PARAMETER_25   (-225)    // parameter on command line was invalid
#define DQ_BAD_PARAMETER_26   (-226)    // parameter on command line was invalid
#define DQ_BAD_PARAMETER_27   (-227)    // parameter on command line was invalid
#define DQ_BAD_PARAMETER_28   (-228)    // parameter on command line was invalid
#define DQ_BAD_PARAMETER_29   (-229)    // parameter on command line was invalid
#define DQ_BAD_PARAMETER_30   (-230)    // parameter on command line was invalid
#define DQ_BAD_PARAMETER_31   (-231)    // parameter on command line was invalid
#define DQ_BAD_PARAMETER_32   (-232)    // parameter on command line was invalid
#define DQ_BAD_PARAMETER_33   (-233)    // parameter on command line was invalid
#define DQ_BAD_PARAMETER_34   (-234)    // parameter on command line was invalid
#define DQ_BAD_PARAMETER_35   (-235)    // parameter on command line was invalid
#define DQ_BAD_PARAMETER_36   (-236)    // parameter on command line was invalid
#define DQ_BAD_PARAMETER_37   (-237)    // parameter on command line was invalid
#define DQ_BAD_PARAMETER_38   (-238)    // parameter on command line was invalid
#define DQ_BAD_PARAMETER_39   (-239)    // parameter on command line was invalid

#define DQ_ERR_NEGATIVE_RETURN 0x80000000   // invert return code

#define DQ_CHK_ERROR_NONE           0   // no error
#define DQ_CHK_ERROR_IGNORE         1   // error can be ignored for now
#define DQ_CHK_ERROR_NONCRITICAL    2   // error non-critical, most likely in function parameters
#define DQ_CHK_ERROR_CRITICAL       3   // critical error, execution must stop
#define DQ_CHECK_ALIVE              1   // check whether cube is alive by sending heartbeat packet
#define DQ_ERROR_TIMEOUT_CTN        2   // maximum number of timeouts in a row

typedef struct {
    int max_payload_sz;     // maximum size of payload, packet will be fragmented if > GetIOM(handle)->MaxDqPayload
    int mtu;                // maximum transfer unit, packets will be fragmented if exceeded
    double refreshRate;     // desired refresh rate, used as a guideline to program layer parameters
} DQ_RTMAP_PARAM, *pDQ_RTMAP_PARAM;


#define DQ_DIO404_REF_3_3V  3
#define DQ_DIO404_REF_5V    5
#define DQ_DIO404_REF_12V   12
#define DQ_DIO404_REF_24V   24
#define DQ_DIO404_REF_36V   36
#endif