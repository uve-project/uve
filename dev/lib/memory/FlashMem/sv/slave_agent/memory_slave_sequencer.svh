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
 * memory slave sequencer header file
 *
 *******************************************************************************
 */
`ifndef MEMORY_SLAVE_SEQUENCER_SVH
`define MEMORY_SLAVE_SEQUENCER_SVH

class memory_slave_sequencer extends uvm_sequencer #(memory_transfer);

  // Provide implementations of virtual methods such as get_type_name and create
  `uvm_sequencer_utils_begin(memory_slave_sequencer)
     // USER: Register fields 
  `uvm_sequencer_utils_end

  // new - constructor
  extern function new (string name, uvm_component parent);

  // This method assigns the virtual interfaces to the this component
  extern function void assign_vi(memory_if_wrapper inst);
  
//-- guo -- add<
  uvm_analysis_imp #(memory_transfer, memory_slave_sequencer) item_collected_eport;
  memory_transfer item_collected;
//  extern virtual function memory_transfer get_item_collected();
  extern virtual function void write(memory_transfer);
  extern protected function void transfer_action(memory_transfer);
//-- guo -- add>

  //////////////////////////////////////////////////////////////////////////////
  //
  //  Implementation (private) interface
  //
  //////////////////////////////////////////////////////////////////////////////

  // The virtual interface used to drive and view HDL signals.
  virtual memory_if intf;

endclass : memory_slave_sequencer

`endif // MEMORY_SLAVE_SEQUENCER_SVH


