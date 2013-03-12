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
 * memory slave driver implementation
 *
 *******************************************************************************
 */
`ifndef MEMORY_SLAVE_DRIVER_SV
`define MEMORY_SLAVE_DRIVER_SV

function memory_slave_driver::new (string name, uvm_component parent);
  super.new(name, parent);
endfunction : new

task memory_slave_driver::run_phase(uvm_phase phase);
   fork
     get_and_drive();
     reset_signals();
   join_none
endtask : run_phase

// assign vi
function void memory_slave_driver::assign_vi(memory_if_wrapper inst);
   this.intf = inst.intf;
endfunction : assign_vi

//-- guo -- add<
function int memory_slave_driver::detect_transfer();

  if (
//        intf.CLE  == 1 |
//        intf.ALE  == 1 |
       intf.RE_n == 0
//        intf.WE_n == 0
      )begin

    uvm_report_info(
      get_type_name(),
      "Flash memory transfer request detected",
      UVM_DEBUG );
    return 1;

  end

  uvm_report_info(
    get_type_name(),
    "_NO_ Flash memory transfer request detected",
    UVM_DEBUG );

  return 0;

endfunction : detect_transfer
//-- guo -- add>

task memory_slave_driver::get_and_drive();
    uvm_sequence_item item;
    memory_transfer t;

    forever begin

      if ( detect_transfer() ) begin
        // if transfer requested on bus

        if ( seq_item_port.has_do_available() ) begin
          // transfer available

          seq_item_port.get_next_item(item);

          uvm_report_info(
            get_type_name(),
            "sequencer got next item",
            UVM_DEBUG );

          $cast(t, item);
          drive_transfer(t);
          seq_item_port.item_done();

          uvm_report_info(
            get_type_name(),
            "sequencer item_done_triggered",
            UVM_DEBUG );

        end

        uvm_report_info(
          get_type_name(),
          "sequencer item_done_triggered",
          UVM_DEBUG );

      end

      // Advance clock
      send_idle();
    end

endtask : get_and_drive

task memory_slave_driver::drive_transfer (memory_transfer trans);
  //-- guo -- add<
  string msg;
  //-- guo -- add>

  uvm_report_info(get_type_name(), "drive_transfer");
  // USER: Add implementation
  //-- guo -- add<

  // sync to clock
  //   only needed on first access, further accesses are synced implicitly
  if (!synced) begin
    @(intf.cb);
  end

//  if (trans.rnw == 1) begin
  // read
    // drive data
    intf.IO0_drive  = trans.data;
    $sformat(
    	msg,
    	"access with data = 0x%h",
    	trans.data
    	);
    uvm_report_info(get_type_name(), {"Memory read ", msg});
//  end else begin
  // write
    // get data -- not needed, done in monitor
    //trans.data = intf.IO0;
//  end
  // and busy
  intf.RB_n = !trans.rnb;

  //-- guo -- add>

endtask : drive_transfer

task memory_slave_driver::send_idle();
  uvm_report_info(
    get_type_name(),
    "send_idle ...",
    UVM_DEBUG
  );
  @(intf.cb);
endtask:send_idle

task memory_slave_driver::reset_signals();
  uvm_report_info(get_type_name(), "reset_signals ...");
  // flash outputs
  intf.RB_n   = '0; // not (ready not busy) = busy not ready
endtask : reset_signals

`endif // MEMORY_SLAVE_DRIVER_SV
