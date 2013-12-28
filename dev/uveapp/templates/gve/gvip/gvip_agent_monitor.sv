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
 * A monitor is a passive entity that samples DUT signals but does not drive
 * them.
 ****************************************************************************/

`ifndef $@FILENAME@$
`define $@FILENAME@$

`include "uvm_macros.svh"

class $@ClassName@$ extends uvm_monitor;

    // Enable or diseable monitor's check and coverage functionalities
    bit checks_enable = 1;
    bit coverage_enable = 1;
    bit get_from_driver = 0;

    // Virtual Interface
    protected virtual $@vip_interface@$ vif;

    // Transaction
    protected $@vip_transfer@$ trans_collected;

    // Analysis_port to scoreboard
    uvm_analysis_port #($@vip_transfer@$) item_collected_port;

    // Port to sequencer
    uvm_blocking_peek_imp #($@vip_transfer@$, $@ClassName@$) seq_item_imp;

    // This event unblock the peek function of the port to sequencer
    // It is send with : "-> send_to_sequencer"
    protected event send_to_sequencer;

    // Port implementation of the driver item export
    uvm_blocking_put_imp #($@vip_transfer@$, $@ClassName@$) mon_item_imp;

    uvm_analysis_imp #($@vip_transfer@$, $@ClassName@$) coll_mon_port;



    // This event unblock the collect_transactions function
    // It is send with : "-> receive_from_driver"
    protected event receive_from_driver;


    // Covergroups
/*---------------------------------------------------------------------------
 * @TODO : Implement Covergroup(s)
 *--------------------------------------------------------------------------*/

    // Provide implementations of virtual methods such as get_type_name and create
    `uvm_component_utils_begin($@ClassName@$)
        `uvm_field_int(checks_enable, UVM_DEFAULT)
        `uvm_field_int(coverage_enable, UVM_DEFAULT)
    `uvm_component_utils_end

    /**********************************************************
     * Object creation
     **********************************************************/

    // Constructor
    function new(string name = "agent monitor", uvm_component parent);
        super.new(name, parent);

        // Create covergroups
/*---------------------------------------------------------------------------
 * @TODO : Instanciate covergroup(s)
 *--------------------------------------------------------------------------*/

        // Creates captured transfer
        trans_collected = new();

        // Creating Analysis port
        item_collected_port = new("item_collected_port", this);
                
        seq_item_imp = new("set_item_imp", this);

        coll_mon_port = new("coll_mon_port", this);

    endfunction : new

    // Build
    virtual function void build_phase(uvm_phase phase);

        // Interface
        if(!uvm_config_db#(virtual $@vip_interface@$)::get(this,"","$@vip_interface@$",vif))
            `uvm_fatal("NOVIF",{"virtual interface must be set for: ",get_full_name(),".vif"});

        // Create implementation port (implementation of the driver item export)
        // This not so nifty, the monitor should extract transaction from
        //  monitoring the interface.
        mon_item_imp = new("mon_item_imp", this);

    endfunction : build_phase

    /**********************************************************
     * Behavior
     **********************************************************/

    // Run
    virtual task run_phase(uvm_phase phase);
        if (get_from_driver == 1)
            return;
            fork
                collect_transactions();
            join
    endtask : run_phase

    /************************************************************************
     * Collection
     ************************************************************************/

    // Collect the current transaction and perform check and coverage
    virtual protected task collect_transactions();
        forever begin

            // Collect the data from the bus into trans_collected
/*---------------------------------------------------------------------------
 * @TODO : Implement data collection
 *--------------------------------------------------------------------------*/
            if (get_from_driver == 1)
                @receive_from_driver;
            else begin
                $@ monitor_collect
            end


            // Display transfer
            `uvm_info(get_type_name(),
                      $psprintf("Transfer collected :\n%s", trans_collected.sprint()),
                      UVM_HIGH)

            // -> send_to_sequencer;

            // Perform transfer checker
            if (checks_enable)
                perform_transfer_checks();

            // Perform transfer coverage
            if (coverage_enable)
                perform_transfer_coverage();

            // Transmit transfer via analysis port
                item_collected_port.write(trans_collected);
        end
    endtask : collect_transactions

    /************************************************************************
     * Checks
     ************************************************************************/

    virtual protected function void perform_transfer_checks();

/*---------------------------------------------------------------------------
 * @TODO : Implement checks
 *--------------------------------------------------------------------------*/

    endfunction : perform_transfer_checks

    /*************************************************************************
     * Coverage
     *************************************************************************/

    virtual protected function void perform_transfer_coverage();

/*----------------------------------------------------------------------------
 * @TODO : Implement coverage
 *---------------------------------------------------------------------------*/

    endfunction : perform_transfer_coverage

    // Implementation of peek function of the port to sequencer
    task peek(output $@vip_transfer@$ trans);
        @send_to_sequencer;
        trans = trans_collected;
    endtask : peek

    // Implementation of put function of the port from driver
    task put($@vip_transfer@$ trans);
        trans_collected.copy(trans);
        -> receive_from_driver;
    endtask: put


    // Transaction interface to the collector
    virtual function void write($@vip_transfer@$ trans);
        trans_collected = trans;


        // Display transfer
        `uvm_info(get_type_name(),
                  $psprintf("Transfer collected :\n%s", trans_collected.sprint()),
                  UVM_HIGH)

        if (checks_enable)
            perform_transfer_checks();
        if (coverage_enable)
            perform_transfer_coverage();
        item_collected_port.write(trans_collected);
  endfunction : write

endclass : $@ClassName@$

`endif /* $@FILENAME@$ */
