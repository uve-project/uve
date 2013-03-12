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
 * memory slave sequencer implementation
 *
 *******************************************************************************
 */
`ifndef MEMORY_SLAVE_SEQUENCER_SV
`define MEMORY_SLAVE_SEQUENCER_SV

function memory_slave_sequencer::new (string name, uvm_component parent);
  super.new(name, parent);
  `uvm_update_sequence_lib_and_item(memory_transfer)
//-- guo -- add<
  item_collected_eport = new("item_collected_eport", this);
//-- guo -- add>
endfunction : new

// assign vi
function void memory_slave_sequencer::assign_vi(memory_if_wrapper inst);
   this.intf = inst.intf;
endfunction : assign_vi

//-- guo -- add<
// get transactions from monitor
function void memory_slave_sequencer::write(memory_transfer trans);
  transfer_action(trans);
  item_collected = trans;
endfunction : write

// act on transfer
function void memory_slave_sequencer::transfer_action(input memory_transfer trans);

  string msg;

    $sformat(
      msg,
      {"  data  : 0x%h\n",
       "  cle   : %b\n",
       "  ale   : %b\n",
       "  rnw   : %b"},
      trans.data,
      trans.cle,
      trans.ale,
      trans.rnw
    );
    uvm_report_info(
      .id(get_type_name()),
      .message({"\nSequencer analysis transaction reception\n", msg}),
      .verbosity(UVM_DEBUG),
      .filename(`__FILE__),
      .line(`__LINE__)
      );

endfunction : transfer_action

//function memory_trans memory_slave_sequencer::get_item_collected();
//  return item_collected;
//endfunction : get_item_collected

//-- guo -- add>

`endif // MEMORY_SLAVE_SEQUENCER_SV


