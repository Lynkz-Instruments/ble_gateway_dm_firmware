/**
 * @file attr_table.h
 * @brief
 *
 * Copyright (c) 2022 Laird Connectivity LLC
 *
 * SPDX-License-Identifier: LicenseRef-LairdConnectivity-Clause
 */

#ifndef __ATTR_TABLE_H__
#define __ATTR_TABLE_H__

/**************************************************************************************************/
/* Includes                                                                                       */
/**************************************************************************************************/
#include <zephyr.h>
#include <zephyr/types.h>
#include <stddef.h>

#include "attr_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************/
/* Global Constants, Macros and Type Definitions                                                  */
/**************************************************************************************************/
/* pystart - enumerations */
enum lwm2m_security {
	LWM2M_SECURITY_PSK = 0,
	LWM2M_SECURITY_RPK = 1,
	LWM2M_SECURITY_CERT = 2,
	LWM2M_SECURITY_NO_SEC = 3,
	LWM2M_SECURITY_CERT_EST = 4,
};

enum lwm2m_pwr_src {
	LWM2M_PWR_SRC_DC = 0,
	LWM2M_PWR_SRC_INT_BATT = 1,
	LWM2M_PWR_SRC_EXT_BATT = 2,
	LWM2M_PWR_SRC_FUEL_CELL = 3,
	LWM2M_PWR_SRC_POE = 4,
	LWM2M_PWR_SRC_USB = 5,
	LWM2M_PWR_SRC_AC = 6,
	LWM2M_PWR_SRC_SOLAR = 7,
};

enum lwm2m_batt_stat {
	LWM2M_BATT_STAT_NORM = 0,
	LWM2M_BATT_STAT_CHARGING = 1,
	LWM2M_BATT_STAT_CHARGE_COMP = 2,
	LWM2M_BATT_STAT_DAMAGED = 3,
	LWM2M_BATT_STAT_LOW = 4,
	LWM2M_BATT_STAT_NOT_INST = 5,
	LWM2M_BATT_STAT_UNKNOWN = 6,
};

enum memfault_transport {
	MEMFAULT_TRANSPORT_NONE = 0,
	MEMFAULT_TRANSPORT_HTTP = 1,
	MEMFAULT_TRANSPORT_MQTT = 2,
	MEMFAULT_TRANSPORT_COAP = 3,
};

enum lte_network_state {
	LTE_NETWORK_STATE_NOT_REGISTERED = 0,
	LTE_NETWORK_STATE_HOME_NETWORK = 1,
	LTE_NETWORK_STATE_SEARCHING = 2,
	LTE_NETWORK_STATE_REGISTRATION_DENIED = 3,
	LTE_NETWORK_STATE_OUT_OF_COVERAGE = 4,
	LTE_NETWORK_STATE_ROAMING = 5,
	LTE_NETWORK_STATE_EMERGENCY = 8,
	LTE_NETWORK_STATE_UNABLE_TO_CONFIGURE = 240,
};

enum lte_startup_state {
	LTE_STARTUP_STATE_READY = 0,
	LTE_STARTUP_STATE_WAITING_FOR_ACCESS_CODE = 1,
	LTE_STARTUP_STATE_SIM_NOT_PRESENT = 2,
	LTE_STARTUP_STATE_SIM_LOCK = 3,
	LTE_STARTUP_STATE_UNRECOVERABLE_ERROR = 4,
	LTE_STARTUP_STATE_UNKNOWN = 5,
	LTE_STARTUP_STATE_INACTIVE_SIM = 6,
};

enum lte_init_error {
	LTE_INIT_ERROR_NONE = 0,
	LTE_INIT_ERROR_NO_IFACE = -1,
	LTE_INIT_ERROR_IFACE_CFG = -2,
	LTE_INIT_ERROR_DNS_CFG = -3,
	LTE_INIT_ERROR_MODEM = -4,
	LTE_INIT_ERROR_AIRPLANE = -5,
};

enum lte_sleep_state {
	LTE_SLEEP_STATE_UNINITIALIZED = 0,
	LTE_SLEEP_STATE_HIBERNATE = 1,
	LTE_SLEEP_STATE_AWAKE = 2,
	LTE_SLEEP_STATE_LITE_HIBERNATE = 3,
	LTE_SLEEP_STATE_SLEEP = 4,
};

enum lte_rat {
	LTE_RAT_CAT_M1 = 0,
	LTE_RAT_CAT_NB1 = 1,
};

enum lte_log_lvl {
	LTE_LOG_LVL_NONE = 0,
	LTE_LOG_LVL_ERROR = 1,
	LTE_LOG_LVL_WARNING = 2,
	LTE_LOG_LVL_INFO = 3,
	LTE_LOG_LVL_DEBUG = 4,
};

enum lte_fup_status {
	LTE_FUP_STATUS_IDLE = 0,
	LTE_FUP_STATUS_START = 1,
	LTE_FUP_STATUS_WIP = 2,
	LTE_FUP_STATUS_PAD = 3,
	LTE_FUP_STATUS_SEND_EOT = 4,
	LTE_FUP_STATUS_FILE_ERR = 5,
	LTE_FUP_STATUS_INSTALL = 6,
	LTE_FUP_STATUS_REBOOT_RECONFIG = 7,
	LTE_FUP_STATUS_COMPLETE = 8,
};

/* pyend */

/* pystart - enum size check */
BUILD_ASSERT(sizeof(enum lwm2m_security) == ATTR_SIZE_U8);
BUILD_ASSERT(sizeof(enum lwm2m_pwr_src) == ATTR_SIZE_U8);
BUILD_ASSERT(sizeof(enum lwm2m_batt_stat) == ATTR_SIZE_U8);
BUILD_ASSERT(sizeof(enum memfault_transport) == ATTR_SIZE_U8);
BUILD_ASSERT(sizeof(enum lte_network_state) == ATTR_SIZE_U8);
BUILD_ASSERT(sizeof(enum lte_startup_state) == ATTR_SIZE_U8);
BUILD_ASSERT(sizeof(enum lte_init_error) == ATTR_SIZE_S8);
BUILD_ASSERT(sizeof(enum lte_sleep_state) == ATTR_SIZE_U8);
BUILD_ASSERT(sizeof(enum lte_rat) == ATTR_SIZE_U8);
BUILD_ASSERT(sizeof(enum lte_log_lvl) == ATTR_SIZE_U8);
BUILD_ASSERT(sizeof(enum lte_fup_status) == ATTR_SIZE_U8);
/* pyend */

/**************************************************************************************************/
/* Global Data Definitions                                                                        */
/**************************************************************************************************/
/* pystart - attribute ids */
#define ATTR_ID_reserved0                             0
#define ATTR_ID_api_version                           1
#define ATTR_ID_firmware_version                      2
#define ATTR_ID_dump_path                             3
#define ATTR_ID_load_path                             4
#define ATTR_ID_board                                 5
#define ATTR_ID_log_on_boot                           6
#define ATTR_ID_disable_flow_control                  7
#define ATTR_ID_baud_rate                             8
#define ATTR_ID_lwm2m_server_url                      9
#define ATTR_ID_lwm2m_endpoint                        10
#define ATTR_ID_lwm2m_security                        11
#define ATTR_ID_lwm2m_psk_id                          12
#define ATTR_ID_lwm2m_psk                             13
#define ATTR_ID_lwm2m_bootstrap                       14
#define ATTR_ID_lwm2m_short_id                        15
#define ATTR_ID_lwm2m_mfg                             16
#define ATTR_ID_lwm2m_mn                              17
#define ATTR_ID_lwm2m_sn                              18
#define ATTR_ID_lwm2m_fw_ver                          19
#define ATTR_ID_lwm2m_pwr_src                         20
#define ATTR_ID_lwm2m_pwr_src_volt                    21
#define ATTR_ID_lwm2m_sw_ver                          22
#define ATTR_ID_lwm2m_hw_ver                          23
#define ATTR_ID_lwm2m_batt_stat                       24
#define ATTR_ID_lwm2m_fup_pkg_name                    25
#define ATTR_ID_lwm2m_fup_pkg_ver                     26
#define ATTR_ID_lwm2m_fup_proxy_srv                   27
#define ATTR_ID_bluetooth_address                     28
#define ATTR_ID_dm_cnx_delay                          29
#define ATTR_ID_dm_cnx_delay_min                      30
#define ATTR_ID_dm_cnx_delay_max                      31
#define ATTR_ID_factory_load_path                     32
#define ATTR_ID_device_id                             33
#define ATTR_ID_smp_auth_req                          34
#define ATTR_ID_gw_smp_auth_req                       35
#define ATTR_ID_smp_auth_timeout                      36
#define ATTR_ID_dm_cnx_retries                        37
#define ATTR_ID_dm_cnx_backoff_multi                  38
#define ATTR_ID_dm_cnx_backoff_retries                39
#define ATTR_ID_shell_password                        40
#define ATTR_ID_shell_session_timeout                 41
#define ATTR_ID_tel_trust_path                        42
#define ATTR_ID_tel_key_path                          43
#define ATTR_ID_dm_trust_path                         44
#define ATTR_ID_dm_key_path                           45
#define ATTR_ID_fs_trust_path                         46
#define ATTR_ID_fs_key_path                           47
#define ATTR_ID_p2p_trust_path                        48
#define ATTR_ID_p2p_key_path                          49
#define ATTR_ID_ipv4_addr                             50
#define ATTR_ID_network_id_filter                     51
#define ATTR_ID_memfault_transport                    52
#define ATTR_ID_mqtt_memfault_topic                   53
#define ATTR_ID_coap_mflt_proxy_route                 54
#define ATTR_ID_store_memfault_data                   55
#define ATTR_ID_lte_imei                              56
#define ATTR_ID_lte_iccid                             57
#define ATTR_ID_lte_imsi                              58
#define ATTR_ID_lte_sn                                59
#define ATTR_ID_lte_version                           60
#define ATTR_ID_lte_network_state                     61
#define ATTR_ID_lte_startup_state                     62
#define ATTR_ID_lte_init_error                        63
#define ATTR_ID_lte_apn                               64
#define ATTR_ID_lte_rsrp                              65
#define ATTR_ID_lte_sinr                              66
#define ATTR_ID_lte_bands                             67
#define ATTR_ID_lte_active_bands                      68
#define ATTR_ID_lte_operator_index                    69
#define ATTR_ID_lte_sleep_state                       70
#define ATTR_ID_lte_rat                               71
#define ATTR_ID_lte_log_lvl                           72
#define ATTR_ID_lte_fup_status                        73
#define ATTR_ID_lte_udp_tx                            74
#define ATTR_ID_lte_udp_rx                            75
#define ATTR_ID_lte_tcp_tx                            76
#define ATTR_ID_lte_tcp_rx                            77
#define ATTR_ID_lte_data_total                        78
/* pyend */

/* pystart - attribute constants */
#define ATTR_TABLE_SIZE                                 79
#define ATTR_TABLE_MAX_ID                               78
#define ATTR_TABLE_WRITABLE_COUNT                       50
#define ATTR_TABLE_CRC_OF_NAMES                         0x1a0a9144
#define ATTR_MAX_STR_LENGTH                             255
#define ATTR_MAX_STR_SIZE                               256
#define ATTR_MAX_BIN_SIZE                               16
#define ATTR_MAX_INT_SIZE                               4
#define ATTR_MAX_KEY_NAME_SIZE                          23
#define ATTR_MAX_VALUE_SIZE                             256
#define ATTR_MAX_FILE_SIZE                              3443
#define ATTR_ENABLE_FPU_CHECK                           1

/* Attribute Max String Lengths */
#define ATTR_API_VERSION_MAX_STR_SIZE                   12
#define ATTR_FIRMWARE_VERSION_MAX_STR_SIZE              65
#define ATTR_DUMP_PATH_MAX_STR_SIZE                     33
#define ATTR_LOAD_PATH_MAX_STR_SIZE                     33
#define ATTR_BOARD_MAX_STR_SIZE                         65
#define ATTR_LWM2M_SERVER_URL_MAX_STR_SIZE              256
#define ATTR_LWM2M_ENDPOINT_MAX_STR_SIZE                65
#define ATTR_LWM2M_PSK_ID_MAX_STR_SIZE                  65
#define ATTR_LWM2M_MFG_MAX_STR_SIZE                     33
#define ATTR_LWM2M_MN_MAX_STR_SIZE                      33
#define ATTR_LWM2M_SN_MAX_STR_SIZE                      65
#define ATTR_LWM2M_FW_VER_MAX_STR_SIZE                  33
#define ATTR_LWM2M_SW_VER_MAX_STR_SIZE                  33
#define ATTR_LWM2M_HW_VER_MAX_STR_SIZE                  33
#define ATTR_LWM2M_FUP_PKG_NAME_MAX_STR_SIZE            33
#define ATTR_LWM2M_FUP_PKG_VER_MAX_STR_SIZE             33
#define ATTR_LWM2M_FUP_PROXY_SRV_MAX_STR_SIZE           256
#define ATTR_BLUETOOTH_ADDRESS_MAX_STR_SIZE             13
#define ATTR_FACTORY_LOAD_PATH_MAX_STR_SIZE             33
#define ATTR_DEVICE_ID_MAX_STR_SIZE                     65
#define ATTR_SHELL_PASSWORD_MAX_STR_SIZE                33
#define ATTR_TEL_TRUST_PATH_MAX_STR_SIZE                33
#define ATTR_TEL_KEY_PATH_MAX_STR_SIZE                  33
#define ATTR_DM_TRUST_PATH_MAX_STR_SIZE                 33
#define ATTR_DM_KEY_PATH_MAX_STR_SIZE                   33
#define ATTR_FS_TRUST_PATH_MAX_STR_SIZE                 33
#define ATTR_FS_KEY_PATH_MAX_STR_SIZE                   33
#define ATTR_P2P_TRUST_PATH_MAX_STR_SIZE                33
#define ATTR_P2P_KEY_PATH_MAX_STR_SIZE                  33
#define ATTR_IPV4_ADDR_MAX_STR_SIZE                     16
#define ATTR_MQTT_MEMFAULT_TOPIC_MAX_STR_SIZE           256
#define ATTR_COAP_MFLT_PROXY_ROUTE_MAX_STR_SIZE         33
#define ATTR_LTE_IMEI_MAX_STR_SIZE                      16
#define ATTR_LTE_ICCID_MAX_STR_SIZE                     21
#define ATTR_LTE_IMSI_MAX_STR_SIZE                      16
#define ATTR_LTE_SN_MAX_STR_SIZE                        15
#define ATTR_LTE_VERSION_MAX_STR_SIZE                   30
#define ATTR_LTE_APN_MAX_STR_SIZE                       65
#define ATTR_LTE_BANDS_MAX_STR_SIZE                     21
#define ATTR_LTE_ACTIVE_BANDS_MAX_STR_SIZE              21

/* Attribute Byte Array Lengths */
#define ATTR_LWM2M_PSK_SIZE                             16
/* pyend */

/**************************************************************************************************/
/* Global Function Prototypes                                                                     */
/**************************************************************************************************/
/* pystart - prepare for read */
/* pyend */

/* pystart - get string */
const char *const attr_get_string_lwm2m_security(int value);
const char *const attr_get_string_lwm2m_pwr_src(int value);
const char *const attr_get_string_lwm2m_batt_stat(int value);
const char *const attr_get_string_memfault_transport(int value);
const char *const attr_get_string_lte_network_state(int value);
const char *const attr_get_string_lte_startup_state(int value);
const char *const attr_get_string_lte_init_error(int value);
const char *const attr_get_string_lte_sleep_state(int value);
const char *const attr_get_string_lte_rat(int value);
const char *const attr_get_string_lte_log_lvl(int value);
const char *const attr_get_string_lte_fup_status(int value);
/* pyend */

#ifdef __cplusplus
}
#endif
#endif /* __ATTR_TABLE_H__ */
