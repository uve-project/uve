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
 * memory master monitor implementation
 *
 *******************************************************************************
 */
`ifndef MEMORY_MASTER_MONITOR_SV
`define MEMORY_MASTER_MONITOR_SV

function memory_master_monitor::new(string name, uvm_component parent=null);
  super.new(name, parent);
  item_collected_port = new("item_collected_port",this);
  trans_collected = new();
endfunction:new

// build
function void memory_master_monitor::build_phase(uvm_phase phase);
   super.build_phase(phase);
endfunction : build_phase  

// assign vi
function void memory_master_monitor::assign_vi(memory_if_wrapper inst);
   this.intf = inst.intf;
endfunction : assign_vi
    
task memory_master_monitor::monitor_transactions();

   forever begin
 
      // Extract data from interface into transaction
      collect_transfer();

      // Check transaction
      if (checks_enable)
 	 perform_transfer_checks();

      // Update coverage
      if (coverage_enable)
 	 perform_transfer_coverage();

      // Publish to subscribers
      item_collected_port.write(trans_collected);

   end
endtask // monitor_transactions
   

task memory_master_monitor::run_phase(uvm_phase phase);
  fork
    monitor_transactions();
  join_none
endtask // run
  
  
task memory_master_monitor::collect_transfer();

  void'(this.begin_tr(trans_collected));
  // USER: Extract data and fill ata in memory_transfer trans_collected

  // Advance clock
  @(intf.cb);

  // Wait for some start event..., indicate start of transaction
  void'(this.begin_tr(trans_collected));


  // Wait for some start event..., indicate end of transaction
  this.end_tr(trans_collected);


endtask // collect_transfer_start_phase


// perform_transfer_checks
function void memory_master_monitor::perform_transfer_checks();

 // USER: do some checks on the transfer here

endfunction : perform_transfer_checks

// perform_transfer_coverage
function void memory_master_monitor::perform_transfer_coverage();

 // USER: coverage implementation
  -> memory_master_cov_transaction;	

endfunction : perform_transfer_coverage

`endif // MEMORY_MASTER_MONITOR_SV

