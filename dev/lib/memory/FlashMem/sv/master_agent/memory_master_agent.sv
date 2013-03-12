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
 * memory master agent implementation
 *
 *******************************************************************************
 */
`ifndef MEMORY_MASTER_AGENT_SV
`define MEMORY_MASTER_AGENT_SV

function memory_master_agent::new(string name, uvm_component parent);
  super.new(name, parent);
endfunction : new


function void memory_master_agent::build_phase(uvm_phase phase);
  super.build_phase(phase);
  monitor = memory_master_monitor::type_id::create("monitor",this);
  if(is_active == UVM_ACTIVE) begin
    sequencer = memory_master_sequencer::type_id::create("sequencer",this);
    driver = memory_master_driver::type_id::create("driver",this);
  end
endfunction : build_phase

function void memory_master_agent::connect_phase(uvm_phase phase);
  if(is_active == UVM_ACTIVE) begin
    driver.seq_item_port.connect(sequencer.seq_item_export);       
  end

endfunction : connect_phase
  
function void memory_master_agent::assign_vi(memory_if_wrapper inst);
   monitor.assign_vi(inst);
   if (is_active == UVM_ACTIVE) begin
      sequencer.assign_vi(inst); 
      driver.assign_vi(inst); 
    end
endfunction : assign_vi



`endif // MEMORY_MASTER_AGENT_SV

