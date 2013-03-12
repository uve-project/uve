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
 * memory interface env implementation
 *
 *******************************************************************************
 */
`ifndef MEMORY_ENV_SV
`define MEMORY_ENV_SV

//------------------------------------------------------------------------------
//
// CLASS: memory_env implementation
//
//------------------------------------------------------------------------------

function memory_env::new(string name, uvm_component parent);
  super.new(name, parent);
endfunction : new
  
function void memory_env::build_phase(uvm_phase phase);
  string inst_name;
  super.build_phase(phase);
  
  if (has_bus_monitor) begin
    bus_monitor = memory_bus_monitor::type_id::create("bus_monitor",this);
  end
  masters = new[num_masters];
  for(int i = 0; i < num_masters; i++) begin
   $sformat(inst_name, "masters[%0d]", i); 
    masters[i] = memory_master_agent::type_id::create(inst_name,this);
  end
    
  slaves = new[num_slaves];
  for(int i = 0; i < num_slaves; i++) begin
    $sformat(inst_name, "slaves[%0d]", i); 
    slaves[i] = memory_slave_agent::type_id::create(inst_name,this);
  end
endfunction : build_phase

task memory_env::run_phase(uvm_phase phase);
    fork
      update_vif_enables();
    join
endtask : run_phase


task memory_env::update_vif_enables();
  forever begin
    @(intf_checks_enable || intf_coverage_enable);
    memory_if_wrapper_inst.intf.has_checks = intf_checks_enable; 
    memory_if_wrapper_inst.intf.has_coverage = intf_coverage_enable; 
  end
endtask : update_vif_enables

function void memory_env::assign_vi();

  if (has_bus_monitor) begin
    bus_monitor.assign_vi(memory_if_wrapper_inst);
  end

  for(int i = 0; i < num_masters; i++) begin
    masters[i].assign_vi(memory_if_wrapper_inst);
  end
    
  for(int i = 0; i < num_slaves; i++) begin
    slaves[i].assign_vi(memory_if_wrapper_inst);
  end
endfunction : assign_vi

`endif // MEMORY_ENV_SV
