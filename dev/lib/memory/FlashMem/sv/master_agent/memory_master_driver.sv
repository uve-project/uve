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
 * memory master driver implementation
 *
 *******************************************************************************
 */
`ifndef MEMORY_MASTER_DRIVER_SV
`define MEMORY_MASTER_DRIVER_SV

function memory_master_driver::new (string name, uvm_component parent);
  super.new(name, parent);
endfunction : new

task memory_master_driver::run_phase(uvm_phase phase);
   fork
     get_and_drive();
     reset_signals();
   join_none
endtask : run_phase

// assign vi
function void memory_master_driver::assign_vi(memory_if_wrapper inst);
   this.intf = inst.intf;
endfunction : assign_vi

task memory_master_driver::get_and_drive();
    uvm_sequence_item item;
    memory_transfer t;

    forever begin
      
      seq_item_port.get_next_item(item);

      // debug
      uvm_report_info(get_type_name(), "sequencer got next item");

      $cast(t, item);
      drive_transfer(t);
      seq_item_port.item_done();

      // debug
      uvm_report_info(get_type_name(), "sequencer item_done_triggered");
      // Advance clock
      send_idle();    
    end
   
endtask : get_and_drive

task memory_master_driver::drive_transfer (memory_transfer trans);
  uvm_report_info(get_type_name(), "drive_transfer");
  // USER: Add implementation  
endtask : drive_transfer

task memory_master_driver::send_idle();
  uvm_report_info(get_type_name(), "send_idle ...");
  // USER: Add implementation
  @(intf.cb);
endtask:send_idle

task memory_master_driver::reset_signals();
  uvm_report_info(get_type_name(), "reset_signals ...");
  // USER: Add implementation
endtask : reset_signals

`endif // MEMORY_MASTER_DRIVER_SV
