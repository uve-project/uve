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
 * memory master monitor header file
 *
 *******************************************************************************
 */
`ifndef MEMORY_MASTER_MONITOR_SVH
`define MEMORY_MASTER_MONITOR_SVH

class memory_master_monitor extends uvm_monitor;

  //////////////////////////////////////////////////////////////////////////////
  //
  //  Public interface (Component users may manipulate these fields/methods)
  //
  //////////////////////////////////////////////////////////////////////////////

  // This field controls if this monitor has its checkers enabled
  // (by default checkers are on)
  bit checks_enable = 1;

  // This field controls if this monitor has its coverage enabled
  // (by default coverage is on)
  bit coverage_enable = 1;

  // USER: Add your fields here


  // The following is the analysis port that allows this monitor's transaction
  // information to be sent to other verification componets such as
  // scoreboards
  uvm_analysis_port #(memory_transfer) item_collected_port;

  // This macro performs UVM object creation, type control manipulation, and 
  // factory registration
  `uvm_component_utils_begin(memory_master_monitor)
     `uvm_field_int(checks_enable, UVM_ALL_ON)
     `uvm_field_int(coverage_enable, UVM_ALL_ON)
     // USER: Register fields here
  `uvm_component_utils_end

   // new - constructor     
   extern function new(string name, uvm_component parent=null);

   // uvm build phase
   extern function void build_phase(uvm_phase phase);

   // uvm run phase
   extern virtual task run_phase(uvm_phase phase);

  // Events needed to trigger covergroups
  event memory_master_cov_transaction;

  // Transfer collected covergroup
  covergroup memory_master_cov_trans @memory_master_cov_transaction;
    // USER implemented coverpoints
  endgroup : memory_master_cov_trans

  // This method assigns the virtual interfaces to the this component
  extern function void assign_vi(memory_if_wrapper inst);

  //////////////////////////////////////////////////////////////////////////////
  //
  //  Implementation (private) interface
  //
  //////////////////////////////////////////////////////////////////////////////

  // This property is the virtual interface needed for this component to drive
  // and view HDL signals
  virtual memory_if intf;

  //This is the transaction being collected by this monitor	
  protected memory_transfer trans_collected;

  // This method is responsible for collecting transactions, checking,
  // and updating coverage 
  extern virtual protected task monitor_transactions();

  // This is the methods that collects transactions
  extern virtual protected task collect_transfer();

  // This is the method that performs checks on a transaction
  extern protected function void perform_transfer_checks();

  // This is the method that updates coverage based on a transaction
  extern protected function void perform_transfer_coverage();

endclass : memory_master_monitor

`endif // MEMORY_MASTER_MONITOR_SVH

