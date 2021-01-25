#!/bin/bash
PROFINET_HEADER_DIR="/usr/local/include"
PROFINET_LIBRARY_DIR="/usr/lib64/"
HEADER_LIST=("osal.h" "osal_log.h" "pnal.h" "pnal_sys.h" "pnet_api.h" "pnet_export.h")
LIBRARY_LIST=("libosal.so" "libprofinet.so")
status=true
for index in "${HEADER_LIST[@]}"; do
    filename="${PROFINET_HEADER_DIR}/""${index}"
    if [[ ! -e "$filename" ]]; then
        status=false
    fi
done
for index in "${LIBRARY_LIST[@]}"; do
    filename="${PROFINET_LIBRARY_DIR}""${index}"
    if [[ ! -e "$filename" ]]; then
        status=false
    fi
done
echo $status $PROFINET_HEADER_DIR $PROFINET_LIBRARY_DIR
