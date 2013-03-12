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
 * memory slave agent sequence library
 *
 *******************************************************************************
 */
`ifndef MEMORY_SLAVE_SEQ_LIB_SV
`define MEMORY_SLAVE_SEQ_LIB_SV

//------------------------------------------------------------------------------
// SEQUENCE: example
//------------------------------------------------------------------------------
class example_memory_slave_seq extends uvm_sequence #(memory_transfer);

  function new(string name="");
    super.new(name);
  endfunction : new

  `uvm_sequence_utils(example_memory_slave_seq, memory_slave_sequencer)

  memory_transfer this_transfer;

  virtual task body();
    p_sequencer.uvm_report_info(get_type_name(),"Starting example sequence");
    `uvm_do(this_transfer)
    p_sequencer.uvm_report_info(get_type_name(),$psprintf("Done example sequence: %s",this_transfer.convert2string()));
  endtask

endclass : example_memory_slave_seq

//guo add<
class objection_memory_slave_seq extends uvm_sequence #(memory_transfer);

  function new(string name="");
    super.new(name);
  endfunction : new

  task pre_body();
    //raise objection if started as root sequence
    uvm_test_done.raise_objection(this);
    uvm_report_info(
      .id(get_type_name()),
      .message({"Sequence activated raise of objection: ", this.get_name()}),
      .filename(`__FILE__),
      .line(`__LINE__)
      );
  endtask

  task post_body();
    //drop objection if started as a root sequence
    uvm_test_done.drop_objection(this);
    uvm_report_info(
      .id(get_type_name()),
      .message({"Sequence activated drop of objection: ", this.get_name()}),
      .filename(`__FILE__),
      .line(`__LINE__)
      );
  endtask

endclass : objection_memory_slave_seq

class default_memory_slave_seq extends objection_memory_slave_seq;

  function new(string name="");
    super.new(name);
  endfunction : new

  `uvm_sequence_utils(default_memory_slave_seq, memory_slave_sequencer)

  memory_transfer this_transfer;

  virtual task body();
    p_sequencer.uvm_report_info(get_type_name(),"Starting default memory slave sequence");
    forever begin
//      this_transfer = p_sequencer.get_item_collected();
      p_sequencer.uvm_report_info(get_type_name(),"Blocking peek waiting in default memory sequence");
      `uvm_do_with(
        req,
        {
//          req.data  == this_transfer.data;
//          req.rnb   == '1;
        }
      );
    end
    p_sequencer.uvm_report_info(get_type_name(),$psprintf("Done default memory slave sequence: %s",this_transfer.convert2string()));
  endtask

endclass : default_memory_slave_seq
//guo add>

`endif // memory_SLAVE_SEQ_LIB_SV

