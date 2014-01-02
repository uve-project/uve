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
 * The scoreboard verifies the proper operation of your design at a functional
 * level.
 ****************************************************************************/

`ifndef $@FILENAME@$
`define $@FILENAME@$

`include "uvm_macros.svh"

import uvm_pkg::*;
$@ import_pkg

$@ decl_ports_types

class $@ClassName@$ extends uvm_scoreboard;

    // Analysis port, get transfer from monitor
    $@ decl_ports

    $@ decl_config

    // Scoreboard enabled/disabled
    protected bit disable_scoreboard = 0;

    // Scoreboard attributs
/*---------------------------------------------------------------------------
 * @TODO : Define scoreboard attributes
 *---- Example -------------------------------------------------------------*/
    protected int num_writes = 0;
    protected int num_init_reads = 0;
    protected int num_uninit_reads = 0;
/*--------------------------------------------------------------------------*/

    // Indicate if an error occur
    int sbd_error = 0;

    // Provide implementations of virtual methods such as get_type_name and create
    `uvm_component_utils_begin($@ClassName@$)
        `uvm_field_int(disable_scoreboard, UVM_DEFAULT)
/*---------------------------------------------------------------------------
 * @TODO : Use macros for scoreboard attributes
 *---- Example -------------------------------------------------------------*/
        `uvm_field_int(num_writes, UVM_DEFAULT|UVM_DEC)
        `uvm_field_int(num_init_reads, UVM_DEFAULT|UVM_DEC)
        `uvm_field_int(num_uninit_reads, UVM_DEFAULT|UVM_DEC)
/*--------------------------------------------------------------------------*/
    `uvm_component_utils_end

    // Constructor
    function new (string name = "Scoreboard", uvm_component parent);
        super.new(name, parent);
    endfunction : new

    // Build phase
    function void build_phase(uvm_phase phase);

        $@ build_phase

    endfunction

    $@ connect_phase

    $@ write_methods

    // Report_phase
    virtual function void report_phase(uvm_phase phase);
        if(!disable_scoreboard) begin
            // Display scoreboard information
            `uvm_info(get_type_name(),
            $psprintf("Reporting scoreboard information...\n%s", this.sprint()), UVM_LOW)
        end
    endfunction : report_phase

endclass : $@ClassName@$

`endif /* $@FILENAME@$ */
