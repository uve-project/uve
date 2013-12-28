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
 * Contains the agents and a bus monitor. That is the top level of a
 * verification IP.
 ****************************************************************************/

`ifndef $@FILENAME@$
`define $@FILENAME@$

`include "uvm_macros.svh"

class $@ClassName@$ extends uvm_env;

    // VIP parameters
    $@ numbers
    protected bit has_bus_monitor = 1;
    $@class_name@$_config cfg;

    // The following two bits are used to control whether checks and coverage are
    // done both in the bus monitor class and the interface.
    bit intf_checks_enable = 1;
    bit intf_coverage_enable = 1;

    // Virtual Interface variable
    protected virtual interface $@vip_interface@$ vif;

    // Components
    $@ vip_components

    // Provide implementations of virtual methods such as get_type_name and create
    `uvm_component_utils_begin($@ClassName@$)
        `uvm_field_int(has_bus_monitor, UVM_DEFAULT)
        $@ comp_utils
        `uvm_field_int(intf_checks_enable, UVM_DEFAULT)
        `uvm_field_int(intf_coverage_enable, UVM_DEFAULT)
    `uvm_component_utils_end

    // Constructor
    function new(string name, uvm_component parent);
        super.new(name,parent);
        cfg = null;
    endfunction : new

    // Build phase
    function void build_phase(uvm_phase phase);

        string inst_name;

        super.build_phase(phase);

        // Try to get a configuration object
        void'(uvm_config_db#($@class_name@$_config)::get(this, "", "config", cfg));

        if (cfg == null) begin
            cfg = $@class_name@$_config::type_id::create("cfg");
            if (!cfg.randomize())
                `uvm_fatal("RNDFAIL", "Failed to build a configuration")
        end

        // Interface
        if(!uvm_config_db#(virtual $@vip_interface@$)::get(this,"","$@vip_interface@$",vif))
            `uvm_fatal("NOVIF",{"virtual interface must be set for: ",get_full_name(),".vif"});

        $@ vip_build_components

    endfunction : build_phase


    // Connect phase
    function void connect_phase(uvm_phase phase);

        $@ vip_connect_phase

    endfunction : connect_phase

/*---------------------------------------------------------------------------
 * @TODO : Maybe implement some configuration functions for agents. In example
 * if each agent have a different attributes like address.
 *--------------------------------------------------------------------------*/


    // update_vif_enables
    protected task update_vif_enables();
        forever begin
            @(intf_checks_enable || intf_coverage_enable);
            vif.has_checks <= intf_checks_enable;
            vif.has_coverage <= intf_coverage_enable;
        end
    endtask : update_vif_enables

    // Run phase
    task run_phase(uvm_phase phase);
        fork
            update_vif_enables();
        join
    endtask : run_phase

endclass : $@ClassName@$

`endif /* $@FILENAME@$ */
