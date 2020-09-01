#!/bin/bash

for D in {01..05}; do
	if [ -e "submit/${D}.tar.xz" ]; then
		continue
	fi

	if [ ! -e "$D" ]; then
		continue
	fi

	cd "$D" > /dev/null 2> /dev/null

	make clean
	make package
	mv "submit.tar.xz" "../submit/${D}.tar.xz"

	cd - > /dev/null 2> /dev/null
done
