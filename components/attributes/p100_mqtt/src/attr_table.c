/**
 * @file attr_table.c
 * @brief
 *
 * Copyright (c) 2022 Laird Connectivity LLC
 *
 * SPDX-License-Identifier: LicenseRef-LairdConnectivity-Clause
 */

/**************************************************************************************************/
/* Includes                                                                                       */
/**************************************************************************************************/
#include <zephyr.h>
#include <string.h>
#include <errno_str.h>
#include "attr_validator.h"
#include "attr_table.h"

#ifdef CONFIG_ATTR_CUSTOM_VALIDATOR
#include "attr_custom_validator.h"
#endif

/**************************************************************************************************/
/* Local Constant, Macro and Type Definitions                                                     */
/**************************************************************************************************/
#define DRW DEFAULT_RW_ATTRIBUTE_VALUES
#define DRO DEFAULT_RO_ATTRIBUTE_VALUES
#define y true
#define n false

/* pystart - rw attributes */
typedef struct rw_attribute {
	char dump_path[32 + 1];
	char load_path[32 + 1];
	bool log_on_boot;
	bool disable_flow_control;
	uint32_t baud_rate;
	char lwm2m_server_url[255 + 1];
	char lwm2m_endpoint[64 + 1];
	enum lwm2m_security lwm2m_security;
	char lwm2m_psk_id[64 + 1];
	uint8_t lwm2m_psk[16];
	bool lwm2m_bootstrap;
	uint16_t lwm2m_short_id;
	char lwm2m_mfg[32 + 1];
	char lwm2m_mn[32 + 1];
	char lwm2m_sn[64 + 1];
	char lwm2m_fw_ver[32 + 1];
	char lwm2m_sw_ver[32 + 1];
	char lwm2m_hw_ver[32 + 1];
	char lwm2m_fup_proxy_srv[255 + 1];
	uint16_t dm_cnx_delay;
	uint16_t dm_cnx_delay_min;
	uint16_t dm_cnx_delay_max;
	char factory_load_path[32 + 1];
	char device_id[64 + 1];
	bool smp_auth_req;
	bool gw_smp_auth_req;
	uint32_t smp_auth_timeout;
	uint8_t dm_cnx_retries;
	float dm_cnx_backoff_multi;
	uint8_t dm_cnx_backoff_retries;
	char shell_password[32 + 1];
	uint8_t shell_session_timeout;
	char tel_trust_path[32 + 1];
	char tel_key_path[32 + 1];
	char dm_trust_path[32 + 1];
	char dm_key_path[32 + 1];
	char fs_trust_path[32 + 1];
	char fs_key_path[32 + 1];
	char p2p_trust_path[32 + 1];
	char p2p_key_path[32 + 1];
	uint16_t network_id_filter;
	enum memfault_transport memfault_transport;
	char mqtt_memfault_topic[255 + 1];
	char coap_mflt_proxy_route[32 + 1];
	bool store_memfault_data;
	enum lte_log_lvl lte_log_lvl;
	char mqtt_user_name[127 + 1];
	char mqtt_password[127 + 1];
	char mqtt_endpoint[127 + 1];
	char mqtt_port[4 + 1];
	char mqtt_id[127 + 1];
	uint32_t mqtt_watchdog;
	uint8_t mqtt_publish_qos;
	enum mqtt_peer_verify mqtt_peer_verify;
	uint8_t mqtt_subscribe_qos;
	bool mqtt_connect_on_request;
	bool mqtt_transport_secure;
	bool mqtt_root_only;
	bool mqtt_clean_session;
	bool mqtt_ble_enable;
	char mqtt_ble_topic[255 + 1];
	char mqtt_ble_prefix[63 + 1];
	char mqtt_ble_delimiter[1 + 1];
	char mqtt_ble_postfix[15 + 1];
	bool mqtt_ble_quote;
} rw_attribute_t;
/* pyend */

/* pystart - rw defaults */
static const rw_attribute_t DEFAULT_RW_ATTRIBUTE_VALUES =  {
	.dump_path = "/lfs1/attr_dump.txt",
	.load_path = "/lfs1/enc/attr_load.txt",
	.log_on_boot = 0,
	.disable_flow_control = 0,
	.baud_rate = 115200,
	.lwm2m_server_url = "coap://leshan.eclipseprojects.io:5683",
	.lwm2m_endpoint = "my_device",
	.lwm2m_security = 3,
	.lwm2m_psk_id = "my_device",
	.lwm2m_psk = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f },
	.lwm2m_bootstrap = 0,
	.lwm2m_short_id = 1,
	.lwm2m_mfg = "Laird Connectivity",
	.lwm2m_mn = "model",
	.lwm2m_sn = "serial_number",
	.lwm2m_fw_ver = "0.0.0",
	.lwm2m_sw_ver = "0.0.0",
	.lwm2m_hw_ver = "0.0.0",
	.lwm2m_fup_proxy_srv = "coaps://coap-proxy.salticidae.net:5684",
	.dm_cnx_delay = 0,
	.dm_cnx_delay_min = 1,
	.dm_cnx_delay_max = 300,
	.factory_load_path = "/lfs1/enc/factory_load.txt",
	.device_id = "",
	.smp_auth_req = 0,
	.gw_smp_auth_req = 0,
	.smp_auth_timeout = 300,
	.dm_cnx_retries = 3,
	.dm_cnx_backoff_multi = 2.0,
	.dm_cnx_backoff_retries = 5,
	.shell_password = "zephyr",
	.shell_session_timeout = 5,
	.tel_trust_path = "/lfs1/tel/trust",
	.tel_key_path = "/lfs1/enc/tel/key",
	.dm_trust_path = "/lfs1/dm/trust",
	.dm_key_path = "/lfs1/enc/dm/key",
	.fs_trust_path = "/lfs1/fs/trust",
	.fs_key_path = "/lfs1/enc/fs/key",
	.p2p_trust_path = "/lfs1/p2p/trust",
	.p2p_key_path = "/lfs1/enc/p2p/key",
	.network_id_filter = 0,
	.memfault_transport = 0,
	.mqtt_memfault_topic = "",
	.coap_mflt_proxy_route = "mflt",
	.store_memfault_data = 0,
	.lte_log_lvl = 2,
	.mqtt_user_name = "",
	.mqtt_password = "",
	.mqtt_endpoint = "",
	.mqtt_port = "8883",
	.mqtt_id = "",
	.mqtt_watchdog = 0,
	.mqtt_publish_qos = 1,
	.mqtt_peer_verify = 2,
	.mqtt_subscribe_qos = 1,
	.mqtt_connect_on_request = 1,
	.mqtt_transport_secure = 1,
	.mqtt_root_only = 0,
	.mqtt_clean_session = 1,
	.mqtt_ble_enable = 0,
	.mqtt_ble_topic = "",
	.mqtt_ble_prefix = "",
	.mqtt_ble_delimiter = "",
	.mqtt_ble_postfix = "",
	.mqtt_ble_quote = 0
};
/* pyend */

/* pystart - ro attributes */
typedef struct ro_attribute {
	uint32_t reserved0;
	char api_version[11 + 1];
	char firmware_version[64 + 1];
	char board[64 + 1];
	enum lwm2m_pwr_src lwm2m_pwr_src;
	int32_t lwm2m_pwr_src_volt;
	enum lwm2m_batt_stat lwm2m_batt_stat;
	char lwm2m_fup_pkg_name[32 + 1];
	char lwm2m_fup_pkg_ver[32 + 1];
	char bluetooth_address[12 + 1];
	char ipv4_addr[15 + 1];
	char lte_imei[15 + 1];
	char lte_iccid[20 + 1];
	char lte_imsi[15 + 1];
	char lte_sn[14 + 1];
	char lte_version[29 + 1];
	enum lte_network_state lte_network_state;
	enum lte_startup_state lte_startup_state;
	enum lte_init_error lte_init_error;
	char lte_apn[64 + 1];
	int16_t lte_rsrp;
	int16_t lte_sinr;
	char lte_bands[20 + 1];
	char lte_active_bands[20 + 1];
	uint8_t lte_operator_index;
	enum lte_sleep_state lte_sleep_state;
	enum lte_rat lte_rat;
	enum lte_fup_status lte_fup_status;
	uint32_t lte_udp_tx;
	uint32_t lte_udp_rx;
	uint32_t lte_tcp_tx;
	uint32_t lte_tcp_rx;
	uint32_t lte_data_total;
} ro_attribute_t;
/* pyend */

/* pystart - ro defaults */
static const ro_attribute_t DEFAULT_RO_ATTRIBUTE_VALUES =  {
	.reserved0 = 0,
	.api_version = "1.1.1",
	.firmware_version = "0.0.0+0",
	.board = "my_board",
	.lwm2m_pwr_src = 0,
	.lwm2m_pwr_src_volt = 0,
	.lwm2m_batt_stat = 6,
	.lwm2m_fup_pkg_name = "my_firmware",
	.lwm2m_fup_pkg_ver = "0.0.0",
	.bluetooth_address = "0",
	.ipv4_addr = "",
	.lte_imei = "",
	.lte_iccid = "",
	.lte_imsi = "",
	.lte_sn = "",
	.lte_version = "",
	.lte_network_state = 0,
	.lte_startup_state = 0,
	.lte_init_error = 0,
	.lte_apn = "",
	.lte_rsrp = 0,
	.lte_sinr = 0,
	.lte_bands = "",
	.lte_active_bands = "",
	.lte_operator_index = 255,
	.lte_sleep_state = 0,
	.lte_rat = 0,
	.lte_fup_status = 0,
	.lte_udp_tx = 0,
	.lte_udp_rx = 0,
	.lte_tcp_tx = 0,
	.lte_tcp_rx = 0,
	.lte_data_total = 0,
};
/* pyend */

/**************************************************************************************************/
/* Local Data Definitions                                                                         */
/**************************************************************************************************/
static rw_attribute_t rw;
static ro_attribute_t ro;

/**************************************************************************************************/
/* Global Data Definitions                                                                        */
/**************************************************************************************************/
/**
 * @brief Table shorthand
 *
 * @ref CreateStruct (Python script)
 * Writable but non-savable values are populated using RO macro.
 *
 *.........name...value...default....size...writable..readable..get enum str
 */
#define RW_ATTRS(n) STRINGIFY(n), rw.n, DRW.n, sizeof(rw.n), NULL
#define RW_ATTRX(n) STRINGIFY(n), &rw.n, &DRW.n, sizeof(rw.n), NULL
#define RW_ATTRE(n) STRINGIFY(n), &rw.n, &DRW.n, sizeof(rw.n), attr_get_string_ ## n
#define RO_ATTRS(n) STRINGIFY(n), ro.n, DRO.n, sizeof(ro.n), NULL
#define RO_ATTRX(n) STRINGIFY(n), &ro.n, &DRO.n, sizeof(ro.n), NULL
#define RO_ATTRE(n) STRINGIFY(n), &ro.n, &DRO.n, sizeof(ro.n), attr_get_string_ ## n

/* If min == max then range isn't checked.
 *
 * index....name.....................type.flags.validator..prepare..min.max.
 */
/* pystart - table */
const struct attr_table_entry ATTR_TABLE[ATTR_TABLE_SIZE] = {
	[0  ] = { RO_ATTRX(reserved0)                           , ATTR_TYPE_U32           , 0x0   , av_uint32           , NULL                                , .min.ux = 0         , .max.ux = 0         },
	[1  ] = { RO_ATTRS(api_version)                         , ATTR_TYPE_STRING        , 0x2   , av_string           , NULL                                , .min.ux = 6         , .max.ux = 11        },
	[2  ] = { RO_ATTRS(firmware_version)                    , ATTR_TYPE_STRING        , 0x2   , av_string           , NULL                                , .min.ux = 5         , .max.ux = 64        },
	[3  ] = { RW_ATTRS(dump_path)                           , ATTR_TYPE_STRING        , 0x13  , av_string           , NULL                                , .min.ux = 1         , .max.ux = 32        },
	[4  ] = { RW_ATTRS(load_path)                           , ATTR_TYPE_STRING        , 0x13  , av_string           , NULL                                , .min.ux = 1         , .max.ux = 32        },
	[5  ] = { RO_ATTRS(board)                               , ATTR_TYPE_STRING        , 0x2   , av_string           , NULL                                , .min.ux = 1         , .max.ux = 64        },
	[6  ] = { RW_ATTRX(log_on_boot)                         , ATTR_TYPE_BOOL          , 0x1b  , av_bool             , NULL                                , .min.ux = 0         , .max.ux = 1         },
	[7  ] = { RW_ATTRX(disable_flow_control)                , ATTR_TYPE_BOOL          , 0x13  , av_bool             , NULL                                , .min.ux = 0         , .max.ux = 1         },
	[8  ] = { RW_ATTRX(baud_rate)                           , ATTR_TYPE_U32           , 0x13  , av_uint32           , NULL                                , .min.ux = 0         , .max.ux = 0         },
	[9  ] = { RW_ATTRS(lwm2m_server_url)                    , ATTR_TYPE_STRING        , 0x1b  , av_string           , NULL                                , .min.ux = 11        , .max.ux = 255       },
	[10 ] = { RW_ATTRS(lwm2m_endpoint)                      , ATTR_TYPE_STRING        , 0x1b  , av_string           , NULL                                , .min.ux = 1         , .max.ux = 64        },
	[11 ] = { RW_ATTRE(lwm2m_security)                      , ATTR_TYPE_U8            , 0x1b  , av_uint8            , NULL                                , .min.ux = 0         , .max.ux = 4         },
	[12 ] = { RW_ATTRS(lwm2m_psk_id)                        , ATTR_TYPE_STRING        , 0x1b  , av_string           , NULL                                , .min.ux = 1         , .max.ux = 64        },
	[13 ] = { RW_ATTRX(lwm2m_psk)                           , ATTR_TYPE_BYTE_ARRAY    , 0x59  , av_array            , NULL                                , .min.ux = 16        , .max.ux = 16        },
	[14 ] = { RW_ATTRX(lwm2m_bootstrap)                     , ATTR_TYPE_BOOL          , 0x1b  , av_bool             , NULL                                , .min.ux = 0         , .max.ux = 1         },
	[15 ] = { RW_ATTRX(lwm2m_short_id)                      , ATTR_TYPE_U16           , 0x1b  , av_uint16           , NULL                                , .min.ux = 1         , .max.ux = 65534     },
	[16 ] = { RW_ATTRS(lwm2m_mfg)                           , ATTR_TYPE_STRING        , 0x1b  , av_string           , NULL                                , .min.ux = 1         , .max.ux = 32        },
	[17 ] = { RW_ATTRS(lwm2m_mn)                            , ATTR_TYPE_STRING        , 0x1b  , av_string           , NULL                                , .min.ux = 1         , .max.ux = 32        },
	[18 ] = { RW_ATTRS(lwm2m_sn)                            , ATTR_TYPE_STRING        , 0x1b  , av_string           , NULL                                , .min.ux = 1         , .max.ux = 64        },
	[19 ] = { RW_ATTRS(lwm2m_fw_ver)                        , ATTR_TYPE_STRING        , 0x1b  , av_string           , NULL                                , .min.ux = 1         , .max.ux = 32        },
	[20 ] = { RO_ATTRE(lwm2m_pwr_src)                       , ATTR_TYPE_U8            , 0xb   , av_uint8            , NULL                                , .min.ux = 0         , .max.ux = 7         },
	[21 ] = { RO_ATTRX(lwm2m_pwr_src_volt)                  , ATTR_TYPE_S32           , 0xa   , av_int32            , NULL                                , .min.sx = -600000   , .max.sx = 600000    },
	[22 ] = { RW_ATTRS(lwm2m_sw_ver)                        , ATTR_TYPE_STRING        , 0x1b  , av_string           , NULL                                , .min.ux = 1         , .max.ux = 32        },
	[23 ] = { RW_ATTRS(lwm2m_hw_ver)                        , ATTR_TYPE_STRING        , 0x1b  , av_string           , NULL                                , .min.ux = 1         , .max.ux = 32        },
	[24 ] = { RO_ATTRE(lwm2m_batt_stat)                     , ATTR_TYPE_U8            , 0xa   , av_uint8            , NULL                                , .min.ux = 0         , .max.ux = 6         },
	[25 ] = { RO_ATTRS(lwm2m_fup_pkg_name)                  , ATTR_TYPE_STRING        , 0xa   , av_string           , NULL                                , .min.ux = 1         , .max.ux = 32        },
	[26 ] = { RO_ATTRS(lwm2m_fup_pkg_ver)                   , ATTR_TYPE_STRING        , 0xa   , av_string           , NULL                                , .min.ux = 0         , .max.ux = 32        },
	[27 ] = { RW_ATTRS(lwm2m_fup_proxy_srv)                 , ATTR_TYPE_STRING        , 0x1b  , av_string           , NULL                                , .min.ux = 11        , .max.ux = 255       },
	[28 ] = { RO_ATTRS(bluetooth_address)                   , ATTR_TYPE_STRING        , 0x2   , av_string           , NULL                                , .min.ux = 12        , .max.ux = 12        },
	[29 ] = { RW_ATTRX(dm_cnx_delay)                        , ATTR_TYPE_U16           , 0x1b  , av_cp16             , NULL                                , .min.ux = 0         , .max.ux = 600       },
	[30 ] = { RW_ATTRX(dm_cnx_delay_min)                    , ATTR_TYPE_U16           , 0x1b  , av_uint16           , NULL                                , .min.ux = 1         , .max.ux = 599       },
	[31 ] = { RW_ATTRX(dm_cnx_delay_max)                    , ATTR_TYPE_U16           , 0x1b  , av_uint16           , NULL                                , .min.ux = 2         , .max.ux = 600       },
	[32 ] = { RW_ATTRS(factory_load_path)                   , ATTR_TYPE_STRING        , 0x13  , av_string           , NULL                                , .min.ux = 1         , .max.ux = 32        },
	[33 ] = { RW_ATTRS(device_id)                           , ATTR_TYPE_STRING        , 0x1b  , av_string           , NULL                                , .min.ux = 0         , .max.ux = 64        },
	[34 ] = { RW_ATTRX(smp_auth_req)                        , ATTR_TYPE_BOOL          , 0x1b  , av_bool             , NULL                                , .min.ux = 0         , .max.ux = 1         },
	[35 ] = { RW_ATTRX(gw_smp_auth_req)                     , ATTR_TYPE_BOOL          , 0x1b  , av_bool             , NULL                                , .min.ux = 0         , .max.ux = 1         },
	[36 ] = { RW_ATTRX(smp_auth_timeout)                    , ATTR_TYPE_U32           , 0x1b  , av_uint32           , NULL                                , .min.ux = 0         , .max.ux = 86400     },
	[37 ] = { RW_ATTRX(dm_cnx_retries)                      , ATTR_TYPE_U8            , 0x13  , av_uint8            , NULL                                , .min.ux = 1         , .max.ux = 255       },
	[38 ] = { RW_ATTRX(dm_cnx_backoff_multi)                , ATTR_TYPE_FLOAT         , 0x13  , av_float            , NULL                                , .min.fx = 1.0       , .max.fx = 50.0      },
	[39 ] = { RW_ATTRX(dm_cnx_backoff_retries)              , ATTR_TYPE_U8            , 0x13  , av_uint8            , NULL                                , .min.ux = 1         , .max.ux = 255       },
	[40 ] = { RW_ATTRS(shell_password)                      , ATTR_TYPE_STRING        , 0x91  , av_string           , NULL                                , .min.ux = 4         , .max.ux = 32        },
	[41 ] = { RW_ATTRX(shell_session_timeout)               , ATTR_TYPE_U8            , 0x13  , av_uint8            , NULL                                , .min.ux = 0         , .max.ux = 255       },
	[42 ] = { RW_ATTRS(tel_trust_path)                      , ATTR_TYPE_STRING        , 0x13  , av_string           , NULL                                , .min.ux = 1         , .max.ux = 32        },
	[43 ] = { RW_ATTRS(tel_key_path)                        , ATTR_TYPE_STRING        , 0x13  , av_string           , NULL                                , .min.ux = 1         , .max.ux = 32        },
	[44 ] = { RW_ATTRS(dm_trust_path)                       , ATTR_TYPE_STRING        , 0x13  , av_string           , NULL                                , .min.ux = 1         , .max.ux = 32        },
	[45 ] = { RW_ATTRS(dm_key_path)                         , ATTR_TYPE_STRING        , 0x13  , av_string           , NULL                                , .min.ux = 1         , .max.ux = 32        },
	[46 ] = { RW_ATTRS(fs_trust_path)                       , ATTR_TYPE_STRING        , 0x13  , av_string           , NULL                                , .min.ux = 1         , .max.ux = 32        },
	[47 ] = { RW_ATTRS(fs_key_path)                         , ATTR_TYPE_STRING        , 0x13  , av_string           , NULL                                , .min.ux = 1         , .max.ux = 32        },
	[48 ] = { RW_ATTRS(p2p_trust_path)                      , ATTR_TYPE_STRING        , 0x13  , av_string           , NULL                                , .min.ux = 1         , .max.ux = 32        },
	[49 ] = { RW_ATTRS(p2p_key_path)                        , ATTR_TYPE_STRING        , 0x13  , av_string           , NULL                                , .min.ux = 1         , .max.ux = 32        },
	[50 ] = { RO_ATTRS(ipv4_addr)                           , ATTR_TYPE_STRING        , 0xa   , av_string           , NULL                                , .min.ux = 0         , .max.ux = 15        },
	[51 ] = { RW_ATTRX(network_id_filter)                   , ATTR_TYPE_U16           , 0x13  , av_uint16           , NULL                                , .min.ux = 0         , .max.ux = 0         },
	[52 ] = { RW_ATTRE(memfault_transport)                  , ATTR_TYPE_U8            , 0x13  , av_uint8            , NULL                                , .min.ux = 0         , .max.ux = 3         },
	[53 ] = { RW_ATTRS(mqtt_memfault_topic)                 , ATTR_TYPE_STRING        , 0x13  , av_string           , NULL                                , .min.ux = 0         , .max.ux = 255       },
	[54 ] = { RW_ATTRS(coap_mflt_proxy_route)               , ATTR_TYPE_STRING        , 0x13  , av_string           , NULL                                , .min.ux = 0         , .max.ux = 32        },
	[55 ] = { RW_ATTRX(store_memfault_data)                 , ATTR_TYPE_BOOL          , 0x13  , av_bool             , NULL                                , .min.ux = 0         , .max.ux = 0         },
	[56 ] = { RO_ATTRS(lte_imei)                            , ATTR_TYPE_STRING        , 0x2   , av_string           , NULL                                , .min.ux = 0         , .max.ux = 15        },
	[57 ] = { RO_ATTRS(lte_iccid)                           , ATTR_TYPE_STRING        , 0x2   , av_string           , NULL                                , .min.ux = 0         , .max.ux = 20        },
	[58 ] = { RO_ATTRS(lte_imsi)                            , ATTR_TYPE_STRING        , 0x2   , av_string           , NULL                                , .min.ux = 0         , .max.ux = 15        },
	[59 ] = { RO_ATTRS(lte_sn)                              , ATTR_TYPE_STRING        , 0x2   , av_string           , NULL                                , .min.ux = 0         , .max.ux = 14        },
	[60 ] = { RO_ATTRS(lte_version)                         , ATTR_TYPE_STRING        , 0xa   , av_string           , NULL                                , .min.ux = 0         , .max.ux = 29        },
	[61 ] = { RO_ATTRE(lte_network_state)                   , ATTR_TYPE_U8            , 0xa   , av_uint8            , NULL                                , .min.ux = 0         , .max.ux = 0         },
	[62 ] = { RO_ATTRE(lte_startup_state)                   , ATTR_TYPE_U8            , 0xa   , av_uint8            , NULL                                , .min.ux = 0         , .max.ux = 0         },
	[63 ] = { RO_ATTRE(lte_init_error)                      , ATTR_TYPE_S8            , 0xa   , av_int8             , NULL                                , .min.sx = 0         , .max.sx = 0         },
	[64 ] = { RO_ATTRS(lte_apn)                             , ATTR_TYPE_STRING        , 0xb   , av_string           , NULL                                , .min.ux = 0         , .max.ux = 64        },
	[65 ] = { RO_ATTRX(lte_rsrp)                            , ATTR_TYPE_S16           , 0xa   , av_int16            , NULL                                , .min.sx = 0         , .max.sx = 0         },
	[66 ] = { RO_ATTRX(lte_sinr)                            , ATTR_TYPE_S16           , 0xa   , av_int16            , NULL                                , .min.sx = 0         , .max.sx = 0         },
	[67 ] = { RO_ATTRS(lte_bands)                           , ATTR_TYPE_STRING        , 0xb   , av_string           , NULL                                , .min.ux = 1         , .max.ux = 20        },
	[68 ] = { RO_ATTRS(lte_active_bands)                    , ATTR_TYPE_STRING        , 0xa   , av_string           , NULL                                , .min.ux = 0         , .max.ux = 20        },
	[69 ] = { RO_ATTRX(lte_operator_index)                  , ATTR_TYPE_U8            , 0xa   , av_uint8            , NULL                                , .min.ux = 0         , .max.ux = 0         },
	[70 ] = { RO_ATTRE(lte_sleep_state)                     , ATTR_TYPE_U8            , 0xa   , av_uint8            , NULL                                , .min.ux = 0         , .max.ux = 0         },
	[71 ] = { RO_ATTRE(lte_rat)                             , ATTR_TYPE_U8            , 0xb   , av_uint8            , NULL                                , .min.ux = 0         , .max.ux = 0         },
	[72 ] = { RW_ATTRE(lte_log_lvl)                         , ATTR_TYPE_U8            , 0x1b  , av_uint8            , NULL                                , .min.ux = 0         , .max.ux = 0         },
	[73 ] = { RO_ATTRE(lte_fup_status)                      , ATTR_TYPE_U8            , 0xa   , av_uint8            , NULL                                , .min.ux = 0         , .max.ux = 0         },
	[74 ] = { RO_ATTRX(lte_udp_tx)                          , ATTR_TYPE_U32           , 0xa   , av_uint32           , NULL                                , .min.ux = 0         , .max.ux = 0         },
	[75 ] = { RO_ATTRX(lte_udp_rx)                          , ATTR_TYPE_U32           , 0xa   , av_uint32           , NULL                                , .min.ux = 0         , .max.ux = 0         },
	[76 ] = { RO_ATTRX(lte_tcp_tx)                          , ATTR_TYPE_U32           , 0xa   , av_uint32           , NULL                                , .min.ux = 0         , .max.ux = 0         },
	[77 ] = { RO_ATTRX(lte_tcp_rx)                          , ATTR_TYPE_U32           , 0xa   , av_uint32           , NULL                                , .min.ux = 0         , .max.ux = 0         },
	[78 ] = { RO_ATTRX(lte_data_total)                      , ATTR_TYPE_U32           , 0xa   , av_uint32           , NULL                                , .min.ux = 0         , .max.ux = 0         },
	[79 ] = { RW_ATTRS(mqtt_user_name)                      , ATTR_TYPE_STRING        , 0x11  , av_string           , NULL                                , .min.ux = 0         , .max.ux = 127       },
	[80 ] = { RW_ATTRS(mqtt_password)                       , ATTR_TYPE_STRING        , 0x11  , av_string           , NULL                                , .min.ux = 0         , .max.ux = 127       },
	[81 ] = { RW_ATTRS(mqtt_endpoint)                       , ATTR_TYPE_STRING        , 0x11  , av_string           , NULL                                , .min.ux = 0         , .max.ux = 127       },
	[82 ] = { RW_ATTRS(mqtt_port)                           , ATTR_TYPE_STRING        , 0x13  , av_string           , NULL                                , .min.ux = 3         , .max.ux = 4         },
	[83 ] = { RW_ATTRS(mqtt_id)                             , ATTR_TYPE_STRING        , 0x13  , av_string           , NULL                                , .min.ux = 0         , .max.ux = 127       },
	[84 ] = { RW_ATTRX(mqtt_watchdog)                       , ATTR_TYPE_U32           , 0x13  , av_uint32           , NULL                                , .min.ux = 0         , .max.ux = 0         },
	[85 ] = { RW_ATTRX(mqtt_publish_qos)                    , ATTR_TYPE_U8            , 0x13  , av_uint8            , NULL                                , .min.ux = 0         , .max.ux = 0         },
	[86 ] = { RW_ATTRE(mqtt_peer_verify)                    , ATTR_TYPE_U8            , 0x13  , av_uint8            , NULL                                , .min.ux = 0         , .max.ux = 0         },
	[87 ] = { RW_ATTRX(mqtt_subscribe_qos)                  , ATTR_TYPE_U8            , 0x13  , av_uint8            , NULL                                , .min.ux = 0         , .max.ux = 0         },
	[88 ] = { RW_ATTRX(mqtt_connect_on_request)             , ATTR_TYPE_BOOL          , 0x13  , av_bool             , NULL                                , .min.ux = 0         , .max.ux = 0         },
	[89 ] = { RW_ATTRX(mqtt_transport_secure)               , ATTR_TYPE_BOOL          , 0x13  , av_bool             , NULL                                , .min.ux = 0         , .max.ux = 0         },
	[90 ] = { RW_ATTRX(mqtt_root_only)                      , ATTR_TYPE_BOOL          , 0x13  , av_bool             , NULL                                , .min.ux = 0         , .max.ux = 0         },
	[91 ] = { RW_ATTRX(mqtt_clean_session)                  , ATTR_TYPE_BOOL          , 0x13  , av_bool             , NULL                                , .min.ux = 0         , .max.ux = 0         },
	[92 ] = { RW_ATTRX(mqtt_ble_enable)                     , ATTR_TYPE_BOOL          , 0x13  , av_bool             , NULL                                , .min.ux = 0         , .max.ux = 0         },
	[93 ] = { RW_ATTRS(mqtt_ble_topic)                      , ATTR_TYPE_STRING        , 0x13  , av_string           , NULL                                , .min.ux = 0         , .max.ux = 255       },
	[94 ] = { RW_ATTRS(mqtt_ble_prefix)                     , ATTR_TYPE_STRING        , 0x13  , av_string           , NULL                                , .min.ux = 0         , .max.ux = 63        },
	[95 ] = { RW_ATTRS(mqtt_ble_delimiter)                  , ATTR_TYPE_STRING        , 0x13  , av_string           , NULL                                , .min.ux = 0         , .max.ux = 1         },
	[96 ] = { RW_ATTRS(mqtt_ble_postfix)                    , ATTR_TYPE_STRING        , 0x13  , av_string           , NULL                                , .min.ux = 0         , .max.ux = 15        },
	[97 ] = { RW_ATTRX(mqtt_ble_quote)                      , ATTR_TYPE_BOOL          , 0x13  , av_bool             , NULL                                , .min.ux = 0         , .max.ux = 0         }
};
/* pyend */

/**************************************************************************************************/
/* Local Function Definitions                                                                     */
/**************************************************************************************************/
/* pystart - get string */
const char *const attr_get_string_lwm2m_security(int value)
{
	switch (value) {
		case 0:           return "PSK";
		case 1:           return "RPK";
		case 2:           return "Cert";
		case 3:           return "No Sec";
		case 4:           return "Cert Est";
		default:          return "?";
	}
}

const char *const attr_get_string_lwm2m_pwr_src(int value)
{
	switch (value) {
		case 0:           return "DC";
		case 1:           return "Int Batt";
		case 2:           return "Ext Batt";
		case 3:           return "Fuel Cell";
		case 4:           return "PoE";
		case 5:           return "USB";
		case 6:           return "AC";
		case 7:           return "Solar";
		default:          return "?";
	}
}

const char *const attr_get_string_lwm2m_batt_stat(int value)
{
	switch (value) {
		case 0:           return "Norm";
		case 1:           return "Charging";
		case 2:           return "Charge Comp";
		case 3:           return "Damaged";
		case 4:           return "Low";
		case 5:           return "Not Inst";
		case 6:           return "Unknown";
		default:          return "?";
	}
}

const char *const attr_get_string_memfault_transport(int value)
{
	switch (value) {
		case 0:           return "None";
		case 1:           return "Http";
		case 2:           return "Mqtt";
		case 3:           return "Coap";
		default:          return "?";
	}
}

const char *const attr_get_string_lte_network_state(int value)
{
	switch (value) {
		case 0:           return "Not Registered";
		case 1:           return "Home Network";
		case 2:           return "Searching";
		case 3:           return "Registration Denied";
		case 4:           return "Out Of Coverage";
		case 5:           return "Roaming";
		case 8:           return "Emergency";
		case 240:         return "Unable To Configure";
		default:          return "?";
	}
}

const char *const attr_get_string_lte_startup_state(int value)
{
	switch (value) {
		case 0:           return "Ready";
		case 1:           return "Waiting For ACcess Code";
		case 2:           return "Sim Not Present";
		case 3:           return "Sim Lock";
		case 4:           return "Unrecoverable Error";
		case 5:           return "Unknown";
		case 6:           return "Inactive Sim";
		default:          return "?";
	}
}

const char *const attr_get_string_lte_init_error(int value)
{
	switch (value) {
		case 0:           return "None";
		case -1:          return "No Iface";
		case -2:          return "Iface Cfg";
		case -3:          return "Dns Cfg";
		case -4:          return "Modem";
		case -5:          return "Airplane";
		default:          return "?";
	}
}

const char *const attr_get_string_lte_sleep_state(int value)
{
	switch (value) {
		case 0:           return "Uninitialized";
		case 1:           return "Hibernate";
		case 2:           return "Awake";
		case 3:           return "Lite Hibernate";
		case 4:           return "Sleep";
		default:          return "?";
	}
}

const char *const attr_get_string_lte_rat(int value)
{
	switch (value) {
		case 0:           return "Cat M1";
		case 1:           return "Cat NB1";
		default:          return "?";
	}
}

const char *const attr_get_string_lte_log_lvl(int value)
{
	switch (value) {
		case 0:           return "None";
		case 1:           return "Error";
		case 2:           return "Warning";
		case 3:           return "Info";
		case 4:           return "Debug";
		default:          return "?";
	}
}

const char *const attr_get_string_lte_fup_status(int value)
{
	switch (value) {
		case 0:           return "Idle";
		case 1:           return "Start";
		case 2:           return "Wip";
		case 3:           return "Pad";
		case 4:           return "Send Eot";
		case 5:           return "File Err";
		case 6:           return "Install";
		case 7:           return "Reboot Reconfig";
		case 8:           return "Complete";
		default:          return "?";
	}
}

const char *const attr_get_string_mqtt_peer_verify(int value)
{
	switch (value) {
		case 0:           return "None";
		case 1:           return "Optional";
		case 2:           return "Required";
		case 3:           return "Unset";
		default:          return "?";
	}
}

/* pyend */

/**************************************************************************************************/
/* Global Function Definitions                                                                    */
/**************************************************************************************************/
void attr_table_initialize(void)
{
	memcpy(&rw, &DRW, sizeof(rw_attribute_t));
	memcpy(&ro, &DRO, sizeof(ro_attribute_t));
}

void attr_table_factory_reset(void)
{
	size_t i = 0;
	for (i = 0; i < ATTR_TABLE_SIZE; i++) {
		memcpy(ATTR_TABLE[i].pData, ATTR_TABLE[i].pDefault, ATTR_TABLE[i].size);
	}
}

const struct attr_table_entry *const attr_map(attr_id_t id)
{
	if (id == 0 || id > ATTR_TABLE_MAX_ID) {
		return NULL;
	} else {
		return &ATTR_TABLE[id];
	}
}

attr_index_t attr_table_index(const struct attr_table_entry *const entry)
{
	__ASSERT(PART_OF_ARRAY(ATTR_TABLE, entry), "Invalid entry");
	return (entry - &ATTR_TABLE[0]);
}
