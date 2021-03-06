/****************************************************************************
 *	Copyright 2012 HES-SO HEIG-VD
 *	Copyright 2011 HES-SO Valais Wallis
 *	Copyright 2007-2010 Mentor Graphics Corporation
 *	Copyright 2007-2010 Cadence Design Systems, Inc.
 *	Copyright 2010 Synopsys, Inc.
 *	All Rights Reserved Worldwide
 *
 *	Licensed under the Apache License, Version 2.0 (the "License");
 *	you may not use this file except in compliance with the License.
 *	You may obtain a copy of the License at
 *
 *		http://www.apache.org/licenses/LICENSE-2.0
 *
 *	Unless required by applicable law or agreed to in writing, software
 *	distributed under the License is distributed on an "AS IS" BASIS,
 *	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *	See the License for the specific language governing permissions and
 *	limitations under the License.
 ****************************************************************************
 * This file has been generated by UVE version $@uve_version@$
 * Date of generation: $@date@$
 *
 * Project : $@projectname@$
 * File : $@filename@$
 * Version : 1.0
 ****************************************************************************
 * Description :
 * This is the testbench. Here the VIPs are created and connected.
 ****************************************************************************/

`ifndef $@FILENAME@$
`define $@FILENAME@$

`include "uvm_macros.svh"

$@ include_scoreboard

import uvm_pkg::*;
$@ import_pkg

class $@ClassName@$ extends uvm_env;

    // Provide implementations of virtual methods such as get_type_name and create
    `uvm_component_utils($@ClassName@$)

    // VIPs declaration
    $@ decl_vips

    // Scoreboard to check the memory operation of the slave.
    $@ decl_tb_components

    // Constructor
    function new (string name, uvm_component parent=null);
        super.new(name, parent);
    endfunction : new

    // Build phase
    virtual function void build_phase(uvm_phase phase);
        super.build_phase(phase);

        // Create VIPs environment
        $@ create_vips
        
        $@ tb_build_components

    endfunction : build_phase

    // Connect phase
    function void connect_phase(uvm_phase phase);

        $@ connect_components

    endfunction : connect_phase

    function void end_of_elaboration_phase(uvm_phase phase);
/*---------------------------------------------------------------------------
 * @TODO : Maybe set some parameters, like agent address
 *--------------------------------------------------------------------------*/
    endfunction : end_of_elaboration_phase

endclass : $@ClassName@$

`endif /* $@FILENAME@$ */
