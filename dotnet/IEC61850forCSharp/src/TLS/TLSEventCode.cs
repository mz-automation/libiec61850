namespace IEC61850
{
    namespace TLS
    {
        public enum TLSEventCode
        {
            ALM_ALGO_NOT_SUPPORTED = 1,
            ALM_UNSECURE_COMMUNICATION = 2,
            ALM_CERT_UNAVAILABLE = 3,
            ALM_BAD_CERT = 4,
            ALM_CERT_SIZE_EXCEEDED = 5,
            ALM_CERT_VALIDATION_FAILED = 6,
            ALM_CERT_REQUIRED = 7,
            ALM_HANDSHAKE_FAILED_UNKNOWN_REASON = 8,
            WRN_INSECURE_TLS_VERSION = 9,
            INF_SESSION_RENEGOTIATION = 10,
            ALM_CERT_EXPIRED = 11,
            ALM_CERT_REVOKED = 12,
            ALM_CERT_NOT_CONFIGURED = 13,
            ALM_CERT_NOT_TRUSTED = 14,
            ALM_NO_CIPHER = 15,
            INF_SESSION_ESTABLISHED = 16,
            WRN_CERT_EXPIRED = 17,
            WRN_CERT_NOT_YET_VALID = 18,
            WRN_CRL_EXPIRED = 19,
            WRN_CRL_NOT_YET_VALID = 20,
            ALM_TLS_VERSION_CHANGE = 21,
            WRN_MIN_KEY_LENGTH = 22,
            ALM_INSUFFICIENT_KEY_LENGTH = 23,
            WRN_CRL_NOT_ACCESSIBLE = 24,
            ALM_CA_CERT_NOT_AVAILABLE = 25,
            ALM_RENEGOTIATION_TIMEOUT = 26,
            INF_SESSION_RESUMED = 27,
            INF_SESSION_EXPIRED = 28
        }
    }
}