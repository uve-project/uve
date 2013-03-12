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
 * Transaction for memory
 *
 *******************************************************************************
 */

`ifndef MEMORY_TRANSFER_SV
`define MEMORY_TRANSFER_SV

//------------------------------------------------------------------------------
//
// transfer enums, parameters, and events
//
//------------------------------------------------------------------------------
typedef enum {MEMORY_BOGUS_VAL } memory_trans_kind;


//------------------------------------------------------------------------------
//
// CLASS: memory_transfer
//
//------------------------------------------------------------------------------

class memory_transfer extends uvm_sequence_item;
  // USER: Add transaction fields

//guo add<
//  parameter int unsigned page_size_p = 4224; // 4224 for 4k Flash; 2112 for 2k flash
//  parameter int unsigned addr_cycles_p = 4; // 4 for currently implemented flash; 5 for real flash
//  parameter int unsigned addr_bits_p = 4; // 27 for currently implemented flash; 31 for real flash
  parameter int unsigned NCHIP = 3; //should be placed in config database?

  rand logic[(NCHIP*8)-1:0] data; // data
  rand logic                cle;  // command
  rand logic                ale;  // address
  rand logic                rnw;  // read not write
  rand logic                rnb;  // ready not busy
//guo add>

  rand memory_trans_kind      trans_kind;
  string                         master = "";
  string                         slave = "";

   // USER: Add constraint blocks

  //guo add<
//  constraint data_c { data >= 0; data <= 32'hFF; }
//  constraint addr_c { addr >= 0; addr <= 31'hFF; }
  //guo add>

  `uvm_object_utils_begin(memory_transfer)
    `uvm_field_enum     (memory_trans_kind, trans_kind, UVM_ALL_ON)
    `uvm_field_string   (master,                           UVM_ALL_ON|UVM_NOCOMPARE)
    `uvm_field_string   (slave,                            UVM_ALL_ON|UVM_NOCOMPARE)
    // USER: Register fields here

    //guo add<
    `uvm_field_int      (data,                              UVM_ALL_ON)
    `uvm_field_int      (cle,                              UVM_ALL_ON)
    `uvm_field_int      (ale,                              UVM_ALL_ON)
    `uvm_field_int      (rnw,                               UVM_ALL_ON)
    //guo add>

  `uvm_object_utils_end

  // new - constructor
  function new (string name = "memory_transfer_inst");
    super.new(name);
  endfunction : new

  // This function is used by the uvm_in_order_class_comparator in this
  // template's scoreboard
  // USER : you may want to remove this if not using this comparator
  function bit comp(uvm_object rhs);
      string exp_str, act_str, l_trans;
      memory_transfer to;

      uvm_report_info("", "memory_transfer::comp()");
      if (!$cast(to,rhs)) `uvm_error("DUT",$psprintf("comp():rhs is not a memory_transfer!"))

      comp = compare(rhs);
      exp_str = $psprintf("Expected: %s",convert2string());
      act_str = $psprintf("Actual: %s",to.convert2string());

      `uvm_info("FIXME",$sformatf("-------------------"),UVM_MEDIUM)
      `uvm_info("FIXME",$sformatf("%s",exp_str),UVM_MEDIUM)
      `uvm_info("FIXME",$sformatf("-------------------"),UVM_MEDIUM)
      `uvm_info("FIXME",$sformatf("%s:",act_str),UVM_MEDIUM)
      `uvm_info("FIXME",$sformatf("-------------------"),UVM_MEDIUM)
      if (comp)
  `uvm_info("FIXME",$sformatf("Transaction Match!"),UVM_MEDIUM)
      else
  `uvm_error("DUT",$psprintf("Transaction MisMatch!"))

   endfunction: comp


endclass : memory_transfer

`endif // MEMORY_TRANSFER_SV

