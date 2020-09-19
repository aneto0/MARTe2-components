/**
 * @file MDSObjLoader.cpp
 * @brief Source file for class MDSObjLoader
 * @date 29/05/2019
 * @author Galperti Cristian
 *
 * @copyright Copyright 2019 EPFL SPC Lausanne
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
 * the class MDSObjLoader (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <stdio.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "MDSObjLoader.h"
#include <mdsobjects.h>

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {


MDSObjLoader::MDSObjLoader() :
        ParObjLoader() {
    shot=0;
    //printf("MDSObjLoader constructor called\n");
}

MDSObjLoader::~MDSObjLoader() {
}

bool MDSObjLoader::Initialise(StructuredDataI &data) {
    MDSplus::Tree *currenttree;
    MDSplus::Connection *currentconnection;
    MDSplus::Data *nodeData;

    StreamString basename = this->GetName();
    bool ret = ParObjLoader::Initialise(data);



    //printf("MDSObjLoader Initialise called, ReferenceContainer size: %d\n", Size());

    if(ret)
    {
        ret=data.Read("Shot", shot);
        if(!ret)
        {
            REPORT_ERROR(ErrorManagement::ParametersError, "Shot not set");
        }
    }

    if(ret){
         
         // Loop over all the children of this node (which are supposed to be MDSObjConnection objects)
         for(uint32 i=0; i<Size(); i++){
            
            // Get a reference to the i-th child
            ReferenceT<MDSObjConnection> ref = Get(i);
            
            if (!ref.IsValid()) {
                
                // MDSObjLoader populates itself with AnyObjects, so references to AnyObject are ignored (no warning issued).
                ReferenceT<AnyObject> objRef = Get(i);
                if (!objRef.IsValid()) {
                    REPORT_ERROR(ErrorManagement::InitialisationError, "Node %u is of incompatible class. Invalid reference.", i);
                }
            }
            
            if (ref.IsValid()) {
            
                // If the connection has a local shot defined, override the one defined in the MDSObjLoader main object
                int32 shottoopen;
                if(ref->hasLocalShot()) shottoopen = ref->getLocalShot(); else shottoopen = shot;

                //printf("Here we should connect to MDS %s::%s, shot %d\n", ref->getServer().Buffer(), ref->getTree().Buffer(),shot);
                REPORT_ERROR(ErrorManagement::Information, "%s connecting to server: %s, tree: %s, shot: %d", this->GetName(), ref->getServer().Buffer(), ref->getTree().Buffer(), shottoopen);

                try{
                    //mdsi.OpenTree(ref->getTree().Buffer(), shottoopen);

                    // Distributed client implementation, not suitable for us since
                    // we have remote TDI and Signals evaluation even in the parameters
                    //currenttree = new MDSplus::Tree(ref->getTree().Buffer(), shottoopen, "NORMAL");
                    currenttree = NULL;

                    // This client implementation, this is the corret way to replicate mdsvalue matlab command
                    currentconnection = new MDSplus::Connection((char *)(ref->getServer().Buffer()));



                    currentconnection->openTree((char *)(ref->getTree().Buffer()), (int)shottoopen);


                    //currentconnection = NULL;
                    /*
                    Data *args[] = new Data *[2];
                    args[0] = new Int32(2);
                    args[1] = new Int32(10);
                    Data *result = conn->get("(MY_NODE:DATA * $1)*$2", args, 2);
    */

                }
                catch(MDSplus::MdsException ex)
                {
                    REPORT_ERROR(ErrorManagement::InitialisationError, "Error opening the tree: %s\n",ex.what());
                    return false;
                }

                StreamString currentshotrequest;

                if(shottoopen==0)
                {
                    try {
                        currentshotrequest = "current_shot('";
                        currentshotrequest += ref->getTree();
                        currentshotrequest += "')";
                        nodeData = currentconnection->get(currentshotrequest.Buffer());

                        int32 currentshotno = nodeData->getInt();
                        REPORT_ERROR(ErrorManagement::Information, "%s, current shot is: %d", this->GetName(), currentshotno);
                    }
                    catch(MDSplus::MdsException ex)
                    {
                        REPORT_ERROR(ErrorManagement::Warning, "Error calling %s, no current shot info available\n",currentshotrequest.Buffer(),ex.what());
                    }
                }
                
                    
                    // ... loop over all children of the connection, that are supposed to be MDSParameters
                    for(uint32 j=0; j<ref->Size(); j++){
                        ReferenceT<MDSParameter> refPar=ref->Get(j);
                        if (refPar.IsValid()) {
                            
                            // Actualisation (data from MDSplus are copied into this MDSParameter)
                            ret = refPar->Actualize(privatecdb, currentconnection, currenttree, basename);
                            
                            // Also insert an AnyObject copy of this parameter at the root level of this ReferenceContainer
                            ReferenceT<AnyObject> paramObject("AnyObject", GlobalObjectsDatabase::Instance()->GetStandardHeap());
                            if (ret && paramObject.IsValid() && refPar->IsStaticDeclared()) {
                                ret = paramObject->Serialise(*(refPar.operator ->())); // required since we can't do Serialise(*refPar), * is not overloaded for ReferenceT
                                if (ret) {
                                    paramObject->SetName(refPar->GetName());
                                    Insert(paramObject);
                                }
                            }
                        
                            if(!ret) goto actualizeerrorexit;
                            
                        }
                    }
                    
                //printf("Here we should disconnect.\n");
                //TODO: ask how to force a disconnect
                
                REPORT_ERROR(ErrorManagement::Information, "%s disconnecting", this->GetName());
             }

             
             //delete currenttree;

         }
    }
    
    actualizeerrorexit:
    
    return ret;
}

CLASS_REGISTER(MDSObjLoader, "1.0")

}
