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
 * memory master sequencer implementation
 *
 *******************************************************************************
 */
`ifndef MEMORY_MASTER_SEQUENCER_SV
`define MEMORY_MASTER_SEQUENCER_SV

function memory_master_sequencer::new (string name, uvm_component parent);
  super.new(name, parent);
  `uvm_update_sequence_lib_and_item(memory_transfer)
endfunction : new

// assign vi
function void memory_master_sequencer::assign_vi(memory_if_wrapper inst);
   this.intf = inst.intf;
endfunction : assign_vi

`endif // MEMORY_MASTER_SEQUENCER_SV


