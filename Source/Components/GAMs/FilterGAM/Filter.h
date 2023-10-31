/**
 * @file Filter.h
 * @brief Header file for class Filter
 * @date 30/10/2023
 * @author Llorenc Capella
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

 * @details This header file contains the declaration of the class Filter
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef FILTERGAM_FILTER_H_
#define FILTERGAM_FILTER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

namespace MARTe {
class Filter {
public:
    virtual ~Filter();
    virtual bool Initialise(void *numIn,
                            uint32 nOfNumCoeffIn,
                            void *denIn,
                            uint32 nOfDenCoeffIn) = 0;
    virtual void Process(void *input,
                         void *output,
                         uint32 nOfElements) = 0;
    virtual bool Reset() = 0;
    virtual uint32 GetNumberOfNumCoeff() = 0;
    virtual uint32 GetNumberOfDenCoeff() = 0;
    virtual bool GetNumCoeff(void *coeff) = 0;
    virtual bool GetDenCoeff(void *coeff) = 0;
    virtual bool CheckNormalisation() = 0;
private:

};

}
#endif /* SOURCE_COMPONENTS_GAMS_FILTERGAM_FILTER_H_ */
