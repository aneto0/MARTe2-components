/**
 * @file EventConditionTrigger.cpp
 * @brief Source file for class EventConditionTrigger
 * @date 06/07/2018
 * @author Giuseppe Ferrò
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
 * the class EventConditionTrigger (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "EventConditionTrigger.h"
#include "EventSem.h"
#include "QueuedReplyMessageCatcherFilter.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

EventConditionTrigger::EventConditionField::EventConditionField() {
    signalMetadata = NULL_PTR(SignalMetadata *);

}

/*lint -e{1551} no exception will be called in destructor.*/
EventConditionTrigger::EventConditionField::~EventConditionField() {

    if (!at.IsVoid()) {
        uint8 *ptr = reinterpret_cast<uint8*>(at.GetDataPointer());
        delete ptr;
    }
    signalMetadata = NULL_PTR(SignalMetadata *);

}

EventConditionTrigger::EventConditionTrigger() :
        ReferenceContainer(),
        EmbeddedServiceMethodBinderI(),
        MessageI(),
        executor(*this) {
    eventConditions = NULL_PTR(EventConditionField *);
    numberOfConditions = 0u;
    replied = 0u;
    if (!eventSem.Create()) {
        REPORT_ERROR(ErrorManagement::FatalError, "Could not create EventSem.");
    }
    if (!eventSem.Reset()) {
        REPORT_ERROR(ErrorManagement::FatalError, "Could not reset EventSem.");
    }
}

/*lint -e{1551} no exception will be called in destructor.*/
EventConditionTrigger::~EventConditionTrigger() {

    if (eventConditions != NULL_PTR(EventConditionField *)) {
        delete[] eventConditions;
    }
}

bool EventConditionTrigger::Initialise(StructuredDataI &data) {
    bool ret = ReferenceContainer::Initialise(data);
    if (ret) {
        ret = data.MoveRelative("EventTrigger");
        if (ret) {
            numberOfConditions = data.GetNumberOfChildren();
            ret = data.MoveToAncestor(1u);
        }
        else {
            REPORT_ERROR(ErrorManagement::ParametersError, "Please define the block EventTrigger containing the trigger conditions");
        }
        if (ret) {
            ret = data.Copy(signalsDatabase);
        }
    }
    if (ret) {
        uint32 cpuMaskIn;
        if (!data.Read("CPUMask", cpuMaskIn)) {
            cpuMaskIn = 0xFFu;
            REPORT_ERROR(ErrorManagement::Warning, "CPUMask not specified using: %d", cpuMaskIn);
        }
        cpuMask = cpuMaskIn;
    }
    if (ret) {
        uint32 n;
        for (n = 0u; (n < Size()) && (ret); n++) {
            ReferenceT < Message > msg = Get(n);
            ret = msg.IsValid();
            if (!ret) {
                REPORT_ERROR(ErrorManagement::ParametersError, "Only Messages are allowed inside the container");
            }
        }
    }
    if (ret) {
        executor.SetCPUMask(cpuMask);
    }
    return ret;
}

bool EventConditionTrigger::SetMetadataConfig(const SignalMetadata * const metadataConf,
                                            const uint32 numberOfFields) {
    /*lint -e{613} NULL pointer checked.*/
    eventConditions = new EventConditionField[numberOfConditions];

    bool ret = (eventConditions != NULL);
    if (ret) {
        ret = signalsDatabase.MoveRelative("EventTrigger");

        for (uint32 i = 0u; (i < numberOfConditions) && (ret); i++) {

            StreamString fieldName = signalsDatabase.GetChildName(i);
            bool found = false;
            for (uint32 j = 0u; (j < numberOfFields) && (!found); j++) {
                if (metadataConf[j].name == fieldName) {
                    /*lint -e{613} NULL pointer checked.*/
                    eventConditions[i].signalMetadata = &metadataConf[j];
                    found = true;
                }
            }
            /*lint -e{429} the pointer will be freed by the EventConditionField destructor.*/
            if (found) {
                /*lint -e{613} NULL pointer checked.*/
                uint32 sizeMem = static_cast<uint32>(((eventConditions[i].signalMetadata)->type).numberOfBits) / 8u;
                uint8 *mem = new uint8[sizeMem];
                /*lint -e{613} NULL pointer checked.*/
                eventConditions[i].at = AnyType((eventConditions[i].signalMetadata)->type, 0u, mem);
                /*lint -e{613} NULL pointer checked.*/
                ret = signalsDatabase.Read(fieldName.Buffer(), eventConditions[i].at);
                if (!ret) {
                    REPORT_ERROR(ErrorManagement::FatalError, "Failed Read of %s: probably type mismatch", fieldName.Buffer());
                }
            }
            else {
                ret = false;
                REPORT_ERROR(ErrorManagement::FatalError, "The field %s does not match any field in the accelerator", fieldName.Buffer());
            }
        }
        if (ret) {
            ret = signalsDatabase.MoveToAncestor(1u);
            if (ret) {
                ErrorManagement::ErrorType err = executor.Start();
                ret = err.ErrorsCleared();
            }
        }
    }
    return ret;
}

bool EventConditionTrigger::Check(const uint8 * const memoryArea,
                                  const SignalMetadata * const metadataIn) {
    bool ret = (metadataIn != NULL);
    if (ret) {
        ret = (metadataIn->isCommand);
        if (!ret) {
            StreamString name = metadataIn->name;
            REPORT_ERROR(ErrorManagement::FatalError, "The field %s is not a command and cannot trigger an event", name.Buffer());
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "Signal metadata given as input is null!");
    }
    bool trigger = false;
    if (ret) {

        for (uint32 i = 0u; (i < numberOfConditions) && (!trigger); i++) {
            /*lint -e{613} NULL pointer checked.*/
            trigger = (metadataIn == eventConditions[i].signalMetadata);
        }
        for (uint32 i = 0u; (i < numberOfConditions) && (trigger); i++) {
            /*lint -e{613} NULL pointer checked.*/
            uint32 byteSize = static_cast<uint32>(eventConditions[i].signalMetadata->type.numberOfBits) / 8u;
            /*lint -e{613} NULL pointer checked.*/
            trigger = (MemoryOperationsHelper::Compare(&memoryArea[eventConditions[i].signalMetadata->offset], eventConditions[i].at.GetDataPointer(), byteSize)
                    == 0);
        }

        if (trigger) {
            uint32 numberOfChildren = Size();
            for (uint32 i = 0u; i < numberOfChildren; i++) {
                ReferenceT < Message > message = Get(i);
                if (message.IsValid()) {
                    if (static_cast<bool>(spinLock.FastLock())) {
                        REPORT_ERROR(ErrorManagement::Information, "Inserted message");
                        if (!messageQueue.Insert(message)) {
                            REPORT_ERROR(ErrorManagement::Warning, "Failed Insert of the message in the queue");
                        }
                        spinLock.FastUnLock();
                    }
                }
            }
            (void) eventSem.Post();
        }
    }

    return trigger;

}

ErrorManagement::ErrorType EventConditionTrigger::Execute(ExecutionInfo & info) {

    ErrorManagement::ErrorType err = ErrorManagement::NoError;

    if (info.GetStage() == ExecutionInfo::StartupStage) {

    }
    else if (info.GetStage() != ExecutionInfo::BadTerminationStage) {
        //pull from the queue
        uint32 nMessages = 0u;
        if (static_cast<bool>(spinLock.FastLock())) {
            nMessages = messageQueue.Size();
            spinLock.FastUnLock();
        }

        if (nMessages > 0u) {

            //Semaphore to wait for replies from events which require a reply
            EventSem waitSem;
            bool ok = waitSem.Create();
            if (ok) {
                ok = waitSem.Reset();
            }

            //Prepare to wait for eventual replies
            ReferenceContainer eventReplyContainer;

            //Only accept indirect replies
            for (uint32 i = 0u; i < nMessages; i++) {
                ReferenceT < Message > eventMsg;
                if (static_cast<bool>(spinLock.FastLock())) {
                    eventMsg = messageQueue.Get(i);
                    spinLock.FastUnLock();
                }
                if (eventMsg.IsValid()) {
                    if (eventMsg->ExpectsIndirectReply()) {
                        err = !eventReplyContainer.Insert(eventMsg);
                        if(err.ErrorsCleared()) {
                            REPORT_ERROR(ErrorManagement::Warning, "Error after inserting message");
                        }
                        //REPORT_ERROR(ErrorManagement::Information, "Message %d set as indirect reply", i);
                    }
                }
            }

            if (ok) {
                ok = err.ErrorsCleared();
            }
            ReferenceT < QueuedReplyMessageCatcherFilter > filter;
            //Prepare the filter which will wait for all the replies
            if ((eventReplyContainer.Size() > 0u) && (ok)) {
                filter = ReferenceT < QueuedReplyMessageCatcherFilter > (new (NULL) QueuedReplyMessageCatcherFilter());
                filter->SetMessagesToCatch(eventReplyContainer);
                filter->SetEventSemaphore(waitSem);
                err = MessageI::InstallMessageFilter(filter, 0);
                if(err.ErrorsCleared()) {
                    REPORT_ERROR(ErrorManagement::Warning, "Error after installing message filter");
                }
                //REPORT_ERROR(ErrorManagement::Information, "Filter installed");
            }

            ok = err.ErrorsCleared();
            /*lint -e{850} the loop variable i is not modified within the loop*/
            for (uint32 i = 0u; (i < nMessages) && (ok); i++) {
                ReferenceT < Message > eventMsg;
                if (static_cast<bool>(spinLock.FastLock())) {
                    eventMsg = messageQueue.Get(0u);
                    ok = messageQueue.Delete(eventMsg);
                    spinLock.FastUnLock();
                }
                if (ok) {
                    REPORT_ERROR(ErrorManagement::Information, "Message %s [%d] extracted", eventMsg->GetName(), i);

                    if (eventMsg.IsValid()) {
                        eventMsg->SetAsReply(false);
                        err = MessageI::SendMessage(eventMsg, this);
                        if(!err.ErrorsCleared()) {
                            REPORT_ERROR(ErrorManagement::Warning, "Error after sending message");
                        }
                        if (!eventMsg->ExpectsIndirectReply()) {
                            if (static_cast<bool>(spinLock.FastLock())) {
                                replied++;
                                spinLock.FastUnLock();
                            }
                        }
                        REPORT_ERROR(ErrorManagement::Information, "Send message");
                    }
                    ok = err.ErrorsCleared();
                    if (!ok) {
                        REPORT_ERROR(ErrorManagement::FatalError, "Could not send message");
                    }
                }
            }
            //Wait for all the replies to arrive...
            if (ok) {
                if (eventReplyContainer.Size() > 0u) {
                    REPORT_ERROR(ErrorManagement::Information, "Wait reply");
                    err = waitSem.Wait();

                    if(!err.ErrorsCleared()) {
                        REPORT_ERROR(ErrorManagement::Warning, "Error after waiting semaphore");
                    }
                    if (static_cast<bool>(err)) {
                        if (static_cast<bool>(spinLock.FastLock())) {
                            replied += eventReplyContainer.Size();
                            spinLock.FastUnLock();
                        }
                        err = MessageI::RemoveMessageFilter(filter);
                        if(!err.ErrorsCleared()) {
                            REPORT_ERROR(ErrorManagement::Warning, "Error after removing message filter");
                        }
                    }
                }
            }
        }
        else {
            if (static_cast<bool>(spinLock.FastLock())) {
                if (messageQueue.Size() == 0u) {
                    err = !eventSem.Reset();
                }
                spinLock.FastUnLock();
            }
            if (err.ErrorsCleared()) {
                err = eventSem.Wait(500u);
                
                if(!err.ErrorsCleared()) {
                    err.timeout = false;
                }
            }
        }

    }
    else {
        //TODO: It was empty here
        REPORT_ERROR(ErrorManagement::Warning, "Thread in BadTerminationStage");
    }
    
    return err;
}

uint32 EventConditionTrigger::Replied(const SignalMetadata * const metadataIn, const uint32 maxReplies) {
    uint32 retVal = 0u;
    bool ret = (metadataIn != NULL);
    if (ret) {
        ret = (metadataIn->isCommand);
        if (!ret) {
            StreamString name = metadataIn->name;
            REPORT_ERROR(ErrorManagement::FatalError, "The field %s is not a command and cannot trigger an event", name.Buffer());
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "Input signal metadata is null!");
    }
    bool trigger = false;
    if (ret) {
        for (uint32 i = 0u; (i < numberOfConditions) && (!trigger); i++) {
            /*lint -e{613} NULL pointer checked.*/
            trigger = (metadataIn == eventConditions[i].signalMetadata);
        }

        if (trigger) {
            uint32 remains = 0u;
            if (static_cast<bool>(spinLock.FastLock())) {
                retVal = replied;
                if(retVal>maxReplies){
                    remains=(retVal-maxReplies);
                    retVal = maxReplies;
                }
                spinLock.FastUnLock();
            }
            if (retVal > 0u) {
                if (static_cast<bool>(spinLock.FastLock())) {
                    replied = remains;
                    spinLock.FastUnLock();
                }
            }
        }
    }
    return retVal;
}

void EventConditionTrigger::Purge(ReferenceContainer &purgeList) {
    (void) eventSem.Post();
    if (!executor.Stop()) {
        if (!executor.Stop()) {
            REPORT_ERROR(ErrorManagement::FatalError, "Could not stop SingleThreadService.");
        }
    }
    if (!eventSem.Close()) {
        REPORT_ERROR(ErrorManagement::FatalError, "Could not close the EventSem.");
    }

    messageQueue.Purge();
    ReferenceContainer::Purge(purgeList);

}

const ProcessorType& EventConditionTrigger::GetCPUMask() const {
    return cpuMask;
}

CLASS_REGISTER(EventConditionTrigger, "1.0")

}
