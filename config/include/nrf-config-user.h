/**
 * @file app_mbedtls_config.h
 *
 * Copyright (c) 2022 Laird Connectivity
 *
 * SPDX-License-Identifier: LicenseRef-LairdConnectivity-Clause
 */
#ifndef __APP_MBED_TLS_CONFIG_H__
#define __APP_MBED_TLS_CONFIG_H__

#ifdef CONFIG_LCZ_MQTT_AZURE
#include <lcz_mqtt_mbedtls_config.h>
#endif

/* 4 BT610 devices (CONFIG_LWM2M_GATEWAY_MAX_INSTANCES) * 4 keys per session + 6 overhead slots */
#define PSA_KEY_SLOT_COUNT 20
#ifdef MBEDTLS_PSA_KEY_SLOT_COUNT
#undef MBEDTLS_PSA_KEY_SLOT_COUNT
#define MBEDTLS_PSA_KEY_SLOT_COUNT PSA_KEY_SLOT_COUNT
#else
#define MBEDTLS_PSA_KEY_SLOT_COUNT PSA_KEY_SLOT_COUNT
#endif

#endif /* __APP_MBED_TLS_CONFIG_H__ */
