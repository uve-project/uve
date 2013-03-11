# NOTE: duv_lib global variable is defined in main.do

# Create library when this is needed
if {[file exists $duv_lib] == 0} {
    vlib $duv_lib
}

# Compile VHD files of the DUV
$@ vcom
