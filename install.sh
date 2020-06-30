#!/bin/sh
# Copy TSPWM
cp out/tspwm /usr/bin/tspwm
# Copy session entries
cp assets/tspwm-session /usr/bin/tspwm-session
cp assets/tspwm-session.desktop /usr/share/xsessions/tspwm-session.desktop
# Set permissions
chmod a+x /usr/bin/tspwm
chmod a+x /usr/bin/tspwm-session
