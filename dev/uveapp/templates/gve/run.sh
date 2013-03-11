#!/bin/sh
# -- set path to Questa
$@ set_simulator_ux_path

# -- If we are not executing a command which requires the
#    GUI, start in console mode (which is faster)
if [ "x$1" != "xall" -a "x$1" != "xsim" -a "x$1" != "x" ]
then
        CONSOLE="-c"
fi

if [ "x$2" = "x-c" ]
then
		CONSOLE="-c"
fi

# -- start main menu with optional argument.
${SIMULATOR}vsim $CONSOLE -do "do main.do $1"
