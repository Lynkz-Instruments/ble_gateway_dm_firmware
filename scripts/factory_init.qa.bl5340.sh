#!/zsh

# Setup DM connection basics
attr set lwm2m_server_url coaps://lwm2m-bootstrap.stage.edgeiq.io:5684
attr set lwm2m_security 0
attr set lwm2m_bootstrap 1

# Firmware update server
attr set lwm2m_fup_proxy_srv coaps://coap-proxy.salticidae.net:5684

# Set the device to generate a random connection backoff on next boot
attr set dm_cnx_delay 0

# Generate private keys
pki keygen dm
pki keygen tel
pki keygen p2p
pki keygen fs
