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
 * memory master agent sequence library 
 *
 *******************************************************************************
 */
`ifndef MEMORY_MASTER_SEQ_LIB_SV
`define MEMORY_MASTER_SEQ_LIB_SV

//------------------------------------------------------------------------------
// SEQUENCE: default
//------------------------------------------------------------------------------
typedef class memory_transfer;
typedef class memory_master_sequencer;

class example_memory_master_seq extends uvm_sequence #(memory_transfer);

    function new(string name=""); 
      super.new(name);
    endfunction : new


  `uvm_sequence_utils(example_memory_master_seq, memory_master_sequencer)    
  
    memory_transfer this_transfer;

    virtual task body();
      p_sequencer.uvm_report_info(get_type_name(),"Starting example sequence");
       `uvm_do(this_transfer) 
	
      p_sequencer.uvm_report_info(get_type_name(),$psprintf("Done example sequence: %s",this_transfer.convert2string()));
 
    endtask
  
endclass : example_memory_master_seq

// USER: Add your sequences here

`endif // memory_MASTER_SEQ_LIB_SV

