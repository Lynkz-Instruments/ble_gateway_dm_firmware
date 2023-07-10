# SPDX-License-Identifier: LicenseRef-LairdConnectivity-Clause

cmake_minimum_required(VERSION 3.20.0)

set(lcz_module_dir ${CMAKE_SOURCE_DIR}/../modules/lib/laird_connect)

# Set model number based on board
if(BOARD STREQUAL mg100)
    set(MODEL_NAME mg100)
elseif(BOARD STREQUAL pinnacle_100_dvk)
    set(MODEL_NAME p100)
elseif(BOARD STREQUAL bl5340_dvk_cpuapp)
    set(MODEL_NAME bl5340)
elseif(BOARD STREQUAL nrf7002dk_nrf5340_cpuapp)
    set(MODEL_NAME bl5340)
endif()

set(b0_DTC_OVERLAY_FILE
    ${CMAKE_SOURCE_DIR}/boards/${BOARD}.overlay
)

set(mcuboot_DTC_OVERLAY_FILE
    ${CMAKE_SOURCE_DIR}/boards/${BOARD}.overlay
)

set(b0_OVERLAY_CONFIG
    ${CMAKE_SOURCE_DIR}/config/b0.${BOARD}.conf
)

set(mcuboot_CONF_FILE
    ${CMAKE_SOURCE_DIR}/config/mcuboot.${BOARD}.conf
)

set(PM_STATIC_YML_FILE
    ${lcz_module_dir}/zephyr_lib/mcuboot_config/pm_static.sb.${BOARD}.yml
)

if((BOARD STREQUAL bl5340_dvk_cpuapp) OR(BOARD STREQUAL nrf7002dk_nrf5340_cpuapp))
    set(hci_rpmsg_OVERLAY_CONFIG ${CMAKE_SOURCE_DIR}/config/hci_rpmsg.bl5340_dvk_cpunet.conf)
endif()

if(BOARD STREQUAL nrf7002dk_nrf5340_cpuapp)
    set(WIFI_NRF_DRV_BASE ${WEST_TOPDIR}/nrf/drivers/wifi/nrf700x)
    if(CONFIG_WPA_SUPP)
        set(WPA_SUPP_BASE ${WEST_TOPDIR}/modules/lib/hostap)
    endif()
endif()

# Generate build ID based on UTC timestamp
string(TIMESTAMP build_time "%s" UTC)
set(APP_VERSION_BUILD ${build_time})
set(gen_dir ${CMAKE_BINARY_DIR}/zephyr/include/generated/app)
set(ver_file ${gen_dir}/app_version.h)
configure_file(${CMAKE_SOURCE_DIR}/src/version/app_version.h.in ${ver_file})

# Get application version
file(READ ${ver_file} ver)
string(REGEX MATCH "APP_VERSION_MAJOR ([0-9]+)" _ ${ver})
set(ver_major ${CMAKE_MATCH_1})
string(REGEX MATCH "APP_VERSION_MINOR ([0-9]+)" _ ${ver})
set(ver_minor ${CMAKE_MATCH_1})
string(REGEX MATCH "APP_VERSION_PATCH ([0-9]+)" _ ${ver})
set(ver_patch ${CMAKE_MATCH_1})
set(app_version ${ver_major}.${ver_minor}.${ver_patch}+${build_time})
message("app version: ${app_version}")

# Set application version for mcuboot
set(mcuboot_ver_conf ${CMAKE_BINARY_DIR}/generated_mcuboot_image_version.conf)
set(MCUBOOT_IMAGE_VERSION ${app_version})

# This configuration file will get automatically merged when building.
# No need to append to OVERLAY_CONFIG
configure_file(${CMAKE_SOURCE_DIR}/src/version/mcuboot_image_version.conf.in ${mcuboot_ver_conf})

# Enable firmware update
list(APPEND OVERLAY_CONFIG ${CMAKE_SOURCE_DIR}/config/firmware_update.conf)

# Enable file system
list(APPEND OVERLAY_CONFIG ${CMAKE_SOURCE_DIR}/config/filesystem.conf)

# MBED TLS for secure IoT
list(APPEND OVERLAY_CONFIG ${CMAKE_SOURCE_DIR}/config/mbedtls.conf)

# Enable hardware unique key generation
list(APPEND OVERLAY_CONFIG ${CMAKE_SOURCE_DIR}/config/hardware_key.conf)

# Enable networking support
list(APPEND OVERLAY_CONFIG ${CMAKE_SOURCE_DIR}/config/networking.conf)

# Enable BLE
list(APPEND OVERLAY_CONFIG ${CMAKE_SOURCE_DIR}/config/ble.conf)

# Enable device management
list(APPEND OVERLAY_CONFIG ${CMAKE_SOURCE_DIR}/config/device_management.conf)

# Enable memfault
list(APPEND OVERLAY_CONFIG ${CMAKE_SOURCE_DIR}/config/memfault.conf)
set(MEMFAULT_SW_VER ${ver_major}.${ver_minor}.${ver_patch})
set(MEMFAULT_HW_VER ${BOARD})
set(memfault_info_conf ${CMAKE_BINARY_DIR}/generated_memfault_info.conf)

# This configuration file will get automatically merged when building.
# No need to append to OVERLAY_CONFIG
configure_file(${CMAKE_SOURCE_DIR}/src/version/memfault_info.conf.in ${memfault_info_conf})

# Enable Laird Connectivity BLE sensor scanning
list(APPEND OVERLAY_CONFIG ${CMAKE_SOURCE_DIR}/config/ble_sensors.conf)

# WiFi configuration
if(BOARD STREQUAL nrf7002dk_nrf5340_cpuapp)
    list(APPEND OVERLAY_CONFIG ${CMAKE_SOURCE_DIR}/config/wifi.conf)
endif()

# App configuration
if(APP_TYPE STREQUAL lwm2m)
    list(APPEND OVERLAY_CONFIG ${CMAKE_SOURCE_DIR}/config/lwm2m_telemetry.conf)
    message("Device Management with LwM2M Telemetry")
elseif(APP_TYPE STREQUAL mqtt)
    list(APPEND OVERLAY_CONFIG ${CMAKE_SOURCE_DIR}/config/mqtt_telemetry.conf)
    message("Device Management with MQTT Telemetry")
elseif(APP_TYPE STREQUAL none)
    message("Device Management without Telemetry")
else()
    message(FATAL_ERROR "Unknown app type ${APP_TYPE}")
endif()
set(API_KEYS ${MODEL_NAME}+${APP_TYPE})
set(API_NAME ${MODEL_NAME}_${APP_TYPE})
set(ATTR_CUSTOM_PATH_BASE ${CMAKE_SOURCE_DIR}/components/attributes/${API_NAME})

# Add framework ID and msgcode files prior to Zephyr package init
set(FWK_APP_ID_FILE_LIST ${CMAKE_SOURCE_DIR}/src/framework_config/fwk_ids.h)
set(FWK_APP_MSG_FILE_LIST ${CMAKE_SOURCE_DIR}/src/framework_config/fwk_msg_codes.h)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(ble_gateway_dm_firmware)

# Build the API
execute_process(
    COMMAND
    ${PYTHON_EXECUTABLE}
    ${CMAKE_SOURCE_DIR}/../modules/lib/laird_connect/attributes/generator/api.py
    ${CMAKE_SOURCE_DIR}
    ${API_KEYS}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    RESULT_VARIABLE api_result
)

if(NOT "${api_result}" STREQUAL 0)
    message(FATAL_ERROR "Failed to build API")
endif()

# Include Memfault configs
if(CONFIG_LCZ_MEMFAULT)
    if((BOARD STREQUAL mg100) OR(BOARD STREQUAL pinnacle_100_dvk))
        zephyr_include_directories(${CMAKE_SOURCE_DIR}/memfault/mg100)
    elseif((BOARD STREQUAL bl5340_dvk_cpuapp) OR(BOARD STREQUAL nrf7002dk_nrf5340_cpuapp))
        zephyr_include_directories(${CMAKE_SOURCE_DIR}/memfault/bl5340)
    endif()
endif()

zephyr_include_directories(${CMAKE_SOURCE_DIR}/config/include)

target_include_directories(app PRIVATE ${gen_dir})

target_sources(app PRIVATE src/main.c)
