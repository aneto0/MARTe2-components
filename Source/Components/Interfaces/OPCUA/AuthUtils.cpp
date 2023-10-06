
/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "AdvancedErrorManagement.h"
#include "AuthUtils.h"
#include "ConfigurationDatabase.h"
#include "StandardParser.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

bool ReadAuthenticationKey(StreamI &stream,
                           StreamString &username,
                           StreamString &password) {
    bool ok = true;
    ConfigurationDatabase userPasswordDatabase;
    if (ok) {
        StandardParser parser(stream, userPasswordDatabase);
        ok = parser.Parse();
        if (!ok) {
            REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "Failed to parse the UserPasswordFile!");
        }
    }
    if (ok) {
        ok = userPasswordDatabase.MoveToChild(0u);
        if (!ok) {
            REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "Authentication key not defined in the UserPasswordFile!");
        }
    }
    if (ok) {
        ok = userPasswordDatabase.Read("Username", username);
        if (!ok) {
            REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "Failed to read 'Username'");
        }
        if (ok) {
            ok = userPasswordDatabase.Read("Password", password);
            if (!ok) {
                REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "Failed to read 'Password'");
            }
        }
    }
    return ok;
}

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

