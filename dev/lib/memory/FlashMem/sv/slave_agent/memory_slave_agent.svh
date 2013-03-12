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
 * memory slave agent header file
 *
 *******************************************************************************
 */
`ifndef MEMORY_SLAVE_AGENT_SVH
`define MEMORY_SLAVE_AGENT_SVH

class memory_slave_agent extends uvm_agent;

  //////////////////////////////////////////////////////////////////////////////
  //
  //  Public interface (Component users may manipulate these fields/methods)
  //
  //////////////////////////////////////////////////////////////////////////////

  // This property controls whether this agent is active or passive
  // An active agent 'UVM_ACTIVE' drives stimulis and monitors the DUT's pins
  // A passive agent 'UVM_PASSIVE' only monitors the DUT's pins  
  protected uvm_active_passive_enum  is_active = UVM_ACTIVE;

  // The following are the verification components that make up
  // this agent
  memory_slave_driver driver;
  memory_slave_sequencer sequencer;
  memory_slave_monitor monitor;

  // USER: Add your fields here

  // This macro performs UVM object creation, type control manipulation, and 
  // factory registration
  `uvm_component_utils_begin(memory_slave_agent)
    `uvm_field_enum(uvm_active_passive_enum, is_active, UVM_ALL_ON)
    // USER: Register your fields here
  `uvm_component_utils_end

  // new - constructor
  extern function new (string name, uvm_component parent);

  // uvm build phase
  extern function void build_phase(uvm_phase phase);
  
  // uvm connection phase
  extern function void connect_phase(uvm_phase phase);

  // This method assigns the virtual interfaces to the agent's children
  extern function void assign_vi(memory_if_wrapper inst);

  //////////////////////////////////////////////////////////////////////////////
  //
  //  Implementation (private) interface
  //
  //////////////////////////////////////////////////////////////////////////////


endclass : memory_slave_agent

`endif // MEMORY_SLAVE_AGENT_SVH

