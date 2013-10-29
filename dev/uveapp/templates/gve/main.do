# #############################################################################
# User configuration - Edit if needed

proc env_init {} {
    global duv_lib duv_folder tb_lib tb_top testcase verbosity

    # -- Choose verbosity among:
    #    UVM_NONE, UVM_LOW, UVM_MEDIUM, UVM_HIGH, UVM_FULL
    set verbosity "UVM_HIGH"

    # -- Name of UVM DPI library
    # Not used right now
    #set dpi_lib "uvm_dpi"

    # -- Set test library
    set tb_lib work

    # -- Set VHDL library
    $@ set_duv

    # -- Set testcase to run
    $@ set_testcase

    # -- Set top file
    $@ set_top
}


# #############################################################################
# DO NOT EDIT BELOW, unless you know whatz you are doing.
# #############################################################################

# #############################################################################
# User Interactions

proc menu_init {} {
        global cmd cmd_quit cmd_vhdl cmd_sv cmd_sim cmd_all

        # Command list
        set cmd_quit	0
        set cmd_all		1
        set cmd_vhdl	2
        set cmd_sv		3
        set cmd_sim		4
}

proc set_arg { } {
        global cmd cmd_quit cmd_vhdl cmd_sv cmd_sim cmd_all

        # Read user input
        while { 1 } {
                puts "\nList of operations: \n"
                puts "  $cmd_all : Compile everything and simulate"
                puts "  $cmd_vhdl : VHDL compilation"
                puts "  $cmd_sv : SystemVerilog compilation"
                puts "  $cmd_sim : Simulation"
                puts ""
                puts "  $cmd_quit : exit\n"
                puts -nonewline " Select => "
                flush stdout

                set cmd [gets stdin]

                if { $cmd < 0 || $cmd > 4 } {
                        puts "  Invalid value\n"
                        set cmd -1
                } else {
                        break
                }
        }
}

# #############################################################################
# Compile the VHDL files

proc do_vhdl {} {
        global duv_lib duv_folder

        puts ""
        puts "** COMPILING VHDL **"
        puts ""

        do compile_vhdl.do
}

# #############################################################################
# Compile the SystemVerilog files

proc do_sv {} {
        global tb_lib tb_top

        puts ""
        puts "** COMPILING SystemVerilog **"
        puts ""

        do compile_sv.do
}

# #############################################################################
# Start the simulation

proc do_sim {} {
        global tb_top tb_lib duv_lib testcase verbosity
        puts ""
        puts "** SIMULATING **"
        puts ""

        vsim -novopt +UVM_TESTNAME=$testcase +UVM_VERBOSITY=$verbosity -L $tb_lib -L $duv_lib  $tb_top

        do wave.do
        restart -f
        run -all
}

# #############################################################################
# Recompile and directly start the simulation

proc do_all {} {
        do_vhdl
        do_sv
        do_sim
}

# #############################################################################
# Entry point
# Check whether we have or not recevied a valid argument, if not present the
#  available options and exits.
# If no option given, present a menu.

menu_init

if { $argc == 1} {

        if {[string compare $1 "all"] == 0} {
                set cmd $cmd_all
        } elseif {[string compare $1 "vhdl"] == 0} {
                set cmd $cmd_vhdl
        } elseif {[string compare $1 "sv"] == 0} {
                set cmd $cmd_sv
        } elseif {[string compare $1 "sim"] == 0} {
                set cmd $cmd_sim
        } else {
                puts "\nList of available operations: \n"
                puts "  all\t: Compile everything and simulate"
                puts "  vhdl\t: VHDL compilation"
                puts "  sv\t: SystemVerilog compilation"
                puts "  sim\t: Simulation"
                exit
        }

} else {
        set_arg
}

# Now that we known what to do... just do it!
# First setup common environment
env_init

# then, execute the given command
if {$cmd == $cmd_all} {
        do_all
} elseif {$cmd == $cmd_sim} {
        do_sim
} elseif {$cmd == $cmd_sv} {
        do_sv
} elseif {$cmd == $cmd_vhdl} {
        do_vhdl
}

exit
