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
/*******************************************************************************
 *
 * Author:      $Author:$
 * File:        $File:$
 * Revision:    $Revision:$
 * Date:        $Date:$
 *
 *******************************************************************************
 *
 * memory slave monitor implementation
 *
 *******************************************************************************
 */
`ifndef MEMORY_SLAVE_MONITOR_SV
`define MEMORY_SLAVE_MONITOR_SV

function memory_slave_monitor::new(string name, uvm_component parent=null);
  super.new(name, parent);
  trans_collected = new();
  item_collected_port = new("item_collected_port",this);
endfunction:new

// build
function void memory_slave_monitor::build_phase(uvm_phase phase);
   super.build_phase(phase);
endfunction : build_phase  

// assign vi
function void memory_slave_monitor::assign_vi(memory_if_wrapper inst);
   this.intf = inst.intf;
endfunction : assign_vi


task memory_slave_monitor::monitor_transactions();

  //-- guo -- add<
  forever begin
    if (detect_transfer()) begin

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

    // Advance clock
    @(intf.cb);
  end
  //-- guo -- add>
   
endtask // monitor_transactions
   
task memory_slave_monitor::run_phase(uvm_phase phase);
  fork
    monitor_transactions();
  join_none
endtask : run_phase

//-- guo -- add<
function int memory_slave_monitor::detect_transfer();

	if (  intf.CLE  == 1 | 
	      intf.ALE  == 1 |
	      intf.RE_n == 0 | 
	      intf.WE_n == 0 ) 
	begin
      uvm_report_info(
	    get_type_name(),
		"Flash memory transfer request detected",
		UVM_DEBUG
		);
	  return 1;
	end;

    uvm_report_info(
      get_type_name(),
      "Flash memory _NO_ transfer request detected",
      UVM_DEBUG
      );
	
	return 0;

endfunction : detect_transfer
//-- guo -- add>


task memory_slave_monitor::collect_transfer();

  string msg;
  
  // indicate start of transaction
  void'(this.begin_tr(trans_collected));
  
  if (intf.cb.CLE == 1) begin
    // command latch
    msg = "command";
  end else if (intf.cb.ALE == 1) begin
    // address latch
    msg = "address";
  end else begin
    //data latch
    msg = "data";
  end

  // wait on WE_n to be low
  if (intf.cb.WE_n == 1) begin
  	@(intf.cb.WE_n);
  end
  // wait on rising edge WE_n
  if (intf.cb.WE_n == 0) begin
  	@(intf.cb.WE_n);
  end
  
  // collect data for transfer
  trans_collected.data  = intf.IO0;   // data
  trans_collected.cle   = intf.CLE;   // command
  trans_collected.ale   = intf.ALE;   // address
  trans_collected.rnw   = !intf.RE_n; // read not write
  trans_collected.rnb   = !intf.RB_n; // ready not busy

  $sformat( 
    msg,
    {"\nCollected ", msg, " transfer with\n", 
     "  data: 0x%h\n",
     "  CLE : %b\n",
     "  ALE : %b\n",
     "  REn : %b\n",
     "  RBn : %b"},
     trans_collected.data,
     trans_collected.cle,
     trans_collected.ale,
     !trans_collected.rnw,
     !trans_collected.rnb);
  uvm_report_info( get_type_name(), msg, UVM_DEBUG );
  
  // indicate end of transaction
  this.end_tr(trans_collected);
  
endtask // collect_transfer


// perform_transfer_checks
function void memory_slave_monitor::perform_transfer_checks();

 // USER: do some checks on the transfer here

endfunction : perform_transfer_checks

// perform_transfer_coverage
function void memory_slave_monitor::perform_transfer_coverage();

 // USER: coverage implementation

 -> memory_slave_cov_transaction;

endfunction : perform_transfer_coverage

`endif // MEMORY_SLAVE_MONITOR_SV

