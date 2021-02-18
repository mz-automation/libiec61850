/*
 * config.h (template for cmake)
 *
 * Contains global defines to configure the stack. You need to recompile the stack to make
 * changes effective (make clean; make)
 *
 */

#ifndef STACK_CONFIG_H_
#define STACK_CONFIG_H_

/* set to 0 for a little-endian target, 1 for a big-endian target */
#cmakedefine01 PLATFORM_IS_BIGENDIAN

/* define if the system supports clock_gettime */
#cmakedefine CONFIG_SYSTEM_HAS_CLOCK_GETTIME

/* include asserts if set to 1 */
#cmakedefine01 DEBUG

/* print debugging information with printf if set to 1 */
#cmakedefine01 DEBUG_SOCKET
#cmakedefine01 DEBUG_COTP
#cmakedefine01 DEBUG_ISO_SERVER
#cmakedefine01 DEBUG_ISO_CLIENT
#cmakedefine01 DEBUG_IED_SERVER
#cmakedefine01 DEBUG_IED_CLIENT
#cmakedefine01 DEBUG_MMS_CLIENT
#cmakedefine01 DEBUG_MMS_SERVER
#cmakedefine01 DEBUG_GOOSE_SUBSCRIBER
#cmakedefine01 DEBUG_GOOSE_PUBLISHER
#cmakedefine01 DEBUG_SV_SUBSCRIBER
#cmakedefine01 DEBUG_SV_PUBLISHER
#cmakedefine01 DEBUG_HAL_ETHERNET

/* 1 ==> server runs in single threaded mode (one dedicated thread for the server)
 * 0 ==> server runs in multi threaded mode (one thread for each connection and
 * one server background thread )
 */
#cmakedefine01 CONFIG_MMS_SINGLE_THREADED

/* Optimize stack for threadless operation - don't use semaphores */
#cmakedefine01 CONFIG_MMS_THREADLESS_STACK

/* Maximum MMS PDU SIZE - default is 65000 */
#cmakedefine CONFIG_MMS_MAXIMUM_PDU_SIZE @CONFIG_MMS_MAXIMUM_PDU_SIZE@

/* number of concurrent MMS client connections the server accepts, -1 for no limit */
#cmakedefine CONFIG_MAXIMUM_TCP_CLIENT_CONNECTIONS @CONFIG_MAXIMUM_TCP_CLIENT_CONNECTIONS@

/* activate TCP keep alive mechanism. 1 -> activate */
#cmakedefine01 CONFIG_ACTIVATE_TCP_KEEPALIVE

/* time (in s) between last message and first keepalive message */
#define CONFIG_TCP_KEEPALIVE_IDLE 5

/* time between subsequent keepalive messages if no ack received */
#define CONFIG_TCP_KEEPALIVE_INTERVAL 2

/* number of not missing keepalive responses until socket is considered dead */
#define CONFIG_TCP_KEEPALIVE_CNT 2

/* maximum COTP (ISO 8073) TPDU size - valid range is 1024 - 8192 */
#define CONFIG_COTP_MAX_TPDU_SIZE 8192

/* Ethernet interface ID for GOOSE and SV */
#define CONFIG_ETHERNET_INTERFACE_ID "eth0"
/* #define CONFIG_ETHERNET_INTERFACE_ID "vboxnet0" */
/* #define CONFIG_ETHERNET_INTERFACE_ID "en0"  // OS X uses enX in place of ethX as ethernet NIC names. */

/* Set to 1 to include GOOSE support in the build. Otherwise set to 0 */
#cmakedefine01 CONFIG_INCLUDE_GOOSE_SUPPORT

/* Set to 1 to include Sampled Values support in the build. Otherwise set to 0 */
#define CONFIG_IEC61850_SAMPLED_VALUES_SUPPORT 1

/* Set to 1 to compile for edition 1 server - default is 0 to compile for edition 2 */
#define CONFIG_IEC61850_EDITION_1 0

#ifdef _WIN32

/* GOOSE will be disabled for Windows if ethernet support (winpcap) is not available */
#ifdef EXCLUDE_ETHERNET_WINDOWS
#ifdef CONFIG_INCLUDE_GOOSE_SUPPORT
#undef CONFIG_INCLUDE_GOOSE_SUPPORT
#endif
#define CONFIG_INCLUDE_GOOSE_SUPPORT 0
#define CONFIG_INCUDE_ETHERNET_WINDOWS 0
#else
#define CONFIG_INCLUDE_ETHERNET_WINDOWS 1
#undef CONFIG_ETHERNET_INTERFACE_ID
#define CONFIG_ETHERNET_INTERFACE_ID "0"
#endif
#endif

/* The GOOSE retransmission interval in ms for the stable condition - i.e. no monitored value changed */
#define CONFIG_GOOSE_STABLE_STATE_TRANSMISSION_INTERVAL 5000

/* The GOOSE retransmission interval in ms in the case an event happens. */
#define CONFIG_GOOSE_EVENT_RETRANSMISSION_INTERVAL 500

/* The number of GOOSE retransmissions after an event */
#define CONFIG_GOOSE_EVENT_RETRANSMISSION_COUNT 2

/* Define if GOOSE control block elements are writable (1) or read-only (0) */
#define CONFIG_GOOSE_GOID_WRITABLE 0
#define CONFIG_GOOSE_DATSET_WRITABLE 0
#define CONFIG_GOOSE_CONFREV_WRITABLE 0
#define CONFIG_GOOSE_NDSCOM_WRITABLE 0
#define CONFIG_GOOSE_DSTADDRESS_WRITABLE 0
#define CONFIG_GOOSE_MINTIME_WRITABLE 0
#define CONFIG_GOOSE_MAXTIME_WRITABLE 0
#define CONFIG_GOOSE_FIXEDOFFS_WRITABLE 0

/* The default value for the priority field of the 802.1Q header (allowed range 0-7) */
#define CONFIG_GOOSE_DEFAULT_PRIORITY 4

/* The default value for the VLAN ID field of the 802.1Q header - the allowed range is 2-4096 or 0 if VLAN/priority is not used */
#define CONFIG_GOOSE_DEFAULT_VLAN_ID 0

/* Configure the 16 bit APPID field in the GOOSE header */
#define CONFIG_GOOSE_DEFAULT_APPID 0x1000

/* Default destination MAC address for GOOSE */
#define CONFIG_GOOSE_DEFAULT_DST_ADDRESS {0x01, 0x0c, 0xcd, 0x01, 0x00, 0x01}

/* include support for IEC 61850 control services */
#cmakedefine01 CONFIG_IEC61850_CONTROL_SERVICE

/* The default select timeout in ms. This will apply only if no sboTimeout attribute exists for a control object. Set to 0 for no timeout. */
#define CONFIG_CONTROL_DEFAULT_SBO_TIMEOUT 15000

/* include support for IEC 61850 reporting services */
#cmakedefine01 CONFIG_IEC61850_REPORT_SERVICE

/* support buffered report control blocks with ResvTms field */
#define CONFIG_IEC61850_BRCB_WITH_RESVTMS 1

/* allow only configured clients (when pre-configured by ClientLN) - note behavior in PIXIT Rp13 */
#define CONFIG_IEC61850_RCB_ALLOW_ONLY_PRECONFIGURED_CLIENT 0

/* The default buffer size of buffered RCBs in bytes */
#cmakedefine CONFIG_REPORTING_DEFAULT_REPORT_BUFFER_SIZE @CONFIG_REPORTING_DEFAULT_REPORT_BUFFER_SIZE@

/* include support for setting groups */
#cmakedefine01 CONFIG_IEC61850_SETTING_GROUPS

/* default reservation time of a setting group control block in s */
#define CONFIG_IEC61850_SG_RESVTMS 100

/* include support for IEC 61850 log services */
#cmakedefine01 CONFIG_IEC61850_LOG_SERVICE

/* include support for IEC 61850 service tracking */
#cmakedefine01 CONFIG_IEC61850_SERVICE_TRACKING

/* allow user to control read access by callback */
#cmakedefine01 CONFIG_IEC61850_SUPPORT_USER_READ_ACCESS_CONTROL

/* allow application to set server identity (for MMS identity service) at runtime */
#define CONFIG_IEC61850_SUPPORT_SERVER_IDENTITY 1

/* Force memory alignment - required for some platforms (required more memory for buffered reporting) */
#define CONFIG_IEC61850_FORCE_MEMORY_ALIGNMENT 1

/* default results for MMS identify service */
#define CONFIG_DEFAULT_MMS_VENDOR_NAME "libiec61850.com"
#define CONFIG_DEFAULT_MMS_MODEL_NAME "LIBIEC61850"
#define CONFIG_DEFAULT_MMS_REVISION "${LIB_VERSION_MAJOR}.${LIB_VERSION_MINOR}.${LIB_VERSION_PATCH}"

/* support flatted named variable name space required by IEC 61850-8-1 MMS mapping */
#define CONFIG_MMS_SUPPORT_FLATTED_NAME_SPACE 1

/* VMD scope named variables are not used by IEC 61850 */
#define CONFIG_MMS_SUPPORT_VMD_SCOPE_NAMED_VARIABLES 0

/* MMS virtual file store base path - where file services are looking for files */
#define CONFIG_VIRTUAL_FILESTORE_BASEPATH "./vmd-filestore/"

/* Maximum number of open file per MMS connection (for MMS file read service) */
#define CONFIG_MMS_MAX_NUMBER_OF_OPEN_FILES_PER_CONNECTION 5

#define CONFIG_MMS_MAX_NUMBER_OF_DOMAIN_SPECIFIC_DATA_SETS 10

#define CONFIG_MMS_MAX_NUMBER_OF_ASSOCIATION_SPECIFIC_DATA_SETS 10

#define CONFIG_MMS_MAX_NUMBER_OF_VMD_SPECIFIC_DATA_SETS 10

/* Maximum number of the members in a data set (named variable list) */
#define CONFIG_MMS_MAX_NUMBER_OF_DATA_SET_MEMBERS 50

/* Definition of supported services */
#define MMS_DEFAULT_PROFILE 1

#if MMS_DEFAULT_PROFILE
#define MMS_READ_SERVICE 1
#define MMS_WRITE_SERVICE 1
#define MMS_GET_NAME_LIST 1
#define MMS_JOURNAL_SERVICE 1
#define MMS_GET_VARIABLE_ACCESS_ATTRIBUTES 1
#define MMS_DATA_SET_SERVICE 1
#define MMS_DYNAMIC_DATA_SETS 1
#define MMS_GET_DATA_SET_ATTRIBUTES 1
#define MMS_STATUS_SERVICE 1
#define MMS_IDENTIFY_SERVICE 1
#define MMS_FILE_SERVICE 1
#define MMS_OBTAIN_FILE_SERVICE 1
#define MMS_DELETE_FILE_SERVICE 1
#define MMS_RENAME_FILE_SERVICE 0
#endif /* MMS_DEFAULT_PROFILE */

/* Sort getNameList response according to the MMS specified collation order - this is required by the standard
 * Set to 0 only for performance reasons and when no certification is required! */
#define CONFIG_MMS_SORT_NAME_LIST 1

#define CONFIG_INCLUDE_PLATFORM_SPECIFIC_HEADERS 0

/* use short FC defines as in old API */
#define CONFIG_PROVIDE_OLD_FC_DEFINES 0

/* Support user acccess to raw messages */
#cmakedefine01 CONFIG_MMS_RAW_MESSAGE_LOGGING

/* Allow to set the virtual filestore basepath for MMS file services at runtime with the
 * MmsServer_setFilestoreBasepath function
 */
#define CONFIG_SET_FILESTORE_BASEPATH_AT_RUNTIME 1

/* enable to configure MmsServer at runtime */
#define CONFIG_MMS_SERVER_CONFIG_SERVICES_AT_RUNTIME 1

/************************************************************************************
 * Check configuration for consistency - DO NOT MODIFY THIS PART!
 ************************************************************************************/

#if (MMS_JOURNAL_SERVICE != 1)

#if (CONFIG_IEC61850_LOG_SERVICE == 1)
#warning "Invalid configuration: CONFIG_IEC61850_LOG_SERVICE requires MMS_JOURNAL_SERVICE!"
#endif

#undef CONFIG_IEC61850_LOG_SERVICE
#define CONFIG_IEC61850_LOG_SERVICE 0

#endif

#if (MMS_WRITE_SERVICE != 1)

#if (CONFIG_IEC61850_CONTROL_SERVICE == 1)
#warning "Invalid configuration: CONFIG_IEC61850_CONTROL_SERVICE requires MMS_WRITE_SERVICE!"
#endif

#undef CONFIG_IEC61850_CONTROL_SERVICE
#define CONFIG_IEC61850_CONTROL_SERVICE 0
#endif

#if (MMS_FILE_SERVICE != 1)

#if (MMS_OBTAIN_FILE_SERVICE == 1)
#warning "Invalid configuration: MMS_OBTAIN_FILE_SERVICE requires MMS_FILE_SERVICE!"
#endif

#undef MMS_OBTAIN_FILE_SERVICE
#define MMS_OBTAIN_FILE_SERVICE 0
#endif

#endif /* STACK_CONFIG_H_ */
