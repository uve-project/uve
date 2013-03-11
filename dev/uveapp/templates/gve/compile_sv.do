# NOTE: tb_lib global variable is defined in main.do

# Create library when this is needed
if {[file exists $tb_lib] == 0} {
    vlib $tb_lib
}

# Compile SV files
$@ vlog
