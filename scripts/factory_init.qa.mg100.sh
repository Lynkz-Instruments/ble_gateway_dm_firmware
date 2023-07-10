#!/zsh

# Setup DM connection basics
attr set lwm2m_server_url coaps://lwm2m-bootstrap.stage.edgeiq.io:5684
attr set lwm2m_security 0
attr set lwm2m_bootstrap 1

# Firmware update server
attr set lwm2m_fup_proxy_srv coaps://coap-proxy.salticidae.net:5684

# Set the device to generate a random connection backoff on next boot
attr set dm_cnx_delay 0

# Reduce logging
attr quiet lwm2m_pwr_src_volt 1

# Generate private keys
pki keygen dm
pki keygen tel
pki keygen p2p
pki keygen fs

# LTE modem setup
# Clear the APN to let SIM cards do the work of attaching to the network
attr set lte_apn ""
# Set a global band config to cover all regions and reduce SKUs
attr set lte_bands 0000000000000809181F

# Reduce logging
attr quiet lte_sinr 1
attr quiet lte_rsrp 1
