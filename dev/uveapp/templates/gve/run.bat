@echo off
title run simulation

REM -- set path to Questa
REM    this line will be replaced with something like this: 
REM    set simulator_path=C:\eda\mentor\questasim64_10.1a\win64\
$@ set_simulator_win_path

REM -- If we are not executing a command which requires the
REM    GUI, start in console mode (which is faster)
set CONSOLE=
if "%1"=="sim" goto run_sim
if "%1"=="all" goto run_sim
if "%1"=="" goto run_sim
set CONSOLE=-c

:run_sim
REM -- start main menu with optional argument.
%simulator%vsim %CONSOLE% -do "do main.do %1"
