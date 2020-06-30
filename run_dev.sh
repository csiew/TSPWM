#!/bin/sh
make
cp out/tspwm /usr/bin/tspwm
DISPLAY=:0
Xephyr -br -ac -noreset -screen 1024x780 :1 &
