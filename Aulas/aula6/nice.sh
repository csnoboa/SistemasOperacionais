#!/bin/sh

gcc -Wall loop.c -o loop

nice -0 loop &
nice -0 loop &
nice -5 loop &
nice -5 loop &
nice -10 loop &
nice -10 loop &
nice -15 loop &
nice -15 loop &
nice -20 loop &
nice -20 loop &

sleep 60
killall loop





