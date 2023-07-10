# Version History

## v1.1.0

- Fix LTE SINR not working (Object 4)
- Fix BT610 FOTA crashing the gateway
- Fix LwM2M non-secure connection not working
- Allow non-telemetry firmware to scan for BT610 Canvas devices. 

## v1.0.0

- Initial production release
- Add DM connection retry backoff
- Change default shell session timeout
- Fix filesystem file read for files less than the block size
- Fix duplicate LwM2M responses when talking to sensor
- Fix set LTE bands and RAT

## v0.4.4

- Factory init scripts for QA and production
- Improve BLE logging
- Add sensor shell to control allowed sensors
- Empty allow list blocks all sensors
- New sensor allow list format

## v0.4.3

- Fix crash when running factory init script in manufacturing

## v0.4.2

- Fix attr_load API to return 0 for WBx3 compatibility

## v0.4.1

- Fix crashes related to BLE
- Fix excessive error logging
- Support up to 4 BLE sensors
- Add LED activity for device management
- Fix SMP communication issues
- MQTT app: Post Memfault data over MQTT

## v0.4.0

- Enable flow control on the console UART
- Reduce boot time
- Fix object 9 resource observations
- Move to in-tree Zephyr LwM2M stack

## v0.3.1

- Fix artifact naming for BL5340
- Enable Read back protection and disable flash patch for nRF52840

## v0.3.0

- Engineering release candidate
- LED indication when in bootloader
- Ble LED blinks when there is sensor telemetry activity

## v0.2.0

Move to nRF Connect SDK (NCS) v2.1.0

## v0.1.0

Initial development build
