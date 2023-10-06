#ifndef OPCUADATASOURCE_AUTHUTILS_H_
#define OPCUADATASOURCE_AUTHUTILS_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "StreamString.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Reads from a stream to parse a username and password. The expected
 * structure is:
 * 
 * AuthenticationKey = {
 *    Username = "user1"
 *    Password = "password1"
 * }
 * 
 * @param[in] stream A stream to read the StructuredDataI from
 * @param[out] username Return the parsed username
 * @param[out] password Return the parsed password
 * @return true when parsing is succesful
 */
bool ReadAuthenticationKey(StreamI &stream,
                           StreamString &username,
                           StreamString &password);

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* OPCUADATASOURCE_AUTHUTILS_H_ */

