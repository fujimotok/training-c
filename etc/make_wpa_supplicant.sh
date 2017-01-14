#!/bin/ash

if [ $# -ne 2 ];then
  echo "usege $0 SSID PASS"
  exit 0
fi

wpa_passphrase $1 $2 \
| sed -e "3,3d" \
| sed -e "3a \  proto=RSN" \
| sed -e "4a \  key_mgmt=WPA-PSK" \
| sed -e "5a \  pairwise=CCMP" \
| sed -e "6a \  group=CCMP" \
| sed -e "7a \  scan_ssid=1" \
| tee /etc/config/wpa_supplicant.conf
