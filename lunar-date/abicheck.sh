#!/bin/sh
nm -D -g --defined-only .libs/liblunar-date-2.0.so | cut -d ' ' -f 3 | egrep -v '^(__bss_start|_edata|_end)' | sort > actual-abi
