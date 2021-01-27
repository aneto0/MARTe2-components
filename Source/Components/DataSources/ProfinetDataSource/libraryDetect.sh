#!/bin/bash
HEADER_DIR="/usr/local/include/"
LIBRARY_DIR="/usr/local/lib/"
HEADER_LIST=("osal.h" "osal_log.h" "pnal.h" "pnal_sys.h" "pnet_api.h" "pnet_export.h")
LIBRARY_LIST=("libosal.so" "libprofinet.so")
status=true
for index in "${HEADER_LIST[@]}"; do
    filename="${HEADER_DIR}""${index}"
    if [[ ! -e "$filename" ]]; then
        status=false
    fi
done
for index in "${LIBRARY_LIST[@]}"; do
    filename="${LIBRARY_DIR}""${index}"
    if [[ ! -e "$filename" ]]; then
        status=false
    fi
done
echo $status $HEADER_DIR $LIBRARY_DIR
