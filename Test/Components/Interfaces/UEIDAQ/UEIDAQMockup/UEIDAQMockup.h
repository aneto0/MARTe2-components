#include "AdditionalDefinitions.h"
#ifndef _UEIDAQMOCKUP_H_
#define _UEIDAQMOCKUP_H_

namespace MARTe{
//Error codes to specify error responses from the PowerDNA API Mockup;
typedef enum{
    NO_ERROR,                  
    DqInitDAQLib_ERROR,        
    DqAdv217SetCfgLayer_ERROR, 
    DqAdv217SetFIR_ERROR,      
    DqAdv217SetFIR_InvalidTaps,
    DqAdv217SetFIR_WrongBank1, 
    DqCloseIOM_Error,          
    DqOpenIOM_Error,
    DqOpenIOM_ZeroHandle,
    DqGetDevnBySlot_ERROR,
    DqRtDmapStop_ERROR,
    DqRtDmapInit_ERROR,
    DqRtDmapAddChannel_INPUT_ERROR,
    DqRtDmapAddChannel_OUTPUT_ERROR,
    DqRtDmapAddChannel_TIMESTAMP_ERROR,
    DqRtDmapAddChannel_ERROR,
    DqRtDmapStart_ERROR,
    DqRtDmapRefresh_ERROR,
    DqRtDmapGetInputMap_ERROR,
    DqRtDmapGetInputMap_NULL,
    DqRtVmapStop_ERROR,
    DqRtVmapStart_ERROR,
    DqRtVmapRqInputDataSz_ERROR,
    DqRtVmapInitEx_ERROR,
    DqRtVmapAddChannel_ERROR,
    DqRtVmapSetChannelList_ERROR,
    DqRtVmapSetScanRate_ERROR
}UEIDAQErrors;


typedef struct{
    uint16 model;
    uint32 devn;
    uint32 serial;
    uint32 address;
    uint8 present;
}IOMDevice;

class IOM {
    public:
        IOM() {
            //initialise the are of memory exchange, used for data acquisition on PowerDNA side
            MemoryExchangeArea = new uint8[200];
            resetIOM();
        }
        ~IOM() {
            delete[] MemoryExchangeArea;
        }
        
        UEIDAQErrors GetErrorCode(){
            return ErrorCode;
        }
        void SetErrorCode(UEIDAQErrors newErrorCode){
            ErrorCode = newErrorCode;
        }
        uint8* MemoryExchangeArea;
        bool SetDevice(uint32 slot, uint32 devn, uint32 serial, uint32 address, uint16 model){
            bool ok = (slot < 12);
            if (ok){
                devices[slot].devn = devn;
                devices[slot].model = model;
                devices[slot].serial = serial;
                devices[slot].address = address;
                devices[slot].present = 1;
            }  
            return ok;
        }
        bool GetDevice(uint32 slot, uint32* devn, uint32* serial, uint32* address, uint16* model){
            bool ok = (slot < 12);
            if (ok){
                *devn = devices[slot].devn;
                *model = devices[slot].model;
                *serial = devices[slot].serial;
                *address = devices[slot].address;
                ok &= (devices[slot].present == 1);
            }
            return ok;
        }
        void resetIOM(){
            ErrorCode = NO_ERROR;
            for (uint32 i = 0; i < 12; i++){
                devices[i].devn = 0;
                devices[i].model = 0;
                devices[i].serial = 0;
                devices[i].address = 0;
                devices[i].present = 0;
            }
            //ToRemove
            devices[0].devn = 0;
            devices[0].model = 0x217;
            devices[0].present = 1;
            devices[1].devn = 1;
            devices[1].model = 0x217;
            devices[1].present = 1;
        }
    private:
        UEIDAQErrors ErrorCode;
        IOMDevice devices[12];
};

class UEIDAQMockupManager : public GlobalObjectI  {
    public:
        static UEIDAQMockupManager* instance;
        IOM* IOMObject;
        //This error code is used to force errors externally onto the PowerDNA API function calls

        void SetErrorCode(UEIDAQErrors newErrorCode){
            ErrorCode = newErrorCode;
        }
        UEIDAQErrors GetErrorCode(){
            return ErrorCode;
        }
        bool GetIOM(int32 DAQ_handle, IOM** thisIOM){
            if (DAQ_handle == defaultHandle){
                *thisIOM = IOMObject;
                return true;
            }else{
                return false;
            }
        }
        static UEIDAQMockupManager* getInstance(){
            //Static declaration, allocates memory for the Singleton reference
            static UEIDAQMockupManager* instance = NULL;
            if (instance == NULL){
                instance = new UEIDAQMockupManager();
                GlobalObjectsDatabase::Instance()->Add(instance, NUMBER_OF_GLOBAL_OBJECTS - 3u);
            }
            return instance;
        }
        void resetManager(){
            ErrorCode = NO_ERROR;
            defaultHandle = 1;
            IOMObject->resetIOM();
        }
        int32 defaultHandle;
        
        const char8 * const GetClassName() const {
            return "UEIDAQMockupManager";
        }  

    private:
        static void *operator new(const osulong size) throw (){
            return GlobalObjectI::operator new(size);
        }

        static void operator delete(void * const p){
            return GlobalObjectI::operator delete(p);
        }

        UEIDAQErrors ErrorCode;
        UEIDAQMockupManager() {
            IOMObject = new IOM();
            resetManager();
        }
        UEIDAQMockupManager(const UEIDAQMockupManager& obj);      
};
}

//Proxy functions for PowerDNA API Mockup
inline int32 DqInitDAQLib(){
    MARTe::UEIDAQMockupManager* manager = MARTe::UEIDAQMockupManager::getInstance();
    if (manager->GetErrorCode() == MARTe::DqInitDAQLib_ERROR){
        return (-1); //Generalc error
    }else{
        return DQ_SUCCESS;
    }
}

inline int32 DqCloseIOM(int32 DAQ_handle){
    MARTe::UEIDAQMockupManager* manager = MARTe::UEIDAQMockupManager::getInstance();
    if (manager->GetErrorCode() == MARTe::DqCloseIOM_Error){
        return (-1); //Generalc error
    }else{
        return DQ_SUCCESS;
    }
}

inline void DqCleanUpDAQLib(){}

inline int32 DqOpenIOM(char *IP, uint16 UDP_Port, uint32 mTimeOut, int *handle, pDQRDCFG *pDqCfg){
    MARTe::UEIDAQMockupManager* manager = MARTe::UEIDAQMockupManager::getInstance();
    *handle = manager->defaultHandle;
    if (manager->GetErrorCode() == MARTe::DqOpenIOM_Error){
        return (-1); //General error
    }else if (manager->GetErrorCode() == MARTe::DqOpenIOM_ZeroHandle){
        *handle = 0;
        return DQ_SUCCESS;        
    }
    return DQ_SUCCESS;
}

inline int32 DqGetDevnBySlot(int Iom, uint32 Slot, uint32* devn, uint32* serial, uint32* address, uint16* model){
    MARTe::UEIDAQMockupManager* manager = MARTe::UEIDAQMockupManager::getInstance();  
    MARTe::IOM* myIOM;
    if (manager->GetIOM(Iom, &myIOM)){
        if (manager->GetErrorCode() == MARTe::DqGetDevnBySlot_ERROR){
            return DQ_ILLEGAL_HANDLE;
        }else{
            if (myIOM->GetDevice(Slot, devn, serial, address, model)){
                return DQ_SUCCESS;
            }else{
                return DQ_BAD_PARAMETER;
            }
        }
    }
    return DQ_ILLEGAL_HANDLE;
}

inline int32 DqRtDmapStop(int32 DAQ_handle, int32 mapid){
    MARTe::UEIDAQMockupManager* manager = MARTe::UEIDAQMockupManager::getInstance();
    if (manager->GetErrorCode() == MARTe::DqRtDmapStop_ERROR){
        return (-1); //General error
    }else{
        return DQ_SUCCESS;        
    }
}

inline int32 DqRtDmapClose(int32 DAQ_handle, int32 mapid){
    return DQ_SUCCESS;
}

inline int32 DqRtDmapInit(int handle, int* dmapid, double refreshRate){
    MARTe::UEIDAQMockupManager* manager = MARTe::UEIDAQMockupManager::getInstance();
    if (manager->GetErrorCode() == MARTe::DqRtDmapInit_ERROR){
        return (-1); //General error
    }else{
        return DQ_SUCCESS;        
    }
}

inline int32 DqRtDmapAddChannel(int handle, int dmapid, int dev, int subsystem, uint32* cl, int clSize){
    MARTe::UEIDAQMockupManager* manager = MARTe::UEIDAQMockupManager::getInstance();
    if (manager->GetErrorCode() == MARTe::DqRtDmapAddChannel_INPUT_ERROR 
        && subsystem == DQ_SS0IN
        && *cl != DQ_LNCL_TIMESTAMP){
        return (-1); //General error
    }else if (manager->GetErrorCode() == MARTe::DqRtDmapAddChannel_OUTPUT_ERROR
              && subsystem == DQ_SS0OUT){
        return (-1); //General error
    }else if (manager->GetErrorCode() == MARTe::DqRtDmapAddChannel_TIMESTAMP_ERROR && *cl == DQ_LNCL_TIMESTAMP){
        return (-1); //General error
    }else if (manager->GetErrorCode() == MARTe::DqRtDmapAddChannel_ERROR){
        return (-1); //General error
    }else{
        return DQ_SUCCESS;        
    }
}

inline int32 DqRtDmapStart(int handle, int dmapid){
    MARTe::UEIDAQMockupManager* manager = MARTe::UEIDAQMockupManager::getInstance();
    if (manager->GetErrorCode() == MARTe::DqRtDmapStart_ERROR){
        return (-1); //General error
    }else{
        return DQ_SUCCESS;
    }
}

inline int32 DqRtDmapRefresh(int handle, int dmapid){
    MARTe::UEIDAQMockupManager* manager = MARTe::UEIDAQMockupManager::getInstance();
    if (manager->GetErrorCode() == MARTe::DqRtDmapRefresh_ERROR){
        return (-1); //General error
    }else{
        return DQ_SUCCESS;
    }
    return DQ_SUCCESS;
}

inline int32 DqRtDmapGetInputMap(int handle, int dmapid, int dev, unsigned char** mappedData){
    MARTe::UEIDAQMockupManager* manager = MARTe::UEIDAQMockupManager::getInstance();
    MARTe::IOM* myIOM;
    manager->GetIOM(handle, &myIOM);
    if (manager->GetErrorCode() == MARTe::DqRtDmapGetInputMap_ERROR){
        return (-1);
    }else if (manager->GetErrorCode() == MARTe::DqRtDmapGetInputMap_NULL){
        return DQ_SUCCESS;
    }else{
        *mappedData = reinterpret_cast<unsigned char*>(myIOM->MemoryExchangeArea);
        return DQ_SUCCESS;
    }
    return DQ_SUCCESS;
}

inline int32 DqRtVmapStop(int handle, int vmapid){
    MARTe::UEIDAQMockupManager* manager = MARTe::UEIDAQMockupManager::getInstance();
    if (manager->GetErrorCode() == MARTe::DqRtVmapStop_ERROR){
        return (-1); //General error
    }else{
        return DQ_SUCCESS;
    }
    return DQ_SUCCESS;
}

inline int32 DqRtVmapClose(int handle, int vmapid){
    return DQ_SUCCESS;
}

inline int32 DqRtVmapAddChannel(int handle, int vmapid, int dev, int subsystem, int* cl, int* flags, int clSize){
    MARTe::UEIDAQMockupManager* manager = MARTe::UEIDAQMockupManager::getInstance();
    if (manager->GetErrorCode() == MARTe::DqRtVmapAddChannel_ERROR){
        return (-1); //General error
    }else{
        return DQ_SUCCESS;
    }
    return DQ_SUCCESS;
}

inline int32 DqRtVmapInitEx(int handle, int* dmapid, pDQ_RTMAP_PARAM param){
    MARTe::UEIDAQMockupManager* manager = MARTe::UEIDAQMockupManager::getInstance();
    if (manager->GetErrorCode() == MARTe::DqRtVmapInitEx_ERROR){
        return (-1); //General error
    }else{
        return DQ_SUCCESS;
    }
    return DQ_SUCCESS;
}

inline int32 DqRtVmapSetChannelList(int hd, int vmapid, int dev, int subsystem, int* cl, int clSize){
    MARTe::UEIDAQMockupManager* manager = MARTe::UEIDAQMockupManager::getInstance();
    if (manager->GetErrorCode() == MARTe::DqRtVmapSetChannelList_ERROR){
        return (-1); //General error
    }else{
        return DQ_SUCCESS;
    }
    return DQ_SUCCESS;
}

inline int32 DqRtVmapSetScanRate(int handle, int vmapid, int dev, int subsystem, float scan_rate){
    MARTe::UEIDAQMockupManager* manager = MARTe::UEIDAQMockupManager::getInstance();
    if (manager->GetErrorCode() == MARTe::DqRtVmapSetScanRate_ERROR){
        return (-1); //General error
    }else{
        return DQ_SUCCESS;
    }
    return DQ_SUCCESS;
}

inline int32 DqRtVmapRefreshExt(int handle, int vmapid, int flags, uint16* dqCounter, uint16* dqTStamp){
    return DQ_SUCCESS;
}

inline int32 DqRtVmapRefresh(int handle, int vmapid, int flags){
    return DQ_SUCCESS;
}

inline int32 DqRtVmapStart(int handle, int dmapid){
    MARTe::UEIDAQMockupManager* manager = MARTe::UEIDAQMockupManager::getInstance();
    if (manager->GetErrorCode() == MARTe::DqRtVmapStart_ERROR){
        return (-1); //General error
    }else{
        return DQ_SUCCESS;
    }
    return DQ_SUCCESS;
}

inline int32 DqRtVmapGetInputData(int handle, int vmapid, int trl_index, uint32 max_size, int* data_size, int* avl_size, uint8* data){
    return DQ_SUCCESS;
}

inline int32 DqRtVmapRqInputDataSz(int handle, int vmapid, int trl_index, uint32 rq_size, int* act_size, uint8 **indataptr){
    MARTe::UEIDAQMockupManager* manager = MARTe::UEIDAQMockupManager::getInstance();
    *act_size = 0;
    if (manager->GetErrorCode() == MARTe::DqRtVmapRqInputDataSz_ERROR){
        return (-1); //General error
    }else{
        return DQ_SUCCESS;
    }
    return DQ_SUCCESS;
}

//AI217 specific functions
inline int32 DqAdv217SetCfgLayer(int hd, int devn, int channels, int function, int data){
    MARTe::UEIDAQMockupManager* manager = MARTe::UEIDAQMockupManager::getInstance();
    MARTe::IOM* myIOM;
    if (manager->GetIOM(hd, &myIOM)){
        if (myIOM->GetErrorCode() == MARTe::DqAdv217SetCfgLayer_ERROR){
            return (-1); //Generalc error
        }else{
            return DQ_SUCCESS;
        }
    }else{
        return (-1);
    }
}

inline int32 DqAdv217SetFIR(int hd, int devn, int bank, int action, int decrat, int tapsize, int* filter_total, double* data){
    MARTe::UEIDAQMockupManager* manager = MARTe::UEIDAQMockupManager::getInstance();
    MARTe::IOM* myIOM;
    if (manager->GetIOM(hd, &myIOM)){
        if (myIOM->GetErrorCode() == MARTe::DqAdv217SetFIR_ERROR){
            return (-1); //Generalc error
        }else if (myIOM->GetErrorCode() == MARTe::DqAdv217SetFIR_WrongBank1 && bank == 1){
            return (-1); //Generalc error
        }else{
            if (filter_total != NULL){
                if (myIOM->GetErrorCode() == MARTe::DqAdv217SetFIR_InvalidTaps){
                    *filter_total = 34500;
                }else{
                    *filter_total = 8388608;
                }
            }
            return DQ_SUCCESS;
        }
    }else{
        return (-1);
    }
}
#endif