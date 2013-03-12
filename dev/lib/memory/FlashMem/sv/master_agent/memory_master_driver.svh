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
 * memory master driver header file
 *
 *******************************************************************************
 */
`ifndef MEMORY_MASTER_DRIVER_SVH
`define MEMORY_MASTER_DRIVER_SVH

class memory_master_driver extends uvm_driver #(memory_transfer);

  //////////////////////////////////////////////////////////////////////////////
  //
  //  Public interface (Component users may manipulate these fields/methods)
  //
  //////////////////////////////////////////////////////////////////////////////

  // USER: Add your fields here

  // This macro performs UVM object creation, type control manipulation, and 
  // factory registration
  `uvm_component_utils_begin(memory_master_driver)
     // USER: Register fields here
  `uvm_component_utils_end

  // new - constructor
  extern function new (string name, uvm_component parent);

  // uvm run phase
  extern virtual task run_phase(uvm_phase phase);

  // This method assigns the virtual interfaces to the this component
  extern function void assign_vi(memory_if_wrapper inst);

  //////////////////////////////////////////////////////////////////////////////
  //
  //  Implementation (private) interface
  //
  //////////////////////////////////////////////////////////////////////////////

  // The virtual interface used to drive and view HDL signals.
  virtual memory_if intf;

  // This is the method that is responsible for getting sequence transactions
  // and driving the transaction into the DUT
  extern virtual protected task get_and_drive();
 
  // This method drives a sequence trasnaction onto the interface
  extern virtual protected task drive_transfer(memory_transfer trans);
 
  // This method that is responsible for sending an idle cycle to the DUT
  extern protected task send_idle();

  // This method drives the DUT into reset 
  extern protected task reset_signals();
 
  
endclass : memory_master_driver

`endif // MEMORY_MASTER_DRIVER_SVH
