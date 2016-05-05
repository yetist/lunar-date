#!/bin/sh
nm -D -g --defined-only .libs/liblunar-date-3.0.so | cut -d ' ' -f 3 | egrep -v '^(__bss_start|_edata|_end)' | sort > actual-abi
