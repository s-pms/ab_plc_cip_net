#!/bin/sh
set -eu
cd "$(dirname "$0")/.."
test_dir=$(mktemp -d)
trap 'rm -rf "$test_dir"' EXIT HUP INT TERM
${CC:-gcc} -g -O1 -fno-omit-frame-pointer -fsanitize=address,undefined -no-pie \
    -Iab_plc_cip_net tests/test_read_reply.c ab_plc_cip_net/*.c -pthread -o "$test_dir/test_reply"
UBSAN_OPTIONS=halt_on_error=1 "$test_dir/test_reply" "$@"
