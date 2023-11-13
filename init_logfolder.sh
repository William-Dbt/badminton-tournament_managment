#!/bin/sh

if [ -d "/var" ]; then
	if [ ! -d "/var/logs" ]; then
		sudo mkdir -p "/var/logs"
	fi

	if [ ! -d "/var/logs/usjm_tournament" ]; then
		sudo mkdir -p "/var/logs/usjm_tournament"
		sudo chown -R ${USER} /var/logs/usjm_tournament
	fi
fi
