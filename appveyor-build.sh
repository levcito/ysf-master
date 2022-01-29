#!/bin/bash

if [[ -n $CONFIGURATION ]]; then
	if [[ $CONFIGURATION == "Debug" ]]; then
		make static-debug && mv YSF_static_d.so YSF_d.static && make debug
		exit
	elif [[ $CONFIGURATION == "Release" ]]; then
		make static && mv YSF_static.so YSF.static && make YSF
		exit
	else
		echo "\$CONFIGURATION is set to $CONFIGURATION. Aborting compilation..."
		exit
	fi
else
	echo -e "\$CONFIGURATION is not set! Aborting compilation..."
	exit
fi