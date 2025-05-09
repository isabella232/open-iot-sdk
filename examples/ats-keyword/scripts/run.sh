#!/bin/bash

#  Copyright (c) 2021 Arm Limited. All rights reserved.
#  SPDX-License-Identifier: Apache-2.0
#
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an "AS IS" BASIS,
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  See the License for the specific language governing permissions and
#  limitations under the License.

BUILD_PATH="build"

function show_usage {
    cat <<EOF
Usage: $0 [options] example

Run an example.

Options:
    -h,--help   Show this help
    -p,--path   Build path

Examples:
    blinky
    kws
EOF
}

SHORT=p:,h
LONG=path:,help
OPTS=$(getopt -n run --options $SHORT --longoptions $LONG -- "$@")

eval set -- "$OPTS"

while :
do
  case "$1" in
    -h | --help )
      show_usage
      exit 0
      ;;
    -p | --path )
      BUILD_PATH=$2
      shift 2
      ;;
    --)
      shift;
      break
      ;;
    *)
      echo "Unexpected option: $1"
      show_usage
      exit 2
      ;;
  esac
done


case "$1" in
    kws)    ;;
    blinky) ;;
    *)
        echo "Usage: $0 <blinky|kws>" >&2
        exit 1
        ;;
esac

set -x

FVP_BIN=VHT-Corstone-300.x

# for AMI compatibility check if model exists in known location
if test -f "/opt/VHT/VHT_Corstone_SSE-300_Ethos-U55"; then
    FVP_BIN=/opt/VHT/VHT_Corstone_SSE-300_Ethos-U55
fi

VSI_PY_PATH=$PWD/lib/VHT/interface/audio/python
OPTIONS="-V $VSI_PY_PATH -C mps3_board.visualisation.disable-visualisation=1 -C mps3_board.smsc_91c111.enabled=1 -C mps3_board.hostbridge.userNetworking=1 -C cpu0.semihosting-enable=1 -C mps3_board.telnetterminal0.start_telnet=0 -C mps3_board.uart0.out_file="-"  -C mps3_board.uart0.unbuffered_output=1 --stat  -C mps3_board.DISABLE_GATING=1"

$FVP_BIN $OPTIONS -a cpu0*="$BUILD_PATH/bootloader/bl2.axf" --data "$BUILD_PATH/secure_partition/tfm_s_signed.bin"@0x38000000 --data "$BUILD_PATH/$1/$1_signed.bin"@0x28060000
