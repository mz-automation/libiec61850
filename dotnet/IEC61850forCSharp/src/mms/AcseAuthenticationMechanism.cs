using System;
using System.Collections.Generic;
using System.Text;

namespace IEC61850
{
    public enum AcseAuthenticationMechanism
    {
        /** Neither ACSE nor TLS authentication used */
        ACSE_AUTH_NONE = 0,

        /** Use ACSE password for client authentication */
        ACSE_AUTH_PASSWORD = 1,

        /** Use ACSE certificate for client authentication */
        ACSE_AUTH_CERTIFICATE = 2,

        /** Use TLS certificate for client authentication */
        ACSE_AUTH_TLS = 3
    }
}
