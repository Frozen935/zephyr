/*
 * Copyright (c) 2018 Texas Instruments, Incorporated
 * Copyright (c) 2023 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file
 * @brief IEEE 802.11 protocol and general Wi-Fi definitions.
 */

/**
 * @brief Wi-Fi Management API.
 * @defgroup wifi_mgmt Wi-Fi Management
 * @since 1.12
 * @version 0.8.0
 * @ingroup networking
 * @{
 */

#ifndef ZEPHYR_INCLUDE_NET_WIFI_H_
#define ZEPHYR_INCLUDE_NET_WIFI_H_

#include <zephyr/sys/util.h>  /* for ARRAY_SIZE */

/** Length of the country code string */
#define WIFI_COUNTRY_CODE_LEN 2

/** @cond INTERNAL_HIDDEN */

#define WIFI_LISTEN_INTERVAL_MIN 0
#define WIFI_LISTEN_INTERVAL_MAX 65535

/** @endcond */

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Wi-Fi connect result codes. To be overlaid on top of \ref wifi_status
 * in the connect result event for detailed status.
 */
enum wifi_conn_status {
	/** Connection successful */
	WIFI_STATUS_CONN_SUCCESS = 0,
	/** Connection failed - generic failure */
	WIFI_STATUS_CONN_FAIL,
	/** Connection failed - wrong password
	 * Few possible reasons for 4-way handshake failure that we can guess are as follows:
	 * 1) Incorrect key
	 * 2) EAPoL frames lost causing timeout
	 *
	 * #1 is the likely cause, so, we convey to the user that it is due to
	 * Wrong passphrase/password.
	 */
	WIFI_STATUS_CONN_WRONG_PASSWORD,
	/** Connection timed out */
	WIFI_STATUS_CONN_TIMEOUT,
	/** Connection failed - AP not found */
	WIFI_STATUS_CONN_AP_NOT_FOUND,
	/** Last connection status */
	WIFI_STATUS_CONN_LAST_STATUS,
	/** Connection disconnected status */
	WIFI_STATUS_DISCONN_FIRST_STATUS = WIFI_STATUS_CONN_LAST_STATUS,
};

/** @brief IEEE 802.11 security types. */
enum wifi_security_type {
	/** No security. */
	WIFI_SECURITY_TYPE_NONE = 0,
	/** WPA2-PSK security. */
	WIFI_SECURITY_TYPE_PSK,
	/** WPA2-PSK-SHA256 security. */
	WIFI_SECURITY_TYPE_PSK_SHA256,
	/** WPA3-SAE security. */
	WIFI_SECURITY_TYPE_SAE,
	/** WPA3-SAE security with hunting-and-pecking loop. */
	WIFI_SECURITY_TYPE_SAE_HNP = WIFI_SECURITY_TYPE_SAE,
	/** WPA3-SAE security with hash-to-element. */
	WIFI_SECURITY_TYPE_SAE_H2E,
	/** WPA3-SAE security with both hunting-and-pecking loop and hash-to-element enabled. */
	WIFI_SECURITY_TYPE_SAE_AUTO,
	/** GB 15629.11-2003 WAPI security. */
	WIFI_SECURITY_TYPE_WAPI,
	/** EAP security - Enterprise. */
	WIFI_SECURITY_TYPE_EAP,
	/** EAP TLS security - Enterprise. */
	WIFI_SECURITY_TYPE_EAP_TLS = WIFI_SECURITY_TYPE_EAP,
	/** WEP security. */
	WIFI_SECURITY_TYPE_WEP,
	/** WPA-PSK security. */
	WIFI_SECURITY_TYPE_WPA_PSK,
	/** WPA/WPA2/WPA3 PSK security. */
	WIFI_SECURITY_TYPE_WPA_AUTO_PERSONAL,
	/** DPP security */
	WIFI_SECURITY_TYPE_DPP,
	/** EAP PEAP MSCHAPV2 security - Enterprise. */
	WIFI_SECURITY_TYPE_EAP_PEAP_MSCHAPV2,
	/** EAP PEAP GTC security - Enterprise. */
	WIFI_SECURITY_TYPE_EAP_PEAP_GTC,
	/** EAP TTLS MSCHAPV2 security - Enterprise. */
	WIFI_SECURITY_TYPE_EAP_TTLS_MSCHAPV2,
	/** EAP PEAP security - Enterprise. */
	WIFI_SECURITY_TYPE_EAP_PEAP_TLS,
	/** FT-PSK security */
	WIFI_SECURITY_TYPE_FT_PSK,
	/** FT-SAE security */
	WIFI_SECURITY_TYPE_FT_SAE,
	/** FT-EAP security */
	WIFI_SECURITY_TYPE_FT_EAP,
	/** FT-EAP-SHA384 security */
	WIFI_SECURITY_TYPE_FT_EAP_SHA384,
	/** SAE Extended key (uses group-dependent hashing) */
	WIFI_SECURITY_TYPE_SAE_EXT_KEY,

	/** @cond INTERNAL_HIDDEN */
	__WIFI_SECURITY_TYPE_AFTER_LAST,
	WIFI_SECURITY_TYPE_MAX = __WIFI_SECURITY_TYPE_AFTER_LAST - 1,
	WIFI_SECURITY_TYPE_UNKNOWN
	/** @endcond */
};

/** @brief EPA method Types. */
enum wifi_eap_type {
	/** No EPA  security. */
	WIFI_EAP_TYPE_NONE = 0,
	/** EPA GTC security, refer to rfc3748 chapter 5. */
	WIFI_EAP_TYPE_GTC = 6,
	/** EPA TLS security, refer to rfc5216. */
	WIFI_EAP_TYPE_TLS = 13,
	/** EPA TTLS security, refer to rfc5281. */
	WIFI_EAP_TYPE_TTLS = 21,
	/** EPA PEAP security, refer to draft-josefsson-pppext-eap-tls-eap-06.txt. */
	WIFI_EAP_TYPE_PEAP = 25,
	/** EPA MSCHAPV2 security, refer to draft-kamath-pppext-eap-mschapv2-00.txt. */
	WIFI_EAP_TYPE_MSCHAPV2 = 26,
};

/** @brief WPA3 Enterprise security types.
 *
 * See Section#3 in WFA WPA3 specification v3.4:
 * https://www.wi-fi.org/file/wpa3-specification for details.
 */
enum wifi_wpa3_enterprise_type {
	/** No WPA3 enterprise, either WPA2 Enterprise or personal mode */
	WIFI_WPA3_ENTERPRISE_NA = 0,
	/** WPA3 enterprise Suite-B (PMFR + WPA3-Suite-B). */
	WIFI_WPA3_ENTERPRISE_SUITEB = 1,
	/** WPA3 enterprise Suite-B-192 (PMFR + WPA3-Suite-B-192). */
	WIFI_WPA3_ENTERPRISE_SUITEB_192,
	/** WPA3 enterprise only (PMFR + WPA2-ENT disabled). */
	WIFI_WPA3_ENTERPRISE_ONLY,

	/** @cond INTERNAL_HIDDEN */
	__WIFI_WPA3_ENTERPRISE_AFTER_LAST,
	WIFI_WPA3_ENTERPRISE_MAX = __WIFI_WPA3_ENTERPRISE_AFTER_LAST - 1,
	WIFI_WPA3_ENTERPRISE_UNKNOWN
	/** @endcond */
};

enum wifi_eap_tls_cipher_type {
	/** EAP TLS with NONE */
	WIFI_EAP_TLS_NONE,
	/** EAP TLS with ECDH & ECDSA with p384 */
	WIFI_EAP_TLS_ECC_P384,
	/** EAP TLS with ECDH & RSA with > 3K */
	WIFI_EAP_TLS_RSA_3K,
};

/** @brief Group cipher and pairwise cipher types. */
enum wifi_cipher_type {
	/** AES in counter mode with CBC-MAC (CCMP-128). */
	WPA_CAPA_ENC_CCMP,
	/** 128-bit Galois/Counter Mode Protocol. */
	WPA_CAPA_ENC_GCMP,
	/** 256-bit Galois/Counter Mode Protocol. */
	WPA_CAPA_ENC_GCMP_256,
};

/** @brief group mgmt cipher types. */
enum wifi_group_mgmt_cipher_type {
	/** 128-bit Broadcast/Multicast Integrity Protocol
	 * Cipher-based Message Authentication Code .
	 */
	WPA_CAPA_ENC_BIP,
	/** 128-bit Broadcast/Multicast Integrity Protocol
	 * Galois Message Authentication Code .
	 */
	WPA_CAPA_ENC_BIP_GMAC_128,
	/** 256-bit Broadcast/Multicast Integrity Protocol
	 * Galois Message Authentication Code .
	 */
	WPA_CAPA_ENC_BIP_GMAC_256,
};

struct wifi_cipher_desc {
	/** Cipher capability. */
	unsigned int capa;
	/** Cipher name string. */
	char *name;
};

struct wifi_eap_cipher_config {
	/** Key management type string. */
	char *key_mgmt;
	/** OpenSSL cipher string. */
	char *openssl_ciphers;
	/** Group cipher cipher string. */
	char *group_cipher;
	/** Pairwise_cipher cipher string. */
	char *pairwise_cipher;
	/** Group management cipher string. */
	char *group_mgmt_cipher;
	/** Used to confiure TLS features. */
	char *tls_flags;
};

struct wifi_eap_config {
	/**  Security type. */
	enum wifi_security_type type;
	/** EAP method type of phase1. */
	enum wifi_eap_type eap_type_phase1;
	/** EAP method type of phase2. */
	enum wifi_eap_type eap_type_phase2;
	/** EAP method string. */
	char *method;
	/** Phase2 setting string. */
	char *phase2;
};

/** Helper function to get user-friendly security type name. */
const char *wifi_security_txt(enum wifi_security_type security);

/** Helper function to get user-friendly wpa3 enterprise security type name. */
const char *wifi_wpa3_enterprise_txt(enum wifi_wpa3_enterprise_type wpa3_ent);

/** @brief IEEE 802.11w - Management frame protection. */
enum wifi_mfp_options {
	/** MFP disabled. */
	WIFI_MFP_DISABLE = 0,
	/** MFP optional. */
	WIFI_MFP_OPTIONAL,
	/** MFP required. */
	WIFI_MFP_REQUIRED,

/** @cond INTERNAL_HIDDEN */
	__WIFI_MFP_AFTER_LAST,
	WIFI_MFP_MAX = __WIFI_MFP_AFTER_LAST - 1,
	WIFI_MFP_UNKNOWN
/** @endcond */
};

/** Helper function to get user-friendly MFP name.*/
const char *wifi_mfp_txt(enum wifi_mfp_options mfp);

/**
 * @brief IEEE 802.11 operational frequency bands (not exhaustive).
 */
enum wifi_frequency_bands {
	/** 2.4 GHz band. */
	WIFI_FREQ_BAND_2_4_GHZ = 0,
	/** 5 GHz band. */
	WIFI_FREQ_BAND_5_GHZ,
	/** 6 GHz band (Wi-Fi 6E, also extends to 7GHz). */
	WIFI_FREQ_BAND_6_GHZ,

	/** Number of frequency bands available. */
	__WIFI_FREQ_BAND_AFTER_LAST,
	/** Highest frequency band available. */
	WIFI_FREQ_BAND_MAX = __WIFI_FREQ_BAND_AFTER_LAST - 1,
	/** Invalid frequency band */
	WIFI_FREQ_BAND_UNKNOWN
};

/** Helper function to get user-friendly frequency band name. */
const char *wifi_band_txt(enum wifi_frequency_bands band);

/**
 * @brief IEEE 802.11 operational frequency bandwidths (not exhaustive).
 */
enum wifi_frequency_bandwidths {
	/** 20 MHz. */
	WIFI_FREQ_BANDWIDTH_20MHZ = 1,
	/** 40 MHz. */
	WIFI_FREQ_BANDWIDTH_40MHZ,
	/** 80 MHz. */
	WIFI_FREQ_BANDWIDTH_80MHZ,

	/** Number of frequency bandwidths available. */
	__WIFI_FREQ_BANDWIDTH_AFTER_LAST,
	/** Highest frequency bandwidth available. */
	WIFI_FREQ_BANDWIDTH_MAX = __WIFI_FREQ_BANDWIDTH_AFTER_LAST - 1,
	/** Invalid frequency bandwidth */
	WIFI_FREQ_BANDWIDTH_UNKNOWN
};

const char *wifi_bandwidth_txt(enum wifi_frequency_bandwidths bandwidth);

/** Max SSID length */
#define WIFI_SSID_MAX_LEN 32
/** Minimum PSK length */
#define WIFI_PSK_MIN_LEN 8
/** Maximum PSK length */
#define WIFI_PSK_MAX_LEN 64
/** Max SAW password length */
#define WIFI_SAE_PSWD_MAX_LEN 128
/** MAC address length */
#define WIFI_MAC_ADDR_LEN 6
/** Max enterprise identity length */
#define WIFI_ENT_IDENTITY_MAX_LEN 64
/** Max enterprise password length */
#define WIFI_ENT_PSWD_MAX_LEN 128

/** Minimum channel number */
#define WIFI_CHANNEL_MIN 1
/** Maximum channel number */
#define WIFI_CHANNEL_MAX 233
/** Any channel number */
#define WIFI_CHANNEL_ANY 255

/** @brief Wi-Fi interface states.
 *
 * Based on https://w1.fi/wpa_supplicant/devel/defs_8h.html#a4aeb27c1e4abd046df3064ea9756f0bc
 */
enum wifi_iface_state {
	/** Interface is disconnected. */
	WIFI_STATE_DISCONNECTED = 0,
	/** Interface is disabled (administratively). */
	WIFI_STATE_INTERFACE_DISABLED,
	/** No enabled networks in the configuration. */
	WIFI_STATE_INACTIVE,
	/** Interface is scanning for networks. */
	WIFI_STATE_SCANNING,
	/** Authentication with a network is in progress. */
	WIFI_STATE_AUTHENTICATING,
	/** Association with a network is in progress. */
	WIFI_STATE_ASSOCIATING,
	/** Association with a network completed. */
	WIFI_STATE_ASSOCIATED,
	/** 4-way handshake with a network is in progress. */
	WIFI_STATE_4WAY_HANDSHAKE,
	/** Group Key exchange with a network is in progress. */
	WIFI_STATE_GROUP_HANDSHAKE,
	/** All authentication completed, ready to pass data. */
	WIFI_STATE_COMPLETED,

/** @cond INTERNAL_HIDDEN */
	__WIFI_STATE_AFTER_LAST,
	WIFI_STATE_MAX = __WIFI_STATE_AFTER_LAST - 1,
	WIFI_STATE_UNKNOWN
/** @endcond */
};

/* We rely on the strict order of the enum values, so, let's check it */
BUILD_ASSERT(WIFI_STATE_DISCONNECTED < WIFI_STATE_INTERFACE_DISABLED &&
	     WIFI_STATE_INTERFACE_DISABLED < WIFI_STATE_INACTIVE &&
	     WIFI_STATE_INACTIVE < WIFI_STATE_SCANNING &&
	     WIFI_STATE_SCANNING < WIFI_STATE_AUTHENTICATING &&
	     WIFI_STATE_AUTHENTICATING < WIFI_STATE_ASSOCIATING &&
	     WIFI_STATE_ASSOCIATING < WIFI_STATE_ASSOCIATED &&
	     WIFI_STATE_ASSOCIATED < WIFI_STATE_4WAY_HANDSHAKE &&
	     WIFI_STATE_4WAY_HANDSHAKE < WIFI_STATE_GROUP_HANDSHAKE &&
	     WIFI_STATE_GROUP_HANDSHAKE < WIFI_STATE_COMPLETED);


/** Helper function to get user-friendly interface state name. */
const char *wifi_state_txt(enum wifi_iface_state state);

/** @brief Wi-Fi interface modes.
 *
 * Based on https://w1.fi/wpa_supplicant/devel/defs_8h.html#a4aeb27c1e4abd046df3064ea9756f0bc
 */
enum wifi_iface_mode {
	/** Infrastructure station mode. */
	WIFI_MODE_INFRA = 0,
	/** IBSS (ad-hoc) station mode. */
	WIFI_MODE_IBSS = 1,
	/** AP mode. */
	WIFI_MODE_AP = 2,
	/** P2P group owner mode. */
	WIFI_MODE_P2P_GO = 3,
	/** P2P group formation mode. */
	WIFI_MODE_P2P_GROUP_FORMATION = 4,
	/** 802.11s Mesh mode. */
	WIFI_MODE_MESH = 5,

/** @cond INTERNAL_HIDDEN */
	__WIFI_MODE_AFTER_LAST,
	WIFI_MODE_MAX = __WIFI_MODE_AFTER_LAST - 1,
	WIFI_MODE_UNKNOWN
/** @endcond */
};

/** Helper function to get user-friendly interface mode name. */
const char *wifi_mode_txt(enum wifi_iface_mode mode);

/** @brief Wi-Fi link operating modes
 *
 * As per https://en.wikipedia.org/wiki/Wi-Fi#Versions_and_generations.
 */
enum wifi_link_mode {
	/** 802.11 (legacy). */
	WIFI_0 = 0,
	/** 802.11b. */
	WIFI_1,
	/** 802.11a. */
	WIFI_2,
	/** 802.11g. */
	WIFI_3,
	/** 802.11n. */
	WIFI_4,
	/** 802.11ac. */
	WIFI_5,
	/** 802.11ax. */
	WIFI_6,
	/** 802.11ax 6GHz. */
	WIFI_6E,
	/** 802.11be. */
	WIFI_7,

/** @cond INTERNAL_HIDDEN */
	__WIFI_LINK_MODE_AFTER_LAST,
	WIFI_LINK_MODE_MAX = __WIFI_LINK_MODE_AFTER_LAST - 1,
	WIFI_LINK_MODE_UNKNOWN
/** @endcond */
};

/** Helper function to get user-friendly link mode name. */
const char *wifi_link_mode_txt(enum wifi_link_mode link_mode);

/** @brief Wi-Fi scanning types. */
enum wifi_scan_type {
	/** Active scanning (default). */
	WIFI_SCAN_TYPE_ACTIVE = 0,
	/** Passive scanning. */
	WIFI_SCAN_TYPE_PASSIVE,
};

/** @brief Wi-Fi power save states. */
enum wifi_ps {
	/** Power save disabled. */
	WIFI_PS_DISABLED = 0,
	/** Power save enabled. */
	WIFI_PS_ENABLED,
};

/** Helper function to get user-friendly ps name. */
const char *wifi_ps_txt(enum wifi_ps ps_name);

/** @brief Wi-Fi power save modes. */
enum wifi_ps_mode {
	/** Legacy power save mode. */
	WIFI_PS_MODE_LEGACY = 0,
	/* This has to be configured before connecting to the AP,
	 * as support for ADDTS action frames is not available.
	 */
	/** WMM power save mode. */
	WIFI_PS_MODE_WMM,
};

/** Helper function to get user-friendly ps mode name. */
const char *wifi_ps_mode_txt(enum wifi_ps_mode ps_mode);

/** Network interface index min value */
#define WIFI_INTERFACE_INDEX_MIN 1
/** Network interface index max value */
#define WIFI_INTERFACE_INDEX_MAX 255

/** @brief Wifi operational mode */
enum wifi_operational_modes {
	/** STA mode setting enable */
	WIFI_STA_MODE = BIT(0),
	/** Monitor mode setting enable */
	WIFI_MONITOR_MODE = BIT(1),
	/** TX injection mode setting enable */
	WIFI_TX_INJECTION_MODE = BIT(2),
	/** Promiscuous mode setting enable */
	WIFI_PROMISCUOUS_MODE = BIT(3),
	/** AP mode setting enable */
	WIFI_AP_MODE = BIT(4),
	/** Softap mode setting enable */
	WIFI_SOFTAP_MODE = BIT(5),
};

/** @brief Mode filter settings */
enum wifi_filter {
	/** Support management, data and control packet sniffing */
	WIFI_PACKET_FILTER_ALL = BIT(0),
	/** Support only sniffing of management packets */
	WIFI_PACKET_FILTER_MGMT = BIT(1),
	/** Support only sniffing of data packets */
	WIFI_PACKET_FILTER_DATA = BIT(2),
	/** Support only sniffing of control packets */
	WIFI_PACKET_FILTER_CTRL = BIT(3),
};

/** @brief Wi-Fi Target Wake Time (TWT) operations. */
enum wifi_twt_operation {
	/** TWT setup operation */
	WIFI_TWT_SETUP = 0,
	/** TWT teardown operation */
	WIFI_TWT_TEARDOWN,
};

/** Helper function to get user-friendly twt operation name. */
const char *wifi_twt_operation_txt(enum wifi_twt_operation twt_operation);

/** @brief Wi-Fi Target Wake Time (TWT) negotiation types. */
enum wifi_twt_negotiation_type {
	/** TWT individual negotiation */
	WIFI_TWT_INDIVIDUAL = 0,
	/** TWT broadcast negotiation */
	WIFI_TWT_BROADCAST,
	/** TWT wake TBTT negotiation */
	WIFI_TWT_WAKE_TBTT
};

/** Helper function to get user-friendly twt negotiation type name. */
const char *wifi_twt_negotiation_type_txt(enum wifi_twt_negotiation_type twt_negotiation);

/** @brief Wi-Fi Target Wake Time (TWT) setup commands. */
enum wifi_twt_setup_cmd {
	/** TWT setup request */
	WIFI_TWT_SETUP_CMD_REQUEST = 0,
	/** TWT setup suggest (parameters can be changed by AP) */
	WIFI_TWT_SETUP_CMD_SUGGEST,
	/** TWT setup demand (parameters can not be changed by AP) */
	WIFI_TWT_SETUP_CMD_DEMAND,
	/** TWT setup grouping (grouping of TWT flows) */
	WIFI_TWT_SETUP_CMD_GROUPING,
	/** TWT setup accept (parameters accepted by AP) */
	WIFI_TWT_SETUP_CMD_ACCEPT,
	/** TWT setup alternate (alternate parameters suggested by AP) */
	WIFI_TWT_SETUP_CMD_ALTERNATE,
	/** TWT setup dictate (parameters dictated by AP) */
	WIFI_TWT_SETUP_CMD_DICTATE,
	/** TWT setup reject (parameters rejected by AP) */
	WIFI_TWT_SETUP_CMD_REJECT,
};

/** Helper function to get user-friendly twt setup cmd name. */
const char *wifi_twt_setup_cmd_txt(enum wifi_twt_setup_cmd twt_setup);

/** @brief Wi-Fi Target Wake Time (TWT) negotiation status. */
enum wifi_twt_setup_resp_status {
	/** TWT response received for TWT request */
	WIFI_TWT_RESP_RECEIVED = 0,
	/** TWT response not received for TWT request */
	WIFI_TWT_RESP_NOT_RECEIVED,
};

/** @brief Target Wake Time (TWT) error codes. */
enum wifi_twt_fail_reason {
	/** Unspecified error */
	WIFI_TWT_FAIL_UNSPECIFIED,
	/** Command execution failed */
	WIFI_TWT_FAIL_CMD_EXEC_FAIL,
	/** Operation not supported */
	WIFI_TWT_FAIL_OPERATION_NOT_SUPPORTED,
	/** Unable to get interface status */
	WIFI_TWT_FAIL_UNABLE_TO_GET_IFACE_STATUS,
	/** Device not connected to AP */
	WIFI_TWT_FAIL_DEVICE_NOT_CONNECTED,
	/** Peer not HE (802.11ax/Wi-Fi 6) capable */
	WIFI_TWT_FAIL_PEER_NOT_HE_CAPAB,
	/** Peer not TWT capable */
	WIFI_TWT_FAIL_PEER_NOT_TWT_CAPAB,
	/** A TWT flow is already in progress */
	WIFI_TWT_FAIL_OPERATION_IN_PROGRESS,
	/** Invalid negotiated flow id */
	WIFI_TWT_FAIL_INVALID_FLOW_ID,
	/** IP address not assigned or configured */
	WIFI_TWT_FAIL_IP_NOT_ASSIGNED,
	/** Flow already exists */
	WIFI_TWT_FAIL_FLOW_ALREADY_EXISTS,
};

/** @brief Wi-Fi Target Wake Time (TWT) teradown status. */
enum wifi_twt_teardown_status {
	/** TWT teardown success */
	WIFI_TWT_TEARDOWN_SUCCESS = 0,
	/** TWT teardown failure */
	WIFI_TWT_TEARDOWN_FAILED,
};

/** @cond INTERNAL_HIDDEN */
static const char * const wifi_twt_err_code_tbl[] = {
	[WIFI_TWT_FAIL_UNSPECIFIED] = "Unspecified",
	[WIFI_TWT_FAIL_CMD_EXEC_FAIL] = "Command Execution failed",
	[WIFI_TWT_FAIL_OPERATION_NOT_SUPPORTED] =
		"Operation not supported",
	[WIFI_TWT_FAIL_UNABLE_TO_GET_IFACE_STATUS] =
		"Unable to get iface status",
	[WIFI_TWT_FAIL_DEVICE_NOT_CONNECTED] =
		"Device not connected",
	[WIFI_TWT_FAIL_PEER_NOT_HE_CAPAB] = "Peer not HE capable",
	[WIFI_TWT_FAIL_PEER_NOT_TWT_CAPAB] = "Peer not TWT capable",
	[WIFI_TWT_FAIL_OPERATION_IN_PROGRESS] =
		"Operation already in progress",
	[WIFI_TWT_FAIL_INVALID_FLOW_ID] =
		"Invalid negotiated flow id",
	[WIFI_TWT_FAIL_IP_NOT_ASSIGNED] =
		"IP address not assigned",
	[WIFI_TWT_FAIL_FLOW_ALREADY_EXISTS] =
		"Flow already exists",
};
/** @endcond */

/** Helper function to get user-friendly TWT error code name. */
static inline const char *wifi_twt_get_err_code_str(int16_t err_no)
{
	if ((err_no) < (int16_t)ARRAY_SIZE(wifi_twt_err_code_tbl)) {
		return wifi_twt_err_code_tbl[err_no];
	}

	return "<unknown>";
}

/** @brief Wi-Fi power save parameters. */
enum wifi_ps_param_type {
	/** Power save state. */
	WIFI_PS_PARAM_STATE,
	/** Power save listen interval (units: (short) beacon intervals). */
	WIFI_PS_PARAM_LISTEN_INTERVAL,
	/** Power save wakeup mode. */
	WIFI_PS_PARAM_WAKEUP_MODE,
	/** Power save mode. */
	WIFI_PS_PARAM_MODE,
	/** Power save exit strategy. */
	WIFI_PS_PARAM_EXIT_STRATEGY,
	/** Power save timeout. */
	WIFI_PS_PARAM_TIMEOUT,
};

/** @brief Wi-Fi power save modes. */
enum wifi_ps_wakeup_mode {
	/** DTIM based wakeup. */
	WIFI_PS_WAKEUP_MODE_DTIM = 0,
	/** Listen interval based wakeup. */
	WIFI_PS_WAKEUP_MODE_LISTEN_INTERVAL,
};

/** Helper function to get user-friendly ps wakeup mode name. */
const char *wifi_ps_wakeup_mode_txt(enum wifi_ps_wakeup_mode ps_wakeup_mode);

/**
 * @brief Wi-Fi power save exit strategy
 */
enum wifi_ps_exit_strategy {
	/** PS-Poll frame based */
	WIFI_PS_EXIT_CUSTOM_ALGO = 0,
	/** QoS NULL frame based */
	WIFI_PS_EXIT_EVERY_TIM,

/** @cond INTERNAL_HIDDEN */
	WIFI_PS_EXIT_LAST,
	WIFI_PS_EXIT_MAX = WIFI_PS_EXIT_LAST - 1,
/** @endcond */
};

/** Helper function to get user-friendly ps exit strategy name. */
const char *wifi_ps_exit_strategy_txt(enum wifi_ps_exit_strategy ps_exit_strategy);

/** @brief Wi-Fi power save error codes. */
enum wifi_config_ps_param_fail_reason {
	/** Unspecified error */
	WIFI_PS_PARAM_FAIL_UNSPECIFIED,
	/** Command execution failed */
	WIFI_PS_PARAM_FAIL_CMD_EXEC_FAIL,
	/** Parameter not supported */
	WIFI_PS_PARAM_FAIL_OPERATION_NOT_SUPPORTED,
	/** Unable to get interface status */
	WIFI_PS_PARAM_FAIL_UNABLE_TO_GET_IFACE_STATUS,
	/** Device not connected to AP */
	WIFI_PS_PARAM_FAIL_DEVICE_NOT_CONNECTED,
	/** Device already connected to AP */
	WIFI_PS_PARAM_FAIL_DEVICE_CONNECTED,
	/** Listen interval out of range */
	WIFI_PS_PARAM_LISTEN_INTERVAL_RANGE_INVALID,
	/** Invalid exit strategy */
	WIFI_PS_PARAM_FAIL_INVALID_EXIT_STRATEGY,
};

/** @cond INTERNAL_HIDDEN */
static const char * const wifi_ps_param_config_err_code_tbl[] = {
	[WIFI_PS_PARAM_FAIL_UNSPECIFIED] = "Unspecified",
	[WIFI_PS_PARAM_FAIL_CMD_EXEC_FAIL] = "Command Execution failed",
	[WIFI_PS_PARAM_FAIL_OPERATION_NOT_SUPPORTED] =
		"Operation not supported",
	[WIFI_PS_PARAM_FAIL_UNABLE_TO_GET_IFACE_STATUS] =
		"Unable to get iface status",
	[WIFI_PS_PARAM_FAIL_DEVICE_NOT_CONNECTED] =
		"Cannot set parameters while device not connected",
	[WIFI_PS_PARAM_FAIL_DEVICE_CONNECTED] =
		"Cannot set parameters while device connected",
	[WIFI_PS_PARAM_LISTEN_INTERVAL_RANGE_INVALID] =
		"Parameter out of range",
};
/** @endcond */

/** IEEE 802.11v BTM (BSS transition management) Query reasons.
 * Refer to IEEE Std 802.11v-2011 - Table 7-43x-Transition and Transition Query reasons table.
 */
enum wifi_btm_query_reason {
	/** Unspecified. */
	WIFI_BTM_QUERY_REASON_UNSPECIFIED = 0,
	/** Low RSSI. */
	WIFI_BTM_QUERY_REASON_LOW_RSSI = 16,
	/** Leaving ESS. */
	WIFI_BTM_QUERY_REASON_LEAVING_ESS = 20,
};

/** Helper function to get user-friendly power save error code name. */
static inline const char *wifi_ps_get_config_err_code_str(int16_t err_no)
{
	if ((err_no) < (int16_t)ARRAY_SIZE(wifi_ps_param_config_err_code_tbl)) {
		return wifi_ps_param_config_err_code_tbl[err_no];
	}

	return "<unknown>";
}

/** @brief Wi-Fi AP mode configuration parameter */
enum wifi_ap_config_param {
	/** Used for AP mode configuration parameter ap_max_inactivity */
	WIFI_AP_CONFIG_PARAM_MAX_INACTIVITY = BIT(0),
	/** Used for AP mode configuration parameter max_num_sta */
	WIFI_AP_CONFIG_PARAM_MAX_NUM_STA = BIT(1),
	/** Used for AP mode configuration parameter bandwidth */
	WIFI_AP_CONFIG_PARAM_BANDWIDTH = BIT(2),
	/** Used for AP mode configuration parameter ht_capab */
	WIFI_AP_CONFIG_PARAM_HT_CAPAB = BIT(3),
	/** Used for AP mode configuration parameter vht_capab */
	WIFI_AP_CONFIG_PARAM_VHT_CAPAB = BIT(4),
};

/** @brief Wi-Fi STA mode configuration parameter */
enum wifi_config_param {
	/** Used for STA mode configuration parameter OKC */
	WIFI_CONFIG_PARAM_OKC = BIT(0),
};

/** Helper function to get user-friendly status name for the status code. */
const char *wifi_conn_status_txt(enum wifi_conn_status status);

#ifdef __cplusplus
}
#endif

/**
 * @}
 */
#endif /* ZEPHYR_INCLUDE_NET_WIFI_H_ */
