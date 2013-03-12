/*******************************************************************************
 * -----------------------------------------------------------------------------
 * Copyright 2009 Paradigm Works, Inc., Andover MA
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *------------------------------------------------------------------------------
*/
/***************************************************************************
 *
 * Author:      $Author:$
 * File:        $File:$
 * Revision:    $Revision:$
 * Date:        $Date:$
 *
 *******************************************************************************
 *
 * memory env header file
 *
 *******************************************************************************
 */
`ifndef MEMORY_ENV_SVH
`define MEMORY_ENV_SVH

//------------------------------------------------------------------------------
//
// CLASS: memory_env
//
//------------------------------------------------------------------------------

   
class memory_env extends uvm_env;

  //////////////////////////////////////////////////////////////////////////////
  //
  //  Public interface (Component users may manipulate these fields/methods)
  //
  //////////////////////////////////////////////////////////////////////////////

  // The following two bits are used to control whether checks and coverage are
  // done both in the bus monitor class and the interface.
  bit intf_checks_enable = 1; 
  bit intf_coverage_enable = 1;

  // This field controls if this env includes a bus monitor
  bit has_bus_monitor = 0;

  // This field controls how many master agents are instantiated in this env 	
  int unsigned num_masters = 0;

  // This field controls how many slave agents are instantiated in this env 	
  int unsigned num_slaves = 1;

  // This is the bus monitor for this env
  memory_bus_monitor bus_monitor;

  // This is an array of the master agents in this env
  memory_master_agent masters[];

  // This is an array of the slave agents in this env
  memory_slave_agent slaves[];

  // This is a wrapper that contains the SystemVerilog virtual interface for this vip
  // Note that this object wrapper is sent down to all appopriate compoents in this VIP
  memory_if_wrapper memory_if_wrapper_inst;

  // USER: Add your fields here

  // This macro performs UVM object creation, type control manipulation, and 
  // factory registration
  `uvm_component_utils_begin(memory_env)
    `uvm_field_int(has_bus_monitor, UVM_ALL_ON)
    `uvm_field_int(num_masters, UVM_ALL_ON)
    `uvm_field_int(num_slaves, UVM_ALL_ON)
    `uvm_field_int(intf_checks_enable, UVM_ALL_ON)
    `uvm_field_int(intf_coverage_enable, UVM_ALL_ON)
    `uvm_field_object(memory_if_wrapper_inst, UVM_ALL_ON)
     // USER: Register fields your here
  `uvm_component_utils_end

  // new - constructor
  extern function new(string name, uvm_component parent);
  
  // uvm build phase
  extern function void build_phase(uvm_phase phase);

  // uvm run phase
  extern task run_phase(uvm_phase phase);

  // This method assigns the virtual interfaces to the agents and bus monitor
  extern function void assign_vi();

  //////////////////////////////////////////////////////////////////////////////
  //
  //  Implementation (private) interface
  //
  //////////////////////////////////////////////////////////////////////////////

  extern protected task update_vif_enables();

endclass : memory_env
   
   
`endif // MEMORY_ENV_SVH
